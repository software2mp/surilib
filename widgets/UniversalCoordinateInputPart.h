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

#ifndef UNIVERSALCOORDINATEINPUTPART_H_
#define UNIVERSALCOORDINATEINPUTPART_H_
// Includes Estandar
#include <string>
#include <vector>
#include <map>

// Includes suri
#include "suri/PartCollection.h"
#include "suri/NavigationInputWidget.h"
#include "LibraryItemEditorPart.h"
#include "suri/CoordinatesTransformation.h"

/** namespace suri */
namespace suri {

class DataViewManager;

/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(ChoiceSelectionEvent);


/** Panel de ingreso de coordenadas universal */
/**
 *  Panel de ingreso de coordenadas universal.
 *  Permite ingresar coordenadas en distintos sistemas de coordenadas,
 *  definidos en la biblioteca de sistemas de coordenadas.
 *  Contiene un selector universal de coordenadas, para seleccionar
 *  el sist de referencia
 *  Contiene controles de distinto tipo para el ingreso de coordenadas,
 *  los cuales iran variando dependiendo del sistema de referencia empleado
 *  Tiene un checkbox con la opcion de mostrar cursor, para elegir si
 *  se mostrara el cursor en pantalla.
 *  Tiene 2 botones:
 *  Ir a: Lleva a la posicion de las coordenadas
 *  Aplicar en: Aplica la accion actual en el punto cuyas coord se ingresaron
 */
class UniversalCoordinateInputPart : public LibraryItemEditorPart {
   /** ctor copia **/
   UniversalCoordinateInputPart(const UniversalCoordinateInputPart&);

public:
   /** Nombre del XRC que representa la ventana */
   static const std::string UNIVERSAL_COORDINATE_XRC_NAME;
   static const std::string PIXEL_LINE_SYSTEM_UNIT;
   static const std::string DECIMAL_SYSTEM_UNIT;
   static const std::string SEXAGESIMAL_SYSTEM_UNIT;
   static const std::string FLATS_COORDS_SYSTEM_UNIT;
// Flag para mostrar el selector universal de coord
   static const int SHOW_RS_SELECTOR = 0x01;
// Flag para mostrar las unidades en las que se muestran las coord
// Ej: sexagesimal, decimal
   static const int SHOW_UNIT_SYSTEM_SELECTOR = 0x02;
   // Flag para mostrar dos pares de coordenadas en la seleccion
   static const int SHOW_SUBSET_SYSTEM_SELECTOR = 0x04;

   /** Ctor */
   UniversalCoordinateInputPart(LibraryManager *pManager,
         int flags = SHOW_RS_SELECTOR | SHOW_UNIT_SYSTEM_SELECTOR,
         const UniversalGraphicalComponentPartInterface::
               FeatureCombinationEnum& Feature =
                   UniversalGraphicalComponentPartInterface::ExtendedROWithFF);
   /** Dtor */
   virtual ~UniversalCoordinateInputPart();
#ifdef __UNUSED_CODE__
   /** Permite a clases que hereden modificar el widget luego de crear la ventana */
   virtual bool ConfigureWidget();
#endif
   /**  Metodo que obtiene a partir de los datos del editor, un vector con los atributos
    *   Este metodo es el que hace el comportamiento particular de cada editor a la hora
    *   de obtener la informacion que posee y transmitirla al sistema de bibliotecas.
    *   Es utilizado a modo de Template Method (en CommitChanges y en el notificador)
    *   @return vector con atributos
    *   @return NULL en caso de que la informnacion del part no sea valida
    *   **/
   virtual std::vector<LibraryItemAttribute*>* CreateAttributesFromFields();
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool GetCoordinate(Coordinates &Coordinate);
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool SetCoordinate(Coordinates Coordinate);
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool GetSubset(Subset& OutputSubset);
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool SetSubset(Subset& NewSubset);
   /** Devuelve el sistema de referencia de trabajo */
   std::string GetWorkingSpatialReference() const;
   /** Indica cual el sistema de referencia de trabajo */
   bool SetWorkingSpatialReference(const std::string& WorkingSpatialReference);
   /** Devuelve el sistema de referencia de salida */
   std::string GetOutputSpatialReference() const;
   /** Indica cual el sistema de referencia de salida */
   bool SetOutputSpatialReference(const std::string& OutputSpatialReference);

   /** */
   std::string GetSystemUnit() { return systemUnitSelected_; }

   /** */
   DataViewManager* GetDataViewManager() {
      return pDataViewManager_;
   }

   /** */
   void SetDataViewManager(DataViewManager* pDataViewManager) {
      pDataViewManager_ = pDataViewManager;
   }

protected:
   /**  Metodo auxiliar que actualiza la transformacion de coordenadas*/
   void UpdateCoordinatesTransform();
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
   /** Metodo auxiliar que actualiza el widget correspondiente a el ingreso de coordenadas
    *  que corresponda*/
   virtual bool UpdateCoordinatesInputWidget(bool ReplaceWidget = false);
   /** Almacena los flags de representacion */
   int representationflags_;
   Library* pLibrary_;
   /** Sistema de Referencia de trabajo **/
   std::string workingSpatialReference_;
   /** Sistema de referencia de salida (es el que elige el usuario) **/
   std::string outputSpatialReference_;
   /** Sistema de unidades seleccionado por el usuario en el choice*/
   std::string systemUnitSelected_;
   /** Puntero a la transformacion creada **/
   CoordinatesTransformation* pCoordinateTransform_;
   /** Puntero a la transformacion creada entre los sistemas que va eligiendo el user**/
   CoordinatesTransformation* pOutputCoordinateTransform_;

protected:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(ChoiceSelectionEvent);
   EVENT_OBJECT_PROPERTY(ChoiceSelectionEvent);
   /** Evento que se ejecuta al seleccionar un item en el combobox cambiando el
    * tooltip asociado **/
   void OnChoiceSelectionEventHandler(wxCommandEvent &Event);
   /** Actualiza las opciones del choice en funcion del SRout **/
   bool ConfigureChoice();
   /** Actualiza el NavigationInputWidget correspondiente */
   bool UpdateNavigationInputWidget();

   /** Inicializa los widgets para toma de datos */
   void InitializeInputWidgets();

   /** Referencia a una instancia de NavigationInputWidget **/
   NavigationInputWidget* pNavagationInputWidget_;

   std::map<std::string, NavigationInputWidget*> inputWidgets_;

   /** */
   DataViewManager* pDataViewManager_;
};
}

#endif /* UNIVERSALCOORDINATEINPUTPART_H_ */
