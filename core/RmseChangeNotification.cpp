/*! \file RmseChangeNotification.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "RmseChangeNotification.h"
#include "TablePart.h"
// Includes Wx
// Defines
// forwards

namespace suri {
/**
 * ctor
 */
RmseChangeNotification::RmseChangeNotification(double Rmse) : rmse_(Rmse){
   // TODO Auto-generated constructor stub
}
/**
 * dtor
 */
RmseChangeNotification::~RmseChangeNotification() {
   // TODO Auto-generated destructor stub
}

/**
 * Aplica la notificacion en el observer
 * @param puntero al observer de la notificacion
 */
bool RmseChangeNotification::ApplyNotification(ObserverInterface* pObserver) {
   TablePart* tablepart = dynamic_cast<TablePart*>(pObserver);
   if (tablepart == NULL)
      return false;
   ModelOrderWidget* modelorder = tablepart->GetModelOrderWidget();
   double presentrmse = modelorder->GetRmse();
   if (presentrmse != rmse_ )
      modelorder->SetRmse(rmse_);
   return true;
}

} /** namespace suri */
