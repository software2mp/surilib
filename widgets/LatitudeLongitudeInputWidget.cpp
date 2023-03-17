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

// Includes standar
#include <sstream>
#include <cmath>

// Includes Suri
#include "suri/LatitudeLongitudeInputWidget.h"
#include "suri/messages.h"
#include "resources.h"
#include "suri/World.h"
#include "logmacros.h"
#include "suri/AuxiliaryFunctions.h"
// Includes Wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"
#include "wx/hyperlink.h"

// forward
void InitLatitudeLongitudeInputXmlResource();

// Defines
/** Separador decimal */
#define DECIMAL_SEPARATOR "."
/** indice del identificador del control de hipervinculo */
#define HYPERLINK_INDEX 0
/** indice del identificador del control de texto de grados */
#define DEGREE_INDEX 1
/** indice del texto */
#define NEGATIVE_LABEL_INDEX 2
/** indice del texto */
#define POSITIVE_LABEL_INDEX 3

/** namespace suri */
namespace suri {
/** Valores de los controles de ingreso de coordenadas de latitud */
const char *pLatitudeControlIds[] = { wxT("ID_LATITUDEHYPERLINK"), wxT("ID_DEGREE_N"),
                                      wxT(label_SOUTH), wxT(label_NORTH) };
/** valores de los controles de ingreso de coordenadas de longitud */
const char *pLongitudeControlIds[] = { wxT("ID_LONGITUDEHYPERLINK"), wxT("ID_DEGREE_E"),
                                       wxT(label_WEST), wxT(label_EAST) };
/** Valores permitidos para ingreso de datos */
wxString NumberFilter[] = { wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6"),
                            wxT("7"), wxT("8"), wxT("9"), wxT("0") };

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(LatitudeLongitudeInputWidgetEvent, LatitudeLongitudeInputWidget)
   IMPLEMENT_EVENT_CALLBACK(OnLatitudeHyperlink, OnLatitudeHyperlink(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnLongitudeHyperlink, OnLongitudeHyperlink(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnLostFocus, OnLostFocus(event), wxFocusEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/**
 * Constructor
 * @param[in] Validate indica si hay que validar coordenadas
 */
LatitudeLongitudeInputWidget::LatitudeLongitudeInputWidget(bool Validate) :
      NavigationInputWidget(), north_(true), east_(true), validate_(Validate),
      NEW_EVENT_OBJECT(LatitudeLongitudeInputWidgetEvent)
{
   // Creo ventana
   windowTitle_ = GetWindowTitle();
}

/**
 * Destructor
 */
LatitudeLongitudeInputWidget::~LatitudeLongitudeInputWidget() {
   DELETE_EVENT_OBJECT;
}

// ------------------------------------------------------------------------------
/**
 * Carga herramieta que permite al usuario ingresar coordenadas en latitud
 * y longitud.
 * Hace connect de cerrar, click sobre iconos 'n/s' y 'e/o' y sobre eventos
 * lostfocus de los textCtrl para actualizar signo.
 * @return bool que indica si se pudo crear la ventana
 */
bool LatitudeLongitudeInputWidget::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_LATITUDE_LONGITUDE_INPUT"));

   GET_CONTROL(*pToolWindow_, wxT("ID_LATITUDEHYPERLINK"), wxHyperlinkCtrl)
      ->Connect(wxEVT_COMMAND_HYPERLINK, wxCommandEventHandler(
            LatitudeLongitudeInputWidgetEvent::OnLatitudeHyperlink),
                NULL, pEventHandler_);

   GET_CONTROL(*pToolWindow_, wxT("ID_LONGITUDEHYPERLINK"), wxHyperlinkCtrl)
      ->Connect(wxEVT_COMMAND_HYPERLINK, wxCommandEventHandler(
            LatitudeLongitudeInputWidgetEvent::OnLongitudeHyperlink),
                NULL, pEventHandler_);

   // Conecto el lost focus a todos los text
   GET_CONTROL(*pToolWindow_, wxT("ID_DEGREE_N"), wxTextCtrl)
      ->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(
            LatitudeLongitudeInputWidgetEvent::OnLostFocus),
                NULL, pEventHandler_);

   GET_CONTROL(*pToolWindow_, wxT("ID_MIN_N"), wxTextCtrl)
      ->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(
            LatitudeLongitudeInputWidgetEvent::OnLostFocus),
                NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_, wxT("ID_SEC_N"), wxTextCtrl)
      ->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(
            LatitudeLongitudeInputWidgetEvent::OnLostFocus),
                NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_, wxT("ID_DEGREE_E"), wxTextCtrl)
      ->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(
            LatitudeLongitudeInputWidgetEvent::OnLostFocus),
                NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_, wxT("ID_MIN_E"), wxTextCtrl)
      ->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(
            LatitudeLongitudeInputWidgetEvent::OnLostFocus),
                NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_, wxT("ID_SEC_E"), wxTextCtrl)
      ->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(
            LatitudeLongitudeInputWidgetEvent::OnLostFocus),
                NULL, pEventHandler_);

   // wxTextValidator degree
   wxTextValidator textvalidatordegree = wxTextValidator(wxFILTER_INCLUDE_CHAR_LIST);
   wxArrayString degreearray = wxArrayString(10, NumberFilter);
   degreearray.Add(wxT("-"));
   textvalidatordegree.SetIncludes(degreearray);
   GET_CONTROL(*pToolWindow_, wxT("ID_DEGREE_N"), wxTextCtrl)
      ->SetValidator(textvalidatordegree);
   GET_CONTROL(*pToolWindow_, wxT("ID_DEGREE_N"), wxTextCtrl)->InitDialog();
   GET_CONTROL(*pToolWindow_, wxT("ID_DEGREE_E"), wxTextCtrl)
      ->SetValidator(textvalidatordegree);
   GET_CONTROL(*pToolWindow_, wxT("ID_DEGREE_E"), wxTextCtrl)->InitDialog();

   // wxTextValidator minute
   wxTextValidator textvalidatorminutes = wxTextValidator(wxFILTER_INCLUDE_CHAR_LIST);
   textvalidatorminutes.SetIncludes(wxArrayString(10, NumberFilter));
   GET_CONTROL(*pToolWindow_, wxT("ID_MIN_N"), wxTextCtrl)
      ->SetValidator(textvalidatorminutes);
   GET_CONTROL(*pToolWindow_, wxT("ID_MIN_N"), wxTextCtrl)->InitDialog();
   GET_CONTROL(*pToolWindow_, wxT("ID_MIN_E"), wxTextCtrl)
      ->SetValidator(textvalidatorminutes);
   GET_CONTROL(*pToolWindow_, wxT("ID_MIN_E"), wxTextCtrl)->InitDialog();

   // wxTextValidator second
   wxTextValidator textvalidatorseconds = wxTextValidator(wxFILTER_INCLUDE_CHAR_LIST);
   wxArrayString secondarray = wxArrayString(10, NumberFilter);
   secondarray.Add(DECIMAL_SEPARATOR);
   textvalidatorseconds.SetIncludes(secondarray);
   GET_CONTROL(*pToolWindow_, wxT("ID_SEC_N"), wxTextCtrl)
      ->SetValidator(textvalidatorseconds);
   GET_CONTROL(*pToolWindow_, wxT("ID_SEC_N"), wxTextCtrl)->InitDialog();
   GET_CONTROL(*pToolWindow_, wxT("ID_SEC_E"), wxTextCtrl)
      ->SetValidator(textvalidatorseconds);
   GET_CONTROL(*pToolWindow_, wxT("ID_SEC_E"), wxTextCtrl)->InitDialog();

   pParentWindow_->Fit();

   DisplayCoordinates();

   return true;
}

