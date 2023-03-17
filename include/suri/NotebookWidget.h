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

#ifndef NOTEBOOKWIDGET_H_
#define NOTEBOOKWIDGET_H_

// Include suri
#include "PartContainerWidget.h"

// Defines
/**
 * Con este define se evita un bug que pasa cuando se dispara el evento
 * PAGE_CHANGING, que despues de un veto dispara el mismo evento. Si
 * PREVENT_EVENT_REPETITION esta definido se agregan dos flags que permiten
 * bloquear la segunda ocurrencia del mismo evento.
 * \note la repeticion del evento despues de un veto pasa en windows, no en linux
 * \todo ver por que se dispara el evento despues de vetarlo.
 */
#ifdef __WXMSW__
#define PREVENT_EVENT_REPETITION
#endif

/** namespace suri */
namespace suri {

/** Declara la clase NotebookWidgetEvent */
DECLARE_EVENT_OBJECT(NotebookWidgetEvent);

/** PartContainerWidget que usa wx(Note|Tool|List)book para mostrar Parts */
/**
 *  Determina que herramienta wxWidgets se va a utlizar para mostrar los
 *  datos del PartContainerWidget.
 *  Implementa los metodos virtuales para agregar las partes a la herramienta elegida.
 *  \note especializa PartContainerWidget para definir su interfaz grafica
 */
class NotebookWidget : public PartContainerWidget {
public:
   /** Declara friend la clase de evento NotebookWidgetEvent */
   FRIEND_EVENT_OBJECT(NotebookWidgetEvent);
   /** Tipos de books */
   typedef enum {
      Notebook, Listbook, Toolbook, Choicebook, Treebook
   } BookType;
   /** Constructor */
   NotebookWidget(const wxString &Title, const long &Style = wxNB_LEFT, BookType Type = Listbook,
                  const long VisibleButtons = SUR_BTN_ALL, const long EnabledButtons = SUR_BTN_ALL);
   /** Dtor. */
   virtual ~NotebookWidget();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
   /** Evento click sobre Boton Aplicar */
   virtual void OnButtonApply(wxCommandEvent &Event);
   /** Evento click sobre Boton BackWard */
   virtual void OnButtonBackward(wxCommandEvent &Event);
   /** Evento click sobre Boton ForWard */
   virtual void OnButtonForward(wxCommandEvent &Event);
   /** Evento UIUpdate */
   virtual void OnUIUpdate(wxUpdateUIEvent &Event);
   // Separo las llamadas en la conexion de eventos
#ifdef __UNUSED_CODE__
   /** Evento Notebook modificado */
   virtual void OnNotebookChange(wxNotebookEvent &Event) {
      OnBookCtrl(Event);
   }
   /** Evento Listbook modificado */
   virtual void OnListbookChange(wxListbookEvent &Event) {
      OnBookCtrl(Event);
   }
   /** Evento generico para los book */
   virtual void OnBookCtrl(wxBookCtrlBaseEvent &Event);
#endif   // __UNUSED_CODE__
   /** Evento que se dispara cuando se esta cambiando de pagina del book */
   virtual void OnPageChanging(wxBookCtrlBaseEvent &Event);
   /** Evento que se dispara cuando se cambio de pagina del book */
   virtual void OnPageChanged(wxBookCtrlBaseEvent &Event);
   /** El evento no se propaga */
   virtual void OnChildFocus(wxChildFocusEvent &Event);
   /** Retorna la ventana padre de las partes. */
   virtual wxWindow *GetPartParent();

protected:
   /** Agrega Part al Notebook. */
   virtual bool DoInsert(suri::Part *pPart);
   /** Elimina Part del Notebook. */
   virtual bool DoRemove(suri::Part *pPart);
   /** Modifica los iconos de las partes segun estan habilitadas o no */
   void SetPartsIconsState();
   /** Acomoda los textos de las partes para que haya newlines */
   wxString WrapTitle(const wxString &Title) const;
   wxBookCtrlBase *pBookCtrl_; /*! Puntero al notebook que almacena los Part's. */
   long notebookStyle_; /*! Estilo del Notebook.(modifica la pos. de los tab.) */
#ifdef PREVENT_EVENT_REPETITION
   bool eventVetoedInApplyChangesQuestion_; /*! Flag para cancelar repeticion de eventos */
   bool eventVetoedInInvalidDataQuestion_; /*! Flag para cancelar repeticion de eventos */
   bool buttonEvent_; /*! Flag que se activa en caso que se usen los botones siguiente/anterior. */
#endif
   BookType type_; /*! Tipo de book */

private:
   /** Ctor. de Copia. */
   NotebookWidget(const NotebookWidget &NotebookWidget);
   EVENT_OBJECT_PROPERTY(NotebookWidgetEvent); /*! Objeto para conectar eventos */
};
} /** namespace suri */

#endif /*NOTEBOOKWIDGET_H_*/
