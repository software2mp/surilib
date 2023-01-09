/*! \file RasterGenericImporterWidget.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>
#include <iostream>
#include <fstream>
#include <map>

// Includes Suri
#include "RasterGenericImporterWidget.h"
#include "GenericRasterHeaderDescriptionFile.h"
#include "suri/FileManagementFunctions.h"
#include "suri/AuxiliaryFunctions.h"
#include "RasterOffsetConfigurationWidget.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/DataTypes.h"
#include "suri/messages.h"

// Includes Wx
#include "wx/filepicker.h"

// Defines
#define UNSIGNED_CHAR_TYPE "unsigned char"
#define CHAR_TYPE "char"
#define USHORT_TYPE "unsigned short"
#define SHORT_TYPE "short"
#define UINT_TYPE "unsigned int"
#define INT_TYPE "int"
#define ULONG_TYPE "unsigned long"
#define LONG_TYPE "long"
#define FLOAT_TYPE "float"
#define DOUBLE_TYPE "double"

#define UNSIGNED_CHAR_NAME "uchar"
#define CHAR_NAME "char"
#define USHORT_NAME "ushort"
#define SHORT_NAME "short"
#define UINT_NAME "uint"
#define INT_NAME "int"
#define ULONG_NAME "ulong"
#define LONG_NAME "long"
#define FLOAT_NAME "float"
#define DOUBLE_NAME "double"

// forwards

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(RasterGenericImporterWidgetHandler, RasterGenericImporterWidget)
   IMPLEMENT_EVENT_CALLBACK(OnFileSelectionChange, OnFileSelectionChange(event),
                            wxFileDirPickerEvent)
   IMPLEMENT_EVENT_CALLBACK(OnCancelClick, OnCancelClick(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnOkClick, OnOkClick(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnFieldUpdate, OnFieldUpdate(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnOffsetClick, OnOffsetClick(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

RasterGenericImporterWidget::RasterGenericImporterWidget(
      DatasourceManagerInterface* pDsManager) :
      pEventHandler_(new RasterGenericImporterWidgetHandler(this)),
      pDsManager_(pDsManager) {
	windowTitle_ = _(caption_RASTER_IMPORTER);
}

RasterGenericImporterWidget::~RasterGenericImporterWidget() {
   delete pEventHandler_;
}

/** Se crea la ventana de herramienta en las clases derivadas */
bool RasterGenericImporterWidget::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_RASTERGENERICIMPORT"));
   if (pToolWindow_ == NULL)
      return false;
   ConfigureControls();
   std::string types[] = { UNSIGNED_CHAR_TYPE, CHAR_TYPE, USHORT_TYPE, SHORT_TYPE,
                             UINT_TYPE, INT_TYPE, ULONG_TYPE, LONG_TYPE, FLOAT_TYPE,
                             DOUBLE_TYPE};
   std::string names[] = { UNSIGNED_CHAR_NAME, CHAR_NAME, USHORT_NAME, SHORT_NAME,
                             UINT_NAME, INT_NAME, ULONG_NAME, LONG_NAME, FLOAT_NAME,
                             DOUBLE_NAME};
   for (int k = 0; k < 10; ++k) {
      dataTypeMap_.insert(std::make_pair(types[k], names[k]));
   }
   return true;
}

/** Metodo que se llama cuando se selecciona un nuevo archivo **/
void RasterGenericImporterWidget::OnFileSelectionChange(wxFileDirPickerEvent &Event) {
   wxFilePickerCtrl* pfilepicker =
         XRCCTRL(*(GetWindow()), "ID_RASTER_FILE_PICKER", wxFilePickerCtrl);
   unsigned long size = GetFileSize(pfilepicker->GetPath().c_str());
   GET_CONTROL(*(GetWindow()),
         "ID_SIZE_LABEL", wxStaticText)->SetLabel(NumberToString<unsigned long>(size));
}

/**
 * Metodo auxiliar que realiza una configuracion sobre los controles graficos
 * del widget
 */
