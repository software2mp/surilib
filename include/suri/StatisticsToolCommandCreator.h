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

#ifndef __STATISTICS_TOOL_COMMAND_CREATOR_H__
#define __STATISTICS_TOOL_COMMAND_CREATOR_H__

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
 *  el calculo de estadisticas globales sobre una capa raster
 **/
class GlobalStatisticsCommandCreator : public CommandCreatorInterface {
	/** ctor copia **/
	GlobalStatisticsCommandCreator(const GlobalStatisticsCommandCreator&);

public:
	/** constructor **/
	GlobalStatisticsCommandCreator();
	/** destructor **/
	~GlobalStatisticsCommandCreator();

private:
	/** 
	 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
    * del comando sea del dominio de la clase. 
    **/
	virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/** Clase que encargada de configurar el Command para 
 *  el calculo de estadisticas clasificadas sobre una capa raster
 **/
class ClassStatisticsCommandCreator : public CommandCreatorInterface {
	/** ctor copia **/
	ClassStatisticsCommandCreator(const ClassStatisticsCommandCreator&);

public:
	/** constructor **/
	ClassStatisticsCommandCreator();
	/** destructor **/
	~ClassStatisticsCommandCreator();

private:
	/** 
	 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
    * del comando sea del dominio de la clase. 
    **/
	virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/** Clase que encargada de configurar el Command para la exportacion de estadisticas 
 *  de una capa raster
 **/
class ClassStatisticsExporterCommandCreator : public CommandCreatorInterface {
	/** ctor copia **/
	ClassStatisticsExporterCommandCreator(const ClassStatisticsExporterCommandCreator&);

public:
	/** constructor **/
	ClassStatisticsExporterCommandCreator();
	/** destructor **/
	~ClassStatisticsExporterCommandCreator();

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
