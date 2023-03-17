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

#include "RasterFeaturesWidget.h"
#include "RasterOffsetConfigurationWidget.h"
#include "suri/AuxiliaryFunctions.h"

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(RasterFeaturesWidgetEvent, RasterFeaturesWidget)
   IMPLEMENT_EVENT_CALLBACK(OnOffsetClicked, OnOffsetClicked(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnOptionsChanged, OnOptionsChanged(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT

const std::string RasterFeaturesWidget::kFeatureBandHeaderOffset = "BandHeaderOffset";
const std::string RasterFeaturesWidget::kFeatureBandTailOffset = "BandTailOffset";
const std::string RasterFeaturesWidget::kFeatureBandCount = "Bandcount";
const std::string RasterFeaturesWidget::kFeatureDataType = "Datatype";
const std::string RasterFeaturesWidget::kFeatureFileHeaderOffset = "FileHeaderOffset";
const std::string RasterFeaturesWidget::kFeatureFileTailOffset = "FileTailOffset";
const std::string RasterFeaturesWidget::kFeatureFormat = "Format";
const std::string RasterFeaturesWidget::kFeatureLineHeaderOffset = "LineHeaderOffset";
const std::string RasterFeaturesWidget::kFeatureLineTailOffset = "LineTailOffset";
const std::string RasterFeaturesWidget::kFeatureMux = "Mux";

/**
 * Ctor.
 */
RasterFeaturesWidget::RasterFeaturesWidget() :
      NEW_EVENT_OBJECT(RasterFeaturesWidgetEvent), pChanged_(NULL), pIOParams_(NULL) {
}

/**
 * Dtor.
 */
RasterFeaturesWidget::~RasterFeaturesWidget() {

}

/**
 * Se crea la ventana de herramienta en las clases derivadas.
 */
bool RasterFeaturesWidget::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_RASTERFEATURES_WIDGET"));

   GET_CONTROL(*pToolWindow_,
         "ID_RASTERFEATURES_OFFSET_BUTTON", wxButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(RasterFeaturesWidgetEvent::OnOffsetClicked), NULL,
         pEventHandler_);

   GET_CONTROL(*pToolWindow_, "ID_RASTERFEATURES_BITS_CHOICE", wxChoice)->Connect(
         wxEVT_COMMAND_CHOICE_SELECTED,
         wxCommandEventHandler(RasterFeaturesWidgetEvent::OnOptionsChanged), NULL,
         pEventHandler_);

   GET_CONTROL(*pToolWindow_, "ID_RASTERFEATURES_INTERLEAVE_CHOICE", wxChoice)->Connect(
         wxEVT_COMMAND_CHOICE_SELECTED,
         wxCommandEventHandler(RasterFeaturesWidgetEvent::OnOptionsChanged), NULL,
         pEventHandler_);

   GET_CONTROL(*pToolWindow_, "ID_RASTERFEATURES_ORDER_CHOICE", wxChoice)->Connect(
         wxEVT_COMMAND_CHOICE_SELECTED,
         wxCommandEventHandler(RasterFeaturesWidgetEvent::OnOptionsChanged), NULL,
         pEventHandler_);

   InitializeValues();

   return true;
}

/**
 * Establece los parametros de entrada y salida.
 */
void RasterFeaturesWidget::SetIOParams(ParameterCollection* pIOParams) {
   pIOParams_ = pIOParams;
}

/**
 * Indica si hubo cambios en el control.
 */
void RasterFeaturesWidget::SetChangedFlag(bool* pChanged) {
   pChanged_ = pChanged;
}

/**
 * Manejador para el evento click del boton offset.
 */
void RasterFeaturesWidget::OnOffsetClicked(wxCommandEvent& Event) {
   RasterOffsetConfigurationWidget::RasterOffsetInfo info = offsetInfo_;
   RasterOffsetConfigurationWidget* poffwidget = new RasterOffsetConfigurationWidget(info);

   if (poffwidget->CreateTool() && poffwidget->GetFrame()->EnableCloseButton()
         && poffwidget->ShowModal() == wxID_OK) {

      if (offsetInfo_ != info) {
         if (pChanged_) {
            *pChanged_ = true;
         }
         offsetInfo_ = info;
         SavaValues();
      }
   }
}

/**
 * Manejador para el evento de cambio de opcion en el
 * choice de tipo de dato.
 */
void RasterFeaturesWidget::OnOptionsChanged(wxCommandEvent& Event) {
   if (pChanged_) {
      *pChanged_ = true;
      SavaValues();
   }
}

/**
 * Inicializa los controles con los datos disponibles.
 */
void RasterFeaturesWidget::InitializeValues() {
   if (pIOParams_ != NULL) {
      std::string buffer;
      if (pIOParams_->GetValue<std::string>(kFeatureDataType, buffer)) {
         GET_CONTROL(*pToolWindow_, "ID_RASTERFEATURES_BITS_CHOICE", wxChoice)
               ->SetStringSelection(buffer.c_str());
      }

      buffer.clear();
      if (pIOParams_->GetValue<std::string>(kFeatureMux, buffer)) {
         GET_CONTROL(*pToolWindow_, "ID_RASTERFEATURES_INTERLEAVE_CHOICE", wxChoice)->SetStringSelection(
               buffer.c_str());
      }

      // Aca se tiene que hacer el seteo del orden de byte. Por el momento
      // no esta implementado en el parseo del los archivos crudos asi
      // que no se usa.

      buffer.clear();
      if (pIOParams_->GetValue<std::string>(kFeatureBandHeaderOffset, buffer)) {
         offsetInfo_.bandOffset_.headerOffset_ = StringToNumber<int>(buffer);
      }

      buffer.clear();
      if (pIOParams_->GetValue<std::string>(kFeatureBandTailOffset, buffer)) {
         offsetInfo_.bandOffset_.tailOffset_ = StringToNumber<int>(buffer);;
      }

      buffer.clear();
      if (pIOParams_->GetValue<std::string>(kFeatureFileHeaderOffset, buffer)) {
         offsetInfo_.fileOffset_.headerOffset_ = StringToNumber<int>(buffer);;
      }

      buffer.clear();
      if (pIOParams_->GetValue<std::string>(kFeatureFileTailOffset, buffer)) {
         offsetInfo_.fileOffset_.tailOffset_ = StringToNumber<int>(buffer);;
      }

      buffer.clear();
      if (pIOParams_->GetValue<std::string>(kFeatureLineHeaderOffset, buffer)) {
         offsetInfo_.LineOffset_.headerOffset_ = StringToNumber<int>(buffer);;
      }

      buffer.clear();
      if (pIOParams_->GetValue<std::string>(kFeatureLineTailOffset, buffer)) {
         offsetInfo_.LineOffset_.tailOffset_ = StringToNumber<int>(buffer);;
      }
   }
}

/**
 * Actualiza los valores de los parametros de salida.
 */
void RasterFeaturesWidget::SavaValues() {
   std::string buffer = XRCCTRL(*(GetWindow()), "ID_RASTERFEATURES_BITS_CHOICE",
                                wxChoice)->GetStringSelection().c_str();
   pIOParams_->AddValue<std::string>(kFeatureDataType, buffer);

   buffer = XRCCTRL(*(GetWindow()), "ID_RASTERFEATURES_INTERLEAVE_CHOICE",
                    wxChoice)->GetStringSelection().c_str();
   pIOParams_->AddValue<std::string>(kFeatureMux, buffer);

   buffer = NumberToString<int>(offsetInfo_.bandOffset_.headerOffset_);
   pIOParams_->AddValue<std::string>(kFeatureBandHeaderOffset, buffer);

   buffer = NumberToString<int>(offsetInfo_.bandOffset_.tailOffset_);
   pIOParams_->AddValue<std::string>(kFeatureBandTailOffset, buffer);

   buffer = NumberToString<int>(offsetInfo_.fileOffset_.headerOffset_);
   pIOParams_->AddValue<std::string>(kFeatureFileHeaderOffset, buffer);

   buffer = NumberToString<int>(offsetInfo_.fileOffset_.tailOffset_);
   pIOParams_->AddValue<std::string>(kFeatureFileTailOffset, buffer);

   buffer = NumberToString<int>(offsetInfo_.LineOffset_.headerOffset_);
   pIOParams_->AddValue<std::string>(kFeatureLineHeaderOffset, buffer);

   buffer = NumberToString<int>(offsetInfo_.LineOffset_.tailOffset_);
   pIOParams_->AddValue<std::string>(kFeatureLineTailOffset, buffer);
}

}  // namespace suri
