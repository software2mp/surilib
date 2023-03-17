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
#include <map>
#include <set>
#include <string>
#include <vector>
#include <limits>
#include <iterator>

// Includes suri
#include "wxmacros.h"
#include "logmacros.h"
#include "DefaultTable.h"
#include "wxGenericTableBase.h"
#include "wxGridColorCellEditor.h"
#include "wxGridColAttrProvider.h"
#include "wxGridColorCellRenderer.h"

// Defines

// Forwards

namespace suri {

const std::string wxGenericTableBase::ReadError = "";          /*! Valor para el error de lectura */
const std::string wxGenericTableBase::Hidden = "HIDDEN";       /*! Valor para ocultos */

/**
 * Constructor
 */
wxGenericTableBase::wxGenericTableBase(Table* pTable) {
   pTable_ = pTable;
#ifdef __UNUSED_CODE__
   if (pDefaultTable) {
      pDefaultTable->SetWxGenericTableBase(this);
   }
#endif
   pTable->Attach(this);
   int cols = pTable->GetColumns();
   // Guardo los ids de las columnas iniciales
   for (int i = 0; i < cols; ++i) {
      colIndexHolder_.push_back(pTable->GetColumnId(i));
   }
   hasChangedFlag_ = false;
   UpdateProvider();
   UpdateGrid();
}

/**
 * Destructor
 */
wxGenericTableBase::~wxGenericTableBase() {
}

/**
 * Actualiza el Provider de Editores de Atributos
 * \attention Debe ser invocado en la tabla en caso de AddColumn, DelColumn, ChangeRestrictions
 */
void wxGenericTableBase::UpdateProvider() {
   UpdateGrid();
   // El provider se hace cargo de la memoria
   SetAttrProvider(new wxGridColAttrProvider(pTable_, this, pTable_->GetOptions()));
}

/**
 * Determina si hubo cambios en la tabla
 * @return bool devuelve true si hubo cambios en la tabla, false en caso contrario
 */
bool wxGenericTableBase::HasChanged() const {
   return hasChangedFlag_;
}

/**
 * Resetea el flag de cambios de la tabla
 */
void wxGenericTableBase::ResetChangesFlag() {
   hasChangedFlag_ = false;
   UpdateGrid();
}

/**
 * Retrona el valor la celda(en formato string) en posicion pedida.
 * @param[in] Row numero de fila de la celda
 * @param[in] Column numero de columna de la celda
 * @return wxString con el valor de la celda
 */
wxString wxGenericTableBase::GetValue(int Row, int Column) {
   std::string value = wxGenericTableBase::ReadError;
   int colindex = GetFixedColumnIndex(Column);
   if (colindex != std::numeric_limits<int>::max())
      pTable_->GetCellValue(colindex, Row, value);
   return value;
}

/**
 * Actualiza el valor de un campo en vector con el valor que se pasa por parametro
 * Si la columna es de solo lectura, escribe igual el valor (SoloLectura es solo para wxGrid)
 * @param[in] Row numero de fila de la celda
 * @param[in] Column numero de columna de la celda
 * @param[in] Value wxString con el valor de la celda (sera interpretado con el tipo correcto)
 */
void wxGenericTableBase::SetValue(int Row, int Column, const wxString& Value) {
   std::string stringvalue = WS2S(Value);
   int colindex = GetFixedColumnIndex(Column);
   if (colindex != std::numeric_limits<int>::max()
         && pTable_->SetCellValue(colindex, Row, stringvalue)) {
      hasChangedFlag_ = true;
      UpdateGrid();
   } else {
      SHOW_ERROR(message_READ_FILE_ERROR);
   }
}

/** Obtiene el indice de columna teniendo en cuenta las ocultas
 *  @return indice de la columna ajustado
 *  @return std::numeric_limits<int>::max() en caso contrario**/
int wxGenericTableBase::GetFixedColumnIndex(int Column) const {
   int size = colIndexHolder_.size();
   if (Column > size - 1)
      return std::numeric_limits<int>::max();
   long id = colIndexHolder_.at(Column);
   return pTable_->GetColumnIndexBy(id);
}
/**
 * Informa el numero de filas que tiene la tabla.
 * @return int numero de filas en tabla de vector
 */
int wxGenericTableBase::GetNumberRows() {
   return pTable_->GetRows();
}

/**
 * Informa el numero de columnas que tiene la tabla.
 * @return int numero de columnas que tiene la tabla
 */
int wxGenericTableBase::GetNumberCols() {
   return colIndexHolder_.size();
}

/**
 * Informa si la celda esta vacia (siempre false)
 * Solo si la tabla contiene un valor de string vacio se encuentra vacia
 * (Esto no es lo mismo que verificar un valor nulo con el que se suele inicializar la tabla)
 * @param[in] Row numero de fila de la celda
 * @param[in] Column numero de columna de la celda
 * @return bool devuelve true si hay una inconsistencia en el almacenamiento de la tabla
 */
bool wxGenericTableBase::IsEmptyCell(int Row, int Column) {
   std::string value;
   int col = GetFixedColumnIndex(Column);
   bool empty = false;
   if (col != std::numeric_limits<int>::max())
      empty = pTable_->GetCellValue(col, Row, value);
   return !empty;
}

/**
 * Retorna el nombre de una columna
 * @param[in] Column numero de columna de la celda
 * @return nombre de la columna o un string vacio si no existe la columna o no tiene nombre
 */
wxString wxGenericTableBase::GetColLabelValue(int Column) {
   int col = GetFixedColumnIndex(Column);
   wxString label;
   if (col != std::numeric_limits<int>::max())
      label = pTable_->GetColumnName(col);
   return label;
}

/**
 * Agrega filas a la tabla
 * @param[in] NumRows cantidad de filas de la celda a ser agregadas al final
 * @return devuelve true si pudo agregar las filas, false en caso contrario
 */
bool wxGenericTableBase::AppendRows(size_t NumRows) {
   if (NumRows <= 0) {
      return false;
   }
   for (size_t i = 0; i < NumRows; i++) {
      if (!pTable_->AppendRow()) {
         return false;
      }
   }
   UpdateGrid();
   return true;
}

/**
 * Elimina filas de la tabla
 * @param[in] Pos posicion inicial de las filas a eliminar
 * @param[in] NumRows cantidad de filas a eliminar a partir de Pos
 * @return devuelve true si pudo eliminar todas las filas, false en caso contrario
 * \attention Las filas se van eliminando de a una y no hay vuelta atras si falla en alguna
 */
bool wxGenericTableBase::DeleteRows(size_t Pos, size_t NumRows) {
   if (NumRows <= 0)
      return false;
   size_t endpos = Pos + NumRows - 1;
   size_t rows = GetNumberRows();
   if (Pos < 0 || Pos >= (rows - 1) || endpos <= 0 || endpos > (rows -1) || Pos >= endpos) {
      return false;
   }
   for (size_t row = Pos; row <= endpos; row++) {
      if (!pTable_->DeleteRow(row))
         return false;
   }
   UpdateGrid();
   return true;
}

/** Elimina columnas de la tabla */
bool wxGenericTableBase::DeleteCols(size_t Pos, size_t NumCols) {
   size_t endpos = Pos + NumCols - 1;
   size_t cols = GetNumberCols();
   if (Pos < 0 || Pos >= (cols - 1) || endpos <= 0 || endpos > (cols - 1)
         || Pos >= endpos) {
      return false;
   }
   for (size_t col = Pos; col <= endpos; ++col) {
      if (!pTable_->DeleteColumn(col)) break;
   }
   std::vector<long>::iterator dit = colIndexHolder_.begin();
   std::advance(dit, Pos);
   colIndexHolder_.erase(dit, dit + NumCols);
   UpdateGrid();
   return true;
}

/**
 * Elimina todas las filas y su contenido de la tabla
 */
void wxGenericTableBase::Clear() {
   pTable_->Clear();
   UpdateGrid();
}

/**
 * Obtiene la tabla del modelo
 * @return devuelve la tabla sobre la cual se efectuan las acciones
 */
Table* wxGenericTableBase::GetTable() {
   return pTable_;
}

/**
 * Informa si la tabla tiene alguna celda en edicion
 * @return devuelve true si se encuentra en edicion el grid contenido, false en caso contrario
 */
bool wxGenericTableBase::IsEditing() {
   if (!GetView())
      return false;
   return GetView()->IsCellEditControlEnabled();
}

/**
 * Fuerza la finalizacion de la edicion del grid contenido
 */
void wxGenericTableBase::EndEdition() {
   if (!GetView())
      return;
   GetView()->HideCellEditControl();
}

/**
 * Actualiza dimension y tipo de datos en columnas de wxGrid para que coincida con los datos
 * en el vector que se despliega
 * \attention Es necesario invocar a este metodo despues de modificar la estructura de la tabla
 */
void wxGenericTableBase::UpdateGrid() {
   if (!GetView())
      return;
   UpdateGridSize();
   GetView()->Refresh();
}

/**
 * Actualiza las dimensiones (numero de columnas y filas) de la grilla para que coincidan
 * con la dimension de la tabla a desplegar
 * @return bool que indica si pudo actualizar dimension de wxGrid
 */
bool wxGenericTableBase::UpdateGridSize() {
   if (!GetView()) {
      return false;
   }
   UpdateRows();
   UpdateColumns();
   return true;
}

/**
 * Actualiza las filas de la grilla en pantalla
 */
void wxGenericTableBase::UpdateRows() {
   int rows = GetNumberRows();
   int gridrows = GetView()->GetNumberRows();
   if (rows > gridrows) {
      wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_INSERTED, 0, rows - gridrows);
      GetView()->ProcessTableMessage(msg);
      hasChangedFlag_ = true;
   } else if (rows < gridrows) {
      wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_DELETED, 0, gridrows - rows);
      GetView()->ProcessTableMessage(msg);
      hasChangedFlag_ = true;
   }
}

