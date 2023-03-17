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

// Includes Estandard
#include <string>
#include <list>
#include <vector>

// Includes suri
#include "SpatialSubsetSelectionPart.h"
#include "SubsetCoordinateInputPart.h"
#include "suri/DataViewManager.h"
#include "resources.h"
#include "suri/HtmlTreeWidget.h"
#include "suri/LayerHtmlTreeNodeHandler.h"
#include "suri/LayerTreeModel.h"
#include "IndependentTreeSelectionManager.h"
#include "suri/TransformationFactoryBuilder.h"
#include "wxmacros.h"
#include "HtmlListSelectionWidget.h"
#include "LayerHtmlListItem.h"

// Includes wx
#include "wx/choicebk.h"
#include "wx/listbox.h"

// defines
#define LAYEREXTENTPOSITION 0

namespace suri {


/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(SpatialSubsetSelectionPartEvent, SpatialSubsetSelectionPart)
   IMPLEMENT_EVENT_CALLBACK(OnUpdateSubsetButton,
                            OnUpdateSubsetButton(event), wxCommandEvent)
#ifdef __WXMSW__
   IMPLEMENT_EVENT_CALLBACK(OnSize, OnSize(event); event.Skip(), wxSizeEvent)
#endif
   IMPLEMENT_EVENT_CALLBACK(OnRefreshSubsetButtonClick,
                            OnRefreshSubsetButtonClick(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */


/** Ctor */
SpatialSubsetSelectionPart::SpatialSubsetSelectionPart(
      DataViewManager *pDataViewManager, World* pInputWorld) :
      pTreeSelection_(NULL), NEW_EVENT_OBJECT(SpatialSubsetSelectionPartEvent) {
   pDataViewManager_ = pDataViewManager;
   pInputWorld_ = pInputWorld;
   windowTitle_ = _(caption_SPATIAL_SELECTION);
}

/** Dtor */
SpatialSubsetSelectionPart::~SpatialSubsetSelectionPart() {
   delete pEventHandler_;
}

/** Metodo auxiliar que carga la capa de capas **/
void SpatialSubsetSelectionPart::LoadLayerList() {
   ViewcontextInterface* pviewcontext =
         pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();
   DatasourceManagerInterface* pdsmgr = pDataViewManager_->GetDatasourceManager();
   std::vector<SuriObject::UuidType> ids = pdsmgr->GetOrderedIds();
   LayerInterface* player = NULL;
   ui::HtmlListItemInterface* pitem = NULL;
   for (std::vector<SuriObject::UuidType>::iterator it = ids.begin(); it != ids.end();
         ++it) {
      player = NULL;
      DatasourceInterface* pdatasource = pdsmgr->GetDatasource(*it);
      if (pdatasource) {
          player = pviewcontext->GetAssociatedLayer(
               pdatasource->GetId());
         if (player) {
             pitem = new ui::LayerHtmlListItem(player);
            pTreeSelection_->AppendItem(pitem);
         }
         if (player->GetElement()){
            pitem = GetItemSelection(player->GetElement());
            if (pitem)
               pTreeSelection_->AppendItem(pitem);
         }
      }
   }
}
/**
 * Crea la ventana de la parte
 * @return bool que indica si tuvo exito
 */
bool SpatialSubsetSelectionPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(
         pParentWindow_, wxT("ID_SPATIAL_SELECTION_PANEL"));

   Subset defaultsubset;
   pInputWorld_->GetWindow(defaultsubset);
   pSubsetInputPart_ = new UniversalSubsetInputPart(pDataViewManager_,
         pDataViewManager_->GetLibraryManager(), pInputWorld_, defaultsubset,
         GetProcessLayer());
   pSubsetInputPart_->SetWorkingSpatialReference(pInputWorld_->GetSpatialReference());
   AddControl(pSubsetInputPart_->GetWidget(), "ID_SUBSET_SELECTION_PANEL");

   pTreeSelection_ = new ui::HtmlListSelectionWidget(
         !ui::HtmlListSelectionWidget::Checkeable
               & !ui::HtmlListSelectionWidget::MultipleSelection);
   pTreeSelection_->Attach(this);
   LoadLayerList();
   AddControl(pTreeSelection_, wxT("ID_LAYER_EXTENT_PANEL"));
   SetInitialValues();
#ifdef __WXMSW__
   /**
    * El algoritmo de layout de wx falla en windows si existe una jerarquia de
    * ventanas muy profunda(empeora en 64 bits). Es un problema reconocido en
    * wx, ver TCK #2572 para una descripcion mas completa.
    * En sopi esto producia que no se actualizaran los viewers al hacer un resize
    * de la ventana. En OnSize se ejecuta codigo que parchea el bug de
    * windows/wx
    */
   if (wxPlatformInfo::Get().GetArchitecture() == wxARCH_64) {
      XRCCTRL(*pParentWindow_, wxT("ID_SPATIAL_SELECTION_PANEL"), wxPanel)->Connect(
         wxEVT_SIZE, wxSizeEventHandler(SpatialSubsetSelectionPartEvent::OnSize),
         NULL, pEventHandler_);
   }
#endif


   XRCCTRL(*pParentWindow_, wxT("ID_VIEWPORTSELECTION_LISTBOX"),
         wxListBox)->Connect(wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler(
           SpatialSubsetSelectionPartEvent::OnUpdateSubsetButton), NULL, pEventHandler_);

   XRCCTRL(*pParentWindow_, wxT("ID_BTN_UPDATE_SUBSET"),
           wxButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED , wxCommandEventHandler(
           SpatialSubsetSelectionPartEvent::OnRefreshSubsetButtonClick), NULL, pEventHandler_);

   return true;
}

/**
 * Devuelve el estado del Part.
 * @return bool que indica si tuvo exito
 */
bool SpatialSubsetSelectionPart::IsEnabled() {
   return enabled_;
}

/**
 * Indica si el Part tiene cambios para salvar.
 * @return bool que indica si tuvo exito
 */
bool SpatialSubsetSelectionPart::HasChanged() {
   return pSubsetInputPart_->HasChanged();
}

/** Salva los cambios realizados en el Part. */
bool SpatialSubsetSelectionPart::CommitChanges() {
   bool returnvalue = pSubsetInputPart_->CommitChanges();
   if (pPal_ && returnvalue) {
      Subset subsetin;
      pSubsetInputPart_->GetSubset(subsetin);
      pPal_->AddSerializableAttribute<Subset>(
            ProcessAdaptLayer::InputSubsetWindowKeyAttr, subsetin);
      pPal_->AddSerializableAttribute<Subset>(
            ProcessAdaptLayer::OutputSubsetWindowKeyAttr, subsetin);
   }
   return returnvalue;
}

/**
 * Restaura los valores originales del Part.
 * @return bool que indica si tuvo exito
 */
bool SpatialSubsetSelectionPart::RollbackChanges() {
   if (pSubsetInputPart_->RollbackChanges()) {
      Subset subsetin;
      pSubsetInputPart_->GetSubset(subsetin);
      pPal_->AddSerializableAttribute<Subset>(
            ProcessAdaptLayer::InputSubsetWindowKeyAttr,  subsetin);
      pPal_->AddSerializableAttribute<Subset>(
            ProcessAdaptLayer::OutputSubsetWindowKeyAttr, subsetin);
      return true;
   }
   return false;
}

/** Inicializa el part */
void SpatialSubsetSelectionPart::SetInitialValues() {
   pSubsetInputPart_->SetInitialValues();
   Subset subsetin;
   pSubsetInputPart_->GetSubset(subsetin);
   pPal_->AddSerializableAttribute<Subset>(ProcessAdaptLayer::InputSubsetWindowKeyAttr, subsetin);
   pPal_->AddSerializableAttribute<Subset>(ProcessAdaptLayer::OutputSubsetWindowKeyAttr, subsetin);
   // Se configuran visualizadore activos.
   ViewportManagerInterface* pviewportmanager = pDataViewManager_->GetViewportManager();
   std::vector<SuriObject::UuidType> ids = pviewportmanager->GetViewportIds();
   std::vector<SuriObject::UuidType>::iterator it = ids.begin();
   viewportListIndex_.clear();
   GET_CONTROL(*pToolWindow_, wxT("ID_VIEWPORTSELECTION_LISTBOX"),
                                                   wxListBox)->Clear();
   int index = 0;
   for (; it != ids.end(); ++it) {
      ViewerWidget* pviewer = AS_VW(pDataViewManager_->
                                    GetViewportManager()->GetViewport(*it));
      if (pviewportmanager->IsViewportActive(*it) && pviewer->GetWorld() &&
                                        pviewer->GetWorld()->IsInitialized()) {
         wxString text;
         text.Printf(_(caption_VIEWER_d), index + 1);

         GET_CONTROL(*pToolWindow_, wxT("ID_VIEWPORTSELECTION_LISTBOX"),
                                                wxListBox)->Append(text);
         viewportListIndex_.push_back(pviewer->GetId());
      }
      index++;
   }
   pToolWindow_->Refresh();
   pToolWindow_->Update();
}

/** Actualiza el estado de la parte */
void SpatialSubsetSelectionPart::Update() {
   pSubsetInputPart_->Update();
}

/**
 * Retorna el icono de la herramienta
 * @param[in] X ancho del icono
 * @param[in] Y alto del icono
 * @param[out] ToolBitmap icono de la herramienta.
 */
void SpatialSubsetSelectionPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_SPATIAL_SELECTION, ToolBitmap);
}

