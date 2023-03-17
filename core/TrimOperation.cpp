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
#include "TrimOperation.h"
#include "VectorDatasource.h"
#include "suri/Vector.h"
// Includes Wx
// Defines
// forwards
// Includes otros
#include "ogrsf_frmts.h"
#include "ogr_feature.h"

namespace suri {

/** Constructor */
TrimOperation::TrimOperation() {
}

/** Destructor */
TrimOperation::~TrimOperation() {
}

/** Procesa las fuentes de datos de entrada generando la diferencia de las capas en una nueva
 * fuente de datos
 * @param[in] Operation tipo de operacion a realizar
 * @param[in] Datasources fuentes de datos sobre las que se quiere operar
 * @param[out] Fuente de datos vectorial con resultado de la operacion o NULL en caso de que
 * Operation no sea la operacion correspondiente a la clase */
VectorDatasource* TrimOperation::DoProcessDatasource(
      VectorOperation::OperationType& Operation, std::vector<DatasourceInterface*>& Datasources) {
   if (Operation != VectorOperation::Trim)
      return NULL;
   std::string tmpFilename = GetTempFile();
   Vector* pdesttarget = Vector::Open(tmpFilename, Vector::ReadWrite);
   Vector* pfirsttarget = Datasources[0]->GetDatasourceAsVector();
   Vector* psecondtarget = Datasources[1]->GetDatasourceAsVector();

   if (!pdesttarget || !pdesttarget->GetVectorDataSource())
      return NULL;
   OGRLayer* pfirstlayer = pfirsttarget->GetLayer(0);
   OGRLayer* psecondlayer = psecondtarget->GetLayer(0);
   if (!pfirstlayer || !psecondlayer)
      return NULL;
   Vector::VectorType type = pfirsttarget->GetLayerType(0);
   OGRLayer* pdestlayer = pdesttarget->GetLayer(0);
   if (pdestlayer)
      pdesttarget->DeleteLayer(0);
   char* pwkt;
   pfirstlayer->GetSpatialRef()->exportToWkt(&pwkt);
   pdestlayer = pdesttarget->CreateLayer(pfirstlayer->GetName(), pwkt, type);
   OGRFree(pwkt);
   CreateFields(pfirstlayer, pdestlayer);

   OGRFeatureDefn* pfirstlayerfeaturedef = pfirstlayer->GetLayerDefn();

   pfirstlayer->ResetReading();

   int firstlayercount = pfirstlayer->GetFeatureCount();
   int secondlayercount = psecondlayer->GetFeatureCount();
   OGRFeatureDefn* pdestlayerfeaturedef = pdestlayer->GetLayerDefn();
   for (int i = 0; i < firstlayercount; ++i) {
      OGRFeature *pfeature = pfirstlayer->GetNextFeature();
      OGRGeometry* pgeom = pfeature->GetGeometryRef()->clone();
      int j = 0;
      bool lastfigure = false;
      for (j = 0; j < secondlayercount; ++j) {
         OGRFeature *psecondfeature = psecondlayer->GetNextFeature();
         OGRFeature* pnewfeature = new OGRFeature(pdestlayerfeaturedef);
         OGRGeometry* psecondgeom = psecondfeature->GetGeometryRef()->clone();
         if (j == secondlayercount - 1)
            lastfigure = true;
         if (pgeom->Intersects(psecondgeom)) {
            pgeom = pgeom->Difference(psecondgeom);
         }
         if (lastfigure) {
            // Lleno el contenido del feature con los campos de las geometrias de origen
            // copio los datos de la geometria 1.
            CopyFieldsFromOrigin(pfeature, pnewfeature, pfirstlayerfeaturedef->GetFieldCount());
            pnewfeature->SetGeometry(pgeom);
            pdestlayer->CreateFeature(pnewfeature);
         }
         OGRFeature::DestroyFeature(pnewfeature);
      }
      psecondlayer->ResetReading();
   }

   Vector::Close(pdesttarget);

   return VectorDatasource::Create(tmpFilename);
}

} /** namespace suri */
