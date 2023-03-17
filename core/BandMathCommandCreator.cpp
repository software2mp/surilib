/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