// ------------------------------------------------------------------------------
/** Devuelve titulo de la ventana */
const wxString LatitudeLongitudeInputWidget::GetWindowTitle() const {
   return _(caption_NAVIGATION_TOOL);
}

/**
 * Si la coordenada que ingresaron es valida y es diferente a la actual
 * se actualiza la variable interna.
 * Luego retorna la coordenada interna. Esto evita que se pierda
 * precision si no se modifica el control.
 * @param[out] LatLongCoordinate coordenada que se ingreso en control
 * @return bool que indica si la coordenada en control es valida
 */
bool LatitudeLongitudeInputWidget::GetCoordinate(Coordinates &LatLongCoordinate) {
   Coordinates newcoordinate;
   // -------------- Para norte -----------------
   if (!pToolWindow_)
      return false;
   wxString degree = USE_CONTROL(*pToolWindow_, wxT("ID_DEGREE_N"), wxTextCtrl,
         GetValue(), wxT(message_NOT_APPLICABLE) );
   wxString min = USE_CONTROL(*pToolWindow_, wxT("ID_MIN_N"), wxTextCtrl,
         GetValue(), wxT(message_NOT_APPLICABLE) );
   wxString sec = USE_CONTROL(*pToolWindow_, wxT("ID_SEC_N"), wxTextCtrl,
         GetValue(), wxT(message_NOT_APPLICABLE) );
   wxString sectemp = sec;
   // Obtengo las coordenadas cargadas en control
   bool returnvalue = GetCoordinateFromStrings(message_LATITUDE, degree, min, sec,
                                               newcoordinate.y_);

   // Modifico el signo segun N o S este seleccionado
   if (!north_ && degree.Find(wxT("-")) == wxNOT_FOUND) {
      newcoordinate.y_ = -newcoordinate.y_;
   }

   // -------------- Para este -----------------

   degree = USE_CONTROL(*pToolWindow_, wxT("ID_DEGREE_E"), wxTextCtrl,
         GetValue(), wxT(message_NOT_APPLICABLE) );
   min = USE_CONTROL(*pToolWindow_, wxT("ID_MIN_E"), wxTextCtrl,
         GetValue(), wxT(message_NOT_APPLICABLE) );
   sec = USE_CONTROL(*pToolWindow_, wxT("ID_SEC_E"), wxTextCtrl,
         GetValue(), wxT(message_NOT_APPLICABLE) );
   sectemp = sec;
   // Obtengo las coordenadas cargadas en control
   returnvalue = returnvalue
         && GetCoordinateFromStrings(message_LONGITUDE, degree, min, sec,
                                     newcoordinate.x_);

   if (!east_ && degree.Find(wxT("-")) == wxNOT_FOUND) {
      newcoordinate.x_ = -newcoordinate.x_;
   }

   if (returnvalue)
      ValidateAndUpdateCoordinates(LatLongCoordinate, newcoordinate, latLongCoordinate_);
   return returnvalue;
}