/**
 * Actualiza las columnas de la grilla en pantalla
 */
void wxGenericTableBase::UpdateColumns() {
   int cols = GetNumberCols();
   int gridcols = GetView()->GetNumberCols();
   if (cols > gridcols) {
      wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_COLS_INSERTED, 0, cols - gridcols);
      GetView()->ProcessTableMessage(msg);
      hasChangedFlag_ = true;
   } else if (cols < gridcols) {
      wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_COLS_DELETED, 0, gridcols - cols);
      GetView()->ProcessTableMessage(msg);
      hasChangedFlag_ = true;
   }
}

/** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
void wxGenericTableBase::Update(NotificationInterface* pNotification) {
   pNotification->ApplyNotification(this);
}

/** Oculta la columna correspondiente al indice que se pasa por parametro**/
bool wxGenericTableBase::HiddeColumn(int ColumnIndex) {
   int cols = colIndexHolder_.size() -1;
   if (ColumnIndex > cols) {
      return false;
   }
   std::vector<long>::iterator it = colIndexHolder_.begin();
   // se mueve hasta la posicion y elimina el indice
   it+=ColumnIndex;
   colIndexHolder_.erase(it);
   return true;
}
/** Muestra la columna correspondiente al indice que se pasa por parametro**/
bool wxGenericTableBase::ShowHiddenColumn(int ColumnIndex) {
   // TODO(Gabriel - TCK #3988): Implementar
   return false;
}

