/*! \file ReprojectionRenderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef REPROJECTIONRENDERER_H_
#define REPROJECTIONRENDERER_H_

// Includes Estandar
#include <vector>
#include <list>
#include <string>

// Includes Suri
#include "Renderer.h"
#include "suri/Image.h"
#include "suri/Subset.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class GcpList;
class CoordinatesTransformation;

/*!
 * Renderer que lee un raster, y a partir de la reproyeccion definida realiza una
 * transformacion de coordenadas acorde a lo definido en el nodo de reproyeccion para
 * este render. Es el responsable de hacer creer a RasterRenderer que ocurre bajo
 * el mismo Sistema de Referencia
 */
class ReprojectionRenderer : public suri::Renderer {
   /** ctor copia**/
   ReprojectionRenderer(const ReprojectionRenderer&);

public:
   ReprojectionRenderer();
   virtual ~ReprojectionRenderer();
   /** Crea un renderizador a partir de un pElement */
   /**
    *  Metodo virtual abstracto puro utilizado para crear con el metodo
    * estatico Create() instancias de clases derivadas registradas con el
    * metodo RegisterViewer().
    * @param[in] pElement Elemento a renderizar.
    * @param[in] pPreviousRenderer Renderizador anterior.
    */
   virtual Renderer *Create(Element *pElement,
                            Renderer *pPreviousRenderer = NULL) const;
   /** Nombre del renderizador == al nombre del nodo */
   /**
    *  Retorna el nombre del nodo que maneja el renderizador.
    * @return Nombre del nodo manejado por la clase derivada.
    */
   virtual std::string CreatedNode() const;
   // ----------------------- METODOS DE RENDERIZACION -------------------------
   /** Renderiza el elemento dado un World en un Canvas */
   virtual bool Render(const World *pWorldWindow, Canvas* pCanvas, Mask* pMask);
   /** Obtiene el "bounding box" del elemento renderizado */
   /**
    * \pre el subset debe ser seteado antes de llamar a esta funcion con un
    *      valor invalido.
    * \post Se refleja el subset de mundo que renderiza.
    * \post No se modifican los parametros en caso de falla o subset nulo
    * @param[in] pWorld Mundo sobre el cual se debe devolver el BB
    * @param[out] Ulx ULx de mundo renderizado.
    * @param[out] Uly ULy de mundo renderizado.
    * @param[out] Lrx LRx de mundo renderizado.
    * @param[out] Lry LRy de mundo renderizado.
    */
   virtual void GetBoundingBox(const World *pWorld, double &Ulx, double &Uly,
                               double &Lrx, double &Lry);
   /** Obtiene los parametros de entrada del renderer */
   /**
    *  Permite obtener los parametros de entrada que requiere el renderizador,
    * es decir, los datos que espera de pPreviousRenderer.
    *
    *  En caso de que no tenga algun requerimiento, el parametro tomara los
    * siguientes valores.
    * @code
    *  SizeX = -1
    *  SizeY = -1
    *  BandCount = -1
    *  DataType = DataInfo<void>::Name
    * @endcode
    *
    * @param[out] SizeX Tamanio en X requerido por el renderizador.
    * @param[out] SizeY Tamanio en Y requerido por el renderizador.
    * @param[out] BandCount Bandas requeridas.
    * @param[out] DataType Tipo de dato.
    */
   virtual void GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
                                   std::string &DataType) const;
   /** Obtiene los parametros de salida del renderer */
   /**
    *  Permite obtener los parametros de salida del renderizador.
    *
    * @param[out] SizeX Tamanio en X requerido por el renderizador.
    * @param[out] SizeY Tamanio en Y requerido por el renderizador.
    * @param[out] BandCount Bandas requeridas.
    * @param[out] DataType Tipo de dato.
    */
   virtual void GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                    std::string &DataType) const;
   // ----------------------- METODOS DE ACTUALIZACION -------------------------
   /** Actualizacion de parametros para representar un elemento */
   /**
    *  Metodo abstracto de actualizacion del renderizador. Examina el elemento
    * y actualiza sus parametros de manera de reflejar los cambios que hayan
    * sido aplicados.
    * @param[in] pElement Elemento a analizar por cambios.
    */
   virtual void Update(Element *pElement);

