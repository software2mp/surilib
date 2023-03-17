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

// Includes standard
#include <string>
#include <limits>
#include <vector>

// Includes Suri
#include "suri/AuxiliaryFunctions.h"
#include "RasterOutputSizePart.h"
#include "resources.h"
#include "SpatialReference.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/TextFileGcpLoader.h"

// Includes Wx
#include "wx/radiobut.h"

// Includes App

// Defines
#define LINES_TEXTCRTL_ID "ID_OUTPUT_LINE_COUNT_TEXT"
#define PIXEL_TEXTCTRL_ID "ID_OUTPUT_PIXEL_COUNT_TEXT"
#define TIMES_X_TEXTCTRL_ID "ID_OUTPUT_X_TIMES_TEXT"
#define TIMES_Y_TEXTCTRL_ID "ID_OUTPUT_Y_TIMES_TEXT"
#define PIXEL_SIZE_X_OUT_TEXTCTRL_ID "ID_OUTPUT_PIXEL_X_SIZE_TEXT"
#define PIXEL_SIZE_Y_OUT_TEXTCTRL_ID "ID_OUTPUT_PIXEL_Y_SIZE_TEXT"
#define ID_RASTER_SIZE_RADIO "ID_RASTER_SIZE_RADIO"
#define ID_INPUT_PIXEL_RATIO_RADIO "ID_INPUT_PIXEL_RATIO_RADIO"
#define ID_OUTPUT_PIXEL_SIZE_RADIO "ID_OUTPUT_PIXEL_SIZE_RADIO"
#define ID_OUTPUT_SR_UNITS_X "ID_OUTPUT_SR_UNITS_X"
#define ID_OUTPUT_SR_UNITS_Y "ID_OUTPUT_SR_UNITS_Y"
#define ID_KEEP_PIXEL_ASPECT_RATIO_CHECK "ID_KEEP_PIXEL_ASPECT_RATIO_CHECK"
#define CENTER_DELTA 0.5

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ParametersChangedEvent, RasterOutputSizePart)
      IMPLEMENT_EVENT_CALLBACK(OnParametersChangedEventHandler,
      OnParametersChangedEventHandler(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(RadioButtonEvent, RasterOutputSizePart)
      IMPLEMENT_EVENT_CALLBACK(OnRadioButtonEventHandler,
      OnRadioButtonEventHandler(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(AspectCheckEvent, RasterOutputSizePart)
      IMPLEMENT_EVENT_CALLBACK(OnAspectCheckEventHandler, OnAspectCheckEventHandler(event),
                               wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT

/** Ctor */
RasterOutputSizePart::RasterOutputSizePart(
      ReprojectionProcess::ReprojectionParameters *pReprojectionParameters,
      World* pInputWorld, World* pOutputWorld, bool Enable, bool Modified) :
      Part(Enable, Modified), NEW_EVENT_OBJECT(ParametersChangedEvent),
      pRadioButtonEvent_(new RadioButtonEvent(this)),
      pAspectCheckEvent_(new AspectCheckEvent(this)),
      pReprojectionParameters_(pReprojectionParameters), pInputWorld_(pInputWorld),
      pOutputWorld_(pOutputWorld),
      preserveAspect_(true), lastPreserveAspect_(preserveAspect_),
      centerPixelSizeIn_(Coordinates(0, 0), Coordinates(0, 0)),
      centerPixelSizeOut_(Coordinates(0, 0), Coordinates(0, 0)),
      pPixelsText_(NULL), pLinesText_(NULL), pPixelSizeXText_(NULL), pPixelSizeYText_(NULL),
      pTimesXText_(NULL), pTimesYText_(NULL),
      pixelLineActive_(false), pixelSizeActive_(false), timesActive_(true),
      lastPixelLineActive_(false), lastPixelSizeActive_(false), lastTimesActive_(true),
      validData_(true), pixelSizeChanged_(false) {
   windowTitle_ = _(caption_RASTER_OUTPUT_SIZE_PART);
   currentSrs_ = "";
}

/** Dtor */
RasterOutputSizePart::~RasterOutputSizePart() {
   DELETE_EVENT_OBJECT;
   delete pRadioButtonEvent_;
   delete pAspectCheckEvent_;
}

/** Retorna el icono de la herramienta */
void RasterOutputSizePart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_RASTER_OUTPUT_SIZE, ToolBitmap);
}

/** Indica si el Part tiene cambios para salvar. */
bool RasterOutputSizePart::HasChanged() {
   Subset palsubset;
   pPal_->GetAttribute<Subset>(ProcessAdaptLayer::OutputSubsetWindowKeyAttr, palsubset);
   if (subsetOut_.lr_ != palsubset.lr_ || subsetOut_.ul_ != palsubset.ul_) {
      subsetIn_.lr_ = palsubset.lr_;
      subsetIn_.ul_ = palsubset.ul_;
      SetInitialValues();
   }
   return modified_;
}

/** Devuelve si la parte tiene datos validos */
bool RasterOutputSizePart::HasValidData() {
   return validData_;
}

/** Salva los cambios realizados en el Part. */
bool RasterOutputSizePart::CommitChanges() {
   InputRasterExtentUpdate();
   SetupOutputWorld();
   return true;
}

/** Restaura los valores originales del Part. */
bool RasterOutputSizePart::RollbackChanges() {
   if (pixelLineActive_) {
      CalculateValuesByPixelLines(pixels_, lines_,
                                  pixelSizeOutX_, pixelSizeOutY_,
                                  timesX_, timesY_);
   } else if (pixelSizeActive_) {
      CalculateValuesByPixelSize(pixelSizeOutX_, pixelSizeOutY_,
                                 pixels_, lines_, timesX_,
                                 timesY_);
   } else if (timesActive_) {
      CalculateValuesByTimesFactor(timesX_, timesY_, pixels_,
                                   lines_, pixelSizeOutX_,
                                   pixelSizeOutY_);
   }
   UpdateGuiControls(pixels_, lines_, pixelSizeOutX_,
                     pixelSizeOutY_, timesX_, timesY_, true);
   RestoreValues();
   InputRasterExtentUpdate();
   SanitizeOutputSize();
   modified_ = false;
   SetupOutputWorld();
   return true;
}

/** Metodo auxiliar que restaura los valores**/
void RasterOutputSizePart::RestoreValues() {
   pixelsLastValue_ = pixels_;
   linesLastValue_ = lines_;
   pixelSizeOutXLastValue_ = pixelSizeOutX_;
   pixelSizeOutYLastValue_ = pixelSizeOutY_;
   timesXLastValue_ = timesX_;
   timesYLastValue_ = timesY_;
   wxRadioButton* ppixellineradio = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                            ID_RASTER_SIZE_RADIO, wxRadioButton);
   wxRadioButton* pxyradiobutton = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                           ID_OUTPUT_PIXEL_SIZE_RADIO, wxRadioButton);
   wxRadioButton* ptimesradiobutton = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                              ID_INPUT_PIXEL_RATIO_RADIO,
                                              wxRadioButton);
   wxCheckBox* paspectcheck = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                      ID_KEEP_PIXEL_ASPECT_RATIO_CHECK, wxCheckBox);
   lastPixelLineActive_ = pixelLineActive_;
   lastPixelSizeActive_ = pixelSizeActive_;
   lastTimesActive_ = timesActive_;
   lastPreserveAspect_ = preserveAspect_;

   paspectcheck->SetValue(preserveAspect_);
   ppixellineradio->SetValue(pixelLineActive_);
   pxyradiobutton->SetValue(pixelSizeActive_);
   ptimesradiobutton->SetValue(timesActive_);
}


