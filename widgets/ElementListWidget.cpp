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
#include <sstream>
#include <set>
#include <algorithm>
#include <utility>
#include <map>
#include <string>

// Includes Suri
#include "suri/ElementListWidget.h"
#include "suri/Element.h"
#include "suri/LayerList.h"
#include "resources.h"
#include "suri/messages.h"
#include "suri/DataViewManager.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"
#include <wx/dcbuffer.h>
#include <wx/fs_mem.h> // filesystem
#include "wx/htmllbox.h"
#include <wx/image.h>

// Forwards
/** Inicializa los recursos de la ventana */
void InitLayerListXmlResource();

/** namespace suri */
namespace suri {
// Defines
/** cte para ajustar tamanio del HTML */
#define MAX_WIDTH_SUBSTRACTION 64
#if defined(WX_VLISTBOX_HAS_GETITEMRECT)
/** Macro que utiliza el GetItemRect de HtmlSimpleList del patch wxsuri */
#  define GETITEMRECT(Index) plist->GetItemRect(Index)
#else
/** Clase workaround para las versiones de wx sin el patch wxsuri */
class _dummy_Item_Rect_
{
public:
   /** ctor */
   _dummy_Item_Rect_() {
   }
   /** retorna un ancho prefijado */
   int GetWidth() {
      return 128;
   }
   /** retorna alto prefijado */
   int GetHeight() {
      return 64;
   }
};
/** Macro que utiliza la clase dummy para poder compilar sin el patch wxsuri */
#  define GETITEMRECT(Index) _dummy_Item_Rect_()
#endif
/** Elemento tipo hatl */
#define ELEMENT_HTML \
   "																			\
				<table>																				\
					<tr>																				\
						<td><img src=\"%s\"><!--GetIcon()--></td>							\
						<td>																			\
							<table width=\"%dpx\"> <!-- GetMaxItemWidth() -->			\
								<tr>																	\
									<td>%s<!-- GetName() --></td>								\
									<tr>																\
										<table align= \"left\" width=\"%dpx\">				\
											<tr>														\
												<td align=\"right\">								\
													<font size=1>									\
														%s <!-- GetDetails() -->				\
													</font>											\
													<a href=\"checkbox\">						\
														<img src=\"%s\"><!--IsActive()-->	\
													</a>												\
												</td>													\
											</tr>														\
										</table>														\
									</tr>																\
								</tr>																	\
							</table>																	\
						</td>																			\
					</tr>																				\
				</table>"
/** Intervalo de tiempo en milisegundos */
#define TIMER_INTERVAL 500 // milisegundos
/** ID para el objeto wx que representa la Html Layerlist \todo eliminar, no se usa */
#define ID_HTMLLAYERLIST 10010
/** ID para el objeto wxtimer */
#define ID_TIMER 10020

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(LayerEventHandler, ElementListWidget)IMPLEMENT_EVENT_CALLBACK(OnDoubleClick, OnDoubleClick(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnMouseClick, OnMouseClick(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnLeftDown, OnLeftDown(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnContextMenu, OnContextMenu(event), wxMenuEvent)
   IMPLEMENT_EVENT_CALLBACK(OnSelectionChanged, OnSelectionChanged(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnResize, OnResize(event), wxSizeEvent)
   IMPLEMENT_EVENT_CALLBACK(OnCheck, OnCheck(event), wxHtmlLinkEvent)
   IMPLEMENT_EVENT_CALLBACK(OnDrag, OnDrag(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnLeftUp, OnLeftUp(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnUIUpdate, OnUIUpdate(event), wxUpdateUIEvent)
   IMPLEMENT_EVENT_CALLBACK(OnKeyPress, OnKeyPress(event), wxKeyEvent)
   IMPLEMENT_EVENT_CALLBACK(OnTimer, OnTimer(event), wxTimerEvent)
   IMPLEMENT_EVENT_CALLBACK(OnCaptureLost, OnCaptureLost(event), wxMouseCaptureLostEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Constructor.
 * @param[in]	pParent: ventana padre.
 * @param[in]	pLayerList: Lista de elementos.
 */
ElementListWidget::ElementListWidget(wxWindow *pParent, suri::LayerList *pLayerList,
                                     suri::DataViewManager *DataViewManager) :
      suri::Widget(pParent), pEventHandler_(new LayerEventHandler(this)),
      pLayerList_(pLayerList), pDataViewManager_(DataViewManager) {
   wantedWidth_ = 150;
   wantedHeight_ = 150;
   isDragging_ = false;
   resized_ = false;
   pElementFrom_ = NULL;
   indexHtmlItemFrom_ = wxNOT_FOUND;
   indexHtmlItemTo_ = wxNOT_FOUND;
   coordLastLine_ = -1;
   scrollDirection_ = 0;
   pElementViewer_ = new CallbackView<ElementListWidget>(
         this, &ElementListWidget::Update, &ElementListWidget::ElementDelete);

   isMenuChecked_ = false;
   dragActivated_ = false;
   pLayerList_->RegisterViewer(this);
}

/**
 * Objetivo: Destructor
 */
ElementListWidget::~ElementListWidget() {
   if (Model::IsValid(pLayerList_)) {
      pLayerList_->UnregisterViewer(this);
      int layerlistcount = pLayerList_->GetElementCount(!isMenuChecked_);
      for (int elemidx = 0; elemidx < layerlistcount; elemidx++) {
         suri::Element *pelement = pLayerList_->GetElement(elemidx, !isMenuChecked_);
         pelement->UnregisterViewer(pElementViewer_);
      }
   }
   delete pTimer_;
   delete pElementViewer_;
   DELETE_EVENT_OBJECT
   ;
}

/**
 * Carga la ventana diseniada. Si tuvo exito, conecta los eventos con los metodos.
 * @return true Creacion exitosa.
 * @return false No se pudo cargar el recurso XRC
 */
bool ElementListWidget::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_SLLAYERLIST"));
   if (!pToolWindow_) {
      return false;
   }
   wxSimpleHtmlListBox *plistbox = XRCCTRL(*pToolWindow_, wxT("ID_HTMLLAYERLIST"),
         wxSimpleHtmlListBox);
   if (plistbox) {
      plistbox->SetBackgroundStyle(wxBG_STYLE_CUSTOM);

      plistbox->Connect(wxEVT_LEFT_DCLICK,
                        wxMouseEventHandler(LayerEventHandler::OnDoubleClick), NULL,
                        pEventHandler_);
      plistbox->Connect(wxEVT_LEFT_DOWN,
                        wxMouseEventHandler(LayerEventHandler::OnLeftDown), NULL,
                        pEventHandler_);
      plistbox->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(LayerEventHandler::OnLeftUp),
                        NULL, pEventHandler_);
      plistbox->Connect(wxEVT_RIGHT_UP,
                        wxMouseEventHandler(LayerEventHandler::OnMouseClick), NULL,
                        pEventHandler_);
      plistbox->Connect(wxEVT_RIGHT_DOWN,
                        wxMouseEventHandler(LayerEventHandler::OnMouseClick), NULL,
                        pEventHandler_);
      plistbox->Connect(wxEVT_COMMAND_LISTBOX_SELECTED,
                        wxCommandEventHandler(LayerEventHandler::OnSelectionChanged),
                        NULL, pEventHandler_);

      plistbox->Connect(wxEVT_MOTION, wxMouseEventHandler(LayerEventHandler::OnDrag),
                        NULL, pEventHandler_);
      plistbox->Connect(wxEVT_SIZE, wxSizeEventHandler(LayerEventHandler::OnResize),
                        NULL, pEventHandler_);
      plistbox->Connect(wxEVT_COMMAND_HTML_LINK_CLICKED,
                        wxHtmlLinkEventHandler(LayerEventHandler::OnCheck), NULL,
                        pEventHandler_);
      plistbox->Connect(wxEVT_UPDATE_UI,
                        wxUpdateUIEventHandler(LayerEventHandler::OnUIUpdate), NULL,
                        pEventHandler_);
      plistbox->Connect(wxEVT_KEY_DOWN,
                        wxKeyEventHandler(LayerEventHandler::OnKeyPress), NULL,
                        pEventHandler_);
      plistbox->Connect(wxEVT_TIMER, wxTimerEventHandler(LayerEventHandler::OnTimer),
                        NULL, pEventHandler_);
      plistbox->Connect(
            wxEVT_MOUSE_CAPTURE_LOST,
            wxMouseCaptureLostEventHandler(LayerEventHandler::OnCaptureLost), NULL,
            pEventHandler_);
      pTimer_ = new wxTimer(plistbox, ID_TIMER);
   }
   return true;
}

/**
 * Devuelve el titulo de la ventana(esto es utilizado cuando la ventana se crea
 * en modo standalone).
 * @return wxString con el titulo de la ventana.
 */
const wxString ElementListWidget::GetWindowTitle() const {
   return _(caption_LAYERS);
}

/**
 * Activa el elemento cuyo indice se corresponde con el del parametro Index
 * @param[in]	Index: indice del elemento dentro de la lista de elementos.
 */
void ElementListWidget::ActivateElement(int Index) {
   pLayerList_->ChangeActiveElement(Index);
}

/**
 * Modifica el item de menu que permite ver los elementos ocultos en el menu contextual.
 * Actualiza el flag que indica esta funcionalidad.
 * \note Este metodo se llama desde OnContextMenu() para el item ID_MENU_HIDDEN_ITEMS
 * \post El item del menu contextual cambia el texto de acuerdo a si oculta o muestra
 *  los elementos ocultos.
 * \post El flag isMenuChecked_ invierte su valor.
 * \todo si se usa un validator o LoadDataFromWindow no se podria eliminar
 * este metodo?
 */
void ElementListWidget::CheckMenuItem() {
   wxMenu *pmenu = wxXmlResource::Get()->LoadMenu(wxT("ID_ELEMENT_MENU"));
   if (pmenu->FindItem(wxT("Mostrar elementos ocultos")) == wxNOT_FOUND)  // No se traduce, es de
                                                                          // debug
   {
      pmenu->AppendCheckItem(ID_MENU_HIDDEN_ITEMS, wxT("Mostrar elementos ocultos"));  // No se
                                                                                       // traduce,
                                                                                       // es de
                                                                                       // debug
   }
   pmenu->Check(ID_MENU_HIDDEN_ITEMS, !isMenuChecked_);
   isMenuChecked_ = !isMenuChecked_;
   delete pmenu;
   pLayerList_->SetChanged();
   pLayerList_->SendViewerUpdate();
}

/**
 * Objetivo: Actualiza la lista.
 * Recorre la lista de elementos generando para cada uno el HTML que inserta en
 * el control wxSimpleHtmlList.
 * \note En linux modifica el string correspondiente al HTML para convertirlo
 *  a UTF8 y asi mostrar los caracteres no-ASCII 127 en forma correcta.
 * \note Esto se debe a que el control usa una codificacion y los valores de
 *  las variables y datos en otra.
 * @param[in]	pModel: modelo que llama al Update.
 */
void ElementListWidget::Update(Model *pModel) {
   wxSimpleHtmlListBox *plist = XRCCTRL(*pToolWindow_, wxT("ID_HTMLLAYERLIST"),
         wxSimpleHtmlListBox);
   if (!plist) {
      REPORT_AND_FAIL("D:Lista de elementos invalida.");
   }
   wxArrayString elementarray;
   int currentitem = wxNOT_FOUND;

   // Filtro los elementos
   int layerlistcount = pLayerList_->GetElementCount(!isMenuChecked_);
   displayedElements_.clear();
   int displayedlistsize = 0;
   for (int elemidx = 0; elemidx < layerlistcount; elemidx++) {
      suri::Element *pelement = pLayerList_->GetElement(elemidx, !isMenuChecked_);
      if (CanShowElement(pelement)) {
         displayedElements_.insert(
               std::pair<int, Element*>(displayedlistsize, pelement));
         displayedlistsize++;
      }
   }

   if (displayedlistsize < (int) plist->GetCount()) {
      plist->Clear();
   }

   for (int i = 0; i < displayedlistsize; i++) {
      suri::Element *pelement = displayedElements_[i];
      if (!pelement) {
         REPORT_AND_FAIL("D:Elemento invalido");
      }

      pelement->RegisterViewer(pElementViewer_);

      // me fijo cual es el elemento actual (borde negro)
      if (plist->IsCurrent(i)) {
         currentitem = i;
      }
      int width = std::max(GetMaxItemWidth(), 1);

      // Agrega * a nombre si esta en edicion
      std::string elementname = pelement->GetName().c_str();
      if (pelement->IsEdited()) {
         elementname = elementname + " *";
      }

      // Genera htmls
      wxString string = wxString::Format(
            ELEMENT_HTML,
            GetElementIcon(pelement).c_str(),
            width,
            elementname.c_str(),
            width,
            GetElementDescription(pelement).c_str(),
            IsElementActive(pelement) ? wxT(button_CHECK_CHECKED) :
                                        wxT(button_CHECK_UNCHECKED));
#if defined(__LINUX__)
      string = wxString(wxConvUTF8.cMB2WC(string.c_str()),
                        wxCSConv(suri::XmlElement::xmlEncoding_.c_str()));
      if (string.empty()) {
         string = wxString::Format(
               ELEMENT_HTML,
               GetElementIcon(pelement).c_str(),
               width,
               elementname.c_str(),
               width,
               GetElementDescription(pelement).c_str(),
               IsElementActive(pelement) ? wxT(button_CHECK_CHECKED) :
                                           wxT(button_CHECK_UNCHECKED));
      }
#endif
      if (string.empty()) {
         REPORT_ERROR("D:Error al generar el string para el elemento.");
         string = wxString::Format(
               ELEMENT_HTML,
               wxT(""),
               width,
               wxT(""),
               width,
               wxT(""),
               IsElementActive(pelement) ? wxT(button_CHECK_CHECKED) :
                                           wxT(button_CHECK_UNCHECKED));
      }

      // Reemplazo o agrego segun este en el final de la lista
      if ((int) plist->GetCount() > i) {
         plist->SetString(i, string);
      } else {
         plist->Append(string);
      }
   }
   plist->Freeze();
   // Limpio la seleccion
   ((wxVListBox*) plist)->SetSelection(-1);
   // Selecciono los elementos
   std::map<int, Element*>::iterator it = displayedElements_.begin();
   while (it != displayedElements_.end()) {
      if (IsElementSelected((*it).second)) {
         ((wxVListBox*) plist)->Select((*it).first, true);
      }
      it++;
   }

   // Activo el que estaba con el recuadro negro
   if (currentitem != wxNOT_FOUND && ((wxVListBox*) plist)->IsSelected(currentitem)) {
      ((wxVListBox*) plist)->Select(currentitem, true);
   }
   plist->Thaw();
}

/**
 * Recibe el aviso de destruccion del modelo, anula el puntero a la lista y
 * se auto-destruye (comportamiento posible aunque delicado)
 * @param[in]	pModel: modelo que llama al Update.
 */
void ElementListWidget::OnModelDelete(Model *pModel) {
   pLayerList_ = NULL;
   delete this;
}

/**
 * Objetivo: Recibe el aviso de delete.
 * @param[in]	pModel: modelo que se esta actualizando (llama al Update)
 */
void ElementListWidget::ElementDelete(Model *pModel) {
   pModel->UnregisterViewer(pElementViewer_);
}

/**
 * Este metodo se ejecuta cuando la lista recibe un evento de doble click.
 * Activa/Desactiva un elemento de la lista.
 * @param[in]	Event: evento.
 */
void ElementListWidget::OnDoubleClick(wxMouseEvent &Event) {
   wxSimpleHtmlListBox *plist = XRCCTRL(*pToolWindow_, wxT("ID_HTMLLAYERLIST"),
         wxSimpleHtmlListBox);
   // no encontro la lista
   if (!plist) {
      return;
   }
   int item = plist->HitTest(Event.m_x, Event.m_y);
   // fuera de item ignoro
   if (item == wxNOT_FOUND) {
      return;
   }

   ActivateElement(displayedElements_[item],
                   !IsElementActive(displayedElements_[item]));
}

/**
 * Este metodo se ejecuta cuando la lista recibe un evento de click.
 * Tiene distinto comportamiento segun haya recibido un click-der. o click-izq.
 *    - Click-izq.: selecciona el elemento sobre el cual se hizo click.
 *    - Click-der.: despliega menu contextual sobre el elemento.
 * @param[in]	Event: evento.
 */
void ElementListWidget::OnMouseClick(wxMouseEvent &Event) {
   wxSimpleHtmlListBox *plist = XRCCTRL(*pToolWindow_, wxT("ID_HTMLLAYERLIST"),
         wxSimpleHtmlListBox);
   // no encontro la lista
   if (!plist) {
      return;
   }

   // libero el mouse ante un evento de boton derecho.
   if (plist->HasCapture()) {
      plist->ReleaseMouse();
   }
   int item = plist->HitTest(Event.m_x, Event.m_y);

   ClearSelectedElements();

   plist->SetSelection(item);
   wxCommandEvent dummy;
   OnSelectionChanged(dummy);

   /* Este if esta ya que a este metodo se lo invoca desde el metodo OnCheck
    * para reutilizar la porcion de codigo que maneja la seleccion y deseleccion
    * de los elementos de la lista.
    * Si se elimina el if, cuando se hace click-izq. sobre la checkbox de los
    * elementos se lanzaria el menu contextual.
    * */
   if (Event.RightUp()) {
      wxMenu *pmenu = CreateContextMenu(Event, item);
      if (pmenu) {
         pmenu->Connect(wxEVT_COMMAND_MENU_SELECTED,
                        wxMenuEventHandler(LayerEventHandler::OnContextMenu), NULL,
                        pEventHandler_);
         plist->PopupMenu(pmenu, Event.m_x, Event.m_y);
         delete pmenu;
      }
   }
   Event.Skip();
}

/**
 * Este metodo se ejecuta cuando la lista recibe el evento de click izq.(sin
 * soltar el boton).
 * Obtiene el indice del elemento sobre el cual se hizo click para luego
 * permitir el arrastre de dicho elemento dentro de la lista.
 * @param[in]	Event: evento.
 */
void ElementListWidget::OnLeftDown(wxMouseEvent &Event) {
   wxSimpleHtmlListBox *plist = XRCCTRL(*pToolWindow_, wxT("ID_HTMLLAYERLIST"),
         wxSimpleHtmlListBox);
   // no encontro la lista
   if (!plist) {
      return;
   }

   indexHtmlItemFrom_ = plist->HitTest(Event.m_x, Event.m_y);
   if (indexHtmlItemFrom_ == wxNOT_FOUND) {
      ClearSelectedElements();
      plist->SetSelection(wxNOT_FOUND);
      wxCommandEvent dummy;
      OnSelectionChanged(dummy);
   }
   Event.Skip();
}

/**
 * Este metodo se ejecuta cuando la lista recibe el evento de soltado del boton
 * izq. del mouse.
 * Determina si se estaba arrastrando un elemento y en tal caso detiene el timer
 * (utilizado para producir el scrolleo de automatico de la lista).
 * Setea el flag que indica que se esta arrastrando un elemento en false y mueve
 * el elemento seleccionado a la posicion detectada.
 * @param[in]	Event: evento.
 */
void ElementListWidget::OnLeftUp(wxMouseEvent &Event) {
   wxSimpleHtmlListBox *plist = XRCCTRL(*pToolWindow_, wxT("ID_HTMLLAYERLIST"),
         wxSimpleHtmlListBox);
   if (!plist) {
      return;
   }

   // Evita que al hacer click sobre lista y luego sobre otra cosa, la lista
   // atrape el segundo evento
   if (plist->HasCapture()) {
      plist->ReleaseMouse();
   }

   if (Event.LeftUp() && isDragging_ && indexHtmlItemFrom_ != wxNOT_FOUND) {
      isDragging_ = false;
      pTimer_->Stop();
      suri::Element *pelementto = NULL;

      if (indexHtmlItemFrom_ == indexHtmlItemTo_) {
         plist->Refresh();
         return;
      }

      if (indexHtmlItemTo_ != (int) displayedElements_.size()) {
         pelementto = displayedElements_[indexHtmlItemTo_];
      }

      if (pElementFrom_ != pelementto) {
         if (pDataViewManager_ == NULL)
            pLayerList_->MoveElement(pElementFrom_, pelementto);
         else {
            if (pelementto != NULL)
               pDataViewManager_->GetDatasourceManager()->ChangeOrderFromElementId(pElementFrom_->GetUid(),
                                                             pelementto->GetUid());
            else
               pDataViewManager_->GetDatasourceManager()->MoveToEndFromElementId(pElementFrom_->GetUid());
         }
         //pLayerList_->MoveElement(pElementFrom_, pelementto);
      }

      plist->Refresh();
   }

   if (plist->HasCapture()) {
      plist->ReleaseMouse();
   }
   plist->Refresh();
   Event.Skip();
}

/**
 * Este metodo se ejecuta cuando el menu contextual, creado al hacer click der.
 * sobre la lista, recibe un evento de click sobre alguno de sus items.
 * \post Se ejecuta el metodo asociado a cada item del menu.
 * @param[in] Event click sobre algun elemento del menu contextual
 */
void ElementListWidget::OnContextMenu(wxMenuEvent &Event) {
   DoOnContextMenu(Event);

   Event.Skip(false);
}

/**
 * Este metodo se ejecuta cuando se produce un evento de seleccion de items
 * de la lista de elementos.
 * Tiene por objetivo marcar los elementos de la lista que se van seleccionando.
 * @param[in]	Event: evento.
 */
void ElementListWidget::OnSelectionChanged(wxCommandEvent &Event) {
   /**
    * \todo Demasiadas entradas a Update por culpa de los elementos
    */
   wxSimpleHtmlListBox *plist = XRCCTRL(*pToolWindow_, wxT("ID_HTMLLAYERLIST"),
         wxSimpleHtmlListBox);
   // no encontro la lista => salgo
   if (!plist) {
      return;
   }
   unsigned long cookie;
   std::vector<int> selection;
   int sel = plist->GetFirstSelected(cookie);
   while (sel != wxNOT_FOUND) {
      selection.push_back(sel);
      sel = plist->GetNextSelected(cookie);
   }

   ClearSelectedElements();
   std::vector<int>::iterator it = selection.begin();
   while (it != selection.end()) {
      SelectElement(displayedElements_[*it], true);
      it++;
   }
}

/**
 * Este metodo se ejecuta cuando la lista recibe los eventos de resize para
 * poder alinear la informacion de los elementos de la misma.
 * @param[in]	Event: evento
 */
void ElementListWidget::OnResize(wxSizeEvent &Event) {
   resized_ = true;
   wxSimpleHtmlListBox *plist = XRCCTRL(*pToolWindow_, wxT("ID_HTMLLAYERLIST"),
         wxSimpleHtmlListBox);
   if (!plist) {
      return;
   }
   plist->UpdateWindowUI(wxUPDATE_UI_FROMIDLE);
   Event.Skip();
}

/**
 * Este metodo se ejecuta cuando algun item de la lista recibe un evento de
 * click.
 * Verifica que el click se haya realizado sobre un link que poseen los items
 * de la lista cuyo imagen es una checkbox.
 * Este metodo tiene por objetivo Activar/Desactivar un elemento de la lista y
 * en base a dicha accion cambia la imagen del link(checkbox marcada/desmarcada)
 * @param[in]	Event: evento.
 */
void ElementListWidget::OnCheck(wxHtmlLinkEvent &Event) {
   wxSimpleHtmlListBox *plist = XRCCTRL(*pToolWindow_, wxT("ID_HTMLLAYERLIST"),
         wxSimpleHtmlListBox);
   // no encontro la lista
   if (!plist) {
      return;
   }

   /*invoco al evento OnMouseClick para que me seleccione el elemento sobre
    * el cual se clickeo para chequear la checkbox.*/
   wxHtmlLinkInfo stlinkinfo = Event.GetLinkInfo();
   wxMouseEvent eventmouse = *(stlinkinfo.GetEvent());
   OnMouseClick(eventmouse);

   if (stlinkinfo.GetHref() == "checkbox")  // No se traduce, elemento de XRC
         {
      unsigned long cookie;
      int sel = plist->GetFirstSelected(cookie);
      if (sel == wxNOT_FOUND) {
         return;
      }

      // activo/desactivo el elemento seleccionado
      suri::Element *pelement = displayedElements_[sel];
      if (pelement) {
         ActivateElement(pelement, !IsElementActive(pelement));
      }
   }
   Event.Skip();
}

/**
 * Este metodo se ejecuta cuando se produce un evento de drag sobre la lista.
 * Setea el flag de que se esta arrastrando un elemento, obtiene el elemento que
 * se esta arrastrando.
 * Fuerza que se dispare el evento UIUpdate para que se dibuje sobre la lista
 * una linea que indica la siguiente posicion del elemento arrastrado.
 * Si no esta activo el drag no se continua con los eventos.
 *	@param[in]	Event: evento.
 */
void ElementListWidget::OnDrag(wxMouseEvent &Event) {
   if (dragActivated_) {
      wxSimpleHtmlListBox *plist = XRCCTRL(*pToolWindow_, wxT("ID_HTMLLAYERLIST"),
            wxSimpleHtmlListBox);
      if (!plist) {
         return;
      }

      if (Event.Dragging() && !(Event.RightIsDown() || Event.RightDown())) {
         if (!isDragging_ && indexHtmlItemFrom_ != wxNOT_FOUND) {
            isDragging_ = true;
            pElementFrom_ = displayedElements_[indexHtmlItemFrom_];
         } else if (indexHtmlItemFrom_ == wxNOT_FOUND) {
            return;
         }

         if (!plist->HasCapture()) {
            plist->CaptureMouse();
         }

         mouseX_ = Event.m_x;
         mouseY_ = Event.m_y;
         if (SelectNextElement(mouseX_, mouseY_)) {
            plist->UpdateWindowUI(wxUPDATE_UI_FROMIDLE);
         }
      } else {
         isDragging_ = false;
         indexHtmlItemFrom_ = indexHtmlItemTo_ = wxNOT_FOUND;
      }
      Event.Skip();
   }
}

/**
 * Este metodo responde al evento UIUpdate que se ejecuta cuando no hay eventos
 * de usuario en la cola de eventos de wx.
 * Tiene 2 objetivos:
 *    - si se esta llevando a cabo una operacion de drag:
 *       pinta una linea marcando la nueva posicion del elemento que se esta draggeando.
 *    - si se esta modificando el tamanio de la lista:
 *        ejecutar un Update para actualizar los elementos de la misma.
 * @param[in]	Event: evento.
 */
void ElementListWidget::OnUIUpdate(wxUpdateUIEvent &Event) {
   if (isDragging_) {
      wxSimpleHtmlListBox *plist = XRCCTRL(*pToolWindow_, wxT("ID_HTMLLAYERLIST"),
            wxSimpleHtmlListBox);
      if (!plist) {
         Event.Skip();
      }

      /*Sumo las alturas de los items para pintar la linea al final del item.
       * Nota: no se xq no necesito hacer el for inverso cuando voy de abajo
       * hacia arriba*/
      int coordy = 0;
      for (int i = plist->GetVisibleBegin(); i < indexHtmlItemTo_; i++)
         coordy += GETITEMRECT(i).GetHeight();

      if (coordLastLine_ != coordy) {
         plist->Refresh();
         coordLastLine_ = coordy;
      } else if (coordLastLine_ == 0) {
         plist->Refresh();
      }

      wxClientDC *pclientdc = new wxClientDC(plist);
      pclientdc->SetBrush(wxBrush(wxColour(0, 0, 0)));
      pclientdc->SetPen(wxPen(wxColour(0, 0, 0), 6));
      pclientdc->DrawLine(0, coordy, plist->GetSize().GetX(), coordy);
      delete pclientdc;
   }

   if (resized_) {
      Update(NULL);
      resized_ = false;
   }
   Event.Skip();
}

/**
 * Este metodo responde a los eventos generados por el usuario al presionar una
 * tecla del teclado. Solo se esta procesando la tecla "ESC".
 * \post Finaliza la operacion de drag al presionar la tecla ESC.
 * @param[in]	Event: evento.
 */
void ElementListWidget::OnKeyPress(wxKeyEvent &Event) {
   if (Event.GetKeyCode() == 27 && isDragging_) {
      isDragging_ = false;
      wxSimpleHtmlListBox *plist = XRCCTRL(*pToolWindow_, wxT("ID_HTMLLAYERLIST"),
            wxSimpleHtmlListBox);
      if (!plist) {
         return;
      }
      plist->Refresh();
      Event.Skip(false);
   }
   Event.Skip();
}

/**
 * Este metodo responde a los eventos de Timer sobre la lista.
 * El objetivo de este metodo es poder realizar un scroll vertical , de forma
 * automatica, sobre la lista de elementos.
 * @param[in]	Event: evento.
 */
void ElementListWidget::OnTimer(wxTimerEvent &Event) {
   wxSimpleHtmlListBox *plist = XRCCTRL(*pToolWindow_, wxT("ID_HTMLLAYERLIST"),
         wxSimpleHtmlListBox);
   if (!plist) {
      return;
   }
   plist->ScrollLines(scrollDirection_);
   indexHtmlItemTo_ = plist->HitTest(
         mouseX_,
         mouseY_ + (scrollDirection_ * GETITEMRECT(indexHtmlItemTo_).GetHeight()));

   if (indexHtmlItemTo_ == wxNOT_FOUND) {
      indexHtmlItemTo_ = plist->GetVisibleEnd();
   }

   Event.Skip();
}

/**
 * Identifica el elemento cuya posicion es el nuevo destino del elemento
 * que se esta draggeando, siempre que sea posible moverlo a dicha posicion.
 * \post indexHtmlItemTo_ tiene el indice donde se insertaria el elemento
 *  que esta siendo arrastrado si se soltara luego de esta llamada.
 * \post indexHtmlItemTo_ debe ser validado por la lista para poder tomar
 *  el valor calculado, de lo contrario no se modifica.
 * @param[in]	MouseX: coordenada X del elemento que se esta draggeando.
 * @param[in]	MouseY: coordenada Y del elemento que se esta draggeando.
 * @return false No se pudo obtener la lista
 * @return true Hay un solo elemento en la lista
 */
bool ElementListWidget::SelectNextElement(long MouseX, long MouseY) {
   wxSimpleHtmlListBox *plist = XRCCTRL(*pToolWindow_, wxT("ID_HTMLLAYERLIST"),
         wxSimpleHtmlListBox);
   if (!plist) {
      return false;
   }

   // si hay 1 solo elemento no draggeo.
   if (plist->GetCount() == 1) {
      isDragging_ = false;
      indexHtmlItemFrom_ = indexHtmlItemTo_ = 0;
      plist->Refresh();
      return true;
   }

   indexHtmlItemTo_ = plist->HitTest(
         MouseX, MouseY + GETITEMRECT(indexHtmlItemFrom_).GetHeight() / 2);

   if (indexHtmlItemTo_ == wxNOT_FOUND) {  // despues del ultimo elemento de la lista.
      indexHtmlItemTo_ = plist->GetVisibleEnd();
   }

   // Esto es para activar el Timer y poder scrollear.
   if (indexHtmlItemTo_ == (int) plist->GetVisibleEnd()) {
      scrollDirection_ = 1;
      pTimer_->Start(TIMER_INTERVAL, wxTIMER_CONTINUOUS);
   } else if (indexHtmlItemTo_ == (int) plist->GetVisibleBegin()) {
      scrollDirection_ = -1;
      pTimer_->Start(TIMER_INTERVAL, wxTIMER_CONTINUOUS);
   } else {
      pTimer_->Stop();
   }

   if (!pLayerList_->CanMoveElement(
         pLayerList_->GetElement(indexHtmlItemFrom_, !isMenuChecked_),
         displayedElements_[indexHtmlItemTo_])) {
      isDragging_ = false;
      if (indexHtmlItemFrom_ < indexHtmlItemTo_) {
         while (!pLayerList_->CanMoveElement(displayedElements_[indexHtmlItemFrom_],
                                             displayedElements_[--indexHtmlItemTo_]));
      } else {
         while (!pLayerList_->CanMoveElement(displayedElements_[indexHtmlItemFrom_],
                                             displayedElements_[++indexHtmlItemTo_]));
      }
      isDragging_ = true;
   }
   return true;
}

/**
 * Este metodo se ejecuta cuando la lista libera el mouse.
 * \post isDragging pasa a false
 * \post Se refresca la lista HTML.
 * \post Se libera la captura del mouse.
 * @param[in]	Event: evento.
 */
void ElementListWidget::OnCaptureLost(wxMouseCaptureLostEvent &Event) {
   isDragging_ = false;
   wxSimpleHtmlListBox *plist = XRCCTRL(*pToolWindow_, wxT("ID_HTMLLAYERLIST"),
         wxSimpleHtmlListBox);
   if (!plist) {
      return;
   }
   if (plist->HasCapture()) {
      plist->ReleaseMouse();
   }

   plist->Refresh();
}

/**
 * Determina si hay que mostrar el elemento. Por default se muestran todos
 * @param[in] pElement elemento que se analiza
 * @return bool que indica si hay me mostrarlo
 */
bool ElementListWidget::CanShowElement(Element* pElement) {
   return true;
}

/**
 * Retorna el dibujo que hay que mostrar junto al elemento
 * @param[in] pElement elemento para el que se obtiene el icono
 * @return string con direccion del icono en disco
 */
std::string ElementListWidget::GetElementIcon(Element* pElement) {
   return pElement->GetIcon().c_str();
}

/**
 * Retorna la descripcion del elemento
 * @param[in] pElement elemento del que se obtiene la descripcion
 * @return descripcion del elemento
 */
std::string ElementListWidget::GetElementDescription(Element* pElement) {
   return pElement->GetDetails().c_str();
}

/**
 * Informa si el elemento esta activo. Lo busca en las listas internas
 * @param[in] pElement elemento analizado
 * @return informa si elemento esta en lista de elementos seleccionados
 */
bool ElementListWidget::IsElementActive(Element* pElement) {
   bool returnvalue = false;
   std::set<Element*>::iterator it = activeElements_.find(pElement);
   if (it != activeElements_.end()) {
      returnvalue = true;
   }
   return returnvalue;
}

/**
 * Des/Activa el elemento agregandolo o eliminadolo de la lista de
 * activos
 * @param[in] pElement elemento a modificar
 * @param[in] Value estado del elemento
 */
void ElementListWidget::ActivateElement(Element* pElement, bool Value) {
   if (Value == true) {
      activeElements_.insert(pElement);
   } else {
      activeElements_.erase(pElement);
   }
   Update(NULL);
}

/**
 * Informa si el elemento esta seleccinado. Lo busca en las listas internas
 * @param[in] pElement elemento analizado
 * @return informa si elemento esta en lista de elementos seleccionados
 */
bool ElementListWidget::IsElementSelected(Element* pElement) {
   bool returnvalue = false;
   std::set<Element*>::iterator it = selectedElements_.find(pElement);
   if (it != selectedElements_.end()) {
      returnvalue = true;
   }
   return returnvalue;
}

/**
 * Des/Selecciona el elemento agregandolo o eliminadolo de la lista de
 * seleccionados
 * @param[in] pElement elemento a modificar
 * @param[in] Value estado del elemento
 */
void ElementListWidget::SelectElement(Element* pElement, bool Value) {
   if (Value == true) {
      selectedElements_.insert(pElement);
   } else {
      selectedElements_.erase(pElement);
   }
   Update(NULL);
}

/**
 * Muestra un menu sobre la lista. Por default no hay menu
 * \attention el menu debe ser eliminado por la clase que invoco el metodo
 * @param[in] Event evento del mouse
 * @param[in] Item item seleccionado
 * @return menu creado
 */
wxMenu* ElementListWidget::CreateContextMenu(wxMouseEvent &Event, int Item) {
   return NULL;
}

/**
 * Todos los elementos tendran selected=false
 */
void ElementListWidget::ClearSelectedElements() {
   /** Esto es un parche para que funcione ver como hacer sin bloquear lista */
   int listsize = displayedElements_.size();
   for (int i = 0; i < listsize; i++) {
      SelectElement(displayedElements_[i], false);
   }
}

/**
 * Objetivo: Devuelve el ancho maximo de los elementos visibles de la lista.
 * @return Ok? maxWidthRect_: -1;
 */
int ElementListWidget::GetMaxItemWidth() {
   wxSimpleHtmlListBox *plist = XRCCTRL(*pToolWindow_, wxT("ID_HTMLLAYERLIST"),
         wxSimpleHtmlListBox);
   if (!plist) {
      return -1;
   }
   int maxwidth = 0, widthrect = 0;
   // para que el primer item tome el ancho del control
   if (plist->GetItemCount() == 0) {
      maxwidth = plist->GetSize().GetWidth();
   } else {
      // obtengo la long. mayor de los elementos visibles de la lista.
      for (int i = (int) plist->GetVisibleBegin(); i < static_cast<int>(plist->GetVisibleEnd());
            i++)
         if (maxwidth < (widthrect = GETITEMRECT( (size_t)(i) ).GetWidth())) {
            maxwidth = widthrect;
         }
   }
   // TODO Ver como eliminar
   return maxwidth -= MAX_WIDTH_SUBSTRACTION;
}

}

