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

#ifndef ELLIPSOIDEDITIONPART_H_
#define ELLIPSOIDEDITIONPART_H_

// Includes standard
#include <string>

// Includes Suri
#include "LibraryItemEditorPart.h"
#include "suri/SuriObject.h"

namespace suri {

// forwards
class LibraryManager;

/**
 * Permite setear los parametros de un sistema de elipsoide,
 * para definir elipsoides propios.
 *
 * Los parametros que permite configurar son:
 * - Nombre.
 * - Autoridad:Codigo.
 * - Semieje Mayor.
 * - Semieje menor o Inversa Factor Achatamiento.
 *
 */
class EllipsoidEditionPart : public LibraryItemEditorPart {
   /** ctor copia**/
   EllipsoidEditionPart(const EllipsoidEditionPart &);

   /** Nombre del XRC que representa la ventana. */
   static const std::string ELLIPSOID_EDITION_XRC_NAME;
   /** Nombre del XRC que representa el panel donde se ubica el CGU */
   static const std::string ELLIPSOID_CGU_XRC_NAME;

public:
   /** Ctor*/
   EllipsoidEditionPart(
         const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
         UniversalGraphicalComponentPartInterface::FeatureCombinationEnum Feature);
   /** Dtor. */
   virtual ~EllipsoidEditionPart();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /**  Metodo que obtiene a partir de los datos del editor, un vector con los atributos
    *   Este metodo es el que hace el comportamiento particular de cada editor a la hora
    *   de obtener la informacion que posee y transmitirla al sistema de bibliotecas.
    *   Es utilizado a modo de Template Method (en CommitChanges y en el notificador)
    *   @return vector con atributos
    *   @return NULL en caso de que la informnacion del part no sea valida
    *   **/
   virtual std::vector<LibraryItemAttribute*>* CreateAttributesFromFields();

private:
   /**
    *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
    *  Este metodo es el que realmente tienen que implementar las clases hijas para
    * lograr la especializacion a la hora de iniciar el part (metodo que se llama
    * en CreateToolWindow, a modo de Template Method).
    **/
   virtual bool ConfigureFeatures();

   /**
    * Metodo auxiliar privado que es necesario implementar en las clases hijas.
    * Este metodo es el encargado de actualizar realmente la informacion en el part
    * Es llamado en el metodo SetActiveItem a modo de Template Method
    **/
   virtual void UpdateFields(const LibraryItem* pItem, bool Modifiable = true);
};
}

#endif /* ELLIPSOIDEDITIONPART_H_ */