/** Inicializa el part */
void RasterOutputSizePart::SetInitialValues() {
   if (!pReprojectionParameters_->pTranform_) {
      pReprojectionParameters_->pTranform_ = getCoordinatesTransfomation(
            pReprojectionParameters_);
   }
   /** Supongo que inicialmente el times es el factor fijo (es el radio button
    *  que se encuentra activado por defecto) **/
   timesX_ = 1;
   timesY_ = 1;

   CalculateValuesByTimesFactor(timesX_, timesY_, pixels_,
                                lines_, pixelSizeOutX_,
                                pixelSizeOutY_);

   UpdateGuiControls(pixels_, lines_, pixelSizeOutX_,
                     pixelSizeOutY_, timesX_, timesY_, true);

   pixelsLastValue_ = pixels_;
   linesLastValue_ = lines_;
   pixelSizeOutXLastValue_ = pixelSizeOutX_;
   pixelSizeOutYLastValue_ = pixelSizeOutY_;
   timesXLastValue_ = timesX_;
   timesYLastValue_ = timesY_;
   pPal_->AddAttribute<int>(ProcessAdaptLayer::OutputSizePixelsKeyAttr, pixels_);
   pPal_->AddAttribute<int>(ProcessAdaptLayer::OutputSizeLinesKeyAttr, lines_);
   pPal_->AddAttribute<double>(ProcessAdaptLayer::OutputSizePixelWidthKeyAttr,
                             pixelSizeOutX_);
   pPal_->AddAttribute<double>(ProcessAdaptLayer::OutputSizePixelHeightKeyAttr,
                                  pixelSizeOutY_);
   pPal_->AddSerializableAttribute<Subset>(
         ProcessAdaptLayer::OutputSubsetWindowKeyAttr, subsetOut_);
   modified_ = true;
}

/**
 * Metodo auxiliar que dado VecesX y Veces Y calcula los demas parametros
 * @param[in] TimesX
 * @param[in] TimesY
 * @param[out] Pixels
 * @param[out] Lines
 * @param[out] PixelSizeX
 * @param[out] PixelSizeY
 */
