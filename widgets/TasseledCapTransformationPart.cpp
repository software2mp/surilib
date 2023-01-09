/*! \file TasseledCapTransformationPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "TasseledCapTransformationPart.h"

#include "TasseledCapFactory.h"
#include "suri/ProcessAdaptLayer.h"
#include "resources.h"

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(TasseledCapTransformationPartEvent, TasseledCapTransformationPart)
   IMPLEMENT_EVENT_CALLBACK(OnCbSensorChanged, OnCbSensorChanged(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnChkBandsToggled, OnChkBandsToggled(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT

/**
 * Ctor.
 */
TasseledCapTransformationPart::TasseledCapTransformationPart(RasterElement* pElement,
                                                             bool Enable, bool Modified) :
      Part(Enable, Modified), NEW_EVENT_OBJECT(TasseledCapTransformationPartEvent),
      pElement_(pElement), changed_(false) {
   windowTitle_ = _(caption_CONFIGURATION_PART);
}

/**
 * Dtor.
 */
TasseledCapTransformationPart::~TasseledCapTransformationPart() {
   // Libero data de sensores.
   std::vector<SensorClientData*>::iterator sit = sensorClientData_.begin();
   for (; sit != sensorClientData_.end(); ++sit) {
      delete *sit;
   }
   sensorClientData_.clear();
}

/**
 * Indica si el Part tiene cambios para salvar.
 */
bool TasseledCapTransformationPart::HasChanged() {
   return changed_ || !HasValidData();
}

/**
 * Salva los cambios realizados en el Part.
 */
bool TasseledCapTransformationPart::CommitChanges() {
   wxChoice* pchbase = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_TASSELED_CAP_SENSOR_CB"), wxChoice);

   // Guardo indo del sensor.
   SensorClientData* pdata = static_cast<SensorClientData*>(
         pchbase->GetClientData(pchbase->GetSelection()));

   commitSensor_ = pdata->GetSensor();

   // Guardo info de las bandas seleccionadas.
   commitBands_.clear();


   wxCheckListBox* pchklist = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                      wxT("ID_TASSELED_CAP_OUTPUT_CHKLIST"),
                                      wxCheckListBox);

   for (int ix = 0, lenix = pchklist->GetCount(); ix < lenix; ++ix) {
      if (pchklist->IsChecked(ix)) {
         commitBands_.push_back(ix);
      }
   }

   GetProcessLayer()->AddAttribute<std::string>(
         ProcessAdaptLayer::TasseledCapSensorKeyAttr, commitSensor_);

   GetProcessLayer()->AddAttribute<std::vector<int> >(
         ProcessAdaptLayer::TasseledCapOutputBandsKeyAttr, commitBands_);

   changed_ = false;

   return true;
}

/**
 * Restaura los valores originales del Part.
 */
bool TasseledCapTransformationPart::RollbackChanges() {
   LoadCbSensor();
   LoadPnlOutputBands(false);

   // Reselecciono el sensor que habia commiteado.
   wxChoice* pchbase = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_TASSELED_CAP_SENSOR_CB"), wxChoice);

   for (int ix = 0, lenix = pchbase->GetCount(); ix < lenix; ++ix) {
      SensorClientData* pdata = static_cast<SensorClientData*>(pchbase->GetClientData(ix));
      if (pdata->GetSensor().compare(commitSensor_) == 0) {
         pchbase->SetSelection(ix);
         break;
      }
   }

   // Reselecciono las bandas de salida que habia commiteado.
   wxCheckListBox* pchklist = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                      wxT("ID_TASSELED_CAP_OUTPUT_CHKLIST"),
                                      wxCheckListBox);

   for (int jx = 0, lenjx = commitBands_.size(); jx < lenjx; ++jx) {
      pchklist->Check(commitBands_[jx]);
   }

   return true;
}

/**
 * Inicializa el part.
 */
void TasseledCapTransformationPart::SetInitialValues() {
   CommitChanges();
}

/**
 * Devuelve si la parte tiene datos validos.
 */
