/*! \file wxVectorStyleCellRenderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXVECTORSTYLECELLRENDERER_H_
#define WXVECTORSTYLECELLRENDERER_H_



// Includes Estandar
// Includes Suri
// Includes Wx
#include "wx/grid.h"

// Defines
// forwards

namespace suri {

/** Clase encargada de renderizar un estilo vectorial en una celda de una tabla de wx. **/
class wxVectorStyleCellRenderer : public wxGridCellRenderer {
public:
   wxVectorStyleCellRenderer();
   virtual ~wxVectorStyleCellRenderer();
   /** Pinta el rectangulo de la celda con el valor asociado a la celda */
   virtual void Draw(wxGrid& Grid, wxGridCellAttr& Attribute, wxDC& Dc,
                     const wxRect& Cell, int Row, int Column, bool IsSelected);
   /** Devuelve el size por defecto */
   virtual wxSize GetBestSize(wxGrid& Grid, wxGridCellAttr& Attribute, wxDC& Dc,
                              int Row, int Column);
   /** Clonador */
   virtual wxVectorStyleCellRenderer* Clone() const;
};

} /** namespace suri */

#endif /* WXVECTORSTYLECELLRENDERER_H_ */
