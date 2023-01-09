/*! \file ProcessFactory.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
