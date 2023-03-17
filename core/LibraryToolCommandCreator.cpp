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
#include "suri/LibraryToolCommandCreator.h"
#include "suri/ToolCommand.h"
#include "suri/messages.h"
#include "resources.h"

namespace suri {

namespace core {

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* SpatialReferenceLibraryCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command *pcommand = NULL;
	if (CommandName == "SpatialReferenceLibrary") {     
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");    
		pcommand->SetLocation("ID_SR_LIBRARY_MENUTIEM"); 
	}    
	return pcommand;
}
/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* DatumLibraryCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command *pcommand = NULL;
	if (CommandName == "DatumLibrary") {     
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");    
		pcommand->SetLocation("ID_DATUM_LIBRARY_MENUTIEM"); 
	}    
	return pcommand;
}

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* EllipsoidLibraryCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command *pcommand = NULL;
	if (CommandName == "EllipsoidLibrary") {     
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");    
		pcommand->SetLocation("ID_PELLIPSOID_LIBRARY_MENUTIEM"); 
	}    
	return pcommand;
}

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* DensitySliceLibraryCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command *pcommand = NULL;
	if (CommandName == "DensitySlice") {     
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");    
		pcommand->SetLocation("ID_DENSITY_SLICE_LIBRARY_MENUTIEM");
	}    
	return pcommand;
}

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* WmsLibraryCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command *pcommand = NULL;
	if (CommandName == "WmsLibrary") {     
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");    
		pcommand->SetLocation("ID_WMS_LIBRARY_MENUTIEM"); 
	}    
	return pcommand;
}

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* VectorStyleLibraryCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command *pcommand = NULL;
	if (CommandName == "VectorstyleLibrary") {     
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");    
		pcommand->SetLocation("ID_VECTORSTYLE_LIBRARY_MENUTIEM"); 
	}    
	return pcommand;
}

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* SpectralSignLibraryCommandCreator::DoCreateCommand(
      const std::string& CommandName, ToolInterface* pTool) {
   Command *pcommand = NULL;
   if (CommandName == "SpectralSignLibrary") {
      pcommand = new ToolCommand(pTool);
      pcommand->SetType("Menu");
      pcommand->SetLocation("ID_SPECTRAL_SIGN_LIBRARY_MENUTIEM");
   }
   return pcommand;
}

/**
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
 * del comando sea del dominio de la clase.
 **/
Command* ConvolutionfiltersLibraryCommandCreator::DoCreateCommand(const std::string& CommandName,
                                                         ToolInterface* pTool) {
   Command *pcommand = NULL;
   if (CommandName == "ConvolutionFiltersLibrary") {
      pcommand = new ToolCommand(pTool);
      pcommand->SetType("Menu");
      pcommand->SetLocation("ID_CONVOLUTION_FILTERS_LIBRARY_MENUTIEM");
   }
   return pcommand;
}

/**
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
 * del comando sea del dominio de la clase.
 **/
Command* EquationLibraryCommandCreator::DoCreateCommand(const std::string& CommandName,
                                                        ToolInterface* pTool) {
   Command *pcommand = NULL;
   if (CommandName == "EquationLibrary") {
      pcommand = new ToolCommand(pTool);
      pcommand->SetType("Menu");
      pcommand->SetLocation("ID_EQUATION_LIBRARY_MENUTIEM");
   }
   return pcommand;
}

/**
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
 * del comando sea del dominio de la clase.
 **/
Command* IndexLibraryCommandCreator::DoCreateCommand(const std::string& CommandName,
                                                        ToolInterface* pTool) {
   Command *pcommand = NULL;
   if (CommandName == "IndexLibrary") {
      pcommand = new ToolCommand(pTool);
      pcommand->SetType("Menu");
      pcommand->SetLocation("ID_INDEX_LIBRARY_MENUTIEM");
   }
   return pcommand;
}
}	/** namespace core */
}  /** namespace suri */
