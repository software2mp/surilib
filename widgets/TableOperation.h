/*! \file TableOperation.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TABLEOPERATION_H_
#define TABLEOPERATION_H_

// Includes Estandar
#include <string>
#include <cstddef>

// Includes Suri
#include "PermissionList.h"
#include "suri/ParameterCollection.h"

namespace suri {

/**
 * Modela las operaciones de DriverInterface que son susceptibles de ser buffereadas
 * para asistir a la logica transaccional
 */
class TableOperation {
public:
   /**
    * Ctor.
    */
   explicit TableOperation(PermissionList::OperationType Operation, int TableColumn,
                           int DSColumn, int TableRow, int DSRow,
                           const std::string& value, BaseParameter* pTagP = NULL);
   /**
    * Dtor.
    */
   virtual ~TableOperation();

   /**
    * Devuelve el tipo de operacion.
    */
   PermissionList::OperationType GetOperationType() const;

   /**
    * Obtiene el numero de columna de la tabla.
    */
   int GetTableColumn() const;

   /**
    * Obtiene el numero de columna del origen de datos.
    */
   int GetDSColumn() const;

   /**
    * Obtiene el numero de fila de la tabla.
    */
   int GetTableRow() const;

   /**
    * Obtiene el numero de fila del origen de datos.
    */
   int GetDSRow() const;

   /**
    * Devuelve el valor asociado.
    */
   std::string GetValue() const;

   /**
    * Cambia el valor asociado.
    */
   void SetValue(const std::string& Value);

   /**
    * Devuelve la informacion extra asociada.
    */
   BaseParameter* GetTag() const;

private:
   PermissionList::OperationType operation_;  // Tipo de Operacion
   int tableColumn_;
   int dsColumn_;
   int tableRow_;  // Indice de la fila en la tabla
   int dsRow_;  // Indice de la fila en el origen de datos
   std::string value_;  // Valor
   BaseParameter* pTag_;  // Informacion extra
};

} /** namespace suri */

#endif /* TABLEOPERATION_H_ */
