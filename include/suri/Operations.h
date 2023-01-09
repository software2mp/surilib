/*! \file Operations.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef OPERATIONS_H_
#define OPERATIONS_H_

// Include suri
#include "Element.h"

/** namespace suri */
namespace suri {
/** Ancho de la barra de contrastes */
#define CONTRAST_SIZE 21

/** Modifica los valores de brillo y contraste de un elemento */
void SetBrightnessContrast(Element *pElement, int Brightness, int Contrast);

/** Obtiene los valores de brillo y contraste de un elemento */
void GetBrightnessContrast(Element *pElement, int &Brightness, int &Contrast);
}
;
// namespace suri

#endif /*OPERATIONS_H_*/