private:
   /** Parametros de RasterRenderer(Tipo de lectura, url, bandas, etc.) */
   class Parameters {
   public:
      Parameters(): generateMask_(false), noDataValue_(0), pGcpList_(NULL) {}
      /** enum con metodos de lectura que se usa para leer imagen */
      typedef enum {
         Decimate, Aggregate
      } ReadFunctionType;
      std::string imageUrl_; /*! info de URL en nodo */
      std::vector<int> bandCombination_; /*! que bandas se van a renderizar */
      ReadFunctionType readFunction_; /*! forma de leer pixeles al decimar */
      std::string rasterModel_; /*! info contexto de un SPATIAL_REFERENCE_NODE */
      std::string spatialReference_; /*! info contexto de un RASTER_SPATIAL_MODEL_NODE */
      bool generateMask_;  /*! Generar mascara a partir de datos raster */
      long long noDataValue_; /*! Valor a tomar como dato no valido */
      std::string srOut_; /*! Sistema de Referencia de llegada */
      std::string transformationType_; /*! tipo de transformacion a realizar*/
      int transformationOrder_; /*! Orden de la transformacion (en caso de ser necesario) */
      std::string transformationAlgorithm_; /*! Algoritmo que a utilizar en la trasnformacion*/
      GcpList* pGcpList_; /*! lista de puntos de control */
      Subset imageExtent_; /*! Extent de la imagen **/
      int delta_; /** cada cuantas coordenadas hay que sacar una para calcular los GCPs */
   };
   /** Metodo auxiliar que carga los parametros necesarios para la renderizacion
    *  que posee el elemento que se pasa por parametro */
   void LoadParameters(Element* pElement, Parameters* pParameters) const;
   /** Metodo auxiliar que realiza la copia de datos de una instancia Parameters a otra */
   void CopyParameters(Parameters* pSourceParameters, Parameters* pDestinationParameters) const;
   /** Metodo que determina si la clase Parameters pasada por parametro tiene datos validos**/
   bool IsValidParameters(const Parameters& ParametersToCheck) const;
   /** Metodo auxiliar encargado de parametros de georreferencia **/
   void LoadGeorreferenceParameters(wxXmlNode* pGeorreferenceNode,
                                    Parameters* pParameters) const;
   /** Metodo auxiliars encargado de la carga de parametros bajo el nodo renderizacion**/
   void LoadRenderizationParameters(wxXmlNode* pRenderNode,
                                    Parameters* pParameters) const;
   /** Metodo auxiliar que se encarga de cargar la lista de puntos de control
    *  a partir de la definicion en el nodo XML que se pasa por parametro
    */
   void LoadGcpListFromXml(wxXmlNode* pGcpNode, Parameters* &pParameters) const;
   /**
    * Tranforma el Url del elemento a string que usa Image para abrir la imagen.
    * Este string depende del driver de Gdal que se use.
    * @return string que se le pasa a Image.
    */
   std::string GenerateImageId(const std::string &ImageId);
   /** Metodo auxiliar que dada la url de la imagen en los parametros de renderizacion,
    *  carga la imagen en caso de que no se encuentre cargada */
   void UpdateImage();
   /** Metodo auxiliar que dado el mundo de salida y la transformacion
    *  calcula el mundo de entrada.*/
   void InitializeInputWorld(const World* pWorldIn,
                                  World* &pTransformWorld,
                                  suri::CoordinatesTransformation* pct) const;
   /** */
   void WalkThroughViewport(const World* pWorldWindow,
                            World* pTransformWorld, Canvas* pPreviousCanvas,
                            Mask* pPreviousMask, Canvas* pCanvas, Mask* pMask,
                            std::vector<void*>& PreviousData,
                            std::vector<void*>& ViewportData,
                            std::vector<void*>& PreviousMaskData,
                            std::vector<void*>& MaskData,
                            suri::CoordinatesTransformation* pct);
   /** */
   void InitializeDataFromCanvas(int VpWidth, int VpHeight, Canvas* pPreviousCanvas,
                                 std::vector<int>& Canvasindex,
                                 std::vector<void*>& PreviousData,
                                 std::vector<void*>& ViewportData);
   /** */
   void InitializeDataFromMask(int VpWidth, int VpHeight,
                               Canvas* pPreviousCanvas, Mask* pPreviousMask,
                               std::vector<void*>& PreviousMaskData,
                               std::vector<void*>& MaskData);

   /**
    * Devuelve el objeto para realizar la transformacion dado los parametros
    * especificados.
    */
   suri::CoordinatesTransformation* GetTransformation(const World* pWorldIn) const;

   Image *pImage_; /*! Imagen asociada */
   Image *pMask_; /*! Mascara de la imagen */
   bool changed_; /*! flag para determinar en Update si cambio algun parametro */
   Parameters parameters_; /*! parametros de renderizacion (visualizacion) */
};

} /** namespace suri */

#endif /* REPROJECTIONRENDERER_H_ */
