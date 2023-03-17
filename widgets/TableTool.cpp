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
#include <string>
#include <vector>
#include <cstddef>
#include <iostream>
#include <algorithm>

// Includes Suri
#include "wxmacros.h"
#include "resources.h"
#include "TableTool.h"
#include "TablePart.h"
#include "DefaultTable.h"
#include "suri/Table.h"
#include "suri/Command.h"
#include "suri/messages.h"
#include "suri/GenericTool.h"
#include "suri/ToolCommand.h"
#include "suri/Configuration.h"
#include "AddColumnPart.h"
#ifdef __SHOW_TABLE_LOG__
#include "BufferedDriver.h"
#endif

// Includes Wx
#include "wx/dynarray.h"

// Defines

// forwards
class ProjectTool;

namespace suri {

/**
 * Constructor
 */
TableTool::TableTool(DataViewManager* pDataViewManager, TablePart* pTablePart) :
      GenericTool(pDataViewManager), pTablePart_(pTablePart), pCloseVectorFunction_(NULL) {
   configurationVariableName_ = "basic_table_editor";
   Initialize();
}

/**
 * Destructor
 */
TableTool::~TableTool() {
   Finalize();
}

/**
 * Obtener la tabla
 * @return
 */
Table* TableTool::GetTable() {
   if (pTablePart_ == NULL) {
      return NULL;
   }
   return pTablePart_->GetTable();
}

/**
 * Setea el TablePart
 * @param pTablePart
 */
void TableTool::SetTablePart(TablePart* pTablePart) {
   pTablePart_ = pTablePart;
}

/**
 * Getea el TablePart
 * @return
 */
TablePart* TableTool::GetTablePart() {
   return pTablePart_;
}

/**
 * Setea el puntero a funcion de cerrar vector
 * @param pCloseVectorFunction
 */
void TableTool::SetCloseVectorFunction(void (*pCloseVectorFunction)()) {
   pCloseVectorFunction_ = pCloseVectorFunction;
}

/**
 * Obtener indice de seleccion de Grid
 * @return
 */
std::vector<int> TableTool::GetSelectedColumns() {
   Table* ptable = GetTable();
   std::vector<int> selection;
   if (ptable != NULL) {
      wxArrayInt array = pTablePart_->GetSelectedColumns();
      size_t count = array.GetCount();
      for (size_t i = 0; i < count; i++) {
         int column = array.Item(i);
         selection.push_back(column);
      }
   }
   return selection;
}

/**
 * Obtener indice de seleccion de Grid
 * @return
 */
std::vector<int> TableTool::GetSelectedRows() {
   // Se hace el dynamic cast porque solo se soporta la tabla por defecto
   Table* ptable = GetTable();
   std::vector<int> selection;
   if (ptable != NULL) {
      wxArrayInt array = pTablePart_->GetSelectedRows();
      size_t count = array.GetCount();
      for (size_t i = 0; i < count; i++) {
         int row = array.Item(i);
         selection.push_back(row);
      }
   }
   return selection;
}

/**
 * Accion de "Ejecutar" de AddColumn
 */
void TableTool::ExecuteAddColumn() {
   AddColumnPart* paddcolumn = new AddColumnPart(this, pTablePart_);
   paddcolumn->CreateTool();
   paddcolumn->ShowModal();
}

/**
 * Accion de "Ejecutar" de DeleteColumn
 */
void TableTool::ExecuteDeleteColumn() {
   if (pTablePart_ == NULL) {
      return;
   }
   if (!pTablePart_->GetTable()->GetPermissionList()->IsOperationPermitted(
         pTablePart_->GetTable()->GetPermission(), PermissionList::DELCOL)) {
      return;
   }

   std::vector<int> columns = GetSelectedColumns();
   for (int i = columns.size() - 1; i >= 0; i--) {
      int column = columns[i];
      // Si la columna es de solo lectura no se puede borrar
      std::string name = pTablePart_->GetTable()->GetColumnName(
            pTablePart_->GetFixedColumnIndex(column));
      if (!pTablePart_->IsReadOnlyColumn(name))
         GetTable()->DeleteColumn(pTablePart_->GetFixedColumnIndex(column));
   }
}

/**
 * Accion de "Ejecutar" de AddRow
 */
void TableTool::ExecuteAddRow() {
   if (pTablePart_ == NULL) {
      return;
   }

   bool append = pTablePart_->GetTable()->GetPermissionList()->IsOperationPermitted(
         pTablePart_->GetTable()->GetPermission(), PermissionList::APPROW);

   bool insert = pTablePart_->GetTable()->GetPermissionList()->IsOperationPermitted(
         pTablePart_->GetTable()->GetPermission(), PermissionList::INSROW);

   if (!append && !insert) {
      return;
   }

   std::vector<int> rows = GetSelectedRows();
   if (insert) {
      int size = rows.size();
      if (size == 0) {
         GetTable()->AppendRow();
      } else if (size == 1) {
         int row = rows[0];
         GetTable()->InsertRow(row);
      }
   } else {
      GetTable()->AppendRow();
   }
}

/**
 * Accion de "Ejecutar" de DeleteRow
 */
void TableTool::ExecuteDeleteRow() {
   if (pTablePart_ == NULL) {
      return;
   }
   if (!pTablePart_->GetTable()->GetPermissionList()->IsOperationPermitted(
         pTablePart_->GetTable()->GetPermission(), PermissionList::DELROW)) {
      return;
   }

   std::vector<int> rows = GetSelectedRows();
   std::sort(rows.begin(), rows.end());
   for (int ix = rows.size() - 1; ix >= 0; --ix) {
      GetTable()->DeleteRow(rows[ix]);
   }
}

/**
 * Accion de "Ejecutar" de Commit
 */
void TableTool::ExecuteCommit() {
   if (pTablePart_ == NULL) {
      return;
   }

#ifdef __SHOW_TABLE_LOG__
   DefaultTable* ptable = dynamic_cast<DefaultTable*>(GetTable());
   BufferedDriver* pdriver =
         dynamic_cast<BufferedDriver*>(const_cast<DriverInterface*>(ptable->GetDriver()));

   if (ptable) {
      std::string table = ptable->GetTable();
      std::cout << "PRE COMMIT" << std::endl;
      std::cout << "--- ------" << std::endl;
      std::cout << "Table:"<< std::endl << table << std::endl;
      if (pdriver) {
         std::cout << "Buffer:" << std::endl;
         std::cout << "Operations:\t" << pdriver->GetOperations() << std::endl;
         std::cout << "Col Indexs:\t" << pdriver->GetColumnIndex() << std::endl;
         std::cout << "Row Indexs:\t" << pdriver->GetRowIndex() << std::endl << std::endl;
         std::cout << "MemoryDriver:" << std::endl << pdriver->GetSubDriverString() << std::endl;
      }
   }
#endif

   GetTable()->Commit();
   // GetTable()->UpdateGrid();

#ifdef __SHOW_TABLE_LOG__
   if (ptable) {
      std::string table = ptable->GetTable();
      std::cout << "POST COMMIT" << std::endl;
      std::cout << "---- ------" << std::endl;
      std::cout << "Table:"<< std::endl << table << std::endl;
      if (pdriver) {
         std::cout << "Buffer:" << std::endl;
         std::cout << "Operations:\t" << pdriver->GetOperations() << std::endl;
         std::cout << "Col Indexs:\t" << pdriver->GetColumnIndex() << std::endl;
         std::cout << "Row Indexs:\t" << pdriver->GetRowIndex() << std::endl << std::endl;
         std::cout << "MemoryDriver:" << std::endl << pdriver->GetSubDriverString() << std::endl;
      }
   }
#endif
}

/**
 * Accion de "Ejecutar" de Rollback
 */
void TableTool::ExecuteRollback() {
   if (pTablePart_ == NULL) {
      return;
   }

   GetTable()->Rollback();
   // GetTable()->UpdateGrid();
}

/**
 * Accion de "Ejecutar" de Cerrar Vector
 */
void TableTool::ExecuteCloseVector() {
   if (IsReadOnly()) {
      ExecuteRollback();
      pCloseVectorFunction_();
   } else {
      switch (SHOWQUERY(NULL, _(question_APPLY_CHANGES), wxYES|wxNO|wxCANCEL|wxICON_QUESTION)) {
         case wxID_YES:
            ExecuteCommit();
            pCloseVectorFunction_();
            break;
         case wxID_NO:
            ExecuteRollback();
            pCloseVectorFunction_();
            break;
         case wxCANCEL:
            break;
      }
   }
}

/** Accion de limpiar la seleccion de la tabla*/
void TableTool::ExecuteClearSelection() {
   pTablePart_->ClearSelection();
}

/** Accion de seleccion de todos los registros de la tabla*/
void TableTool::ExecuteSelectAll() {
   pTablePart_->SelectAll();
}

/** Accion de invertir la seleccion de la tabla*/
void TableTool::ExecuteInvertSelection() {
   pTablePart_->InvertSelection();
}

/** Accion de armado de consultas*/
void TableTool::ExecuteQueryBuilder() {
   pTablePart_->QueryBuilder();
}

/**
 * Si el comando indicado, se encuentra entre la lista de ejecutables, se ejecutara
 * @param[in]  pToolCommand: Comando a ejecutar
 */
void TableTool::Execute(const Command* pToolCommand) {
   int id = pToolCommand->GetId();
   if (pTablePart_->GetTable()->GetTableReadOnlyState()
         && id != GetId("QueryBuilder")) {
      SHOW_ERROR(message_ERROR_EDITING_READ_ONLY_TABLE);
      return;
   }
   if (id == GetId("AddColumn")) {
      ExecuteAddColumn();
   } else if (id == GetId("DeleteColumn")) {
      ExecuteDeleteColumn();
   } else if (id == GetId("AddRow")) {
      ExecuteAddRow();
   } else if (id == GetId("DeleteRow")) {
      ExecuteDeleteRow();
   } else if (id == GetId("SaveVector")) {
      ExecuteCommit();
   } else if (id == GetId("CloseVector")) {
      ExecuteCloseVector();
   } else if (id == GetId("ClearSelection")) {
      ExecuteClearSelection();
   } else if (id == GetId("SelectAll")) {
      ExecuteSelectAll();
   } else if (id == GetId("InvertSelection")) {
      ExecuteInvertSelection();
   } else if (id == GetId("QueryBuilder")) {
      ExecuteQueryBuilder();
   }
}

/**
 * Fabrica el comando si es uno de los predefinidos
 * @param[in]  CommandName: Nombre del comando a ejecutar
 * @return     Devuelve el comando creado si tuvo exito, sino NULL
 */
Command* TableTool::CreateCommand(const std::string &CommandName) {
   // TODO(Sebastian - TCK #2218): Revisar a donde se direccionan los comandos
   Command* pcommand = NULL;
   if (CommandName == "AddColumn") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(tooltip_TABLE_ADD_COL);
      pcommand->SetDecoration(tool_TABLE_ADD_COL);
   } else if (CommandName == "DeleteColumn") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(tooltip_TABLE_DEL_COL);
      pcommand->SetDecoration(tool_TABLE_DEL_COL);
   } else if (CommandName == "AddRow") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(tooltip_TABLE_ADD_ROW);
      pcommand->SetDecoration(tool_TABLE_ADD_ROW);
   } else if (CommandName == "DeleteRow") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(tooltip_TABLE_DEL_ROW);
      pcommand->SetDecoration(tool_TABLE_DEL_ROW);
   } else if (CommandName == "SaveVector") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(tooltip_SAVE);
      pcommand->SetDecoration(button_SAVE_VECTOR);
   } else if (CommandName == "CloseVector") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(tooltip_CLOSE);
      pcommand->SetDecoration(button_CLOSE_VECTOR);
   } else if (CommandName == "ClearSelection") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(tooltip_CLEAR_SELECTION);
      pcommand->SetDecoration(button_CLEAR_SELECTION);
   } else if (CommandName == "SelectAll") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(tooltip_SELECT_ALL);
      pcommand->SetDecoration(button_SELECT_ALL);
   } else if (CommandName == "InvertSelection") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(tooltip_INVERT_SELECTION);
      pcommand->SetDecoration(button_INVERT_SELECTION);
   } else if (CommandName == "QueryBuilder") {
#ifdef __UNUSED_CODE__    //de esto ahora se encarga SearchWidget
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(tooltip_QUERY_BUILDER);
      pcommand->SetDecoration(button_QUERY_BUILDER);
#endif
   }
   if (pcommand != NULL) {
      pcommand->SetLocation("ID_TABLE_EDITOR_TOOLBAR_PANEL");
   }
   return pcommand;
}

