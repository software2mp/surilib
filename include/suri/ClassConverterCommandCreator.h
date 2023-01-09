/*! \file ClassConverterCommandCreator.h */                                                     
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
*/ 

#ifndef __CLASS_CONVERTER_COMMAND_CREATOR_H__
#define __CLASS_CONVERTER_COMMAND_CREATOR_H__

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
 *  Clase que encargada de configurar el Command para 
 *  agregar la informacion necesaria a una capa vectorial
 *  para que pueda ser interpretada como una clase
 *
 **/
class ClassConverterCommandCreator : public CommandCreatorInterface {
	/** ctor copia **/
	ClassConverterCommandCreator(const ClassConverterCommandCreator&);

public:
	/** constructor **/
	ClassConverterCommandCreator();
	/** destructor **/
	~ClassConverterCommandCreator();

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
