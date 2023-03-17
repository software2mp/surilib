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

// Includes Estandar
#include <vector>
#include <string>

// Includes Suri
#include "ThresholdClassDefinitionWidget.h"
#include "TablePart.h"
#include "DefaultTable.h"
#include "suri/MemoryDriver.h"
#include "TablePartConfigurator.h"
#include "suri/RasterElement.h"
#include "BandInfo.h"
#include "TablePartConfigurator.h"
#include "suri/AuxiliaryFunctions.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ThresholdClassDefinitionEventHandler,
                             ThresholdClassDefinitionWidget)
   IMPLEMENT_EVENT_CALLBACK(OnCancelClick, OnCancelClick(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnOkClick, OnOkClick(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */


ThresholdClassDefinitionWidget::ThresholdClassDefinitionWidget(
      DataViewManager* pDataView, Element* pInputElement,
      ThresholdClassInformation& ClassInfo) :
      pDataView_(pDataView), pInputElement_(pInputElement),
      pEventHandler_(new ThresholdClassDefinitionEventHandler(this)),
      currentClassInfo_(ClassInfo) {
   windowTitle_ = _(caption_THRESHOLD_DEFINITION);
}

ThresholdClassDefinitionWidget::~ThresholdClassDefinitionWidget() {
   delete pEventHandler_;
}

/**
 *  Metodo privado que inicializa la tabla que corresponde a la definicion
 *  de umbrales para una clase
 *  @return true en caso de que pueda configurarse la tabla
 *  @return false en caso contrario
 */
bool ThresholdClassDefinitionWidget::InitializeTable() {
   RasterElement* praster = dynamic_cast<RasterElement*>(pInputElement_);
   if (!praster)
      return false;
   pTable_ = new DefaultTable();
   MemoryDriver* pdriver = new MemoryDriver();
   pTable_->SetDriver(pdriver);
   pTable_->AppendColumn("Bandas", Table::STRING);
   pTable_->AppendColumn("Min", Table::FLOAT);
   pTable_->AppendColumn("Max", Table::FLOAT);

   TablePartConfigurator config;
   Option options;
   options.SetOption(TablePartConfigurator::TableEditorKeyOption,
                     TablePartConfigurator::VectorTableEditorOption);
   options.SetOption(TablePartConfigurator::RoColumnsKeyOption, "Bandas");
   pTablePart_ = config.Initialize(pDataView_, pTable_, options);
   // si la clase no posee informacion de bandas => la completo
   if (currentClassInfo_.bandsThreshold_.empty()) {
      for (int b = 0; b < praster->GetBandCount(); ++b) {
         ThresholdBandInformation bandinfo;
         praster->GetBandName(bandinfo.bandname_, b);
         int valid = 1;
         praster->GetBandValidity(valid, b);
         if (valid != 0) {
            bandinfo.bandindex_ = b;
            bandinfo.min_ = 0.0;
            bandinfo.max_ = 0.0;
            currentClassInfo_.bandsThreshold_.push_back(bandinfo);
         }
      }
   }

   for (size_t r = 0; r < currentClassInfo_.bandsThreshold_.size(); ++r) {
      pTable_->AppendRow();
      ThresholdBandInformation bandinfo = currentClassInfo_.bandsThreshold_.at(r);
      pTable_->SetCellValue(0, r, bandinfo.bandname_);
      std::string min = DoubleToString(bandinfo.min_);
      pTable_->SetCellValue(1, r, min);
      std::string max = DoubleToString(bandinfo.max_);
      pTable_->SetCellValue(2, r, max);
   }
   return true;
}
/** Metodo que se llama cuando se presiona Cancel **/
void ThresholdClassDefinitionWidget::OnCancelClick(wxCommandEvent& Event) {
   EndModal(wxID_CANCEL);
}
/** Metodo que se llama cuando se presiona Aceptar **/
void ThresholdClassDefinitionWidget::OnOkClick(wxCommandEvent& Event) {
   if (!HasValidClassDefinition()) {
      SHOW_ERROR(_(message_INVALID_THRESHOLD_DEF));
      return;
   }
   wxTextCtrl* pcnamectrl = XRCCTRL(*pToolWindow_, "ID_CLASS_NAME", wxTextCtrl);
   currentClassInfo_.classname_ = pcnamectrl->GetValue().c_str();
   currentClassInfo_.bandsThreshold_.clear();
   RasterElement* praster = dynamic_cast<RasterElement*>(pInputElement_);
   if (!praster)
      return;
   for (int r = 0; r < pTable_->GetRows(); ++r) {
      ThresholdBandInformation bandinfo;
      pTable_->GetCellValue(0, r, bandinfo.bandname_);
      int bandindex = -1;
      for (int b = 0; b < praster->GetBandCount(); ++b) {
         std::string bname;
         praster->GetBandName(bname, b);
         if (bname.compare(bandinfo.bandname_) == 0) {
            bandindex = b;
            break;
         }
      }
      if (bandindex < 0)
         break;

      bandinfo.bandindex_ = bandindex;

      std::string min, max;
      pTable_->GetCellValue(1, r, min);
      bandinfo.min_ = StringToNumber<double>(min);
      pTable_->GetCellValue(2, r, max);
      bandinfo.max_ = StringToNumber<double>(max);
      currentClassInfo_.bandsThreshold_.push_back(bandinfo);
   }
   EndModal(wxID_OK);
}

/** Se crea la ventana de herramienta en las clases derivadas */
bool ThresholdClassDefinitionWidget::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_THRESHOLD_CLASS_DEF_PANEL"));
   if (!pToolWindow_)
      return false;
   GET_CONTROL(*pToolWindow_,
         "ID_CANCEL_BTN", wxButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
   wxCommandEventHandler(ThresholdClassDefinitionEventHandler::OnCancelClick),
                                                      NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_,
         "ID_OK_BTN", wxButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
   wxCommandEventHandler(ThresholdClassDefinitionEventHandler::OnOkClick),
                                                      NULL, pEventHandler_);
   wxTextCtrl* pcnamectrl = XRCCTRL(*pToolWindow_, "ID_CLASS_NAME", wxTextCtrl);
   pcnamectrl->SetValue(currentClassInfo_.classname_.c_str());
   if (InitializeTable())
      AddControl(pTablePart_->GetWidget(), "ID_CLASS_TABLE_PANEL");
   return true;
}

/** Metodo que verifica la validez de la definicion de una clase **/
bool ThresholdClassDefinitionWidget::HasValidClassDefinition() {
   wxTextCtrl* pcnamectrl = XRCCTRL(*pToolWindow_, "ID_CLASS_NAME", wxTextCtrl);
   if (pcnamectrl->GetValue().empty())
      return false;
   bool validdef = true;
   for (int r = 0; validdef && r < pTable_->GetRows(); ++r) {
      std::string minstr, maxstr;
      pTable_->GetCellValue(1, r, minstr);
      double min = StringToNumber<double>(minstr);
      pTable_->GetCellValue(2, r, maxstr);
      double max = StringToNumber<double>(maxstr);
      if (min > max)
         validdef = false;
   }
   return validdef;
}
} /** namespace suri */
