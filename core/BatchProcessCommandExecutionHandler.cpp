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

// Include estandard
#include <string>

// Includes Suri
#include "suri/BatchProcessCommandExecutionHandler.h"
#include "suri/DataViewManager.h"
#include "suri/Command.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/GenericTool.h"
#include "suri/xmlnames.h"
#include "suri/ProcessInterface.h"
#include "suri/ProcessFactory.h"
#include "suri/ProcessCreatorInterface.h"
#include "suri/FileExporterProcess.h"

#include "wx/sstream.h"

namespace suri {

namespace core {
   
/** Constructor **/
BatchProcessCommandExecutionHandler::BatchProcessCommandExecutionHandler(DataViewManager* pDataView) 
									: pDataView_(pDataView) {
}

/** Destructor **/
BatchProcessCommandExecutionHandler::~BatchProcessCommandExecutionHandler() {
}
/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool BatchProcessCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   
   /** TODO(Gabriel - TCK #7139): En la inicializacion de la aplicacion deberia configurarse
    * para registrar los creadores de procesos correspondientes.
   **/
	if (pCommand->GetId() != pTool->GetId("BatchProcess"))
		return false; 
   std::string filename;
   wxFileDialog dlg(NULL, _("Seleccione el archivo de configuracion de proceso"), wxT(""), wxT(""),
                    _("Archivos XML (*.xml)|*.xml"));
   if (dlg.ShowModal() != wxID_OK) return true;

   ProcessInterface* pprocess = NULL;
   wxXmlDocument doc;
   if (doc.Load(dlg.GetPath())) {
      // configura el proceso en funcion de los parametros del proceso
      wxXmlNode* proot = doc.GetRoot();
      wxXmlNode* pchild = proot->GetChildren();
      while (pchild && pchild->GetName().compare(PROCESS_ADAPT_LAYER_NODE) == 0){
         ProcessAdaptLayer* pPal = (ProcessAdaptLayer*)
               SerializableFactory::GetSerializable(pchild);
         if (pPal){
            std::string processName = pPal->GetProcessName();
            pprocess = suri::process::ProcessFactory::
                  GetInstance()->Create(processName, pDataView_);
            pprocess->SetAdaptLayer(pPal);
         }
         pchild = pchild->GetNext();
      }
   }      
   wxStringOutputStream stream;
   doc.Save(stream);
   if (pprocess &&pprocess->RunProcess()) {
      SHOW_INFO("Proceso exitoso");
   } else {
      SHOW_ERROR("Proceso mal configurado");
   }
   return true;
}


}  /** namespace core **/
}  /** namespace suri **/