/**
 * Retorna el subset que ingreso el usuario en srs de trabajo,
 * @param[out] OutputSubset subset que ingreso el usuario
 * @return bool que informa si es valida
 */
bool SpatialSubsetSelectionPart::GetSubset(Subset& OutputSubset) {
   return pSubsetInputPart_->GetSubset(OutputSubset);
}

/** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
void SpatialSubsetSelectionPart::Update(NotificationInterface* pNotification) {
   pNotification->ApplyNotification(this);
}

/**
 * Configura el subset en el part a partir del extent de fuente de datos
 * @param[in] DatasouceId id de la fuente de datos
 */
void SpatialSubsetSelectionPart::UpdateSubsetWithDatasource(
                                       SuriObject::UuidType DatasouceId) {
   DatasourceInterface* pdatasource = pDataViewManager_->
                   GetDatasourceManager()->GetDatasource(DatasouceId);
    if (!pdatasource || !pdatasource->GetElement()) {
       if(!pSelection_.empty()) {
                pdatasource = GetVectorDatasourceFromLayer(DatasouceId);
       } else {
          return;
       }
     }
    Subset datasourceextent;
    std::string subsetsrs = pdatasource->GetElement()->GetSpatialReference().c_str();
    pdatasource->GetElement()->GetElementExtent(datasourceextent);
    SetNewSubset(datasourceextent, subsetsrs);
}

