/*! \file VectorOperationBuilder.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTOROPERATIONBUILDER_H_
#define VECTOROPERATIONBUILDER_H_

// Includes Estandar
#include <vector>
#include <string>
// Includes Suri
#include "VectorOperation.h"
// Includes Wx
// Defines
// forwards

namespace suri {

class VectorOperationBuilder {
public:
   /** Constructor */
   VectorOperationBuilder(Option& Parameters);
   /** Destructor */
   virtual ~VectorOperationBuilder();
   /** Obtiene una instancia de la clase correspondiente a la operacion que quiera realizarse */
   VectorOperation* GetOperation(VectorOperation::OperationType Operation);
   static const std::string DistanceFieldKey; /** Clave de campo de distancia de buffer */
   static const std::string DistanceValueKey; /** Clave de distancia de buffer */
   static const std::string NRingsKey; /** Clave de numero de anillos concentricos */
   static const std::string ConversionFactorKey; /** Clave de factor de conversion */
   static const std::string MergeFieldsMapKey; /** Clave de campos a fusionar */

private:
   double distance_; /** Distancia de buffer */
   int nrings_; /** Numero de anillos concentricos */
   std::string distancefield_; /** Campo de distancia de buffer */
   double conversionfactor_; /** Factor de conversion */
   VectorOperation::OperationType operation_; /** Vector de operaciones */
   Option operationOptions_;
};

} /** namespace suri */

#endif /* VECTOROPERATIONBUILDER_H_ */
