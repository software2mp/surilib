/*! \file Viewer3D.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VIEWER3D_H_
#define VIEWER3D_H_

#include <vector>
// Includes suri
#include "ViewerWidget.h"
#include "View.h"
#include "suri/Element.h"
#include "suri/RasterElement.h"
#include "suri/Canvas.h"
#include "suri/VectorStyle.h"

/** Forward wx */
class wxCommandEvent;
class wxCloseEvent;
class wxPanel;
class wxColourPickerEvent;
class wxToggleBitmapButton;
class wxColourPickerCtrl;


/** Otros forwards */
class Model;

/** namespace suri */
namespace suri {
/** Declara la clase Viewer3DEvent */
DECLARE_EVENT_OBJECT(Viewer3DEvent);

/** Forwards */
class LayerList;
class RenderizationManager;
class MemoryCanvas;
class World;
class TerrainGLCanvas;
class Terrain;
class RasterSpatialModel;
class WorldExtentManager;
class CoordinatesTransformation;

template<class CallbackClass>
class WorldView;

template<class CallbackClass>
class ListViewer;

/** ViewerWidget para visualizacion de elementos de terreno */
/**
 * Crea los raster de terreno y textura e inicia su
 * actualizacion a partir de la lista.
 * Cuando se modifica la lista se llama a ElementUpdate que realiza
 * la cracion y/o modificaciones correspondientes de Terrain y de
 * TerrainGlCanvas
 */
class Viewer3D : public ViewerWidget {
   /** Ctor. de Copia. */
   Viewer3D(const Viewer3D &Viewer3D);

public:
   /** Declara friend la clase de evento Viewer3DEvent */
   FRIEND_EVENT_OBJECT(Viewer3DEvent);

   /**
    * @param[in] pList Lista de elementos a visualizar.
    */
   Viewer3D(wxWindow *pParent = NULL, DataViewManager* pDataViewManager = NULL);
   /** dtor */
   virtual ~Viewer3D();

   /** Metodo que recibe al destruirse el modelo */
   void ListDelete(Model *pModel);
   /** Creacion de la ventana */
   virtual bool CreateToolWindow();
   /** Se le setea la lista de elementos  */
   virtual void DoSetElementList(LayerList *pList);
   /** Resetea el View */
   virtual void ResetViewer();
   /** Retorna el canvas usado */
   TerrainGLCanvas *GetCanvas();
   /** Obtiene la transformacion asociada **/
   virtual CoordinatesTransformation* GetTransformation();
   /** ------- Modificacion del mundo del viewer -------------------------------**/
   /** Hace ZoomIn */
   virtual void ZoomIn(int X, int Y, int Factor = 2, int Z = 0);
   /** Hace ZoomOut */
   virtual void ZoomOut(int X, int Y, int Factor = 2, int Z = 0);
   /** Cambia la ventana del mundo */
   virtual void ChangeWindow(const Subset &NewWindow, bool PreventUpdate = false);
   /** Setea el WorldExtentManager */
   virtual void SetWorldExtentManager(WorldExtentManager* pExtentManager);
   /** Retorna el WorldExtentManager */
   virtual WorldExtentManager* GetWorldExtentManager();
   /** Metodo de actualizacion de la lista, el mundo y/o los elementos. */
   virtual void UpdateViewers(Model *pModel);
   /** Verifica el estado de la renderizacion. */
   virtual void CheckRenderizationStatus();
   /** Setea el elemento raster del terreno */
   void SetTerrain(RasterElement* pRasterElement);
   void EraseCrosshair();
   void DrawCrosshair(Coordinates Point);
protected:
   /** Permite obtener la ventana con las herramientas del viewer */
   virtual wxWindow *DoGetViewerTools(wxWindow *pParent);
   /** Despliega un elemento (puede ser funcion de RenderizationControl) */
   virtual void DisplayElement(Element *pElement);
   /** Oculta un elemento (puede ser funcion de RenderizationControl) */
   virtual void HideElement(Element *pElement);
   /** El elemento ha sido modificado */
   virtual void UpdateElement(Element *pElement);
// --------------------------------- EVENTOS --------------------------------
   /** evento OnSize */
   void OnSize();
// --------------------------------- TOOLBAR --------------------------------
   /** evento de seleccion de color de fondo */
   void OnColourChangeButtonClick(wxColourPickerEvent& Event);
// ------------------------ ACTUALIZACION DE MODELOS ------------------------
   /** Actualizacion de la lista */
   void ListUpdate(Model *pModel);
   /** Actualizacion del mundo */
   void ElementUpdate(Model *pModel);
   /** Renderiza los vectores activos */
   void DrawActiveVectorElements();
   /** Crea el terreno */
   void CreateTerrain(Element *pElement);
   /** Crea los objetos que necesita el RenderizationManger y los asocia */
   void InitRenderizationManager(RasterElement* pElement,
                                 RenderizationManager* &pRenderizationManager,
                                 World* &pWorld, LayerList* &pList, Canvas* &pCanvas,
                                 WorldExtentManager* pExtentManager_);
   /** Crea los objetos que necesita el RenderizationManger y los asocia */
   void InitRenderizationManager(RenderizationManager* &pRenderizationManager,
                                 World* &pWorld, LayerList* &pList, Canvas* &pCanvas,
                                 WorldExtentManager* pExtentManager_);