void RasterGenericImporterWidget::ConfigureControls() {
   GET_CONTROL(*(GetWindow()),
         "ID_RASTER_FILE_PICKER", wxFilePickerCtrl)->Connect(
         wxEVT_COMMAND_FILEPICKER_CHANGED,
         wxFileDirPickerEventHandler(RasterGenericImporterWidgetHandler::OnFileSelectionChange),
         NULL,
         pEventHandler_);
   GET_CONTROL(*pToolWindow_,
         "ID_CANCEL_BTN", wxButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
   wxCommandEventHandler(RasterGenericImporterWidgetHandler::OnCancelClick),
                                             NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_,
         "ID_OK_BTN", wxButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
   wxCommandEventHandler(RasterGenericImporterWidgetHandler::OnOkClick),
                                         NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_,
         "ID_OFFSET_BTN", wxButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
   wxCommandEventHandler(RasterGenericImporterWidgetHandler::OnOffsetClick),
                                             NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_,
         "ID_BAND_COUNT_TEXT", wxTextCtrl)->Connect(wxEVT_COMMAND_TEXT_UPDATED,
   wxCommandEventHandler(RasterGenericImporterWidgetHandler::OnFieldUpdate),
                                                    NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_,
         "ID_LINES_TEXT", wxTextCtrl)->Connect(wxEVT_COMMAND_TEXT_UPDATED,
   wxCommandEventHandler(RasterGenericImporterWidgetHandler::OnFieldUpdate),
                                               NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_,
         "ID_PIXEL_TEXT", wxTextCtrl)->Connect(wxEVT_COMMAND_TEXT_UPDATED,
   wxCommandEventHandler(RasterGenericImporterWidgetHandler::OnFieldUpdate),
                                               NULL, pEventHandler_);

   XRCCTRL(*(GetWindow()), "ID_NBITS_CHOICE", wxChoice)->Connect(
         wxEVT_COMMAND_CHOICE_SELECTED,
         wxCommandEventHandler(RasterGenericImporterWidgetHandler::OnFieldUpdate),
         NULL, pEventHandler_);

   wxTextValidator validator(wxFILTER_NUMERIC);
   GET_CONTROL(*(GetWindow()),
         "ID_BAND_COUNT_TEXT", wxTextCtrl)->SetValidator(validator);
   GET_CONTROL(*(GetWindow()),
         "ID_LINES_TEXT", wxTextCtrl)->SetValidator(validator);
   GET_CONTROL(*(GetWindow()),
         "ID_PIXEL_TEXT", wxTextCtrl)->SetValidator(validator);
}

/** Metodo que se llama cuando se presiona Cancel **/
void RasterGenericImporterWidget::OnCancelClick(wxCommandEvent& Event) {
   EndModal(wxID_CANCEL);
}
/** Metodo que se llama cuando se presiona Aceptar **/
void RasterGenericImporterWidget::OnOkClick(wxCommandEvent& Event) {
   wxFilePickerCtrl* pfilepicker = XRCCTRL(*(GetWindow()), "ID_RASTER_FILE_PICKER",
                                           wxFilePickerCtrl);
   std::string file = pfilepicker->GetPath().c_str();
   std::string sizestr = XRCCTRL(*(GetWindow()),
            "ID_SIZE_LABEL", wxStaticText)->GetLabel().c_str();
   std::string reqsizestr =
         XRCCTRL(*(GetWindow()), "ID_REQ_SIZE_LABEL", wxStaticText)->GetLabel().c_str();

   if (StringToNumber<int>(reqsizestr) == 0 || file.empty() ||
         StringToNumber<int>(reqsizestr) > StringToNumber<int>(sizestr)) {
      SHOW_ERROR("No es posible realizar la importacion, por favor verifique la configuracion");
      return;
   }

   Option metadata;
   GetMetadataOptions(metadata);
   // TODO: Configurar los metadatos para que se pueda configurar la interpretacion
   // de los archivos de forma correcta
   DatasourceInterface* pdatasource = DatasourceInterface::Create("RasterDatasource",
                                                                     file.c_str(),
                                                                     metadata);

   if (pdatasource != NULL && pDsManager_->AddDatasource(pdatasource)) {
      EndModal(wxID_OK);
      return;
   }
   SHOW_ERROR("No es posible realizar la importacion, por favor verifique la configuracion");
   return;
}

