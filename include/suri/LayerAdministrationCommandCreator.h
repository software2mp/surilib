/*! \file LayerAdministrationCommandCreator.h */                                                     
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
*/ 

#ifndef __LAYER_ADMIN_COMMAND_CREATOR_H__
#define __LAYER_ADMIN_COMMAND_CREATOR_H__

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
 *  Clase encargada de crear y configurar los comandos asociados
 *  a la administracion de capas de la aplicacion
 **/
class LayerAdministrationCommandCreator : public CommandCreatorInterface {
	/** constructor copia **/
	LayerAdministrationCommandCreator(const LayerAdministrationCommandCreator&);

public:
	/** constructor **/
	LayerAdministrationCommandCreator(); 
	/** destrcutor **/
	~LayerAdministrationCommandCreator();

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
