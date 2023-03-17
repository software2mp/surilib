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

// Includes Estandar
// Includes Suri
#include "suri/DynamicViewerWidget.h"
#include "suri/Viewer2D.h"
#include "suri/Viewer3D.h"
#include "suri/ViewportEvent.h"
#include "suri/ToolSupport.h"
#include "suri/DataViewManager.h"
#include "PaintEvent.h"
#include "MouseEvent.h"
#include "CommandEvent.h"
#include "DefaultDatasourceManager.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor */
DynamicViewerWidget::DynamicViewerWidget(wxWindow *pParent, DataViewManager* pDataViewManager) :
      ViewerWidget(pParent, pDataViewManager), activeMode_(Raster), demid_("") {
   createWithMiniframe_ = false;
   ViewerWidget* p2dviewer = new Viewer2D(pParentWindow_, pDataViewManager_);
   viewers_[Gis] = p2dviewer;
   viewers_[Raster] = p2dviewer;
   viewers_[Z] = new Viewer3D(pParentWindow_, pDataViewManager_);
}

/** Destructor */
DynamicViewerWidget::~DynamicViewerWidget() {
}

/** Creacion de la ventana */
bool DynamicViewerWidget::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_, wxT("ID_DYNAMIC_WIDGET_PANEL"));
   if (!pToolWindow_)
      return false;
	ViewerWidget* p2dviewer = viewers_[Raster];
	ViewerWidget* p3dviewer = viewers_[Z];
   AddControl(p2dviewer, "ID_2D_VIEWER_PANEL");
   AddControl(p3dviewer, "ID_3D_VIEWER_PANEL");
   XRCCTRL(*pToolWindow_, wxT("ID_3D_VIEWER_PANEL"), wxPanel)->Show(activeMode_ == Z);

   return true;
}

/** Resetea el estado del visualizador */
void DynamicViewerWidget::ResetViewer() {
   viewers_[activeMode_]->ResetViewer();
}

/** Hace ZoomIn */
void DynamicViewerWidget::ZoomIn(int X, int Y, int Factor, int Z) {
   viewers_[activeMode_]->ZoomIn(X, Y, Factor);
}
/** Hace ZoomOut */
void DynamicViewerWidget::ZoomOut(int X, int Y, int Factor, int Z) {
   viewers_[activeMode_]->ZoomOut(X, Y, Factor);
}

/** Cambia la ventana del mundo */
void DynamicViewerWidget::ChangeWindow(const Subset &NewWindow, bool PreventUpdate) {
   viewers_[activeMode_]->ChangeWindow(NewWindow, PreventUpdate);
}
/** Setea el WorldExtentManager */
void DynamicViewerWidget::SetWorldExtentManager(WorldExtentManager* pExtentManager) {
   viewers_[activeMode_]->SetWorldExtentManager(pExtentManager);
}

/** Retorna el WorldExtentManager */
WorldExtentManager* DynamicViewerWidget::GetWorldExtentManager() {
   return viewers_[activeMode_]->GetWorldExtentManager();
}

/** Metodo de actualizacion de la lista, el mundo y/o los elementos. */
void DynamicViewerWidget::UpdateViewers(Model *pModel) {
   viewers_[activeMode_]->UpdateViewers(pModel);
}

/** Verifica el estado de la renderizacion. */
void DynamicViewerWidget::CheckRenderizationStatus() {
   viewers_[activeMode_]->CheckRenderizationStatus();
}

/** Obtiene la transformacion asociada **/
CoordinatesTransformation* DynamicViewerWidget::GetTransformation() {
   return viewers_[activeMode_]->GetTransformation();
}

/** Obtiene el modo de visualizacion configurado. **/
DynamicViewerWidget::VisualizationMode DynamicViewerWidget::GetVisualizationMode() const {
   return activeMode_;
}

