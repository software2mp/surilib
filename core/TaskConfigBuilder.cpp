/*! \file TaskConfigBuilder.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


// Includes Estandar
#include <string>

// Includes Suri
#include "suri/TaskConfigBuilder.h"

// Includes Wx
// Defines
// forwards

namespace suri {
const std::string TaskConfigBuilder::kDataViewParam = "DataView";
const std::string TaskConfigBuilder::kTaskParam = "Task";
const std::string TaskConfigBuilder::kVectorDatasourceParam = "VectorDatasource";
const std::string TaskConfigBuilder::kFileNameParam = "FileName";
const std::string TaskConfigBuilder::kToolNameParam = "ToolName";
const std::string TaskConfigBuilder::KRasterParam = "RasterDatasource";

/** Constructor **/
TaskConfigBuilder::TaskConfigBuilder() {
}

/** Destructor **/
TaskConfigBuilder::~TaskConfigBuilder() {
}

}  /** namespace suri **/
