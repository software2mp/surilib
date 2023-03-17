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

#ifndef MAPELEMENT_H_
#define MAPELEMENT_H_

// -------------------------------- MAP ELEMENT --------------------------------
// Includes standard

// Includes Suri
#include "suri/Element.h"
#include "suri/Part.h"

// Includes Wx

// Includes App

// Defines

// Forwards

/** namespace suri */
namespace suri {
/** Clase que representa un elemento Overlay Mapa */
/**
 * Clase que representa el Overlay de Mapa creado por la herramienta de Mapa.
 *
 * En el nodo "renderizacion|mapa" que guarda los parametros (leyenda, grilla,
 * norte, escala) que luego seran renderizados por el MapRenderer.
 * El nodo mencionado es actualizado por la clase MapTool.
 * \note los parametros guardados en el nodo son booleanos, solo indican si
 * dichos parametros fueron activados.
 * \note Tener en cuenta que el nodo "grilla" este no es utilizado ya que
 * la grilla se trata como un caso aparte.
 */
class MapElement : public Element {
   /** Ctor. de Copia. */
   MapElement(const MapElement &MapElement);

public:
   /** Constructor */
   MapElement();
   /** Destructor */
   virtual ~MapElement();
   /** retorna el nombre del elemento */
   virtual wxString GetName() const {
      return _(label_MAP_LAYERS);
   }
   /** retorna los detalles del elemento */
   virtual wxString GetDetails() const {
      return wxT("");
   }
   /** Muestra la herramienta de propiedades */
   virtual void ShowPropertyTool() {
   }
   ;
   /** Funcion estatica para crear un MapElement */
   static MapElement* Create();

protected:
private:
   /** Inicializa elemento con archivo en ruta indicada */
   MapElement* Initialize(const std::string &FileName);
};
}  // namespace suri

#endif /*MAPELEMENT_H_*/
