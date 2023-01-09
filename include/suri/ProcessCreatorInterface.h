/*! \file ProcessCreatorInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef __PROCESS_CREATOR_INTERFACE_H__
#define __PROCESS_CREATOR_INTERFACE_H__

namespace suri {

// Forwards
class ProcessInterface;
class DataViewManager;

namespace process {

/** Interfaz que define el comportamiento esperado para todo credor de procesos . **/
class ProcessCreatorInterface {
   /** ctor copia **/
   ProcessCreatorInterface(const ProcessCreatorInterface&);
public:
   /** ctor default **/
   ProcessCreatorInterface(){}
   /** dtor **/
   virtual ~ProcessCreatorInterface() {}
   /** Crea una instancia del proceso asociado al creador concreto 
    * @rertun ProcessInterface nueva instancia de proceso 
   **/
   virtual ProcessInterface* Create(DataViewManager* pDataViewManager)const=0;
};

}  /** namespace process **/
}  /** namespace suri **/
#endif
