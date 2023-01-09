/*! \file ClassifiedRasterDatasourceValidator.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CLASSIFIEDRASTERDATASOURCEVALIDATOR_H_
#define CLASSIFIEDRASTERDATASOURCEVALIDATOR_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/DatasourceInterface.h"
#include "suri/DatasourceValidatorInterface.h"
#include "suri/DataTypes.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * El ClassifiedRasterDatasourceValidator se encarga de validar los casos en que un
 * RasterDatasource se puede utilizar como un raster clasificado (una banda y tipo entero)
 */
class ClassifiedRasterDatasourceValidator : public suri::DatasourceValidatorInterface {
public:
   /** Constructor */
   ClassifiedRasterDatasourceValidator();
   /** Destructor */
   virtual ~ClassifiedRasterDatasourceValidator();
   /** Efectua la validacion del objeto en cuestion y devuelve verdadero si es valido */
   virtual bool IsValid(DatasourceInterface* pDatasource) const;

private:
   /** Metodo auxiliar para validar el xml de la fuente de datos */
   virtual bool IsXmlValid(DatasourceInterface* pDatasource) const;
   /** Verifica si un tipo es entero */
   bool IsIntegerType(const std::string& Type) const;
};

} /** namespace suri */

#endif /* CLASSIFIEDRASTERDATASOURCEVALIDATOR_H_ */
