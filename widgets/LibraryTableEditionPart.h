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

#ifndef LIBRARYTABLEEDITIONPART_H_
#define LIBRARYTABLEEDITIONPART_H_

// Includes Estandar
// Includes Suri
#include "suri/PartCollection.h"
#include "LibraryItemEditorPart.h"

// Includes Wx
// Defines

namespace suri {

// forwards
DECLARE_EVENT_OBJECT(LibraryTableEventHandler);
class Table;
class TablePart;
class wxGenericTableBase;

/**
 * Clase que encapsula funciones basicas del manejo de tablas en bibliotecas
 */
class LibraryTableEditionPart : public suri::LibraryItemEditorPart {
public:
   /** Constructor */
   LibraryTableEditionPart(
         const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
         const std::string EditionXrcName, const std::string CguPanel,
         UniversalGraphicalComponentPartInterface::FeatureCombinationEnum Feature);
   /** Destructor */
   virtual ~LibraryTableEditionPart();
   /**  Metodo que obtiene a partir de los datos del editor, un vector con los atributos
    *   Este metodo es el que hace el comportamiento particular de cada editor a la hora
    *   de obtener la informacion que posee y transmitirla al sistema de bibliotecas.
    *   Es utilizado a modo de Template Method (en CommitChanges y en el notificador)
    *   @return vector con atributos
    *   @return NULL en caso de que la informnacion del part no sea valida
    *   **/
   virtual std::vector<LibraryItemAttribute*>* CreateAttributesFromFields();
   /** Indica si el Part tiene cambios para salvar */
   virtual bool HasChanged();

protected:
   FRIEND_EVENT_OBJECT(LibraryTableEventHandler);
   EVENT_OBJECT_PROPERTY(LibraryTableEventHandler);
   /**
    * Metodo auxiliar privado que configura el part en si para cada editor en particular.
    * Este metodo es el que realmente tienen que implementar las clases hijas para lograr
    * la especializacion a la hora de iniciar el part (metodo que se llama en
    * CreateToolWindow, a modo de Template Method).
    */
   virtual bool ConfigureFeatures();
   /**
    * Metodo auxiliar privado que es necesario implementar en las clases hijas.
    * Este metodo es el encargado de actualizar realmente la informacion en el part
    * Es llamado en el metodo SetActiveItem a modo de Template Method
    **/
   virtual void UpdateFields(const LibraryItem* pItem, bool Modifiable = true);
   /**
    * Metodo auxiliar privado que agrega comportamiento adicional al commit de la
    * clase base. Este comportamiento adicional es necesario cuando
    * el commit involucra objetos adicionales a los items de biblioteca
    * del editor (ej. reproyeccion, en donde se necesita actualizar los params
    * de reproyeccion ademas de leer el item de biblioteca)
    * @return true en caso de realizar correctamente el commit "interno"
    * @return false en caso de que ocurra algun error durante el commit
    */
   virtual bool OnCommitChanges();
   /**
    * Metodo auxiliar privado que agrega comportamiento adicional al rollback de la
    * clase base. Este comportamiento adicional es necesario cuando
    * el rollback involucra objetos adicionales a los items de biblioteca
    * del editor (ej. parts compuesto en donde se necesita llamar a los rollbacks de
    * los mismos)
    * @return true en caso de realizar correctamente el rollback "interno"
    * @return false en caso de que ocurra algun error durante el rollback
    */
   virtual bool OnRollback();
   /**
    * Metodo auxiliar que carga el contenido del item complejo seleccionado en la tabla del Part
    * @param[in] pItem item que sera representado por la tabla
    */
   virtual void LoadItemToTable(const LibraryItem* pItem, bool AddRangeToItem = false)=0;
   /** Metodo que se llama cuando se presiona el boton de agregar columna */
   virtual void OnAddRowClick(wxCommandEvent& Event);
   /** Metodo que se llama cuando se presiona el boton de eliminar columna */
   virtual void OnDeleteRowClick(wxCommandEvent& Event);
	/**
	 * Obtiene de la tabla los items complejos cargados y crea un item asociado
	 * @param pAttributes vector en donde se guardaran los atributos de la tabla
	 */
   virtual void LoadAttributesFromTable(std::vector<LibraryItemAttribute*>* &pAttributes)=0;
   /** Agrega las columnas de la tabla */
   virtual void AppendColumns()=0;
   /** Tabla que contiene los intervalos */
   Table* pTable_;
   TablePart* pTablePart_;
   /** Item en edicion **/
   const LibraryItem* pCurrentItem_;
};

} /** namespace suri */

#endif /* LIBRARYTABLEEDITIONPART_H_ */
