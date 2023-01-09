/*! \file OutputPageConfigPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Suri
#include "OutputPageConfigPart.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/messages.h"
#include "suri/Dimension.h"
#include "Margin.h"

// Includes Wx
#include "wx/choice.h"

// Defines
#define CENTER_DELTA 0.5

namespace suri {

namespace ui {

START_IMPLEMENT_EVENT_OBJECT(OutputPageConfigPartEvent, OutputPageConfigPart)
   IMPLEMENT_EVENT_CALLBACK(OnCheckboxAddTitleSubtititleClicked,
         OnCheckboxAddTitleSubtititleClicked(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnCheckboxAddMarginsClicked,
         OnCheckboxAddMarginsClicked(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnSpinDpiChange, OnSpinDpiChange(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnParametersChangedEventHandler,
                            OnParametersChangedEventHandler(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT

/** Constructor */
OutputPageConfigPart::OutputPageConfigPart(
      ReprojectionProcess::ReprojectionParameters *pReprojectionParameters, World* pInputWorld,
      World* pOutputWorld) : NEW_EVENT_OBJECT(OutputPageConfigPartEvent),
      pReprojectionParameters_(pReprojectionParameters), pInputWorld_(pInputWorld),
      pOutputWorld_(pOutputWorld) {
   windowTitle_ = _(caption_OUTPUT_PAGE_CONFIG);
}

/** Destructor */
OutputPageConfigPart::~OutputPageConfigPart() {
   DELETE_EVENT_OBJECT;
}

/** Implementacion del metodo de creacion de ventana de Widget */
bool OutputPageConfigPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_, "ID_OUTPUTPAGECONFIG_PANEL");
   pTopText_ = XRCCTRL(*(this->GetWidget()->GetWindow()), "ID_TOP_TEXTCTRL", wxTextCtrl);
   pLeftText_ = XRCCTRL(*(this->GetWidget()->GetWindow()), "ID_LEFT_TEXTCTRL", wxTextCtrl);
   pBottomText_ = XRCCTRL(*(this->GetWidget()->GetWindow()), "ID_BOTTOM_TEXTCTRL", wxTextCtrl);
   pRightText_ = XRCCTRL(*(this->GetWidget()->GetWindow()), "ID_RIGHT_TEXTCTRL", wxTextCtrl);
   pTitleText_ = XRCCTRL(*(this->GetWidget()->GetWindow()), "ID_TITLE_TEXTCTRL", wxTextCtrl);
   pSubtitleText_ = XRCCTRL(*(this->GetWidget()->GetWindow()), "ID_SUBTITLE_TEXTCTRL", wxTextCtrl);
   pWidthText_ = XRCCTRL(*(this->GetWidget()->GetWindow()), "ID_WIDTH_TEXTCTRL", wxTextCtrl);
   pHeightText_ = XRCCTRL(*(this->GetWidget()->GetWindow()), "ID_HEIGHT_TEXTCTRL", wxTextCtrl);
   pTitleCheck_ = XRCCTRL(*(this->GetWidget()->GetWindow()), "ID_ADD_TITLESUBTITLE_CHECKBOX", wxCheckBox);
   pMarginsCheck_ = XRCCTRL(*(this->GetWidget()->GetWindow()), "ID_ADD_MARGINS_CHECKBOX", wxCheckBox);
   pDpiSpin_ = XRCCTRL(*(this->GetWidget()->GetWindow()), "ID_DPI_SPINCTRL", wxSpinCtrl);
   ConnectEvents();
   return true;
}

/** Metodo auxiliar para conectar eventos en los componentes del part */
void OutputPageConfigPart::ConnectEvents() {
   // Conecto todos los textctrl al mismo evento.
   GET_CONTROL(*pToolWindow_, "ID_ADD_TITLESUBTITLE_CHECKBOX", wxCheckBox)->Connect(
         wxEVT_COMMAND_CHECKBOX_CLICKED,
         wxCommandEventHandler(OutputPageConfigPartEvent::OnCheckboxAddTitleSubtititleClicked),
         NULL, pEventHandler_);

   GET_CONTROL(*pToolWindow_, "ID_ADD_MARGINS_CHECKBOX", wxCheckBox)->Connect(
         wxEVT_COMMAND_CHECKBOX_CLICKED,
         wxCommandEventHandler(OutputPageConfigPartEvent::OnCheckboxAddMarginsClicked),
         NULL, pEventHandler_);

   std::string textctrlids[] = {"ID_WIDTH_TEXTCTRL", "ID_HEIGHT_TEXTCTRL", "ID_TOP_TEXTCTRL",
                                "ID_LEFT_TEXTCTRL", "ID_BOTTOM_TEXTCTRL",  "ID_RIGHT_TEXTCTRL"};

   for (int i = 0; i < 6; ++i) {
      GET_CONTROL(*pToolWindow_, textctrlids[i].c_str(), wxTextCtrl)->Connect(
            wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(
                  OutputPageConfigPartEvent::OnParametersChangedEventHandler),
            NULL, pEventHandler_);
   }

   pDpiSpin_->Connect(wxEVT_COMMAND_SPINCTRL_UPDATED,
                      wxCommandEventHandler(OutputPageConfigPartEvent::OnSpinDpiChange), NULL,
                      pEventHandler_);
}

