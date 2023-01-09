/** \file DefaultTable.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DEFAULTTABLE_H_
#define DEFAULTTABLE_H_

// Includes standard
#include <map>
#include <set>
#include <vector>
#include <string>

// Includes suri
#include "suri/Table.h"
#include "suri/DriverInterface.h"
#include "wxGenericTableBase.h"

// Defines

// Forwards

namespace suri {

/**
 * Define la implementacion por defecto de TableInterface que provee SuriLib
 *
 * Por defecto se crea con un MemoryDriver, pero se puede cambiar con SetDriver.
 * La tabla se hace cargo de la memoria del driver, por lo que se le entrega un puntero con new
 * y la tabla misma se encarga de liberar la memoria cuando corresponda.
 *
 * Posee una dependencia ciclica con wxGenericTableBase para hacer las actualizaciones que
 * correspondan
 *
 * Se tienen distintos valores de Permisos para ser utilizados:
 *
 * DefaultTable::PermissionReadOnly
 * DefaultTable::PermissionFixedSize
 * DefaultTable::PermissionAddRows
 * DefaultTable::PermissionAddColumns
 * DefaultTable::PermissionHideColumn
 * DefaultTable::PermissionReadOnlyColumn
 *
 * Por defecto se utiliza DefaultTable::PermissionReadOnlyColumn
 *
 * Las operaciones permitidas por los permisos se consultan antes de la ejecucion de un metodo.
 * Tambien es necesario preguntar al driver por los permisos porque si no se permite en el driver
 * una operacion, tampoco sera permitida en la tabla.
 *
 * Las transacciones de Commit y Rollback se ejecutan solo en el caso de que el driver lo soporte
 *
 * Por el momento se soportan los siguientes tipos: INT, COLOR_RGBA_HEXA, STRING; FLOAT
 *
 */
class DefaultTable : public Table {
public:
   /** Constructor */
   DefaultTable();
   /** Destructor */
   virtual ~DefaultTable();
   /** Obtiene el contenido de la celda y lo interpreta el tipo asociado a la columna de celda */
   virtual bool GetCellValue(int Column, int Row, std::string& CellValue) const;
   /** Modifica el contenido de la celda y lo interpreta el tipo asociado a la columna de celda */
   virtual bool SetCellValue(int Column, int Row, std::string& CellValue);
   /** Verifica si un tipo es soportado por esta implementacion */
   virtual bool IsSupportedType(ColumnType ColumnType) const;
#ifdef __SHOW_TABLE_LOG___
   /** Getter de un puntero const del Driver */
   virtual const DriverInterface* GetDriver() const;
   /** Obtiene las tabla en un string \note Metodo Debug */
   virtual std::string GetTable() const;
#endif
};

} /** namespace suri */

#endif /* DEFAULTTABLE_H_ */
