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

#ifndef GMTMSXMLWRITER_H_
#define GMTMSXMLWRITER_H_

#include "TmsXmlWriter.h"

namespace suri {

/**
 * Genera el XML particular para Google Maps.
 */
class GMTmsXmlWriter : public TmsXmlWriter {
public:
   /**
    * Ctor.
    */
   GMTmsXmlWriter(TmsXmlWriter* pSuccessor = NULL);

   /**
    * Dtor.
    */
   virtual ~GMTmsXmlWriter();

   /**
    * Genera el XML correspondiente al servicio y basado en los parametros suministrados.
    */
   virtual std::string GetXml(const std::string& UrlStr,
                              std::map<std::string, std::string>& Params,
                              const Option& Metadata);

private:
   /**
    * Obtiene el ID de la capa en base al nombre de la misma.
    * map = m
    * satellite = s
    * hybrid = y
    * terrain = t
    * terrain, streets and water = p
    */
   static std::string GetLayerId(const std::string& LayerName);

   /**
    * Obtiene la descripcion de la capa en base al nombre de la misma.
    */
   static std::string GetLayerDescription(const std::string& LayerName);
};

}  // namespace suri

#endif  // GMTMSXMLWRITER_H_
