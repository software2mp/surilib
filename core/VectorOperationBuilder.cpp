/*! \file VectorOperationBuilder.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "VectorOperationBuilder.h"
#include "BufferOperation.h"
#include "UnionOperation.h"
#include "IntersectionOperation.h"
#include "TrimOperation.h"
#include "MergeLayersOperation.h"
#include "suri/AuxiliaryFunctions.h"
// Includes Wx
// Defines
// forwards

namespace suri {

const std::string VectorOperationBuilder::DistanceFieldKey = "distancefield";
const std::string VectorOperationBuilder::DistanceValueKey = "distancevalue";
const std::string VectorOperationBuilder::NRingsKey = "Nrings";
const std::string VectorOperationBuilder::ConversionFactorKey = "conversionfactor";
const std::string VectorOperationBuilder::MergeFieldsMapKey = "mergefields";

/** Constructor */
VectorOperationBuilder::VectorOperationBuilder(Option& Parameters) : operationOptions_(Parameters) {
   distancefield_ = Parameters.GetOption(DistanceFieldKey);
   distance_ = StringToNumber<double>(Parameters.GetOption(DistanceValueKey));
   nrings_ = StringToNumber<int>(Parameters.GetOption(NRingsKey));
   conversionfactor_ = StringToNumber<double>(Parameters.GetOption(ConversionFactorKey));
}

/** Destructor */
VectorOperationBuilder::~VectorOperationBuilder() {
}

/** Obtiene una instancia de la clase correspondiente a la operacion que quiera realizarse
 * @param[in] Operation tipo de operacion a realizar
 * @param[out] pvop instancia de la operacion o NULL en caso de no encontrar la correspondiente */
VectorOperation* VectorOperationBuilder::GetOperation(VectorOperation::OperationType Operation) {
   /* Opciones de configuracion de buffer **/
   std::string distancefield = operationOptions_.GetOption(DistanceFieldKey);
   double distance = StringToNumber<double>(operationOptions_.GetOption(DistanceValueKey));
   int nrings = StringToNumber<int>(operationOptions_.GetOption(NRingsKey));
   double conversionfactor = StringToNumber<double>(operationOptions_.GetOption(ConversionFactorKey));
   VectorOperation* pvop = new BufferOperation(distancefield, nrings, distance, conversionfactor);
   pvop->AddSucesor(new UnionOperation());
   pvop->AddSucesor(new IntersectionOperation());
   pvop->AddSucesor(new TrimOperation());
   /** Opciones de configuracion de operacion "fusion de capas" */
   /** Formato mapa: idds=campo1,idds=campo2,iddds_2=campo1.... **/
   std::string fieldsmap = operationOptions_.GetOption(MergeFieldsMapKey);
   /** id de fuente de datos y clave nombre del campo **/
   /** un id, varios nombres de campo **/
   std::multimap<std::string, std::string> fieldsmultipmap;
   std::vector<std::string> fieldsmapvalues = tokenizer(fieldsmap, ",");
   for (std::vector<std::string>::iterator it = fieldsmapvalues.begin();
         it != fieldsmapvalues.end(); ++it) {
      std::vector<std::string> keymaptoken = tokenizer(*it, "=");
      if (keymaptoken.size() != 2)
         break;
      std::string dsid = keymaptoken[0];
      std::string dsfieldsel = keymaptoken[1];
      fieldsmultipmap.insert(std::make_pair<std::string, std::string>(dsid, dsfieldsel));
   }
   pvop->AddSucesor(new MergeLayersOperation(fieldsmultipmap));
   return pvop;
}

} /** namespace suri */
