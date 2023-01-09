/*! \file ModelOrderChangeNotification.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "ModelOrderChangeNotification.h"

// Includes Estandar
// Includes Suri
#include "suri/GcpDriver.h"
// Includes Wx
// Defines
// forwards

namespace suri {
/**
 * ctro
 */
ModelOrderChangeNotification::ModelOrderChangeNotification(
      ModelOrderWidget *pOrderWidget, int Order) :
      order_(Order), pOrderWidget_(pOrderWidget) {
}
/**
 * dtro
 */
ModelOrderChangeNotification::~ModelOrderChangeNotification() {
}

/**
 * Metodo que ejecuta una accion sobre el observador a ser notificado
 * @param[in] pObserver Observador que escucha la notificacion
 * @return true en caso de haber notificado correctamente, false en otro caso
 */
bool ModelOrderChangeNotification::ApplyNotification(ObserverInterface* pObserver) {
   GcpDriver* pGcpDriver = dynamic_cast<GcpDriver*>(pObserver);
   if (pGcpDriver == NULL)
      return false;
   pGcpDriver->SetOrderModel(order_);
   rmse_ = pGcpDriver->GetRmse();
   bool returnvalue = rmse_ != std::numeric_limits<double>::min();
   if (returnvalue && pOrderWidget_) {
      pOrderWidget_->SetRmse(rmse_);
   }
   return returnvalue;
}
/**
 * Metodo que devuelve el valor RMSE por aproximacion polinomica
 * @return valor del rmse de la aproximacion polinomica
 */
double ModelOrderChangeNotification::GetRmse() {
   return rmse_;
}

} /** namespace suri */
