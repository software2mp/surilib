/*! \file Setup3DPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "Setup3DPart.h"
#include "resources.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Configuration.h"

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(Setup3DPartEvent, Setup3DPart)
      IMPLEMENT_EVENT_CALLBACK(OnSliderScroll, OnSliderScroll(event), wxScrollEvent)
END_IMPLEMENT_EVENT_OBJECT/** \endcond */

/** Ctor. */
Setup3DPart::Setup3DPart() : NEW_EVENT_OBJECT(Setup3DPartEvent){
   windowTitle_ = wxT(window_title_3D_CONFIGURATION);
}
/** Destructor- */
Setup3DPart::~Setup3DPart() {
   // TODO Auto-generated destructor stub
}

/** Crea el part (el interior de la ventana)*/
bool Setup3DPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_PANEL_TERRAIN_PROP"));
   modified_ = false;
   return true;
}
/** Asigna el icono para el part.  */
void Setup3DPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_GENERIC_PROPERTIES, ToolBitmap); // Agregar el icono indicado.
}
/** Inicializa los controles */
void Setup3DPart::SetInitialValues() {
   iquality_ = Configuration::GetParameter(std::string("v3d_quality"), 1);
   iexageration_ = Configuration::GetParameter(std::string("v3d_exageration"), 1);
   icamera_ = Configuration::GetParameter(std::string("v3d_camera"), 1);
   GET_CONTROL(*pToolWindow_, "ID_SLIDER_QUALITY", wxSlider)
      ->SetValue(iquality_);
   GET_CONTROL(*pToolWindow_, "ID_SLIDER_EXAGERATION", wxSlider)
      ->SetValue(iexageration_);
   GET_CONTROL(*pToolWindow_, "ID_SLIDER_CAMERA", wxSlider)
      ->SetValue(icamera_);
   // Conectamos
   GET_CONTROL(*pToolWindow_, "ID_SLIDER_QUALITY", wxSlider)->Connect(
         wxEVT_SCROLL_CHANGED,
         wxScrollEventHandler(Setup3DPartEvent::OnSliderScroll), NULL,
         pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_SLIDER_EXAGERATION", wxSlider)->Connect(
         wxEVT_SCROLL_CHANGED,
         wxScrollEventHandler(Setup3DPartEvent::OnSliderScroll), NULL,
         pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_SLIDER_CAMERA", wxSlider)->Connect(
         wxEVT_SCROLL_CHANGED,
         wxScrollEventHandler(Setup3DPartEvent::OnSliderScroll), NULL,
         pEventHandler_);
   // Seteamos los labels
   GET_CONTROL(*pToolWindow_, "ID_QUAL_VALUE_LABEL", wxStaticText)->SetLabel(
         NumberToString<int>(iquality_).c_str());
   GET_CONTROL(*pToolWindow_, "ID_EXAG_VALUE_LABEL", wxStaticText)->SetLabel(
         NumberToString<int>(iexageration_).c_str());
   GET_CONTROL(*pToolWindow_, "ID_VEL_CAM_VALUE_LABEL", wxStaticText)->SetLabel(
         NumberToString<int>(icamera_).c_str());
   modified_ = false;
}
/** Indica si el Part tiene cambios para salvar. */
bool Setup3DPart::HasChanged() {
   int value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_QUALITY", wxSlider, GetValue(), 1);
   if (value != iquality_) {
      return true;
   }
   value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_EXAGERATION", wxSlider,
         GetValue(), 1);
   if (value != iexageration_) {
      return true;
   }
   value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_CAMERA", wxSlider, GetValue(), 1);
   if (value != icamera_) {
      return true;
   }
   return false;
}
/** Salva los cambios realizados en el Part. */
bool Setup3DPart::CommitChanges() {
   // obtengo valor del slider de calidad
   int value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_QUALITY", wxSlider, GetValue(), 1);
   Configuration::SetParameter(std::string("v3d_quality"), long(value));


   // obtengo valor del slider de exageracion.
   value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_EXAGERATION", wxSlider,
         GetValue(), 1);
   // seteo factor de exageracion del terreno.
   Configuration::SetParameter(std::string("v3d_exageration"), long(value));

   // obtengo valor del slider de velocidad de camara.
   value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_CAMERA", wxSlider, GetValue(), 1);
   // seteo paso de camara.
   Configuration::SetParameter(std::string("v3d_camera"), long(value));

   modified_ = false;
   return true;
}
/** Restaura los valores originales del Part. */
bool Setup3DPart::RollbackChanges() {
   SetInitialValues();
   return true;
}
/** Actualiza el estado de la parte */
void Setup3DPart::Update() {

}
/** Evento de movimiento de sliders */
void Setup3DPart::OnSliderScroll(wxScrollEvent &Event) {
   int value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_QUALITY", wxSlider, GetValue(), 1);
   GET_CONTROL(*pToolWindow_, "ID_QUAL_VALUE_LABEL", wxStaticText)->SetLabel(
         NumberToString<int>(value).c_str());
   value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_EXAGERATION", wxSlider,
                       GetValue(), 1);
   GET_CONTROL(*pToolWindow_, "ID_EXAG_VALUE_LABEL", wxStaticText)->SetLabel(
               NumberToString<int>(value).c_str());
   value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_CAMERA", wxSlider,
                          GetValue(), 1);
   GET_CONTROL(*pToolWindow_, "ID_VEL_CAM_VALUE_LABEL", wxStaticText)->SetLabel(
                NumberToString<int>(value).c_str());
}


} /* namespace suri */
