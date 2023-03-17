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

#ifndef __PLOTTER_WIDGET_H__
#define __PLOTTER_WIDGET_H__

#include "suri/Widget.h"
#include "suri/wxPlotterInterface.h"

namespace suri {
// forward
class MouseEventInterface;

namespace ui {
// forward
class PlotterWidgetEventHandler;
class PlotterPaintEvent;

/** Componente grafico en donde el graficador se encargara de renderizar. **/
class PlotterWidget : public Widget {
	/** ctor copia **/
	PlotterWidget(const PlotterWidget&);

public:
	/** ctor **/
	explicit PlotterWidget(wx::wxPlotterInterface* pPlotter = NULL);
	/** dtor **/
	virtual ~PlotterWidget();
	/** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
	/** Configura el grafifador a utilizar. Al cambiar el plotter se forzara 
	 * la renderizacion del widget*/
	void SetPlotter(wx::wxPlotterInterface* pPlotter);
	/** Obtiene el plotter asociado **/
	wx::wxPlotterInterface*	GetPlotter();
	
protected:
	friend class PlotterWidgetEventHandler;
	/** Metodo que captura el evento de resize de la ventana **/
	void OnResize(wxSizeEvent& Event);
	/** Metodo que captura el evento de cambio de banda **/
	void OnBandSelectionChange(wxCommandEvent& Event);
	wx::wxPlotterInterface* pPlotter_;
	PlotterWidgetEventHandler* pEventHandler_;
	PlotterPaintEvent* pPaintEvent_;
};

/** 
 *	Interfaz que define el comportamiento base para un observer de cambios sobre el histograma
 * a traves de eventos generados dentro del widget
  **/
class HistogramPlotterWidgetEventObserverInterface {

public:
	/** Metodos polimorficos que se llaman cuando 
		se cambiaron los limites del histograma sobre la banda activa **/
	virtual void HistogramMaxChange()=0;
	virtual void HistogramMinChange()=0;
	virtual void PointSelected(double X, double Y)=0;
};

class HistogramPlotterWidget : public PlotterWidget {
	/** ctor copia **/
	HistogramPlotterWidget(const HistogramPlotterWidget&);

public:
	/** ctor **/
	HistogramPlotterWidget(HistogramPlotterWidgetEventObserverInterface* pObserver);
	/** dtor **/
	~HistogramPlotterWidget();
	/** habilita la edicion del mouse **/
	void EnableMouseEdition();
	/** deshabilita la edicion del mouse */
	void DisableMouseEdition();
	/** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();

private:
	MouseEventInterface* pMouseEvent_;
	HistogramPlotterWidgetEventObserverInterface* pObserver_;
};

}  /** namespace ui **/
}  /** namespace suri **/
#endif
