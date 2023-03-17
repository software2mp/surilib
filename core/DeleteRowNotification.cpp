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

#include <vector>

#include "DeleteRowNotification.h"
#include "suri/TableEditionTask.h"
#include "suri/VectorEditionTask.h"

namespace suri {

/**
 * Ctor.
 */
DeleteRowNotification::DeleteRowNotification() : pTag_(NULL) {
}

/**
 * Dtor.
 */
DeleteRowNotification::~DeleteRowNotification() {
}

/**
 * Metodo que ejecuta una accion sobre el observador a ser notificado
 */
bool DeleteRowNotification::ApplyNotification(ObserverInterface* pObserver) {
   VectorEditionTask* pvectoreditiontask = dynamic_cast<VectorEditionTask*>(pObserver);
   if (pvectoreditiontask) {
      GenericParameter<int>* fid = dynamic_cast<GenericParameter<int>*>(GetTag());
      if (!pvectoreditiontask->IsModifiedFeature(fid->GetValue())) {
         pvectoreditiontask->AddDeletedFeature(fid->GetValue());
      } else {
         pvectoreditiontask->RemoveModifiedFeature(fid->GetValue());
      }
      pvectoreditiontask->GetFeatureSelector()->ClearSelection();
      pvectoreditiontask->UpdateViewers();
   }

   TableEditionTask* peditionobserver = dynamic_cast<TableEditionTask*>(pObserver);
   if (peditionobserver) {
      if (!peditionobserver->EndFeatureEdition(true)) {
         peditionobserver->EndFeatureEdition(false);
         REPORT_DEBUG("D: No se pudo finalizar la edicion del feature editado.");
      }
   }

   return peditionobserver != NULL;
}

/**
 * Obtiene el dato extra pasado a la instancia
 */
BaseParameter* DeleteRowNotification::GetTag() {
   return pTag_;
}

/**
 * Establece el dato extra para la instancia
 */
void DeleteRowNotification::SetTag(BaseParameter* pTag) {
   pTag_ = pTag;
}

}  // namespace suri