/**
 * Metodo auxiliar que se encarga de
 * validar la nueva coordenada y actualizar
 * el valor de la coordenada guardada
 * @param NewCoordinate
 * @param LatLongCoordinate
 * @return
 */
bool LatitudeLongitudeInputWidget::ValidateAndUpdateCoordinates(
      Coordinates& LatLongCoordinate, Coordinates& NewCoordinate,
      Coordinates& ReferenceCoordinate) {
   bool returnvalue = true;
// Verifico que longitud entre -90 y 90 y longitud entre -180 y 180
   if (ValidateCoordinates(NewCoordinate)) {
      LatLongCoordinate = NewCoordinate;
   } else if (returnvalue) {
      returnvalue = false;
      SHOW_STATUS(message_COORDINATE_OUT_OF_RANGE);
   }

   // Si las coordenadas ingresadas son validas.
   if (returnvalue) {
      // Muestro status vacio
      SHOW_STATUS("");

      // Veo si cambio la latitud
      if (!EqualInControlPrecision(ReferenceCoordinate.y_, LatLongCoordinate.y_)) {
         ReferenceCoordinate.y_ = LatLongCoordinate.y_;
      }

      // Veo si cambio la longitud
      if (!EqualInControlPrecision(ReferenceCoordinate.x_, LatLongCoordinate.x_)) {
         ReferenceCoordinate.x_ = LatLongCoordinate.x_;
      }

      // Si no cambiaron retorno el valor con toda la precision.
      LatLongCoordinate = ReferenceCoordinate;
   }
   return returnvalue;
}

/**
 * Si el parametro es valido actualiza la coordenada interna y la despliega
 * @param[in] LatLongCoord nueva coordenada en sistema latitud longitud
 */
