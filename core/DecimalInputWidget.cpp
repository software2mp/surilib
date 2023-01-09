/*! \file DecimalInputWidget.cpp */
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
#include "suri/DecimalInputWidget.h"
#include "suri/AuxiliaryFunctions.h"
#include "SpatialReference.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"

// Includes Wx

// Defines

// forwards
#ifdef __NOT_USE_LATITIUDE_PART__

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(LatitudeLongitudeInputWidgetEvent, LatitudeLongitudeInputWidget)
   IMPLEMENT_EVENT_CALLBACK(OnLatitudeHyperlink, OnLatitudeHyperlink(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnLongitudeHyperlink, OnLongitudeHyperlink(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnLostFocus, OnLostFocus(event), wxFocusEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** Valores permitidos para ingreso de datos */
wxString DecimalFilter[] = { wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6"),
                            wxT("7"), wxT("8"), wxT("9"), wxT("0") };

/** Valores de los controles de ingreso de coordenadas de latitud */
const char *pDecimalLatitudeControlIds[] = { wxT("ID_LATITUDEHYPERLINK"), wxT(
      "ID_DECIMAL_N_COORD_TEXT"), wxT(label_SOUTH), wxT(label_NORTH) };
/** valores de los controles de ingreso de coordenadas de longitud */
const char *pDecimalLongitudeControlIds[] = { wxT("ID_LONGITUDEHYPERLINK"), wxT(
      "ID_DECIMAL_E_COORD_TEXT"), wxT(label_WEST), wxT(label_EAST) };

/** Constructor **/
DecimalInputWidget::DecimalInputWidget(bool Validate) :
      LatitudeLongitudeInputWidget(Validate) {
}

/** Destructor **/
DecimalInputWidget::~DecimalInputWidget() {
}

/** Creacion de la ventana */
bool DecimalInputWidget::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_DECIMAL_COORD_PANEL"));
   if (!pToolWindow_)
      return false;

   GET_CONTROL(*pToolWindow_, wxT("ID_LATITUDEHYPERLINK"), wxHyperlinkCtrl)
      ->Connect(wxEVT_COMMAND_HYPERLINK, wxCommandEventHandler(
            LatitudeLongitudeInputWidgetEvent::OnLatitudeHyperlink),
                NULL, pEventHandler_);

   GET_CONTROL(*pToolWindow_, wxT("ID_LONGITUDEHYPERLINK"), wxHyperlinkCtrl)
      ->Connect(wxEVT_COMMAND_HYPERLINK, wxCommandEventHandler(
            LatitudeLongitudeInputWidgetEvent::OnLongitudeHyperlink),
                NULL, pEventHandler_);
   // Conecto el lost focus a todos los text
   GET_CONTROL(*pToolWindow_, wxT("ID_DECIMAL_N_COORD_TEXT"), wxTextCtrl)
      ->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(
            LatitudeLongitudeInputWidgetEvent::OnLostFocus),
                NULL, pEventHandler_);
   // Conecto el lost focus a todos los text
   GET_CONTROL(*pToolWindow_, wxT("ID_DECIMAL_E_COORD_TEXT"), wxTextCtrl)
      ->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(
            LatitudeLongitudeInputWidgetEvent::OnLostFocus),
                NULL, pEventHandler_);
   // wxTextValidator degree
   wxTextValidator textvalidatordegree = wxTextValidator(wxFILTER_INCLUDE_CHAR_LIST);
   wxArrayString degreearray = wxArrayString(10, DecimalFilter);
   degreearray.Add(wxT("-"));
   degreearray.Add(wxT("."));
   textvalidatordegree.SetIncludes(degreearray);
    GET_CONTROL(*pToolWindow_, wxT("ID_DECIMAL_N_COORD_TEXT"), wxTextCtrl)
       ->SetValidator(textvalidatordegree);
    GET_CONTROL(*pToolWindow_, wxT("ID_DECIMAL_E_COORD_TEXT"), wxTextCtrl)
       ->SetValidator(textvalidatordegree);
    DisplayCoordinates();
   return true;
}