void RasterOutputSizePart::CalculateValuesByTimesFactor(double TimesX, double TimesY,
                                                        int& Pixels, int& Lines,
                                                        double& PixelSizeX,
                                                        double& PixelSizeY) {
   pPal_->GetAttribute<Subset>(ProcessAdaptLayer::OutputSubsetWindowKeyAttr, subsetOut_);
   CalculateCenterDimension();
   // Calculo el factor en x e y de la trasnformacion. (relacion unidades)
   PixelSizeX = (1 / TimesX) * centerPixelSizeOut_.GetWidth();
   PixelSizeY = (1 / TimesY) * centerPixelSizeOut_.GetHeight();
   if (pReprojectionParameters_->pTranform_ != NULL)
      pReprojectionParameters_->pTranform_->Transform(subsetOut_);
   Dimension dimout(subsetOut_);
   Pixels = SURI_TRUNC(int, SURI_CEIL(dimout.GetWidth() / PixelSizeX));
   Lines = SURI_TRUNC(int, SURI_CEIL(dimout.GetHeight() / PixelSizeY));
   RasterSpatialModel::Destroy(pReprojectionParameters_->pRasterSpatialModelOut_);
   pReprojectionParameters_->pRasterSpatialModelOut_ = CreateRasterSpatialModel(
         subsetOut_, Pixels, Lines);
}

/**
* Metodo auxiliar que dado Pixels y Lines calcula los demas parametros
* @param[in] Pixels
* @param[in] Lines
* @param[out] PixelSizeX
* @param[out] PixelSizeY
* @param[out] TimesX
* @param[out] TimesY
*/
void RasterOutputSizePart::CalculateValuesByPixelLines(int Pixels, int Lines,
                                                       double& PixelSizeX,
                                                       double& PixelSizeY, double& TimesX,
                                                       double& TimesY) {
   pPal_->GetAttribute<Subset>(ProcessAdaptLayer::OutputSubsetWindowKeyAttr, subsetOut_);
   RasterSpatialModel::Destroy(pReprojectionParameters_->pRasterSpatialModelOut_);
   pReprojectionParameters_->pRasterSpatialModelOut_ = CreateRasterSpatialModel(
         subsetOut_, Pixels, Lines);
   CalculateCenterDimension();
   Dimension dimensionout(subsetOut_);
   PixelSizeX = dimensionout.GetWidth() / Pixels;
   PixelSizeY = dimensionout.GetHeight() / Lines;
   TimesX = centerPixelSizeOut_.GetWidth() / PixelSizeX;
   TimesY = centerPixelSizeOut_.GetHeight() / PixelSizeY;
}

/**
 *
 * @param PixelSizeX
 * @param PixelSizeY
 * @param Pixels
 * @param Lines
 * @param TimesX
 * @param TimesY
 */
void RasterOutputSizePart::CalculateValuesByPixelSize(double PixelSizeX,
                                                      double PixelSizeY, int& Pixels,
                                                      int& Lines, double& TimesX,
                                                      double &TimesY) {
   pPal_->GetAttribute<Subset>(ProcessAdaptLayer::OutputSubsetWindowKeyAttr, subsetOut_);
   Dimension dimensionout(subsetOut_);
   Pixels = SURI_TRUNC(int, SURI_CEIL(dimensionout.GetWidth() / PixelSizeX));
   Lines = SURI_TRUNC(int, SURI_CEIL(dimensionout.GetHeight() / PixelSizeY));
   RasterSpatialModel::Destroy(pReprojectionParameters_->pRasterSpatialModelOut_);
   pReprojectionParameters_->pRasterSpatialModelOut_ = CreateRasterSpatialModel(
         subsetOut_, Pixels, Lines);
   CalculateCenterDimension();
   TimesX =  centerPixelSizeOut_.GetWidth() / PixelSizeX;
   TimesY =  centerPixelSizeOut_.GetHeight() / PixelSizeY;
}

/** Implementacion del metodo de creacion de ventana de Widget */
bool RasterOutputSizePart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_OUTPUT_SIZE_SELECTOR_PANEL"));
   if (!pToolWindow_)
      return false;

   pPixelsText_ = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                    PIXEL_TEXTCTRL_ID, wxTextCtrl);
   pLinesText_ = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                    LINES_TEXTCRTL_ID, wxTextCtrl);
   pPixelSizeXText_ = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                     PIXEL_SIZE_X_OUT_TEXTCTRL_ID, wxTextCtrl);
   pPixelSizeYText_ = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                     PIXEL_SIZE_Y_OUT_TEXTCTRL_ID, wxTextCtrl);
   pTimesXText_ = XRCCTRL(*(this->GetWidget()->GetWindow()), TIMES_X_TEXTCTRL_ID,
                                 wxTextCtrl);
   pTimesYText_ = XRCCTRL(*(this->GetWidget()->GetWindow()), TIMES_Y_TEXTCTRL_ID,
                                 wxTextCtrl);
   this->enabled_ = (pReprojectionParameters_->pTranform_ != NULL);
   if (!pPixelsText_ || !pLinesText_ || !pPixelSizeXText_ || !pPixelSizeYText_
         || !pTimesXText_ || !pTimesYText_ || !pReprojectionParameters_)
      return false;
   ConfigurePixelSizeUnits();
   SetInitialValues();
   ConnectEvents();
   return true;
}

