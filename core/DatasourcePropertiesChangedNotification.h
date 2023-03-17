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

#ifndef DATASOURCEPROPERTIESCHANGEDNOTIFICATION_H_
#define DATASOURCEPROPERTIESCHANGEDNOTIFICATION_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/NotificationInterface.h"
#include "suri/DatasourceInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Notificacion de cambio en las propiedades
 */
class DatasourcePropertiesChangedNotification : public suri::NotificationInterface {
public:
   /** Enumerado del tipo de propiedad */
   typedef enum {
      Name = 0, Copyright = 1, Description = 2, Url = 3, Icon = 4
   } PropertiesEnum;
   /** Constructor */
   DatasourcePropertiesChangedNotification(DatasourceInterface *pDatasource,
                                           const std::string &Value,
                                           PropertiesEnum Property = Name);
   /** Destructor */
   virtual ~DatasourcePropertiesChangedNotification();
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

private:
   DatasourceInterface* pDatasource_;        /*! Puntero al datasource */
   PropertiesEnum property_;                 /*! Propiedad */
   std::string value_;                       /*! Valor */
};

} /** namespace suri */

#endif /* DATASOURCEPROPERTIESCHANGEDNOTIFICATION_H_ */
