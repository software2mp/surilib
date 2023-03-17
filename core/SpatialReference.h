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

#ifndef SPATIALREFERENCE_H_
#define SPATIALREFERENCE_H_

// Includes standard
#include <string>

// Includes Suri

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {

// forwards
class CoordinatesTransform;

class SpatialReference {
   SpatialReference(const SpatialReference&);

public:
   /** Inicializa referencia espacial con Wkt */
   explicit SpatialReference(const std::string& Wkt);
   /** Devuelve el nombre del Sistema Geografico */
   static std::string GetGeoCoordinateSystemName(const std::string &Wkt);
   /** Devuelve las unidades del Sistema Geografico */
   static std::string GetGeoCoordinateSystemUnits(const std::string &Wkt);
   /** Devuelve el Datum del Sistema Geografico */
   static std::string GetGeoCoordinateSystemDatumName(const std::string &Wkt);
   /** Devuelve el Elipsoide del Sistema Geografico */
   static std::string GetGeoCoordinateSystemSpheroidName(const std::string &Wkt);
   /** Devuelve string (authority_name:authority_code) del Sistema Geografico */
   static std::string GetGeoCoordinateSystemAuthorityData(const std::string &Wkt);
   /** Indica si el sistema coordenado es geografico. */
   static bool IsGeographic(const std::string &Wkt);
   /** Indica si el sistema coordenado es proyectado. */
   static bool IsProjected(const std::string &Wkt);

   /** Devuelve el nombre del Sistema Proyectado */
   static std::string GetProjCoordinateSystemName(const std::string &Wkt);
   /** Devuelve el nombre de la funcion de proyeccion */
   static std::string GetProjCoordinateSystemProjectionName(const std::string &Wkt);
   /** Devuelve las unidades del Sistema Proyectado */
   static std::string GetProjCoordinateSystemUnits(const std::string &Wkt);
   /** Devuelve string (authority_name:authority_code) del Sistema Proyectado */
   static std::string GetProjCoordinateSystemAuthorityData(const std::string &Wkt);
   /** Devuelve el sistema geografico asociado al proyectado **/
   static std::string GetProjGeoreferenceSpatialReference(
         const std::string &ProjectedWkt);
   /** Devuelve el wkt del Datum asociado al Sistema Geografico */
   static std::string GetGeoCoordinateSystemDatum(const std::string &Wkt);
   /** Devuelve el wkt del elipsoide asociado al Sistema Geografico */
   static std::string GetGeoCoordinateSystemSpheroid(const std::string &Wkt);
   /** Devuelve el eje semi mayor del elipsoide asociado al wkt*/
   static double GetSpheroidSemiMajorAxis(const std::string Wkt);
   /** Devuelve el factor de achatamiento del elipsoide asociado al wkt*/
   static double GetSpheroidInverseFlatteningFactor(const std::string Wkt);
   /** */
   static std::string GetSemiMajorAxis(const std::string Wkt);
   /** Determina si es un sistema de referencia "raster" **/
   static bool IsPixelLineSpatialRef(const std::string& SRWkt);
   /** Determina si es un sistema de referencia proyectado */
   static bool IsProjectedSpatialRef(const std::string &SRWkt);
   /** Determina si es un sistema de referencia geografico */
   static bool IsGeographicSpatialRef(const std::string &SRWkt);

   /** Devuelve string (authority_name:authority_code) del wkt */
   static std::string GetAuthorityId(const std::string &Wkt);
   /** Trata de obtener wkt con autoridad */
   static std::string GetWktWithAuthorityId(const std::string& AuthorityId);

   /** Trata de obtener wkt con autoridad */
   static std::string GetWktWithAuthority(const std::string &AuthorityName,
                                             const std::string &AuthorityCode);
   /** Obtiene el wkt del elipsoide que contiene el datun **/
   static std::string GetDatumSpheroidWkt(const std::string& Wkt);
   /** Devuelve un string con el wkt asociado */
   std::string GetWkt() const;
   /** Obtiene el wkt que representa los parametros bursa wolf. El mismo
    *  viene definido en un nodo del estilo:
    *  TOWGS84[-637,-549,-203,0,0,0,0],AUTHORITY["EPSG","6236"]]
    */
   static std::string GetBursaWolfDatumParameters(const std::string& DatumWkt);
   /** Retorna wkt del nodo especificado */
   static std::string GetNodeWkt(const std::string &Wkt,
                                            const std::string &Parameter);
   /** Verifica la validez del sistema de referencia que se pasa
    *  por parametro (en formato wkt) **/
   static bool IsValidSpatialReference(const std::string& Wkt);

private:
   static std::string GetSpatialReferenceParameter(const std::string &Wkt,
                                            const std::string &Parameter,
                                            const int Position = 0);

   std::string wkt_;
   std::string shortName_;
};
}  // namespace suri

#endif /* SPATIALREFERENCE_H_ */
