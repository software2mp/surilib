/*! \file UnionOperation.cpp */
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
#include "IntersectionOperation.h"
#include "suri/Vector.h"
#include "VectorDatasource.h"
// Includes Wx
// Defines
// forwards
// Includes otros
#include "ogrsf_frmts.h"
#include "ogr_feature.h"

namespace suri {

/** Constructor */
IntersectionOperation::IntersectionOperation() {
}

/** Destructor */
IntersectionOperation::~IntersectionOperation() {
}

/** Procesa las fuentes de datos de entrada generando la interseccion de ambas en una nueva
 * fuente de datos
 * @param[in] Operation tipo de operacion a realizar
 * @param[in] Datasources fuentes de datos sobre las que se quiere operar
 * @param[out] Fuente de datos vectorial con resultado de la operacion o NULL en caso de que
 * Operation no sea la operacion correspondiente a la clase */
VectorDatasource* IntersectionOperation::DoProcessDatasource(
      VectorOperation::OperationType& Operation, std::vector<DatasourceInterface*>& Datasources) {
   if (Operation != VectorOperation::Intersection)
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
   CreateFields(psecondlayer, pdestlayer);
   OGRFeatureDefn* pfirstlayerfeaturedef = pfirstlayer->GetLayerDefn();
   OGRFeatureDefn* pdestlayerfeaturedef = pdestlayer->GetLayerDefn();

   pfirstlayer->ResetReading();

   int firstlayercount = pfirstlayer->GetFeatureCount();
   int secondlayercount = psecondlayer->GetFeatureCount();

   for (int i = 0; i < firstlayercount; i++) {
      OGRFeature *pfeature = pfirstlayer->GetNextFeature();
      OGRGeometry* pgeom = pfeature->GetGeometryRef()->clone();
      int j = 0;
      for (j = 0; j < secondlayercount; j++) {
         OGRFeature *psecondfeature = psecondlayer->GetNextFeature();
         OGRFeature* pnewfeature = new OGRFeature(pdestlayerfeaturedef);
         OGRGeometry* psecondgeom = psecondfeature->GetGeometryRef()->clone();
         if (pgeom->Intersects(psecondgeom)) {
            OGRGeometry* poperation = pgeom->Intersection(psecondgeom);
            // Lleno el contenido del feature con los campos de las geometrias de origen
            // copio los datos de la geometria 1.
            CopyFieldsFromOrigin(pfeature, pnewfeature, pfirstlayerfeaturedef->GetFieldCount());
            // copio los campos de la geometria 2.
            CopyFieldsFromOrigin(psecondfeature, pnewfeature,
                                 pdestlayerfeaturedef->GetFieldCount(),
                                 pfirstlayerfeaturedef->GetFieldCount());
            pnewfeature->SetGeometry(poperation);
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
