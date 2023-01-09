/*! \file LayerListWidget.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */



// Includes standard
#include <vector>
#include <sstream>

// Includes Suri
#include "suri/LayerListWidget.h"
#include "suri/Element.h"
#include "suri/LayerList.h"
#include "suri/messages.h"
#include "suri/ElementPropertiesWidget.h"
#include "suri/ElementManager.h"
#include "resources.h"
#include "suri/ViewcontextInterface.h"
#include "suri/DataViewManager.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"
#include <wx/dcbuffer.h>
#include "wx/htmllbox.h"

// Includes App

// Forwards

/** namespace suri */
namespace suri {
/**
 * Constructor.
 * @param[in]	pParent: ventana padre.
 * @param[in]	pViewcontext: Lista de elementos.
 * @param[in]   pDatasourceManager: DatasourceManager activo
 */
LayerListWidget::LayerListWidget(wxWindow *pParent, ViewcontextInterface* pViewcontext,
                                 DataViewManager* pDataViewManager) :
      ElementListWidget(pParent, pViewcontext->GetLayerList(), pDataViewManager),
      pViewcontext_(pViewcontext), pDataViewManager_(pDataViewManager) {
   dragActivated_ = true;
}

/**
 * Constructor.
 * @param[in]  pParent: ventana padre.
 * @param[in]  pViewcontext: Lista de elementos.
 */
LayerListWidget::LayerListWidget(wxWindow *pParent, ViewcontextInterface* pViewcontext, LayerList* pLayerList, DataViewManager* pDataViewManager) :
      ElementListWidget(pParent, pViewcontext->GetLayerList(), pDataViewManager),
      pViewcontext_(pViewcontext), pLayerList_(pLayerList), pDataViewManager_(pDataViewManager) {
   dragActivated_ = true;
}

/**
 * Constructor.
 * @param[in]  pParent: ventana padre.
 * @param[in]  pViewcontext: Lista de elementos.
 */
LayerListWidget::LayerListWidget(wxWindow *pParent, LayerList* pLayerList, DataViewManager* pDataViewManager) :
      ElementListWidget(pParent, pLayerList, pDataViewManager),
      pViewcontext_(NULL), pLayerList_(pLayerList), pDataViewManager_(pDataViewManager) {
   dragActivated_ = true;
}

/**
 * Objetivo: Destructor
 */
LayerListWidget::~LayerListWidget() {
}

/**
 * Crea la ventana de herramienta
 * @return true / false, resultado del metodo de la clase base
 */
bool LayerListWidget::CreateToolWindow() {
   return ElementListWidget::CreateToolWindow();
}

/**
 * Muestra las propiedades de los elementos seleccionados
 */
void LayerListWidget::ShowElementProperties() {
   wxSimpleHtmlListBox *plist = XRCCTRL(*pToolWindow_, wxT("ID_HTMLLAYERLIST"),
         wxSimpleHtmlListBox);
   //LayerList* playerlist = (pViewcontext_)? pViewcontext_->GetLayerList() : pLayerList_;
   LayerList* playerlist = (pLayerList_)? pLayerList_ : pViewcontext_->GetLayerList();
   int elemcount =  playerlist->GetElementCount(!isMenuChecked_);
   for (int i = 0; i < elemcount; i++) {
      if (plist->IsCurrent(i)) {
         // pprop se elimina al cerrar el showmodal
         ElementPropertiesWidget *ppropertieswidget = NULL;

         if (pViewcontext_) {
         LayerInterface *player = pViewcontext_->GetLayerByElementId(
              playerlist->GetElement(i, !isMenuChecked_)->GetUid());
         SuriObject::UuidType datasourceId = player->GetAssociatedDatasourceId();
				DatasourceInterface *pdatasource =
						pDataViewManager_ ?
								pDataViewManager_->GetDatasourceManager()->GetDatasource(
										datasourceId) :
								NULL;
         ppropertieswidget = new ElementPropertiesWidget(pDataViewManager_, pdatasource, player);
         } else {
            Element* pelement = playerlist->GetElement(i, !isMenuChecked_);
            ppropertieswidget = new ElementPropertiesWidget(pDataViewManager_, pelement);
         }

         ppropertieswidget->ShowModal();
      }
   }
}

/**
 * Objetivo: Cierra los elementos seleccionados.
 * \todo end parece que no se usa para nada, verificar y eliminar.
 */
void LayerListWidget::CloseSelectedElements() {
   //LayerList* playerlist = (pViewcontext_)? pViewcontext_->GetLayerList() : pLayerList_;
   LayerList* playerlist = (pLayerList_)? pLayerList_ : pViewcontext_->GetLayerList();
   ElementManager::Instance().DeleteElements(
         playerlist->GetSelectedElementsUid(LayerList::AllGroups));
}

/**
 * Activa los elementos seleccionados en el visualizador asociado al contexto.
 */
void LayerListWidget::DisplaySelectedElements(bool Display) {
   //LayerList* playerlist = (pViewcontext_)? pViewcontext_->GetLayerList() : pLayerList_;
   LayerList* playerlist = (pLayerList_)? pLayerList_ : pViewcontext_->GetLayerList();
   int elemcount = playerlist->GetElementCount(!isMenuChecked_);
   Element *pfirstelem = NULL;
   for (int i = 0; i < elemcount; i++)
      if (playerlist->GetElementData(i, !isMenuChecked_)->selected_) {
         Element *pelement = playerlist->GetElement(i, !isMenuChecked_);
         if (!pfirstelem) {
            pfirstelem = pelement;
         }
         playerlist->ActivateElement(pelement, Display);
      }

   if (pfirstelem) {
      pfirstelem->SetChanged();
      pfirstelem->SendViewerUpdate();
   }
}

/**
 * Activa el elemento cuyo indice se corresponde con el del parametro Index
 * @param[in]	Index: indice del elemento dentro de la lista de elementos.
 */
void LayerListWidget::ActivateElement(int Index) {
   //LayerList* playerlist = (pViewcontext_)? pViewcontext_->GetLayerList() : pLayerList_;
   LayerList* playerlist = (pLayerList_)? pLayerList_ : pViewcontext_->GetLayerList();
   playerlist->ChangeActiveElement(Index, !isMenuChecked_);
}

/**
 * Muestra un menu sobre la lista.
 * \attention el menu debe ser eliminado por la clase que invoco el metodo
 * @param[in] Event evento del mouse
 * @param[in] Item item seleccionado
 * @return menu creado
 */
wxMenu* LayerListWidget::CreateContextMenu(wxMouseEvent &Event, int Item) {
   // creo menu contextual(distinto segun donde se hizo click)
   wxMenu *pmenu = NULL;
   if (Item != wxNOT_FOUND) {
      pmenu = wxXmlResource::Get()->LoadMenu(wxT("ID_ELEMENT_MENU"));
      // No se traduce, viene del XRC
      wxMenuItem *pitemdisplay = pmenu->FindItem(pmenu->FindItem(_(menu_DISPLAY)));
      // No se traduce, viene del XRC
      wxMenuItem *pitemhide = pmenu->FindItem(pmenu->FindItem(_(menu_HIDE)));
      if (pitemdisplay && pitemhide) {
         //LayerList* playerlist = (pViewcontext_)? pViewcontext_->GetLayerList() : pLayerList_;
         LayerList* playerlist = (pLayerList_)? pLayerList_ : pViewcontext_->GetLayerList();
         if (playerlist->GetElement(Item, !isMenuChecked_)->IsActive()) {
            pmenu->Remove(pitemdisplay);
         } else {
            pmenu->Remove(pitemhide);
         }
      } else {
         REPORT_ERROR(
               "D:No se encontraron items \"Desplegar\" u \"Ocultar\" en el menu");
         delete pmenu;
         return NULL;
      }
   } else {
      pmenu = new wxMenu();
   }
#ifdef __DEBUG__
   if (pmenu->FindItem(_(menu_SHOW_HIDDEN_ELEMENTS)) == wxNOT_FOUND) {
      pmenu->AppendCheckItem(ID_MENU_HIDDEN_ITEMS, _(menu_SHOW_HIDDEN_ELEMENTS));
   }
   pmenu->Check(ID_MENU_HIDDEN_ITEMS, isMenuChecked_);
#endif

   return pmenu;
}

/**
 * Informa si el elemento esta activo en la LayerList
 * @param[in] pElement elemento analizado
 * @return informa si elemento esta en lista de elementos seleccionados
 */
bool LayerListWidget::IsElementActive(Element* pElement) {
   return pElement->IsActive();
}

/**
 * Des/Activa el elemento en la LayerList
 * @param[in] pElement elemento a modificar
 * @param[in] Value estado del elemento
 */
void LayerListWidget::ActivateElement(Element* pElement, bool Value) {
   //LayerList* playerlist = (pLayerList_) ? pLayerList_ : pViewcontext_->GetLayerList();
   LayerList* playerlist = (pLayerList_)? pLayerList_ : pViewcontext_->GetLayerList();
   playerlist->ActivateElement(pElement, Value);
}

/**
 * Informa si el elemento esta seleccinado. Lo busca en la LayerList
 * @param[in] pElement elemento analizado
 * @return informa si elemento esta en lista de elementos seleccionados
 */
bool LayerListWidget::IsElementSelected(Element* pElement) {
   //LayerList* playerlist =  (pLayerList_)? pLayerList_ : pViewcontext_->GetLayerList();
   LayerList* playerlist = (pLayerList_)? pLayerList_ : pViewcontext_->GetLayerList();
   return playerlist->GetElementData(pElement)->selected_;
}

/**
 * Des/Selecciona el elemento en la LayerList
 * @param[in] pElement elemento a modificar
 * @param[in] Value estado del elemento
 */
void LayerListWidget::SelectElement(Element* pElement, bool Value) {
   //LayerList* playerlist = (pViewcontext_)? pViewcontext_->GetLayerList() : pLayerList_;
   LayerList* playerlist = (pLayerList_)? pLayerList_ : pViewcontext_->GetLayerList();
   playerlist->SelectElement(pElement, Value);
}

/**
 * Se llama cuando se activa boton del menu contextual
 * @param[in] Event evento
 */
void LayerListWidget::DoOnContextMenu(wxMenuEvent &Event) {
   // Menu de propiedades
   if (Event.GetId() == XRCID("ID_ELEMENT_PROPERTIES")) {
      ShowElementProperties();
   } else if (Event.GetId() == XRCID("ID_ELEMENT_CLOSE")) {
      CloseSelectedElements();
   } else if (Event.GetId() == XRCID("ID_ELEMENT_DISPLAY")) {
      DisplaySelectedElements(true);
   } else if (Event.GetId() == XRCID("ID_ELEMENT_HIDE")) {
      DisplaySelectedElements(false);
   } else if (Event.GetId() == ID_MENU_HIDDEN_ITEMS) {
      CheckMenuItem();
   }
}

}  // namespace suri
