/*! \file LutRenderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LUTRENDERER_H_
#define LUTRENDERER_H_

// Includes standard
#include <list>
#include <map>

// Includes suri
#include "suri/Renderer.h"
#include "RgbColor.h"
#include "suri/LutArray.h"

// defines

/** namespace suri */
namespace suri {
class Element;

/** Clase que genera una imagen aplicando una Lut a las bandas de entrada */
/**
 * Genera una imagen aplicandole una tabla a las bandas de la entrada.
 * La imagen resultante puede tener menos bandas que la original pero
 * no pueden haber bandas sin tabla asignada.
 */
class LutRenderer : public Renderer {
   /** Ctor. de Copia. */
   LutRenderer(const LutRenderer &LutRenderer);

public:
   /** Ctor */
   LutRenderer();
   /** Dtor */
   virtual ~LutRenderer();
// ------------------------- ESTRUCTURA DE DATOS ----------------------------
   /** parametros de LutRenderer(bandas, tipo de dato de salida, tipo de dato de entrada) */
   class Parameters {
   public:
      /** Ctor. */
      Parameters() {
      }
      /** tipo de dato para la funcion de la lut */
      typedef void (*LutFunctionType)(void*, void*, size_t, std::map<double, double>);
      LutArray lut_; /*! Tabla con lut que uso para renderizar */
      LutFunctionType pFunction_; /*! tipo de funcion */
   };

// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
   /** genera un nodo XML a partir de los parametros */
   static wxXmlNode *GetXmlNode(const LutRenderer::Parameters &RenderParameters);
   /** genera los parametros a partir de un nodo */
   static LutRenderer::Parameters GetParameters(const wxXmlNode *pNode);
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

// ----------------------- METODOS DE MODIFICACION DE LUT -------------------------

   /** Setea un nuevo Lut en un elemento raster, ya hecho el checkeo de si existe otro */
protected:

   /** Carga params desde elemento */
   static bool LoadFunction(Parameters &params, Element* pElement,
                            Renderer *pPreviousRenderer);

   /** Cambia el contenido de parametres y verifica su contenido */
   static bool ValidateTable(const Parameters &Params, Renderer *pPreviousRenderer);

   LutRenderer::Parameters parameters_; /*! Guarda informacion necesaria */
   /* para aplicar Render */
};

bool SetNewLutNode(Element* pElement, wxXmlNode *pRenderizationNode,
                   wxXmlNode* pLutNode);
}

#endif /*LUTRENDERER_H_*/
