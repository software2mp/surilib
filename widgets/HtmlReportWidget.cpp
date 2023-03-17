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

// Includes standard
#include <string>
#include <map>

// Includes suri
#include "HtmlReportWidget.h"
#include "suri/messages.h"
#include "TextFormatConverterInterface.h"
#include "resources.h"
#include "suri/HtmlViewerPart.h"
#include "suri/ReportInterface.h"

// Includes wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"
#include "wx/file.h"

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(HtmlReportWidgetEventHandler, HtmlReportWidget)
      IMPLEMENT_EVENT_CALLBACK(OnSaveToDiskButton, OnSaveToDiskButton(), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** Ctor */
HtmlReportWidget::HtmlReportWidget(const std::string &WindowTitle,
                                    const std::string &OutputFormat) :
                                    outputFormat_(OutputFormat),
                                    NEW_EVENT_OBJECT(HtmlReportWidgetEventHandler) {
   windowTitle_ = WindowTitle;
}

/** Dtor */
HtmlReportWidget::~HtmlReportWidget() {
}

/**
 * Crea la ventana
 * @return flag que indica si pudo crear la ventana.
 */
bool HtmlReportWidget::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_HTML_REPORT_PANEL"));
   if (!pToolWindow_)
      return false;

   HtmlViewerPart* phtmlviewerpart = new HtmlViewerPart();
   SetHtmlViewer(phtmlviewerpart);

   AddControl(pHtmlViewerPart_, "ID_REPORT_HTML_PANEL");

   XRCCTRL(*pToolWindow_, wxT("ID_EXPORTHTML_BUTTON"), wxBitmapButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(HtmlReportWidgetEventHandler::OnSaveToDiskButton),
         NULL, pEventHandler_);
   pHtmlViewerPart_->LoadHtml(pReport_->GetContentAsText(outputFormat_).c_str());
   return true;
}

/**
 *  Retorna un bitmap con el icono de la ventana.
 *  @param[out] ToolBitmap icono de la ventana
 *  @param[in] X ancho del icono
 *  @param[in] Y alto del icono
 */
void HtmlReportWidget::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(wxT(icon_FRAME), ToolBitmap);
}

/**
 * Obtiene widget que despliega html
 * @return HtmlViewerPart* desplegado
 */
HtmlViewerPart* HtmlReportWidget::GetHtmlViewer() const {
   return pHtmlViewerPart_;
}

/**
 * Configura widget que despliega html
 * @param[in] pHtmlViewerPart widget que se quiere desplegar
 */
void HtmlReportWidget::SetHtmlViewer(HtmlViewerPart *pHtmlViewerPart) {
   pHtmlViewerPart_ = pHtmlViewerPart;
}

/**
 * Configura el reporte que a desplegar
 * @param[in] pReport reporte a desplegar
 */
void HtmlReportWidget::SetReport(ReportInterface* pReport) {
   pReport_ = pReport;
}

/** Evento de guardar a disco */
void HtmlReportWidget::OnSaveToDiskButton() {
   wxFileDialog dlg(pToolWindow_, _(caption_SAVE_REPORT), wxT(""), wxT(""),
   filter_SAVE_STATISTICS,
                    wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

   if (dlg.ShowModal() == wxID_OK) {
      wxFile wxfile;
      if (!wxfile.Open(dlg.GetPath(), wxFile::write)) return;

      wxfile.Write(pReport_->GetContentAsText(outputFormat_).c_str());
   }
}
/** Configura el titulo de la ventana **/
void HtmlReportWidget::SetWindowTitle(const std::string& Title) {
   windowTitle_ = Title;
}
}  // namespace suri
