/*! \file ManualEnhancementPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef __MANUAL_ENHANCEMENT_PART_H__
#define __MANUAL_ENHANCEMENT_PART_H__

#include "suri/HistogramVisualizationPart.h"
#include "SRDHistogram.h"
#include "suri/PlotterWidget.h"

namespace suri {

// forward
class RasterLayer;

namespace ui {

namespace wx {
	class wxHistogramPlotter;
}

class ManualEnhancementEventHandler;

/** Clase que representa el control grafica para la configuracion manual de realce a traves 
 *  de la visualizacion del histograma
 **/
class ManualEnhancementPart : public HistogramVisualizationPart {
	/** ctor copia **/
	ManualEnhancementPart(const ManualEnhancementPart&);
	
public:
	explicit ManualEnhancementPart(RasterLayer* pRaster);
	~ManualEnhancementPart();
   /** Indica si el Part tiene cambios para salvar. */
   bool HasChanged();   
	/** Inicializa el part */
   void SetInitialValues();
   /** Salva los cambios realizados en el Part. */
   bool CommitChanges();
   /** Restaura los valores originales del Part. */
   bool RollbackChanges();
	/** Inicializa la ventana y sus componentes **/
	bool CreateToolWindow();
	/** Metodo polimorfico que se llama cuando se cambiaron los limites del histograma sobre la banda activa**/
	virtual void HistogramMaxChange();
	virtual void HistogramMinChange();
   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;

protected:
	friend class ManualEnhancementEventHandler;
	/** Metodo que se llama cuando se cambia el minimo del histograma **/
	void OnMinValueChange(wxCommandEvent& Event);
	/** Metodo que se llama cuando se cambia el maximo del histograma **/
	void OnMaxValueChange(wxCommandEvent& Event);
	/** Metodo que se llama cuando se presiona el boton para reiniciar los cambios del histograma **/
	void OnResetButtonClicked(wxCommandEvent& Event);
	/** metodo auxiliar que obtiene el indice de la banda seleccionada en el choice **/
	int GetSelectedBandIndex();
	ManualEnhancementEventHandler* pEnhancementHandler_;
};

}  /** namespace ui **/
}  /** namespace suri **/

#endif
