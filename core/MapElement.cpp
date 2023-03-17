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

// Includes standard

// Includes Suri
#include "MapElement.h"
#include "suri/xmlnames.h"

// Includes Wx

// Includes App
#include "MapRenderer.h"

// Defines

// Forwards

/** namespace suri */
namespace suri {

/**
 * Constructor
 */
MapElement::MapElement() :
      Element(wxT(TYPE_PROPERTY_VALUE_MAP)) {
}

/**
 * Destructor
 */
MapElement::~MapElement() {
}

/**
 * Funcion estatica para crear un MapElement
 * @return puntero a un elemento o null.
 * \attention quien invoque a este metodo es responsable del puntero devuelto,
 * por lo tanto debe hacerse cargo de su eliminacion.
 */
MapElement* MapElement::Create() {
   MapElement *pelement = new MapElement;
   if (pelement->Initialize("") != pelement) {
      return NULL;
   }
   return pelement;
}

MapElement* MapElement::Initialize(const std::string &FileName) {
   // Nodos de renderizacion - No se traduce, elemento de XML
   wxXmlNode *prnode = AddNode(GetNode(wxT("")), wxT(RENDERIZATION_NODE));
   AddNode(prnode, MapRenderer::GetXmlNode());
   // Nodo Archivo - No se traduce, elemento de XML
   SetFileNode(wxT(TYPE_PROPERTY_VALUE_MAP));
   return this;
}

} /** namespace suri */
