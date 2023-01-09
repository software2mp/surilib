/*! \file NullQueryInterpreter.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "NullQueryInterpreter.h"

// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor */
NullQueryInterpreter::NullQueryInterpreter(DriverInterface* pDriver) :
		pDriver_(pDriver) {
}

/** Destructor */
NullQueryInterpreter::~NullQueryInterpreter() {
}

/** Retorna la fila del driver, es decir, de la tabla sin filtrar */
int NullQueryInterpreter::GetRow(int RowId){
   return -1;
}

/** Retorna la columna del driver, es decir, de la tabla sin filtrar */
int NullQueryInterpreter::GetColumn(int ColId){
   return -1;
}

/** Retorna la cantidad total de columnas en la tabla */
int NullQueryInterpreter::GetColumns() {
   return pDriver_ ? pDriver_->GetColumns() : -1;
}

/** Retorna la cantidad total de filas en la tabla */
int NullQueryInterpreter::GetRows() {
   return pDriver_ ? pDriver_->GetRows() : -1;
}

} /** namespace suri */
