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

#ifndef QUERYBUILDERPART_H_
#define QUERYBUILDERPART_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/PartContainerWidget.h"
// Includes Wx
#include "wx/event.h"
#include "wx/grid.h"
// Defines

namespace suri {
// forwards
class TablePart;
class SearchWidget;
class Table;

/** Macro para declarar el objeto de los eventos */
DECLARE_EVENT_OBJECT(QueryBuilderPartEvent);

class QueryBuilderPart : public suri::PartContainerWidget {
public:
   typedef enum {
      Greater = 1, Less = 2, Equal = 3, And = 4, Or = 5, NotEqual = 6
   } OperationType;
   /** Constructor */
   QueryBuilderPart(TablePart* pTablePart, SearchWidget* pSearchWidget,
                    std::string LastCondition);
   /** Constructor */
   QueryBuilderPart(Table* pTable,
                    std::string* pCondition);
   /** Destructor */
   virtual ~QueryBuilderPart();
   /** Creacion de Ventana  */
   virtual bool CreateToolWindow();
private:
   EVENT_OBJECT_PROPERTY(QueryBuilderPartEvent);
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(QueryBuilderPartEvent);
   /** Inicializa la ejecución de la consulta */
   virtual void OnButtonOk(wxCommandEvent &Event);
   /** Captura el evento de click sobre el boton con el operador > */
   void OnButtonGreaterOperation(wxCommandEvent &Event);
   /** Captura el evento de click sobre el boton con el operador < */
   void OnButtonLessOperation(wxCommandEvent &Event);
   /** Captura el evento de click sobre el boton con el operador = */
   void OnButtonEqualOperation(wxCommandEvent &Event);
   /** Captura el evento de click sobre el boton con el operador != */
   void OnButtonNotEqualOperation(wxCommandEvent &Event);
   /** Captura el evento de click sobre el boton con el operador AND */
   void OnButtonAndOperation(wxCommandEvent &Event);
   /** Captura el evento de click sobre el boton con el operador OR */
   void OnButtonOrOperation(wxCommandEvent &Event);
   /** Captura el evento de doble click sobre la grilla */
   void OnCellDoubleClick(wxGridEvent &Event);
   /** Captura el evento de click sobre el boton Agregar columna */
   void OnButtonAppendColumnName(wxCommandEvent &Event);
   /** Agrega un string al final del TextCtrl */
   void AppendString(OperationType operationType);
   /** Agrega un string al final del TextCtrl */
   void AppendColumnName(std::string ColumnName);
   TablePart* pTablePart_; /* Puntero a Table */
   SearchWidget* pSearchWidget_; /* Puntero a SearchWidget */
   std::string lastCondition_; /** Guarda la ultima condicion ejecutada */
   std::string* pCondition_;
   Table* pTable_;
};

} /** namespace suri */

#endif /* QUERYBUILDERPART_H_ */
