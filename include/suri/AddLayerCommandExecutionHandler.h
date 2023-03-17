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

#ifndef __ADD_LAYER_COMMAND_EXEC_HANDLER_H__
#define __ADD_LAYER_COMMAND_EXEC_HANDLER_H__

// Includes Estandard
#include <string>

// Includes Suri
#include "suri/CommandExecutionHandlerInterface.h"

namespace suri {

// forwards
class DataViewManager;

namespace core {


/** Clase encargada de administrar la ejecucion del comando AddLayer **/
class AddLayerCommandExecutionHandler : public CommandExecutionHandlerInterface {

	/** constructor copia **/
	AddLayerCommandExecutionHandler(const AddLayerCommandExecutionHandler&);

public:
   typedef enum  {
      NoGroup = -1, PointGroup = 0, LineGroup = 1, PolygonGroup = 2,
      RasterGroup = 3, GeometryCollectionGroup = 4
   } LayerGroup;
   /** Constructor **/
   AddLayerCommandExecutionHandler(DataViewManager* pDataView);
   /** Destructor **/
   virtual ~AddLayerCommandExecutionHandler();
   /**
    * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
    * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    * @return true en caso de poder manejar la ejecucion del comando
    * @return false en caso contrario
    */
	virtual bool Execute(const Command* pCommand, GenericTool* pTool);

protected:
	/** Metodo auxiliar encargado de agregar de forma concreta una capa al arbol **/
	void AddLayer(const std::string& Filter);

	DataViewManager* pDataView_;
};

}  /** namespace core **/
}  /** namespace suri **/
#endif
