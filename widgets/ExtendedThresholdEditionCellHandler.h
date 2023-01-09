/*! \file ExtendedThresholdEditionCellHandler.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef EXTENDEDTHRESHOLDEDITIONCELLHANDLER_H_
#define EXTENDEDTHRESHOLDEDITIONCELLHANDLER_H_

// Includes Estandar
// Includes Suri
#include "CellTableHandler.h"

// Includes Wx
// Defines
// forwards

namespace suri {
/** Clase que captura el evento de edicion de una clase definida por umbrales.
 *  Lanza una nueva ventana con la informacion extendida de la clase
 *  (umbrales por banda). **/
class ExtendedThresholdEditionCellHandler : public CellTableHandler {
   /** ctor copia **/
   ExtendedThresholdEditionCellHandler(const ExtendedThresholdEditionCellHandler&);

public:
	ExtendedThresholdEditionCellHandler();
	virtual ~ExtendedThresholdEditionCellHandler();

private:
   /**
    * Metodo que se encargada de realizar la captura del evento por parte
    * del handler
    * @return true en caso de poder capturar el evento
    * @return false en caso contrario
    */
   virtual bool DoHandle(int rowNumber, int colNumber, wxGrid* pGrid, Table* table,
                         KeyDown KeyDown = NONE, MouseEvent MEvent = LCLICK);
};

} /** namespace suri */

#endif /* EXTENDEDTHRESHOLDEDITIONCELLHANDLER_H_ */
