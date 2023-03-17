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

#ifndef DEFAULTVIEWCONTEXTMANAGER_H_
#define DEFAULTVIEWCONTEXTMANAGER_H_

// Includes Estandar
#include <string>
#include <vector>
#include <utility>
#include <map>

// Includes Suri
#include "suri/ViewcontextManagerInterface.h"
#include "suri/ViewcontextInterface.h"
#include "suri/LayerInterface.h"
#include "suri/DatasourceManagerInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Clase que administra los viewcontext para cada contexto.
 */
class DefaultViewcontextManager : public suri::ViewcontextManagerInterface {
public:
   /** Contructor */
   explicit DefaultViewcontextManager(DatasourceManagerInterface* pDatasourceManager = NULL);
   /** Destructor */
   virtual ~DefaultViewcontextManager();
   /** Obtiene el viewcontext */
   virtual ViewcontextInterface* GetSelectedViewcontext();
   /** Obtiene el Id del viewcontext seleccionado */
   virtual SuriObject::UuidType GetSelectedViewcontextId();
   /** Selecciona un Viewcontext a partir de un Id */
   virtual void SelectViewcontext(const SuriObject::UuidType &ViewcontextId);
   /** Obtiene un Viewcontext a partir de Id */
   virtual ViewcontextInterface *GetViewcontext(const SuriObject::UuidType &ViewcontextId) const;
   /** Obtiene todos los ids de los viewcontext que administra */
   virtual std::vector<SuriObject::UuidType> GetViewcontextIds() const;
   /** Agrega un viewcontext */
   virtual bool AddViewcontext(const SuriObject::UuidType &ViewcontextId,
                               ViewcontextInterface* pViewcontext);
   /** Elimina un Viewcontext a partir de un Id*/
   virtual bool RemoveViewcontext(const SuriObject::UuidType &ViewcontextId);
   /** Obtiene un Layer a partir de un DatasourceId */
   virtual std::vector<SuriObject::UuidType> GetAssociatedLayer(
         const SuriObject::UuidType& DatasourceId);
   /** Obtiene un Layer a partir de un DatasourceId  \deprecated */
   virtual LayerInterface *GetAssociatedLayerByElementId(const SuriObject::UuidType& ElementId);
   /** Activa un layer en su correspondiente Viewcontext a partir de su id */
   virtual bool ActivateLayer(LayerInterface* pLayer, bool Activate);
   /** Activa los items seleccionados del viewcontext especificado, por defecto en el vc selec */
   virtual void ActivateSelectedLayersInViewcontext(ViewcontextInterface* pViewcontext,
                                                    bool Activate);
   /** Activa los items seleccionados del viewcontext especificado en todos los viewcontexts */
   virtual void ActivateSelectedLayersInAllViewcontexts(bool Activate);
   /** Obtiene el ID del ultimo viewcontext agregado */
   virtual SuriObject::UuidType GetLastViewcontextIdAdded();
   /** Obtiene el contexto activo \deprecated */
   virtual Context* GetContext();
   /** Setea el contexto activo */
   virtual Context* SetContext(Context* pContext);
   /** Vacia la lista de viewcontext */
   virtual void Clear();
   /** Cierra todos los  elementos */
   template<class IteratorType>
   size_t CloseElements(const IteratorType &ItBegin, const IteratorType &ItEnd);
   /** Obtiene la instancia fuente de datos asociada a una capa **/
   virtual DatasourceInterface* GetAssociatedDatasource(
         const SuriObject::UuidType &LayerId) const;
   virtual ViewcontextInterface* GetViewcontextByType(suri::ViewcontextInterface::ViewcontextType Type);
   /** Obtiene la capa que corresponde al id pasado por parametro **/
   virtual LayerInterface* GetLayer(const SuriObject::UuidType& LayerId);

private:
   /** Constructor copia */
   DefaultViewcontextManager(DefaultViewcontextManager &);

   typedef std::pair<int, ViewcontextInterface *> IdObserverPairType; /** Par asoc vctx como obs */
   typedef std::pair<SuriObject::UuidType, IdObserverPairType> IdAssociationType; /*! Asoc map */
   typedef std::map<SuriObject::UuidType, IdObserverPairType> ViewcontextIdAssociationMap; /*! vc */
   ViewcontextIdAssociationMap viewcontextMap_; /*! Mapa de Viewcontext */
   ViewcontextInterface* pSelectedViewcontext_; /*! Viewcontext seleccionado */
   Context* pContext_; /*! Puntero al contexto activo */
   DatasourceManagerInterface* pDatasourceManager_; /*! Puntero al DatasourceManager */
   // Para verificar que realmente el viewcontext que se pide pertenezca al context
   // que lo esta pidiendo que es justamente el que se tiene comop referencia
   typedef std::map<SuriObject::UuidType, std::string> ViewcontextContextMap; /*! Mapa Id - Ctx */
   typedef std::pair<SuriObject::UuidType, std::string> ViewcontextContextAssociation; /*! Id-Ctx */
   ViewcontextContextMap viewcontextContextMap_; /*! Mapa entre Viewcontext con Context */
   SuriObject::UuidType lastViewcontextIdAdded_; /*! Id del ultimo Viewcontext agregado */
   SuriObject::UuidType selectedViewcontextId_; /*! Id del ultimo Viewcontext agregado */
};

/** Cierra todos los  elementos */
template<class IteratorType>
size_t DefaultViewcontextManager::CloseElements(const IteratorType &ItBegin,
                                                const IteratorType &ItEnd) {
   std::vector<suri::SuriObject::UuidType> viewcontextids = GetViewcontextIds();
   std::vector<suri::SuriObject::UuidType>::iterator it = viewcontextids.begin();
   size_t retval = 0;
   for (; it != viewcontextids.end(); ++it) {
      retval = GetViewcontext(*it)->GetLayerList()->DelElement(ItBegin, ItEnd);
   }
   return retval;
}

} /** namespace suri */

#endif /* DEFAULTVIEWCONTEXTMANAGER_H_ */
