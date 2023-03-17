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

#ifndef WIDGET_H_
#define WIDGET_H_

// Includes standard
#include <cstddef>
#include <set>

// Includes Suri
#include "wxmacros.h"
#include "suri/ControlArrangement.h"

// Forwards para no tener que incluir mas cosas de wx
class wxWindow;
class wxTopLevelWindow;
class wxBitmap;
class wxString;
class wxWindowDestroyEvent;

// ------------------------------------------------------------------------------
#ifdef __UNUSED_CODE__
#  ifdef __DEBUG__
#     define __SHOW_WIDGET_FIT_BUTTON__
#     define __SHOW_WIDGET_LAYOUT_BUTTON__
#  endif
#endif

/** Namespace para los objetos creados por SUR */
/*!
 *  En este namespace se agrupan los objetos basicos de ventanas/procesamiento/
 * etc.
 */
namespace suri {
/** Declara la clase ToolEventHandler */
DECLARE_EVENT_OBJECT(ToolEventHandler);

// forwards
class MouseEventInterface;
class PaintEventInterface;
class CommandEventInterface;

/** clase base de herramientas */
/*!
 *  Esta clase abstracta sirve como soporte para el desarrollo de ventanas
 * utilizando XRCs u otros mecanismos.
 *
 *  Esta clase crea un wxPanel (en la ventana derivada) y en caso de no
 * asignarse ninguna ventana "padre", un wxFrame que contiene al panel.
 *
 *  Esta clase mantiene una ventana y/o panel. En caso de cierre o
 * destruccion desde el codigo de la ventana y/o el panel, se destruira
 * la herramienta asociada. La ventana y/o panel se cerrara en caso de
 * la destruccion de la herramienta (delete tool;). Esto permite
 * agregar la ventana asociada a otras ventanas y delegar la
 * responsabilidad de la destruccion.
 *
 * La utilizacion es la siguiente:
 *  - derivar una clase
 *  - Implementar CreateToolWindow() con el codigo que crea un wxPanel con
 *    los controles. En este metodo, asignar a pToolWindow_ dicho wxPanel.
 *  - Setear wantedWidth_ y wantedHeight_ al minimo tamanio posible que
 *    se desee que tenga la ventana (en caso de wxParent==NULL).
 *  - Implementar GetWindowTitle()const que retorne el nombre de la ventana.
 *  - Se puede overlodear GetWindowIcon() para que retorne un icono (para el
 *    wxFrame asociado en caso de que pParent==NULL)
 *  - Para responder a los eventos de la ventana asociada, se debe crear una
 *    clase derivada de wxEventHandler. Agregar las macros en la
 *    declaracion de la clase:
 *     - DECLARE_EVENT_OBJECT en el .h antes de la declaracion de la clase
 *       derivada.
 *     - FRIEND_EVENT_OBJECT en la parte publica de la clase
 *     - EVENT_OBJECT_PROPERTY en la parte privada de la clase
 *     El parametro de las macros es el nombre de la clase que maneja los
 *     eventos (ej. ToolEventHandler) y el nombre de la propiedad es
 *     pEventHandler_.
 *    Utilizar las siguientes macros en el .cpp para implementar la clase
 *    y conectar/desconectar los eventos:
 *     - START_IMPLEMENT_EVENT_OBJECT y END_IMPLEMENT_EVENT_OBJECT para
 *       encerrar los callbacks.
 *     - IMPLEMENT_EVENT_CALLBACK para implementar el callback.
 *     - CONNECT_TOOL_EVENT en el codigo de la clase para conectar un evento
 *       a la propiedad pEventHandler_.
 *     - DISCONNECT_TOOL_EVENT para desconectar el evento de la propiedad.
 *  - Crear una instancia de la clase derivada.
 *  - Llamar al metodo CreateTool()
 * \attention Las clases que heredan de Widget no deben ser creadas en el
 *     stack ya que realizan la operacion delete this; ante la destruccion
 *     de la ventana asociada.
 * \attention La ventana asociada es eliminada por la biblioteca wxWidgets
 *     y solo en situaciones extraordinarias debe ser destruida por el codigo
 *     cliente.
 */
class Widget {
   /** Ctor de copia */
   Widget(const Widget &Widget);
public:
   /** Declara friend la clase de evento ToolEventHandler */
   FRIEND_EVENT_OBJECT(ToolEventHandler);
   /** Declara friend a la clase Part, remover cuando cortemos la herencia Part->Widget */
   friend class Part;
   /** Ctor */
   Widget(wxWindow *pParent = NULL);
   /** Dtor */
   virtual ~Widget();

   /** Crea la herramienta */
   virtual bool CreateTool(wxTopLevelWindow *pParentFrame = NULL, bool Show = true);
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow()=0;
   /** Obtiene el titulo del frame que va a crear */
   const wxString GetWindowTitle() const {
      return _(windowTitle_);
   }

   /** establece el titulo de la ventana */
   virtual void SetWindowTitle(const wxString& WindowTitle);

   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
   /** Regresa un puntero a la ventana de la herramienta */
   wxWindow *GetWindow();
   /** Retorna el frame dentro del cual se encuentra la ventana */
   virtual wxTopLevelWindow *GetFrame() const;
   /** Agrega Componentes */
   virtual bool AddControl(Widget *pTool, const wxString &ParentControl);
   /** Agrega Componentes */
   virtual bool AddControl(Widget *pTool, const wxString &ParentControl,
                           const ControlArrangement &Arrangement);
   /** Agrega Componentes */
   virtual bool AddControl(Widget *pTool, wxWindow *pParentControl);
   /** Agrega Componentes */
   virtual bool AddControl(Widget *pTool, wxWindow *pParentControl,
                           const ControlArrangement &Arrangement);
   /** Elimina Componentes */
   virtual bool RemoveControl(Widget *pTool);

