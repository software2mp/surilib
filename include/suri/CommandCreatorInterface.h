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