/** Configura el modo de visualizacion**/
bool DynamicViewerWidget::ChangeVisualizationMode(VisualizationMode Mode) {
   if (viewers_.find(Mode) == viewers_.end())
      return false;
   ViewerWidget* pviewer = viewers_[Mode];
   if (Mode != Z) {
      pDataViewManager_->GetDatasourceManager()->UnblockAllDatasource();
      LayerList* player = ToolSupport::GetLayerList(pDataViewManager_->GetViewcontextManager());
      Viewer2D* p2d = dynamic_cast<Viewer2D*>(pviewer);
      p2d->SetMode(Mode == Gis ? Viewer2D::Gis : Viewer2D::Raster);
      activeMode_ = Mode;
      if (Mode == Raster) {
         std::vector<Element*> elements = player->GetRenderizationOrderList();
         std::vector<Element*>::const_iterator it = elements.begin();
         // Borro los elementos asociados si cambio a modo raster
         for (; it != elements.end(); ++it) {
            if ( (*it)->HasAssociatedElement() ) {
               delete (*it)->GetAssociatedElement();
               (*it)->SetAssociatedElement(NULL);
            }
         }
      }

   } else {
      GetList()->SetActivationLogic(NULL);
      Viewer3D* p3d = dynamic_cast<Viewer3D*>(pviewer);
      LayerList* player = ToolSupport::GetLayerList(pDataViewManager_->GetViewcontextManager());
      Element* pelement = player->GetElement(GetDemId());
      p3d->SetTerrain(dynamic_cast<RasterElement*>(pelement));
      activeMode_ = Mode;
      /**
       * Si se bloquea la fuente de datos no se pueden activar otros rasters en el V3D
       * (ver DefaultViewcontext::CanDisplayLayer)
       * DatasourceInterface* psource =
       * pDataViewManager_->GetDatasourceManager()->GetDatasourceByElementId(
       * pelement->GetUid());
       * pDataViewManager_->GetDatasourceManager()->BlockDatasource(psource->GetId());
       */
   }

   XRCCTRL(*pToolWindow_, wxT("ID_3D_VIEWER_PANEL"), wxPanel)->Show(Mode == Z);
   XRCCTRL(*pToolWindow_, wxT("ID_2D_VIEWER_PANEL"), wxPanel)->Show(Mode != Z);
   activeMode_ = Mode;
   pToolWindow_->Refresh();
   pToolWindow_->Update();
   pToolWindow_->Layout();
   return true;
}

/** Retorna el id del modelo de elevacion */
suri::Element::UidType DynamicViewerWidget::GetDemId() const {
   return demid_;
}

/**
 * Setea el id del modelo de elevacion
 * @param[in] Dem modelo de elevacion seleccionado en las propiedades del visualizador 3D
 * @return true si hay un modelo de elevacion seleccionado
 * @return false si el modelo de elevacion no fue seleccionado
 */
bool DynamicViewerWidget::ChangeDem(suri::Element::UidType Dem) {
   if (Dem.empty())
      return false;
   DefaultDatasourceManager* pDatasourceManager = dynamic_cast<DefaultDatasourceManager*>
                                                         (pDataViewManager_->GetDatasourceManager());
   DatasourceInterface * pDatasource = NULL;
   if (pDatasourceManager){
        pDatasource = pDatasourceManager->GetDatasourceByElementId(Dem);
   }
   if (pDatasource) pDatasource->Block();
   if (!demid_.empty()) {
      pDatasource = pDatasourceManager->GetDatasourceByElementId(demid_);
      if (pDatasource)
         pDatasource->Unblock();
   }
   demid_ = Dem;
   return true;
}

/** Permite obtener la ventana con las herramientas del viewer */
wxWindow *DynamicViewerWidget::DoGetViewerTools(wxWindow *pParent) {
   return viewers_[activeMode_]->DoGetViewerTools(pParent);
}
/** Despliega un elemento (puede ser funcion de RenderizationControl) */
void DynamicViewerWidget::DisplayElement(Element *pElement) {
   viewers_[activeMode_]->DisplayElement(pElement);
}
/** Oculta un elemento (puede ser funcion de RenderizationControl) */
void DynamicViewerWidget::HideElement(Element *pElement) {
   viewers_[activeMode_]->HideElement(pElement);
}
/** El elemento ha sido modificado */
void DynamicViewerWidget::UpdateElement(Element *pElement) {
   viewers_[activeMode_]->UpdateElement(pElement);
}
/** Se le setea la lista de elementos */
void DynamicViewerWidget::DoSetElementList(LayerList *pList) {
   viewers_[activeMode_]->SetElementList(pList);
}

/** Metodo polimorfico que se llama cuando se hace foco sobre un Widget **/
void DynamicViewerWidget::DoOnFocus(bool Focused) {
	viewers_[activeMode_]->Focus(Focused);
}

SuriObject::UuidType DynamicViewerWidget::GetId() const {
   std::map<VisualizationMode, ViewerWidget*>::const_iterator it = viewers_.find(activeMode_);
	return it->second->GetId();
}

SuriObject::UuidIntType DynamicViewerWidget::GetIntId() const {
   std::map<VisualizationMode, ViewerWidget*>::const_iterator it = viewers_.find(activeMode_);
   return it->second->GetIntId();
}

/** Obtiene el mundo */
World *DynamicViewerWidget::GetWorld() const {
   std::map<VisualizationMode, ViewerWidget*>::const_iterator it = viewers_.find(activeMode_);
   return it->second->GetWorld();
}

/**
 * Retorna el visualizador especificado por parametro
 * @param[in] Mode modo de visualizacion (Raster/SIG = 2D, Z = 3D)
 * @return puntero visualizador especificado
 */
