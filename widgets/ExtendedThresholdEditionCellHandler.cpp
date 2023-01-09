/*! \file ExtendedThresholdEditionCellHandler.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "ExtendedThresholdEditionCellHandler.h"

// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

/** ctor **/
ExtendedThresholdEditionCellHandler::ExtendedThresholdEditionCellHandler() {

}

/** dtor */
ExtendedThresholdEditionCellHandler::~ExtendedThresholdEditionCellHandler() {
}

/** Metodo que se encarga del manejo de click sobre la celda de una tabla */
bool ExtendedThresholdEditionCellHandler::DoHandle(int rowNumber, int colNumber,
		wxGrid* pGrid, Table* table, KeyDown KeyDown, MouseEvent MEvent) {
	if (KeyDown != suri::CellTableHandler::CTRL
			|| MEvent != CellTableHandler::DLCLICK)
		return false;
	return false;
}

} /** namespace suri */
