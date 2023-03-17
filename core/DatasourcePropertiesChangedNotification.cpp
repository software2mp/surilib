/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