/**
 * Metodo auxiliar que a partir del Sistema de Referencia de
 * llegada configura las unidades de X-Y
 */
void RasterOutputSizePart::ConfigurePixelSizeUnits() {
   std::string units;
   if (SpatialReference::IsProjected(currentSrs_)) {
      units = SpatialReference::GetProjCoordinateSystemUnits(currentSrs_);
   } else {
      units = SpatialReference::GetGeoCoordinateSystemUnits(currentSrs_);
   }
   wxStaticText* punitsx = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                   ID_OUTPUT_SR_UNITS_X, wxStaticText);
   punitsx->SetLabel(units);
   wxStaticText* punitsy = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                   ID_OUTPUT_SR_UNITS_Y, wxStaticText);
   punitsy->SetLabel(units);
}

/** Metodo auxiliar para conectar eventos en los componentes del part **/
void RasterOutputSizePart::ConnectEvents() {
   // Conecto todos los textctrl al mismo evento.
   std::string textctrlids[] = { PIXEL_TEXTCTRL_ID, LINES_TEXTCRTL_ID,
                                 TIMES_X_TEXTCTRL_ID, TIMES_Y_TEXTCTRL_ID,
                                 PIXEL_SIZE_X_OUT_TEXTCTRL_ID,
                                 PIXEL_SIZE_Y_OUT_TEXTCTRL_ID };
   for (int i = 0; i < 6; ++i) {
      GET_CONTROL(*(this->GetWidget()->GetWindow()),
            textctrlids[i].c_str(), wxTextCtrl)->Connect(
               wxEVT_COMMAND_TEXT_UPDATED,
               wxCommandEventHandler(
                     ParametersChangedEvent::OnParametersChangedEventHandler),
               NULL, pEventHandler_);
   }

   std::string radiobuttonsids[] =
   {ID_RASTER_SIZE_RADIO, ID_OUTPUT_PIXEL_SIZE_RADIO, ID_INPUT_PIXEL_RATIO_RADIO};
   for (int j = 0 ; j < 3 ; ++j) {
      GET_CONTROL(*(this->GetWidget()->GetWindow()),
                  radiobuttonsids[j].c_str(), wxRadioButton)->Connect(
                        wxEVT_COMMAND_RADIOBUTTON_SELECTED,
                     wxCommandEventHandler(
                           RadioButtonEvent::OnRadioButtonEventHandler),
                     NULL, pRadioButtonEvent_);
   }

   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         ID_KEEP_PIXEL_ASPECT_RATIO_CHECK, wxCheckBox)->Connect(
            wxEVT_COMMAND_CHECKBOX_CLICKED,
            wxCommandEventHandler(AspectCheckEvent::OnAspectCheckEventHandler), NULL,
            pAspectCheckEvent_);
}

