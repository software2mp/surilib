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

#include <vector>
#include <utility>
#include <limits>
#include <sstream>
#include <ios>

#include "suri/wx2DPlotter.h"
#include "SRDHistogram.h"

#include "wx/dc.h"
#include "wx/window.h"

namespace suri {
namespace ui {

namespace wx {

/** ctor **/
wx2DPlotter::wx2DPlotter(bool Continuos) : continuos_(Continuos),
      xMargin_(0.03), yMargin_(0.05) {
}

/** dtor **/
wx2DPlotter::~wx2DPlotter() {
}

/** Metodo auxiliar que dibuja la escala en el eje X **/
void wx2DPlotter::DrawScaleX(wxDC* pDc, wxWindow* pWindow, double ScaleFactor) {
   // TODO: hacer escala configurable.
   //double rangex = fabs(minX_ > 0 ? (maxX_ - minX_) : (maxX_ + minX_)) / ScaleFactor;
   double rangex = fabs(maxX_ - minX_) / ScaleFactor;
   for (double k = 1.0; k <= ScaleFactor ; ++k) {
      int xcoord = SURI_TRUNC(int, SURI_CEIL(minX_ + rangex*k));
      std::stringstream ssx;
      ssx << std::fixed;
      ssx << xcoord;
      std::pair<double, double> escalexcoords =
            ConvertPointToViewerCoords(pWindow, xcoord, 0);
      pDc->DrawText(ssx.str(), escalexcoords.first, escalexcoords.second);
   }
}

/** Metodo auxiliar que dibuja la escala en el eje Y **/
void wx2DPlotter::DrawScaleY(wxDC* pDc, wxWindow* pWindow, double ScaleFactor) {
   // TODO: hacer escala configurable.
   double rangey = (maxY_ - minY_) / ScaleFactor;
   wxSize size = pWindow->GetSize();
   double xorigin = xMargin_ * size.GetWidth();
   for (double k = 1.0; k <= ScaleFactor; ++k) {
      int ycoord = SURI_TRUNC(int, SURI_CEIL(minY_ + rangey*k));
      std::pair<double, double> escaleycoords = ConvertPointToViewerCoords(
                                                pWindow, xorigin, ycoord);
      std::stringstream ssy;
      ssy << std::fixed;
      ssy << ycoord;
      pDc->DrawText(ssy.str(), xorigin, escaleycoords.second);
   }
}

/** Metodo polimorfico encargado de graficar sobre una ventana de wx. */
bool wx2DPlotter::Plot(wxDC* pDC, wxWindow* pWindow) {
	pWindow->ClearBackground();
	pDC->SetBackground(*wxWHITE_BRUSH);
	pDC->Clear();
	wxPen axispen(*wxBLUE, 2);
   pDC->SetPen(axispen);
	wxSize size = pWindow->GetSize();
	/** Dibujo los ejes **/
	double xorigin = xMargin_ * size.GetWidth();
	double yorigin = size.GetHeight() - yMargin_*size.GetHeight();

   pDC->DrawLine(xorigin, yorigin, size.GetWidth() - xMargin_*size.GetWidth(), yorigin);
	pDC->DrawLine(xorigin, yorigin, xorigin, yMargin_ * size.GetHeight());

	if (!xLabel_.empty()) {
		pDC->DrawText(xLabel_.c_str(), xorigin*(1 - xMargin_), yorigin);
	}
	if (!yLabel_.empty()) {
		pDC->DrawRotatedText(yLabel_.c_str(), 0,
		                     (yMargin_ * (double)(size.GetHeight()))*3.0, 90);
	}
// Renderizado el conjunto de puntos
	double lastx = xorigin, lasty = yorigin;
	std::vector< std::pair<double, double> >::iterator point = points_.begin();
	wxPen graphpen(*wxRED, 1); 
   pDC->SetPen(graphpen);
	for ( ; point != points_.end(); ++point) {
		std::pair<double, double> coord =
		      ConvertPointToViewerCoords(pWindow, point->first, point->second);
	   // traza una recta entre el punto actual y el anterior
		if (continuos_)
		   pDC->DrawLine(lastx, lasty, coord.first, coord.second);
		else
			pDC->DrawPoint(coord.first, coord.second);
	   lastx = coord.first;
		lasty = coord.second;
	}

	double scaley = size.GetHeight() < 600 ? 3.0 : 4.0;
	DrawScaleY(pDC, pWindow, scaley);
	double scalex = size.GetWidth() < 600 ? 8.0 : 6.0;
	DrawScaleX(pDC, pWindow, scalex);
	return true;
}

/** Agrega un punto a la lista de puntos del graficador */
void wx2DPlotter::AddPoint(double X, double Y) {
	points_.push_back(std::make_pair<double, double>(X, Y));
}

/** 
 * Metodo que convierte un punto en una coordenada de pantalla (teniendo en cuenta
 * las dimensiones de la pantalla en donde se grafica
 * @param[in] pWindow ventana donde se dibujara el punto
 * @param[in] X componente x del punto
 * @param[in] Y componente y del punto
 * @return par de coordenadas que representa el punto en pantalla 
 **/
std::pair<double, double> wx2DPlotter::ConvertPointToViewerCoords(wxWindow* pWindow,
                                                                  double X, double Y) const {
	wxSize size = pWindow->GetSize();
	double xorigin = xMargin_ * size.GetWidth();
   double difftominx = minX_ < 0 ? X - minX_ : X + minX_;
   double rangex = fabs(maxX_ - minX_);
   double stepx = size.GetWidth() * (1 - 3.0 * xMargin_) / rangex;
	/** se usa dos veces el margen ya que el margen en x es a la izq y a la derecha **/
	double x = difftominx*stepx + xorigin;
   double yorigin = size.GetHeight() - yMargin_ * size.GetHeight();
	double stepy = yorigin / (maxY_ - minY_);
	double y= yorigin - Y*stepy * (1.0 - yMargin_);
	return std::make_pair<double, double>(x,y);
}		
/** 
 * Metodo que realiza una transformacion entre las coordenadas de pantalla y coordenadas x,y 
 * de la funcion 
 * @param[in] pWindow v
 * entana donde se dibujara el punto
 * @param[in] Vy componente x del punto en pantalla
 * @param[in] Vy componente y del punto en pantalla
 * i@return par de coordenadas x, y que presentan la coordenada dentro del dominio de la funcion
 */
std::pair<double, double> wx2DPlotter::ConvertViewerToPointCoords(wxWindow* pWindow, 
																								double Vx, double Vy) const {
	wxSize size = pWindow->GetSize();
	double xorigin = xMargin_ * size.GetWidth();
	double rangex = fabs(maxX_ - minX_);
   double stepx = size.GetWidth() * (1 - 3.0 * xMargin_) / rangex;
   double x = (Vx - xorigin)/stepx - fabs(minX_);
   double yorigin = size.GetHeight() - yMargin_ * size.GetHeight();
	double stepy = yorigin / (maxY_ - minY_);
	double y = (yorigin - Vy)/(stepy*(1.0 - yMargin_));
	return std::make_pair<double, double>(x,y);
}

/** */
void wx2DPlotter::SetXMargin(double Margin) {
	xMargin_ = Margin;
}

/** */
void wx2DPlotter::SetYMargin(double Margin) {
	yMargin_ = Margin;
}

/** **/
void wx2DPlotter::SetXLabel(const std::string& Label) {
	xLabel_ = Label;
}

/** **/
void wx2DPlotter::SetYLabel(const std::string& Label) {
	yLabel_ = Label;
}

void wx2DPlotter::SetYLimits(double Min, double Max) {
	minX_ = Min;
	maxX_ = Max;
}

void wx2DPlotter::SetXLimits(double Min, double Max) {	
	minY_ = Min;
	maxY_ = Max;
}
/** Obtiene la leyenda que se mostrara para el eje x**/
std::string wx2DPlotter::GetXLabel() {
   return xLabel_;
}
/** Obtiene la leyenda que se mostrara para el eje y**/
std::string wx2DPlotter::GetYLabel() {
   return yLabel_;
}

/** ctor **/
wxHistogramPlotter::wxHistogramPlotter(suri::raster::data::HistogramBase* pHistogram) : 
	wx2DPlotter(true), activeBand_(0), min_(0), max_(0), 
	pHistogram_(pHistogram) {
}

/** dtor **/
wxHistogramPlotter::~wxHistogramPlotter() {
	delete pHistogram_;
}
/** Metodo polimorfico encargado de graficar sobre una ventana de wx. */
bool wxHistogramPlotter::Plot(wxDC* pDC, wxWindow* pWindow) {
	SetYLabel("Freq.");
	SetXLabel("DN");
	points_.clear();
	HistogramToMatrix(pHistogram_, points_);
	if (wx2DPlotter::Plot(pDC, pWindow)) {
		/** dibujo las rectas de max y min **/
		wxPen boundarypen(*wxBLACK, 2, wxDOT);
		pDC->SetPen(boundarypen);
		std::pair<double, double> mincoord = ConvertPointToViewerCoords(pWindow, min_, 0);
		std::pair<double, double> yfinalcoord = ConvertPointToViewerCoords(pWindow, min_, maxY_);
		pDC->DrawLine(mincoord.first, mincoord.second, mincoord.first, yfinalcoord.second);
		std::pair<double, double> maxcoord = ConvertPointToViewerCoords(pWindow, max_, 0);
		pDC->DrawLine(maxcoord.first, maxcoord.second, maxcoord.first, yfinalcoord.second);
	} else {
		return false;
	}
	return true;
}


/** Metodo auxiliar que convierte un histograma en una matriz de 2 dimensiones **/
void wxHistogramPlotter::HistogramToMatrix(suri::raster::data::HistogramBase* pHistogram, 
																std::vector<std::pair<double, double> >& Points) {
	double min = pHistogram->GetMin()[activeBand_];
   double max = pHistogram->GetMax()[activeBand_];
   double binsize = (max - min) / static_cast<double>(pHistogram->GetNumBins()[activeBand_]);
	double minfreq = std::numeric_limits<double>::max();
	double maxfreq = std::numeric_limits<double>::min();
	for (int bin = 0; bin < pHistogram->GetNumBins()[activeBand_]; ++bin) {
		double freq = pHistogram->GetBins()[activeBand_][bin];
		double currentbin = min + bin * (binsize);
      points_.push_back(std::make_pair<double, double>(currentbin, freq));
		if (freq > maxfreq)
			maxfreq = freq;
		else if (freq < minfreq)
			minfreq = freq;
   }
	/** Configuro los limites de los ejes a graficar **/
	SetYLimits(min, max);
	SetXLimits(minfreq, maxfreq);
	//min_ = min;
	//max_ = max;
}

/** Selecciona la banda que utilizara el graficador de histograma **/
void wxHistogramPlotter::SetActiveBand(int Band) {
	activeBand_ = Band;
}
/** Configura el minimo para el grafico del histograma **/
void wxHistogramPlotter::SetMinValue(double Min) {
	min_ = Min;
}
/** Configura el maximo pasra el grafico del histograma */
void wxHistogramPlotter::SetMaxValue(double Max) {
	max_ = Max;
}
}  /** namespace wx **/
}  /** namespace ui **/
}  /** namespace suri **/
