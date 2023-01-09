/*! \file VectorRenderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTORRENDERER_H_
#define VECTORRENDERER_H_

// Includes standard
#include <map>
#include <vector>

// Includes suri
#include "suri/Renderer.h"
#include "suri/Vector.h"
#include "suri/VectorStyle.h"

// Includes wx
#include "wx/string.h"

class wxXmlNode;
class wxPoint;
class wxDC;
class wxBitmap;
class OGRFeature;

/** namespace suri */
namespace suri {

// forwards
class CoordinatesTransformation;

/** Renderer de los vectores (utiliza las clases del OGR/GDAL) */
/**
 *  Esta clase renderiza los vectores usando el estilo correspondiente. Los
 *  vectores soportados son: punto(y anotacion), linea y poligono.
 *  Utiliza OGR/GDAL para filtrar los vectores fuera de la ventana.
 *  /todo Soporte de biblioteca de estilos
 *  /todo Soporte de capas con distintos sistemas de georreferencia
 *  \note hereda de renderer para formar parte del pipeline de renderizacion
 */
class VectorRenderer : public Renderer {
   /** Ctor. de Copia. */
   VectorRenderer(const VectorRenderer &VectorRenderer);

public:
   /** ctor */
   VectorRenderer();
   /** dtor */
   virtual ~VectorRenderer();
// ------------------------- ESTRUCTURA DE DATOS ----------------------------
   /** parametros de VectorRenderer(URL, georrefernecia, estilos de capas) */
   class Parameters {
   public:
      /*! Url del vector que se renderiza */
      std::string vectorUrl_;
      /*! Georreferencia de las capas */
      std::map<int, std::string> layersSR_;
      /*! Indice y estilos de las capas */
      std::map<int, std::string> layerstyle_;
      std::string attributeFilter_;
      int activelayer_;
   };
// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
   /** genera un nodo XML a partir de los parametros */
   static wxXmlNode *GetXmlNode(const VectorRenderer::Parameters &RenderParameters);
   /** Obtiene el nodo de interes a partir de un elemento */
   static wxXmlNode *GetXmlNode(const Element *pElement);
   /** Reemplaza el nodo renderizado por el nuevo */
   static bool SetXmlNode(Element *pElement, wxXmlNode *pNode);
   /** genera los parametros a partir de un nodo */
   static Parameters GetParameters(const wxXmlNode *pNode);
// -------------------------- METODO DE CREACION ----------------------------
   /** creador + inicializador */
   virtual VectorRenderer *Create(Element *pElement, Renderer *pLastRenderer) const;
   /** Nombre del renderizador == al nombre del nodo */
   virtual std::string CreatedNode() const;
// ----------------------- METODOS DE RENDERIZACION -------------------------
   /** Renderiza el elemento dado un World en un Canvas */
   virtual bool Render(const World *pWorldWindow, Canvas* pCanvas, Mask* pMask);
   /** Obtiene el "bounding box" del elemento renderizado */
   virtual void GetBoundingBox(const World *pWorld, double &Ulx, double &Uly,
                               double &Lrx, double &Lry);
   /** Obtiene los parametros de entrada del renderer */
   virtual void GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
                                   std::string &DataType) const;
   /** Obtiene los parametros de salida del renderer */
   virtual void GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                    std::string &DataType) const;
   /** Renderiza una capa vectorial */
   virtual bool RenderLayer(int LayerIndex, const World *pWorldWindow, Canvas* pCanvas,
                            Mask* pMask);

   /** Renderiza una vista previa de un estilo */
   static void PreviewStyle(VectorStyle * pVStyle, wxBitmap &Bitmap, size_t xsize,
                            size_t ysize);

// ----------------------- METODOS DE ACTUALIZACION -------------------------
   /** Actualizacion de parametros para representar un elemento */
   virtual void Update(Element *pElement);

