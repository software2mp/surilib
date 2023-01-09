/** \file HtmlReportWidget.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
