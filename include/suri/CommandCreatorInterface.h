/*! \file CommandCreatorInterface.h */                                                     
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
*/ 

#ifndef __COMMAND_CREATOR_INTERFACE_H__
#define __COMMAND_CREATOR_INTERFACE_H__

// Includes estandard
#include <string>

namespace suri {
	// forwards
	class Command;
	class ToolInterface;

namespace core {

/** 
 * Interfaz que define el comportamiento esperado de una clase encargada 
 *  de la creacion de un comando a partir de su nombre.  
 **/
class CommandCreatorInterface {
	/** constructor copia **/
	CommandCreatorInterface(const CommandCreatorInterface&);

public:
	/** constructor **/
	CommandCreatorInterface() : pSuccessor_(NULL) {}
	/** destructor **/
	virtual ~CommandCreatorInterface() {
		CommandCreatorInterface* pnext = pSuccessor_;
		/** elimino la cadena de responsabilidad */
		while (pnext) {
			CommandCreatorInterface* paux = pnext->pSuccessor_;
			delete pnext;
			pnext = paux;
		}
   }

	/** 
	 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
    * del comando sea del dominio de la clase. 
    **/
	Command* CreateCommand(const std::string& CommandName, ToolInterface* pTool) {
		Command* pcommand = DoCreateCommand(CommandName, pTool);
		if (!pcommand && pSuccessor_)
			pcommand = pSuccessor_->CreateCommand(CommandName, pTool);
		return pcommand;
	}

	/** configura el siguiente sucesor en la cadena de responsabilidades **/
	void AddSuccessor(CommandCreatorInterface* pSuccessor) {
		CommandCreatorInterface* pnext = pSuccessor_;
		while (pnext && pnext->pSuccessor_) {
			pnext = pnext->pSuccessor_;
		}
		if (!pnext)
			pSuccessor_ = pSuccessor;
		else
			pnext->pSuccessor_ = pSuccessor;
	}

protected:
	/** Metodo polimorfico que crea el comando en funcion del nombre del mismo **/
	virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool)=0;
	CommandCreatorInterface* pSuccessor_;
};

}  /** namespace core **/
}  /** namespace suri **/
#endif