bool LatitudeLongitudeInputWidget::SetCoordinate(Coordinates LatLongCoord) {
   bool returnvalue = false;
   if (ValidateCoordinates(LatLongCoord)) {
      returnvalue = true;
      latLongCoordinate_ = LatLongCoord;
      if (pToolWindow_)
         DisplayCoordinates();
   }
   return returnvalue;
}

/**
 * Transforma texto en formato grados, minutos y segundos a double.
 * Si hay un error en los valores o en el simbolo usado para separar decimales
 * retorna false. Tambien imprime en status bar un mensaje con lo que fallo.
 * Solo considera el signo que obtiene de control grados(no utiliza controles
 * n/s o e/o)
 * @param[in] Coordinate string que identifica el sentido de desplazamiento
 * @param[in] Degree string con texto ingresado por usr para grados
 * @param[in] Minute string con texto ingresado por usr para minutos
 * @param[in] Second string con texto ingresado por usr para segundos
 * @param[out] Second string con cant valores despues de la coma correctos
 * @param[out] Value valor obtenido
 */
bool LatitudeLongitudeInputWidget::GetCoordinateFromStrings(
      const std::string &Coordinate, const wxString &Degree, const wxString &Minute,
      const wxString &Second, double &Value) {
   wxString second = Second;
   double textasdouble = 0;
   long textaslong = 0;
   bool returnvalue = true;
   // Transformo grados
   if (!Degree.ToDouble(&textasdouble) || !Degree.ToLong(&textaslong)) {
      SHOW_STATUS(message_DEGREE_ERROR_s, Coordinate.c_str());
      returnvalue = false;
   }
   Value = textasdouble;
   textasdouble = 0;
   // Transformo minutos. Deben estar entre 0(-60 si grados = 0) y 60.
   if (!Minute.ToDouble(&textasdouble) || !Minute.ToLong(&textaslong)
         || textasdouble >= 60 || textasdouble < 0) {
      SHOW_STATUS(message_MINUTE_ERROR_s, Coordinate.c_str());
      returnvalue = false;
   }
   // Si los min son negativos invierto el signo de los minutos
   if (Value >= 0) {
      Value += textasdouble / 60;
   } else {
      Value -= textasdouble / 60;
   }
   textasdouble = 0;

   // Modifico los seg para que tengan separador correcto
   second.Replace(wxT(","), wxT("."));
   if (!Second.ToDouble(&textasdouble) || (textasdouble >= 60) || textasdouble < 0) {
      SHOW_STATUS(message_SECOND_ERROR_s, Coordinate.c_str());
      returnvalue = false;
   }

   // Si tiene decimales de mas despues de la coma los elimino
   if (returnvalue && second.Find(wxT(".")) != wxNOT_FOUND
         && (second.size() - 1 - second.Find(wxT("."))) > 2) {
      second.Truncate(second.Find(wxT(".")) + 3);
      second.ToDouble(&textasdouble);
   }

   // Si los grados y min son negativos invierto el signo de los segundos
   if (Value >= 0) {
      Value += textasdouble / 3600;
   } else {
      Value -= textasdouble / 3600;
   }

   return returnvalue;
}

/**
 * Transforma texto en formato grados, minutos y segundos a double.
 * Si hay un error en los valores o en el simbolo usado para separar decimales
 * retorna false. Tambien imprime en status bar un mensaje con lo que fallo.
 * @param[in] Coordinate string que identifica el sentido de desplazamiento
 * @param[out] Degree string con texto para grados
 * @param[out] Minute string con texto para minutos
 * @param[out] Second string con texto para segundos
 */
bool LatitudeLongitudeInputWidget::GetStringFromCoordinate(std::string &Degree,
                                                           std::string &Minute,
                                                           std::string &Second,
                                                           double Coordinate) {
   double coordinate = std::abs(Coordinate);

   int degrees = SURI_TRUNC(int, coordinate);
   int minutes = SURI_TRUNC(int, (coordinate - degrees)*60);
   double seconds = SURI_ROUND(int, ( (coordinate - degrees)*60 - minutes)*60*100);
   seconds = seconds / 100;
   // Como redondeo puede ser que segundos = 60, en este caso aumento un minuto
   if (seconds == 60) {
      seconds = 0;
      minutes = minutes + 1;
      if (minutes == 60) {
         minutes = 0;
         degrees = degrees + 1;
      }
   }
   // Cargo signo de grados
   Degree = "";
   if (Coordinate < 0) {
      Degree = "-";
   }

   // Cargo valores a stringstrams
   std::stringstream degreestream;
   std::stringstream minutestream;
   std::stringstream secondstream;
   degreestream << degrees;
   minutestream << minutes;
   secondstream << std::fixed << seconds;

   // Los guardo en variables
   Degree += degreestream.str();
   Minute = minutestream.str();
   Second = secondstream.str();

   return true;
}

