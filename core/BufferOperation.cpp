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
#include "BufferOperation.h"
#include "suri/Vector.h"
#include "VectorDatasource.h"
#include "SpatialReference.h"
// Includes Wx
// Defines
// forwards
// Includes otros
#include "ogrsf_frmts.h"
#include "ogr_feature.h"

#define BUFFER_ORIGIN_FIELD_NAME "Origen"
#define BUFFER_DISTANCE_FIELD_NAME "DistBufer"
#define BUFFER_ID_FIELD_NAME "ID"

namespace suri {

/** Constructor */
BufferOperation::BufferOperation(const std::string& FieldName, int Nrings, double distance,
                                 double ConversionFactor) : bufferfield_(FieldName),
                                       distancevalue_(distance), nrings_(Nrings),
                                       conversionfactor_(ConversionFactor) {
}

/** Destructor */
BufferOperation::~BufferOperation() {
}

/** Procesa la fuente de datos de entrada generando un buffer en una nueva fuente de datos
 * @param[in] Operation tipo de operacion a realizar
 * @param[in] Datasources fuente de datos sobre la que se quiere operar
 * @param[out] Fuente de datos vectorial con resultado de la operacion o NULL en caso de que
 * Operation no sea la operacion correspondiente a la clase */
VectorDatasource* BufferOperation::DoProcessDatasource(VectorOperation::OperationType& Operation,
                                                  std::vector<DatasourceInterface*>& Datasources) {
   if (Operation != VectorOperation::Buffer)
      return NULL;
   bool isfromfield = false;
   if (!bufferfield_.empty())
      isfromfield = true;
   std::string tmpFilename = GetTempFile();
   Vector* pdesttarget = Vector::Open(tmpFilename, Vector::ReadWrite);
   Vector* pfirsttarget = Datasources[0]->GetDatasourceAsVector();

   if (!pdesttarget || !pdesttarget->GetVectorDataSource())
      return NULL;
   OGRLayer* player = pfirsttarget->GetLayer(0);

   if (!player)
      return NULL;
   OGRLayer* pdestlayer = pdesttarget->GetLayer(0);
   if (pdestlayer)
      pdesttarget->DeleteLayer(0);
   char* pwkt;
   player->GetSpatialRef()->exportToWkt(&pwkt);
   pdestlayer = pdesttarget->CreateLayer(player->GetName(), pwkt, Vector::Polygon);
   double distance = GetDistance(distancevalue_, pwkt);
   OGRFeatureDefn* pfeaturedef = player->GetLayerDefn();
   for (int i = 0; i < pfeaturedef->GetFieldCount(); ++i) {
      pdestlayer->CreateField(pfeaturedef->GetFieldDefn(i));
   }

   // Agrega los campos origen y distancia de buffer en el feature origen temporal
   OGRFieldDefn originfield(BUFFER_ORIGIN_FIELD_NAME, OFTString);
   OGRFieldDefn distancefield(BUFFER_DISTANCE_FIELD_NAME, OFTReal);
   pdestlayer->CreateField(&originfield);
   pdestlayer->CreateField(&distancefield);
   OGRFeatureDefn* pdestfeaturedef = pdestlayer->GetLayerDefn();
   int originfieldindex = pdestfeaturedef->GetFieldIndex(BUFFER_ORIGIN_FIELD_NAME);
   int distancefielddindex = pdestfeaturedef->GetFieldIndex(BUFFER_DISTANCE_FIELD_NAME);
   int idfieldindex = pdestfeaturedef->GetFieldIndex(BUFFER_ID_FIELD_NAME);

   player->ResetReading();
   OGRFeature *pfeature = player->GetNextFeature(); // Apunta al primer feature
   OGRGeometry* plastbuff = NULL;
   // Sumo 1 a quantity para empezar i = 1 ya que si es igual a 0 la primer distancia esta mal
   nrings_++;
   int id = 1;
   while (pfeature) {
      OGRFeature* pnewfeature = pfeature->Clone();
      for (int i = 1; i < nrings_; i++) {
         if (isfromfield) {
            distance = GetDistance(pfeature->GetFieldAsDouble(
                  pfeature->GetFieldIndex(bufferfield_.c_str())), pwkt);
         }
         OGRGeometry* pnewbuff = pnewfeature->GetGeometryRef()->Buffer(distance);
         OGRGeometry* plastauxbuff = pnewbuff;
         if (plastbuff != NULL)
            pnewbuff = pnewbuff->Difference(plastbuff);

         plastbuff = plastauxbuff;
         pnewfeature->SetGeometry(pnewbuff);

         pdestlayer->CreateFeature(pnewfeature);
         OGRFeature* pfea = pdestlayer->GetFeature(pnewfeature->GetFID());
         if (pfea) {
            double dist = i * distance;
            pfea->SetField(idfieldindex, id);
            pfea->SetField(distancefielddindex, dist);
            pfea->SetField(originfieldindex, player->GetName());
            pdestlayer->SetFeature(pfea);
            id++;
         }
      }
      OGRFeature::DestroyFeature(pnewfeature);
      pfeature = player->GetNextFeature();
      plastbuff = NULL;
   }
   OGRFree(pwkt);
   Vector::Close(pdesttarget);
   return VectorDatasource::Create(tmpFilename);
}

/**
 * Retorna el valor de la distancia
 * @param[in] Distance valor de la distancia
 * @param[in] pWkt sistema de referencia
 * @return distancia correspondiente al tipo de sistema de referencia
 */
double BufferOperation::GetDistance(double Distance, char* pWkt) {
   // 111120 es el factor de conversion utilizado para pasar de grados decimales a metros
   if (SpatialReference::IsProjectedSpatialRef(pWkt))
      return Distance * conversionfactor_;
   else if (SpatialReference::IsGeographicSpatialRef(pWkt))
      return Distance * conversionfactor_ / 111120.0;
   else
      return 0;
}

} /** namespace suri */
