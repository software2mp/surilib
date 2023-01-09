/*! \file HotLinkCellTableHandler.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef HOTLINKCELLTABLEHANDLER_H_
#define HOTLINKCELLTABLEHANDLER_H_

// Includes Estandar
// Includes Suri
#include "CellTableHandler.h"
#include "suri/ToggleButton.h"
#include "suri/messages.h"
#include "resources.h"

// Includes Wx
// Defines
// forwards

namespace suri {

class HotLinkCellTableHandler : public CellTableHandler{
   /** ctor copia **/
   HotLinkCellTableHandler(const HotLinkCellTableHandler&);

public:
   HotLinkCellTableHandler();
   virtual ~HotLinkCellTableHandler();

private:
   /**
    * Metodo que se encarga del manejo de click sobre la celda.
    * Para el caso del handler
    * de hotlink, este se activa realizando ctrl+click en vez de click
    * @return true en caso de poder capturar el evento
    * @return false en caso contrario
    */
   virtual bool DoHandle(int rowNumber, int colNumber, wxGrid* pGrid, Table* table,
                         KeyDown KeyDown = NONE, MouseEvent MEvent = LCLICK);
   void DisplayHotLink(const std::string &Url);
};

} /** namespace suri */

#endif /* HOTLINKCELLTABLEHANDLER_H_ */
