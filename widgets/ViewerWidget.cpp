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

// Includes Standard

// Includes suri
#include "suri/messages.h"
#include "wxToggleBitmapButton.h"
#include "suri/Button.h"
#include "suri/CoordinatesTransform.h"
#include "suri/ViewerWidget.h"
#include "suri/ViewcontextInterface.h"
#include "suri/Configuration.h"

// Includes wx
#include "wx/wx.h"
#include "wx/statline.h"
#include "wx/artprov.h"

/** namespace suri */
namespace suri {
ActiveViewerManager *ViewerWidget::pViewerManager_ = NULL;

// -----------------------------------------------------------------------------
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ViewerWidgetEvent, ViewerWidget)
IMPLEMENT_EVENT_CALLBACK(OnViewerToolsUiUpdate, OnViewerToolsUiUpdate(event), wxUpdateUIEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Constructor.
 * @param[in] pParent puntero a una ventana padre.
 * @param[in] pDataViewManager administrador de info de la aplicacion
 * @return instancia de la clase ViewerWidget
 */
ViewerWidget::ViewerWidget(wxWindow *pParent, DataViewManager* pDataViewManager) :
      Widget(pParent),
      pDataViewManager_(pDataViewManager),
      pListViewer_(NULL),
      NEW_EVENT_OBJECT(ViewerWidgetEvent),
      pElementViewer_(NULL),
      pNavigator_(NULL),
      pmeasure_(NULL) {
   windowTitle_ = _(caption_VIEWER);
   pWorld_ = NULL;
   /**
    * todo(gabriel - tck #5694): fix temporal para que no aparezca el titulo en simur.
    * esto debe mejorarse y poder configurarse desde el archivo de configuracion de la aplicacion
    */
   createWithMiniframe_ =
         Configuration::GetParameter("app_short_name", "") != wxT("SoPI") ? false :
                                                                            true;
}

/** dtor */
ViewerWidget::~ViewerWidget() {
   // Desasocio los viewers antes de eliminarlos
   LayerList *playerlist = pListViewer_ ? pListViewer_->GetList() : NULL;
   if (Model::IsValid(playerlist)) {
      LayerList::ElementListType::iterator it = playerlist->GetElementIterator();
      for (; it != playerlist->GetElementIterator(false); ++it) {
         Element *pelement = *it;
         if (Model::IsValid(pelement)) {
            pelement->UnregisterViewer(pElementViewer_);
         }
      }
      playerlist->UnregisterViewer(pListViewer_);
   }
   delete pNavigator_;
   delete pListViewer_;
   delete pElementViewer_;
   // borro el mundo
   delete pWorld_;
   // elimino el helper de eventos
   DELETE_EVENT_OBJECT
   ;
}

/**
 * Creacion de la ventana
 * @return true si pudo crear la ventana.
 * @return false si no pudo crear la ventana.
 */
bool ViewerWidget::CreateToolWindow() {
   // Si no se seteo de antemano el pToolWindow_, crea un panel
   if (!pToolWindow_) {
      pToolWindow_ = new wxPanel(pParentWindow_);
   }
   pToolWindow_->SetName(GetWindowTitle());
   // si es autonoma, adapta para tener la barra de herramientas
   if (isAutonomous_) {
      // tamanio minimo deseado de la ventana del visualizador en modo
      // standalone (isAutonomous_==true)
      wantedWidth_ = 400;
      wantedHeight_ = 400;
      // creo un panel, agrego sizer, la barra, un separador y la ventana scroll
      wxPanel *pbackpanel = new wxPanel(pParentWindow_);
      pbackpanel->SetSizer(new wxBoxSizer(wxVERTICAL));
      pbackpanel->GetSizer()->Add(GetViewerTools(pbackpanel), 0, wxALIGN_LEFT, 0);
      pbackpanel->GetSizer()->Add(new wxStaticLine(pbackpanel), 0, wxEXPAND, 0);
      pToolWindow_->Reparent(pbackpanel);
      pbackpanel->GetSizer()->Add(pToolWindow_, 1, wxEXPAND, 0);
      pToolWindow_ = pbackpanel;
   }
   return true;
}

/**
 * Permite obtener la ventana con las herramientas del viewer
 * @param[in] pParent si no encuentra toolbar configurado llama a
 * DoGetViewerTools y conecta a la herramienta de pParent el evento
 * OnViewerToolsUiUpdate
 * @param[in] pDataViewManager administrador de la aplicacion
 * @return retorna resultado de GetToolbar()
 */
wxWindow *ViewerWidget::GetViewerTools(wxWindow *pParent) {
   if (!GetToolbar() && pParent) {
      wxWindow* ptoolbar = DoGetViewerTools(pParent);
      if (ptoolbar) {
         ptoolbar->Connect(
               wxEVT_UPDATE_UI,
               wxUpdateUIEventHandler(ViewerWidgetEvent::OnViewerToolsUiUpdate), NULL,
               pEventHandler_);
      }
   }
   return GetToolbar();
}

/** Se le setea la lista de elementos */
/**
 * @param[in] pList nueva lista de elementos
 * \todo si la lista tiene elementos y estan activos, estos elementos
 *       son ignorados (comportamiento no deseado)
 */
void ViewerWidget::SetElementList(LayerList *pList) {
   // proteccion contra lista nula o doble inicializacion
   if (!pList || pListViewer_) {
      return;
   }
   // reseteo el viewer
   ResetViewer();
   // Creo el observador de la lista
   pListViewer_ = new ListView<ViewerWidget>(this, &ViewerWidget::ListChanged,
                                             &ViewerWidget::ListDeleted);
   // registro
   pList->RegisterViewer(pListViewer_);
   // creo el  observador de elementos
   pElementViewer_ = new CallbackView<ViewerWidget>(this,
                                                    &ViewerWidget::ElementChanged);
   // \todo si la lista tiene elementos y estan activos se ignora
   DoSetElementList(pList);
   // Reseteo el viewer
   ResetViewer();
}

/**
 * Elimina un boton de herramienta
 * @param[in] pTool boton que se desea eliminar
 * @return retonra true si el boton fue eliminado o no existia en
 * herramienta
 */
bool ViewerWidget::DeleteButtonTool(Button *pTool) {
   if (!pTool) {
      return true;
   }
   return true;
}

/** Actualiza la barra de herramientas */
/**
 *  Esta funcion por defecto desactiva el panel completo. Sobrecargar para una
 * logica mas compleja.
 * @param[in] Event de update de ui
 */
void ViewerWidget::OnViewerToolsUiUpdate(wxUpdateUIEvent &Event) {
   Event.Skip(false);
   LayerList *plist = GetList();
   if (!plist) {
      Event.Enable(false);
   } else {
      /** Desactiva los botones de toolbal principal cuando no hay imagen
       * desplegada */
      /** \todo (ramirogonzalez - #4196): PARCHE, En el futuro mover esta
       * funcionalidad a GuiStateChangeMediator */
      bool enable = pToolWindow_->IsEnabled() && plist->GetActiveCount() != 0;
      std::set<Button*>::iterator it = pViewerToolButtons_.begin();
      for (; it != pViewerToolButtons_.end(); ++it)
         (*it)->SetEnabled(enable);
   }
}

/**
 * Evento de elemento modificado
 * @param[in] pModel elemento modificado
 */
void ViewerWidget::ElementChanged(Model *pModel) {
   LayerList *plist = pListViewer_->GetList();
   if (!plist) {
      return;
   }
   Element *pelement = plist->GetElement(pModel);
   if (!pelement) {
      return;
   }
   if (pelement->IsActive()) {
      DisplayElement(pelement);
      UpdateElement(pelement);
   } else {
      HideElement(pelement);
   }
}

/**
 * Obtiene la lista
 * @return puntero a LayerList con la lista de elementos.
 */
LayerList* ViewerWidget::GetList() const {
   return pListViewer_ ? pListViewer_->GetList() : NULL;
}

/**
 * Obtiene el Viewer Activo
 * @return puntero a ViewerWidget con el Viewer Activo.
 * @return NULL en caso que no exista el Viewer Activo.
 */
ViewerWidget *ViewerWidget::GetActiveViewer() {
   if (pViewerManager_) {
      return pViewerManager_->GetActiveViewer();
   }
   return NULL;
}

/**
 * Carga un objeto que devuelve el Viewer activo
 * @param[in] pViewerManager nuevo ViewerManager
 * @return true.
 */
bool ViewerWidget::SetActiveViewerManager(ActiveViewerManager *pViewerManager) {
   pViewerManager_ = pViewerManager;
   return true;
}

SuriObject::UuidType ViewerWidget::GetId() const {
   return idObject_.GetId();
}

SuriObject::UuidIntType ViewerWidget::GetIntId() const {
   return SuriObject::CreateIntId(GetId());
}

SuriObject::UuidType ViewerWidget::GetViewcontextUuid() const {
   return viewcontextIdObject_.GetId();
}

bool ViewerWidget::SetViewcontext(ViewcontextInterface *pViewcontext) {
   SetElementList(pViewcontext->GetLayerList());
   viewcontextIdObject_ = SuriObject();
   return true;
}

/** Obtiene el widget de medicion */
MeassureWidget* ViewerWidget::GetMeasureWidget() {
   return pmeasure_;
}

/** Establece el widget de medicion */
void ViewerWidget::SetMeasureWidget(MeassureWidget* pMeasure) {
   pmeasure_ = pMeasure;
}

/**
 * Evento lista agregada
 * @param[in] pModel la lista que fue modificada
 */
void ViewerWidget::ListChanged(Model *pModel) {
   LayerList *plist = pListViewer_->GetList();
   if (plist != pModel && pElementViewer_) {
      return;
   }
   // Si se agregaron elementos, registro el viewer
   if (pListViewer_->ElementAdded()) {
      LayerList::ElementListType newelements = pListViewer_->GetNewElements();
      LayerList::ElementListType::iterator it = newelements.begin();
      for (; it != newelements.end(); ++it) {
         Element *pelement = *it;
         pelement->RegisterViewer(pElementViewer_);
         if (pelement->IsActive()) {
            DisplayElement(pelement);
         }
      }
   }
   // Si la cuenta de elementos activos llega a 0, reseteo el viewer
   if (plist->GetActiveCount() == 0) {
      ResetViewer();
   }
}

/**
 * Evento lista eliminada
 * @param[in] pModel lista que fue eliminada
 */
void ViewerWidget::ListDeleted(Model *pModel) {
   if (pModel == GetList()) {
      delete this;
   }
}

/**
 * Retorna navegador, si no existe lo crea.
 * \pre se debe haber inicializado el pListViewer del ViewerWidget
 * @return puntero a Navigator
 * @return NULL si no existe el View de la Lista.
 */
Navigator* ViewerWidget::GetNavigator() {
   if (!pListViewer_) {
      return NULL;
   }
   if (!pNavigator_) {
      pNavigator_ = new Navigator(pDataViewManager_);
      pNavigator_->SetSpatialReference(std::string(LATLON_SPATIAL_REFERENCE));
   }
   return pNavigator_;
}
}  // namespace suri
