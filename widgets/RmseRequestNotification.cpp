/*! \file RmseRequestNotification.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


// Includes Estandar
// Includes Suri
#include "RmseRequestNotification.h"
#include "suri/GcpDriver.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/**
 * Constructor
 * @param pOrderWidget referencia al widget que se encarga de mostrar el orden
 * y error de la transformacion que se esta utilizando
 */
RmseRequestNotification::RmseRequestNotification(ModelOrderWidget *pOrderWidget) :
      pOrderWidget_(pOrderWidget) {
}

/** Destructor **/
RmseRequestNotification::~RmseRequestNotification() {
}

/**
 * Metodo que ejecuta una accion sobre el observador a ser notificado
 * @param[in] pObserver Observador que escucha la notificacion
 * @return true en caso de haber notificado correctamente, false en otro caso
 */
bool RmseRequestNotification::ApplyNotification(ObserverInterface* pObserver) {
   GcpDriver* pGcpDriver = dynamic_cast<GcpDriver*>(pObserver);
   if (pGcpDriver == NULL)
      return false;
   pOrderWidget_->SetRmse(pGcpDriver->GetRmse());
   return true;
}
} /** namespace suri */
