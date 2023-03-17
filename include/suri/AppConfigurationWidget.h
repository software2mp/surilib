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

#ifndef APPCONFIGURATIONWIDGET_H_
#define APPCONFIGURATIONWIDGET_H_

#include "suri/NotebookWidget.h"
#include "AppConfigPartsProviderInterface.h"

/** namespace suri */
namespace suri {

// forwards
class DataViewManager;

DECLARE_EVENT_OBJECT(AppConfigurationWidgetEvent);

/**
 * Clase encargada de administrar los parts de configuracion general de la aplicacion.
 * Hereda de NotebookWidget para aprovechar el comportamiento de Wizard que tiene el mismo. 
**/
class AppConfigurationWidget : public NotebookWidget {   
   /* Constructor copia. */
   AppConfigurationWidget(const AppConfigurationWidget &Appconfigurationwidget);

public:
   /* Constructor. */
   AppConfigurationWidget(AppConfigPartsProviderInterface* pappconfigparts,
         wxString Titulo, DataViewManager* pDataviewmanager);
   /* Destructor */
   virtual ~AppConfigurationWidget();
   /** Commit de los cambios efectuados. */
   virtual bool ApplyChanges();
   /** Inserta los parts necesarios para la configuracion de la aplicacion */
   virtual bool InsertParts();
   /** Evento click sobre Boton Ok */
   virtual void OnButtonOk(wxCommandEvent &Event);

private:
   /** Proveedor de parts de configuracion **/
   AppConfigPartsProviderInterface* pAppconfigparts_;
   /** Administrador general de la aplicacion **/
   DataViewManager* pDataviewmanager_;
   EVENT_OBJECT_PROPERTY(AppConfigurationWidgetEvent); /*! Objeto para conectar */
};

} // End namespace.

#endif /* APPCONFIGURATIONWIDGET_H_ */
