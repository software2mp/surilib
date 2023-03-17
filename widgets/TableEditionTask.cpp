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

// Includes standard
#include <string>
#include <utility>
#include <vector>

// Includes suri
#include "suri/TableEditionTask.h"
#include "GeometryColumnEditionState.h"
#include "suri/messages.h"

namespace suri {

/** Ctor */
TableEditionTask::TableEditionTask() {
   editedFeatureId_ = INVALID_FEATURE_ID;
   isActive_ = false;
   pTable_ = NULL;
   pFeatureSelection_ = NULL;
}

/** Dtor */
TableEditionTask::~TableEditionTask() {
}

/** Retorna clase que administra la seleccion de geometrias. */
FeatureSelection* TableEditionTask::GetFeatureSelector() {
   return pFeatureSelection_;
}

/** Retorna clase que administra la seleccion de geometrias. */
bool TableEditionTask::SetFeatureSelector(FeatureSelection* pFeatureSelector) {
   if (IsActive())
      return false;
   pFeatureSelection_ = pFeatureSelector;
   return true;
}

/**
 * Retorna la tabla en edicion
 * @return tabla en edicion.
 */
Table* TableEditionTask::GetTable() {
   return pTable_;
}

/**
 * Configura la tabla a editar
 * @param[in] pTable tabla que se va a usar en la tarea
 * \attention La tarea toma posecion de la tabla. No eliminar fuera de la misma
 * @return bool que indica si se pudo configurar la tabla.
 */
bool TableEditionTask::SetTable(Table* pTable) {
   if (IsActive())
      return false;
   delete pTable_;
   pTable_ = pTable;
   return true;
}


/**
 * Comienza la tarea
 * @return bool que indica si se pudo iniciar la tarea
 */
bool TableEditionTask::Start() {
   if (!CanStart())
      return false;

   if (!ConfigureGeometryColumnEditors())
      return false;

   if (!pFeatureSelection_->Configure(CreateFeatureSelectionSource())) {
      EndTableEdition();
      return false;
   }
   observerId_ = pTable_->GetDataChangeNotifier()->Attach(this);

   isActive_ = true;
   return true;
}

/**
 * Finaliza la tarea. Intenta guardar los cambios.
 * @return bool que indica si se pudieron guardar los cambios
 */
bool TableEditionTask::End() {
   if (!IsActive())
      return false;

   EndTableEdition();

   return pTable_->Commit();
}

/**
 * Cancela la tarea sin guardar
 * @return siempre true
 */
bool TableEditionTask::Cancel() {
   EndTableEdition();
   return true;
}

/**
 * Indica si la tarea esta activa
 * @return bool que indica la tarea esta activa
 */
bool TableEditionTask::IsActive() const {
   return isActive_;
}

/**
 * Inicia la edicion de la geometria asociada al feature
 * @param[in] FeatureId id del feature que se desea editar.
 * @return bool que indica si se inicio la edicion correctamente.
 */
bool TableEditionTask::StartFeatureEdition(FeatureIdType FeatureId) {
   if (IsEditingFeature())
      return false;

   bool result = true;
   GeometryColumnEditorMap::iterator it = pGeometryColumnEditorMap_.begin();
   for (; it != pGeometryColumnEditorMap_.end() && result; ++it)
      result = it->second->StartFeatureEdition(FeatureId);

   editedFeatureId_ = result ? FeatureId : INVALID_FEATURE_ID;

   return result;
}

/**
 * Agrega una fila a la tabla
 * @return bool que indica si pudo agregar la fila
 */
bool TableEditionTask::StartNewFeatureEdition() {
   if (pTable_->GetTableReadOnlyState()){
      SHOW_ERROR(message_ERROR_EDITING_READ_ONLY_TABLE);
      return false;
   }
   if (IsEditingFeature())
      return false;

   return pTable_->AppendRow() && IsEditingFeature();
}

/**
 * Finaliza la edicion de la geometria editada
 * @return bool que indica si pudo finalizar la edicion
 */
bool TableEditionTask::EndFeatureEdition(bool SaveChanges) {
   if (!IsEditingFeature())
      return true;

   bool returnvalue = ValidateEdition();
   GeometryColumnEditorMap::iterator it = pGeometryColumnEditorMap_.begin();
   for (; it != pGeometryColumnEditorMap_.end(); ++it)
      returnvalue = it->second->EndFeatureEdition(returnvalue);

   editedFeatureId_ = INVALID_FEATURE_ID;

   pFeatureSelection_->ClearSelection();

   if (!returnvalue)
      pTable_->DeleteRow(pTable_->GetRows() - 1);

   return returnvalue;
}

/**
 * Indica si esta editando un feature.
 * @return true si se esta editando un feature
 */
bool TableEditionTask::IsEditingFeature() {
   return IsActive() && editedFeatureId_ != INVALID_FEATURE_ID;
}

/**
 * Retorna el id del feature en edicion.
 * @return id del feature en edicion.
 * @return INVALID_FEATURE_ID si no hay feature en edicion
 */
long TableEditionTask::GetEditedFeatureId() {
   return editedFeatureId_;
}

/** Retorna la geometria que se esta editando */
Geometry* TableEditionTask::GetEditedGeometry(std::string GeometryColumnName) {
   if (!IsEditingFeature())
      return NULL;

   GeometryColumnEditorMap::iterator it = pGeometryColumnEditorMap_.find(GeometryColumnName);
   return (it != pGeometryColumnEditorMap_.end()) ? it->second->GetEditedGeometry() : NULL;
}

/** Pisa la geometria existente en la columna. */
bool TableEditionTask::SetEditedGeometry(std::string GeometryColumnName,
                                                      Geometry* pGeometry) {
   if (!IsEditingFeature())
      return false;

   GeometryColumnEditorMap::iterator it = pGeometryColumnEditorMap_.find(GeometryColumnName);

   return (it != pGeometryColumnEditorMap_.end()) ?
         it->second->SetEditedGeometry(pGeometry) : false;
}

/** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
void TableEditionTask::Update(NotificationInterface* pNotification) {
   pNotification->ApplyNotification(this);
}

/**
 * Configura las columnas de tipo geometria que estan en la tabla.
 * El comportamiento default es que no hay columnas con geometrias en la
 * tabla.
 * @return siempre true
 */
bool TableEditionTask::ConfigureGeometryColumnEditors() {
   return true;
}


/**
 * Funcion que indica si se puede iniciar la edicion
 * @return bool que indica si se puede iniciar la edicion
 */
bool TableEditionTask::CanStart() {
   return !IsActive() && pTable_ != NULL;
}

/**
 * Analiza si la edicion realizada es valida.
 * @return bool que indica si la edicion es valida.
 */
bool TableEditionTask::ValidateEdition() {
   bool validedition = IsEditingFeature();
   GeometryColumnEditorMap::iterator it = pGeometryColumnEditorMap_.begin();
   for (; it != pGeometryColumnEditorMap_.end() && validedition; ++it)
      validedition = it->second->IsEditing() ? it->second->IsValid() : true;
   return validedition;
}

/** Finaliza la edicion de todas las geometrias editadas */
void TableEditionTask::EndTableEdition() {
   if (!IsActive())
      return;
   EndFeatureEdition();
   isActive_ = false;
   pTable_->GetDataChangeNotifier()->Detach(observerId_);
   pFeatureSelection_->Configure(NULL);
   GeometryColumnEditorMap::iterator it = pGeometryColumnEditorMap_.begin();
   for (; it != pGeometryColumnEditorMap_.end(); ++it)
      delete it->second;
   pGeometryColumnEditorMap_.clear();
}

/**
 * Crea el SelectionSource que hay que configurar al iniciar la tarea
 * @return instancia de FeatureSelectionSource. Puede ser null.
 * \attention El puntero retornado es responsabilidad del invocante.
 */
FeatureSelectionSource* TableEditionTask::CreateFeatureSelectionSource() {
   return NULL;
}

} /* namespace suri */