/** Oculta la columna correspondiente al indice que se pasa por parametro**/
bool wxGenericTableBase::IsColumnHidden(int ColumnId) const {
   bool visible = false;
   for (size_t c = 0; !visible && c < colIndexHolder_.size(); ++c) {
      visible = (ColumnId == colIndexHolder_.at(c));
   }
   return !visible;
}

/** Se agrega una nueva columna y se debe actualizar el control de las mismas
 *  @param ColumnId id de la columna agregada**/
void wxGenericTableBase::ColumnAdded(int ColumnId) {
   int index = this->pTable_->GetColumnIndexBy(ColumnId);
   if (index < static_cast<int>(colIndexHolder_.size())) {
      std::vector<long>::iterator it = colIndexHolder_.begin();
      std::advance(it, index);
      colIndexHolder_.insert(it, ColumnId);
   } else {
      colIndexHolder_.push_back(ColumnId);
   }
   UpdateProvider();
}

/** Se notifica que fue eliminada la columna con id pasado por parametro **/
void wxGenericTableBase::ColumnDeleted(int ColumnId, const std::string& ColumnName) {
   std::vector<long>::iterator it = colIndexHolder_.begin();
   for (; it != colIndexHolder_.end(); ++it) {
      if ((*it) == ColumnId) {
         // elimina la columna removida
         colIndexHolder_.erase(it);
         break;
      }
   }
   // actualiza las columas de solo lectura
   std::set<std::string>::iterator findit = readOnlyColumns_.find(ColumnName);
   if (findit != readOnlyColumns_.end()) {
      readOnlyColumns_.erase(findit);
   }
   UpdateProvider();
}

/** Configura una columna como readonly */
void wxGenericTableBase::SetReadOnlyColumnView(const std::string& ColumnName,
                                               bool ReadOnly) {
   std::set<std::string>::iterator findit = readOnlyColumns_.find(ColumnName);
   if (ReadOnly && findit == readOnlyColumns_.end()) {
      readOnlyColumns_.insert(ColumnName);
   } else if (ReadOnly && findit != readOnlyColumns_.end()) {
      readOnlyColumns_.erase(findit);
      readOnlyColumns_.insert(ColumnName);
   } else if (!ReadOnly && findit!= readOnlyColumns_.end()) {
      readOnlyColumns_.erase(findit);
   }
   UpdateProvider();
}

/** Consulta si una columna es readonly**/
bool wxGenericTableBase::IsReadOnlyColumn(const std::string& ColumnName) const {
   return readOnlyColumns_.find(ColumnName) != readOnlyColumns_.end();
}
} /** namespace suri */
