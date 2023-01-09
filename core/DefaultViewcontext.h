/*! \file DefaultViewcontext.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DEFAULTVIEWCONTEXT_H_
#define DEFAULTVIEWCONTEXT_H_

// Includes Estandar
#include <list>

// Includes Suri
#include "suri/ViewcontextInterface.h"
#include "suri/NotificationInterface.h"
#include "suri/LayerInterface.h"
#include "suri/LayerList.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Se implementa la interfaz de ViewcontextInterface por defecto provista por SuriLib
 */
class DefaultViewcontext : public suri::ViewcontextInterface {
public:
	// TODO(Federico - TCK #4214): deprecar y remover utilizacion de enable2d, enable3d
   /** Constructor */
	explicit DefaultViewcontext(bool Support3dLayers = false,
			bool Support2dLayers = true,
			suri::ViewcontextInterface::ViewcontextType type = V2D,
			const suri::SuriObject::UuidType& ViewcontextId = SuriObject::NullUuid);
   /** Destructor */
   virtual ~DefaultViewcontext();
   /** Configura la layerlist asociada \deprecated **/
   virtual void SetLayerList(LayerList* pLayerList);
   /** Obtiene la LayerList \deprecated */
   virtual LayerList *GetLayerList();
   /** Agrega Layer \deprecated  */
   virtual bool AddLayer(LayerInterface* pLayer, bool Update);
   /** Agrega Layer */
   virtual bool AddLayer(LayerInterface* pLayer);
   /** Elimina Layer dado un Id */
   virtual bool RemoveLayer(const SuriObject::UuidType &LayerId);
   /** Obtiene Layer dado un id \deprecated */
   virtual LayerInterface *GetLayer(const SuriObject::UuidType &LayerId);
   /** Obtiene la layer asociada al datasource indicado. Trazabilidad entre datasource y layer */
   virtual LayerInterface* GetAssociatedLayer(const SuriObject::UuidType &DatasourceId);
   /** Obtiene la capa asociada al id del elemento \deprecated */
   virtual LayerInterface* GetLayerByElementId(const SuriObject::UuidType &ElementId);
   /** Deselecciona todas las capas */
   virtual void ResetSelection();
   /** Selecciona una capa */
   virtual void SelectLayer(const SuriObject::UuidType &LayerId, bool Status = true);
   /** Obtiene las capas seleccionadas */
   virtual std::list<SuriObject::UuidType> GetSelectedLayers();
   /** Elimina las capas seleccionadas */
   virtual void RemoveSelectedLayers();
   /** Selecciona una capa a partir del Id del elemento asociado \deprecated */
   virtual void SelectLayerByElementId(const SuriObject::UuidType &ElementId, bool Status = true);
   /** Update de Observer */
   virtual void Update(NotificationInterface* Notification);
   /** Cambiar una layer de orden dentro la lista */
   virtual bool OrderChangeLayer(LayerInterface* pLayer, LayerInterface* pLayerDestination);
   /** Mueve una capa al final de la lista */
   virtual bool MoveLayerToEnd(LayerInterface* pLayer);
   /** Activa una capa a partir de un Id */
   virtual bool ActivateLayer(const SuriObject::UuidType &LayerId, bool Status = true);
   /** Indica si soporta capas 3D */
   bool Support3dLayers() const;
   /** Actualiza los datos que son partes de la fuente de datos \deprecated */
   bool DatasourcePropertiesChanged(DatasourceInterface* pDatasource);
   /** Viewcontextmgr al cual pertenece. Esquema contenedor-contenido **/
   virtual void SetViewcontextManager(
         ViewcontextManagerInterface* pVeiwcontextManager);
   /** Obtiene la instancia fuente de datos asociada a una capa **/
      virtual DatasourceInterface* GetAssociatedDatasource(
            const SuriObject::UuidType &LayerId)const;
   /** Habilita los elementos 3D y deshabilita los 2D (los esconde) */
   virtual void Enable3d(bool Enable = true);
   /** Habilita los elementos 2D y deshabilita los 3D (los esconde) */
   virtual void Enable2d(bool Enable = true);
   /** Bloquea la capa que corresponde al id que se pasa por parametro **/
   virtual bool BlockLayer(const SuriObject::UuidType& LayerId);
   /** Desbloquea la capa que corresponde al id que se pasa por parametro **/
   virtual bool UnblockLayer(const SuriObject::UuidType& LayerId);
   /** Bloquea la capa asociada a la fuente de datos con id que se pasa por parametro **/
   virtual bool BlockLayerByDatasourceId(const SuriObject::UuidType& DatasourceId);
   /** Desbloquea la capa asociada a la fuente de datos con id que se pasa por parametro **/
   virtual bool UnblockLayerByDatasourceId(const SuriObject::UuidType& DatasourceId);
   /** Bloquea todas las capas **/
   virtual void BlockAllLayers();
   /** Desbloquea todas las capas **/
   virtual void UnblockAllLayers();
   /** Consulta si una capa se encuentra bloqueada **/
   virtual bool IsLayerBlocked(const SuriObject::UuidType& LayerId);
   /** Verifica que no exista una capa de tipo raster desplegada y bloqueada **/
   bool CanDisplayLayer(LayerInterface* player);
   /** Consulta si el tipo de capa es compatible en el contexto **/
   virtual bool IsCompatibleLayerType(LayerInterface* player);
   /** Obtiene el id del viewcontext asociado **/
   virtual suri::SuriObject::UuidType GetViewcontextId() const;
   /** Configura por unica vez el id del viewcontext */
   virtual bool SetViewcontextId(const SuriObject::UuidType& ViewcontextId);
   /** Obtiene el tipo de viewcontext que posee**/
   ViewcontextType GetType() {
      return type_;
   }
   /** Elimina todas las capas que posee el viewcontext **/
   virtual void RemoveAllLayers();
   /** Devuelve la cantidad de capas que tiene almacenado el viewcontext **/
   virtual int GetLayersCount() {
      return layerInterfaceList_.size();
   }

private:
   /** Verifica si existe la capa en la lista del viewcontext**/
   bool Exists(LayerInterface* pLayer) const;
   /** Constructor de Copia */
   DefaultViewcontext(DefaultViewcontext &);
   /** Reemplaza el xml del elemento menos los nodos correspondientes a las capas \deprecated */
   bool ReplaceElementNodes(Element* pElement, Element* pElementDest);
   /** Elimina una capa de la lista de seleccionados */
   void UnselectLayer(const SuriObject::UuidType &LayerId);
   /** Agrega una capa de la lista de seleccionados */
   void DoSelectLayer(const SuriObject::UuidType &LayerId);
   /** Elimina una capa de la lista de seleccionados a partir del id de un element \deprecated */
   void UnselectLayerByElementId(const SuriObject::UuidType &ElementId);
   /** Agrega una capa de la lista de seleccionados a partir del id de un element \deprecated */
   void DoSelectLayerByElementId(const SuriObject::UuidType &ElementId);
   /** Obtiene el elemento asociado a una capa \deprecated */
   Element* GetAssociatedElement(const SuriObject::UuidType &LayerId);
   /** Indica si el viewcontext soporta capas 3d \deprecated */
   bool Is3dLayersSupported() const;
   /** Indica si el viewcontext soporta capas 3d \deprecated */
   bool Is2dLayersSupported() const;
   /** Obtiene el grupo al cual pertenece un elemento \deprecated */
   int GetElementGroup(LayerInterface* pLayer);
   /** Elimina de selectedLayersList_ las capas invalidas. */
   void RemoveInvalidSelectedLayers();
   /*! Referencia a la LayerList que encapsula */
   LayerList* pLayerList_;
   /*! Def tipo de dato de lista de capas */
   typedef std::list<LayerInterface *> LayerInterfaceList;
   /*! Lista de capas */
   LayerInterfaceList layerInterfaceList_;
   /*! Lista de capas seleccionadas */
   std::list<SuriObject::UuidType> selectedLayersList_;
   bool support3dLayers_; /*! Booleano que determina si soporta capas 3D */
   bool support2dLayers_;/*! Booleano que determina si soporta capas 2D */
   suri::ViewcontextInterface::ViewcontextType type_;
   /** puntero al Viewcontextmgr **/
   suri::SuriObject::UuidType viewcontextId_;  /*! Identifica el contexto */
   ViewcontextManagerInterface* pViewcontextManager_; /*! Visualizador de contexto */
};

} /** namespace suri */

#endif /* DEFAULTVIEWCONTEXT_H_ */
