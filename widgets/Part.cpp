/*! \file Part.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