/** retorna la coordenada que ingresaron, informa si es valida */
bool DecimalInputWidget::GetCoordinate(Coordinates& DecimalCoord) {
   // TODO(Ramiro - TCK #2446): Ver porque llama a GetCoordinate sin pToolWindow inicializado.
   if (!pToolWindow_)
      return false;
   wxString latitude =
         USE_CONTROL(*pToolWindow_, wxT("ID_DECIMAL_N_COORD_TEXT"), wxTextCtrl,
               GetValue(), wxT(message_NOT_APPLICABLE) );
   wxString longitude =
         USE_CONTROL(*pToolWindow_, wxT("ID_DECIMAL_E_COORD_TEXT"), wxTextCtrl,
               GetValue(), wxT(message_NOT_APPLICABLE) );
   double latitudevalue = 0.0, longitudevalue = 0.0;
   latitude.ToDouble(&latitudevalue);
   longitude.ToDouble(&longitudevalue);
   Coordinates newcoordinate = Coordinates(longitudevalue, latitudevalue);
   // Modifico el signo segun N o S este seleccionado
   if (!IsProj()) {
      if ((!north_) && (latitude.Find(wxT("-")) == wxNOT_FOUND)) {
         newcoordinate.y_ = -newcoordinate.y_;
      }
      if ((!east_) && (longitude.Find(wxT("-")) == wxNOT_FOUND)) {
         newcoordinate.x_ = -newcoordinate.x_;
      }
   }
   return ValidateAndUpdateCoordinates(DecimalCoord, newcoordinate, latLongCoordinate_);
}

/** Metodo auxiliar que se encarga de validar la nueva coordenada y actualizar
 * el valor de la coordenada guardada */
bool DecimalInputWidget::ValidateAndUpdateCoordinates(Coordinates& DecimalCoordinate,
                                  Coordinates& NewCoordinate,
                                  Coordinates& ReferenceCoordinate) {
   bool returnvalue = false;
   if (SpatialReference::IsProjected(spatialReference_)
         || ValidateCoordinates(NewCoordinate)) {
      returnvalue = true;
      DecimalCoordinate = NewCoordinate;
   }
   double longitude = 0.0, latitude = 0.0;
   longitudeStringValue_.ToDouble(&longitude);
   latitudeStringValue_.ToDouble(&latitude);
   if (DecimalCoordinate.x_ == longitude)
      DecimalCoordinate.x_ = ReferenceCoordinate.x_;
   else
      ReferenceCoordinate.x_ = DecimalCoordinate.x_;
   if (DecimalCoordinate.y_ == latitude)
      DecimalCoordinate.y_ = ReferenceCoordinate.y_;
   else
      ReferenceCoordinate.y_ = DecimalCoordinate.y_;
   return returnvalue;
}

/** Despliega la coordenada interna en los controles */
void DecimalInputWidget::DisplayCoordinates() {
   std::string latitude = DoubleToString(latLongCoordinate_.y_);
   std::string longitude = DoubleToString(latLongCoordinate_.x_);
   longitudeStringValue_ = longitude;
   latitudeStringValue_ = latitude;
   GET_CONTROL(*pToolWindow_, wxT("ID_DECIMAL_N_COORD_TEXT"),
               wxTextCtrl)->SetValue(latitude);
   GET_CONTROL(*pToolWindow_, wxT("ID_DECIMAL_E_COORD_TEXT"),
               wxTextCtrl)->SetValue(longitude);
   // Actualizo signos de control
   UpdateControlSign(latLongCoordinate_.x_, east_, pDecimalLongitudeControlIds);
   UpdateControlSign(latLongCoordinate_.y_, north_, pDecimalLatitudeControlIds);
}

/** Metodo que se llama cuando se configura el sistema de referencia. Permite
 *  realizar alguna accion luego de configurar el sistema de referencia
 *  de trabajo
 */
void DecimalInputWidget::OnSpatialReferenceSet() {
   if (pToolWindow_) {
      std::string xlabel, ylabel;
      bool showlinks = true;
      if (!SpatialReference::IsProjected(spatialReference_)) {
         xlabel = "Latitud:";
         ylabel = "Longitud:";
      } else {
         xlabel = "N: ";
         ylabel = "E: ";
         showlinks = false;
      }
      GET_CONTROL(*pToolWindow_, wxT("ID_SYSTEM_UNIT_X"),
            wxStaticText)->SetLabel(xlabel.c_str());
      GET_CONTROL(*pToolWindow_, wxT("ID_SYSTEM_UNIT_Y"),
            wxStaticText)->SetLabel(ylabel.c_str());
      GET_CONTROL(*pToolWindow_, wxT("ID_LATITUDEHYPERLINK"),
            wxHyperlinkCtrl)->Show(showlinks);
      GET_CONTROL(*pToolWindow_, wxT("ID_LONGITUDEHYPERLINK"),
            wxHyperlinkCtrl)->Show(showlinks);
   }
}

} /** namespace suri */

#endif
