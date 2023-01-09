/*! \file MinimumDistanceParametersPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MINIMUMDISTANCEPARAMETERSPART_H_
#define MINIMUMDISTANCEPARAMETERSPART_H_

#include "ClusterClassificationPart.h"

namespace suri {
/** Permite setear los parametros del algoritmo de clasificacion distancia minima. */
/**
 * Permite setear los parametros del algoritmo de clasificacion distancia minima.
 * Consta de un area para escribir el umbral dentro del cual se considerara
 * que un punto pertenece a la clase.
 */
class MinimumDistanceParametersPart : public ClusterClassificationPart {
public:
   /** Ctor */
   MinimumDistanceParametersPart();
   /** Dtor */
   virtual ~MinimumDistanceParametersPart();

// --- Metodos de Part ---
   /** Crea la ventana de la parte */
   virtual bool CreateToolWindow();
   /** Inicializa los datos mostrados */
   virtual void SetInitialValues();


   // -- Metodos de ParametersSelectionPart ---
   /** Retorna el nombre del algoritmo que se usara para clasificar */
   virtual std::string GetAlgorithmName();

   // -- Metodos internos ---
   /** Cambia el color del control */
   virtual void SetColourCtrlValue(VectorStyle::Color Color);
   /** Retorna el color del control */
   virtual VectorStyle::Color GetColourCtrlValue();
   /** Cambia el valor del threshold */
   virtual void SetThresholdCtrlValue(double Threshold);
   /** Retorna el valor del threshold */
   virtual double GetThresholdCtrlValue();

   static const double DEFAULT_THRESHOLD;  /*! threshold inicial */
   static const VectorStyle::Color DEFAULT_COLOR;  /*! color inicial */
};
}

#endif /* MINIMUMDISTANCEPARAMETERSPART_H_ */
