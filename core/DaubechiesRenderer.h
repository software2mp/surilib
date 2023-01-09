/*! \file DaubechiesRenderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DAUBECHIESRENDERER_H_
#define DAUBECHIESRENDERER_H_

// Includes Estandar
#include <string>
// Includes Suri
#include "suri/Renderer.h"
// Includes Wx
// Defines
// forwards

namespace suri {

class DaubechiesSplitStrategy;
class DaubechiesTransformStrategy;

class DaubechiesRenderer : public Renderer {
   /** Ctor. de Copia. */
   DaubechiesRenderer(const DaubechiesRenderer&);

public:
   /** Ctor */
   DaubechiesRenderer();
   /** Dtor */
   virtual ~DaubechiesRenderer();
// ------------------------- ESTRUCTURA DE DATOS ----------------------------
   /** parametros de DaubechiesRenderer */
   class Parameters {
   public:
      /** Ctor. */
      Parameters() :
            filterName_("") {
      }

      std::string filterName_; /*! Nombre del filtro a aplicar */
   };

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
   virtual void SetParameters(const Parameters &params);
   Parameters parameters_; /*! Guarda informacion necesaria para aplicar Render */
   DaubechiesSplitStrategy* pSplitStrategy_;
   DaubechiesTransformStrategy* pTransformStrategy_;
   // Cantidad de bandas nuevas por banda transformada.
   static const int RendererOutputBands;
};

} /** namespace suri */

#endif /* DAUBECHIESRENDERER_H_ */
