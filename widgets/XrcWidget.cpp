/*! \file XrcWidget.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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

