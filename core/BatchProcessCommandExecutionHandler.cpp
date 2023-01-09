/*! \file BatchProcessCommandExecutionHandler.cpp */

/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib. 
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L. 
 *                                                                                                  
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/) 
 *  
 */

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
