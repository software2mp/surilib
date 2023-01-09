/** \file GeometryCollection.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GEOMETRYCOLLECTION_H_
#define GEOMETRYCOLLECTION_H_

// Includes standard
#include <vector>
#include <string>

// Includes suri
#include "Geometry.h"

namespace suri {

/**
 * Representa una collection de geometrias.
 * Todas las geometrias contenidas deben tener el mismo sistema de referencia.
 * La coleccion toma posesion de las geometrias y realiza delete en
 * destructor. Si se desea evitar, usar RemoveGeometry antes
 * de destruir.
 */
class GeometryCollection : public Geometry {
public:
   /** Ctor */
   explicit GeometryCollection(const GeometryCollection &ACollection);
   /** Ctor */
   explicit GeometryCollection(const std::string &SpatialReference = "");
   /** Dtor */
   virtual ~GeometryCollection();

   /** Indica si la geometria es valida */
   virtual bool IsValid() const;
   /** Retorna una copia de la geometria. Tambien copia subgeometrias */
   virtual Geometry* Clone() const;
   /** Configura el srs de la geometria */
   virtual void AssignSpatialReference(const std::string &SpatialReference);
   /** Indica si la geometria esta cerrada, solo valida si es poligono */
   virtual bool IsClosed() const;

   /** Elimina todas las geometrias contenidas */
   void Clear();
   /** Retorna vector con id de las geometrias contenidas */
   std::vector<SuriObject::UuidType> GetGeometries() const;
   /** Retorna la geometria a partir de un id */
   Geometry* GetGeometry(SuriObject::UuidType GeometryId) const;
   /** Saca la geometria con el id GeometryId(No elimina el puntero) */
   Geometry* RemoveGeometry(SuriObject::UuidType GeometryId);
   /** Elimina la geometria con el id GeometryId */
   bool DeleteGeometry(SuriObject::UuidType GeometryId);
   /** Agrega una geometria a la coleccion */
   bool InsertGeometryBefore(Geometry* pGeometry,
                   SuriObject::UuidType ReferenceGeometryId);
   /** Agrega una geometria al final de la coleccion */
   bool AppendGeometry(Geometry* pGeometry);

private:
   /** Valida si la geometria se puede agregar */
   bool ValidateGeometry(const Geometry* pGeometry);

   /** Vector con las geometrias de la coleccion */
   std::vector<Geometry*> geometries_;
};

} /* namespace suri */
#endif /* GEOMETRYCOLLECTION_H_ */
