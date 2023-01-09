/*! \file ZoomRenderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ZOOMRENDERER_H_
#define ZOOMRENDERER_H_

// Includes suri
#include "suri/Renderer.h"

/** Forwards */
class wxXmlNode;

/** namespace suri */
namespace suri {
/** Renderer que realiza interpolacion NN (Nearest Neighbour) */
/**
 * Usa interpolacion NN (Nearest Neighbour) para generar pixeles intermedios
 * cuando el nroPixeles en el viewport es mayor que en canvas
 * \note hereda de renderer para formar parte del pipeline de renderizacion
 */
class ZoomRenderer : public Renderer {
   /** Ctor. de Copia. */
   ZoomRenderer(const ZoomRenderer &ZoomRenderer);

public:
   /** ctor */
   ZoomRenderer();
   /** dtor */
   virtual ~ZoomRenderer();
// ------------------------- ESTRUCTURA DE DATOS ----------------------------
   /** parametros de zoom */
   class Parameters {
   public:
      /** enum con tipos de interpolacion */
      typedef enum {
         NearestNeighbour, CubicCombolution, None
      } InterpolationType;
      /** interfaz de las funciones de interpolacion */
      typedef void (*InterpolationFunctionType)(void*, void*, size_t, double, double);
      InterpolationType interpolation_; /*! Tipo de interpolacion que se usa */
      /* cuando hay mas pixeles de salida que */
      /* de entrada */
      InterpolationFunctionType function_; /*! Funcion que aplica zoom. */
      std::string spatialModel_; /*! Referencia espacial del raster */
      std::string rasterModel_; /*! Relacion entre pixel-linea y la ref */
      /* espacial del raster */
   };
// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
   /** genera un nodo XML a partir de los parametros */
   static wxXmlNode *GetXmlNode(const Parameters &RenderParameters);
   /** genera los parametros a partir de un nodo */
   static Parameters GetParameters(const wxXmlNode *pNode);
// -------------------------- METODO DE CREACION ----------------------------
   /** creador + inicializador */
   virtual ZoomRenderer *Create(Element *pElement, Renderer *pPreviousRenderer) const;
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
// ----------------------- METODOS DE ACTUALIZACION -------------------------
   /** Actualizacion de parametros para representar un elemento */
   virtual void Update(Element *pElement);
protected:
private:
   Parameters parameters_; /*! Parametros de la instancia */
};
}

#endif /*ZOOMRENDERER_H_*/