   /** Crea un punto en la posicion del VectorElement que se le pasa */
   bool CreatePointVector(Element *pElement);
   /** Retorna el tamano de pixel en unidades de la referencia espacial */
   size_t PixelSize(Element *pElement);
   /**
    * Actualiza las texturas en funcion de la activacion/desactivacion de
    * elementos en la lista de capas
    **/
   bool UpdateTexture();
private:
   Subset oldSubset_;
   Coordinates oldCenter_;
   EVENT_OBJECT_PROPERTY(Viewer3DEvent); /*! Objeto para conectar eventos */
   RenderizationManager *pTerrainRenderizationManager_; /*! Renderizador terreno */
   RenderizationManager *pTextureRenderizationManager_; /*! Renderizador texture */
   RenderizationManager *pVectorRenderizationManager_; /*! Renderizador vectores */
   World *pTerrainWorld_; /*! Mundo para el terreno */
   World *pTextureWorld_; /*! Mundo para la textura */
   Canvas *pTerrainMemoryCanvas_; /*! Canvas de memoria para el terreno */
   Canvas *pTextureMemoryCanvas_; /*! Canvas de memoria para la textura */
   LayerList *pTerrainList_; /*! Lista con el terreno */
   LayerList *pTextureList_; /*! Lista con la textura */
   RasterElement *pTerrainElement_; /*! Elemento raster del terreno */
   ListView<Viewer3D> *pListViewer_; /*! View de la lista */
   CallbackView<Viewer3D> *pElementViewer_; /*! View de elementos */
   WorldView<Viewer3D> *pWorldViewer_;
   /* Para actualizar vista si hay visualizadores sincronizados*/
   TerrainGLCanvas *pGlCanvas_; /*! Maneja eventos 3D y comunicacion con OpenGL */
   Terrain *pTerrain_; /*! Terreno */
   wxColourPickerCtrl* pColourPickerButton_; /*! Permite seleccionar un color */
   Element *pActiveElement_; /*! Elemento activo, para saber cuando cambia de elemento. */
   /*! Manejador que no modifica mundo al renderizar terreno*/
   WorldExtentManager* pExtentManagerTerrain_;
   /*! Manejador que no modifica mundo al renderizar textura */
   WorldExtentManager* pExtentManagerTexture_;
   /** Transformacion utilizada por el visualizador **/
   CoordinatesTransformation* pTransform_;
   RasterElement *pElement_; /*! Elemento raster del terreno */
   Subset newWindow_;
   /**
    * Obtiene el estilo vectorial configurado en la capa
    * teniendo en cuenta la configuracion vieja que posee
    * el nodo <estilo>
    */
   VectorStyle* GetVectorStyle(Element* pElement);
   /** Configura el mundo del terreno 3D a desplegar **/
   bool ConfigureTerrainWorld(Element* pElement, Subset& Intersection, Subset& TerrainSubset);
   /** Configura el mundo para la renderizacion de texturas sobre un terreno 3D (DEM) **/
   bool ConfigureTextureWorld(Element* pElement,
                              std::vector<RasterSpatialModel*>& Rmodels,
                              std::vector<RasterElement*>& Rasters,
                              Subset& TextureSubset, Canvas* pTextureCanvas,
                              World* pTextureWorld);
   /** Borra el nodo multiplexor de las capas raster de una banda */
   void RemoveMultiplexorNode(Element* pElement);
   /** Agrega a las capas raster el nodo multiplexor borrado */
   void AddMultiplexorNode(Element* pElement);
   /** Actualizacion del mundo para el visualizador dinamico */
   void ElementUpdateForDynamicViewer(Model *pModel);
   /** Actualizacion del mundo para el visualizador 3D */
   void ElementUpdateForViewer3D(Model *pModel);
};
}

#endif /*VIEWER3D_H_*/
