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

#ifndef SpatialReferenceEditionPart_H_
#define SpatialReferenceEditionPart_H_

// Includes standard
#include <string>
#include <vector>

// Includes Suri
#include "suri/SuriObject.h"
#include "UniversalGraphicalComponentPartInterface.h"

// Includes Wx

// Defines
#define ID_SPATIAL_SUBSET_SELECTION_PART "ID_SPATIAL_SUBSET_SELECTION_PART"

namespace suri {

// forwards
class LibraryManager;
class Table;
class TablePart;
DECLARE_EVENT_OBJECT(ParametersTableEventHandler);

/**
 * Permite setear los parametros de un sistema de referencia geografico,
 * para definir sistemas de referencia propios.
 *
 * Los parametros que permite configurar son:
 * - Nombre
 * - Autoridad:Codigo
 * - Datum
 * - Unidades
 * - ejes gemelos (si se usan y su orientacion)
 *
 */
class SpatialReferenceEditionPart : public LibraryItemEditorPart {
   /** Nombre del XRC que representa la ventana. */
   static const std::string SPATIAL_REFERENCE_EDITION_XRC_NAME;
   /** Nombre del XRC que representa el panel del cgu*/
   static const std::string SPATIAL_REFERENCE_CGU_XRC_NAME;
   /** ctor copia **/
   SpatialReferenceEditionPart(const SpatialReferenceEditionPart&);

public:
   /** Ctor */
   SpatialReferenceEditionPart(const SuriObject::UuidType& LibraryId,
                               LibraryManager* pManager,
                               UniversalGraphicalComponentPartInterface::
                               FeatureCombinationEnum Feature =
                                     UniversalGraphicalComponentPartInterface::
                                     SaveFavoritesAndNoLabel);
   /** Dtor */
   virtual ~SpatialReferenceEditionPart();
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
   /** Metodo auxiliar que se encarga de rellenar los campos del editor con
    *  la informacion pasada por parametro */
   void FillTextFields(const std::string& Authority, const std::string& Code,
                       const std::string& DatumOrGeoSR, const std::string& units,
                       const std::string& Axis1, const std::string& Axis2,
                       bool Modifiable = true);
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
   /** Metodo auxiliar encargado de configurar la tabla **/
   void ConfigureTable(bool Enable = true, const std::string& Wkt ="");
   /** Metodo auxiliar que se llama cuando se presiona el boton de agregar fila **/
   void OnAddRowClick(const wxCommandEvent& Event);
   /** Metodo auxiliar que se llama cuando se presiona el boton de eliminar fila **/
   void OnDelRowClick(const wxCommandEvent& Event);
   /** Metodo auxiliar que se llama cuando se presiona el check de ejes gemelos **/
   void OnTwinAxisCheck(const wxCommandEvent& Event);
   /** Configura los controles graficos asociados a la configuracion de ejes
    *  gemelos en funcion de la activacion del Checkbox
    */
   void ConfigureTwinAxisControls(bool Check);
   /** A traves de la definicion de los parametros arma un wkt con la
    *  proyeccion y los parametros. Resultado algo similar a:
    *  PROJECTION["Transverse_Mercator"],
    *  PARAMETER["latitude_of_origin",0],
    *  PARAMETER["central_meridian",-81],
    *  PARAMETER["scale_factor",0.9996],
    *  PARAMETER["false_easting",500000],
    *  PARAMETER["false_northing",0]
   **/
   std::string GetProjectionParametersWkt();
   /** Obtiene el wkt asociado a los ejes gemelos definidos. El wkt resultado
    *  es la definicion de un par de ejes del estilo:
    *  AXIS["<name>", NORTH | SOUTH | EAST | WEST | UP | DOWN | OTHER]
     **/
   std::string GetTwinAxisWkt();
   /** Metodo que a partir del nombre y orientacion del eje genera el wkt asociado
    * @param[in] Name nombre del eje
    * @param[in] Orientation orientacion del eje
    * @param[out] wkt resultante de la generacion con formato:
    *  AXIS["<name>", NORTH | SOUTH | EAST | WEST | UP | DOWN | OTHER]
    **/
   void GetAxisWkt(const std::string& Name, const std::string& Orientation,
                   std::string& Wkt);
   /** Metodo que a partir de la clave y el valor del parametro obtiene el wkt
    *  correspondiente
    *  @param[in] Key clave(nombre) del parametro
    *  @param[in] Valor del parametro
    *  @param[out] Wkt wkt resultado: PARAMETER["<name>", <value>]
    */
   void GetParameterWkt(const std::string& Key, const std::string& Value,
                        std::string& Wkt);
   /** Metodo que a partir de la unidad del sistema devuelve el wkt asociado **/
   std::string GetUnitWkt();
   /** Metodo que en funcion de la autoridad y codigo definida genera el wkt correspondiente **/
   std::string GetAuthorityWkt();
   /** Genera el wkt correspondiente al sistema de referencia configurado en el editor */
   std::string GetSpatialReferenceWkt(bool Projected = true);
   /** Obtiene el wkt del item asociado al sr que se esta configurando
    *  @return string con el wkt obtenido
    *  @return string vacio en caso de no poder obtener el wkt correcto
    */
   std::string GetAssociatedItemWkt(bool Projected);
   /** Puntero al cgu asociado al SR que se quiere editar **/
   UniversalGraphicalComponentPartInterface* pDatumSelector_;
   /** Puntero al administrador de biblioteca para interactuar con la misma **/
   LibraryManager* pManager_;
   /** Tabla que se usa para asignar las bandas */
   Table* pTable_;
   /** Part que contiene la tabla **/
   TablePart* pTablePart_;
   FRIEND_EVENT_OBJECT(ParametersTableEventHandler);
   EVENT_OBJECT_PROPERTY(ParametersTableEventHandler);
   /** Entero que marca el indice seleccionado **/
   int unitIndexSelected_;
   /** wkt resultado de la configuracion del sistema de referencia */
   std::string resultWkt_;
   /** Puntero al ultimo item seleccionado.
    *  Esto cachea el item para evitar actualizar siempre el mismo */
   const LibraryItem* pItemCurrentSel_;
   /** Referencia a la biblioteca de datums **/
   const Library* pDatumLibrary_;
};
}

#endif /* SpatialReferenceEditionPart_H_ */
