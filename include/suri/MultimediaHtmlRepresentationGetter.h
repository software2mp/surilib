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

#ifndef __MULTIMEDIA_HTML_FILE_REPRESENTATION_H__
#define __MULTIMEDIA_HTML_FILE_REPRESENTATION_H__

// Include Estandar
#include <string>

// Includes Suri
#include "suri/ElementHtmlRepresentationGetter.h"

class wxString;

namespace suri {
// forward
class Element;
/** 
 *  que define el comportamiento esperado de toda clase encargada de obtener la
 * representacion html de una capa. 
**/
class MultimediaHtmlRepresentationGetter : public ElementHtmlRepresentationGetter {
   /** ctor copia **/
   MultimediaHtmlRepresentationGetter(const MultimediaHtmlRepresentationGetter&);

public:
   /** ctor default **/
   MultimediaHtmlRepresentationGetter();
   /** dtor */
   virtual ~MultimediaHtmlRepresentationGetter();
   /** Metodo polimorfico encargado de obtener la representacion html de la capa que se
    *  encuentra por parametro.
    * @param[in] pElement elemento sobre el que se obtendra su representacion html
    * @param[out] Representation representacion en formato html del elemento
    * @return true en caso de poder obtener correctamente la interpretacion html del elemento
    * @return false en caso contrario 
   **/
   virtual bool DoGetRepresentation(const Element* pElement, std::string& Representation) const;

private:
   /**
    * Objetivo: Verifica que exista el directorio donde se encuentran los templates
    * html para visualizar la informacion del PAT.
    * En caso de que el directorio no exista lo crea y extrae los templates.
    * @param[in]	Path: path del directorio a verificar.
    * @return si existe o si puede crearlo retorna true; otro caso false.
    */
   bool CheckDataDirectory(const wxString &Path) const;
};
}  /** namespace suri **/
#endif   // __MULTIMEDIAMULTIMEDIA_HTML_FILE_REPRESENTATION_H__