/** Metodo que maneja el evento de cambio de texto en un TextCtrl*/
void RasterOutputSizePart::OnParametersChangedEventHandler(wxCommandEvent &Event) {
   if (pReprojectionParameters_->pTranform_ == NULL)
      return;

   double pixelsizeoutx, pixelsizeouty, timesx, timesy;
   int pixels, lines;
   // Obtengo valores iniciales
   GetTextValuesFromGui(pixels, lines, pixelsizeoutx, pixelsizeouty, timesx, timesy);
   // para evitar divisiones por 0 o datos invalidos
   if (pixels == 0 || lines == 0 || FLOAT_COMPARE(pixelsizeoutx, 0) ||
         FLOAT_COMPARE(pixelsizeouty, 0) || timesx == 0 || timesy == 0) {
      validData_ = false;
      return;
   } else {
      validData_ = true;
   }

   pixelSizeChanged_ = false;

   // me fijo que textctrl se modifico y calculo los nuevos valores en funcion de eso
   Subset subsetout;
   pPal_->GetAttribute<Subset>(ProcessAdaptLayer::OutputSubsetWindowKeyAttr, subsetout);
   pReprojectionParameters_->pTranform_->Transform(subsetout);
   Dimension dimensionout(subsetout);
   wxRadioButton* ppixellineradio =
         XRCCTRL(*(this->GetWidget()->GetWindow()), ID_RASTER_SIZE_RADIO,
                                            wxRadioButton);
   wxRadioButton* pxyradiobutton =
         XRCCTRL(*(this->GetWidget()->GetWindow()), ID_OUTPUT_PIXEL_SIZE_RADIO,
                                            wxRadioButton);
   wxRadioButton* ptimesradiobutton =
         XRCCTRL(*(this->GetWidget()->GetWindow()), ID_INPUT_PIXEL_RATIO_RADIO,
                                            wxRadioButton);
   if (pPixelsText_->IsModified() || pLinesText_->IsModified()) {
      ppixellineradio->SetValue(true);
      pxyradiobutton->SetValue(false);
      ptimesradiobutton->SetValue(false);
      lastPixelLineActive_ = true;
      lastPixelSizeActive_ = false;
      lastTimesActive_ = false;
      // Cambio los parametros en funcion de si mantienen el aspecto
      double pixelscalculation = (pLinesText_->IsModified() && lastPreserveAspect_) ?
            dimensionout.GetWidth() * (static_cast<double>(lines) /
                                             dimensionout.GetHeight()) : pixels;

      double linescalculation = (pPixelsText_->IsModified() && lastPreserveAspect_) ?
            dimensionout.GetHeight() * (static_cast<double>(pixels) /
                                             dimensionout.GetWidth()) : lines;

      pixels = SURI_TRUNC(int, SURI_CEIL(pixelscalculation));
      lines = SURI_TRUNC(int, SURI_CEIL(linescalculation));
      pixelsLastValue_ = pixels;
      linesLastValue_ = lines;
      CalculateValuesByPixelLines(pixels, lines, pixelSizeOutXLastValue_, pixelSizeOutYLastValue_,
                                  timesXLastValue_, timesYLastValue_);
   } else if (pPixelSizeYText_->IsModified() || pPixelSizeXText_->IsModified()) {
      // Cambio los parametros en funcion de si mantienen el aspecto
      pixelsizeouty = (pPixelSizeXText_->IsModified() && lastPreserveAspect_) ?
            (pixelsizeouty * (pixelsizeoutx / pixelSizeOutXLastValue_)) : pixelsizeouty;
      pixelsizeoutx = (pPixelSizeYText_->IsModified() && lastPreserveAspect_) ?
            (pixelsizeoutx * (pixelsizeouty / pixelSizeOutYLastValue_)) : pixelsizeoutx;
      pixelSizeOutXLastValue_ = pixelsizeoutx;
      pixelSizeOutYLastValue_ = pixelsizeouty;
      ppixellineradio->SetValue(false);
      pxyradiobutton->SetValue(true);
      ptimesradiobutton->SetValue(false);
      lastPixelLineActive_ = false;
      lastPixelSizeActive_ = true;
      lastTimesActive_ = false;
      CalculateValuesByPixelSize(pixelSizeOutXLastValue_, pixelSizeOutYLastValue_,
                                 pixelsLastValue_, linesLastValue_, timesXLastValue_,
                                 timesYLastValue_);

      pixelSizeChanged_ = true;

   } else if (pTimesXText_->IsModified() || pTimesYText_->IsModified()) {
      // Cambio los parametros en funcion de si mantienen el aspecto
      timesYLastValue_ = (pTimesXText_->IsModified() && lastPreserveAspect_) ?
            timesx : timesy;
      timesXLastValue_ = (pTimesYText_->IsModified() && lastPreserveAspect_) ?
            timesy : timesx;

      ppixellineradio->SetValue(false);
      pxyradiobutton->SetValue(false);
      ptimesradiobutton->SetValue(true);
      lastPixelLineActive_ = false;
      lastPixelSizeActive_ = false;
      lastTimesActive_ = true;
      CalculateValuesByTimesFactor(timesXLastValue_, timesYLastValue_, pixelsLastValue_,
                                   linesLastValue_, pixelSizeOutXLastValue_,
                                   pixelSizeOutYLastValue_);
   }
   modified_ = true;
   // Actualizo los textctrl
   UpdateGuiControls(pixelsLastValue_, linesLastValue_, pixelSizeOutXLastValue_,
                     pixelSizeOutYLastValue_, timesXLastValue_, timesYLastValue_);
}


/** Handler para el evento de activacion/desactivacion de un radio button en el Part*/
void RasterOutputSizePart::OnRadioButtonEventHandler(wxCommandEvent &Event) {
   wxRadioButton* ppixellineradio =
         XRCCTRL(*(this->GetWidget()->GetWindow()), ID_RASTER_SIZE_RADIO,
                                            wxRadioButton);
   wxRadioButton* pxyradiobutton =
         XRCCTRL(*(this->GetWidget()->GetWindow()), ID_OUTPUT_PIXEL_SIZE_RADIO,
                                            wxRadioButton);
   wxRadioButton* ptimesradiobutton =
         XRCCTRL(*(this->GetWidget()->GetWindow()), ID_INPUT_PIXEL_RATIO_RADIO,
                                            wxRadioButton);
   lastPixelLineActive_ = ppixellineradio->GetValue();
   lastPixelSizeActive_ = pxyradiobutton->GetValue();
   lastTimesActive_ = ptimesradiobutton->GetValue();
   modified_ = true;
}

