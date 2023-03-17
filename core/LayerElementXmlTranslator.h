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

#ifndef LayerElementXmlTranslator_H_
#define LayerElementXmlTranslator_H_

// Includes Estandar

// Includes Suri
#include "suri/XmlTranslatorInterface.h"
#include "suri/Element.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/*!
 * Implementacion concreta de XmlTranslatorInterface. Es la encargada de realizar
 * la traduccion del contenido de los XML de LayerInterface y de Element
 */
class LayerElementXmlTranslator : public suri::XmlTranslatorInterface {
public:
   LayerElementXmlTranslator(LayerInterface* pLayer, Element* pElement);
   virtual ~LayerElementXmlTranslator();
   /**
    * Realiza la traduccion XML directa o inversa que es representada por la clase.
    * Devuelve el XML resultante o NULL en caso de no poder realizar la traduccion.
    */
   virtual wxXmlNode* GenerateXmlTranslation(bool Direct = true) const;
   /**
    * Aplica la traduccion XML directa o inversa que es representada por la clase,
    * actualizando el contenido, que proviene del obtjeto a traducir, del objeto que
    * recibe el contenido traducido.
   */
   virtual bool ApplyTranslation(bool Direct = true);

private:
   /** Metodo auxiliar que genera la traduccion Layer->Element **/
   wxXmlNode* GenerateDirectTranslation() const;
   /** Metodo auxiliar que genera la traduccion Element->Layer **/
   wxXmlNode* GenerateReverseTranslation() const;
   /** Metodo auxiliar que genera la traduccion Layer->Element **/
   bool ApplyDirectTranslation();
   /** Metodo auxiliar que genera la traduccion Element->Layer **/
   bool ApplyReverseTranslation();
   /** Determina que tipo de elemento se usa. El interno a la capa o el externo **/
   Element* GetElementInUse() const;
   /** Valida que se tenga los requerimientos necesarios para poder realizar
    * la traduccion. **/
   virtual bool IsPossibleToTranslate() const;
   /** Puntero a la capa **/
   LayerInterface* pLayer_;
   /** Puntero al element **/
   Element* pElement_;
};

} /** namespace suri */

#endif /* LayerElementXmlTranslator_H_ */
