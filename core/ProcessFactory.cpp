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

#include <map>
#include <string>

#include "suri/ProcessCreatorInterface.h"
#include "suri/ProcessFactory.h"

namespace suri {

namespace process {

/** 
 * Metodo que registra un creador asociando al id de proceso indicado
 * @param[in] ProcessId id de proceso al cual se asociara el creador
 * @param[in] pCreator instancia de creacion de procesos
 **/
void ProcessFactory::RegisterCreator(const std::string& ProcessId,
                                     ProcessCreatorInterface* pCreator) {
   creators_.insert(std::pair<std::string, ProcessCreatorInterface*>(ProcessId, pCreator));
}

/** 
 * Metodo encargado de generar una instancia de proceso en funcion del id que se 
 * pasa por parametro 
 * @param[in] ProcessId Id del proceso sobre el cual se requiere una nueva instancia 
 * @return ProcessInterface* Instancia del proceso
 * @return NULL en caso de que se encuentra un creador para el tipo de proceso solicitado
 **/
ProcessInterface* ProcessFactory::Create(const std::string& ProcessId,
                                         DataViewManager* pDataViewManager) const {
   ProcessInterface* pprocess = NULL;
   std::map<std::string, ProcessCreatorInterface*>::const_iterator creator = creators_.find(ProcessId);
   if (creator != creators_.end())  
      pprocess = creator->second->Create(pDataViewManager);
   return pprocess;
}

/** 
 * Metodo estatico que devuelve la instancia vigente del factory de process. Utiliza
 * la estructura de singleton.
**/
ProcessFactory* ProcessFactory::GetInstance() {
   static ProcessFactory* pfactory = new ProcessFactory;
   return pfactory;
}

}  /** namespace process **/
}  /** namespace suri **/
