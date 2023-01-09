/** \file SpatialSubsetSelectionPart.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SPATIALSUBSETSELECTIONPART_H_
#define SPATIALSUBSETSELECTIONPART_H_

// Includes standard
#include <string>
#include <vector>

// Includes suri
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/Subset.h"
#include "suri/ObserverInterface.h"
#include "suri/SuriObject.h"
#include "UniversalSubsetInputPart.h"
#include "suri/UseFeatureSelection.h"

namespace suri {

// fordwards
class World;
class DataViewManager;
class LayerTreeModel;
class TreeSelectionManagerInterface;
/** Declara clase ParametricClassificationPartEvent */
DECLARE_EVENT_OBJECT(SpatialSubsetSelectionPartEvent);

namespace ui {
   class HtmlListSelectionWidget;
}


/**
 * Part que permite seleccionar un subset.
 * El subset se puede ingresar directamente o copiar de una capa/visualizador
 * existente.
 */
class SpatialSubsetSelectionPart : public UseFeatureSelection, public Part,
                                   public ProcessAtributeProvider,
                                   public ObserverInterface {
   /** Declara friend la clase de evento ParametricClassificationPartEvent. */
   FRIEND_EVENT_OBJECT(SpatialSubsetSelectionPartEvent);

public:
   /** Ctor */
   SpatialSubsetSelectionPart(DataViewManager *pDataViewManager,
                                                World* pInputWorld);
   /** Dtor */
   virtual ~SpatialSubsetSelectionPart();

   /** Crea la ventana de la parte */
   virtual bool CreateToolWindow();
   /** Devuelve el estado del Part. */
   virtual bool IsEnabled();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** retorna la coordenada que ingresaron en srs de trabajo, informa si es valida */
   virtual bool GetSubset(Subset& OutputSubset);

   /** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
   virtual void Update(NotificationInterface* pNotification);
   /** Configura el subset en el part a partir del extent de fuente de datos */
   void UpdateSubsetWithDatasource(SuriObject::UuidType DatasouceId);
   /** Configura el subset en el part a partir del extent a traves de una capa */
   void UpdateSubsetWithLayer(SuriObject::UuidType LayerId);

protected:
   // ----------------------------- Eventos -----------------------------
   /** Configura el subset en el part */
   void SetNewSubset(const Subset& NewSubset, const std::string &SubsetSrs);
   /** Evento que se ejecuta al hacer click sobre boton actualizar subset */
   void OnUpdateSubsetButton(wxCommandEvent &Event);
   /** Evento que se ejecuta al presionar el boton -> **/
   void OnRefreshSubsetButtonClick(wxCommandEvent &Event);
   /** Metodo auxiliar que carga la capa de capas **/
   void LoadLayerList();
#ifdef __WXMSW__
   /** evento OnSize */
   void OnSize(wxSizeEvent& Event);
#endif

   /** Parte para seleccion de subset **/
   UniversalSubsetInputPart* pSubsetInputPart_;
   /** Puntero al mundo con subset de la imagen de entrada*/
   World *pInputWorld_;
   /** Administrador con informacion de proyecto */
   DataViewManager *pDataViewManager_;
   /** Clase que administra los nodos seleccion */
   ui::HtmlListSelectionWidget* pTreeSelection_;
   /** Mapa que indica la posicion de cada visualizador en pantalla */
   std::vector<SuriObject::UuidType> viewportListIndex_;

   EVENT_OBJECT_PROPERTY(SpatialSubsetSelectionPartEvent); /*! Objeto que conecta los eventos */
};

} /* namespace suri */
#endif /* SPATIALSUBSETSELECTIONPART_H_ */