bool TasseledCapTransformationPart::HasValidData() {
   bool found = false;

   wxCheckListBox* pchklist = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                      wxT("ID_TASSELED_CAP_OUTPUT_CHKLIST"),
                                      wxCheckListBox);

   for (int ix = 0, lenix = pchklist->GetCount(); ix < lenix && !found; ++ix) {
      if (pchklist->IsChecked(ix))
         found = true;
   }

   return found;
}

/**
 * Actualiza el estado de la parte.
 */
void TasseledCapTransformationPart::Update() {

}

/**
 * Implementacion del metodo de creacion de ventana de Widget.
 */
bool TasseledCapTransformationPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_TASSELED_CAP_PART"));

   wxChoice* pchbase = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                  wxT("ID_TASSELED_CAP_SENSOR_CB"), wxChoice);

   pchbase->Connect(
            wxEVT_COMMAND_CHOICE_SELECTED,
            wxCommandEventHandler(TasseledCapTransformationPartEvent::OnCbSensorChanged),
            NULL, pEventHandler_);

   wxCheckListBox* pchklist = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                      wxT("ID_TASSELED_CAP_OUTPUT_CHKLIST"),
                                      wxCheckListBox);

   pchklist->Connect(
         wxEVT_COMMAND_CHECKLISTBOX_TOGGLED,
         wxCommandEventHandler(TasseledCapTransformationPartEvent::OnChkBandsToggled),
         NULL,
         pEventHandler_);

   LoadCbSensor();
   LoadPnlOutputBands();

   return true;
}

/**
 * Handler para el evento de cambio de sensor.
 */
void  TasseledCapTransformationPart::OnCbSensorChanged(wxCommandEvent &Event) {
   LoadPnlOutputBands();
   changed_ = true;
}

/**
 * Handler para el evento de check de la lista.
 */
void TasseledCapTransformationPart::OnChkBandsToggled(wxCommandEvent &Event) {
   changed_ = true;
}

/**
 * Carga el combo de transformaciones.
 */
void TasseledCapTransformationPart::LoadCbSensor() {
   namespace SRT = suri::raster::transformation;

   wxChoice* pchbase = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_TASSELED_CAP_SENSOR_CB"), wxChoice);

   pchbase->Clear();

   std::vector<std::string> tranfs =
         SRT::TasseledCapFactory::GetInstance()->GetAvailableTCTransformations();

   for (int ix = 0, lenix = tranfs.size(); ix < lenix; ++ix) {
      std::string sensor = tranfs[ix];
      SensorClientData* pdata = new SensorClientData(sensor);
      int index = pchbase->Append(_(sensor.c_str()));
      pchbase->SetClientData(index, pdata);
      sensorClientData_.push_back(pdata);
   }

   pchbase->SetSelection(0);
}

/**
 * Carga selector de bandas de salida.
 */
void TasseledCapTransformationPart::LoadPnlOutputBands(bool CheckAll) {
   namespace SRT = suri::raster::transformation;

   wxChoice* pchbase = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_TASSELED_CAP_SENSOR_CB"), wxChoice);

   void* pclientdata = pchbase->GetClientData(pchbase->GetSelection());

   SensorClientData* pdata = static_cast<SensorClientData*>(pclientdata);

   wxCheckListBox* pchklist = XRCCTRL(*(this->GetWidget()->GetWindow()),
                             wxT("ID_TASSELED_CAP_OUTPUT_CHKLIST"), wxCheckListBox);

   std::map<int, std::string> outbands =
         SRT::TasseledCapFactory::GetInstance()->GetOutputBandsForTCTransformation(
               pdata->GetSensor());

   pchklist->Clear();

   std::map<int, std::string>::iterator it = outbands.begin();
   for ( ; it != outbands.end(); ++it) {
      int index = pchklist->Append(_(it->second.c_str()));
      if (CheckAll)
         pchklist->Check(index);
   }
}
/** Retorna el icono de la herramienta */
void TasseledCapTransformationPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(tool_TASSELED_CAP_32, ToolBitmap);
}

}  // namespace suri
