/*! \file PlotterWidget.h */                                                     
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
*/ 

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
