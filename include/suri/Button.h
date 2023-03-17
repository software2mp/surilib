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

#ifndef BUTTON_H_
#define BUTTON_H_

// Include suri
#include "suri/Widget.h"
#include "suri/LayerList.h"

// Defines
/** Tamanio (en pixel) del boton */
#define BUTTON_SIZE 24

/** namespace suri */
namespace suri {
// Forwards
class ViewerWidget;

// Defines para los eventos que  se producen sobre la ventana del viewer y no
// sobre el boton en si.
/** Flag para no conectar eventos del mouse */
#define BUTTON_NULL                 0x00000000
/** Flag para conectar evento LeftUp del mouse */
#define BUTTON_LEFT_UP              0x00000001
/** Flag para conectar evento LeftDown del mouse */
#define BUTTON_LEFT_DOWN            0x00000002
/** Flag para conectar evento LeftDoubleClick del mouse */
#define BUTTON_LEFT_DOUBLECLICK     0x00000004
/** Flag para conectar todos los eventos del boton izquierdo del mouse */
#define BUTTON_LEFT                 (BUTTON_LEFT_UP | BUTTON_LEFT_DOWN |      \
                                     BUTTON_LEFT_DOUBLECLICK)
/** Flag para conectar evento RightUp del mouse */
#define BUTTON_RIGHT_UP             0x00000008
/** Flag para conectar evento RightDown del mouse */
#define BUTTON_RIGHT_DOWN           0x00000010
/** Flag para conectar evento RightDoubleClick del mouse */
#define BUTTON_RIGHT_DOUBLECLICK    0x00000020
/** Flag para conectar todos los eventos del boton derecho del mouse */
#define BUTTON_RIGHT                (BUTTON_RIGHT_UP | BUTTON_RIGHT_DOWN |    \
                                     BUTTON_RIGHT_DOUBLECLICK)
/** Flag para conectar evento MiddleUp del mouse */
#define BUTTON_MIDDLE_UP            0x00000040
/** Flag para conectar evento MiddleDown del mouse */
#define BUTTON_MIDDLE_DOWN          0x00000080
/** Flag para conectar evento MiddleDoubleClick del mouse */
#define BUTTON_MIDDLE_DOUBLECLICK   0x00000100
/** Flag para conectar todos los eventos del boton del medio del mouse */
#define BUTTON_MIDDLE               (BUTTON_MIDDLE_UP | BUTTON_MIDDLE_DOWN |  \
                                     BUTTON_MIDDLE_DOUBLECLICK)
/** Flag para conectar todos los eventos de todos los botones del mouse */
#define BUTTON_ANY_BUTTON           (BUTTON_LEFT | BUTTON_RIGHT | BUTTON_MIDDLE)
/** Flag para conectar el evento entrar a la ventana con el mouse */
#define BUTTON_ENTER                0x00000200
/** Flag para conectar el evento salir de la ventana con el mouse */
#define BUTTON_LEAVE                0x00000400
/** Flag para conectar el evento moverse con el mouse */
#define BUTTON_MOVE                 0x00000800
/** Flag para conectar el evento scroll con el mouse */
#define BUTTON_WHEEL                0x00001000
/** Flag para conectar todos los eventos del mouse */
#define BUTTON_MOUSE                (BUTTON_ANY_BUTTON |                      \
                                     BUTTON_ENTER | BUTTON_LEAVE |             \
                                     BUTTON_MOVE | BUTTON_WHEEL)
/** Flag para conectar el evento paint */
#define WINDOW_PAINT                0x00002000

/** Macro para declarar el objeto de los eventos */
DECLARE_EVENT_OBJECT(ButtonEvent);

/** Widget herramienta tipo boton de los viewers */
/**
 * Clase base de los botones que encapsula el manejo de eventos. En el
 * constructor se le pasa un flag que determina los eventos que escucha, luego
 * cuando recive el evento llama al metodo virtual DoOnNombreEvento cuya logica
 * debe ser implementada por los decendientes
 * Agrega una propiedad enabled_ que sirve para habilitar el control wx.
 * Se incorpora el metodo IsEnabled que permite a cada boton determinar su
 * logica de activacion. Y el metodo OnUIUpdate que en base al resultado de
 * IsEnabled des/habilita el control wx del boton.
 */
