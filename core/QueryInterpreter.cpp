/*! \file QueryInterpreter.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "QueryInterpreter.h"
#include <ogrsf_frmts.h>
#include <ogr_feature.h>
// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor **/
QueryInterpreter::QueryInterpreter(const std::string& TableUrl,
                                   const std::string& Query) :
      pFilteredVector_(NULL), tableUrl_(TableUrl), query_(Query), canExecute_(true) {
   pFilteredVector_ = new FilteredVector(Vector::Open(TableUrl), new Filter(Query));
   LoadRowsMap();
}

/** Destructor **/
QueryInterpreter::~QueryInterpreter() {
}

/**
 * Cuenta la cantidad de filas en la tabla filtrada
 * @return int con la cantidad de filas en la tabla filtrada
 */
int QueryInterpreter::GetRows() {
   int rows = -1;
   OGRLayer* player = pFilteredVector_->GetLayer(0);
   rows = player->GetFeatureCount();
   return rows;
}

/**
 * Busca una fila de la tabla sin filtrar en la tabla filtrada
 * @param[in] RowNumber numero de fila de la tabla sin filtrar
 * @return int correspondiente a la fila de la tabla filtrada
 */
int QueryInterpreter::GetRow(int RowNumber) {
   int row = -1;
   if(!rowsMap_.empty())
      row = rowsMap_.find(RowNumber)->second;
   return row;
}

/**
 * Cuenta la cantidad de columnas en la tabla filtrada
 * @return int con la cantidad de columnas en la tabla filtrada
 */
int QueryInterpreter::GetColumns() {
   int cols = -1;
   OGRLayer* player = pFilteredVector_->GetLayer(0);
   cols = player->GetNextFeature()->GetFieldCount();
   return cols;
}

/**
 * Busca una columna de la tabla sin filtrar en la tabla filtrada
 * @param[in] RowNumber numero de columna de la tabla sin filtrar
 * @return int correspondiente a la fila de la tabla filtrada
 */
int QueryInterpreter::GetColumn(int ColumnId) {
   int col = -1;
   return col;
}

/** Arma un mapa con numero de fila de fila y feature id */
void QueryInterpreter::LoadRowsMap() {
   OGRLayer* player = pFilteredVector_->GetLayer(0);
   if (!player) {
      canExecute_ = false;
      return;
   }
   player->ResetReading();
   OGRFeature* pfeature = player->GetNextFeature();
   for (int i = 0; pfeature != NULL; i++) {
      rowsMap_.insert(std::pair<int,int>(i, pfeature->GetFID()));
      OGRFeature::DestroyFeature(pfeature);
      pfeature = player->GetNextFeature();
   }
}

/** Retorna si es posible ejecutar la transaccion */
bool QueryInterpreter::CanExecute() {
   return canExecute_;
}
} /** namespace suri */
