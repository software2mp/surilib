/*! \file DatasourcePropertiesChangedNotification.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>

// Includes Suri
#include "DatasourcePropertiesChangedNotification.h"
#include "DefaultViewcontext.h"
#include "suri/LayerInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
DatasourcePropertiesChangedNotification::DatasourcePropertiesChangedNotification(
      DatasourceInterface *pDatasource, const std::string &Value, PropertiesEnum Property) :
      pDatasource_(pDatasource), property_(Property), value_(Value) {
}

/**
 * Desctructor
 */
DatasourcePropertiesChangedNotification::~DatasourcePropertiesChangedNotification() {
}

/** Metodo que ejecuta una accion sobre el observador a ser notificado
 * @param[in] pObserver referencia al observador a notificar el cambio
 * @return true en caso de poder notificar correctamente el cambio en las propiedades
 * @return false en caso de no poder notificar correctamente el cambio en las propiedades
 */
bool DatasourcePropertiesChangedNotification::ApplyNotification(ObserverInterface* pObserver) {
   // Es necesario realizar el casteo hace un viewcontext, ya que es necesario
   // aplicar la notificacion sobre una instancia de viewcontext y no sobre cualquier
   // obeserver
   ViewcontextInterface* pviewcontext = dynamic_cast<ViewcontextInterface *>(pObserver);
   if (pviewcontext == NULL) {
      return true;
   }
   wxString wstringvalue(value_);
   LayerInterface* player = pviewcontext->GetAssociatedLayer(pDatasource_->GetId());
   if (player == NULL)
      return true;
   Element* pelement = player->GetElement();
   switch (property_) {
      case Name:
         pelement->SetName(wstringvalue);
         break;
      case Copyright:
         pelement->SetCopyRight(wstringvalue);
         break;
      case Description:
         pelement->SetDescription(wstringvalue);
         break;
      case Url:
         pelement->SetUrl(wstringvalue);
         break;
      case Icon:
         pelement->SetIcon(wstringvalue);
         break;
      default:
         return false;
   }
   return true;
}

} /** namespace suri */
