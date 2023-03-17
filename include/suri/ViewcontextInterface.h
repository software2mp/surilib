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

#ifndef VIEWCONTEXTINTERFACE_H_
#define VIEWCONTEXTINTERFACE_H_

// Includes Estandar
#include <list>

// Includes Suri
#include "suri/Subject.h"
#include "suri/SuriObject.h"
#include "suri/ObserverInterface.h"
#include "suri/LayerList.h"
#include "suri/LayerInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

// forwards
class ViewcontextManagerInterface;
/**
 * Clase Abstracta que encapsula el comportamiento de LayerList
 */
class ViewcontextInterface : public ObserverInterface {
public:
    typedef enum {
    	V2D = 1, V3D = 2, Multimedia = 3, Html = 4, Map = 5, Documents = 6, Exercises = 7, DV = 8
	} ViewcontextType;
   /** Constructor **/
   ViewcontextInterface(ViewcontextType Vtype = V2D) : type_(Vtype) {}
   /** Destructor **/
   virtual ~ViewcontextInterface() {}
   /** Agrega una capa e indica si se debe hacer un update. Acoplado con la vista! \deprecated */
   virtual bool AddLayer(LayerInterface* pLayer, bool Update)=0;
   /** Agrega un Layer al Viewcontext y devuelve un booleano que determina si tuvo o no exito */
   virtual bool AddLayer(LayerInterface* pLayer)=0;
   /** Elimina Layer dado un Id y devuelve un booleano que determina si tuvo o no exito */
   virtual bool RemoveLayer(const SuriObject::UuidType &LayerId)=0;
   /** Obtiene Layer dado un id, devolviendo NULL si falla */
   virtual LayerInterface* GetLayer(const SuriObject::UuidType &LayerId)=0;
   /** Obtiene la layer asociada al datasource indicado. Trazabilidad entre datasource y layer */
   virtual LayerInterface* GetAssociatedLayer(const SuriObject::UuidType &DatasourceId)=0;
   /** Obtiene la capa asociada al id del elemento \deprecated */
   virtual LayerInterface* GetLayerByElementId(const SuriObject::UuidType &ElementId)=0;
   /** Configura la layerlist asociada \deprecated **/
   virtual void SetLayerList(LayerList* pLayerList)=0;
   /** Obtiene la LayerList \deprecated */
   virtual LayerList *GetLayerList()=0;
   /** Deselecciona todas las capas */
   virtual void ResetSelection() = 0;
   /** Selecciona una capa a partir de un Id */
   virtual void SelectLayer(const SuriObject::UuidType &LayerId, bool State = true)=0;
   /** Selecciona una capa a partir del Id del elemento asociado \deprecated */
   virtual void SelectLayerByElementId(const SuriObject::UuidType &ElementId, bool State = true)=0;
   /** Obtiene las capas seleccionadas */
   virtual std::list<SuriObject::UuidType> GetSelectedLayers()=0;
   /** Elimina las capas seleccionadas */
   virtual void RemoveSelectedLayers()=0;
   /** Cambiar una layer de orden dentro la lista */
   virtual bool OrderChangeLayer(LayerInterface* pLayer, LayerInterface* pLayerDestination)=0;
   /** Mueve una capa al final de la lista */
   virtual bool MoveLayerToEnd(LayerInterface* pLayer)=0;
   /** Activa una capa a partir de un Id */
   virtual bool ActivateLayer(const SuriObject::UuidType &LayerId, bool State = true)=0;
   /** Viewcontextmgr al cual pertenece. Esquema contenedor-contenido **/
   virtual void SetViewcontextManager(
         ViewcontextManagerInterface* pViewcontextManager)=0;
   /** Obtiene la instancia fuente de datos asociada a una capa **/
   virtual DatasourceInterface* GetAssociatedDatasource(
         const SuriObject::UuidType &LayerId)const=0;
   /** Habilita los elementos 3D y deshabilita los 2D (los esconde) */
   virtual void Enable3d(bool Enable = true)=0;
   /** Habilita los elementos 2D y deshabilita los 3D (los esconde) */
   virtual void Enable2d(bool Enable = true)=0;
   /** Bloquea la capa que corresponde al id que se pasa por parametro **/
   virtual bool BlockLayerByDatasourceId(const SuriObject::UuidType& DatasourceId)=0;
   /** Desbloquea la capa que corresponde al id que se pasa por parametro **/
   virtual bool UnblockLayerByDatasourceId(const SuriObject::UuidType& DatasourceId)=0;
   /** Bloquea la capa que corresponde al id que se pasa por parametro **/
   virtual bool BlockLayer(const SuriObject::UuidType& LayerId)=0;
   /** Desbloquea la capa que corresponde al id que se pasa por parametro **/
   virtual bool UnblockLayer(const SuriObject::UuidType& LayerId)=0;
   /** Bloquea todas las capas **/
   virtual void BlockAllLayers()=0;
   /** Desbloquea todas las capas **/
   virtual void UnblockAllLayers()=0;
   /** Consulta si una capa se encuentra bloqueada **/
   virtual bool IsLayerBlocked(const SuriObject::UuidType& LayerId)=0;
   /** Consulta si el tipo de capa es compatible en el contexto **/
   virtual bool IsCompatibleLayerType(LayerInterface* player)=0;
   /** Obtiene el id del viewcontext asociado **/
   virtual suri::SuriObject::UuidType GetViewcontextId() const=0;
   /** Configura por unica vez el id del viewcontext */
   virtual bool SetViewcontextId(const SuriObject::UuidType& ViewcontextId)=0;
   /** Obtiene el tipo de viewcontext que posee**/
   virtual ViewcontextType GetType()=0;
   /** Elimina todas las capas que posee el viewcontext **/
   virtual void RemoveAllLayers()=0;
   /** Devuelve la cantidad de capas que tiene almacenado el viewcontext **/
   virtual int GetLayersCount()=0;

protected:
   ViewcontextType type_; /** Tipo de visualizador de contexto */
private:
   /** Constructor de Copia */
   ViewcontextInterface(ViewcontextInterface &);
};

} /* namespace suri */

#endif /* VIEWCONTEXTINTERFACE_H_ */
