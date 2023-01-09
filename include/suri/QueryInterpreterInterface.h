/*! \file QueryInterpreterInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef QUERYINTERPRETERINTERFACE_H_
#define QUERYINTERPRETERINTERFACE_H_

// Includes Estandar
#include <string>
#include <map>
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

class QueryInterpreterInterface {
public:
   /** Constructor */
   QueryInterpreterInterface() {}
   /** Constructor */
   QueryInterpreterInterface(const std::string& TableUrl, const std::string& Query);
   /** Destructor */
   virtual ~QueryInterpreterInterface() {}
   /** Retorna la fila de la tabla */
   virtual int GetRow(int RowId)=0;
   /** Retorna la cantidad total de filas de la tabla */
   virtual int GetRows()=0;
   /** Retorna la columna de la tabla */
   virtual int GetColumn(int ColId)=0;
   /** Retorna la cantidad total de columnas de la tabla */
   virtual int GetColumns()=0;
protected:
   std::map<int, int> colsMap_;
   std::map<int, int> rowsMap_;
};

} /** namespace suri */

#endif /* QUERYINTERPRETERINTERFACE_H_ */
