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
#include <vector>
#include <map>
#include <list>

// Includes Suri
#include "DefaultViewcontextManager.h"
#include "DefaultDatasourceManager.h"
#include "DefaultViewcontext.h"
#include "suri/LayerInterface.h"
#include "HtmlTreeGroupChangedNotification.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/** Constructor **/
DefaultViewcontextManager::DefaultViewcontextManager(
      DatasourceManagerInterface *pDatasourceManager) :
      pSelectedViewcontext_(NULL), pContext_(NULL) , pDatasourceManager_(pDatasourceManager) {
}

/** Destructor **/
DefaultViewcontextManager::~DefaultViewcontextManager() {
   Clear();
}

/** Vacia la lista de viewcontext y hace detach de los viewcontext que tenia */
void DefaultViewcontextManager::Clear() {
   if (pDatasourceManager_) {
      ViewcontextIdAssociationMap::const_iterator it = viewcontextMap_.begin();
      while (it != viewcontextMap_.end()) {
         pDatasourceManager_->Detach(it->second.first);
         it++;
      }
   }
   viewcontextContextMap_.clear();
}
/**
 * Devuelve el contexto seleccionado
 * @return NULL en caso de que no se haya seleccionado ningun viewcontext
 * @return referencia al viewcontext seleccionado
 */
ViewcontextInterface* DefaultViewcontextManager::GetSelectedViewcontext() {
   return pSelectedViewcontext_;
}

/**
 * Devuelve el contexto seleccionado
 * @return Id del viewcontext seleccionado
 */
SuriObject::UuidType DefaultViewcontextManager::GetSelectedViewcontextId() {
   return selectedViewcontextId_;
}

/**
 * Obtiene un contexto dado un id
 * @param[in] ViewcontextId Id del contexto
 */
ViewcontextInterface* DefaultViewcontextManager::GetViewcontext(
      const SuriObject::UuidType &ViewcontextId) const {
   ViewcontextInterface *pviewcontext = NULL;
   ViewcontextIdAssociationMap::const_iterator it = viewcontextMap_.find(ViewcontextId);
   if (it != viewcontextMap_.end()) {
      pviewcontext = it->second.second;
   }
   return pviewcontext;
}

/**
 * Obtiene todos los ids en un vector
 * @return vector<SuriObject::UuidType>
 */
std::vector<SuriObject::UuidType> DefaultViewcontextManager::GetViewcontextIds() const {
   std::vector<suri::SuriObject::UuidType> vcontextids;
   ViewcontextIdAssociationMap::const_iterator vit = viewcontextMap_.begin();
   for (; vit != viewcontextMap_.end(); ++vit) {
      vcontextids.push_back(vit->first);
   }
   return vcontextids;
}

/**
 * Selecciona un viewcontext a partir de un id
 *  @param[in] ViewcontextId Id del contexto
 */
void DefaultViewcontextManager::SelectViewcontext(const SuriObject::UuidType &ViewcontextId) {
   ViewcontextIdAssociationMap::const_iterator it = viewcontextMap_.find(ViewcontextId);
   pSelectedViewcontext_ = NULL;
   if (it != viewcontextMap_.end()) {
         pSelectedViewcontext_ = it->second.second;
         selectedViewcontextId_ = ViewcontextId;
   }
}

/**
 * Agrega un viewcontext
 *  @param[in] ViewcontextId Id del contexto
 *  @param[in] pViewcontext puntero al contexto
 *  @return true
 */
bool DefaultViewcontextManager::AddViewcontext(const SuriObject::UuidType &ViewcontextId,
                                               ViewcontextInterface* pViewcontext) {
   int observerid = pDatasourceManager_->Attach(pViewcontext);
   if (observerid != DefaultDatasourceManager::InvalidCount) {
      IdObserverPairType idobserverpair(observerid, pViewcontext);
      IdAssociationType idassociation(ViewcontextId, idobserverpair);
      viewcontextMap_.insert(idassociation);
      lastViewcontextIdAdded_ = ViewcontextId;
   } else {
      return false;
   }
   return true;
}

