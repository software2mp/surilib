/** \file MathFunctions.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


#ifndef MATHFUNCTIONS_H_
#define MATHFUNCTIONS_H_

// Includes standard
#include <vector>

namespace suri {

/**
 * Calcula matriz LU. No tengo que modificar los pivote debido a que la matriz
 * es de correlacion y tiene los maximos de cada fila en la diagonal.
 * @param[in] Matrix matriz de correlacion
 * @param[out] Lu matriz LU
 */
void CalculateLu(const std::vector<std::vector<double> > Matrix,
                                    std::vector<std::vector<double> > &Lu);

}  // namespace suri


#endif /* MATHFUNCTIONS_H_ */
