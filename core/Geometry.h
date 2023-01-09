/** \file Geometry.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

// Includes standard
#include <string>

// Includes suri
#include "suri/SuriObject.h"

namespace suri {

// forward
class VectorStyle;

/**
 * Clase que representa una geometria.
 */
class Geometry : public SuriObject {
public:
   /** Ctor */
   explicit Geometry(const Geometry &AGeometry);
   /** Ctor */
   explicit Geometry(const std::string &SpatialReference = "");
   /** Dtor */
   virtual ~Geometry();
   /** Indica si la geometria es valida */
   virtual bool IsValid() const = 0;
   /** Indica si la geometria esta cerrada, solo valida si es poligono */
   virtual bool IsClosed() const;
   /** Fuerza a que se cierre la geometria. Solo para poligonos */
   virtual bool CloseRings();
   /** Retorna una copia de la geometria. Tambie copia subgeometrias */
   virtual Geometry* Clone() const = 0;
   /** Retorna el estilo configurado a la geometria */
   virtual VectorStyle* GetStyle() const;
   /** Configura el estilo en la geometria */
   virtual void SetStyle(VectorStyle *Style);
   /** Retorna el srs en el que estan los puntos de la geometria */
   virtual std::string GetSpatialReference() const;
   /** Configura el srs de la geometria. No reproyecta */
   virtual void AssignSpatialReference(const std::string &SpatialReference);

private:
   /** Estilo asignado a la geometria */
   VectorStyle* pStyle_;
   /** Sistema de referencia de los puntos en la geometria */
   std::string spatialReference_;
};

} /* namespace suri */
#endif /* GEOMETRY_H_ */
