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

#include "XrcWidget.h"

// Includes standard
#include <sstream>

// Includes Suri
#include "suri/Part.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"

// Includes App

// Defines

namespace suri {
/**
 * Ctor. Setea el Id del xrc a emplear para representar el widget
 * @param pParentPart part que configurara el Widget creado con XRC.
 * @param XrcId nombre del archivo xrc
 * @param WidgetTitle titulo del widget.
 */
XrcWidget::XrcWidget(Part *pParentPart, const std::string &XrcId,
                     const std::string &WidgetTitle) :
      pParentPart_(pParentPart), xrcId_(XrcId) {
   windowTitle_ = WidgetTitle;
}

/** Dtor */
XrcWidget::~XrcWidget() {
}

/**
 * Implementa la creacion de un panel a partir del ID pasado en el constructor.
 * El panel debe existir como recurso XRC.
 * @return bool true si se realizo una creacion exitosa.
 */
bool XrcWidget::CreateToolWindow() {
   wxPanel *ppanel = wxXmlResource::Get()->LoadPanel(pParentWindow_, xrcId_.c_str());
   pToolWindow_ = ppanel;
   if (pToolWindow_ && pParentPart_) {
      pParentPart_->ConfigureWidget();
   }
   return pToolWindow_ != NULL;
}

/**
 * Busca en los recursos XRC un icono con nombre XrcId+"_ICON", si no lo
 * encuentra, cae en el metodo de la clase padre.
 * @param[out] ToolBitmap Bitmap de la ventana
 * @param[in]  X Ancho en pixels del Bitmap deseado.
 * @param[in]  Y Alto en pixels del Bitmap deseado.
 */
void XrcWidget::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   ToolBitmap = wxXmlResource::Get()->LoadBitmap(
         wxString(xrcId_.c_str()) + wxT("_ICON"));
   if (!ToolBitmap.IsOk()) {
      Widget::GetWindowIcon(ToolBitmap, X, Y);
   }
   // Copy paste del metodo de Widget para darle dimension correcta al
   // bitmap
   if (X > 0 && Y < 0) {
      Y = static_cast<int>((double) ToolBitmap.GetWidth() * X / ToolBitmap.GetHeight());
   }
   if (X < 0 && Y > 0) {
      X = static_cast<int>((double) ToolBitmap.GetHeight() * Y / ToolBitmap.GetWidth());
   }

   if (ToolBitmap.GetWidth() != X || ToolBitmap.GetHeight() != Y) {
      wxImage img = ToolBitmap.ConvertToImage();
      img.Rescale(X, Y, wxIMAGE_QUALITY_HIGH);
      // copio la imagen modificada al bmp (se hace siempre)
      ToolBitmap = wxBitmap(img);
   }
}

/**
 * Retorna el contenido del control como string
 * @param[in] pWidget ventana que contiene al control.
 * @param[in] ControlName nombre del control.
 * @param[out] Value valor del control.
 * @return Retorna el valor del control
 */
bool GetCtrlValue(Widget* pWidget, const std::string &ControlName, std::string &Value) {
   size_t lastseparator = ControlName.rfind("_");
   std::string controltype = "";
   if (lastseparator > 0 && lastseparator < ControlName.size() - 1) controltype =
         ControlName.substr(lastseparator + 1);

   if (controltype.compare("TEXT") == 0) {
      wxTextCtrl *ptextctrl =
            XRCCTRL(*pWidget->GetWindow(), ControlName.c_str(), wxTextCtrl);
      if (!ptextctrl) return false;
      Value = ptextctrl->GetValue().c_str();
   }

   if (controltype.compare("COMBOBOX") == 0) {
      wxComboBox *pcombobox =
            XRCCTRL(*pWidget->GetWindow(), ControlName.c_str(), wxComboBox);
      if (!pcombobox) return false;
      Value = pcombobox->GetValue().c_str();
   }

   return true;
}

/**
 * Retorna el contenido del control como entero
 * @param[in] pWidget ventana que contiene al control.
 * @param[in] ControlName nombre del control.
 * @param[out] Value entero con valor del control.
 * @return Retorna el valor del control.
 */
bool GetCtrlValue(Widget* pWidget, const std::string &ControlName, int &Value) {
   std::string value;
   if (!GetCtrlValue(pWidget, ControlName, value)) return false;

   std::stringstream converter(value);
   converter >> Value;

   return !converter.fail();
}

/**
 * Retorna el contenido del control como double
 * @param[in] pWidget ventana que contiene al control.
 * @param[in] ControlName nombre del control.
 * @param[out] Value double con valor del control.
 * @return Retorna el valor del control.
 */
bool GetCtrlValue(Widget* pWidget, const std::string &ControlName, double &Value) {
   std::string value;
   if (!GetCtrlValue(pWidget, ControlName, value)) return false;

   std::stringstream converter(value);
   converter >> Value;

   return !converter.fail();
}

/**
 * Modifica el contenido del control con string
 * @param[in] pWidget ventana que contiene al control.
 * @param[in] ControlName nombre del control.
 * @param[in] NewValue nuevo valor del control.
 * @return bool que indica si tuvo exito
 */
bool SetCtrlValue(Widget* pWidget, const std::string &ControlName,
                  const std::string &NewValue) {
   size_t lastseparator = ControlName.rfind("_");
   std::string controltype = "";
   if (lastseparator > 0 && lastseparator < ControlName.size() - 1) controltype =
         ControlName.substr(lastseparator + 1);
   std::string returnvalue = "";

   if (controltype.compare("TEXT") == 0) {
      wxTextCtrl *ptextctrl =
            XRCCTRL(*pWidget->GetWindow(), ControlName.c_str(), wxTextCtrl);
      if (ptextctrl == NULL) return false;
      ptextctrl->SetValue(NewValue.c_str());
   }

   if (controltype.compare("COMBOBOX") == 0) {
      wxComboBox *pcombobox =
            XRCCTRL(*pWidget->GetWindow(), ControlName.c_str(), wxComboBox);
      if (pcombobox == NULL) return false;
      pcombobox->SetValue(NewValue.c_str());
   }
   return true;
}

/**
 * Modifica el contenido del control con entero
 * @param[in] pWidget ventana que contiene al control.
 * @param[in] ControlName nombre del control.
 * @param[in] NewValue nuevo valor entero del control .
 * @return bool que indica si tuvo exito
 */
bool SetCtrlValue(Widget* pWidget, const std::string &ControlName, int NewValue) {
   std::stringstream converter;
   converter << NewValue;
   return SetCtrlValue(pWidget, ControlName, converter.str());
}

/**
 * Modifica el contenido del control con double
 * @param[in] pWidget ventana que contiene al control.
 * @param[in] ControlName nombre del control.
 * @param[in] NewValue nuevo valor double del control .
 * @return bool que indica si tuvo exito
 */
bool SetCtrlValue(Widget* pWidget, const std::string &ControlName, double NewValue) {
   std::stringstream converter;
   converter << NewValue;
   return SetCtrlValue(pWidget, ControlName, converter.str());
}

}  // namespace suri