protected:
   virtual bool DoRenderLayer(OGRLayer* pLayer, VectorStyle * pVstyle, int LayerIndex,
                              const World *pWorldWindow, Canvas* pCanvas, Mask* pMask);
   /** Metodo que obtiene la anotacion o etiqueta correspondiente a un feature **/
   std::string GetAnnotationFromFeature(OGRFeature* pfeature, int LabelIndex,
                                        const std::string& Expression);
   /**
    * Metodo auxiliar que obtiene la etiqueta configurada de un estilo
    */
   void GetExpressionFromStyle(OGRLayer* pLayer, VectorStyle* pStyle,
                               std::string& Expression, int& ExpIndex);
   /** Metodo encargado de aplicar un filtro espacial a la capa **/
   virtual bool ApplySpatialFilter(OGRLayer* pLayer,
                                   CoordinatesTransformation *pCoordsTransform,
                                   const World* pWorldWindow);
   /** Metodo que encargado de la renderizacion segun el tipo de geometrias de
    *  la capa**/
   virtual bool RenderGeometries(int LayerIndex, std::vector<OGRGeometry *>& Geometries,
                                 std::vector<std::string>& Annotations,
                                 VectorStyle* pVectorStyle, const World *pWorldWindow,
                                 wxDC * pDC, Mask* &pMask);
   /** Retorna vector a renderizar */
   virtual Vector* OpenVector();
   /** Renderiza una capa de poligonos */
   static bool RenderPolygons(
         const std::vector<OGRGeometry *> &GeometriesVector, VectorStyle * pVStyle,
         const World *pWorldWindow, wxDC * pDC, Mask* &pMask,
         std::vector<std::string> LabelsVector = std::vector<std::string>());
   /** Renderiza una capa de lineas */
   static bool RenderLines(const std::vector<OGRGeometry *> &GeometriesVector,
                           VectorStyle * pVStyle, const World *pWorldWindow, wxDC * pDC,
                           Mask* &pMask, std::vector<std::string> LabelsVector =
                                 std::vector<std::string>());
   /** Renderiza una capa de puntos con su respectivo label para cada uno */
   static bool RenderPoints(
         const std::vector<OGRGeometry *> &GeometriesVector, VectorStyle * pVStyle,
         const World *pWorldWindow, wxDC * pDC, Mask* &pMask,
         std::vector<std::string> LabelsVector = std::vector<std::string>());
   /** Pinta */
   static bool Paint(std::vector<std::pair<wxPoint *, int> > Polygons,
                     VectorStyle * pVStyle, wxDC * pDC, Mask * &pMask,
                     int NumberOfPolygons, int* Count);
   /** Traza */
   static bool Trace(std::vector<std::pair<wxPoint *, int> > Lines,
   VectorStyle * pVStyle,
                     wxDC * pDC, Mask * &pMask);
   /** Pone marcas */
   static bool Mark(std::vector<wxPoint *> Points, VectorStyle * pVStyle, wxDC * pDC,
                    Mask * &pMask);
   /** Pone etiquetas */
   static bool Annotate(std::vector<wxPoint *> Points,
                        std::vector<std::string> LabelsVector, VectorStyle * pVStyle,
                        wxDC * pDC, Mask * &pMask);

   Parameters parameters_; /*! parametros de renderizacion (visualizacion) */

   Vector *pVector_; /*! Vector */
   /** Carga las capas al mapa pasado por parametro **/
   static void LoadLayersSrs(const wxXmlNode *plnode,
                             std::map<int, std::string> *layers);
   /** Carga las capas al mapa pasado por parametro **/
   static void LoadLayersStyle(const wxXmlNode *plnode,
                               std::map<int, std::string> *layers);
   /** Carga los parametros referentes al nodo georefencia */
   static void LoadGeorreferenceNodeIntoParameters(const wxXmlNode *pnode,
                                                   Parameters *pParams);
   /** Carga los parametros referentes al nodo renderizacion */
   static void LoadRenderizationNodeIntoParameters(const wxXmlNode *pnode,
                                                   Parameters *pParams);
};

/** Configura el pDC con formato correspondiente a pPen */
bool ConfigureDC(const VectorStyle::Pen* pPen, wxDC* pDC);
/** Configura el pDC con formato correspondiente a pBrush */
bool ConfigureDC(VectorStyle::Brush* pBrush, wxDC* pDC);
/** Configura el pDC con formato correspondiente a pSymbol */
bool ConfigureDC(VectorStyle::Symbol* pSymbol, wxDC* pDC);
/** Crea los puntos de un simbolo a partir del tipo */
bool GetSymbolPoints(VectorStyle::Symbol* pSymbol,
                   wxPoint* &pWxPoints, int &PointCount);
}

#endif /*VECTORRENDERER_H_*/
