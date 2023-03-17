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
#include <vector>
#include <string>
#include <list>
#include <set>

// Includes Suri
#include "ClassSelectionPart.h"
#include "suri/VectorElement.h"
#include "FiltredElementListWidget.h"
#include "suri/LayerList.h"
#include "resources.h"
#include "suri/HtmlTreeWidget.h"
#include "suri/LayerHtmlTreeNodeHandler.h"
#include "suri/LayerTreeModel.h"
#include "suri/DataViewManager.h"
#include "EnclosureNodeFilter.h"
#include "suri/VectorStyleTable.h"
#include "suri/VectorStyleManager.h"
#include "suri/ClassStatisticsProcess.h"
#include "HtmlListSelectionWidget.h"
#include "EnclosureValidator.h"
#include "LayerHtmlListItem.h"
#include "ClassStatisticsHtmlListItem.h"
#include "EnclosureManipulator.h"
#include "EnclosureInformation.h"

// Includes Wx
// Includes App
// Defines
#define SELECT_ALL_BTN_LABEL "Seleccionar todo"
#define CLEAR_SELECT_BTN_LABEL "Deseleccionar todo"
#define IMPORT_CLASS_BTN_LABEL "Importar clase"
#define message_CLASS_IMPORT_ERROR "No se pudieron importar clases desde el archivo indicado"

/** Define la decoracion de GetTypeAsString para poder utilizarla como un tipo */
#define TYPE_DECORATOR(Type) wxString::Format(_(message_VECTOR_LAYER_OF_s), _(Type) )

// Forwards

/** namespace suri */
namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ClassSelectionPartEventHandler, ClassSelectionPart)
   IMPLEMENT_EVENT_CALLBACK(OnImportButton, OnImportButton(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnSelectAllClick, OnSelectAllClick(event), wxCommandEvent)   
   IMPLEMENT_EVENT_CALLBACK(OnClearSelectionClick, OnClearSelectionClick(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/**
 * Ctor
 * @param[in] pElements lista de elementos
 */
ClassSelectionPart::ClassSelectionPart(DataViewManager* pDataViewManager, int LayoutFlags) :
      // pTreeNodeHandler_(NULL), pTreeWidget_(NULL), pGuiTreeModel_(NULL),
      pTreeWidget_(NULL),
      pDataViewManager_(pDataViewManager), pEventHandler_(new ClassSelectionPartEventHandler(this)),
      layoutflags_(LayoutFlags) {
   windowTitle_ = _(caption_CLASS_SELECTION_PART);
}

/** Dtor */
ClassSelectionPart::~ClassSelectionPart() {
   delete pEventHandler_;
}

/** Metodo que inicializa la lista html que contiene las clases **/
void ClassSelectionPart::InitializeTreeWidget() {
   ViewcontextInterface* pviewcontext =
         pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();

   DatasourceManagerInterface* pdsmgr = pDataViewManager_->GetDatasourceManager();

   std::vector<SuriObject::UuidType> ids = pdsmgr->GetOrderedIds();
   EnclosureValidator validator;

   for (std::vector<SuriObject::UuidType>::iterator it = ids.begin(); it != ids.end();
         ++it) {
      DatasourceInterface* pdatasource = pdsmgr->GetDatasource(*it);
      if (validator.IsValid(pdatasource)) {
         EnclosureManipulator manipulator;
         EnclosureInformation info = manipulator.GetEnclosureInformation(pdatasource);
         LayerInterface* player = pviewcontext->GetAssociatedLayer(
               pdatasource->GetId());
         bool trainfilter = layoutflags_ & TrainClassFilter;
         bool testfilter = layoutflags_ & TestClassFilter;
         if (player &&
               ((trainfilter && info.GetEnclosureType() == EnclosureInformation::Train) ||
               (testfilter && info.GetEnclosureType() == EnclosureInformation::Test))) {
            ui::HtmlListItemInterface* pitem = new ui::LayerHtmlListItem(player);
            pTreeWidget_->AppendItem(pitem);
         }
      }
   }
}

/**
 *  Agrega un item al final de la lista
 *  @param[in] pItem item a insertar. Se guarda la referencia, no se hace una copia.
 */
void ClassSelectionPart::AppendItem(ui::HtmlListItemInterface* pitem) {
   pTreeWidget_->AppendItem(pitem);
}

/**
 * Crea la ventana de la parte
 * @return bool que indica si se pudo crear la ventana
 */
bool ClassSelectionPart::CreateToolWindow() {
   pToolWindow_ = new wxPanel(pParentWindow_, wxID_ANY);
   wxBoxSizer *psizer = new wxBoxSizer(wxVERTICAL);
   pToolWindow_->SetSizer(psizer);
   wxBoxSizer* pauxsizer = new wxBoxSizer(wxHORIZONTAL);
   if (layoutflags_ & ImportClassSelectionButtonFlag) {
      wxButton* pimportbutton = new wxButton(pToolWindow_, wxID_ANY, wxT(_(IMPORT_CLASS_BTN_LABEL)));
      pimportbutton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, 
                     wxCommandEventHandler(ClassSelectionPartEventHandler::OnImportButton),
            NULL, pEventHandler_);
      pauxsizer->Add(pimportbutton, wxSizerFlags().Right());
   }
   if (layoutflags_ & SelectionButtonFlag) {
      wxButton* pbutton = new wxButton(pToolWindow_, wxID_ANY, wxT(_(SELECT_ALL_BTN_LABEL)));
      pbutton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, 
                  wxCommandEventHandler(ClassSelectionPartEventHandler::OnSelectAllClick),
            NULL, pEventHandler_);
      pauxsizer->Add(pbutton, wxSizerFlags().Right());
   }
   if (layoutflags_ & ClearSelectionButtunFlag) {
   wxButton* pclearbutton = new wxButton(pToolWindow_, wxID_ANY, wxT(_(CLEAR_SELECT_BTN_LABEL)));
   pclearbutton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, 
                  wxCommandEventHandler(ClassSelectionPartEventHandler::OnClearSelectionClick),
         NULL, pEventHandler_);
   pauxsizer->Add(pclearbutton, wxSizerFlags().Right());
   }
   psizer->Add(pauxsizer);
   pTreeWidget_ = new ui::HtmlListSelectionWidget();
   InitializeTreeWidget();
   AddControl(pTreeWidget_, pToolWindow_);
   return true;
}