/** Handler para el evento del check de conservar aspecto*/
void RasterOutputSizePart::OnAspectCheckEventHandler(wxCommandEvent &Event) {
   wxCheckBox* paspectcheck = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                      ID_KEEP_PIXEL_ASPECT_RATIO_CHECK, wxCheckBox);
   lastPreserveAspect_ = paspectcheck->IsChecked();
   modified_ = true;
}

/**
 * Actualiza los TextCtrol del part en funcion de los valores pasado por parametro
 * @param Pixels
 * @param Lines
 * @param PixelSizeX
 * @param PixelSizeY
 * @param TimesX
 * @param TimesY
 * @param Rollback booleano que indica si la llama proviene de un rollback.
 * En este caso hay que volver a escribir el valor aunque se haya modificado el TextCtrl
 */
void RasterOutputSizePart::UpdateGuiControls(int Pixels, int Lines, double PixelSizeX,
                                             double PixelSizeY, double TimesX,
                                             double TimesY, bool Rollback) {
   /** Se utiliza ChangeValue en vez de SetValue para que no se llame nuevamente
    *  a este evento. Se actualizan los valores menos el que se esta modificando*/
   if (!pPixelsText_->IsModified() || Rollback)
      pPixelsText_->ChangeValue(NumberToString<int>(Pixels));
   if (!pLinesText_->IsModified() || Rollback)
      pLinesText_->ChangeValue(NumberToString<int>(Lines));
   if (!pPixelSizeXText_->IsModified() || Rollback)
      pPixelSizeXText_->ChangeValue(DoubleToString(PixelSizeX));
   if (!pPixelSizeYText_->IsModified() || Rollback)
      pPixelSizeYText_->ChangeValue(DoubleToString(PixelSizeY));
   if (!pTimesXText_->IsModified() || Rollback)
      pTimesXText_->ChangeValue(DoubleToString(TimesX));
   if (!pTimesYText_->IsModified() || Rollback)
      pTimesYText_->ChangeValue(DoubleToString(TimesY));

   pPixelsText_->SetModified(false);
   pLinesText_->SetModified(false);
   pPixelSizeXText_->SetModified(false);
   pPixelSizeYText_->SetModified(false);
   pTimesXText_->SetModified(false);
   pTimesYText_->SetModified(false);
}

/** Metodo auxiliar que obtiene de los textctrls los valores **/
void RasterOutputSizePart::GetTextValuesFromGui(int& Pixels, int& Lines,
                                                double& PixelSizeX, double& PixelSizeY,
                                                double& TimesX, double& TimesY) {
   Pixels = StringToNumber<int>(pPixelsText_->GetValue().c_str());
   Lines = StringToNumber<int>(pLinesText_->GetValue().c_str());
   PixelSizeX = StringToNumber<double>(pPixelSizeXText_->GetValue().c_str());
   PixelSizeY = StringToNumber<double>(pPixelSizeYText_->GetValue().c_str());
   TimesX = StringToNumber<double>(pTimesXText_->GetValue().c_str());
   TimesY = StringToNumber<double>(pTimesYText_->GetValue().c_str());
}

/** Actualiza el estado de la parte */
void RasterOutputSizePart::Update() {
}

