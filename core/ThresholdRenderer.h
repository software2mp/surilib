/*! \file ThresholdRenderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef THRESHOLDRENDERER_H_
#define THRESHOLDRENDERER_H_

// Includes Estandar
#include <string>
#include <vector>
// Includes Suri
#include "suri/Renderer.h"
#include "SRDHistogram.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/**
 * Clase que escribe una mascara que contiene determinado porcentaje de los
 * valores mas significativos
 */
class ThresholdRenderer : public Renderer {
public:
   /**
    * Destructor
    */
   virtual ~ThresholdRenderer();
// ------------------------- ESTRUCTURA DE DATOS ----------------------------
   class Parameters {
   public:
      /** Valor que se utiliza para marcar que se supera el umbral **/
      int thresholdHigh;
      /** Valor que se utiliza para marcar que se no supera el umbral **/
      int thresholdLow;
      /** Porcentaje de pixeles con los que me quedo **/
      double thresholdPercent;
      /** Constructor **/
      Parameters();
   };
// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
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

private:
   raster::data::Histogram<float>* CreateHistograms(std::vector<void*> data,
                                                   int canvasSize);
   /** Recalcula los limites del histograma, utilizando un histograma temporal **/
   void AdjustLimits(std::vector<void*> data, int canvasSize,
                     int* pNumBins, double* minValues, double* maxValues);
   /** Aplica el umbral a los datos**/
   void ApplyThreshold(std::vector<void*>& data, double* threshold, int canvasSize);
   /** Cantidad de veces que se ajustan los limites del histograma **/
   int adjustmentLoops_;
   /** Valor que se utiliza para marcar que se supera el umbral **/
   int thresholdHigh_;
   /** Valor que se utiliza para marcar que se no supera el umbral **/
   int thresholdLow_;
   /** Porcentaje de pixeles con los que me quedo **/
   double thresholdPercent_;
   /** Cantidad de divisiones en el histograma **/
   static const int kHistogramBins;
};

} /** namespace suri */

#endif /* THRESHOLDRENDERER_H_ */