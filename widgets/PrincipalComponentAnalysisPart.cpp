/*! \file PrincipalComponentAnalysisPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "PrincipalComponentAnalysisPart.h"

#include "suri/AuxiliaryFunctions.h"
#include "resources.h"

#include "wx/radiobut.h"
#include "wx/stattext.h"
#include "wx/textctrl.h"

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(PrincipalComponentAnalysisPartEvent, PrincipalComponentAnalysisPart)
   IMPLEMENT_EVENT_CALLBACK(OnRadCorrelationSelected, OnRadCorrelationSelected(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnRadCovarianceSelected, OnRadCovarianceSelected(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnRadTotalSelected, OnRadTotalSelected(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnRadPartialSelected, OnRadPartialSelected(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT

/**
 * Ctor.
 */
PrincipalComponentAnalysisPart::PrincipalComponentAnalysisPart(RasterElement* pElement,
                                                               bool Enable, bool Modified) :
      Part(Enable, Modified), NEW_EVENT_OBJECT(PrincipalComponentAnalysisPartEvent),
      pElement_(pElement), changed_(false) {
   windowTitle_ = _(caption_CONFIGURATION_PART);
}

/**
 * Dtor.
 */
PrincipalComponentAnalysisPart::~PrincipalComponentAnalysisPart() {

}

/**
 * Indica si el Part tiene cambios para salvar.
 */
bool PrincipalComponentAnalysisPart::HasChanged() {
   return changed_ || !HasValidData();
}

/**
 * Salva los cambios realizados en el Part.
 */
bool PrincipalComponentAnalysisPart::CommitChanges() {
   wxRadioButton* prad = NULL;

   // Chequeo la opcion por la matriz de correlacion.
   prad = XRCCTRL(*(this->GetWidget()->GetWindow()), wxT("ID_PCA_CORRELATION_RADIO"),
                  wxRadioButton);

   if (prad->GetValue() == true) {
      std::string matrixtype = "correlation";
      GetProcessLayer()->AddAttribute<std::string>(ProcessAdaptLayer::MatrixTypeKeyAttr,
                                                   matrixtype);
   }

   // Chequeo la opcion por la matriz de covarianza.
   prad = XRCCTRL(*(this->GetWidget()->GetWindow()), wxT("ID_PCA_COVARIANCE_RADIO"),
                  wxRadioButton);

   if (prad->GetValue() == true) {
      std::string matrixtype = "covariance";
      GetProcessLayer()->AddAttribute<std::string>(ProcessAdaptLayer::MatrixTypeKeyAttr,
                                                   matrixtype);
   }

   // Chequeo la opcion por la cantidad de componentes. Si esta seleccionado
   // el total lo indico con 0.
   prad = XRCCTRL(*(this->GetWidget()->GetWindow()), wxT("ID_PCA_TOTAL_RADIO"),
                  wxRadioButton);

   if (prad->GetValue() == true) {
      int compcount = 0;
      GetProcessLayer()->AddAttribute<int>(ProcessAdaptLayer::ComponentsCountKeyAttr,
                                           compcount);
   }

   // Si esta seleccionado una cantidad parcial entonces guardo lo especificado.
   prad = XRCCTRL(*(this->GetWidget()->GetWindow()), wxT("ID_PCA_PARTIAL_RADIO"),
                  wxRadioButton);

   if (prad->GetValue() == true) {
      wxTextCtrl* ptxt = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                 wxT("ID_PCA_PARTIAL_COUNT_TXT"), wxTextCtrl);

      int compcount = StringToNumber<int>(ptxt->GetValue().c_str());
      GetProcessLayer()->AddAttribute<int>(ProcessAdaptLayer::ComponentsCountKeyAttr,
                                           compcount);
   }

   return true;
}

/**
 * Restaura los valores originales del Part.
 */
bool PrincipalComponentAnalysisPart::RollbackChanges() {
   return true;
}

/**
 * Inicializa el part.
 */
