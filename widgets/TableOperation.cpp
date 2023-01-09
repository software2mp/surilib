/*! \file TableOperation.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <string>
#include <cstddef>

#include "TableOperation.h"
#include "suri/ParameterCollection.h"

namespace suri {

/**
 * Ctor.
 */
TableOperation::TableOperation(PermissionList::OperationType Operation, int TableColumn,
                               int DSColumn, int TableRow, int DSRow,
                               const std::string& value, BaseParameter* pTagP) :
      operation_(Operation), tableColumn_(TableColumn), dsColumn_(DSColumn),
      tableRow_(TableRow), dsRow_(DSRow), value_(value), pTag_(pTagP) {

}

/**
 * Dtor.
 */
TableOperation::~TableOperation() {
}

/**
 * Devuelve el numero de columna
 * @return Devuelve el numero de columna
 */
PermissionList::OperationType TableOperation::GetOperationType() const {
   return operation_;
}

/**
 * Obtiene el numero de columna de la tabla.
 */
int TableOperation::GetTableColumn() const {
   return tableColumn_;
}

/**
 * Obtiene el numero de columna del origen de datos.
 */
int TableOperation::GetDSColumn() const {
   return dsColumn_;
}

/**
 * Obtiene el numero de fila de la tabla.
 */
int TableOperation::GetTableRow() const {
   return tableRow_;
}

/**
 * Obtiene el numero de fila del origen de datos.
 */
int TableOperation::GetDSRow() const {
   return dsRow_;
}


/**
 * Devuelve el valor asociado
 * @return Devuelve el valor asociado
 */
std::string TableOperation::GetValue() const {
   return value_;
}

/**
 * Cambia el valor asociado
 * @param Value Valor a ser seteado
 */
void TableOperation::SetValue(const std::string& Value) {
   value_ = Value;
}

/**
 * Devuelve la informacion extra asociada.
 *
 * @return Informacion extra asociada al objeto.
 */
BaseParameter* TableOperation::GetTag() const {
   return pTag_;
}

} /** namespace suri */
