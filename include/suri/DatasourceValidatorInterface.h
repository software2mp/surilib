/*! \file ValidatorInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VALIDATORINTERFACE_H_
#define VALIDATORINTERFACE_H_

// Includes Estandar

// Includes Suri
#include "DatasourceInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * La clase Validator provee una interfaz para permitir diferentes tipos de validaciones
 * para estados particulares de objetos como Datasources.
 */
class DatasourceValidatorInterface {
public:
   /** Constructor */
   DatasourceValidatorInterface() {}
   /** Destructor */
   virtual ~DatasourceValidatorInterface() {}
   /** Efectua la validacion del objeto en cuestion y devuelve verdadero si es valido */
   virtual bool IsValid(DatasourceInterface* pDatasource) const=0;
};

} /** namespace suri */

#endif /* VALIDATORINTERFACE_H_ */
