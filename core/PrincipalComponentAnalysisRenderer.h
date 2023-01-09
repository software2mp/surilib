/*! \file PrincipalComponentAnalysisRenderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef PRINCIPALCOMPONENTANALYSISRENDERER_H_
#define PRINCIPALCOMPONENTANALYSISRENDERER_H_

#include <vector>

#include "suri/Renderer.h"

#define PCANALYSIS_OUTPUT_DATA_TYPE    double
#define PCANALYSIS_OUTPUT_BAND_COUNT   autoVec_.size()

namespace suri {
namespace raster {

class PrincipalComponentAnalysisRenderer : public Renderer {
public:
   /**
    * Ctor.
    */
   PrincipalComponentAnalysisRenderer();

   /**
    * Dtor.
    */
   virtual ~PrincipalComponentAnalysisRenderer();

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
    * Estable los autovectores a utilizar.
    */
   void SetAutoVec(const std::vector<double*>& AutoVec);

private:
   std::vector<double*> autoVec_;

   /**
    * Transforma los valores de entrada.
    */
   void Transform(const std::vector<void*>& InData, std::vector<void*>& OutData,
                  const std::string& DataType, const std::vector<double*>& AutoVec,
                  int InDataLen);
};

}  // namespace raster
}  // namespace suri

#endif  // PRINCIPALCOMPONENTANALYSISRENDERER_H_
