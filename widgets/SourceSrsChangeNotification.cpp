/** \file SourceSrsChangeNotification.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "SourceSrsChangeNotification.h"
#include "suri/GeoreferenceGcpDriver.h"
#include "TablePart.h"

namespace suri {

/** Ctor */
SourceSrsChangeNotification::SourceSrsChangeNotification(
                                    const std::string &SourceSrs) {
   sourceSrs_ = SourceSrs;
}

/** Dtor */
SourceSrsChangeNotification::~SourceSrsChangeNotification() {
}

/**
 * Metodo que ejecuta una accion sobre el observador a ser notificado
 * @param[in] pObserver Observador que escucha la notificacion
 * @return true en caso de haber notificado correctamente, false en otro caso
 */
bool SourceSrsChangeNotification::ApplyNotification(ObserverInterface* pObserver) {
   bool retval = false;
   GeoreferenceGcpDriver* pdriver = dynamic_cast<GeoreferenceGcpDriver*>(pObserver);
   if (pdriver != NULL) {
      pdriver->SetGcpSourceSrs(sourceSrs_);
      retval = true;
   } else {
      TablePart* ptable = dynamic_cast<TablePart*>(pObserver);
      if (ptable != NULL) {
         ptable->SetSrs(sourceSrs_);
         retval = true;
      }
   }
   return retval;
}

} /* namespace suri */
