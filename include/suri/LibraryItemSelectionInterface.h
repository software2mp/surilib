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

#ifndef LIBRARYITEMSELECTIONINTERFACE_H_
#define LIBRARYITEMSELECTIONINTERFACE_H_

// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {
class LibraryItem;

/* Interfaz necesaria para generalizar la creacion de LibraryItemEditorPart de la cual
 * deberan heredar las clases en las que se quiera incluir una biblioteca */

class LibraryItemSelectionInterface {
public:
   /** Constructor */
   LibraryItemSelectionInterface() {};
   /** Destructor */
   virtual ~LibraryItemSelectionInterface() {};
   /** Configura el editor con el item que se pasa por parametro. Se pasa por parametro
    * un boolean que indica si se trata de la primera seleccion (para que el part no piense
    * que posee cambios cuando en realidad es la seleccion inicial del CGU)*/
   virtual void SetActiveItem(const LibraryItem* pItem)=0;
};

} /** namespace suri */

#endif /* LibraryItemSelectionInterface_H_ */
