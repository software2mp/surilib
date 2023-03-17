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

#ifndef ADDCOLUMNPART_H_
#define ADDCOLUMNPART_H_

// Includes Estandar
// Includes Suri
#include "suri/DialogWidget.h"
#include "suri/Table.h"
// Includes Wx
#include "wx/event.h"
// Defines

namespace suri {

// forwards
class TablePart;
class TableTool;

/** Macro para declarar el objeto de los eventos */
DECLARE_EVENT_OBJECT(AddColumnPartEvent);

class AddColumnPart : public DialogWidget {
public:
   /** Constructor */
   AddColumnPart(TableTool* pTableTool, TablePart* pTablePart);
   /** Destructor */
   virtual ~AddColumnPart();
   /** Creacion de Ventana  */
   virtual bool CreateToolWindow();
private:
   EVENT_OBJECT_PROPERTY(AddColumnPartEvent);
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(AddColumnPartEvent);
   /** Accion de verificar la ecuacion ingresada en el TextCtrl */
   void OnButtonVerifyEquation(wxCommandEvent &Event);
   /** Accion de cambiar la opcion del choice */
   void OnChoiceSelection(wxCommandEvent &Event);
   /** Inicializa los tipos de dato de wxChoice */
   void SetChoiceDataTypes();
   /** Inserta la nueva columna */
   void OnButtonOk(wxCommandEvent &Event);
   /** Evento click sobre Boton Cancel */
   void OnButtonCancel(wxCommandEvent &Event);
   /** Agrega una columna a la tabla */
   void AddColumn();
   /** Agrega una columna calculada a la tabla */
   void AddCalculatedColumn();
   /** Muestra un mensaje informando el estado del parseo de la ecuacion */
   void ShowStatusMessage(bool success);
   /** Obtiene el icono del part */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
   /** Puntero a TableTool */
   TableTool* pTableTool_;
   /** Puntero a TablePart */
   TablePart* pTablePart_;
   /** Nombre de la columna */
   wxString columnname_;
   /** Tipo de columna */
   Table::ColumnType type_;
   /** Retorna la validez del nombre */
   bool validname_;
   /** Retorna los permisos de append */
   bool append_;
   /** Retorna los permisos de insert */
   bool insert_;
};

} /** namespace suri */

#endif /* ADDCOLUMNPART_H_ */