/** Handler para manejar el evento de cambio de estado del checkbox de titulo */
void OutputPageConfigPart::OnCheckboxAddTitleSubtititleClicked(wxCommandEvent &Event) {
   if (pTitleCheck_->GetValue()) {
      pTitleText_->Enable(true);
      pSubtitleText_->Enable(true);
      if (!pMarginsCheck_->GetValue())
         SetMarginControlsStatus();
   } else {
      pTitleText_->Enable(false);
      pSubtitleText_->Enable(false);
   }
   modified_ = true;
}

/** Handler para manejar el evento de cambio de estado del checkbox de margenes */
void OutputPageConfigPart::OnCheckboxAddMarginsClicked(wxCommandEvent &Event) {
   SetMarginControlsStatus();
   modified_ = true;
}

/** Metodo auxiliar que setea el estado de los TextCtrl de los margenes */
void OutputPageConfigPart::SetMarginControlsStatus() {
   pMarginsCheck_->SetValue(!pBottomText_->IsEnabled());
   pBottomText_->Enable(!pBottomText_->IsEnabled());
   pTopText_->Enable(!pTopText_->IsEnabled());
   pLeftText_->Enable(!pLeftText_->IsEnabled());
   pRightText_->Enable(!pRightText_->IsEnabled());
}

/** Recalcula la dimension del output en funcion del nuevo valor */
void OutputPageConfigPart::OnSpinDpiChange(wxCommandEvent &Event) {
   dpi_ = pDpiSpin_->GetValue();
   double width = 0, height = 0;
   CalculateWidthHeightDimensions(width, height);
   SetDimensions(width, height);
   modified_ = true;
}

/** Handler para manejar el evento de cambio de algun parametro en los TextCtrl del Part */
void OutputPageConfigPart::OnParametersChangedEventHandler(wxCommandEvent &Event) {
   double width = StringToNumber<double>(pWidthText_->GetValue().c_str());
   double height = StringToNumber<double>(pHeightText_->GetValue().c_str());
   double pixels = 0, lines = 0;
   if (pWidthText_->IsModified() || pWidthText_->IsModified()) {
      modified_ = true;
      CalculatePixelLineDimensions(pixels, lines, width, height);
   }

   if (pTopText_->IsModified() || pBottomText_->IsModified()) {
      double topmargin = StringToNumber<double>(pTopText_->GetValue().c_str());
      double bottommargin = StringToNumber<double>(pBottomText_->GetValue().c_str());
      modified_ = true;
      height = height - (topmargin + bottommargin);
      CalculatePixelLineDimensions(pixels_, lines_, width, height);
      pTopText_->SetModified(false);
      pBottomText_->SetModified(false);
   }

   if (pLeftText_->IsModified() || pRightText_->IsModified()) {
      double leftmargin = StringToNumber<double>(pLeftText_->GetValue().c_str());
      double rightmargin = StringToNumber<double>(pRightText_->GetValue().c_str());
      modified_ = true;
      width = width - (leftmargin + rightmargin);
      CalculatePixelLineDimensions(pixels_, lines_, width, height);
      pLeftText_->SetModified(false);
      pRightText_->SetModified(false);
   }
}

/**
 * Indica si el Part tiene cambios para salvar.
 * @return bool que indica si tuvo exito
 */
bool OutputPageConfigPart::HasChanged() {
   return modified_;
}

/** Salva los cambios realizados en el Part. */
bool OutputPageConfigPart::CommitChanges() {
   if (pTitleCheck_->IsChecked() && !pMarginsCheck_->IsChecked()) {
      SHOW_ERROR(message_MISSING_MARGINS_ERROR);
      return false;
   }

   Subset subsetout;
   pPal_->GetAttribute<Subset>(ProcessAdaptLayer::OutputSubsetWindowKeyAttr, subsetout);
   int pixels = SURI_TRUNC(int, pixels_);
   int lines = SURI_TRUNC(int, lines_);

   pPal_->AddAttribute<int>(ProcessAdaptLayer::OutputSizePixelsKeyAttr, pixels);
   pPal_->AddAttribute<int>(ProcessAdaptLayer::OutputSizeLinesKeyAttr, lines);

   pOutputWorld_->SetSpatialReference(pReprojectionParameters_->srWktOut_);
   pOutputWorld_->SetWorld(subsetout);
   pOutputWorld_->SetWindow(subsetout);
   pOutputWorld_->SetViewport(pixels, lines);

   pPal_->AddAttribute<World*>(ProcessAdaptLayer::OutputWorldKeyAttr, pOutputWorld_);

   if (pMarginsCheck_->GetValue())
      SetMarginPixelSize();

   modified_ = false;
   return true;
}

