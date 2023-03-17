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

#ifndef EQUATIONINSITUEDITORPART_H_
#define EQUATIONINSITUEDITORPART_H_

// Includes Estandar
// Includes Suri
#include "LibraryItemEditorPart.h"
// Includes Wx
// Defines

namespace suri {

// forwards
class EquationVariableMappingPart;

/**
 * Clase que representa un editor de items de biblioteca de ecuaciones.
 */
class EquationInSituEditorPart : public LibraryItemEditorPart {
   /** Constructor copia**/
   EquationInSituEditorPart(const EquationInSituEditorPart&);
public:
   /** Constructor */
   EquationInSituEditorPart(
         const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
         UniversalGraphicalComponentPartInterface::FeatureCombinationEnum Feature);
   /** Destructor */
   virtual ~EquationInSituEditorPart();
private:
   /**
    * Metodo auxiliar privado que es necesario implementar en las clases hijas.
    * Este metodo es el encargado de actualizar realmente la informacion en el part
    * Es llamado en el metodo SetActiveItem a modo de Template Method
    **/
   virtual void UpdateFields(const LibraryItem* pItem, bool Modifiable = true);
   /**  Metodo que obtiene a partir de los datos del editor, un vector con los atributos
    *   Este metodo es el que hace el comportamiento particular de cada editor a la hora
    *   de obtener la informacion que posee y transmitirla al sistema de bibliotecas.
    *   Es utilizado a modo de Template Method (en CommitChanges y en el notificador)
    *   @return vector con atributos
    *   @return NULL en caso de que la informnacion del part no sea valida
    *   **/
   virtual std::vector<LibraryItemAttribute*>* CreateAttributesFromFields();
   /**
    *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
    *  Este metodo es el que realmente tienen que implementar las clases hijas para
    * lograr la especializacion a la hora de iniciar el part (metodo que se llama
    * en CreateToolWindow, a modo de Template Method).
    **/
   virtual bool ConfigureFeatures();
   /** Indica si el Part tiene cambios para salvar */
   bool HasChanged();
   /** Verifica la validez de ecuacion */
   bool IsValidEquation(const std::string& Equation) const;
   std::vector< std::pair<std::string, std::string> > GetVariableDescription(const LibraryItem* pItem);

   /** xrc de panel de editor de density slice **/
   static const std::string EQUATION_EDITION_XRC_NAME;
   /** id del panel donde se ubica el cgu **/
   static const std::string EQUATION_CGU_XRC_NAME;
   /** Contiene el string con la ecuacion */
   std::string equationvalue_;
   EquationVariableMappingPart* pVariableMappingPart_;
};

} /** namespace suri */

#endif /* EQUATIONINSITUEDITORPART_H_ */
