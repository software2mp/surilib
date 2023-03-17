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

#ifndef SERIALIZABLEFACTORY_H_
#define SERIALIZABLEFACTORY_H_


// Includes Estandar
#include <string>
// Includes Suri
// Includes Wx
// Defines
// forwards

class wxXmlNode;

namespace suri{

class Serializable;

/**
 * Clase que crea instancias de Serializable a partir de los XML
 */
class SerializableFactory{
public:
   /**
    * Crea una instancia de un serializable a partir de un string con la
    * representación xml del objeto
    * @pre: El xml debe estar bien formado.
    * @param[in]: String con una representación válida del objeto Serializable
    * @return: puntero a una instancia de Serializable
    */
   static Serializable* GetSerializable(const std::string& xml);
   static Serializable* GetSerializable(wxXmlNode* pRoot,
                                        const std::string& serializer = "");
private:
   /**
    * Crea una instancia de un serializable a partir del nombre del tag
    * de la raiz.
    */
   static Serializable* GetSerializableByName(wxXmlNode* pRoot);
   /**
    * Crea una instancia de un serializador a partir del atributo de la raiz
    * Luego utiliza este serializador para crear una instancia de Serializable.
    */
   static Serializable* GetSerializableBySerializer(wxXmlNode* pRoot,
                                                    const std::string& name);
   /**
    * Genera un vector de serializables a partir del tipo del primer hijo
    * @pre: El xml debe estar bien formado.
    */
   static Serializable* GetSerializableVector(wxXmlNode* pRoot);
};

}

#endif /* SERIALIZABLEFACTORY_H_ */

