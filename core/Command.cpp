/*! \file Command.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/Command.h"

// Includes Estandar
// Includes Suri
#include "suri/ToolGroupManager.h"

// Includes Wx
// Defines
// forwards

namespace suri {

const Command::CommandExclusionGroupType Command::Standalone =
      ToolGroupManager::StandAlone; /*! No interfiere con otros */

}  // namespace suri

