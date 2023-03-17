/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

// Includes standard
#include <set>
#include <string>
#include <sstream>
// Includes Suri
#include "MaskSelectionPart.h"
#include "suri/HtmlTreeWidget.h"
#include "suri/ViewcontextTreeSelectionManager.h"
#include "suri/LayerTreeModel.h"
#include "suri/LayerHtmlTreeNodeHandler.h"
#include "resources.h"
#include "suri/messages.h"
#include "suri/VectorElement.h"
#include "suri/SuriObject.h"
#include "suri/TreeNodeInterface.h"
#include "suri/VectorStyleTable.h"
#include "suri/VectorStyleManager.h"
#include "VectorLayer.h"
#include "GeometryNodeFilter.h"
#include "suri/AuxiliaryFunctions.h"
#include "HtmlListSelectionWidget.h"
#include "LayerHtmlListItem.h"
#include "suri/Vector.h"
#include "MemoryVector.h"
#include "HotLinkVectorLayer.h"
#include "suri/MemoryVectorElement.h"
#include "Filter.h"
#include "FiltredVectorRenderer.h"

// Includes Wx
#include "wx/xrc/xmlres.h"
#include "wx/wx.h"
// Includes App


// Constantes
#define MEMORY_VECTOR_NAME             "shpmemory:Seleccion.shp"
#define MEMORY_VECTOR_LAYER_NAME       "MEMORY_LAYER_NAME"
#define MEMORY_VECTOR_REF_FIELD_NAME   "ID"
// Defines

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(MaskPartEventHandler, MaskSelectionPart)
      IMPLEMENT_EVENT_CALLBACK(OnInternalRadioClick, OnInternalRadioClick(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnExternalRadioClick, OnExternalRadioClick(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnChoiceEvent, OnChoiceEvent(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** Constructor
 *  @param[in] pDataViewManager administrador general de la aplicacion
 *  @param[in] pInputelement elemento de entrada al proceso
 */
MaskSelectionPart::MaskSelectionPart(DataViewManager* pDataViewManager,
                                     Element* pInputElement) :
      NEW_EVENT_OBJECT(MaskPartEventHandler),
      pLayerList_(NULL),
      pTreeWidget_(NULL),
      pDataViewManager_(pDataViewManager), pInputElement_(pInputElement),
      guiInternalMask_(true), internalMask_(true), noDataValueDefined_(false), maskvalue_(0) {
   windowTitle_ = _(caption_MASK_SELECTION_PART);
   pMemoryVectorEditor_ = NULL;
}

/** Dtor */
MaskSelectionPart::~MaskSelectionPart() {
   DELETE_EVENT_OBJECT;
}

/** Indica si el Part tiene cambios para salvar. */
bool MaskSelectionPart::HasChanged() {
   if (guiInternalMask_ != internalMask_)
      return true;
   std::set<SuriObject::UuidType> pidset = pTreeWidget_->GetSelectedItems();
   // TODO(Gabriel - TCK #3588): Asi no se comparan los sets.
   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_MASK_VALUE_TEXT"), wxTextCtrl);
   std::string value = ptext->GetValue().c_str();
   return checkedSet_ != pidset || (maskvalue_ != StringToNumber<int>(value));
}

/** Salva los cambios realizados en el Part. */
bool MaskSelectionPart::CommitChanges() {
   if (pLayerList_)
      delete pLayerList_;
   pLayerList_ = new LayerList;

   std::set<SuriObject::UuidType> activeleafs = pTreeWidget_->GetSelectedItems();
   std::set<SuriObject::UuidType>::iterator it = activeleafs.begin();
   for (; it != activeleafs.end(); ++it) {
      DatasourceInterface* pdatasource =
            pDataViewManager_->GetViewcontextManager()->GetAssociatedDatasource(*it);
      VectorElement* pvector = NULL;
      if (pdatasource) {
         pvector = GenerateMaskFromDatasource(pdatasource);
      } else if (!pSelection_.empty()) {
         VectorDatasource* pVectorDatasource = GetVectorDatasourceFromLayer(*it);
         if (pVectorDatasource) {
            pvector = GenerateMaskFromDatasource(pVectorDatasource);
         }
      }
      if (pvector)
         pLayerList_->AddElement(pvector);
   }
   checkedSet_ = activeleafs;
   internalMask_ = guiInternalMask_;

   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_MASK_VALUE_TEXT"), wxTextCtrl);

   double ndvalue = StringToNumber<double>(ptext->GetValue().mb_str());

   maskvalue_ = ndvalue;

   pLayerList_->SetNoDataValue(ndvalue);

   pLayerList_->SetNoDataValueAvailable(true);

   if (pPal_) {
      pPal_->AddAttribute<LayerList*>(ProcessAdaptLayer::MaskListKeyAttr, pLayerList_);
   }
   modified_ = false;
   if(pMemoryVectorEditor_) {
      pMemoryVectorEditor_->CloseLayer();
      pMemoryVectorEditor_->CloseVector();
      delete pMemoryVectorEditor_;
      pMemoryVectorEditor_ = NULL;
   }
   /*if (pSelection_)
      delete pSelection_;*/
   return true;
}

/** Metodo que genera un mascara a partir de la fuente de datos que se pasa por parametro
 *  Precondicion: pDatasource tiene que ser no nulo
 *  @param[in] pDatasource fuente de datos sobre la cual se quiere generar una mascara
 *  @return VectorElement* nueva instancia de elemento vectorial de tipo mascara**/
VectorElement* MaskSelectionPart::GenerateMaskFromDatasource(
      DatasourceInterface* pDatasource) {
   VectorElement* pvector = VectorElement::Create(pDatasource->GetUrl().c_str());
   if (pvector) {
      // TODO(Javier - TCK #847): Desharcodear el nombre de la tabla de mascaras
      VectorStyleTable* pvectortable = NULL;
      if (guiInternalMask_) {
         pvectortable = VectorStyleManager::Instance().GetTable("mask",
                                                                Vector::Polygon);
      } else {
         pvectortable = VectorStyleManager::Instance().GetTable("mask_external",
                                                                Vector::Polygon);
      }
      // Se utiliza el estilo default de la tabla de mascaras
      std::string style = pvectortable->GetDefaultStyle()->GetWkt();
      pvector->SetStyle(style);
      pvector->Activate();
   }
   return pvector;
}

/** Restaura los valores originales del Part. */
bool MaskSelectionPart::RollbackChanges() {
   guiInternalMask_= internalMask_;
   wxRadioButton* pinternal = XRCCTRL(*pToolWindow_, wxT("ID_POLYGON_INTERIOR_RADIO"),
                                      wxRadioButton);
   wxRadioButton* pexternal = XRCCTRL(*pToolWindow_, wxT("ID_POLYGON_EXTERIOR_RADIO"),
                                      wxRadioButton);
   pinternal->SetValue(internalMask_);
   pexternal->SetValue(!internalMask_);
   return true;
}

/** Inicializa el part */
void MaskSelectionPart::SetInitialValues() {
}

/** Devuelve si la parte tiene datos validos */
bool MaskSelectionPart::HasValidData() {
   return true;
}

/** Actualiza el estado de la parte */
void MaskSelectionPart::Update() {
}

/** Retorna el icono de la herramienta */
void MaskSelectionPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_MASK_SELECTION, ToolBitmap);
}

/** Implementacion del metodo de creacion de ventana de Widget */
bool MaskSelectionPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_MASK_SELECTION_PANEL"));
   if (!pToolWindow_)
      return false;

   DatasourceManagerInterface* pdsmgr = pDataViewManager_->GetDatasourceManager();
   std::vector<SuriObject::UuidType> ids = pdsmgr->GetOrderedIds();
   pTreeWidget_ = new ui::HtmlListSelectionWidget(ui::HtmlListSelectionWidget::Checkeable);
   ViewcontextInterface* pviewcontext =
         pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();
   for (std::vector<SuriObject::UuidType>::iterator it = ids.begin();
         it != ids.end(); ++it) {

      LayerInterface* player = pviewcontext->GetAssociatedLayer(*it);
      if (!player)
         return true;
      VectorElement* pelement = dynamic_cast<VectorElement*>(player->GetElement());
      if (!pelement)
         continue;
      std::string type = pelement->GetTypeAsString().c_str();
      std::vector<std::string> selectedFeatures = pelement->GetSelectedFeatures();
      if (type.compare(TYPE_DECORATOR(message_POLYGON).c_str()) == 0) {
         ui::HtmlListItemInterface* pitem = new ui::LayerHtmlListItem(player);
         pTreeWidget_->AppendItem(pitem);
         if (!selectedFeatures.empty()) {
            player = CreateSelectionList(pelement);
            pitem = new ui::LayerHtmlListItem(player);
            pTreeWidget_->AppendItem(pitem);
         }
      }

   }

   AddControl(pTreeWidget_, wxT("ID_LAYER_LIST_PANEL"));

   wxXmlNode* pnode = pInputElement_ ? pInputElement_->GetNode() : NULL;
   if (pnode) {
      wxXmlNode* pnotvaliddatanode = pInputElement_->GetNode(
            FILE_NODE NODE_SEPARATION_TOKEN FORMAT_NODE
            NODE_SEPARATION_TOKEN PROPERTIES_NODE NODE_SEPARATION_TOKEN
            NOT_VALID_DATA_VALUE);
      noDataValueDefined_ = pnotvaliddatanode != NULL;
      if (pnotvaliddatanode)
         noDataValue_ = pnotvaliddatanode->GetNodeContent().c_str();
   }

   ConfigureGui();
   return true;
}



/** Obtiene la lista de mascara resultante de la seleccion en el Part luego de
 *  guardar los cambios
 *  @return lista con los elementos que se utilizaran como mascara
 *  @return NULL en caso de que no se haya seleccionado por lo menos una mascara
 */
LayerList* MaskSelectionPart::GetLastMaskList() {
   return pLayerList_;
}

/** Metodo que se llama cuando se hace click sobre el radio para mascara interna*/
void MaskSelectionPart::OnInternalRadioClick(wxCommandEvent& Event) {
   guiInternalMask_ = true;
}

/** Metodo que se llama cuando se hace click sobre el radio para mascara externa*/
void MaskSelectionPart::OnExternalRadioClick(wxCommandEvent& Event) {
   guiInternalMask_ = false;
}

/** Metodo que se llama cuando se hace click sobre el check*/
void MaskSelectionPart::OnChoiceEvent(wxCommandEvent& Event) {
   wxCheckBox* pcheck = XRCCTRL(*pToolWindow_,
                                wxT("ID_USE_INVALID_FROM_IMAGE_CHECKBOX"), wxCheckBox);
   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_MASK_VALUE_TEXT"), wxTextCtrl);
   if (pcheck && ptext) {
      bool check = pcheck->GetValue();
      ptext->Enable(!check);
      if (check && noDataValueDefined_) {
         ptext->ChangeValue(noDataValue_.c_str());
      }
   }
}

/** Metodo auxiliar que configura los controles graficos del Part*/
void MaskSelectionPart::ConfigureGui() {
   checkedSet_.clear();
   internalMask_ = true;
   wxRadioButton* pinternal = XRCCTRL(*pToolWindow_, wxT("ID_POLYGON_INTERIOR_RADIO"),
                                      wxRadioButton);
   wxRadioButton* pexternal = XRCCTRL(*pToolWindow_, wxT("ID_POLYGON_EXTERIOR_RADIO"),
                                      wxRadioButton);
   if (pinternal) {
      pinternal->SetValue(internalMask_);
      pinternal->Connect(
            wxEVT_COMMAND_RADIOBUTTON_SELECTED,
            wxCommandEventHandler(MaskPartEventHandler::OnInternalRadioClick), NULL,
            pEventHandler_);
   }
   if (pexternal) {
      pexternal->SetValue(!internalMask_);
      pexternal->Connect(
            wxEVT_COMMAND_RADIOBUTTON_SELECTED,
            wxCommandEventHandler(MaskPartEventHandler::OnExternalRadioClick), NULL,
            pEventHandler_);
   }
   wxCheckBox* pcheck = XRCCTRL(*pToolWindow_,
                                wxT("ID_USE_INVALID_FROM_IMAGE_CHECKBOX"), wxCheckBox);
   if (pcheck) {
      pcheck->Enable(this->noDataValueDefined_);
      pcheck->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED,
                      wxCommandEventHandler(MaskPartEventHandler::OnChoiceEvent), NULL,
                      pEventHandler_);
   }
}

} /** namespace suri **/

