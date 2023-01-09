/*! \file BandMathCommandCreator.cpp */
/** 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 * 
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 * 
*/

// Includes Suri
#include "suri/BandMathCommandCreator.h"
#include "suri/ToolCommand.h"
#include "suri/messages.h"
#include "resources.h"

namespace suri {

namespace core {

/** constructor **/
BandMathCommandCreator::BandMathCommandCreator() {
}

/** destructor **/
BandMathCommandCreator::~BandMathCommandCreator() {
}

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* BandMathCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command *pcommand = NULL;
	if (CommandName == "ApplyFilter") {     
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");    
		pcommand->SetDescription(tooltip_RASTER_FILTER_BUTTON); 
		pcommand->SetDecoration(tool_RASTER_FILTER);
		pcommand->SetLocation("ID_FILTER_RASTER_MENUTIEM"); 
	} else if (CommandName == "Indexes") {     
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu"); 
		pcommand->SetDescription(tooltip_RASTER_INDEX_BUTTON); 
		pcommand->SetDecoration(tool_RASTER_INDEX);
		pcommand->SetLocation("ID_INDICES_MENUTIEM");
	} else if (CommandName == "BandMath") {  
		pcommand = new ToolCommand(pTool); 
		pcommand->SetType("Menu");      
		pcommand->SetDescription(tooltip_RASTER_BANDMATH_BUTTON); 
		pcommand->SetDecoration(tool_RASTER_BANDMATH); 
		pcommand->SetLocation("ID_BAND_MATH_MENUTIEM"); 
	} else if (CommandName == "FilterLibrary") {
      pcommand = new ToolCommand(pTool);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_RASTER_LIBRARY_FILTER_BUTTON);
      pcommand->SetDecoration(tool_RASTER_FILTER);
      pcommand->SetLocation("ID_FILTER_LIBRARY_MENUTIEM");
   }
	return pcommand;
}

}	/** namespace core */
}  /** namespace suri */
