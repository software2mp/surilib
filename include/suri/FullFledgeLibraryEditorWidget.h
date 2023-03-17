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
#include <string>

// Includes Suri
// Includes Wx
// Defines
#ifndef FullFledgeLibraryEditorWidget_H_
#define FullFledgeLibraryEditorWidget_H_

#include "suri/PartContainerWidget.h"
#include "suri/Part.h"
#include "suri/DialogWidget.h"
#include "LibraryItemListViewWidget.h"
#include "LibraryItemEditorPart.h"

namespace suri {

/** Declara la clase Notebook WidgetEvent */
DECLARE_EVENT_OBJECT(FullFledgeLibraryEditorWidgetEvent);

/**
 * Widget del editor de bibliotecas.
 * Posee un selector de items, para seleccionar el item
 * a editar.
 * Tiene un panel en el cual se ubica el editor especifico
 * para el tipo de item que se este deseando editar.
 *
 */
class FullFledgeLibraryEditorWidget : public suri::PartContainerWidget {
   /** Ctor. de Copia. */
   FullFledgeLibraryEditorWidget(const FullFledgeLibraryEditorWidget &);

public:
   /** Constructor **/
   FullFledgeLibraryEditorWidget(LibraryManager* pManager,
                                 const std::string& LibraryCode, 
											const std::string& WindowTitle = caption_LIBRARY);
   /** Dtor. */
   virtual ~FullFledgeLibraryEditorWidget();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
   /** Retorna la ventana padre de las partes. */
   virtual wxWindow *GetPartParent();
   /** Metodo encargado de actualizar el editor asociado al item seleccionado */
   void UpdateEditor(const LibraryItem* pItem);

private:
   /** Declara friend la clase de evento NotebookWidgetEvent */
   FRIEND_EVENT_OBJECT(FullFledgeLibraryEditorWidgetEvent);
   EVENT_OBJECT_PROPERTY(FullFledgeLibraryEditorWidgetEvent); /*! Objeto para conectar eventos */
   /** Metodo que se llama cuando se presiona el boton de agregar*/
   void OnAddClick(wxCommandEvent& Event);
   /** Metodo que se llama cuando se presiona el boton de eliminar*/
   void OnRemoveClick(wxCommandEvent& Event);
   /** Agrega Part al Notebook. */
   virtual bool DoInsert(suri::Part *pPart);
   /** Elimina Part del Notebook. */
   virtual bool DoRemove(suri::Part *pPart);
   /** Metodo encargado de verficar los cambios sobre el item que se pasa por parametro**/
   void CommitChangesFromActiveItem();
   /** Instancia del responsable de las notificaciones entre editor full-fledge
    *  y selector de items */
   LibraryItemListNotificator notificator_;
   /** Codigo de biblioteca que manejara el editor full fledge **/
   std::string libraryCode_;
   /** Widget activo para seleccion de items de biblioteca*/
   LibraryItemListViewWidget* pItemSelector_;
   /** Referencia al manager de biblioteca activo **/
   LibraryManager* pManager_;
   /** Editor de items en uso **/
   LibraryItemEditorPart* pCurrentEditor_;
   /** bool que indica que se ha elimiando un item recientemente */
   bool itemRemoved_;
   /** referencia a la biblioteca que se encuentra desplegando el editor **/
   const Library* pLibrary_;
};
}

#endif /* FullFledgeLibraryEditorWidget_H_ */
