/*! \file QueryInterpreter.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef QUERYINTERPRETER_H_
#define QUERYINTERPRETER_H_

// Includes Estandar
// Includes Suri
#include "suri/QueryInterpreterInterface.h"
#include "FilteredVector.h"
// Includes Wx
// Defines
// forwards

namespace suri {

class QueryInterpreter : public QueryInterpreterInterface {
public:
   /** Constructor */
   QueryInterpreter(const std::string& TableUrl, const std::string& Query);
   /** Destructor */
   virtual ~QueryInterpreter();
   /** Retorna la fila del interpreter, es decir, de la tabla filtrada */
   virtual int GetRow(int RowId);
   /** Retorna la cantidad total de filas en la tabla filtrada */
   virtual int GetRows();
   /** Retorna la columna del interpreter, es decir, de la tabla filtrada */
   virtual int GetColumn(int ColId);
   /** Retorna la cantidad total de columnas en la tabla filtrada */
   virtual int GetColumns();

   bool CanExecute();
private:
   /** Arma un mapa con numero de fila de fila y feature id */
   void LoadRowsMap();
   /** vector con el contenido filtrado */
   FilteredVector* pFilteredVector_;
   /** string con Url de la tabla */
   std::string tableUrl_;
   /** string con la consulta */
   std::string query_;
   bool canExecute_;
};

} /** namespace suri */

#endif /* QUERYINTERPRETER_H_ */
