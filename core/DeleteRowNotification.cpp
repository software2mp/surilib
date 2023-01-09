/** \file DeleteRowNotification.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
