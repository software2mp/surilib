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

#ifndef __COMMAND_EXEC_HANLDER_INTERFCE_H_
#define __COMMAND_EXEC_HANLDER_INTERFCE_H_

#include <string>

namespace suri {

// forwards
class Command;
class DataViewManager;
class GenericTool;
class ProcessInterface;
class RasterElement;
class Subset;

namespace core {


/** 
 * Interfaz que define el comportamiento esperado de una clase encargada de administrar 
 * la ejecucion de un comando que posee una herramienta. 
 **/
class CommandExecutionHandlerInterface {
   /** Constructor copia **/
	CommandExecutionHandlerInterface(const CommandExecutionHandlerInterface&);

	public:
		/** Constructor **/
		CommandExecutionHandlerInterface() {}
		/** Destructor **/
		virtual ~CommandExecutionHandlerInterface() {}
		/**
		 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
		 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
       * @return true en caso de poder manejar la ejecucion del comando
       * @return false en caso contrario
       */
		virtual bool Execute(const Command* pCommand, GenericTool* pTool)=0;
	protected:
		/** Funcion auxiliar que una vez instanciado el proyecto configurada la capa de adaptacion
		 *  y corre la ventana de configuracion del mismo para que luego corra el proceso
		 *  @param[in] pDataView administrador de informacion de la aplicacion
		 *  @param[in] Caption nombre que tendra la vetana de configuracion del proceso
		 *  @param[in] Instancia del proceso que se correra una vez configurado
		 *  @param[in] Elemento de entrada al proceso
		 **/
		void RunProcess(DataViewManager* pDataView, const std::string& Caption, ProcessInterface* pProcess);
		/** Funcion auxiliar para obtener los parametros basicos de entrada a los procesos
		 *  que operan sobre las bandas de un raster (el elemento de entrada y el subset)
		 **/
		void LoadInputProcessParameters(DataViewManager* pDataView, RasterElement* &pRaster, Subset& ViewerWindow);
};

}  /** namespace core **/
}  /** namespace suri **/

#endif
