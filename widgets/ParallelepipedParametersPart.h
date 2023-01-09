/*! \file ParallelepipedParametersPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef PARALLELEPIPEDPARAMETERSPART_H_
#define PARALLELEPIPEDPARAMETERSPART_H_

#include "ClusterClassificationPart.h"

namespace suri {
/** Permite setear los parametros del algoritmo de clasificacion paralelepipedo. */
/**
 * Permite setear los parametros del algoritmo de clasificacion paralelepipedo.
 * Consta de un area para escribir el umbral dentro del cual se considerara
 * que un punto pertenece a la clase.
 * Se considera que el umbral es el mismo en todas las direcciones.
 */
class ParallelepipedParametersPart : public ClusterClassificationPart {
public:
   ParallelepipedParametersPart();
   virtual ~ParallelepipedParametersPart();

// --- Metodos de Part ---
   /** Crea la ventana de la parte */
   virtual bool CreateToolWindow();
   /** Inicializa los datos mostrados */
   virtual void SetInitialValues();

   /** Retorna el nombre del algoritmo que se usara para clasificar */
   virtual std::string GetAlgorithmName();

   // -- Metodos internos ---
   /** Cambia el color del control */
   virtual void SetColourCtrlValue(VectorStyle::Color Color);
   /** Retorna el color del control */
   virtual VectorStyle::Color GetColourCtrlValue();
   /** Cambia el valor del slider */
   virtual void SetThresholdCtrlValue(double Threshold);
   /** Retorna el valor del slider */
   virtual double GetThresholdCtrlValue();

   static const double DEFAULT_THRESHOLD;  /*! threshold inicial */
   static const VectorStyle::Color DEFAULT_COLOR;  /*! color inicial */
};
}

#endif /* PARALLELEPIPEDPARAMETERSPART_H_ */
