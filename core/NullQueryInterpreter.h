/*! \file NullQueryInterpreter.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef NULLQUERYINTERPRETER_H_
#define NULLQUERYINTERPRETER_H_

// Includes Estandar
// Includes Suri
#include "suri/QueryInterpreterInterface.h"
#include "suri/DriverInterface.h"
// Includes Wx
// Defines
// forwards

namespace suri {

class NullQueryInterpreter : public QueryInterpreterInterface {
public:
	/** Constructor */
   NullQueryInterpreter(DriverInterface* pDriver);
   /** Destructor */
   virtual ~NullQueryInterpreter();
   /** Retorna la fila del driver, es decir, de la tabla sin filtrar */
   virtual int GetRow(int RowId);
   /** Retorna la columna del driver, es decir, de la tabla sin filtrar */
   virtual int GetColumn(int ColId);
   /** Retorna la cantidad total de columnas en la tabla */
   virtual int GetColumns();
   /** Retorna la cantidad total de filas en la tabla */
   virtual int GetRows();

private:
   DriverInterface* pDriver_;
};

} /** namespace suri */

#endif /* NULLQUERYINTERPRETER_H_ */
