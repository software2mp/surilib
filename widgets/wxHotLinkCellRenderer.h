/*! \file wxHotLinkCellRenderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXHOTLINKCELLRENDERER_H_
#define WXHOTLINKCELLRENDERER_H_

// Includes Estandar
// Includes Suri
// Includes Wx
#include "wx/wx.h"
#include "wx/grid.h"
// Defines
// forwards

namespace suri {

class wxHotLinkCellRenderer : public wxGridCellRenderer {
public:
   wxHotLinkCellRenderer();
   virtual ~wxHotLinkCellRenderer();
   /** Devuelve el size por defecto */
   virtual wxSize GetBestSize(wxGrid& Grid, wxGridCellAttr& Attribute, wxDC& Dc, int Row,
                               int Column);
   /** Clonador */
   virtual wxHotLinkCellRenderer* Clone() const;
   void Draw(wxGrid& Grid, wxGridCellAttr& Attribute, wxDC& Dc, const wxRect& Cell,
             int Row, int Column, bool IsSelected);
};

} /** namespace suri */

#endif /* WXHOTLINKCELLRENDERER_H_ */
