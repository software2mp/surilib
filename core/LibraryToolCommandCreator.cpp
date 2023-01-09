/*! \file LibraryToolCommandCreator.cpp */
/** 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 * 
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 * 
*/

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
