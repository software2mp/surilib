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

#ifndef DATUMEDITIONPART_H_
#define DATUMEDITIONPART_H_

// Includes standard
#include <string>
#include <vector>

// Includes Suri
#include "LibraryItemEditorPart.h"
#include "suri/SuriObject.h"

namespace suri {

// forwards
class LibraryManager;
class UniversalGraphicalComponentPart;
class Table;
class TablePart;
DECLARE_EVENT_OBJECT(DatumParametersTableEventHandler);

/**
 * Permite setear los parametros de un datum,
 * para definir datum propios.
 *
 * Los parametros que permite configurar son:
 * - Nombre
 * - Autoridad:Codigo
 * - Elipsoide
 * - Parametros Bursa-Wolf (para transformar a WGS84)
 *
 */
class DatumEditionPart : public LibraryItemEditorPart {
   /** Nombre del XRC que representa la ventana */
   static const std::string DATUM_EDITION_XRC_NAME;
   static const std::string DATUM_EDITION_CGU_XRC_NAME;
   /** XRC que contendra al cgu para el elipsoide asociado **/
   static const std::string ELLIPSOID_EDITION_CGU_XRC_NAME;

   /** ctor copia **/
   DatumEditionPart(const DatumEditionPart&);

public:
   /** Ctor. Carga el XRC de la pantalla */
   DatumEditionPart(
         const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
         UniversalGraphicalComponentPartInterface::FeatureCombinationEnum Feature);
   /** Dtor. */
   virtual ~DatumEditionPart();
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
   /** Configura la tabla para mostrar los parametros bursa wolf para conversion
    *  del datum a wgs84
    *  @param[in] Wkt wkt del datum
    */
   void ConfigureBursaWolfParametersTable(const std::string& Wkt, bool Enable);
   /** Obtiene el wkt en funcion de los parametros definidos en la tabla **/
   std::string GetParametersWkt();
   /** Metodo auxiliar que se llama cuando se presiona el boton de agregar fila **/
   void OnAddRowClick(const wxCommandEvent& Event);
   /** Metodo auxiliar que se llama cuando se presiona el boton de eliminar fila **/
   void OnDelRowClick(const wxCommandEvent& Event);
   /** adminsitrador de la biblioteca */
   LibraryManager* pManager_;
   /** Cgu que mostrara el elipsoide asociado al datum*/
   UniversalGraphicalComponentPart* pSpheroidCGgu_;
   /** Tabla que se usa para asignar las bandas */
   Table* pTable_;
   /** Part que contiene la tabla **/
   TablePart* pTablePart_;
   FRIEND_EVENT_OBJECT(DatumParametersTableEventHandler);
   EVENT_OBJECT_PROPERTY(DatumParametersTableEventHandler);
   /** Puntero al ultimo item seleccionado. Esto evita que se actualice todo el tiempo la tabla */
   const LibraryItem* pItemCurrentSel_;
};
}

#endif /* DATUMEDITIONPART_H_ */