/**
 * Elimina un viewcontext
 *  @param[in] ViewcontextId Id del contexto
 *  @return Devuelve verdadero si tuvo exito la operacion, sino falso
 */
bool DefaultViewcontextManager::RemoveViewcontext(const SuriObject::UuidType &ViewcontextId) {
   ViewcontextIdAssociationMap::const_iterator it = viewcontextMap_.find(ViewcontextId);
   if (it != viewcontextMap_.end()) {
      pDatasourceManager_->Detach(it->second.first);
   }
   return viewcontextMap_.erase(ViewcontextId);
}

/**
 * Activa los items seleccionados del viewcontext especificado, por defecto en el
 * vc seleccionado
 * @param[in] pViewcontext puntero al viewcontextn en donde se quiere activar.
 * @param[in] Activate bool que indica si se desea activar el elemento e
 * la layerlist
 * \deprecated
 */
void DefaultViewcontextManager::ActivateSelectedLayersInViewcontext(
                      ViewcontextInterface* pViewcontext, bool Activate) {
   // Itero los elementos seleccionados en el viewcontext
   std::list<SuriObject::UuidType>selectedlayers = pViewcontext->GetSelectedLayers();
   std::list<SuriObject::UuidType>::const_iterator it = selectedlayers.begin();
   pViewcontext->GetLayerList()->BlockViewerUpdate();
   for (; it != selectedlayers.end(); ++it) {
      LayerInterface* player = pViewcontext->GetLayer(*it);
      pViewcontext->GetLayerList()->ActivateElement(player->GetElement(), Activate);
   }
   pViewcontext->GetLayerList()->UnblockViewerUpdate();
}

/**
 * Obtiene una lista de ids de Layer a partir de un DatasourceId
 */
std::vector<SuriObject::UuidType> DefaultViewcontextManager::GetAssociatedLayer(
      const SuriObject::UuidType& DatasourceId) {
   std::vector<SuriObject::UuidType> layerids;
   ViewcontextIdAssociationMap::const_iterator it = viewcontextMap_.begin();
   for (; it!= viewcontextMap_.end(); ++it) {
      ViewcontextInterface *pviewcontext = it->second.second;
      LayerInterface* player = pviewcontext->GetAssociatedLayer(DatasourceId);
      if (player) {
         layerids.push_back(player->GetId());
      }
   }
   return layerids;
}

/**
 * Activa un layer en su correspondiente Viewcontext a partir de su id
 * @param[in] pLayer
 * @param[in] Activate
 */
bool DefaultViewcontextManager::ActivateLayer(LayerInterface* pLayer, bool Activate) {
   // Validacion
   if (pLayer == NULL)
      return false;

   // Obtengo los Layers
   SuriObject::UuidType datasourceid = pLayer->GetAssociatedDatasourceId();
   std::vector<SuriObject::UuidType> layers = GetAssociatedLayer(datasourceid);

   // Itero por los Viewcontext
   ViewcontextIdAssociationMap::const_iterator it;

   int count = 0;

   for (it = viewcontextMap_.begin(); it != viewcontextMap_.end(); ++it) {
      ViewcontextInterface *pviewcontext = it->second.second;

      // TODO(Sebastian - TCK #852): Eliminar este codigo "magia negra" omite al Viewer3D
      count++;
      if (count == 4)
         continue;

      // Itero por los layers
      std::vector<SuriObject::UuidType>::iterator layerid;
      for (layerid = layers.begin(); layerid != layers.end(); ++layerid) {
         LayerInterface* player = pviewcontext->GetLayer(*layerid);

         // En el caso de encontrar la layer en el viewcontext, la activo/desactivo
         if (player != NULL) {
            pviewcontext->ActivateLayer(*layerid, Activate);
         }
      }
   }

   return true;
}

