/*! \file ConvolutionFilterRenderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CONVOLUTIONFILTERRENDERER_H_
#define CONVOLUTIONFILTERRENDERER_H_

// Includes standard
#include <list>
#include <vector>
#include <string>
// Includes suri
#include "BaseRasterRenderer.h"

// Defines
/** Tipo de dato salida para el resultado de aplicar un filtro */
#define FILTER_OUTPUT_DATA_TYPE float

/** namespace suri */
namespace suri {
/** Clase para crear y aplicar un filtro de convolucion */
/**
 * Aplica un filtro lineal a una imagen. Obtiene la matriz del filtro del nodo
 * del elemento y la aplica usando convolucion.
 * Obtiene los parametros de entrada y salida para la renderizacion, crea el
 * nodo renderizacion y lo aplica al elemento.
 * Implementa metodos de conversion a nodo XML para los parametros del filtro
 */
class ConvolutionFilterRenderer : public BaseRasterRenderer {
   /** Ctor. de Copia. */
   ConvolutionFilterRenderer(const ConvolutionFilterRenderer&);

public:
   /** Ctor */
   ConvolutionFilterRenderer();
   /** Dtor */
   virtual ~ConvolutionFilterRenderer();
// ------------------------- ESTRUCTURA DE DATOS ----------------------------
   /** parametros de ConvolutionFilterRenderer */
   class Parameters {
   public:
      /** Ctor. */
      Parameters() :
            filterWidth_(0), filterHeight_(0), pFunction_(NULL),
            filterName_("") {
      }
      /** Tipo de dato de la funcion */
      typedef void (*FilterFunctionType)(
            void* pDest, void* pSrc,
            std::vector<std::vector<double> > pFilterpFilter, int ImageHeight,
            int ImageWidth, int FilterHeight, int FilterWidth);
      std::list<std::vector<std::vector<double> > > filters_; /*! filtros que */
                                                    /* se aplican a la imagen */
      int filterWidth_; /*! ancho de todos los filtros */
      int filterHeight_; /*! alto de todos los filtros */
      FilterFunctionType pFunction_; /*! funcion que aplica el filtro */
      std::string filterName_; /*! Nombre del filtro a aplicar */
   };

// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
   /** genera un nodo XML a partir de los parametros */
   static wxXmlNode *GetXmlNode(
         const ConvolutionFilterRenderer::Parameters &RenderParameters);
   /** genera los parametros a partir de un nodo */
   static Parameters GetParameters(const wxXmlNode *pNode);
// -------------------------- METODO DE CREACION ----------------------------
   /** Crea un renderizador a partir de un pElement */
   virtual Renderer *Create(Element *pElement, Renderer *pPreviousRenderer) const;
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
   /** Cambia el contenido de parametres y verifica su contenido */
   virtual bool SetParameters(const Parameters &params);
   Parameters parameters_; /*! Guarda informacion necesaria para aplicar Render */
};
}

#endif /* CONVOLUTIONFILTERRENDERER_H_ */
