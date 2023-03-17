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