void PrincipalComponentAnalysisPart::SetInitialValues() {
   std::string matrixtype = "correlation";
   GetProcessLayer()->AddAttribute<std::string>(ProcessAdaptLayer::MatrixTypeKeyAttr,
                                                matrixtype);

   int compcount = 0;
   GetProcessLayer()->AddAttribute<int>(ProcessAdaptLayer::ComponentsCountKeyAttr,
                                        compcount);
}

/**
 * Devuelve si la parte tiene datos validos.
 */
bool PrincipalComponentAnalysisPart::HasValidData() {
   return true;
}

/**
 * Actualiza el estado de la parte.
 */
void PrincipalComponentAnalysisPart::Update() {

}

/**
 * Implementacion del metodo de creacion de ventana de Widget.
 */
bool PrincipalComponentAnalysisPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_PRINCIPAL_COMPONENTS_PART"));

   wxRadioButton* pradcorr = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                      wxT("ID_PCA_CORRELATION_RADIO"), wxRadioButton);

   pradcorr->Connect(
         wxEVT_COMMAND_RADIOBUTTON_SELECTED,
         wxCommandEventHandler(PrincipalComponentAnalysisPartEvent::OnRadCorrelationSelected),
         NULL, pEventHandler_);

   wxRadioButton* pradcov = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                        wxT("ID_PCA_COVARIANCE_RADIO"), wxRadioButton);

   pradcov->Connect(
         wxEVT_COMMAND_RADIOBUTTON_SELECTED,
         wxCommandEventHandler(PrincipalComponentAnalysisPartEvent::OnRadCovarianceSelected),
         NULL, pEventHandler_);

   wxRadioButton* pradtotal = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                      wxT("ID_PCA_TOTAL_RADIO"), wxRadioButton);

   pradtotal->Connect(
         wxEVT_COMMAND_RADIOBUTTON_SELECTED,
         wxCommandEventHandler(PrincipalComponentAnalysisPartEvent::OnRadTotalSelected),
         NULL, pEventHandler_);

   wxRadioButton* pradpartial = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                        wxT("ID_PCA_PARTIAL_RADIO"), wxRadioButton);

   pradpartial->Connect(
         wxEVT_COMMAND_RADIOBUTTON_SELECTED,
         wxCommandEventHandler(PrincipalComponentAnalysisPartEvent::OnRadPartialSelected),
         NULL, pEventHandler_);

   return true;
}

/**
 * Handler para la seleccion del radio button correlacion.
 */
void PrincipalComponentAnalysisPart::OnRadCorrelationSelected(wxCommandEvent& Event) {
   changed_ = true;
}

/**
 * Handler para la seleccion del radio button covarianza.
 */
void PrincipalComponentAnalysisPart::OnRadCovarianceSelected(wxCommandEvent& Event) {
   changed_ = true;
}

/**
 * Handler para la seleccion del radio button total.
 */
void PrincipalComponentAnalysisPart::OnRadTotalSelected(wxCommandEvent& Event) {
   EnablePartialCount(false);
   changed_ = true;
}

/**
 * Handler para la seleccion del radio button parcial.
 */
void PrincipalComponentAnalysisPart::OnRadPartialSelected(wxCommandEvent& Event) {
   EnablePartialCount(true);
   changed_ = true;
}

/**
 * Habilita o deshabilita el control para la carga
 * de componentes principales.
 */
void PrincipalComponentAnalysisPart::EnablePartialCount(bool Enable) {
   wxStaticText* pstt = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                wxT("ID_PCA_PARTIAL_LABEL_STT"), wxStaticText);

   wxTextCtrl* ptxt = XRCCTRL(*(this->GetWidget()->GetWindow()),
                              wxT("ID_PCA_PARTIAL_COUNT_TXT"), wxTextCtrl);

   pstt->Enable(Enable);
   ptxt->Enable(Enable);
}
/** Retorna el icono de la herramienta */
void PrincipalComponentAnalysisPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(tool_ACP_32, ToolBitmap);
}

}  // namespace suri
