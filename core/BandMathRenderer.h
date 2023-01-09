/*! \file BandMathRenderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef BANDMATHRENDERER_H_
#define BANDMATHRENDERER_H_

// Includes standard
#include <vector>
#include <map>
// Includes suri
#include "suri/Renderer.h"

// Defines
#define BAND_MATH_OUTPUT_DATA_TYPE float
/** Ecuacion para la norma */
#define NORM_OPERATION "sqr(b1^2+b2^2)"
/** Ecuacion para el indice */
#define INDEX_OPERATION "(b1-b2)/(b1+b2)"
/** Ecuacion para el indice */
#define RVI_INDEX_OPERATION "(8*SigmaHV)/(SigmaHH+SigmaVV+2*SigmaHV)"

/** namespace suri */
namespace suri {
/**
 * Tipo de dato para la funcion
 * @param[in] pDest datos de salida
 * @param[in] pBand1 datos de entrada, banda 1 de la ecuacion
 * @param[in] pBand2 datos de entrada, banda 2 de la ecuacion
 * @param[in] Size cantidad de pixeles
 */
typedef void (*BandMathFunctionType)(std::vector<void*> pBands,
                                   size_t Size,
                                   std::string Equation,
                                   std::map<std::string, int> EquationVariables,
                                   BAND_MATH_OUTPUT_DATA_TYPE* pDest);

/** Clase para realizar operaciones (algebra) sobre los pixeles de la imagen */
/**
 * La salida es el resultado de aplicar una funcion en cada pixel. La entrada
 * de la funcion es el valor del pixel en distintas bandas.
 * Obtiene los parametros de entrada y salida para el renderizador, crea el nodo
 * renderizacion y renderiza el elemento.
 * Implementa funciones de conversion a nodo XML de los parametros del algebra de
 * bandas.
 */
class BandMathRenderer : public Renderer {
   /** Ctor. de Copia. */
   BandMathRenderer(const BandMathRenderer &BandMathRenderer);

public:
   /** Ctor */
   BandMathRenderer();
   /** Dtor */
   virtual ~BandMathRenderer();

// ------------------------- ESTRUCTURA DE DATOS ----------------------------
   /** parametros de BandMathRenderer */
   class Parameters {
   public:
      /** Funcion que se aplica en el algebra de bandas. */
      class Equation {
      public:
         /** Ctor. */
         Equation() :
               equation_("") {
         }
         /** Ctor. */
         Equation(std::string Equation, std::map<std::string, int> BandNames) :
               bandNames_(BandNames), equation_(Equation) {
         }

         std::map<std::string, int> bandNames_; /*! vector con pares */
                                          /* (nombrebanda-numerobanda) */
                                          /* para cada variable de la */
                                          /* formula */
         std::string equation_; /*! string de funcion que se debe aplicar */
      };
      std::vector<Equation> equations_; /*! vector con ecuaciones */
   };

// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
   /** genera un nodo XML a partir de los parametros */
   static wxXmlNode *GetXmlNode(const BandMathRenderer::Parameters &RenderParameters);
   /** genera los parametros a partir de un nodo */
   static Parameters GetParameters(const wxXmlNode *pNode);
// -------------------------- METODO DE CREACION ----------------------------
   /** Crea un renderizador a partir de un pElement */
   virtual BandMathRenderer::Renderer *Create(Element *pElement,
                                              Renderer *pPreviousRenderer) const;
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
   virtual bool LoadEquations(BandMathRenderer::Parameters Parameters,
                              Renderer *pPreviousRenderer = NULL);

   BandMathRenderer::Parameters parameters_; /*! Guarda informacion necesaria */
                                             /* para aplicar Render */
};
}

#endif /* BANDMATHRENDERER_H_ */
