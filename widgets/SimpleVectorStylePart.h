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

#ifndef SIMPLEVECTORSTYLEPART_H_
#define SIMPLEVECTORSTYLEPART_H_

// Includes Estandar
// Includes Suri
#include "LibraryItemEditorPart.h"
#include "VectorRenderer.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class VectorLayer;
class VectorStyle;

namespace ui {

// forwards
class SimpleVectorStylePartEvent;

/**
 * Clase encargada de la configuracion de estilos simples
 * (un estilo que se aplica a toda la capa vectorial).
 * Tiene los controles:
 * Simbolo: Tipo, Tamano
 * Linea: Tipo, Ancho
 * Relleno: Tipo, Color
 * \note extiende Part para guardar los cambios en propiedades del vector
 */
   /** ctor copia **/
class SimpleVectorStylePart : public LibraryItemEditorPart {
   SimpleVectorStylePart(const SimpleVectorStylePart&);

public:
   /** Constructor por defecto de la clase **/
   SimpleVectorStylePart(
         const SuriObject::UuidType& LibraryId,
         LibraryManager* pManager,
         VectorLayer *pLayer = NULL,
         const UniversalGraphicalComponentPartInterface::FeatureCombinationEnum& Feature =
               UniversalGraphicalComponentPartInterface::EditionWithFavorites);
   explicit SimpleVectorStylePart(std::string* pWkt,LibraryManager* pManager=NULL,const SuriObject::UuidType& LibraryId=SuriObject::NullUuid);

   /** dtor **/
   virtual ~SimpleVectorStylePart();
   /** Inicializa los controles */
   virtual void SetInitialValues();

   /** Actualiza el VectorStyle a partir del elemento(pElement). */
   void Refresh();
   // ------------------------------------------------------------------------------
   /** Indica si el Part tiene cambios para salvar. */
   /**
    * Para cada tipo de propiedad verifica si cambiaron sus valores
    * @return true si se modifican las propiedades del Elemento.
    * @return false si no se modifican las propiedades del Elemento.
    */
   bool HasChanged();
   /** Configura el estilo vectorial que mostrara el editor desde codigo cliente **/
   void SetVectorStyleWkt(const std::string& Wkt);
   /** Retorna el puntero al estilo del vector */
   suri::VectorStyle* GetVectorStyle();

private:
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
   /** Evento UIUpdate */
   void OnUIUpdate(wxUpdateUIEvent &Event);
   /** Metodo que actualiza el estilo que se encuentra configurado */
   void UpdateStyleControls();
   /** Metodo que se llama cuando se clickea sobre el check para utilizar la biblioteca **/
   void OnLibraryCheckClick(wxCommandEvent& Event);
   /** Realiza los cambios en la linea del elemento */
   bool SetPen();
   /** Realiza los cambios en la superficie del elemento */
   bool SetBrush();
   /** Realiza los cambios en el(los) punto del elemento */
   bool SetSymbol();
   /** Carga los controles(combobox) */
   void LoadControls();
   /** atributos de la instancia **/
   EVENT_OBJECT_PROPERTY(SimpleVectorStylePartEvent);
   friend class SimpleVectorStylePartEvent;
   VectorLayer *pLayer_;
   /*! indica si hay que actualizar el preview */
   bool updatePreview_;
   /*! Vector Style */
   suri::VectorStyle *pVecStyle_ ;
   bool libraryCheck_;
   std::string libraryId_;
   /** string que almacena el wkt del ultimo estilo configurado **/
   std::string currentWkt_;
   /** puntero que tiene el wkt actual **/
   std::string* pWkt_;
};

} /** namespace ui **/
} /** namespace suri */

#endif /* SIMPLEVECTORSTYLEPART_H_ */
