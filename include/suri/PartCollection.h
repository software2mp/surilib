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

#ifndef PARTCOLLECTION_H_
#define PARTCOLLECTION_H_

// Includes standard
#include <list>

// Includes Suri
#include "suri/Part.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Contiene una agregacion de Parts(a nivel estructura de datos ). */
/**
 *  Las clases que hereden deberan definir la organizacion grafica y propagar
 * los eventos entre ellos.
 * Implementa metodos de Part propagando la llamada a los subparts.
 */
class PartCollection : public Part {
public:
   /** Ctor */
   PartCollection();
   /** Ctor */
   PartCollection(Widget* pWidget);
   /** Ctor */
   PartCollection(const std::string &XrcId, const std::string &PartName = "");
   /** Dtor */
   virtual ~PartCollection();
   /** Agrega un part a la coleccion */
   void AddPart(Part* pNewPart, const std::string &ParentWidget);
   /** Remueve un part de la coleccion */
   void RemovePart(Part* pPart);
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();

   /** Agrega a pWidget_ los widgets de todos los parts en partColection_. */
   virtual bool ConfigureWidget();

protected:
   std::list<std::pair<Part*, std::string> > partCollection_; /*! mapa con los parts */
   /* contenidos y el nombre del panel donde se agregaron. */
};
}  // namespace suri

#endif /* PARTCOLLECTION_H_ */
