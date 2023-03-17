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

#ifndef UNIVERSALLIBRARYNAMEEDITIONPART_H_
#define UNIVERSALLIBRARYNAMEEDITIONPART_H_

// Includes standard
// Includes Suri
#include "suri/Part.h"

// Includes Wx
// Includes App
// Defines

namespace suri {
/** Editor del nombre de la biblioteca. */
/**
 * Editor del nombre de la biblioteca.
 * Permite seleccionar el nombre de un item de biblioteca.
 * El editor de nombre tiene :
 * - Texto de nombre
 * - Area para escribir el nombre
 * - Boton de guardar item
 * - Boton de eliminar item
 * - Checkbox de mostrar favoritos
 * Las opciones de representacion del editor son las siguientes:
 * - Mostrar/ocultar texto del nombre
 * - Mostrar en modo solo lectura
 * - Mostrar/ocultar texto del nombre Checkbox de mostrar favoritos
 */
class UniversalLibraryNameEditionPart : public suri::Part {
   /** Nombre del XRC que representa la ventana */
   static const std::string LIBRARY_NAME_EDITION_XRC_NAME;

public:

   /** Muestra el nombre */
   static const int SHOW_NAME = 0x01;

   /** Muestra el check de favoritos */
   static const int SHOW_FAVOURITES = 0x02;

   /** Pone el editor en modo read only */
   static const int READ_ONLY = 0x04;

   static const int SHOW_DELETE_BUTTON = 0x08;

public:
   UniversalLibraryNameEditionPart(int flags = SHOW_NAME | SHOW_FAVOURITES);
   virtual ~UniversalLibraryNameEditionPart();

   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valor's originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Permite a clases que hereden modificar el widget luego de crear la ventana */
   virtual bool ConfigureWidget();

protected:
private:
   int representationflags_;
};
}

#endif /* UNIVERSALLIBRARYNAMEEDITIONPART_H_ */
