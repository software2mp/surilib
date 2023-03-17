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

#ifndef TMSXMLWRITER_H_
#define TMSXMLWRITER_H_

#include <map>
#include <string>

namespace suri {

class Option;

/**
 * Clase base para la cadena de responsabilidad para la generacion
 * del XML para cada servicio TMS.
 */
class TmsXmlWriter {
public:
   /**
    * Ctor.
    */
   TmsXmlWriter(TmsXmlWriter* pSuccessor = NULL);

   /**
    * Dtor.
    */
   virtual ~TmsXmlWriter();

   /**
    * Genera el XML correspondiente al servicio y basado en los parametros suministrados.
    */
   virtual std::string GetXml(const std::string& UrlStr,
                              std::map<std::string, std::string>& Params,
                              const Option& Metadata)=0;

protected:
   /**
    * Obtiene al sucesor.
    */
   TmsXmlWriter* GetSuccessor();

private:
   TmsXmlWriter* pSuccessor_;
};

} /** namespace suri */

#endif  // TMSXMLWRITER_H_