CoordinatesTransformation* RasterOutputSizePart::getCoordinatesTransfomation(
      const ReprojectionProcess::ReprojectionParameters *pReproyectionParameters) {
   std::string algorithm = pReproyectionParameters->algorithm_;
   int algorithmOrder = pReproyectionParameters->algorithmOrder_;

   suri::ParameterCollection params;
   params.AddValue<std::string>(suri::TransformationFactory::kParamWktIn,
                                pReproyectionParameters->srWktIn_);
   params.AddValue<std::string>(suri::TransformationFactory::kParamWktOut,
                                pReproyectionParameters->srWktOut_);

   suri::TransformationFactory* pfactory = suri::TransformationFactoryBuilder::Build();

   std::vector<GroundControlPoint> vgcps;

   // Si es una transformacion polinomica primero hay que conseguir
   // un conjunto de GCPs para poder hacer el calculo. Los GCPs se
   // consiguen haciendo una transformacion exacta sobre una cantidad
   // determinada de puntos.
   if (algorithm.compare(suri::TransformationFactory::kPolynomial) == 0
         && pReproyectionParameters->gcpListFileName_.empty()) {
      suri::CoordinatesTransformation* pexact = pfactory->Create(
            suri::TransformationFactory::kExact, params);

      std::vector<Coordinates> coords = Subset::ExtractCoordinates(
            Subset(0, 0, pReproyectionParameters->width_,
                   pReproyectionParameters->height_),
            300, 300);

      for (int ix = 0, len = coords.size(); ix < len; ++ix) {
         Coordinates src = coords[ix];
         Coordinates dest = coords[ix];

         pexact->Transform(src, true);

         vgcps.push_back(GroundControlPoint(src, dest));
      }

      delete pexact;
      pexact = NULL;
   } else if (algorithm.compare(suri::TransformationFactory::kPolynomial) == 0
         && !pReproyectionParameters->gcpListFileName_.empty()) {
      GcpList list;
      TextFileGcpLoader loader;
      loader.Load(pReproyectionParameters->gcpListFileName_, list);
      RasterSpatialModel* psourcerastermodel = RasterSpatialModel::Create(
            list.GetRasterModelSource());
      RasterSpatialModel* pdestinationrastermodel = RasterSpatialModel::Create(
            list.GetRasterModelDestination());
      // Los puntos de control quedan en el srs y no pixel-linea. Ver TCK 3680.
      std::vector<GroundControlPoint> originalgcps = list.CreateAdaptedGcps();
      std::vector<GroundControlPoint>::iterator it = originalgcps.begin();
      for (; it != originalgcps.end(); ++it) {
         Coordinates src = it->GetSource();
         Coordinates dest = it->GetDestination();
         if (psourcerastermodel) psourcerastermodel->Transform(src);
         if (pdestinationrastermodel) pdestinationrastermodel->Transform(dest);
         vgcps.push_back(GroundControlPoint(src, dest));
      }

      RasterSpatialModel::Destroy(psourcerastermodel);
      RasterSpatialModel::Destroy(pdestinationrastermodel);
   }

   params.AddValue<std::vector<GroundControlPoint> >(
         suri::TransformationFactory::kParamGcps, vgcps);

   params.AddValue<int>(suri::TransformationFactory::kParamOrder, algorithmOrder);

   suri::CoordinatesTransformation* pct = pfactory->Create(algorithm, params);

   suri::TransformationFactoryBuilder::Release(pfactory);

   return pct;
}

/** Actualiza los datos en part cuando se modifica subset a exportar */
void RasterOutputSizePart::InputRasterExtentUpdate() {
   Subset newsubset;
   pInputWorld_->GetWindow(newsubset);
   if (subsetIn_ == newsubset && currentSrs_ == pReprojectionParameters_->srWktOut_)
      return;

   subsetIn_ = newsubset;
   currentSrs_ = pReprojectionParameters_->srWktOut_;

   if (pReprojectionParameters_->pTranform_)
      delete pReprojectionParameters_->pTranform_;

   pReprojectionParameters_->pTranform_ = getCoordinatesTransfomation(pReprojectionParameters_);
   if (lastPixelLineActive_)
      CalculateValuesByPixelLines(pixelsLastValue_, linesLastValue_, pixelSizeOutXLastValue_,
                                  pixelSizeOutYLastValue_,
                                  timesXLastValue_, timesYLastValue_);
   if (lastPixelSizeActive_)
      CalculateValuesByPixelSize(pixelSizeOutXLastValue_, pixelSizeOutYLastValue_,
                                 pixelsLastValue_, linesLastValue_, timesXLastValue_,
                                 timesYLastValue_);
   if (lastTimesActive_)
      CalculateValuesByTimesFactor(timesXLastValue_, timesYLastValue_, pixelsLastValue_,
                                   linesLastValue_, pixelSizeOutXLastValue_,
                                   pixelSizeOutYLastValue_);

   modified_ = true;
}

/**
 * Verifica y corrige si es necesario el tamanio de salida (pixel-linea)
 */
void RasterOutputSizePart::SanitizeOutputSize() {
   wxCommandEvent Event;

   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         ID_RASTER_SIZE_RADIO, wxRadioButton)->SetValue(true);
   OnRadioButtonEventHandler(Event);

   wxCheckBox* paspectcheck = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                      ID_KEEP_PIXEL_ASPECT_RATIO_CHECK, wxCheckBox);
   paspectcheck->SetValue(false);
   OnAspectCheckEventHandler(Event);

   pPixelsText_->ChangeValue(NumberToString<int>(pReprojectionParameters_->width_));
   pPixelsText_->SetModified(true);
   OnParametersChangedEventHandler(Event);

   pLinesText_->ChangeValue(NumberToString<int>(pReprojectionParameters_->height_));
   pLinesText_->SetModified(true);
   OnParametersChangedEventHandler(Event);
}

/**
 * Configura el mundo de salida.
 */
