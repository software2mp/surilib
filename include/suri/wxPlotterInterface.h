/*! \file wxPlotterInterface.h */                                                     
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
*/ 

#ifndef __WX_PLOTTER_INTERFACE_H__
#define __WX_PLOTTER_INTERFACE_H__

class wxDC;
class wxWindow;

namespace suri {
namespace ui {

namespace wx {

class wxPlotterInterface {
	/** ctor copia **/
	wxPlotterInterface(const wxPlotterInterface&);

	public:
		/** ctor **/
		wxPlotterInterface() {}
		/** dtor **/
		virtual ~wxPlotterInterface() {}
		/** Metodo polimorfico encargado de graficar sobre una ventana de wx. */
		virtual bool Plot(wxDC* pDc, wxWindow* pWindow)=0;
};

}  /** namespace wx **/
}  /** namespace ui **/
}  /** namespace suri **/
#endif