/**
 * Objetivo: Devuelve el icono de propiedades
 * Por ahora no tiene en cuenta el X e Y dado que devuelve el de 32x32 pixels
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho del icono de la ventana
 * @param[in] Y alto del icono de la ventana
 */
void LatitudeLongitudeInputWidget::GetWindowIcon(wxBitmap& ToolBitmap, int X,
                                                 int Y) const {
   GET_BITMAP_RESOURCE(icon_NAVIGATION_WIDGET, ToolBitmap);
}

/**
 * cambia el valor de norte/sur cuando hace click sobre hipervinculo
 * @param[in] Event evento click sobre el hipervinculo
 */
void LatitudeLongitudeInputWidget::OnLatitudeHyperlink(wxCommandEvent &Event) {
   Event.Skip(false);
   wxHyperlinkCtrl *phyperlink = XRCCTRL(*pToolWindow_,
         wxT("ID_LATITUDEHYPERLINK"), wxHyperlinkCtrl);
   if (!phyperlink) {
      return;
   }
   if (north_) {
      phyperlink->SetLabel(_(label_SOUTH));
   } else {
      phyperlink->SetLabel(_(label_NORTH));
   }
   north_ = !north_;
   phyperlink->SetVisitedColour(phyperlink->GetNormalColour());
}

/**
 * cambia el valor de este/oeste cuando hace click sobre hipervinculo
 * @param[in] Event evento click sobre el hipervinculo
 *
 */
void LatitudeLongitudeInputWidget::OnLongitudeHyperlink(wxCommandEvent &Event) {
   Event.Skip(false);
   wxHyperlinkCtrl *phyperlink = XRCCTRL(*pToolWindow_,
         wxT("ID_LONGITUDEHYPERLINK"), wxHyperlinkCtrl);
   if (!phyperlink) {
      return;
   }
   if (east_) {
      phyperlink->SetLabel(_(label_WEST));
   } else {
      phyperlink->SetLabel(_(label_EAST));
   }
   east_ = !east_;
   phyperlink->SetVisitedColour(phyperlink->GetNormalColour());
}

/**
 * Se actualiza la variable interna llamando a GetCoordinate y luego
 * se despliegan los cambios. De esta forma se evita que el
 * control tenga un valor invalido.
 * @param[in] Event evento que se genera cuando un control de ingreso de datos
 *                   pierde el foco.
 */
void LatitudeLongitudeInputWidget::OnLostFocus(wxFocusEvent& Event) {
   Coordinates LatLongCoord;
   // Dentro de coordinate se invierte el label cuando es necesario
   GetCoordinate(LatLongCoord);
   DisplayCoordinates();
}

/**
 *	Actualiza los controles e/o y s/n para que coincidan con el signo de la
 *	coordenada interna. Si encuentra un '-' en control de grados lo elimina
 * @param[in] Coordinate coordenada ingresada en control
 * @param[in] IsPositive informa si la direccion de desplazamiento es en
 * sentido positivo, por ejemplo, norte = true, sur = false
 * @param[out] IsPositive cambia el signo si Coordinate es negativo y IsPositive
 * es positivo.
 * @param[in] pControlIds vector con los strings que identifican a los controles
 * en orden: id_hipervinculo, id_textctrl_grados y por ultimo el texto que
 * deberia mostrar el hipervinculo en caso de ser necesario invertirlo.
 */
