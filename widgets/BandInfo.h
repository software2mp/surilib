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

#ifndef BANDINFO_H_
#define BANDINFO_H_

// Includes Estandar
#include <string>

// Includes Suri

#include <suri/Serializable.h>
// Includes Wx
// Defines
// forwards

namespace suri {

// Forwards
class RasterElement;

/**
 * Clase que contiene informacion sobre la banda en la que se esta
 * trabajando. Contiene el elemento al que pertence, la posicion de
 * la banda en el elemento y el nombre de la misma (para mostrar en
 * pantalla)
 */
class BandInfo : public Serializable {
public:
   /**
    *
    */
   explicit BandInfo();
   /**
    * Ctor.
    */
   BandInfo(suri::RasterElement* pElementP, int PositionP, const std::string& NameP,
            int Width, int Height, const std::string& TypeP);
   /**
    * Copy Ctor.
    */
   BandInfo(const BandInfo& bandInfo);
   /**
    * Dtor
    */
   virtual ~BandInfo();

   /**
    * Devuelve el elemento al que pertenece la banda.
    */
   suri::RasterElement* GetElement() const;

   /**
    * Devuelve la posicion de la banda en el elemento.
    */
   int GetPosition() const;

   /**
    * Devuelve el nombre de la banda (junto con el nombre del elemento)
    */
   std::string GetName() const;

   /**
    * Devuelve el ancho de la banda.
    */
   int GetWidth() const;

   /**
    * Devuelve el alto de la banda.
    */
   int GetHeight() const;

   /**
    * Devuelve el tipo de la banda.
    */
   std::string GetType() const;

   /**
    *
    */
   static std::string ExtractType(suri::RasterElement* pElement);

   wxXmlNode* SerializeXml() const;

   static BandInfo* DeserializeXml(wxXmlNode* pRoot);

private:
   suri::RasterElement* pElement_;  // Elemento al que pertenece la banda.
   int position_;  // posicion de la banda en el elemento.
   std::string name_;  // nombre de la banda.
   int width_;
   int height_;
   std::string type_;

};

} /** namespace suri */

#endif /* BANDINFO_H_ */
