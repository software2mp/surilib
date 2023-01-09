/*! \file VectorTableStructureDriver.cpp */
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
#include "suri/VectorTableStructureDriver.h"
#include "suri/DataTypes.h"

// Includes Wx
// Defines
#define FIELD_NAME_COLUMN "Campo"
#define FIELD_TYPE_COLUMN "Tipo"

// forwards

namespace suri {

/** numero de columna que corresponde al nombre del campo **/
const int VectorTableStructureDriver::FieldNameColumn = 0;
/** numero de columna que corresponde al tipo del campo **/
const int VectorTableStructureDriver::FieldTypeColumn = 1;

/** Constructor **/
VectorTableStructureDriver::VectorTableStructureDriver(
      DatasourceInterface* pDatasource) {
   pVectorDatasource_ = dynamic_cast<VectorDatasource*>(pDatasource);
   if (!pVectorDatasource_)
      return;
   id_ = pDatasource->GetId();
   InitializeTable();
}

/** Destructor **/
VectorTableStructureDriver::~VectorTableStructureDriver() {
}

/** Metodo auxiliar que inicializa la tabla a partir de la fuente de datos**/
void VectorTableStructureDriver::InitializeTable() {
   std::string file = pVectorDatasource_->GetUrl();
   vectorEditor_.OpenVector(file);
   if (!vectorEditor_.OpenLayer(pVectorDatasource_->GetActiveLayer()))
      return;
   int fields = vectorEditor_.GetFieldCount();
   AppendColumn(DataInfo<unsigned char>::Name, _(FIELD_NAME_COLUMN));
   AppendColumn(DataInfo<unsigned char>::Name, _(FIELD_TYPE_COLUMN));
   for (int i = 0; i < fields; ++i) {
      AppendRow();
      std::string fieldname = vectorEditor_.GetFieldName(i);
      WriteVarchar(FieldNameColumn, GetRows() - 1, fieldname);
      std::string fieldtype = vectorEditor_.GetFieldType(i);
      WriteVarchar(FieldTypeColumn, GetRows() - 1, fieldtype);
   }
   vectorEditor_.CloseVector();
}

} /** namespace suri */