void RasterOutputSizePart::SetupOutputWorld() {
   pixels_ = pixelsLastValue_;
   lines_ = linesLastValue_;
   pixelSizeOutX_ = pixelSizeOutXLastValue_;
   pixelSizeOutY_ = pixelSizeOutYLastValue_;
   timesX_ = timesXLastValue_;
   timesY_ = timesYLastValue_;
   pixelLineActive_ = lastPixelLineActive_;
   pixelSizeActive_ = lastPixelSizeActive_;
   timesActive_ = lastTimesActive_;
   preserveAspect_ = lastPreserveAspect_;
   modified_ = false;
   pOutputWorld_->SetSpatialReference(currentSrs_);
   pOutputWorld_->SetWorld(subsetOut_);
   pOutputWorld_->SetWindow(subsetOut_);
   pOutputWorld_->SetViewport(pixels_, lines_);

   pPal_->AddAttribute<int>(ProcessAdaptLayer::OutputSizePixelsKeyAttr, pixels_);
   pPal_->AddAttribute<int>(ProcessAdaptLayer::OutputSizeLinesKeyAttr, lines_);

   if (pixelSizeChanged_) {
      pPal_->AddAttribute<double>(ProcessAdaptLayer::OutputSizePixelWidthKeyAttr,
                                  pixelSizeOutX_);
      pPal_->AddAttribute<double>(ProcessAdaptLayer::OutputSizePixelHeightKeyAttr,
                                  pixelSizeOutY_);
   }

   if (pPal_) {
      pPal_->AddSerializableAttribute<Subset>(ProcessAdaptLayer::OutputSubsetWindowKeyAttr,
                                  subsetOut_);
      pPal_->AddAttribute<World*>(ProcessAdaptLayer::OutputWorldKeyAttr, pOutputWorld_);
   }
}

/** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
void RasterOutputSizePart::Update(NotificationInterface* pNotification) {
   SetInitialValues();
   InputRasterExtentUpdate();
   SetupOutputWorld();
}

/** Metodo auxiliar que crea un modelo raster a partir del subset, pixel y lineas **/
RasterSpatialModel* RasterOutputSizePart::CreateRasterSpatialModel(
      const Subset& SubsetOut, int Pixels, int Lines) const {
   return RasterSpatialModel::Create(Coordinates(0, 0),
                                     Coordinates(Pixels, Lines),
                                     SubsetOut.ul_, SubsetOut.lr_);
}

/**
 *
 * @param SubsetOut
 * @param Pixels
 * @param Lines
 * @return
 */
Dimension RasterOutputSizePart::CalculatePixelSizeOut(RasterSpatialModel* pRasterModel) {
   double pixelsizeoutx = pRasterModel->GetModelParameters().pixelSizes_[0];
   double pixelsizeouty = pRasterModel->GetModelParameters().pixelSizes_[1];
   return Dimension(Coordinates(0, 0), Coordinates(pixelsizeoutx, pixelsizeouty));
}

/** Metodo auxiliar que calcula el tamanio de pixel de entrada **/
void RasterOutputSizePart::CalculateCenterDimension() {
   pInputWorld_->GetWorld(subsetIn_);
   // Con la transformacion creada obtengo el subset de salidas
   double oldcenterx = (subsetIn_.lr_.x_ + subsetIn_.ul_.x_) / 2;
   double oldcentery = (subsetIn_.lr_.y_ + subsetIn_.ul_.y_) / 2;
   if (oldcentery < 0) oldcentery*=-1;
   if (oldcenterx < 0) oldcenterx*=-1;
   Coordinates rastercentercoordinatesin(oldcenterx, oldcentery);
   pReprojectionParameters_->pRasterSpatialModelIn_->InverseTransform(
         rastercentercoordinatesin);
   // Armo un subset para transformar el centro de la salida
   // Se asume que se calcula el factor "veces" a partir de un sebset chico
   // con el centro del subset de entrada.
   Subset centersubsetin(rastercentercoordinatesin.x_ - CENTER_DELTA,
                         rastercentercoordinatesin.y_ - CENTER_DELTA,
                         rastercentercoordinatesin.x_ + CENTER_DELTA,
                         rastercentercoordinatesin.y_ + CENTER_DELTA);
#ifdef __UNUSED_CODE__
   pRasterSpatialModelIn_->Transform(centersubsetin.ul_);
   pRasterSpatialModelIn_->Transform(centersubsetin.lr_);
#else
   pReprojectionParameters_->pRasterSpatialModelIn_->Transform(centersubsetin);
#endif

   // tamanio pixel entrada
   centerPixelSizeIn_ = Dimension(centersubsetin);
   Subset centersubsetout = centersubsetin;

   // Transformo ese subset a el SR de llegada
   if (pReprojectionParameters_->pTranform_ != NULL)
      pReprojectionParameters_->pTranform_->Transform(centersubsetout);

   // Calcular tamanio pixel SR de llegada
   centerPixelSizeOut_ = Dimension(centersubsetout);
}

}  // end namespace suri
