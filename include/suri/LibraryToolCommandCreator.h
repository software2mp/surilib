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

#ifndef __LIBRARY_TOOL_COMMAND_CREATOR_H__
#define __LIBRARY_TOOL_COMMAND_CREATOR_H__

// Includes Estandard
#include <string>

// Includes Suri
#include "suri/CommandCreatorInterface.h"

namespace suri {

// forwards
class Command;
class ToolInterface;

namespace core {


/** Clase que encargada de configurar el Command para la administracion de la biblioteca de
 *  sistemas de referencia
 **/
class SpatialReferenceLibraryCommandCreator : public CommandCreatorInterface {
	/** 
	 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
    * del comando sea del dominio de la clase. 
    **/
	virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};
/** Clase que encargada de configurar el Command para la administracion de la biblioteca de datums
 * de la aplicacion
 **/
class DatumLibraryCommandCreator : public CommandCreatorInterface {
private:
	/** 
	 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
    * del comando sea del dominio de la clase. 
    **/
	virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};
/** Clase que encargada de configurar el Command para la administracion de la biblioteca de 
 *  elipsoides de la aplicacion
 **/
class EllipsoidLibraryCommandCreator : public CommandCreatorInterface {
private:
	/** 
	 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
    * del comando sea del dominio de la clase. 
    **/
	virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/** Clase que encargada de configurar el Command para la administracion de la biblioteca de 
 *  tabla de intervalos de la aplicacion
 **/
class DensitySliceLibraryCommandCreator : public CommandCreatorInterface {
private:
	/** 
	 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
    * del comando sea del dominio de la clase. 
    **/
	virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/** Clase que encargada de configurar el Command para la administracion de la biblioteca de 
 *  servidores wms de la aplicacion
 **/
class WmsLibraryCommandCreator : public CommandCreatorInterface {
private:
	/** 
	 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
    * del comando sea del dominio de la clase. 
    **/
	virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/** Clase que encargada de configurar el Command para la administracion de la biblioteca de 
 *  estilos vectoriales de la aplicacion
 **/
class VectorStyleLibraryCommandCreator : public CommandCreatorInterface {
private:
	/** 
	 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
    * del comando sea del dominio de la clase. 
    **/
	virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/** Clase que encargada de configurar el Command para la administracion de la biblioteca de 
 *  firmas espectrales vectoriales de la aplicacion
 **/
class SpectralSignLibraryCommandCreator : public CommandCreatorInterface {
private:
	/** 
	 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
    * del comando sea del dominio de la clase)
    **/
	virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/** Clase que encargada de configurar el Command para la administracion de la biblioteca de
 *  filtros de convolucion de la aplicacion
 **/
class ConvolutionfiltersLibraryCommandCreator : public CommandCreatorInterface {
private:
   /**
    * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
    * del comando sea del dominio de la clase.
    **/
   virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/**
 * Clase que encargada de configurar el Command para la administracion de la biblioteca de
 * ecuaciones de la aplicacion
 */
class EquationLibraryCommandCreator : public CommandCreatorInterface {
private:
   /**
    * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
    * del comando sea del dominio de la clase)
    */
   virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

class IndexLibraryCommandCreator : public CommandCreatorInterface {
private:
   /**
    * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
    * del comando sea del dominio de la clase)
    */
   virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};
}  /** namespace core **/
}  /** namespace suri **/

#endif
