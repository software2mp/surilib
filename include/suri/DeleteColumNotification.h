/*! \file DeleteColumNotification.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DELETECOLUMNOTIFICATION_H_
#define DELETECOLUMNOTIFICATION_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/NotificationInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Clase que representa una notificacion de eliminacion de columna **/
class DeleteColumNotification : public suri::NotificationInterface {
   /** Ctor copia **/
   DeleteColumNotification(const DeleteColumNotification&);

public:
   /** Constructor **/
   DeleteColumNotification(int ColId, const std::string& ColumnName);
   /** Destructor **/
   virtual ~DeleteColumNotification();
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

private:
   /** id de la columna eliminada **/
   int colId_;
   /** nombre de la columna eliminada **/
   std::string columnName_;
};

} /** namespace suri */

#endif /* DELETECOLUMNOTIFICATION_H_ */
