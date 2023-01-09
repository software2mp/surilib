/*! \file ProcessFactory.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#ifndef __PROCESS_FACTORY_H__
#define __PROCESS_FACTORY_H__

#include <map>
#include <string>

#define REGISTER_PROCESS_CREATOR(M_ProcessName, M_CreatorClass) \
namespace { \
class _M_Register_Process_ { \
public: \
   _M_Register_Process_() { \
      suri::process::ProcessFactory::GetInstance()->RegisterCreator \
               (#M_ProcessName, new M_CreatorClass()); \
   } \
}; \
_M_Register_Process_ _m_register_process_; \
} \

namespace suri {

// Forwards
class ProcessInterface;
class DataViewManager;

namespace process {

// Forwards
class ProcessCreatorInterface;

/** Clase encargada de administrar el conjunto de creadores de procesos y 
 * de crear un proceso en funcion del creador correspondiente.  
 **/
class ProcessFactory {
   /** ctor copia **/
   ProcessFactory(const ProcessFactory&);

public:
   /** 
    * Metodo que registra un creador asociando al id de proceso indicado
    * @param[in] ProcessId id de proceso al cual se asociara el creador
    * @param[in] pCreator instancia de creacion de procesos
    **/
   void RegisterCreator(const std::string& ProcessId,
                        ProcessCreatorInterface* pCreator);
   /** 
    * Metodo encargado de generar una instancia de proceso en funcion del id que se 
    * pasa por parametro 
    * @param[in] ProcessId Id del proceso sobre el cual se requiere una nueva instancia 
    * @return ProcessInterface* Instancia del proceso
    * @return NULL en caso de que se encuentra un creador para el tipo de proceso solicitado
    **/
   ProcessInterface* Create(const std::string& ProcessId,
                            DataViewManager* pDataViewManager) const;
   /** 
    * Metodo estatico que devuelve la instancia vigente del factory de process. Utiliza
    * la estructura de singleton.
    **/
   static ProcessFactory* GetInstance();

private:
   ProcessFactory() {}
   /** creadores registrados **/
   std::map<std::string, ProcessCreatorInterface*> creators_;
};
}  /** namespace process **/
}  /** namespace suri **/
#endif
