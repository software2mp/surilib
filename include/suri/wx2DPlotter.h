/*! \file wx2DPlotter.h */                                                     
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
*/ 

#ifndef __WX_2D_PLOTTER_H__
#define __WX_2D_PLOTTER_H__

#include <vector>
#include <utility>

#include "suri/wxPlotterInterface.h"
#include "SRDHistogram.h"

class wxDC;
class wxWindow;

namespace suri {
namespace ui {

namespace wx {

/** Clase que representa un graficador en 2 dimensiones (tanto continuo como discreto). 
 *  Posee una lista de puntos (en coords x,y). Renderiza esta lista interpolando con una linea 
 *  recta entre dos puntos consecutivos (en caso de que sea continua).
 **/
class wx2DPlotter : public wxPlotterInterface {
	/** ctor copia **/
	wx2DPlotter(const wx2DPlotter&);

	public:
		/** ctor **/
		explicit wx2DPlotter(bool Continuos = true);
		/** dtor **/
		virtual ~wx2DPlotter();
		/** Metodo polimorfico encargado de graficar sobre una ventana de wx. */
		virtual bool Plot(wxDC* pDc, wxWindow* pWindow);
		/** Agrega un punto a la lista de puntos del graficador */
		void AddPoint(double X, double Y);
		/** Configura el margen horizontal */
		void SetXMargin(double Margin);
		/** Configura el margen vertical */
		void SetYMargin(double Margin);
		/** Configura el label que se mostrara para el eje x**/
		void SetXLabel(const std::string& Label);
		/** Configura el label que se mostrara para el eje y**/
		void SetYLabel(const std::string& Label);
		/** Establece min y max para las coordenadas x **/
		void SetYLimits(double Min, double Max);
		/** Establece min y max para coordenadas y **/
		void SetXLimits(double Min, double Max);
		/** Configura el color con que grafica la funcion deseada **/
		void SetColor(int r, int g, int b);
		/** 
		 * Metodo que realiza una transformacion entre las coordenadas de pantalla y coordenadas x,y 
		 * de la funcion 
     	 * @param[in] pWindow ventana donde se dibujara el punto
       * @param[in] Vy componente x del punto en pantalla
		 * @param[in] Vy componente y del punto en pantalla
		 * @return par de coordenadas x, y que presentan la coordenada dentro del dominio de la funcion
	    */
		std::pair<double, double> ConvertViewerToPointCoords(wxWindow* pWindow, double Vx, double Vy) const;
      /** Obtiene la leyenda que se mostrara para el eje x**/
      std::string GetXLabel();
      /** Obtiene la leyenda que se mostrara para el eje y**/
      std::string GetYLabel();

	protected:
		/** 
		 * Metodo que convierte un punto en una coordenada de pantalla (teniendo en cuenta
		 * las dimensiones de la pantalla en donde se grafica
       * @param[in] pWindow ventana donde se dibujara el punto
       * @param[in] X componente x del punto
		 * @param[in] Y componente y del punto
		 * @return par de coordenadas x, y que presentan la coordenada en pantalla
       **/
		std::pair<double, double> ConvertPointToViewerCoords(wxWindow* pWindow, double X, double Y) const;
		/** Metodo auxiliar que dibuja la escala en el eje X **/
		void DrawScaleX(wxDC* pDc, wxWindow* pWindow, double ScaleFactor);
      /** Metodo auxiliar que dibuja la escala en el eje Y **/
      void DrawScaleY(wxDC* pDc, wxWindow* pWindow, double ScaleFactor);
				/** listado de puntos que componen el grafico **/
		std::vector< std::pair<double, double> > points_;
		/** flag que indica si el grafico es continuo **/
		bool continuos_;
		/** margenes  porcenuales en funcion del size de la ventana**/
		double xMargin_, yMargin_;
		/** Limite de los ejes **/
		double minX_, maxX_;
		double minY_, maxY_;

		/** etiquetas de ejes **/
		std::string xLabel_, yLabel_;
};

/** Clase que a partir de un histograma genera una lista de puntos (en coords x,y) 
 *  que seran graficadas por el graficador 2D.  */
class wxHistogramPlotter : public wx2DPlotter {
	/** ctor copia **/
	wxHistogramPlotter(const wxHistogramPlotter&);

	public:
		/** ctor **/
		explicit wxHistogramPlotter(suri::raster::data::HistogramBase* pHistogram);
		/** dtor **/
		~wxHistogramPlotter();
		/** Selecciona la banda que utilizara el graficador de histograma **/
		void SetActiveBand(int Band);
		/** Metodo polimorfico encargado de graficar sobre una ventana de wx. */
		virtual bool Plot(wxDC* pDc, wxWindow* pWindow);
		/** Configura el minimo para el grafico del histograma **/
		void SetMinValue(double Min);
		/** Configura el maximo pasra el grafico del histograma */
		void SetMaxValue(double Max);
		/** */
		double GetMax() {
			return max_;
		}

		double GetMin() {
			return min_;
		}

	private:
		int activeBand_;
		double min_, max_;
		suri::raster::data::HistogramBase* pHistogram_;
		/** Metodo auxiliar que convierte un histograma en una matriz de 2 dimensiones **/
		void HistogramToMatrix(suri::raster::data::HistogramBase* pHistogram, 
																std::vector<std::pair<double, double> >& Points);
};
}  /** namespace wx **/
}  /** namespace ui **/
}  /** namespace suri **/
#endif
