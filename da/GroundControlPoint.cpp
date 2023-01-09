/** \file GroundControlPoint.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/GroundControlPoint.h"

namespace suri {
/**
 * Constructor copia
 */
GroundControlPoint::GroundControlPoint(const GroundControlPoint& other) {
   this->source_ = other.source_;
   this->destination_ = other.destination_;
}

   
/**
 * Construye un punto de control a partir de las componentes
 * de las coordenadas de origen y destino.
 *
 * @param[in] Xsrc componente X de la coordenada de origen.
 * @param[in] Ysrc componente Y de la coordenada de origen.
 * @param[in] Zsrc componente Z de la coordenada de origen.
 * @param[in] Xdest componente X de la coordenada de destino.
 * @param[in] Ydest componente Y de la coordenada de destino.
 * @param[in] Zdest componente Z de la coordenada de destino.
 */
GroundControlPoint::GroundControlPoint(const double &Xsrc, const double &Ysrc,
                                       const double &Zsrc, const double &Xdest,
                                       const double &Ydest, const double &Zdest) {
   source_ = Coordinates(Xsrc, Ysrc, Zsrc);
   destination_ = Coordinates(Xdest, Ydest, Zdest);
}

/**
 * Construye un punto de control con las coordenadas de origen y destino.
 *
 * @param[in] Source Coordenadas de origen.
 * @param[in] Destination Coordenadas de destino.
 */
GroundControlPoint::GroundControlPoint(const Coordinates& Source, const Coordinates& Destination) {
   source_ = Source;
   destination_ = Destination;
}

/**
 * Destructor.
 */
GroundControlPoint::~GroundControlPoint() { }

/**
 * Obtiene las coordenadas de origen.
 *
 * @return Devuelve las coordenadas de origen del punto de control.
 */
Coordinates GroundControlPoint::GetSource() const {
   return source_;
}

/**
 * Obtiene las coordenadas de destino.
 *
 * @return Devuelve las coordenadas de destino del punto de control.
 */
Coordinates GroundControlPoint::GetDestination() const {
   return destination_;
}

/**
 * Obtiene todas las coordenadas de origen de un vector de puntos de control.
 *
 * @param[in] vGcp Vector de puntos de control del cual se desea extraer todos
 * los puntos de origen.
 * @return Vector con todos los puntos de origen extraidos.
 */
std::vector<Coordinates> GroundControlPoint::ExtractSourceCoordinates(
      const std::vector<GroundControlPoint>& vGcp) {
   std::vector<Coordinates> sources;
   std::vector<GroundControlPoint>& auxgcps = const_cast<std::vector<GroundControlPoint>&>(vGcp);

   for (int ix = 0, len = auxgcps.size(); ix < len; ++ix) {
      sources.push_back(auxgcps[ix].GetSource());
   }
   return sources;
}

GroundControlPointExtended::GroundControlPointExtended
 (const GroundControlPointExtended& other) : GroundControlPoint(other) {
   this->enabled_ = other.enabled_;
}


/**
 * Construye un punto de control a partir de las componentes
 * de las coordenadas de origen y destino e indicando si el
 * punto de control esta habilitado o no.
 */
GroundControlPointExtended::GroundControlPointExtended(const double &Xsrc, const double &Ysrc,
                                                       const double &Zsrc, const double &Xdest,
                                                       const double &Ydest, const double &Zdest,
                                                       bool EnabledP) :
      GroundControlPoint(Xsrc, Ysrc, Zsrc, Xdest, Ydest, Zdest), enabled_(EnabledP) { }

/**
 * Construye un punto de control con las coordenadas de origen y destino e indicando si el
 * punto de control esta habilitado o no.
 */
GroundControlPointExtended::GroundControlPointExtended(const Coordinates& SourceP,
                                                       const Coordinates& DestinationP,
                                                       bool EnabledP) :
      GroundControlPoint(SourceP, DestinationP), enabled_(EnabledP) { }

/**
 * Destructor.
 */
GroundControlPointExtended::~GroundControlPointExtended() { }

/**
 * Indica si el punto de control esta habilitado para su uso o no.
 */
bool GroundControlPointExtended::IsEnabled() const {
   return enabled_;
}

}  // namespace suri