/**
 * Inicializa el estado de Enable y Active
 * @param CommandName
 */
void TableTool::InitializeState(const std::string &CommandName) {
   Command* pcommand = GetCommand(GetId(CommandName));
   if (pTablePart_ == NULL || pTablePart_->GetTable() == NULL) {
      pcommand->SetEnabled(true);
      pcommand->SetActive(false);
   } else {
      Table* ptable = pTablePart_->GetTable();
      int permission = ptable->GetPermission();
      const PermissionList* plist = ptable->GetPermissionList();
      if (CommandName == "AddColumn") {
         bool appcol = plist->IsOperationPermitted(permission, PermissionList::APPCOL);
         bool inscol = plist->IsOperationPermitted(permission, PermissionList::INSCOL);
         if (!appcol && !inscol) {
            pcommand->SetEnabled(false);
            pcommand->SetActive(false);
         }
      } else if (CommandName == "DeleteColumn") {
         bool delcol = plist->IsOperationPermitted(permission, PermissionList::DELCOL);
         if (!delcol) {
            pcommand->SetEnabled(false);
            pcommand->SetActive(false);
         }
      } else if (CommandName == "AddRow") {
         bool approw = plist->IsOperationPermitted(permission, PermissionList::APPROW);
         bool insrow = plist->IsOperationPermitted(permission, PermissionList::INSROW);
         if (!approw && !insrow) {
            pcommand->SetEnabled(false);
            pcommand->SetActive(false);
         }
      } else if (CommandName == "DeleteRow") {
         bool delrow = plist->IsOperationPermitted(permission, PermissionList::DELROW);
         if (!delrow) {
            pcommand->SetEnabled(false);
            pcommand->SetActive(false);
         }
      } else if (CommandName == "SaveVector") {
         bool setval = plist->IsOperationPermitted(permission, PermissionList::SETVAL);
         pcommand->SetEnabled(setval);
         pcommand->SetActive(setval);
      } else if (CommandName == "CloseVector") {
         pcommand->SetEnabled(true);
         pcommand->SetActive(true);
      }
   }
}


/**
 * Destruye un comando previamente creado con CreateCommand
 * @param[in]  pCommand: Puntero a un Comando creado no NULL a destruir
 */
void TableTool::DestroyCommand(Command* &pCommand) const {
   delete pCommand;
   pCommand = NULL;
}

/**
 * Creador del Widget
 * @param Widget
 * @return
 */
Widget* TableTool::CreateWidget(const std::string &Widget) {
   // TODO(Sebastian - TCK #878): Deteccion de codigo sin implementar
   return NULL;
}

/**
 * Verifica si la tabla es de solo lectura.
 *
 * @return true si es de solo lectura.
 */
bool TableTool::IsReadOnly() {
   if (pTablePart_ == NULL || pTablePart_->GetTable() == NULL)
      return true;

   Table* ptable = pTablePart_->GetTable();
   int permission = ptable->GetPermission();
   const PermissionList* plist = ptable->GetPermissionList();
   return !plist->IsOperationPermitted(permission, PermissionList::SETVAL);
}

} /** namespace suri */
