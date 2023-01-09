/*! \file TasseledCapRenderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TASSELEDCAPRENDERER_H_
#define TASSELEDCAPRENDERER_H_

#include "TasseledCapTransformation.h"
#include "suri/Renderer.h"

#define TASSELED_CAP_OUTPUT_DATA_TYPE     float
#define TASSELED_CAP_OUTPUT_BAND_COUNT    4

namespace suri {
namespace raster {

class TasseledCapRenderer : public suri::Renderer {
public:
   /**
    * Ctor.
    */
   TasseledCapRenderer();

   /**
    * Dtor.
    */
   virtual ~TasseledCapRenderer();

   /**
    * Crea un renderizador a partir de un pElement.
    */
   virtual Renderer* Create(Element* pElement,
                            Renderer* pPreviousRenderer = NULL) const;

   /**
    * Nombre del renderizador == al nombre del nodo.
    */
   virtual std::string CreatedNode() const;

   /**
    * Renderiza el elemento dado un World en un Canvas.
    */
   virtual bool Render(const World* pWorldWindow, Canvas* pCanvas, Mask* pMask);

   /**
    * Obtiene el "bounding box" del elemento renderizado.
    */
   virtual void GetBoundingBox(const World* pWorld, double& Ulx, double& Uly,
                               double& Lrx, double& Lry);

   /**
    * Obtiene los parametros de entrada del renderer.
    */
   virtual void GetInputParameters(int& SizeX, int& SizeY, int& BandCount,
                                   std::string& DataType) const;

   /**
    * Obtiene los parametros de salida del renderer.
    */
   virtual void GetOutputParameters(int& SizeX, int& SizeY, int& BandCount,
                                    std::string& DataType) const;

   /**
    * Actualizacion de parametros para representar un elemento.
    */
   virtual void Update(Element* pElement);

   /**
    * Establece la transformacion a ser utilizada.
    */
   void SetTransformation(suri::raster::transformation::TasseledCapTransformation* pTcTranf);

private:
   suri::raster::transformation::TasseledCapTransformation* pTcTranf_;
};

}  // namespace raster
}  // namespace suri

#endif  // TASSELEDCAPRENDERER_H_