/**
 * Devuelve el estado del Part.
 * @return enabled_ estado de la propiedad enabled_
 */
bool ClassSelectionPart::IsEnabled() {
   return enabled_;
}

/**
 * Indica si el Part tiene cambios para salvar.
 * @return returnvalue true / false si hay cambios
 */
bool ClassSelectionPart::HasChanged() {
   if (!pTreeWidget_)
      return false;
   std::set<SuriObject::UuidType> activeleafs = pTreeWidget_->GetSelectedItems();
   return activeleafs != activeLeafs_;
}

/**
 * Salva los cambios realizados en el Part.
 * @return true siempre
 */
bool ClassSelectionPart::CommitChanges() {
   if (!HasValidData())
      return false;
   std::vector<DatasourceInterface*> selectedclasses;
   activeLeafs_ = pTreeWidget_->GetSelectedItems();
   std::set<SuriObject::UuidType>::iterator it = activeLeafs_.begin();
   ViewcontextInterface* pviewcontext =
         pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();

   for (; it != activeLeafs_.end(); ++it) {
      DatasourceInterface* pdatasource = pviewcontext->GetAssociatedDatasource(*it);
      if (pdatasource) {
         selectedclasses.push_back(pdatasource);
      }
      // TODO (Gabriel): Adaptar a clases importadas tambien
   }

   if (pPal_) {
      // TODO (Gabriel): Adaptar a clases importadas tambien
      pPal_->AddAttribute<std::vector<DatasourceInterface*> >(
            ClassStatisticsProcess::SelectedClassesKeyAttr, selectedclasses);
   }
   return true;
}

/**
 * Restaura los valores originales del Part.
 * @return true siempre
 */
bool ClassSelectionPart::RollbackChanges() {
   return true;
}

/**
 * Inicializa los datos mostrados
 */
void ClassSelectionPart::SetInitialValues() {
   RollbackChanges();
}

/**
 * Devuelve el icono de la parte
 * @param[out] ToolBitmap icono de la herramienta
 * @param[in] X ancho del icono
 * @param[in] Y alto del icono
 */
void ClassSelectionPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_CLASSIFICATION_PARAMETERS, ToolBitmap);
}

/**
 * Informa si el subset seleccionado es valido
 * @return true si el control tiene datos validos.
 * @return false en otro caso
 */