ViewerWidget* DynamicViewerWidget::GetViewer(VisualizationMode Mode) {
   return (Mode != Z) ? viewers_[Raster] : viewers_[Z];
}

/**
 * Retorna el visualizador activo
 * @return puntero al visualizador activo
 */
ViewerWidget* DynamicViewerWidget::GetViewer() {
   return viewers_[activeMode_];
}

/**
 * Establece el titulo de la ventana.
 * @param[in] WindowTitle Titulo de la ventana.
 */
void DynamicViewerWidget::SetWindowTitle(const wxString& WindowTitle) {
   viewers_[activeMode_]->SetWindowTitle(WindowTitle);
}

/**
 * Obtiene la lista
 * @return puntero a LayerList con la lista de elementos.
 */
LayerList* DynamicViewerWidget::GetList() const {
   std::map<VisualizationMode, ViewerWidget*>::const_iterator it = viewers_.find(activeMode_);
   LayerList* pList = it->second->GetList();
   return pList ? pList : NULL;
}

/**
 * Marca en el miniframe el foco del widget.
 * \todo: Conectar (opcionalmente) eventos de on-focus para control automatico.
 * @param[in] Focused Estado de foco del miniframe.
 */
void DynamicViewerWidget::Focus(bool Focused) {
   std::map<VisualizationMode, ViewerWidget*>::const_iterator it = viewers_.find(Raster);
   it->second->Focus(Focused);
   it = viewers_.find(Z);
   it->second->Focus(Focused);
}

/**
 * Agrega un handler para eventos de tipo mouse
 * @param[in] pMouseEvent Puntero a una instancia de handler de eventos de mouse
 */
void DynamicViewerWidget::PushMouseEvent(MouseEventInterface *pMouseEvent) {
   std::map<VisualizationMode, ViewerWidget*>::const_iterator it = viewers_.find(activeMode_);
   it->second->PushMouseEvent(pMouseEvent);
}

/**
 * Agrega un handler para eventos de tipo paint
 * @param[in] pPaintEvent Puntero a una instancia de handler de eventos de paint
 */
void DynamicViewerWidget::PushPaintEvent(PaintEventInterface *pPaintEvent, bool DeepSearch) {
   std::map<VisualizationMode, ViewerWidget*>::const_iterator it = viewers_.find(activeMode_);
   it->second->PushPaintEvent(pPaintEvent, DeepSearch);
}

/**
 * Agrega un handler para eventos de tipo command
 * @param[in] pCommandEvent Puntero a una instancia de handler de eventos de paint
 */
void DynamicViewerWidget::PushCommandEvent(CommandEventInterface *pCommandEvent) {
   std::map<VisualizationMode, ViewerWidget*>::const_iterator it = viewers_.find(activeMode_);
   it->second->PushCommandEvent(pCommandEvent);
}

/**
 * Elimina un handler para eventos de tipo mouse
 * @param[in] pMouseEvent Puntero a una instancia de handler de eventos de mouse
 */
MouseEventInterface *DynamicViewerWidget::PopMouseEvent(MouseEventInterface *pMouseEvent) {
   std::map<VisualizationMode, ViewerWidget*>::const_iterator it = viewers_.find(activeMode_);
   return it->second->PopMouseEvent(pMouseEvent);
}

/**
 * Elimina un handler para eventos de tipo paint
 * @param[in] pPaintEvent Puntero a una instancia de handler de eventos de paint
 */
PaintEventInterface *DynamicViewerWidget::PopPaintEvent(PaintEventInterface *pPaintEvent) {
   std::map<VisualizationMode, ViewerWidget*>::const_iterator it = viewers_.find(activeMode_);
   return it->second->PopPaintEvent(pPaintEvent);
}

/**
 * Elimina un handler para eventos de tipo command
 * @param[in] pCommandEvent Puntero a una instancia de handler de eventos de paint
 */
CommandEventInterface *DynamicViewerWidget::PopCommandEvent(CommandEventInterface *pCommandEvent) {
   std::map<VisualizationMode, ViewerWidget*>::const_iterator it = viewers_.find(activeMode_);
   return it->second->PopCommandEvent(pCommandEvent);
}

/** Desactiva los elementos del visualizador activo */
void DynamicViewerWidget::DeactivateViewerElements() {
   Viewer3D* v3d = dynamic_cast<Viewer3D*>(viewers_[activeMode_]);
   if (v3d)
      v3d->SetTerrain(NULL);
   LayerList* playerlist = GetList();
   bool ignorehidden = false;
   int count = playerlist->GetElementCount(ignorehidden);
   for (int i = 0; i < count; ++i) {
      Element* pelement = playerlist->GetElement(i, ignorehidden);
      if (pelement && pelement->IsActive())
         pelement->Activate(false);
   }
}
}  /** namespace suri **/