   /** Muestra el Widget ppal */
   bool Show();
   /** Muestra la herramienta que se indica en WidgetId */
   bool Show(const std::string &WidgetId, bool Show = true);
   /** Oculta el Widget ppal */
   bool Hide();
   /** Oculta la herramienta que se indica en WidgetId */
   bool Hide(const std::string &WidgetId);
   /** Habilita la herramienta que se indica en WidgetId */
   bool Enable();
   /** Habilita la herramienta que se indica en WidgetId */
   bool Enable(const std::string &WidgetId, bool Enabled = true);
   /** Inhabilita Widget ppal */
   bool Disable();
   /** Inhabilita la herramienta que se indica en WidgetId */
   bool Disable(const std::string &WidgetId);
   /** Marca el "foco" en el miniframe */
   virtual void Focus(bool Focused = true);

// ------------------------ EVENTOS -------------------------------------------
   /** Agrega un handler para eventos de tipo mouse sobre pToolWindow_ */
   virtual void PushMouseEvent(MouseEventInterface *pMouseEvent);
   /** Elimina el handler de eventos de tipo mouse */
   virtual MouseEventInterface *PopMouseEvent(MouseEventInterface *pMouseEvent = NULL);
   /** Agrega un handler para eventos de tipo paint sobre pToolWindow_ */
   virtual void PushPaintEvent(PaintEventInterface *pPaintEvent, bool DeepSearch = true);
   /** Elimina el handler de eventos de tipo paint */
   virtual PaintEventInterface *PopPaintEvent(PaintEventInterface *pPaintEvent = NULL);
   /** Agrega un handler de eventos command */
   virtual void PushCommandEvent(CommandEventInterface *pCommandEvent);
   /** Elimina el handler de eventos command */
   virtual CommandEventInterface *PopCommandEvent(CommandEventInterface *pCommandEvent = NULL);

protected:
   /** Crea una ventana simple para la herramienta */
   virtual bool CreateSimpleToolWindow(wxTopLevelWindow *pParentFrame = NULL);
   /** crea el wxTopLevelWindow */
   virtual wxTopLevelWindow* CreateContainingWindow(wxTopLevelWindow *pParentFrame);
   /** Agrega Componentes */
   virtual bool AddControl(wxWindow *pToolWindow, wxWindow *pParentControl);
   /** Agrega Componentes */
   virtual bool AddControl(wxWindow *pToolWindow, wxWindow *pParentControl,
                           const ControlArrangement &Arrangement);
   /** Elimina Componentes */
   virtual bool RemoveControl(wxWindow *pControlWindow);
   /** Evento de salida de mouse de la ventana */
   virtual void OnMouseLeave();
   /** Determina si es valido */
   static bool IsValid(Widget *pWidget);
   /** Muestra la ventana */
   bool Show(wxWindow *pWindow, bool Show);
   /** Habilita la Ventana */
   bool Enable(wxWindow *pWindow, bool Enabled = true);
	/** Metodo polimorfico que se llama cuando se hace foco sobre un Widget **/
	virtual void DoOnFocus(bool Focused);
#ifdef __WXMSW__
/** Recorre los hijos de una ventana actualizando los tamanios.*/
void FixSizers(wxWindow* pWindow, const std::string &Spaces);
#endif

   wxWindow *pToolWindow_; /*! Puntero a la ventana de la herramienta */
   wxWindow *pParentWindow_; /*! Puntero a la ventana padre */
   int wantedWidth_; /*! Ancho prefereido de ventana */
   int wantedHeight_; /*! Alto preferido de ventana */
   wxString windowTitle_; /*! Titulo de la ventana */
   bool isAutonomous_; /*! Indica si se creo un wxFrame para el Tool */
   wxTopLevelWindow *pToolFrame_; /*! Frame donde se encuentra la ventana */
   bool createWithMiniframe_; /*! agregar un miniframe cuando se crea la ventana */
   wxWindow *pMiniFrame_; /*! frame con la decoracion estilo ventana */
private:
   /** Para autodestruirse */
   void OnDestroy(wxWindowDestroyEvent &Event);
   /** Elimina los widgets que pasaron por OnDestroy() */
   static bool DeleteWidgets();

// ------------------------------------------------------------------------------
#ifdef __SHOW_WIDGET_FIT_BUTTON__
   /** Boton Fit en los widgets creados con la ventana padre (pParentWindow_ == NULL) */
   void OnFitButton();
#endif
#ifdef __SHOW_WIDGET_LAYOUT_BUTTON__
   /** Boton Fit en los widgets creados con la ventana padre (pParentWindow_ == NULL) */
   void OnLayoutButton();
#endif

   static std::set<Widget*> destroyedWidgets_; /*! conjunto de widgets a destruir */
   static std::set<Widget*> validWidgets_; /*! conjunto de widgets a destruir */
   bool isExiting_; /*! Flag que indica si esta saliendo */
   EVENT_OBJECT_PROPERTY(ToolEventHandler); /*! Objeto para conectar eventos */
};

}  // namespace suri

#endif /*WIDGET_H_*/

