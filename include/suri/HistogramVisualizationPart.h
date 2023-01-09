/*! \file HistogramVisualizationPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef __HISTOGRAM_VISUALIZATION_PART_H__
#define __HISTOGRAM_VISUALIZATION_PART_H__

#include "suri/Part.h"
#include "SRDHistogram.h"
#include "suri/PlotterWidget.h"

namespace suri {

// forward
class RasterLayer;

namespace ui {

namespace wx {
	class wxHistogramPlotter;
}

class HistogramVisualizationHandler;

/** Clase que representa el control grafica para la configuracion manual de realce a traves 
 *  de la visualizacion del histograma
 **/
class HistogramVisualizationPart : public Part,
											  public HistogramPlotterWidgetEventObserverInterface {
	/** ctor copia **/
	HistogramVisualizationPart(const HistogramVisualizationPart&);
	
public:
	explicit HistogramVisualizationPart(RasterLayer* pRaster);
	virtual ~HistogramVisualizationPart();
   /** Indica si el Part tiene cambios para salvar. */
   bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   bool CommitChanges();
   /** Restaura los valores originales del Part. */
   bool RollbackChanges();
   /** Inicializa el part */
   void SetInitialValues();
   /** Actualiza el estado de la parte */
   void Update();
	/** Inicializa la ventana y sus componentes **/
	bool CreateToolWindow();
   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
	/** Metodo polimorfico que se llama cuando se cambiaron los limites del histograma sobre la banda activa**/
	virtual void HistogramMaxChange();
	virtual void HistogramMinChange();
	virtual void PointSelected(double X, double Y);

protected:
	friend class HistogramVisualizationHandler;
	/** Metodo que se llama cuando se presiona el boton de actualizar histograma **/
	void OnRefreshClicked(wxCommandEvent& Event);
	/** Metodo que se llama cuando cambia la seleccion de banda **/
	void OnBandSelectionChange(wxCommandEvent& Even);
   /** Lanzado cuando cambia el tamanio de la ventana */
   void OnResize(wxSizeEvent &Event);
	/** metodo que fuerza el redibujado en pantalla **/
	void Refresh();
	/** fuente de datos sobre la cual se calcula el histograma **/
	RasterLayer* pRaster_;
	raster::data::StatisticsBase* pStats_;
	raster::data::HistogramBase* pHistogram_;
	HistogramVisualizationHandler* pEventHandler_;
	wx::wxHistogramPlotter* pPlotRender_;
	PlotterWidget* pPlotter_;
	/** minimos y maximos sin cambios **/
	std::vector<double> mins_, maxs_;
   /** minimos y maximos en edicion **/
   std::vector<double> currentMins_, currentMaxs_;
	/** flag que indica si se desea utilizar solo la combinacion de bandas activa **/
	bool useCombination_;
};

}  /** namespace ui **/
}  /** namespace suri **/

#endif
