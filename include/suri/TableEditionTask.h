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

#ifndef TABLEEDITIONTASK_H_
#define TABLEEDITIONTASK_H_

// Includes standard
#include <string>
#include <map>
#include <vector>
#include <limits>

// Includes suri
#include "suri/TaskInterface.h"
#include "suri/ObserverInterface.h"
#include "suri/FeatureSelection.h"

#define INVALID_FEATURE_ID std::numeric_limits<int>::min()

namespace suri {

class GeometryColumnEditionState;

/**
 * Tarea que permite modificar una tabla que contiene uno o mas campos de
 * tipo geometria.
 */
class TableEditionTask : public TaskInterface, public ObserverInterface {
public:
   // static const int INVALID_FEATURE_ID = -2147483648;

   /** Ctor */
   TableEditionTask();
   /** Dtor */
   virtual ~TableEditionTask();

   /** Retorna clase que administra la seleccion de geometrias. */
   virtual FeatureSelection* GetFeatureSelector();
   /** Retorna clase que administra la seleccion de geometrias. */
   virtual bool SetFeatureSelector(FeatureSelection* pFeatureSelector);
   /** Retorna la tabla en edicion */
   Table* GetTable();
   /** Configura la tabla a editar */
   bool SetTable(Table* pTable);

   /** Comienza la tarea */
   virtual bool Start();
   /** Finaliza la tarea. Intenta guardar los cambios. */
   virtual bool End();
   /** Cancela la tarea sin guardar */
   virtual bool Cancel();
   /** Indica si la tarea esta activa */
   virtual bool IsActive() const;

   /** Inicia la edicion de un feature existente */
   virtual bool StartFeatureEdition(FeatureIdType FeatureId);
   /** Crea un registro en la tabla e inicia su edicion. */
   virtual bool StartNewFeatureEdition();
   /** Finaliza la edicion de la geometria editada. */
   virtual bool EndFeatureEdition(bool SaveChanges = true);
   /** Indica si esta editando un feature. */
   virtual bool IsEditingFeature();
   /** Retorna el id del feature en edicion. */
   virtual long GetEditedFeatureId();
   /** Retorna la geometria que se esta editando */
   virtual Geometry* GetEditedGeometry(std::string GeometryColumnName);
   /** Pisa la geometria existente en la columna. */
   virtual bool SetEditedGeometry(std::string GeometryColumnName,
                                                         Geometry* pGeometry);

   /** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
   virtual void Update(NotificationInterface* pNotification);

   /** Nombre asignado a la tarea. */
   static const std::string TASKNAME;

protected:
   /** Configura las columnas de tipo geometria que estan en la tabla. */
   virtual bool ConfigureGeometryColumnEditors();

   /** Funcion que indica si se puede iniciar la edicion */
   virtual bool CanStart();
   /** Retorna bool que indica si la edicion es valida. */
   virtual bool ValidateEdition();
   /** Finaliza la edicion de todas las geometrias editadas */
   virtual void EndTableEdition();
   /** Crea el SelectionSource que hay que configurar al iniciar la tarea */
   virtual FeatureSelectionSource* CreateFeatureSelectionSource();

   /** Tabla que se esta editando */
   Table* pTable_;
   /** Clase que administra la seleccion */
   FeatureSelection* pFeatureSelection_;

   /** bool que indica si la tarea esta activa */
   bool isActive_;
   /** Mapa que se usa para guardar el estado de edicion de las geometrias */
   typedef std::map<std::string, GeometryColumnEditionState*> GeometryColumnEditorMap;
   /** Mapa que contiene estado de corregistro para origen y destino */
   GeometryColumnEditorMap pGeometryColumnEditorMap_;
   /** Id del feature en edicion */
   long editedFeatureId_;

   /** Id del observer en notificador de cambios de Tabla */
   int observerId_;
};

} /* namespace suri */

#endif /* TABLEEDITIONTASK_H_ */
