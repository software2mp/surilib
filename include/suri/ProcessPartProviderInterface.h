/*! \file ProcessPartProviderInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef PROCESSPARTPROVIDERINTERFACE_H_
#define PROCESSPARTPROVIDERINTERFACE_H_


// Includes Estandar
#include <vector>
#include <string>

// Includes Suri
#include "FactoryHelpers.h"

// Includes Wx
// Defines

namespace suri {

// forwards
class Part;
class ProcessAdaptLayer;

/** Interfaz que representa el comportamiento que
 *  posee cualquier proceso dentro del framework. **/
class ProcessPartProviderInterface {
   /** ctor copia **/
   ProcessPartProviderInterface(const ProcessPartProviderInterface&);
public:
   /** Metodo para la creacion de un Part provider
    *  @return instancia del creador correspondiente
    *  @return NULL en caso contrario
    */
   static ProcessPartProviderInterface* Create(
         const std::string& ProcessName, ProcessAdaptLayer* pPal) {
      return NULL;
   }

   /** dtor **/
   virtual ~ProcessPartProviderInterface() {}
   /** Obtiene los parts necesarios para la configuracion visual del proceso */
   virtual bool GetParts(std::vector<Part*> &ProcessParts)=0;

protected:
   /** ctor **/
   explicit ProcessPartProviderInterface(ProcessAdaptLayer* pPal) : pPal_(pPal) {}
   /** Capa de adaptacion de proceso que contiene los atributos */
   ProcessAdaptLayer* pPal_;
};

}  /** namespace suri **/

#endif /* PROCESSPARTPROVIDERINTERFACE_H_ */
