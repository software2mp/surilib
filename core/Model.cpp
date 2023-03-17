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
#include <list>
#include <set>
#include <vector>

// Includes suri
#include "View.h"
#include "Model.h"

// Includes Wx

// Includes App

// Defines

/** Modelos validos */
std::set<Model*> Model::models_;

// ------------------------------------------------------------------------------
/** Constructor */
Model::Model() :
      preventNextViewerUpdate_(false), blockViewerUpdate_(0), hasChanged_(false),
      pUpdatedElement_(NULL), destroying_(false) {
   models_.insert(this);
}   // isUpdating_(false),NO USADO
Model::Model(const Model &M) :
      preventNextViewerUpdate_(false), blockViewerUpdate_(0), hasChanged_(false),
      pUpdatedElement_(NULL), destroying_(false) {
   models_.insert(this);
   viewers_.clear();
   uniqueviewers_.clear();
};

// ------------------------------------------------------------------------------
/** destructor */
Model::~Model() {
   models_.erase(this);
   destroying_ = true;
}

// ------------------------------------------------------------------------------
/** Desregistra todos los viewer del modelo y llama a ModelDelete para cada uno de ellos */
void Model::Clean() {
   // Duplica la lista de viewers y uniqueviewers para poder
   // desregistrar y luego llamar a ModelDelete
   ViewerListType list = viewers_;
   ViewerSetType unique = uniqueviewers_;

   // Desregistra los viewers del modelo
   while (!viewers_.empty())
      UnregisterViewer(viewers_.back());

   // Para cada viewer de viewers_ llama a ModelDelete
   while (!list.empty()) {
      View* pviewertemp = list.back();
      list.pop_back();
      if (unique.find(pviewertemp) != unique.end()) {
         pviewertemp->OnModelDelete(this);
      } else {
         pviewertemp = NULL;
      }
   }
}

// ------------------------------------------------------------------------------
/**
 * Registra un viewer al modelo y le asigna este ULTIMO
 * @param[in] pViewer viewer que se quiere registrar
 * @return false si es un viewer nulo o si el model se esta destruyendo, o
 *          si el viewer ya esta registrado; o si difiere el tamanio entre
 *          el conjunto de viewer de uniqueviewers_ y el de viewers_
 * @return true cuando coinciden los tamanios de uniqueviewers_ y el de viewers_
 * \attention: Parece que hay viewers que miran mas de un modelo.
 *           La asignacion no andaria bien en este caso.
 */
bool Model::RegisterViewer(View *pViewer) {
   // Si no hay viewer o el model se esta destruyendo no registra
   if (!pViewer || destroying_) {
      return false;
   }

   // Si ya hay viewer registrado no registra
   if (uniqueviewers_.find(pViewer) != uniqueviewers_.end()) {
      return false;
   }

   // Al viewer le asigna el modelo
   pViewer->SetModel(this);

   // Agrega el viewer en la lista de viewers y viewers unicos
   uniqueviewers_.insert(pViewer);
   viewers_.push_back(pViewer);

   // si coniciden los conjuntos esta OK
   return uniqueviewers_.size() == viewers_.size();
}

// ------------------------------------------------------------------------------
/**
 * desregistra un visualizado. Recibe un viewer, lo elimina de la lista de
 * unique viewers. Libera el model del viewer y elimina el viewer de la lista
 * de viewers.
 * @param[in] pViewer viewer que se quiere desregistrar
 * @return true si se encontro el viewer en la lista de viewers
 * @return false si no se encontro el viewer en la lista de viewers
 */
bool Model::UnregisterViewer(View *pViewer) {
   // Elimina el viewer de unique viewers
   // Lo hace primero para evitar problemas de concurrencia
   // Si por otro lado se llama al viewer.update
   uniqueviewers_.erase(pViewer);

   // Libera el modelo del viewer y elimina el viewer de la lista de viewers
   ViewerListType::iterator it = viewers_.begin();
   for (; it != viewers_.end() && (*it) != pViewer; ++it);
   if (it != viewers_.end()) {
      // Libera el modelo
      pViewer->SetModel(NULL);
      // Elimina el viewer
      viewers_.erase(it);
      return true;
   } else {
      return false;
   }
}

