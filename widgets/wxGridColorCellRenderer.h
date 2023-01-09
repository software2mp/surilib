/*! \file wxGridColorCellRenderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXGRIDCOLORCELLRENDERER_H_
#define WXGRIDCOLORCELLRENDERER_H_

// Includes Estandar

// Includes Suri

// Includes Wx
#include "wx/wx.h"
#include "wx/grid.h"

// Defines

// forwards

namespace suri {

/**
 * Render encargado de pintar una celda interpretando el valor string de la celda como un color hexa
 */
class wxGridColorCellRenderer : public wxGridCellRenderer {
public:
   /** Constructor */
   wxGridColorCellRenderer();
   /** Destructor */
   virtual ~wxGridColorCellRenderer();
   /** Pinta el rectangulo de la celda con el valor asociado a la celda */
   virtual void Draw(wxGrid& Grid, wxGridCellAttr& Attribute, wxDC& Dc,
                     const wxRect& Cell, int Row, int Column, bool IsSelected);
   /** Devuelve el size por defecto */
   virtual wxSize GetBestSize(wxGrid& Grid, wxGridCellAttr& Attribute, wxDC& Dc,
                              int Row, int Column);
   /** Clonador */
   virtual wxGridColorCellRenderer* Clone() const;
};

} /** namespace suri */

#endif /* WXGRIDCOLORCELLRENDERER_H_ */
