/*! \file MergeLayersCommandCreator.h */                                                     
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
*/ 

#ifndef __MERGE_LAYER_COMMAND_CREATOR_H__
#define __MERGE_LAYER_COMMAND_CREATOR_H__

// Includes Estandard
#include <string>

// Includes Suri
#include "suri/CommandCreatorInterface.h"

namespace suri {

// forwards
class Command;
class ToolInterface;

namespace core {


/** 
 *	Clase que encargada de configurar el Command para la fusion de capas vectoriales  
 **/
class MergeLayersCommandCreator : public CommandCreatorInterface {
	/** ctor copia **/
	MergeLayersCommandCreator(const MergeLayersCommandCreator&);

public:
	/** constructor **/
	MergeLayersCommandCreator();
	/** destructor **/
	~MergeLayersCommandCreator();

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
