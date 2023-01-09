/*! \file wxGridColAttrProvider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXGRIDCOLATTRPROVIDER_H_
#define WXGRIDCOLATTRPROVIDER_H_

// Includes Estandar

// Includes Suri
#include "suri/Table.h"
#include "wxGenericTableBase.h"

// Includes Wx
#include "wx/grid.h"

// Defines

// forwards

namespace suri {

/**
 * Provee una extension de proveedores de celdas para columnas y los tipos definidos en SuriLib
 */
class wxGridColAttrProvider : public wxGridCellAttrProvider {
public:
   /** Constructor */
   wxGridColAttrProvider(Table* pTable, wxGenericTableBase* pGenericTable,
                         const Option& Options = Option());
   /** Destructor */
   virtual ~wxGridColAttrProvider();
   /** Setting attributes */
   virtual void SetAttr(wxGridCellAttr* pAttr, int Row, int Column);
   /** Set attribute for the specified row */
   virtual void SetRowAttr(wxGridCellAttr* pAttr, int Row);

private:
   Table* pTable_;      /*! Puntero a la interfaz de tabla */
   wxGenericTableBase* pGenericTable_;    /*! Puntero a la gui de tabla*/
};

} /** namespace suri */

#endif /* WXGRIDCOLATTRPROVIDER_H_ */
