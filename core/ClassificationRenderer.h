/*! \file ClassificationRenderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CLASSIFICATIONRENDERER_H_
#define CLASSIFICATIONRENDERER_H_

// Includes estandar
#include <vector>

// Includes Suri
#include "suri/Renderer.h"
#include "suri/XmlFunctions.h"
#include "suri/StatisticsFunctions.h"
#include "ClassificationAlgorithmInterface.h"
// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Renderizador que clasifica */
/**
 * Obtiene los parametros de entrada y salida para la renderizacion, crea el
 * nodo renderizacion para la clasificacion.
 * Implementa los metodos de conversion a nodo XML para los parametros de la
 * clasificacion.
 */
class ClassificationRenderer : public Renderer {
   /** ctor de copia */
   ClassificationRenderer(const ClassificationRenderer&);
public:
   /** ctor */
   ClassificationRenderer();
   /** dtor */
   virtual ~ClassificationRenderer();

// ------------------------- ESTRUCTURA DE DATOS ----------------------------
   /** Clase parametros del Renderizador */
   class Parameters {
   public:
      /** Ctor. */
      Parameters() :
            pAlgorithm_(NULL) {
      }
      ClassificationAlgorithmInterface *pAlgorithm_; /*! Algoritmo utilizado para */
      /* clasificar */
   protected:
   private:
   };

// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
   /** genera un nodo XML a partir de los parametros */
   static wxXmlNode *GetXmlNode(const Parameters &RenderParameters);
   /** genera los parametros a partir de un nodo */
   static Parameters GetParameters(const wxXmlNode *pNode);
// -------------------------- METODO DE CREACION ----------------------------
   /** creador + inicializador */
   virtual ClassificationRenderer *Create(Element *pElement,
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
   Parameters parameters_; /*! Parametros para la clasificacion */
private:
};
}

#endif /* CLASSIFICATIONRENDERER_H_ */