class Button : public Widget {
   /** Ctor. de Copia. */
   Button(const Button &Button);

public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(ButtonEvent);
   /** Ctor */
   Button(wxWindow *pViewerToolbar, wxString ButtonTooltip, int Flag);
   /** Ctor */
   Button(wxWindow *pViewerToolbar, const wxBitmap &Bitmap, wxString ButtonTooltip =
         wxT(tooltip_BUTTON),
          int Flag = BUTTON_NULL);
   /** Ctor */
   Button(wxWindow *pViewerToolbar, const wxString &Bitmap, wxString ButtonTooltip =
         wxT(tooltip_BUTTON),
          int Flag = BUTTON_NULL);
   /** Dtor */
   virtual ~Button();

// Metodo de creacion del boton-herramienta
   virtual bool CreateToolWindow();
   /** setea el estado del boton */
   virtual bool SetValue(bool State = true);
   /** retorna el estado del boton */
   virtual bool GetValue() const {
      return state_;
   }
   /** setea la ventana del viewer */
   void SetViewer(ViewerWidget *pViewer);
   /** setea la lista de elementos */
   void SetElementList(LayerList *pList) {
      pList_ = pList;
   }
   /** retorna un bitmap para poner en el boton */
   wxBitmap GetBitmap();
   /** Asigna el cursor de mouse default al viewer */
   virtual void DoSetCursor(bool State);
   /** Setea la propiedad enabled_ */
   virtual void SetEnabled(bool Enabled = true);
   /** Verifica si el boton se puede o no habilitar */
   virtual bool IsEnabled();
   /** Evento que habilita el boton segun el retorno de IsEnabled */
   virtual void OnUIUpdate(wxUpdateUIEvent &Event);
   /** Devuelve el tipo de evento */
   int GetButtonEventType() {
      return eventType_;
   }
protected:
   /** Se llama cuando se setea el valor a true y estaba en false */
   virtual void OnButtonClicked(wxCommandEvent &Event) {
   }
   /** funcion {} que se remplaza si */
// \todo return void cuando se cierre el vector en vez de fallar
   virtual bool DoSetValue(bool State) {
      return true;
   }
   /** Se llama cuando se crea la herramienta */
   virtual bool DoCreateToolWindow();
   /** retorna el bitmap asociado */
   virtual wxBitmap DoGetBitmap();
   /** Agrega el handler al comienzo de la pila de handlers de ViewerWindow */
   virtual void AddHandlerToViewerWindowBeginning();
   /** Elimina el handler del comienzo de la pila */
   virtual void RemoveHandlerFromViewerWindow();
// -------------------------------- EVENTOS ---------------------------------
// Estos eventos son los que se producen sobre la ventana del viewer y no
// sobre el boton en si.
   /** Metodo que se ejecuta ante un evento de mouse sobre la ventana del viewer */
   void OnLeft(wxMouseEvent &Event);
   /** Metodo que se ejecuta ante un evento de mouse sobre la ventana del viewer */
   void OnLeftUp(wxMouseEvent &Event);
   /** Metodo que se ejecuta ante un evento de mouse sobre la ventana del viewer */
   void OnLeftDown(wxMouseEvent &Event);
   /** Metodo que se ejecuta ante un evento de mouse sobre la ventana del viewer */
   void OnLeftDoubleClick(wxMouseEvent &Event);
   /** Metodo que se ejecuta ante un evento de mouse sobre la ventana del viewer */
   void OnRight(wxMouseEvent &Event);
   /** Metodo que se ejecuta ante un evento de mouse sobre la ventana del viewer */
   void OnRightUp(wxMouseEvent &Event);
   /** Metodo que se ejecuta ante un evento de mouse sobre la ventana del viewer */
   void OnRightDown(wxMouseEvent &Event);
   /** Metodo que se ejecuta ante un evento de mouse sobre la ventana del viewer */
   void OnRightDoubleClick(wxMouseEvent &Event);
   /** Metodo que se ejecuta ante un evento de mouse sobre la ventana del viewer */
   void OnMiddle(wxMouseEvent &Event);
   /** Metodo que se ejecuta ante un evento de mouse sobre la ventana del viewer */
   void OnMiddleUp(wxMouseEvent &Event);
   /** Metodo que se ejecuta ante un evento de mouse sobre la ventana del viewer */
   void OnMiddleDown(wxMouseEvent &Event);
   /** Metodo que se ejecuta ante un evento de mouse sobre la ventana del viewer */
   void OnMiddleDoubleClick(wxMouseEvent &Event);
   /** Metodo que se ejecuta ante un evento de mouse sobre la ventana del viewer */
   void OnEnterWindow(wxMouseEvent &Event);
   /** Metodo que se ejecuta ante un evento de mouse sobre la ventana del viewer */
   void OnLeaveWindow(wxMouseEvent &Event);
   /** Metodo que se ejecuta ante un evento de mouse sobre la ventana del viewer */
   void OnWheel(wxMouseEvent &Event);
   /** Metodo que se ejecuta ante un evento de mouse sobre la ventana del viewer */
   void OnMouseMove(wxMouseEvent &Event);
   /** Metodo que se ejecuta ante un evento de mouse sobre la ventana del viewer */
   void OnMouse(wxMouseEvent &Event);
   /** Metodo que se ejecuta ante un evento de mouse sobre la ventana del viewer */
   void OnPaint(wxPaintEvent &Event);
// --------------------------- EVENTOS VIRTUALES ----------------------------
   /** Metodo virtual para las clases derivadas ante un evento de mouse */
   virtual void DoOnLeft(wxMouseEvent &Event) {
   }
   /** Metodo virtual para las clases derivadas ante un evento de mouse */
   virtual void DoOnLeftUp(wxMouseEvent &Event) {
   }
   /** Metodo virtual para las clases derivadas ante un evento de mouse */
   virtual void DoOnLeftDown(wxMouseEvent &Event) {
   }
   /** Metodo virtual para las clases derivadas ante un evento de mouse */
   virtual void DoOnLeftDoubleClick(wxMouseEvent &Event) {
   }
   /** Metodo virtual para las clases derivadas ante un evento de mouse */
   virtual void DoOnRight(wxMouseEvent &Event) {
   }
   /** Metodo virtual para las clases derivadas ante un evento de mouse */
   virtual void DoOnRightUp(wxMouseEvent &Event) {
   }
   /** Metodo virtual para las clases derivadas ante un evento de mouse */
   virtual void DoOnRightDown(wxMouseEvent &Event) {
   }
   /** Metodo virtual para las clases derivadas ante un evento de mouse */
   virtual void DoOnRightDoubleClick(wxMouseEvent &Event) {
   }
   /** Metodo virtual para las clases derivadas ante un evento de mouse */
   virtual void DoOnMiddle(wxMouseEvent &Event) {
   }
   /** Metodo virtual para las clases derivadas ante un evento de mouse */
   virtual void DoOnMiddleUp(wxMouseEvent &Event) {
   }
   /** Metodo virtual para las clases derivadas ante un evento de mouse */
   virtual void DoOnMiddleDown(wxMouseEvent &Event) {
   }
   /** Metodo virtual para las clases derivadas ante un evento de mouse */
   virtual void DoOnMiddleDoubleClick(wxMouseEvent &Event) {
   }
   /** Metodo virtual para las clases derivadas ante un evento de mouse */
   virtual void DoOnEnterWindow(wxMouseEvent &Event) {
   }
   /** Metodo virtual para las clases derivadas ante un evento de mouse */
   virtual void DoOnLeaveWindow(wxMouseEvent &Event) {
   }
   /** Metodo virtual para las clases derivadas ante un evento de mouse */
   virtual void DoOnWheel(wxMouseEvent &Event) {
   }
   /** Metodo virtual para las clases derivadas ante un evento de mouse */
   virtual void DoOnMouseMove(wxMouseEvent &Event) {
   }
   /** Metodo virtual para las clases derivadas ante un evento de mouse */
   virtual void DoOnMouse(wxMouseEvent &Event) {
   }
   /** Metodo virtual para las clases derivadas ante un evento de mouse */
   virtual void DoOnPaint(wxPaintEvent &Event) {
   }
// ------------------------- PROPIEDADES PROTEGIDAS -------------------------
   wxWindow* pViewerWindow_; /*! Ventana del viewer */
   ViewerWidget *pViewer_; /*! Viewer asociado */
   LayerList* pList_; /*! Lista de elementos */
   bool state_; /*! Estado de la herramienta */
   bool enabled_; /*! Estado para habilitar el control wx */
   int eventMask_; /*! Eventos que deseo conectar */
   wxBitmap *pBitmap_; /*! Imagen del boton */
   bool debounceLeft_; /*! Flag para saber si un Down se hizo en la ventana */
   bool isDraggingLeft_; /*! Flag que indica si estaba draggeando */
   bool insideLeft_; /*! Flag que indica si estaba draggeando */
   bool debounceMiddle_; /*! Flag para saber si un Down se hizo en la ventana */
   bool isDraggingMiddle_; /*! Flag que indica si estaba draggeando */
   bool insideMiddle_; /*! Flag que indica si estaba draggeando */
   bool debounceRight_; /*! Flag para saber si un Down se hizo en la ventana */
   bool isDraggingRight_; /*! Flag que indica si estaba draggeando */
   bool insideRight_; /*! Flag que indica si estaba draggeando */
   int eventType_; /*! Tipo de evento */
   EVENT_OBJECT_PROPERTY(ButtonEvent); /*! Objeto para conectar eventos */
private:
};
}

#endif /*BUTTONTOOL_H_*/

