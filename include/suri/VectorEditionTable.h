/*! \file VectorEditionTable.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTOREDITIONTABLE_H_
#define VECTOREDITIONTABLE_H_


// Includes Estandar
#include <string>

// Includes Suri
#include "suri/Table.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class DatasourceInterface;

/*!
 *   Clase que extiende el comportamiento de BufferedTable
 *   para la incorporacion del concepto de registro fantasma.
 *
 */
class VectorEditionTable : public Table {
   /** ctor copia. **/
   VectorEditionTable(const VectorEditionTable&);

public:
   explicit VectorEditionTable(DatasourceInterface* pDatasource);
   virtual ~VectorEditionTable();
   /** Verifica si un tipo es soportado por una implementacion concreta de esta interfaz */
   virtual bool IsSupportedType(ColumnType ColumnType) const;

   /** Obtiene el contenido de la celda y lo interpreta segun el tipo asociado a la celda */
   virtual bool GetCellValue(int Column, int Row, std::string& CellValue) const;
   /** Modifica el contenido de la celda y lo interpreta segun el tipo asociado a la celda */
   virtual bool SetCellValue(int Column, int Row, std::string& CellValue);

private:
   /** Metodo que verifica que la columna posee definido un tipo logico de dato*/
   virtual ColumnType GetMetaTypeColumn(int Column) const;
   /** fuente de datos que se utiliza para la tabla **/
   DatasourceInterface* pDatasource_;
};
}  /** namespace suri */

#endif /* VECTOREDITIONTABLE_H_ */
