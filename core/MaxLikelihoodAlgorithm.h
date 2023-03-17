/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

#ifndef MAXLIKELIHOODALGORITHM_H_
#define MAXLIKELIHOODALGORITHM_H_

// Includes standard
#include <string>
#include <vector>
// Includes Suri
#include "ClusterClassificationAlgorithm.h"
// Includea wx
// Defined
// forward

/** namespace suri */
namespace suri {
/** Clase que implementa el algoritmo MaxLikelihood para la clasificacion */
/**
 * Si se configura el ClassificationRenderer con una instancia de esta clase
 * se clasificara la imagen usando el metodo de maxima verosimilitud.
 */
class MaxLikelihoodAlgorithm : public ClusterClassificationAlgorithm {
   /** Ctor. de Copia. */
   MaxLikelihoodAlgorithm(const MaxLikelihoodAlgorithm &MaxLikAlgorithm);

public:
   /** Ctor */
   MaxLikelihoodAlgorithm();
   /** Dtor */
   virtual ~MaxLikelihoodAlgorithm();
   /** Clasifica los pixels en pSource y asigna resultado a pDest */
   virtual bool Classify(int* pDest, std::vector<void*> pSource, size_t Size,
                                                   const std::string &DataType);
   /** Verifica que las dimensiones de los parametros sean consistentes */
   virtual bool Check() const;

protected:
   /** Metodo que retorna un puntero al algoritmo de clasificacion */
   virtual MaxLikelihoodAlgorithm* Initialize(const std::string &FileName) {
      return this;
   }
};
}  // namespace

#endif /* MAXLIKELIHOODALGORITHM_H_ */
