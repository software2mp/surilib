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

#include "wxVectorLayerGridTable.h"

// Includes estandar
#include <sstream>

// Includes Suri
#include "suri/messages.h"
#include "suri/AuxiliaryFunctions.h"
#include "wxmacros.h"

// Includes Wx

// Includes App

// Defines
/** Nombre para la columna FID (feature ID) */
#define FID_COLUMN_NAME "FID"

/** namespace suri */
namespace suri {
/**
 * Ctor
 * @param[in] pVectorEditor vector editor que se usa para
 * leer/modificar los datos de la tabla
 */
wxVectorLayerGridTable::wxVectorLayerGridTable(VectorEditor* pVectorEditor) :
      readOnly_(false), modified_(false), pVectorEditor_(pVectorEditor)

{
}

/**
 * Dtor
 */
wxVectorLayerGridTable::~wxVectorLayerGridTable() {
}

/**
 * Retrona el valor la celda(en formato string) en posicion pedida.
 * @param[in] Row numero de fila de la celda
 * @param[in] Column numero de columna de la celda
 * @return wxString con el valor de la celda
 */
wxString wxVectorLayerGridTable::GetValue(int Row, int Column) {
   std::string value = wxT(message_NA);
   if (!GetFieldValue(Row, Column, value)) {
      REPORT_DEBUG("D: Error al obtener valor del campo %d;%d", Row, Column);
   }
   return _(value.c_str() );
}

/**
 * Actualiza el valor de un campo en vector con el valor que se pasa por parametro.
 * @param[in] Row numero de fila de la celda
 * @param[in] Column numero de columna de la celda
 * @param[in] Value wxString con el valor de la celda
 */
void wxVectorLayerGridTable::SetValue(int Row, int Column, const wxString& Value) {
   std::string value = "";
   if (!GetFieldValue(Row, Column, value) or Value.IsSameAs(value.c_str())) {
      return;
   }
   if (SetFieldValue(Row, Column, Value.c_str())) {
      modified_ = true;
   }
}

/**
 * Informa el numero de filas que tiene la tabla.
 * @return int, numero de filas en tabla de vector
 */
int wxVectorLayerGridTable::GetNumberRows() {
   if (!pVectorEditor_) {
      return 0;
   }
   return pVectorEditor_->GetFeatureCount();
}
/**
 * Informa el numero de columnas que tiene la tabla.
 * @return int, numero de columnas que tiene la tabla
 */
int wxVectorLayerGridTable::GetNumberCols() {
   return displayedFields_.size();
}

/**
 * Informa si la celda esta vacia(siempre false)
 * @param[in] Row numero de fila de la celda
 * @param[in] Column numero de columna de la celda
 * @return siempre false
 */
bool wxVectorLayerGridTable::IsEmptyCell(int Row, int Column) {
   return false;
}

/**
 * Retorna el nombre de una columna
 * @param[in] Column numero de columna de la celda
 * @return nombre de la columna
 */
wxString wxVectorLayerGridTable::GetColLabelValue(int Column) {
   if (!pVectorEditor_ || Column > GetNumberCols()) {
      return _(message_NA);
   }
   return displayedFields_[Column].first;
}

/**
 * Determina si la tabla del vector esta modificada.
 * @param[in] Modified nuevo estado
 */
void wxVectorLayerGridTable::SetModified(bool Modified) {
   modified_ = Modified;
}

/**
 * Informa si la tabla del vector esta modificada.
 * @return indica si la tabla fue modificada
 */
bool wxVectorLayerGridTable::IsModified() {
   return modified_;
}

/**
 * Determina si la tabla del vector se puede modificar.
 * @param[in] ReadOnly nuevo estado de readOnly_.
 */
void wxVectorLayerGridTable::SetReadOnly(bool ReadOnly) {
   readOnly_ = ReadOnly;
}

/**
 * Informa si la tabla del vector esta en modo readonly.
 * @return indica si la tabla esta en modo readonly
 */
bool wxVectorLayerGridTable::IsReadOnly() {
   return readOnly_;
}

/**
 * Configura los campos que se deben mostrar
 * @param[in] FiltredFieldNames vector con los campos que se deben ignorar.
 */
bool wxVectorLayerGridTable::SetFilter(const std::set<std::string> &FiltredFieldNames) {
   filteredFieldNames_ = FiltredFieldNames;
   return LoadDisplayedFields();
}

/**
 * Usa filteredFieldNames_ y pVectorEditor_ para cargar displayedFields_.
 * @return bool, informa si pudo actualizar displayedFields_
 */
bool wxVectorLayerGridTable::LoadDisplayedFields() {
   if (pVectorEditor_ == NULL) {
      return false;
   }

   // Limpia displayedFields_ para crear lista desde cero
   displayedFields_.clear();

   // Recorre los campos usando pVectorEditor_
   std::string fieldname;
   int fieldcount = pVectorEditor_->GetFieldCount();
   if (fieldcount < 0) {
      return false;
   }
   for (int i = 0; i < fieldcount; i++) {
      // Si no encuentra el campo en filteredFieldNames_ lo agrega a displayedFields_.
      fieldname = pVectorEditor_->GetFieldName(i);
      if (filteredFieldNames_.find(fieldname) == filteredFieldNames_.end()) {
         std::pair<std::string, std::string> displayedfielddata(
               fieldname, pVectorEditor_->GetFieldType(i));
         displayedFields_.push_back(displayedfielddata);
      }
   }

   return true;
}

/**
 * Obtiene el valor del campo en el vector en VectorEditor.
 * @param[in] Row numero de fila de la celda
 * @param[in] Column numero de columna de la celda
 * @param[out] Value valor de la celda
 * @return bool que informa si lo encontro
 */
bool wxVectorLayerGridTable::GetFieldValue(int Row, int Column, std::string &Value) {
   if (!pVectorEditor_ || Column > GetNumberCols() || Row > GetNumberRows()) {
      return false;
   }

   /** Busca indice de feature en posicion Row y lo abre */
   long featureid = pVectorEditor_->GetFeatureId(Row);
   if (featureid < 0 || pVectorEditor_->OpenFeature(featureid, false) < 0) {
      return false;
   }

   std::string fieldcontent = wxString(_(message_NA)).c_str();
   std::stringstream valuestream(std::stringstream::in);
   // Segun el tipo de dato llama a VectorEditor::GetFieldValue de tipo correcto
   // y castea el dato a string
   switch (Type2Id(displayedFields_[Column].second)) {
      case DataInfo<int>::Id: {
         int fieldvalue;
         pVectorEditor_->GetField(displayedFields_[Column].first, fieldvalue);
         fieldcontent = LongToString(fieldvalue);
         break;
      }
      case DataInfo<double>::Id: {
         double fieldvalue;
         pVectorEditor_->GetField(displayedFields_[Column].first, fieldvalue);
         fieldcontent = DoubleToString(fieldvalue);
         break;
      }
      case DataInfo<unsigned char>::Id: {
         pVectorEditor_->GetField(displayedFields_[Column].first, fieldcontent);
         break;
      }
      default:
         break;
   }

   // actualiza Value
   Value = fieldcontent;

   pVectorEditor_->CloseFeature();
   return true;
}

/**
 * Modifica el valor del campo. Puede en el futuro usar un cache.
 * @param[in] Row numero de fila de la celda
 * @param[in] Column numero de columna de la celda
 * @param[in] Value nuevo valor de la celda
 * @return bool que informa si pudo modificarlo
 */
bool wxVectorLayerGridTable::SetFieldValue(int Row, int Column,
                                           const std::string &Value) {
   bool returnvalue = false;
   if (!pVectorEditor_ || Column > GetNumberCols() || Row > GetNumberRows()) {
      return returnvalue;
   }

   /** Busca indice de feature en posicion Row y lo abre */
   long featureid = pVectorEditor_->GetFeatureId(Row);
   if (featureid < 0 || pVectorEditor_->OpenFeature(featureid, false) < 0) {
      return returnvalue;
   }

   // Segun el tipo de dato(displayedFields_), convierte (hay funciones) a
   // string y llamo a VectorEditor::SetFieldValue de tipo correcto
   std::string fieldname = displayedFields_[Column].first;
   wxString cellcontent = Value.c_str();

   // entero
   long cellcontentaslong;
   if ((displayedFields_[Column].second == DataInfo<int>::Name)
         && (cellcontent.ToLong(&cellcontentaslong))) {
      int cellcontentint = static_cast<int>(cellcontentaslong);
      returnvalue = pVectorEditor_->SetField(fieldname, cellcontentint);
   }

   // double
   double cellcontentasdouble;
   if ((displayedFields_[Column].second == DataInfo<double>::Name)
         && (cellcontent.ToDouble(&cellcontentasdouble))) {
      returnvalue = pVectorEditor_->SetField(fieldname, cellcontentasdouble);
   }

   // string
   if (displayedFields_[Column].second == DataInfo<unsigned char>::Name) {
      returnvalue = pVectorEditor_->SetField(fieldname, cellcontent.c_str());
   }

   // Retorna resultado de la operacion.
   pVectorEditor_->CloseFeature();
   return returnvalue;
}

/**
 * Retorna una copia del vector con campos desplegados
 * @return vector con nombre y tipo de dato de los campos desplegados
 */
std::vector<std::pair<std::string, std::string> > wxVectorLayerGridTable::GetDisplayedFields() {
   return displayedFields_;
}

/**
 * Llama a UpdateGridSize.
 * Actualiza tipo de datos en columnas de wxGrid para que coincida
 * con los datos en el vector que se despliega.
 */
void wxVectorLayerGridTable::RefreshGrid() {
   if (!GetView()) {
      return;
   }

   // Update wxGrid size(number of columns/rows)
   UpdateGridSize();

   // Recargar los Renderers y Editors de las celdas
   GetView()->BeginBatch();
   std::vector<std::pair<std::string, std::string> >::iterator it =
         displayedFields_.begin();
   for (int i = 0; it < displayedFields_.end(); it++, i++) {
      wxString fieldname = it->first.c_str();
      std::string datatype = it->second;

      // Crea wxGridCellAttr y Configura read only
      bool readonly = readOnly_;
      if (fieldname.IsSameAs(wxT(FID_COLUMN_NAME), false)) {
         readonly = true;
      }
      wxGridCellAttr* pattr = new wxGridCellAttr();
      pattr->SetReadOnly(readonly);
      // Segun el tipo de dato configura editor(wxGridCellAttr::SetEditor)
      switch (Type2Id(datatype)) {
         case DataInfo<int>::Id:
            // Solo se pueden ingresar enteros en columna
            pattr->SetRenderer(new wxGridCellNumberRenderer);
            pattr->SetEditor(new wxGridCellNumberEditor);
            break;
         case DataInfo<double>::Id:
            // Solo se pueden ingresar doubles en columna
            pattr->SetRenderer(new wxGridCellFloatRenderer);
            pattr->SetEditor(new wxGridCellFloatEditor);
            break;
         case DataInfo<unsigned char>::Id:
            // Solo se pueden ingresar strings en columna
            pattr->SetRenderer(new wxGridCellStringRenderer);
            pattr->SetEditor(new wxGridCellTextEditor);
            break;
         default:
            break;
      }
      // Actualiza atributos de la columna.
      GetView()->SetColAttr(i, pattr);
   }

   GetView()->EndBatch();
   GetView()->ForceRefresh();
}

/**
 * Actualiza las dimensiones(numero de columnas
 * y filas) de la grilla para que coincidan
 * con la dimension de la tabla a desplegar.
 * @return bool que indica si pudo actualizar dimension
 * de wxGrid
 */
bool wxVectorLayerGridTable::UpdateGridSize() {
   // Actualiza numero de filas
   if (!GetView()) {
      return false;
   }

   int rows = GetNumberRows();
   int gridrows = GetView()->GetNumberRows();

   if (rows > gridrows) {
      wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_INSERTED, 0,
                             rows - gridrows);
      GetView()->ProcessTableMessage(msg);
   } else if (rows < gridrows) {
      wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_DELETED, 0, gridrows - rows);
      GetView()->ProcessTableMessage(msg);
   }

   // Actualiza numero de columnas
   int cols = GetNumberCols();
   int gridcols = GetView()->GetNumberCols();

   if (cols > gridcols) {
      wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_COLS_INSERTED, 0,
                             cols - gridcols);
      GetView()->ProcessTableMessage(msg);
   } else if (cols < gridcols) {
      wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_COLS_DELETED, 0, gridcols - cols);
      GetView()->ProcessTableMessage(msg);
   }

   return true;
}
}
