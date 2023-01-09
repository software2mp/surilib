/*! \file RasterOffsetConfigurationWidget.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "RasterOffsetConfigurationWidget.h"
#include "suri/AuxiliaryFunctions.h"

// Includes Wx
// Defines
// forwards

namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(RasterOffsetConfigurationWidgetHandler,
                             RasterOffsetConfigurationWidget)
   IMPLEMENT_EVENT_CALLBACK(OnCancelClick, OnCancelClick(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnOkClick, OnOkClick(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

RasterOffsetConfigurationWidget::RasterOffsetConfigurationWidget(
      RasterOffsetInfo& OffsetInfo) :
      pEventHandler_(new RasterOffsetConfigurationWidgetHandler(this)),
      offsetInfo_(OffsetInfo) {
}

RasterOffsetConfigurationWidget::~RasterOffsetConfigurationWidget() {
   delete pEventHandler_;
}

/** Se crea la ventana de herramienta en las clases derivadas */
bool RasterOffsetConfigurationWidget::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_RASTER_OFFSET_PANEL"));
   if (pToolWindow_ == NULL)
      return false;
   GET_CONTROL(*pToolWindow_,
         "ID_CANCEL_BTN", wxButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
   wxCommandEventHandler(RasterOffsetConfigurationWidgetHandler::OnCancelClick),
                                                      NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_,
         "ID_OK_BTN", wxButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
   wxCommandEventHandler(RasterOffsetConfigurationWidgetHandler::OnOkClick),
                                                      NULL, pEventHandler_);
   LoadGuiControls();
   return true;
}

/** Metodo que se llama cuando se presiona Cancel **/
void RasterOffsetConfigurationWidget::OnCancelClick(wxCommandEvent& Event) {
   EndModal(wxID_CANCEL);
}

/** Metodo que se llama cuando se presiona Aceptar **/
void RasterOffsetConfigurationWidget::OnOkClick(wxCommandEvent& Event) {
   // configuracion offset linea
   std::string taillineoffset = XRCCTRL(*pToolWindow_,
            "ID_TAIL_LINE_OFFSET", wxTextCtrl)->GetValue().c_str();
   std::string headerlineoffset = XRCCTRL(*pToolWindow_,
               "ID_HEADER_LINE_OFFSET", wxTextCtrl)->GetValue().c_str();
   offsetInfo_.LineOffset_.tailOffset_ = StringToNumber<int>(taillineoffset);
   offsetInfo_.LineOffset_.headerOffset_ = StringToNumber<int>(headerlineoffset);

   // configuracion offset banda
   std::string tailbandoffset = XRCCTRL(*pToolWindow_,
               "ID_TAIL_BAND_OFFSET", wxTextCtrl)->GetValue().c_str();
   std::string headerbandoffset = XRCCTRL(*pToolWindow_,
                     "ID_HEADER_BAND_OFFSET", wxTextCtrl)->GetValue().c_str();
   offsetInfo_.bandOffset_.tailOffset_ = StringToNumber<int>(tailbandoffset);
   offsetInfo_.bandOffset_.headerOffset_ = StringToNumber<int>(headerbandoffset);

   // configuracion offset archivo
   std::string tailfileoffset = XRCCTRL(*pToolWindow_,
                  "ID_TAIL_FILE_OFFSET", wxTextCtrl)->GetValue().c_str();
   std::string headerfileoffset = XRCCTRL(*pToolWindow_,
                  "ID_HEADER_FILE_OFFSET", wxTextCtrl)->GetValue().c_str();
   offsetInfo_.fileOffset_.tailOffset_ = StringToNumber<int>(tailfileoffset);
   offsetInfo_.fileOffset_.headerOffset_ = StringToNumber<int>(headerfileoffset);
   EndModal(wxID_OK);
}

/**
 * Metodo que carga los controles graficos a partir del offset
 *  que se configuro en la clase
 */
void RasterOffsetConfigurationWidget::LoadGuiControls() {
   // configuracion offset linea
   GET_CONTROL(*pToolWindow_,
         "ID_TAIL_LINE_OFFSET", wxTextCtrl)->SetValue(
         NumberToString<int>(offsetInfo_.LineOffset_.tailOffset_).c_str());
   GET_CONTROL(*pToolWindow_,
         "ID_HEADER_LINE_OFFSET", wxTextCtrl)->SetValue(
         NumberToString<int>(offsetInfo_.LineOffset_.headerOffset_).c_str());

   // configuracion offset archivo
   GET_CONTROL(*pToolWindow_,
         "ID_TAIL_FILE_OFFSET", wxTextCtrl)->SetValue(
         NumberToString<int>(offsetInfo_.fileOffset_.tailOffset_).c_str());
   GET_CONTROL(*pToolWindow_,
         "ID_HEADER_FILE_OFFSET", wxTextCtrl)->SetValue(
         NumberToString<int>(offsetInfo_.fileOffset_.headerOffset_).c_str());

   // configuracion offset banda
   GET_CONTROL(*pToolWindow_,
         "ID_TAIL_BAND_OFFSET", wxTextCtrl)->SetValue(
         NumberToString<int>(offsetInfo_.bandOffset_.tailOffset_).c_str());
   GET_CONTROL(*pToolWindow_,
         "ID_HEADER_BAND_OFFSET", wxTextCtrl)->SetValue(
         NumberToString<int>(offsetInfo_.bandOffset_.headerOffset_).c_str());
}
} /** namespace suri */
