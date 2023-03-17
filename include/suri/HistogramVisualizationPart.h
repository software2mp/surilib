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