void LatitudeLongitudeInputWidget::UpdateControlSign(double Coordinate,
                                                     bool &IsPositive,
                                                     const char *pControlIds[]) {
   wxString ctrlvalue = USE_CONTROL(*pToolWindow_, pControlIds[DEGREE_INDEX],
         wxTextCtrl, GetValue(), wxT(message_NOT_APPLICABLE) );

   // Si coordenadas negativas cambio a signo.
   if (Coordinate < 0 && IsPositive) {
      IsPositive = false;
      GET_CONTROL(*pToolWindow_, pControlIds[HYPERLINK_INDEX], wxHyperlinkCtrl)
         ->SetLabel(_(pControlIds[NEGATIVE_LABEL_INDEX]));
      GET_CONTROL(*pToolWindow_, pControlIds[HYPERLINK_INDEX], wxHyperlinkCtrl)
         ->Refresh();
   }
   // Si coordenadas negativas cambio a signo.
   if (Coordinate > 0 && !IsPositive) {
      IsPositive = true;
      GET_CONTROL(*pToolWindow_, pControlIds[HYPERLINK_INDEX], wxHyperlinkCtrl)
         ->SetLabel(_(pControlIds[POSITIVE_LABEL_INDEX]));
      GET_CONTROL(*pToolWindow_, pControlIds[HYPERLINK_INDEX], wxHyperlinkCtrl)
         ->Refresh();
   }

   if (ctrlvalue.Contains("-")) {
      ctrlvalue.Replace(wxT("-"), wxT(""));
      GET_CONTROL(*pToolWindow_, pControlIds[DEGREE_INDEX], wxTextCtrl)
         ->SetValue(ctrlvalue);
   }
}

/**
 * Despliega la coordenada interna en los controles. Solo muestra la cantidad
 * de digitos que permite el control.
 */
void LatitudeLongitudeInputWidget::DisplayCoordinates() {
   std::string degree;
   std::string minute;
   std::string second;
   wxString tempdegree;
   // Cargo coordenadas de longitud
   GetStringFromCoordinate(degree, minute, second, latLongCoordinate_.x_);
   tempdegree = degree.c_str();
   tempdegree.Replace("-", "");
   GET_CONTROL(*pToolWindow_, wxT("ID_DEGREE_E"), wxTextCtrl)
      ->SetValue(tempdegree.c_str());
   GET_CONTROL(*pToolWindow_, wxT("ID_MIN_E"), wxTextCtrl)->SetValue(minute);
   GET_CONTROL(*pToolWindow_, wxT("ID_SEC_E"), wxTextCtrl)->SetValue(second);

   // Cargo coordenadas de latitud
   GetStringFromCoordinate(degree, minute, second, latLongCoordinate_.y_);
   tempdegree = degree.c_str();
   tempdegree.Replace("-", "");
   GET_CONTROL(*pToolWindow_, wxT("ID_DEGREE_N"), wxTextCtrl)
      ->SetValue(tempdegree.c_str());
   GET_CONTROL(*pToolWindow_, wxT("ID_MIN_N"), wxTextCtrl)->SetValue(minute);
   GET_CONTROL(*pToolWindow_, wxT("ID_SEC_N"), wxTextCtrl)->SetValue(second);

   // Actualizo signos de control
   UpdateControlSign(latLongCoordinate_.x_, east_, pLongitudeControlIds);
   UpdateControlSign(latLongCoordinate_.y_, north_, pLatitudeControlIds);
}

/**
 * Analiza si la coordenada interna es valida
 * @param[in] LatLongCoord coordenada a validar.
 */
bool LatitudeLongitudeInputWidget::ValidateCoordinates(
      const Coordinates &LatLongCoord) {
   // Verifico que longitud entre -90 y 90 y que longitud este entre -180 y 180
   return !validate_
         || (LatLongCoord.y_ > -90 && LatLongCoord.y_ < 90 && LatLongCoord.x_ > -180
               && LatLongCoord.x_ < 180);
}

/** Verifica si las coordenadas son iguales en formato texto */
bool LatitudeLongitudeInputWidget::EqualInControlPrecision(double Coord1,
                                                           double Coord2) {
   std::string degree1, minute1, second1;
   GetStringFromCoordinate(degree1, minute1, second1, Coord1);

   std::string degree2, minute2, second2;
   GetStringFromCoordinate(degree2, minute2, second2, Coord2);

   return degree1 == degree2 && minute1 == minute2 && second1 == second2;
}
}

