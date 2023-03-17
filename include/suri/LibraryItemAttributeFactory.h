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

#ifndef LIBRARYITEMATTRIBUTEFACTORY_H_
#define LIBRARYITEMATTRIBUTEFACTORY_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
// Includes Wx
// Defines

namespace suri {

// forwards
class RangeItemAttribute;
class SpectralSignItemAttribute;
class ComplexItemAttribute;
class LibraryItemAttribute;
class LibraryLinkItemAttribute;
class ConvolutionFilterItemAttribute;

/*!
 * Clase encargada de la creacion de instancias de atributos complejos.
 * El objetivo de esta clase es centralizar y abstraer la creacion de atributos complejos
 * Si bien la clase lleva en el nombre "Factory" el comportamiento se asemeja mas al
 * de un Builder. Se sigue utilizando "Factory" para ser coherente con el resto de la
 * aplicacion.
 * Colabora en la creacion de atributos que requieran particularidad por la informacion
 * que manejan.
 */
class LibraryItemAttributeFactory {
   /** ctor copia **/
   LibraryItemAttributeFactory(const LibraryItemAttributeFactory&);

public:
   /** Constructor **/
   LibraryItemAttributeFactory();
   /** Destructor */
   virtual ~LibraryItemAttributeFactory();
   /** A partir del atributo que se pasa por parametro,
    * crea una reinterpretacion del mismo como un atributo de rango.*/
   static RangeItemAttribute* CreateRangeItemAttribute(
         LibraryItemAttribute* pAttribute);
   /** Metodo que verifica si el atributo es del tipo rango **/
   static bool IsRangeItemAttribute(const LibraryItemAttribute* pAttribute);
   /** Metodo que reinterpreta un atributo como uno de link a biblioteca
    * generando una instancia de la nueva interpretacion*/
   static LibraryLinkItemAttribute* CreateLibLinkAttribute(
         const LibraryItemAttribute* pAttribute);
   /** A partir del atributo que se pasa por parametro,
    * crea una reinterpretacion del mismo como un atributo complejo.*/
   static SpectralSignItemAttribute* CreateSpectralSignItemAttribute(
         const LibraryItemAttribute* pAttribute);
   /** A partir del atributo que se pasa por parametro,
    * crea una reinterpretacion del mismo como un atributo filtro de convolucion
    */
   static ConvolutionFilterItemAttribute* CreateConvolutionFilterItemAttribute(
         const LibraryItemAttribute* pAttribute, int Dimension);
   /** Metodo que verifica si el atributo es del tipo filtro de convolucion **/
   static bool IsConvolutionFilterItemAttribute(const LibraryItemAttribute* pAttribute);

   /** String que describe los atributos de tipo firma espectral **/
   static const std::string SpectralSignString;
   /** String que describe los atributos complejos **/
   static const std::string ComplexString;
   /** String que describe los atributos de tipo rango **/
   static const std::string RangeString;
   /** String que describe los atributos de tipo de filtros de convolución. */
   static const std::string ConvolutionFilterString;
private:
   /** String que describe los atributos de tipo link de biblioteca **/
   static const std::string LibLinkString;
};

} /** namespace suri */

#endif /* LIBRARYITEMATTRIBUTEFACTORY_H_ */
