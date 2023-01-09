/*! \file SupervisedParameterSelectionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
// Includes Suri
#include "ParametersSelectionPart.h"
#include "ClassificationPart.h"
// Includes Wx
// Defines

#ifndef SUPERVISEDPARAMETERSELECTIONPART_H_
#define SUPERVISEDPARAMETERSELECTIONPART_H_

namespace suri {
/** Factory para almacenar algoritmos de clasificacion  supervisada */
/**
 * Factory para almacenar algoritmos de clasificacion  supervisada.
 * Todos los algoritmos de clasif supervisada deberan heredar
 * de esta clase, y se registran aqui.
 *
 */
class SupervisedParameterSelectionPart : public suri::ParametersSelectionPart {
public:
   /** Macro que declara metodos de la clase */
HAS_FACTORY(SupervisedParameterSelectionPart, std::string);
public:
   /** Ctor */
   SupervisedParameterSelectionPart();
   /** Dtor */
   virtual ~SupervisedParameterSelectionPart();

   /** Retorna las propiedades de los pixels que no tienen clase */
   virtual bool GetNonClassifiedPixelInfo(
                  ClassInformation &Properties) = 0;
};
}

#endif /* SUPERVISEDPARAMETERSELECTIONPART_H_ */
