/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