/** Metodo axuliar que calcular el tamanio en pixeles de los margenes */
void OutputPageConfigPart::SetMarginPixelSize() {
   double topmargin = StringToNumber<double>(pTopText_->GetValue().c_str());
   double bottommargin = StringToNumber<double>(pBottomText_->GetValue().c_str());
   double leftmargin = StringToNumber<double>(pLeftText_->GetValue().c_str());
   double rightmargin = StringToNumber<double>(pRightText_->GetValue().c_str());

   if (topmargin == 0 && bottommargin == 0 && leftmargin == 0 && rightmargin == 0)
      return;

   std::string title = pTitleText_->GetValue().c_str();
   std::string subtitle = pSubtitleText_->GetValue().c_str();

   Margin margin(pReprojectionParameters_, title, subtitle);
   margin.SetProcessLayer(pPal_);
   margin.SetMargins(topmargin, bottommargin, leftmargin, rightmargin, pixels_, lines_);
}

/**
 * Restaura los valores originales del Part.
 * @return bool que indica si tuvo exito
 */
bool OutputPageConfigPart::RollbackChanges() {
   return true;
}

/** Inicializa el part */
void OutputPageConfigPart::SetInitialValues() {
   double width = 0, height = 0;
   dpi_ = 90;
   CalculateWidthHeightDimensions(width, height);
   SetDimensions(width, height);
   CommitChanges();
}

/** Retorna las dimensiones en centimetros basandose en las dimensiones de salida en pixeles */
void OutputPageConfigPart::CalculateWidthHeightDimensions(double &Width, double &Height) {
   Subset subsetout;
   pPal_->GetAttribute<Subset>(ProcessAdaptLayer::OutputSubsetWindowKeyAttr, subsetout);
   Dimension centerpixelsizeout = CalculateCenterDimension();
   // Calculo el factor en x e y de la trasnformacion. (relacion unidades)
   double pixelsizex = centerpixelsizeout.GetWidth();
   double pixelsizey = centerpixelsizeout.GetHeight();
   Dimension dimout(subsetout);
   pixels_ = dimout.GetWidth() / pixelsizex;
   lines_ = dimout.GetHeight() / pixelsizey;
   RasterSpatialModel::Destroy(pReprojectionParameters_->pRasterSpatialModelOut_);
   pReprojectionParameters_->pRasterSpatialModelOut_ = RasterSpatialModel::Create(
         Coordinates(0, 0), Coordinates(pixels_, lines_), subsetout.ul_, subsetout.lr_);
   Width = pixels_ / dpi_;
   Height = lines_ / dpi_;
}

/** Metodo auxiliar que calcula el tamanio de pixel de entrada **/
Dimension OutputPageConfigPart::CalculateCenterDimension() {
   Subset subsetin;
   pInputWorld_->GetWorld(subsetin);
   // Con la transformacion creada obtengo el subset de salidas
   double oldcenterx = (subsetin.lr_.x_ + subsetin.ul_.x_) / 2;
   double oldcentery = (subsetin.lr_.y_ + subsetin.ul_.y_) / 2;
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

   pReprojectionParameters_->pRasterSpatialModelIn_->Transform(centersubsetin);
   Subset centersubsetout = centersubsetin;
   // Transformo ese subset a el SR de llegada
   if (pReprojectionParameters_->pTranform_ != NULL)
      pReprojectionParameters_->pTranform_->Transform(centersubsetout);
   // Calcular tamanio pixel SR de llegada
   return Dimension(centersubsetout);
}

/** Metodo auxiliar que calcula la dimension en pixel linea */
void OutputPageConfigPart::CalculatePixelLineDimensions(
      double &Pixels, double &Lines, double Width, double Height) {
   Pixels = Width * dpi_;
   Lines = Height * dpi_;
}

/** Actualiza los valores de los TextCtrl de dimension */
void OutputPageConfigPart::SetDimensions(double Width, double Height) {
   pWidthText_->ChangeValue(NumberToString<double>(Width));
   pHeightText_->ChangeValue(NumberToString<double>(Height));
}

} /** namespace ui */
} /** namespace suri */