/** Metodo que se llama cuando se presiona el boton de offset **/
void RasterGenericImporterWidget::OnOffsetClick(wxCommandEvent& Event) {
   RasterOffsetConfigurationWidget::RasterOffsetInfo info = offsetInfo_;
   RasterOffsetConfigurationWidget* poffwidget = new RasterOffsetConfigurationWidget(
         info);
   if (poffwidget->CreateTool() && poffwidget->GetFrame()->EnableCloseButton()
         && poffwidget->ShowModal() == wxID_OK) {
      offsetInfo_ = info;
      UpdateSizeReq();
   }
}

/**
 * Metodo auxiliar que obtiene la informacion de metainformacion
 * que configura el usuario
 */
void RasterGenericImporterWidget::GetMetadataOptions(Option& Metadata) {
   Metadata.SetOption("Format", "raw");

   Metadata.SetOption(
         "Mux",
         XRCCTRL(*(GetWindow()), "ID_LAYOUT_CHOICE", wxChoice)->GetStringSelection().c_str());
   Metadata.SetOption(
         "Bandcount",
         XRCCTRL(*(GetWindow()), "ID_BAND_COUNT_TEXT", wxTextCtrl)->GetValue().c_str());
   Metadata.SetOption(
         "Pixels",
         XRCCTRL(*(GetWindow()), "ID_PIXEL_TEXT", wxTextCtrl)->GetValue().c_str());
   Metadata.SetOption(
         "Lines",
         XRCCTRL(*(GetWindow()), "ID_LINES_TEXT", wxTextCtrl)->GetValue().c_str());

   std::string sel =
         XRCCTRL(*(GetWindow()), "ID_NBITS_CHOICE", wxChoice)->GetStringSelection().c_str();
   Metadata.SetOption("Datatype", dataTypeMap_[sel]);

   Metadata.SetOption("LineHeaderOffset",
                      NumberToString<int>(offsetInfo_.LineOffset_.headerOffset_));
   Metadata.SetOption("LineTailOffset", NumberToString<int>(offsetInfo_.LineOffset_.tailOffset_));
   Metadata.SetOption("FileHeaderOffset",
                      NumberToString<int>(offsetInfo_.fileOffset_.headerOffset_));
   Metadata.SetOption("FileTailOffset", NumberToString<int>(offsetInfo_.fileOffset_.tailOffset_));
   Metadata.SetOption("BandHeaderOffset",
                      NumberToString<int>(offsetInfo_.bandOffset_.headerOffset_));
   Metadata.SetOption("BandTailOffset", NumberToString<int>(offsetInfo_.bandOffset_.tailOffset_));

}

/** Metodo que se llama cuando se actualiza algun textctrl del widget **/
void RasterGenericImporterWidget::OnFieldUpdate(wxCommandEvent& Event) {
   UpdateSizeReq();
}

/**
 *  Metodo auxiliar que actualiza el size requerido de la imagen en funcion de
 *  la configuracion indicada.
 */
void RasterGenericImporterWidget::UpdateSizeReq() {
   std::string bcount =
         XRCCTRL(*(GetWindow()), "ID_BAND_COUNT_TEXT", wxTextCtrl)->GetValue().c_str();
   std::string pixels =
         XRCCTRL(*(GetWindow()), "ID_PIXEL_TEXT", wxTextCtrl)->GetValue().c_str();
   std::string lines =
         XRCCTRL(*(GetWindow()), "ID_LINES_TEXT", wxTextCtrl)->GetValue().c_str();
   std::string sel =
           XRCCTRL(*(GetWindow()), "ID_NBITS_CHOICE", wxChoice)->GetStringSelection().c_str();

   unsigned long reqsize = StringToNumber<unsigned long>(bcount) * StringToNumber<unsigned long>(lines)
         * StringToNumber<long>(pixels) * SizeOf(dataTypeMap_[sel]);
   reqsize += offsetInfo_.LineOffset_.headerOffset_ + offsetInfo_.LineOffset_.tailOffset_;
   reqsize += offsetInfo_.bandOffset_.headerOffset_ + offsetInfo_.bandOffset_.tailOffset_;
   reqsize += offsetInfo_.fileOffset_.headerOffset_ + offsetInfo_.fileOffset_.tailOffset_;
   XRCCTRL(*(GetWindow()), "ID_REQ_SIZE_LABEL", wxStaticText)->SetLabel(
         NumberToString<unsigned long>(reqsize));
}
} /** namespace suri */