bool ClassSelectionPart::HasValidData() {
   // Esta funcion puede llamarse antes que pElementList_ se inicialice
   return pTreeWidget_ ? !pTreeWidget_->GetSelectedItems().empty() : true;
}

/** Obtiene las clases seleccionadas **/
std::list<ClassificationClass*> ClassSelectionPart::GetSelectedClasses() {
   std::list<ClassificationClass*> classes;
   std::set<SuriObject::UuidType>::iterator it = activeLeafs_.begin();
   ViewcontextInterface* pviewcontext =
         pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();
   for (; it != activeLeafs_.end(); ++it) {
      DatasourceInterface* pdatasource = pviewcontext->GetAssociatedDatasource(*it);
      ClassificationClass* pclass = NULL;
      if (pdatasource) {
         pclass = new ClassificationElementClass(pdatasource->GetElement());
      } else {
         ui::ClassStatisticsHtmlListItem* pitem =
               dynamic_cast<ui::ClassStatisticsHtmlListItem*>(pTreeWidget_->GetItem(*it));
         pclass =
               pitem ? new ClassificationStatisticsClass(
                             new ClassStatisticsFileParser::ClassStatisticsInfo(
                                   pitem->GetClassInfo())) :
                       NULL;
      }

         // something
      classes.push_back(pclass);
   }
   return classes;
}

/**
 * Obtiene la lista de elementos seleccionados del control.
 * @return Lista de elementos seleccionados
 */
std::list<Element*> ClassSelectionPart::GetActiveElements() {
   std::list<Element*> result;
   std::set<SuriObject::UuidType>::iterator it = activeLeafs_.begin();
   ViewcontextInterface* pviewcontext =
         pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();

   for (; it != activeLeafs_.end(); ++it) {
      DatasourceInterface* pdatasource = pviewcontext->GetAssociatedDatasource(*it);
      if (pdatasource) {
         result.push_back(pdatasource->GetElement());
      }
   }
   return result;
}
/** Metodo que genera un mascara a partir de la fuente de datos que se pasa por parametro
 *  Precondicion: pDatasource tiene que ser no nulo
 *  @param[in] pDatasource fuente de datos sobre la cual se quiere generar una mascara
 *  @return VectorElement* nueva instancia de elemento vectorial de tipo mascara**/
VectorElement* ClassSelectionPart::GenerateMaskFromDatasource(
      DatasourceInterface* pDatasource) {
   VectorElement* pvector = VectorElement::Create(pDatasource->GetUrl().c_str());
   if (pvector) {
      // TODO(Javier - TCK #847): Desharcodear el nombre de la tabla de mascaras
      pvector->SetName(pDatasource->GetName().c_str());
      VectorStyleTable* pvectortable = NULL;
      pvectortable = VectorStyleManager::Instance().GetTable("mask", Vector::Polygon);
      // Se utiliza el estilo default de la tabla de mascaras
      std::string style = pvectortable->GetDefaultStyle()->GetWkt();
      pvector->SetStyle(style);
      pvector->Activate();
   }
   return pvector;
}

void ClassSelectionPart::OnSelectAllClick(wxCommandEvent& Event) {
   pTreeWidget_->SelectAll();
}
void ClassSelectionPart::OnClearSelectionClick(wxCommandEvent& Event) {
   pTreeWidget_->ClearSelection();
}

/** Captura el evento de click sobre el boton importar clase */
void ClassSelectionPart::OnImportButton(wxCommandEvent &Event) {
   std::string filename;
   wxFileDialog dlg(NULL, _(caption_CLASSIFICATION_PARAMETERS_IMPORTER), wxT(""),
                    wxT(""),
                    filter_ALL_FILES,
                    wxFD_OPEN | wxFD_FILE_MUST_EXIST);
   if (dlg.ShowModal() != wxID_OK)
      return;
   ClassStatisticsFileParser parser(dlg.GetPath().c_str());
   std::vector<ClassStatisticsFileParser::ClassStatisticsInfo> items = parser.Parse();
   if (items.empty()) {
      SHOW_ERROR(_(message_CLASS_IMPORT_ERROR));
      return;
   }
   for (size_t i = 0; i < items.size(); ++i)
      AppendItem(new ui::ClassStatisticsHtmlListItem(items[i]));
}
} /** namespace suri **/
