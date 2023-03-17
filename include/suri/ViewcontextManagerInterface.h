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

#ifndef VIEWCONTEXTMANAGERINTERFACE_H_
#define VIEWCONTEXTMANAGERINTERFACE_H_

// Includes Estandar
#include <vector>
#include <map>

// Includes Suri
#include "SuriObject.h"
#include "ViewcontextInterface.h"
#include "LayerInterface.h"

// Includes App

// Includes Wx

// Defines

// forwards
class Context;

namespace suri {

/**
 * Esta clase modela la interfaz del administrador de las diferentes configuraciones visuales}
 * de la lista unica de fuentes de datos
 */
class ViewcontextManagerInterface {
public:
   /** Constructor */
   ViewcontextManagerInterface() {}
   /** Destructor */
   virtual ~ViewcontextManagerInterface() {}
   /** Obtiene el viewcontext seleccionado */
   virtual ViewcontextInterface* GetSelectedViewcontext()=0;
   /** Obtiene el Id del viewcontext seleccionado */
   virtual SuriObject::UuidType GetSelectedViewcontextId()=0;
   /** Selecciona un viewcontext dado un id */
   virtual void SelectViewcontext(const SuriObject::UuidType &ViewcontextId)=0;
   /** Obtiene un Viewcontext a partir de un id */
   virtual ViewcontextInterface *GetViewcontext(const SuriObject::UuidType &ViewcontextId) const=0;
   /** Obtiene todos los ids de los viewcontext en un vector */
   virtual std::vector<SuriObject::UuidType> GetViewcontextIds() const=0;
   /** Agrega un viewcontext */
   virtual bool AddViewcontext(const SuriObject::UuidType &ViewcontextId,
                               ViewcontextInterface* pViewcontext)=0;
   /** Elimina un viewcontext */
   virtual bool RemoveViewcontext(const SuriObject::UuidType &ViewcontextId)=0;
   /** Obtiene una lista de ids de Layer a partir de un DatasourceId */
   virtual std::vector<SuriObject::UuidType> GetAssociatedLayer(
         const SuriObject::UuidType& DatasourceId)=0;
   /** Obtiene un Layer a partir de un ElementId \deprecated */
   virtual LayerInterface* GetAssociatedLayerByElementId(const SuriObject::UuidType& ElementId)=0;
   /** Obtiene el contexto activo \deprecated */
   virtual Context* GetContext()=0;
   /** Setea el contexto activo \deprecated */
   virtual Context* SetContext(Context *Context)=0;
   /** Activa un layer en su correspondiente Viewcontext a partir de su id */
   virtual bool ActivateLayer(LayerInterface* Layer, bool Activate)=0;
   /** Activa los items seleccionados del viewcontext especificado en el viewcontext especificado */
   virtual void ActivateSelectedLayersInViewcontext(ViewcontextInterface* pViewcontext,
                                                    bool Activate)=0;
   /** Activa los items seleccionados del viewcontext especificado en todos los viewcontexts */
   virtual void ActivateSelectedLayersInAllViewcontexts(bool Activate)=0;
   /** Obtiene el ID del ultimo viewcontext agregado */
   virtual SuriObject::UuidType GetLastViewcontextIdAdded()=0;
   /** Vacia la lista de viewcontext */
   virtual void Clear()=0;
   /** Obtiene la instancia fuente de datos asociada a una capa **/
   virtual DatasourceInterface* GetAssociatedDatasource(
         const SuriObject::UuidType &LayerId)const=0;
   /** Obtiene el Viewcontext a partir del tipo de contexto **/
   virtual ViewcontextInterface* GetViewcontextByType(suri::ViewcontextInterface::ViewcontextType Type)=0;
   /** Obtiene la capa que corresponde al id pasado por parametro **/
   virtual LayerInterface* GetLayer(const SuriObject::UuidType& LayerId)=0;

private:
   /** Constructor de copia */
   ViewcontextManagerInterface(const ViewcontextManagerInterface&);
};

} /* namespace suri */

#endif /* VIEWCONTEXTMANAGERINTERFACE_H_ */