/**
 * Activa los items seleccionados del viewcontext especificado en todos
 * los viewcontexts
 * @param[in] Activate bool que indica si activar o desactivar las capas
 */
void DefaultViewcontextManager::ActivateSelectedLayersInAllViewcontexts(bool Activate) {
   std::vector<SuriObject::UuidType> viewcontextids = GetViewcontextIds();
   std::vector<SuriObject::UuidType>::iterator it = viewcontextids.begin();
   for (; it != viewcontextids.end(); it++)
      ActivateSelectedLayersInViewcontext(GetViewcontext(*it), Activate);
}

/**
 * Obtiene el contexto actual
 */
Context *DefaultViewcontextManager::GetContext() {
   return pContext_;
}

/**
 * Indica el contexto actual
 */
Context *DefaultViewcontextManager::SetContext(Context *Context) {
   return pContext_ = Context;
}

/**
 * Devuelve el layer asociado al id de elemento dado
 */
LayerInterface* DefaultViewcontextManager::GetAssociatedLayerByElementId(
      const SuriObject::UuidType& ElementId) {
   LayerInterface *player = NULL;
   ViewcontextIdAssociationMap::const_iterator it = viewcontextMap_.begin();
   for (; it!= viewcontextMap_.end(); ++it) {
      ViewcontextInterface *pviewcontext = it->second.second;
      player = pviewcontext->GetLayerByElementId(ElementId);
      if (player) {
         break;
      }
   }
   return player;
}

/**
 * Obtiene el Id del ultimo viewcontext agregado
 */
SuriObject::UuidType DefaultViewcontextManager::GetLastViewcontextIdAdded() {
   return lastViewcontextIdAdded_;
}

/**
 * Obtiene la instancia fuente de datos asociada a una capa
 * @param LayerId Id de la capa a obtener la fuente de datos
 * @return DatasourceInterface* puntero a la fuente de datos asociadas
 * @return NULL en caso de no existir la fuente de datos asociada
 */
DatasourceInterface* DefaultViewcontextManager::GetAssociatedDatasource(
            const SuriObject::UuidType &LayerId)const {
   ViewcontextIdAssociationMap::const_iterator it = viewcontextMap_.begin();
   DatasourceInterface* pdatasource = NULL;
   for (; it!= viewcontextMap_.end() && pdatasource == NULL; ++it) {
      ViewcontextInterface *pviewcontext = it->second.second;
      LayerInterface* player = pviewcontext->GetLayer(LayerId);
      if (player)
         pdatasource =
               pDatasourceManager_->GetDatasource(player->GetAssociatedDatasourceId());
   }
   return pdatasource;
}

ViewcontextInterface* DefaultViewcontextManager::GetViewcontextByType(
		suri::ViewcontextInterface::ViewcontextType Type) {
   ViewcontextIdAssociationMap::const_iterator it = viewcontextMap_.begin();
   ViewcontextInterface* pviewcontext = NULL;
   for (; !pviewcontext && it != viewcontextMap_.end(); ++it) {
	  suri::ViewcontextInterface::ViewcontextType ct = (it->second.second)->GetType();
	  if (ct == Type) {
         pviewcontext = it->second.second;
      }
   }
   return pviewcontext;
}

/** Obtiene la capa que corresponde al id pasado por parametro **/
LayerInterface* DefaultViewcontextManager::GetLayer(const SuriObject::UuidType& LayerId) {
   ViewcontextIdAssociationMap::const_iterator it = viewcontextMap_.begin();
   LayerInterface* player = NULL;
   for (; it!= viewcontextMap_.end() && player == NULL; ++it) {
      ViewcontextInterface *pviewcontext = it->second.second;
      player = pviewcontext->GetLayer(LayerId);
   }
   return player;
}
} /** namespace suri */
