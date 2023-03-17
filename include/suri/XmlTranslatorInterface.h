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

#ifndef XMLTRANSLATORINTERFACE_H_
#define XMLTRANSLATORINTERFACE_H_


// Includes Estandar

// Includes Suri

// Includes Wx
#include "wx/xml/xml.h"

// Defines

// forwards

namespace suri {
/**
 * Interfaz que representa el comportamiento en comun para todos los traductores XML
 * (podria ser analogo a una XSLT)
 */
class XmlTranslatorInterface {
public:
   XmlTranslatorInterface() {}
   virtual ~XmlTranslatorInterface() {}
   /**
    * Realiza la traduccion XML directa o inversa que es representada por la clase.
    * Devuelve el XML resultante o NULL en caso de no poder realizar la traduccion.
    * @param[in] Direct bool que indica si la transformacion es directa (true)
    * o inversa (false)
    * @return referencia a xml creado, NULL en caso de no poder generar la traduccion.
    */
   virtual wxXmlNode* GenerateXmlTranslation(bool Direct = true)const=0;
   /**
    * Aplica la traduccion XML directa o inversa que es representada por la clase,
    * actualizando el contenido, que proviene del obtjeto a traducir, del objeto que
    * recibe el contenido traducido.
    * (ej. con la informacion de una !LayerInterface concreta actualiza el XML de Element).
    * En caso de que el elemento externo sea NULL, la traduccion no se aplica ya que no
    * tiene sentido.
    * @param Direct bool que indica si la transformacion es directa (true) o inversa (false)
    * @return si pudo aplicarse la traduccion, false en caso contrario
    */
   virtual bool ApplyTranslation(bool Direct = true)=0;

protected:
   /**
    * Valida que se tenga los requerimientos necesarios para poder realizar
    * la traduccion.
    * @return true en caso de poder realizar la traduccion
    * @return false en caso contrario
    */
   virtual bool IsPossibleToTranslate()const=0;
};
}  // namespace suri



#endif /* XMLTRANSLATORINTERFACE_H_ */
