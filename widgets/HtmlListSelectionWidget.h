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

#ifndef ITEMSELECTIONWIDGET_H_
#define ITEMSELECTIONWIDGET_H_

// Includes Estandar
#include <list>
#include <set>
#include <string>

// Includes Suri
#include "suri/Widget.h"
#include "suri/SuriObject.h"
#include "suri/Subject.h"

// Includes Wx
// Defines
// forwards
class wxSimpleHtmlListBox;
class wxHtmlLinkEvent;

namespace suri {
namespace ui {

class HtmlItemSelectionWidgetEvent;
class HtmlListItemInterface;
/**
 * Clase que representa un control grafico para seleccion de items en forma de arbol. Este
 * control grafico posee un icono asociado al item y la posiblidad de incorporar
 * un checkbox al mismo.
 **/
class HtmlListSelectionWidget : public Widget, public Subject {
public:
   /** Flag para layout **/
   static const long Checkeable = 1;
   static const long MultipleSelection = 2;
   /** ctor que se inicialza a partir de una lista de items **/
   explicit HtmlListSelectionWidget(const std::list<HtmlListItemInterface*>& Items,
                                    long flags = Checkeable | MultipleSelection);
   /** ctor sin items cargados previamente **/
   explicit HtmlListSelectionWidget(long flags = Checkeable | MultipleSelection);
   /** dtor **/
   virtual ~HtmlListSelectionWidget();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
   /** Agrega un item al final de la lista **/
   void AppendItem(HtmlListItemInterface* pItem);
   /** Inserta un item en la posocion deseada **/
   void InsertItem(HtmlListItemInterface* pItem, int position);
   /** Elimina el item que coincide con el id que se pasa por parametro */
   bool DeleteItem(const SuriObject::UuidType& ItemId);
   /** Devuelve los items seleccionados por el usuario del widget **/
   std::set<SuriObject::UuidType> GetSelectedItems();
   /** Selecciona todos los items de la lista **/
   void SelectAll();
   /** Metodo que elimina la seleccion actual**/
   void ClearSelection();
   /** Obtiene el item asociado al id **/
   HtmlListItemInterface* GetItem(const SuriObject::UuidType& ItemId);

private:
   friend class HtmlItemSelectionWidgetEvent;
   HtmlItemSelectionWidgetEvent* pEventHandler_;
   /** Se ejecuta cuando se hace click sobre link de un nodo */
   void OnLink(wxHtmlLinkEvent &Event);
   /** Se ejecuta cuando se hace doble click sobre un item de la lista */
   void OnDoubleClick(wxMouseEvent &Event);
   /** Atrapa evento disparado cuando se modifica la seleccion */
   void OnSelectionChanged(wxCommandEvent &Event);
   /** Metodo auxiliar que invierte el estado de un item de la lista **/
   void InvertItemState(int Position);
   /** Metodo auxiliar que convierte el string pasado por parametro en un
    *  enconding valido. Esto es importante para los acentos.
    * En linux modifica el string correspondiente al HTML para convertirlo
    *  a UTF8 y asi mostrar los caracteres no-ASCII 127 en forma correcta.
    */
   std::string GetCompatibleEncodingItemString(const std::string& ItemString);
   /** Control grafico en forma de arbol que es posible decorarlo con html **/
   wxSimpleHtmlListBox* pTreeCtrl_;
   /** Lista de items que posee la lista **/
   std::list<HtmlListItemInterface*> items_;
   /** Seleccion activa **/
   std::set<SuriObject::UuidType> selectedItems_;
   /**
    *  Flag de layout de los items y la lista (ej. si son checkeables, si
    *  se permite la multiple seleccion).
    */
   long layoutFlags_;
};

/** Interfaz que define el comportamiento de una clase encargada de convertir el
 * contenido del item a un string en formato html. **/
class HtmlListItemStringConverterInterface {
   /** ctor copia **/
   HtmlListItemStringConverterInterface(const HtmlListItemStringConverterInterface&);

public:
   static const long Checkeable = 1;
   explicit HtmlListItemStringConverterInterface(long LayoutFlags = Checkeable) :
         layoutFlags_(LayoutFlags) {
   }
   /** dtor */
   virtual ~HtmlListItemStringConverterInterface() {}
   /**
    * A partir del item pasado por parametro genera un string html
    * con el contenido del mismo.
    * @param[in] pItem item a partir del cual se generara el string html a
    * insertar en la lista
    * @param[out] ItemHtmlString string generado a partir del item
    **/
   virtual bool GetItemAsString(const HtmlListItemInterface* pItem,
                        std::string& ItemHtmlString) const=0;

protected:
   /** Flag para la generacion del string html **/
   long layoutFlags_;
};

/** Interfaz que define el comportamiento de un item de la lista. **/
class HtmlListItemInterface {
   /** ctor copia **/
   HtmlListItemInterface(const HtmlListItemInterface&);

public:
   /** constructor **/
   HtmlListItemInterface() :
         itemId_(SuriObject::NullUuid), active_(false) {
   }
   /** Destructor **/
   virtual ~HtmlListItemInterface() {}
   /** Metodo que compara el elemento pasado por parametro.
    * Devuelve true si es igual al elemento de la instancia
    * sobre la cual se invoca el mismo.
    * @param[in] Item item a comparar
    * @return true en caso de que los items sean iguales. (generalmente
    * se comparan los ids de los mismos)
    * @return false en caso contrario. **/
   virtual bool Equals(const HtmlListItemInterface& Item) const=0;
   /** Operador de comparacion
    * @param[in] Item item a comparar
    * @return true en caso de los items sean iguales (a traves del metodo
    * polimorfico Equals).
    * @return false en caso contrario
    **/
   bool operator==(const HtmlListItemInterface& Item) {
      return this->Equals(Item);
   }
   /** Metodo que marca al item como activo. **/
   virtual void Active()=0;
   /** Metodo que marca al item como desactivo. **/
   virtual void Deactive()=0;
   /** Metodo  que consulta si un istem se encuentra activo. **/
   bool IsActive() const {
      return active_;
   }

   /** Obtiene el id asociado al item **/
   SuriObject::UuidType GetId() const {
      return itemId_;
   }

   /**
    * A partir del item pasado por parametro genera un string html
    * con el contenido del mismo.
    * @param[in] LayoutFlags configuracion del layout deseado
    * @return std::string string que representa el html asociado al item
    **/
   virtual std::string GetItemAsString(long LayoutFlags =
         HtmlListItemStringConverterInterface::Checkeable) const=0;

protected:
   /** Id asociado al item **/
   SuriObject::UuidType itemId_;
   /** boolean que indica si el item se encuentra activo (activado/checkeado)**/
   bool active_;
};

} /** namespace ui */
} /** namespace suri */

#endif /* ITEMSELECTIONWIDGET_H_ */
