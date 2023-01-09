/** \file GroundControlPoint.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GROUNDCONTROLPOINT_H_
#define GROUNDCONTROLPOINT_H_

#include <vector>

#include "suri/Coordinates.h"

namespace suri {

/**
 * Clase que reprensenta un punto de control.
 */
class GroundControlPoint {
public:
   /**
    * Constructor copia
    */
   GroundControlPoint(const GroundControlPoint& other);
   /**
    * Construye un punto de control a partir de las componentes
    * de las coordenadas de origen y destino.
    */
   GroundControlPoint(const double &Xsrc = 0.0, const double &Ysrc = 0.0,
                      const double &Zsrc = 0.0, const double &Xdest = 0.0,
                      const double &Ydest = 0.0, const double &Zdest = 0.0);

   /**
    * Construye un punto de control con las coordenadas de origen y destino.
    */
   GroundControlPoint(const Coordinates& Source, const Coordinates& Destination);

   /**
    * Destructor.
    */
   virtual ~GroundControlPoint();

   /**
    * Obtiene las coordenadas de origen.
    */
   Coordinates GetSource() const;

   /**
    * Obtiene las coordenadas de destino.
    */
   Coordinates GetDestination() const;

   /**
    * Obtiene todas las coordenadas de origen de un vector de puntos de control.
    */
   static std::vector<Coordinates> ExtractSourceCoordinates(
         const std::vector<GroundControlPoint>& vGcp);

private:
   Coordinates source_;  /** Coordenadas de origen */
   Coordinates destination_;  /** Coordenadas de destino */
};


/**
 * Clase que reprensenta un punto de control con informacion extra.
 */
class GroundControlPointExtended : public GroundControlPoint {
public:
   /**
    * Constructor copia
    */
   GroundControlPointExtended(const GroundControlPointExtended& other);
   /**
    * Construye un punto de control a partir de las componentes
    * de las coordenadas de origen y destino e indicando si el
    * punto de control esta habilitado o no.
    */
   GroundControlPointExtended(const double &Xsrc = 0.0, const double &Ysrc = 0.0,
                              const double &Zsrc = 0.0, const double &Xdest = 0.0,
                              const double &Ydest = 0.0, const double &Zdest = 0.0,
                              bool EnabledP = true);

   /**
    * Construye un punto de control con las coordenadas de origen y destino e indicando si el
    * punto de control esta habilitado o no.
    */
   GroundControlPointExtended(const Coordinates& SourceP, const Coordinates& DestinationP,
                              bool EnabledP = true);

   /**
    * Destructor.
    */
   virtual ~GroundControlPointExtended();

   /**
    * Indica si el punto de control esta habilitado para su uso o no.
    */
   bool IsEnabled() const;

private:
   bool enabled_;
};

}

#endif  // GROUNDCONTROLPOINT_H_
