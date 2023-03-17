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

// Includes suri
#include "suri/Part.h"
#include "suri/messages.h"
#include "XrcWidget.h"

// Includes wx
#include "wx/xrc/xmlres.h"
#include "wx/wx.h"

/** namespace suri */
namespace suri {
/**
 * Constructor
 * @param[in] Enable bool que indica si el part esta habilitado
 * @param[in] Modified bool que indica si el part esta modificado
 */
Part::Part(const bool &Enable, const bool &Modified) :
      Widget(NULL), enabled_(Enable), modified_(Modified), errorMessage_(wxEmptyString),
      pWidget_(NULL) {
   windowTitle_ = _(caption_PART);
}

/**
 * Constructor
 * @param[in] pWidget Widget asociado al Part.
 * @param[in] Enable bool que indica si el part esta habilitado
 * @param[in] Modified bool que indica si el part esta modificado
 */
Part::Part(Widget* pWidget, const bool &Enable, const bool &Modified) :
      Widget(NULL), enabled_(Enable), modified_(Modified), errorMessage_(wxEmptyString),
      pWidget_(pWidget) {
}

/**
 * Constructor que genera pWidget_ usando XrcId.
 * @param[in] XrcId string que indica el id del xrc asociado al part
 * @param[in] Enable bool que indica si el part esta habilitado
 * @param[in] Modified bool que indica si el part esta modificado
 */
Part::Part(const std::string &XrcId, const std::string &PartName, const bool &Enable,
           const bool &Modified) :
      enabled_(Enable), modified_(Modified), pWidget_(NULL) {
   std::string partname = PartName;
   if (partname.empty()) {
      partname = caption_PART;
   }
   XrcWidget *pxrcwidget = new XrcWidget(this, XrcId, partname);
   pWidget_ = pxrcwidget;
}

/**
 * Dtor.
 */
Part::~Part() {
}

/**
 * Setea el padre del Part. Esto permite crear un Part
 * sin tener la necesidad de obtener previamente la ventana padre.
 * @param[in] pParentWindow ventana padre
 * @return bool que indica si pudo crear la ventana
 */
bool Part::CreatePartTool(wxWindow *pParentWindow) {
   GetWidget()->pParentWindow_ = pParentWindow;
   return true;
}

/**
 * Indica si el part esta o no habilitado
 * @return bool que indica si el part esta habilitado.
 */
bool Part::IsEnabled() {
   return enabled_;
}

/**
 * Indica si los datos que contiene el part tienen cambios
 * @return bool que indica si hay cambios
 */
bool Part::HasChanged() {
   return modified_;
}

/**
 * Salva los cambios realizados en el Part.
 * @return bool que indica si tuvo exito (true siempre)
 */
bool Part::CommitChanges() {
   return true;
}

/**
 * Modifica el estado de enabled_, para habilitar o deshabilitar la parte
 * @param[in] Enabled : booleano que des/habilita la parte
 */
void Part::SetEnabled(bool Enabled) {
   enabled_ = Enabled;
}

/**
 * Devuelve si la parte contiene datos validos.
 * Devuelve true : siempre.
 */
bool Part::HasValidData() {
   return true;
}

/**
 * Objetivo:	Operador ==
 * @param[in]	Other: Part.
 * @return this==Other? true:false.
 */
bool Part::operator==(const Part &Other) {
   return (windowTitle_.Cmp(Other.windowTitle_) == 0)
         && (isAutonomous_ == Other.isAutonomous_) && (enabled_ == Other.enabled_)
         && (modified_ == Other.modified_);
}

/**
 * Obtiene el Widget asociado.
 * @return suri::Widget widget asociado al Part.
 */
Widget* Part::GetWidget() {
   if (pWidget_) {
      return pWidget_;
   } else {
      // Esta linea permite que PartCollection agregue parts que
      // implementan CreateToolWindow.
      return this;
   }
}

/**
 * Metodo heredado de la interfaz de widget
 * \deprecated: usar el widget interno, evitar
 * el uso de metodos de widget
 */
bool Part::CreateToolWindow() {
   return false;
}
}  // namespace suri
