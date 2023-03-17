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
