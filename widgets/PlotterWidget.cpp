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

#include "suri/PlotterWidget.h"
#include "suri/PaintEventInterface.h"
#include "suri/MouseEventInterface.h"
#include "suri/wx2DPlotter.h"

#include "wx/dcclient.h"

namespace suri {
namespace ui {

START_IMPLEMENT_EVENT_OBJECT(PlotterWidgetEventHandler, PlotterWidget)
	IMPLEMENT_EVENT_CALLBACK(OnResize, OnResize(event), wxSizeEvent)
END_IMPLEMENT_EVENT_OBJECT


class PlotterPaintEvent : public PaintEventInterface {
	
public:
	explicit PlotterPaintEvent(wx::wxPlotterInterface* pPlotter, wxWindow* pWindow) : pPlotter_(pPlotter),
																						pWindow_(pWindow) {
	}
	~PlotterPaintEvent() {}
	virtual void OnPaint(wxPaintEvent& Event, wxDC& Dc) {
		pPlotter_->Plot(&Dc, pWindow_);
	}

	virtual void OnBackgroundErase(wxEraseEvent&) {
	}

	virtual PaintEventFlags GetPaintFlags() const {
		return PaintEventInterface::Paint;
	}
	virtual void PaintEnabled(bool Enable = true) {
	}
	virtual bool CanProcessEvent(wxPaintEvent&) const {
		return true;
	}
	virtual bool IsPaintEnabled() const {
		return true;
	}

protected:
	wx::wxPlotterInterface* pPlotter_;
	wxWindow* pWindow_;
};

class HistogramMouseEditionEvent : public MouseEventInterface {

public:
	/** ctor **/
	explicit HistogramMouseEditionEvent(wx::wxHistogramPlotter* pPlotter, 
						HistogramPlotterWidgetEventObserverInterface* pObserver,
						wxWindow* pWindow) : pPlotter_(pPlotter), pObserver_(pObserver), 
						pWindow_(pWindow), enable_(true) {
	}
	virtual void OnLeft(wxMouseEvent&) {}
   virtual void OnLeftUp(wxMouseEvent& Event) {
		std::pair<double, double> coord = 
							pPlotter_->ConvertViewerToPointCoords(pWindow_, Event.GetX(), Event.GetY());	
		double dmin = pPlotter_->GetMin() > coord.first ? 
				pPlotter_->GetMin() - coord.first : coord.first - pPlotter_->GetMin();		
		double dmax = pPlotter_->GetMax() > coord.first ? 
				pPlotter_->GetMax() - coord.first : coord.first - pPlotter_->GetMax();

		if (dmin < dmax) {
			pPlotter_->SetMinValue(coord.first);
			pObserver_->HistogramMinChange();
		} else {
			pPlotter_->SetMaxValue(coord.first);
			pObserver_->HistogramMaxChange();
		}
		pObserver_->PointSelected(coord.first, coord.second);
	}

   virtual void OnLeftDown(wxMouseEvent&) {}
   virtual void OnLeftDoubleClick(wxMouseEvent&) {}
   virtual void OnRight(wxMouseEvent&) {}
   virtual void OnRightUp(wxMouseEvent&) {}
   virtual void OnRightDown(wxMouseEvent&) {}
   virtual void OnRightDoubleClick(wxMouseEvent&) {}
   virtual void OnMiddle(wxMouseEvent&) {}
   virtual void OnMiddleUp(wxMouseEvent&) {}
   virtual void OnMiddleDown(wxMouseEvent&) {}
   virtual void OnMiddleDoubleClick(wxMouseEvent&) {}
   virtual void OnEnterWindow(wxMouseEvent&) {}
   virtual void OnLeaveWindow(wxMouseEvent&) {}
   virtual void OnWheel(wxMouseEvent&) {}
   virtual void OnMouseMove(wxMouseEvent&) {}
   virtual void OnMouse(wxMouseEvent&) {}

   virtual MouseEventFlags GetMouseFlags() const {
		return MouseEventInterface::LeftUp;
	}

   virtual void MouseEnabled(bool Enable = true) {
		enable_ = Enable;
   }
   virtual bool IsMouseEnabled() const {
		return enable_;
	}

   virtual bool CanProcessEvent(wxMouseEvent&) const {
		return IsMouseEnabled();
	}

protected:
	wx::wxHistogramPlotter* pPlotter_;
	HistogramPlotterWidgetEventObserverInterface* pObserver_;
	wxWindow* pWindow_;
	bool enable_;
};

/** ctor **/
PlotterWidget::PlotterWidget(wx::wxPlotterInterface* pPlotter) : pPlotter_(pPlotter), 
		pEventHandler_(new PlotterWidgetEventHandler(this)) {
}

/** dtor **/
PlotterWidget::~PlotterWidget() {
	delete pEventHandler_;
	PopPaintEvent(pPaintEvent_);
}

/** Se crea la ventana de herramienta en las clases derivadas */
bool PlotterWidget::CreateToolWindow() {
	if (pToolWindow_)
   	return false;
   pToolWindow_ =
         wxXmlResource::Get()->LoadPanel(pParentWindow_, wxT("ID_PLOTTERWIDGET_PANEL"));
	GetFrame()->Connect(wxEVT_SIZE, 
					wxSizeEventHandler(PlotterWidgetEventHandler::OnResize), NULL, pEventHandler_);
	pToolWindow_->Connect(wxEVT_SIZE,
	               wxSizeEventHandler(PlotterWidgetEventHandler::OnResize), NULL, pEventHandler_);
	if (pPlotter_) {
		pPaintEvent_ = new PlotterPaintEvent(pPlotter_, pToolWindow_);
		PushPaintEvent(pPaintEvent_);
	}
	return true;
}

/** Configura el grafifador a utilizar. Al cambiar el plotter se forzara 
 * la renderizacion del widget*/
void PlotterWidget::SetPlotter(wx::wxPlotterInterface* pPlotter) {
	pPlotter_ = pPlotter;
}
/** Obtiene el plotter asociado **/
wx::wxPlotterInterface*	PlotterWidget::GetPlotter() {
	return pPlotter_;
}
/** Metodo que captura el evento de resize de la ventana **/
void PlotterWidget::OnResize(wxSizeEvent& Event) {
	pToolWindow_->Refresh();
	pToolWindow_->Update();
}

/** ctor **/
HistogramPlotterWidget::HistogramPlotterWidget(HistogramPlotterWidgetEventObserverInterface* pObserver) : 
									pMouseEvent_(NULL), pObserver_(pObserver) {
}
/** dtor **/
HistogramPlotterWidget::~HistogramPlotterWidget() {
   pObserver_ = NULL;
   PopMouseEvent(pMouseEvent_);
}

bool HistogramPlotterWidget::CreateToolWindow() {
	if (PlotterWidget::CreateToolWindow()) {
		pMouseEvent_ = 
				new HistogramMouseEditionEvent(dynamic_cast<wx::wxHistogramPlotter*>(pPlotter_), 
				pObserver_, pToolWindow_);
		PushMouseEvent(pMouseEvent_);
	}
	return false;
}
/** habilita la edicion del mouse **/
void HistogramPlotterWidget::EnableMouseEdition() {
	pMouseEvent_->MouseEnabled(true);
}

/** deshabilita la edicion del mouse */
void HistogramPlotterWidget::DisableMouseEdition() {
	pMouseEvent_->MouseEnabled(false);
}

}  /** namespace ui **/
}  /** namespace suri **/