// ------------------------------------------------------------------------------
/** Pone el estado como cambiado */
void Model::SetChanged() {
   hasChanged_ = true;
}

// ------------------------------------------------------------------------------
/**
 * Informa si el Modelo fue modificado
 * @return true / false si el model fue o no modificado
 */
bool Model::HasChanged() const {
   return hasChanged_;
}

// ------------------------------------------------------------------------------
/** manda el mensaje de actualizacion a todos los viewers del modelo */
void Model::SendViewerUpdate() {
   // Si blockUpdate es mayor que 0 no manda el update.
   // Debe ser decrementado a mano llamando a UnblockViewerUpdate().
   if (blockViewerUpdate_ > 0) {
      return;
   }

   // Si preventUpdate es true o se esta destruyendo no manda el update
   // pero setea el preventUpdate_ en false para la proxima entada
   if (preventNextViewerUpdate_ || destroying_) {
      PreventNextViewerUpdate(false);
      return;
   }

   // Define viewvec como vector de punteros a viewers
   std::vector<View*> viewvec;
   ViewerListType::iterator it;

   // Arma viewvec con los mismos datos que viewers_
   for (it = viewers_.begin(); it != viewers_.end(); ++it) {
      viewvec.push_back((*it));
   }

   // Para cada view en viewvec
   for (size_t i = 0; i < viewvec.size(); i++) {
      // si el viewer esta en uniqueviewers
      if (uniqueviewers_.find(viewvec[i]) != uniqueviewers_.end()) {
         // si el viewer no es UpdatedElement_
         // Lo asigna y manda el update al viewer
         if (viewvec[i] != pUpdatedElement_) {
            pUpdatedElement_ = viewvec[i];
            viewvec[i]->Update(this);
         }
         // Libera UpdatedElement_
         pUpdatedElement_ = NULL;
      }
      // else ESTE CODIGO SE COMENTA PORQUE SI ENTRA POR EL ELSE uniqueviewers_ no contiene a
      // viewvec[i]
      //	uniqueviewers_.erase(viewvec[i]);
   }
   hasChanged_ = false;
}

// ------------------------------------------------------------------------------

/** Previene que se envie actualizacion en el proximo SendViewerUpdate()
 *
 * @param[in] Status Flag que indica si se debe bloquear el proximo ::Update()
 * \deprecated USAR BlockViewerUpdate() Y UnblockVIewerUpdate() desde ahora
 */
void Model::PreventNextViewerUpdate(bool Status) {
   preventNextViewerUpdate_ = Status;
}
;

// ------------------------------------------------------------------------------

/** Previene que se envie actualizacion hasta llamada a UnblockViewerUpdate() */
// Tener cuidado que es acumulatorio. No bloquea Update si blockUpdate_==0
bool Model::IsViewerUpdateBlocked() {
   return (blockViewerUpdate_ > 0);
}

/** Retorna el valor del contador blockViewerUpdate_ */
int Model::CountViewerUpdateBlock() {
   return (blockViewerUpdate_);
}
// Para debug por si hay anidamiento de Blocks

/** Incrementa el contador blockViewerUpdate_ */
void Model::BlockViewerUpdate() {
   blockViewerUpdate_++;
}

/** Decrementa el contador blockViewerUpdate_ */
void Model::UnblockViewerUpdate() {
   if (blockViewerUpdate_ > 0) {
      blockViewerUpdate_--;
   }
#ifdef REPORT_DEBUG
   else {
      REPORT_DEBUG("D:ViewerUpdate no bloqueado. No se puede desbloquear.");
   }
#endif
}

// ------------------------------------------------------------------------------

/**
 * Verifica que el model recibido este en la lista de models validos.
 * @param[in] pModel model que se quiere verificar
 * @return true si encuentra el model recibido en la lista de models validos,
 *          false en otro caso.
 */
bool Model::IsValid(Model *pModel) {
   // Si no lo encuentra, lo pone en null y retorna falzo
   if (models_.find(pModel) == models_.end()) {
      pModel = NULL;
#ifdef REPORT_DEBUG
      REPORT_DEBUG("D:El modelo (%p) no es valido", pModel);
#endif
   }
   return pModel != NULL;
}
