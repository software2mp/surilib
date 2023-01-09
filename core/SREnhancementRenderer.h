/** \file SREnhancementRenderer.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SRENHANCEMENTRENDERER_H_
#define SRENHANCEMENTRENDERER_H_

#include "suri/Renderer.h"
#include "NoDataValue.h"

namespace suri {
namespace render {

class EnhancementRenderer : public suri::Renderer {
public:
   /**
    * Ctor.
    */
   EnhancementRenderer();

   /**
    * Ctor 2.
    */
   EnhancementRenderer(int bandcount, int* pnumbins, double* plowvalue,
                       double* phighvalue, int** pbins, bool Active, std::string& Name);

   /**
    * Dtor.
    */
   ~EnhancementRenderer();

   /**
    * Crea un renderizador a partir de un pElement.
    */
   virtual Renderer *Create(Element *pElement,
                            Renderer *pPreviousRenderer = NULL) const;

   /**
    * Nombre del renderizador == al nombre del nodo.
    */
   virtual std::string CreatedNode() const;

   /**
    * Renderiza el elemento dado un World en un Canvas.
    */
   virtual bool Render(const World *pWorldWindow, Canvas* pCanvas, Mask* pMask);

   /**
    * Obtiene el "bounding box" del elemento renderizado.
    */
   virtual void GetBoundingBox(const World *pWorld, double &Ulx, double &Uly,
                               double &Lrx, double &Lry);

   /**
    * Obtiene los parametros de entrada del renderer.
    */
   virtual void GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
                                   std::string &DataType) const;

   /**
    * Obtiene los parametros de salida del renderer.
    */
   virtual void GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                    std::string &DataType) const;

   /**
    * Actualizacion de parametros para representar un elemento.
    */
   virtual void Update(Element *pElement);

private:
   int bandCount_;  // Cantidad de bandas.
   int* pNumBins_;  // Numero de bins por banda.
   double* pLowValue_;  // Valor mas chico por banda.
   double* pHighValue_;  // Valor mas grande por banda.
   int** pBins_;  // LUT
   bool active_;  // Si esta activo o no.
   std::string name_;  // Nombre del realce.

   /**
    * Traduce los valores de la imagen al valor de intesidad correspondiente.
    */
   void Translate(const std::string& DataType, int* pLut, void* pInData, void* pOutData,
                  int DataSize, int NumBins, double Min, double Max, double NoDataValue,
                  bool NoDataValueAvailable);
};

}  // namespace render
}  // namespace suri

#endif  // SRENHANCEMENTRENDERER_H_
