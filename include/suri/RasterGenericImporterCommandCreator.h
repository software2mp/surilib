/*! \file RasterGenericImporterCommandCreator.h */                                                     
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
*/ 

#ifndef __RASTER_GEN_IMPORTER_COMMAND_CREATOR_H__
#define __RASTER_GEN_IMPORTER_COMMAND_CREATOR_H___

// Includes Estandard
#include <string>

// Includes Suri
#include "suri/CommandCreatorInterface.h"

namespace suri {

// forwards
class Command;
class ToolInterface;

namespace core {


/** Clase que encargada de configurar el Command para 
 *  la importacion de raster "crudos" con interlineado BIL, BIP o BSQ
 **/
class RasterGenericImporterCommandCreator : public CommandCreatorInterface {
	/** ctor copia **/
	RasterGenericImporterCommandCreator(const RasterGenericImporterCommandCreator&);

public:
	/** constructor **/
	RasterGenericImporterCommandCreator();
	/** destructor **/
	~RasterGenericImporterCommandCreator();

private:
	/** 
	 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
    * del comando sea del dominio de la clase. 
    **/
	virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

}  /** namespace core **/
}  /** namespace suri **/

#endif