/** Configura el subset en el part a partir del extent a traves de una capa */
void SpatialSubsetSelectionPart::UpdateSubsetWithLayer(SuriObject::UuidType LayerId) {
   DatasourceInterface* pdatasource =
         pDataViewManager_->GetViewcontextManager()->GetAssociatedDatasource(LayerId);
   if (!pdatasource || !pdatasource->GetElement()) {
      if(!pSelection_.empty()) {
           pdatasource = GetVectorDatasourceFromLayer(LayerId);
           if (!pdatasource) return ;
      } else {
           return;
      }
   }
   Subset datasourceextent;
   std::string subsetsrs = pdatasource->GetElement()->GetSpatialReference().c_str();
   pdatasource->GetElement()->GetElementExtent(datasourceextent);
   SetNewSubset(datasourceextent, subsetsrs);
}

/**
 * Configura el subset en el part
 * @param[in] NewSubset subset que se mostrara por pantalla en srs de trabajo
 * @param[in] SubsetSrs sistema de referencia en que esta el subset
 */
void SpatialSubsetSelectionPart::SetNewSubset(const Subset& NewSubset,
                                          const std::string &SubsetSrs) {
   Subset newsubset = NewSubset;
   std::string workingsrs = pSubsetInputPart_->GetWorkingSpatialReference();
   // Transformo entre sistema de referencia de subset y el del raster
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn, SubsetSrs);
   params.AddValue<std::string>(TransformationFactory::kParamWktOut, workingsrs);
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   CoordinatesTransformation *pcoordtransform = pfactory->Create(
                                 TransformationFactory::kExact, params);
   TransformationFactoryBuilder::Release(pfactory);

   if (!pcoordtransform || pcoordtransform->Transform(newsubset) <= 0) {
      SHOW_ERROR("Sistema de referencia incompatible");
      delete pcoordtransform;
      return;
   }
   delete pcoordtransform;
   pSubsetInputPart_->SetSubset(newsubset);
}

// ----------------------------- Eventos -----------------------------
/** Evento que se ejecuta al hacer click sobre boton actualizar subset */
void SpatialSubsetSelectionPart::OnUpdateSubsetButton(wxCommandEvent &Event) {
   int selecteditem = Event.GetInt();
   if (selecteditem >= static_cast<int>(viewportListIndex_.size()))
      return;
   ViewerWidget* pviewer = AS_VW(pDataViewManager_->GetViewportManager()->
                                 GetViewport(viewportListIndex_[selecteditem]));
   World* pworld = pviewer ? pviewer->GetWorld() : NULL;
   if (!pworld)
      return;

   Subset extent;
   pworld->GetWindow(extent);
   SetNewSubset(extent, pworld->GetSpatialReference());
   return;
}

void SpatialSubsetSelectionPart::OnRefreshSubsetButtonClick(wxCommandEvent &Event) {

   int choicemade = USE_CONTROL(*pToolWindow_, wxT("ID_SELECTION_CHOICEBOOK"), wxChoicebook,
                                GetSelection(), -1);

   if (choicemade == 0) {
      std::set<SuriObject::UuidType> ids = pTreeSelection_->GetSelectedItems();
      DatasourceInterface *pdatasource =
            pDataViewManager_->GetViewcontextManager()->GetAssociatedDatasource(
                  *ids.begin());
      if (!pdatasource)
         return;

      Subset subset;
      pdatasource->GetElement()->GetElementExtent(subset);

      SetNewSubset(subset, pdatasource->GetElement()->GetSpatialReference().c_str());
      OnUpdateSubsetButton(Event);
   }
}
#ifdef __WXMSW__
/** evento OnSize */
void SpatialSubsetSelectionPart::OnSize(wxSizeEvent& Event) {
   pParentWindow_->Layout();
   FixSizers(pParentWindow_,"");
}
#endif

} /* namespace suri */
