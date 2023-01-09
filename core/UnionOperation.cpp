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
#include "UnionOperation.h"
#include "suri/Vector.h"
#include "VectorDatasource.h"
// Includes Wx
// Defines
// forwards
// Includes otros
#include "ogrsf_frmts.h"

const std::string POLYGON = "POLYGON";

namespace suri {

/** Constructor */
UnionOperation::UnionOperation() {
}

/** Destructor */
UnionOperation::~UnionOperation() {
}

/** Procesa las fuentes de datos de entrada generando la union de ambas en una nueva
 * fuente de datos
 * @param[in] Operation tipo de operacion a realizar
 * @param[in] Datasources fuentes de datos sobre las que se quiere operar
 * @param[out] Fuente de datos vectorial con resultado de la operacion o NULL en caso de que
 * Operation no sea la operacion correspondiente a la clase */
VectorDatasource* UnionOperation::DoProcessDatasource(VectorOperation::OperationType& Operation,
                                                  std::vector<DatasourceInterface*>& Datasources) {
   if (Operation != VectorOperation::Union)
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
   OGRLayer* pdestlayer = pdesttarget->GetLayer(0);
   if (pdestlayer)
      pdesttarget->DeleteLayer(0);
   char* pwkt;
   pfirstlayer->GetSpatialRef()->exportToWkt(&pwkt);
   pdestlayer = pdesttarget->CreateLayer(
         pfirstlayer->GetName(), pwkt, pfirsttarget->GetLayerType(0));

   // Creo los campos de las capas en la capa destino
   CreateFields(pfirstlayer, pdestlayer);
   CreateFields(psecondlayer, pdestlayer);

   OGRFeatureDefn* pfirstlayerfeaturedef = pfirstlayer->GetLayerDefn();

   OGRFree(pwkt);

   pfirstlayer->ResetReading();
   psecondlayer->ResetReading();

   int firstlayercount = pfirstlayer->GetFeatureCount();
   int secondlayercount = psecondlayer->GetFeatureCount();


   /** mapa con geometrias que tuvieron alguna operacion
    *  Se agrupan por numero de geometria. Luego se deberia tomar
    *  todas las goemtrias con el mismo numero y realizar una
    *  interseccion entre ellas
    */
   std::multimap<int, OGRGeometry*> pendinggeoms;
   OGRFeatureDefn* pdestlayerfeaturedef = pdestlayer->GetLayerDefn();

   for (int i = 0; i < firstlayercount; ++i) {
      OGRFeature *pfeature = pfirstlayer->GetNextFeature();
      OGRGeometry* pgeom = pfeature->GetGeometryRef()->clone();
      std::string geomname = pgeom->getGeometryName();
      for (int j = 0; j < secondlayercount; ++j) {
         OGRFeature *psecondfeature = psecondlayer->GetNextFeature();
         OGRGeometry* pgeom2 = psecondfeature->GetGeometryRef()->clone();
         // Los unicos casos que tienen que contemplarse en el if son los de los poligonos
         if ((pgeom->Intersects(pgeom2) || pgeom2->Intersects(pgeom)) &&
               geomname.compare(POLYGON) == 0 &&
               (!pgeom->Contains(pgeom2) && !pgeom2->Contains(pgeom))) {
            /**
             * Ademas de la resta surgen 2 geometrias:
             * 1. La interseccion
             * 2. La resta geom2 - geom1
             */

            // Geometria 1.
            OGRGeometry* pintersectgeom = pfeature->GetGeometryRef()->clone();
            pintersectgeom = pintersectgeom->Intersection(pgeom2);
            // Se debe generar un nuevo feature con los campos de la capa destino
            OGRFeature* pintersectfeature = new OGRFeature(pdestlayerfeaturedef);
            pintersectfeature->SetGeometryDirectly(pintersectgeom);
            // Lleno el contenido del feature con los campos de las geometrias de origen
            // copio los datos de la geometria 1.
            CopyFieldsFromOrigin(pfeature, pintersectfeature,
                                 pfirstlayerfeaturedef->GetFieldCount());

            // copio los campos de la geometria 2.
            CopyFieldsFromOrigin(psecondfeature, pintersectfeature,
                                 pdestlayerfeaturedef->GetFieldCount(),
                                 pfirstlayerfeaturedef->GetFieldCount());

            // creo el feature en la capa destino
            pdestlayer->CreateFeature(pintersectfeature);

            // Geometria 2.
            pendinggeoms.insert(
                  std::make_pair(j, pgeom2->Difference(pfeature->GetGeometryRef())));

            pgeom = pgeom->Difference(pgeom2);

            OGRFeature::DestroyFeature(pintersectfeature);
         } else {
            // Se debe generar un nuevo feature con los campos de la capa destino
            // Insertar geometria en multimap. Si se hace una vez sola entonces
            // se copia bien la geometria.
            // Si se inserta varias veces tambien pero se interseca contra si misma
            pendinggeoms.insert(std::make_pair(j, pgeom2));
         }
      }
      // Se debe generar un nuevo feature con los campos de la capa destino
      OGRFeature* pnewfeature = new OGRFeature(pdestlayerfeaturedef);
      pnewfeature->SetGeometryDirectly(pgeom);
      // Lleno el contenido del feature con los campos de las geometrias de origen
      // copio los datos de la geometria 1.
      CopyFieldsFromOrigin(pfeature, pnewfeature, pfirstlayerfeaturedef->GetFieldCount());
      pdestlayer->CreateFeature(pnewfeature);
      OGRFeature::DestroyFeature(pnewfeature);
      psecondlayer->ResetReading();
   }

   // Recorro el multimap y interseco por grupo
   DoGroupIntersection(pendinggeoms, pfirstlayer, psecondlayer, pdestlayer);

   Vector::Close(pdesttarget);

   return VectorDatasource::Create(tmpFilename);
}

/** Realiza la interseccion de las geometrias sin generar duplicados
 *  @param[in] PendingGeoms: multimap con geometrias pendientes de procesar
 *  @param[in] pLayer: primer capa origen
 *  @param[in] pSecondLayer: segunda capa origen
 *  @param[in] pDestLayer: capa destino
 * */
void UnionOperation::DoGroupIntersection(std::multimap<int, OGRGeometry*> PendingGeoms,
                                         OGRLayer* pLayer, OGRLayer* pSecondLayer,
                                         OGRLayer* pDestLayer) {
   OGRFeatureDefn* pdestlayerfeaturedef = pDestLayer->GetLayerDefn();
   OGRFeatureDefn* pfirstlayerfeaturedef = pLayer->GetLayerDefn();
   int layercount = 0;
   if (pLayer->GetFeatureCount() > pSecondLayer->GetFeatureCount())
      layercount = pLayer->GetFeatureCount();
   else
      layercount = pSecondLayer->GetFeatureCount();
   for (int g = 0; g < layercount; ++g) {
      std::pair<std::multimap<int, OGRGeometry*>::iterator,
            std::multimap<int, OGRGeometry*>::iterator> ret;
      ret = PendingGeoms.equal_range(g);
      OGRGeometry* pgeom = NULL;
      for (std::multimap<int, OGRGeometry*>::iterator it = ret.first; it != ret.second; ++it) {
         pgeom = pgeom ? pgeom->Intersection(it->second) : it->second;
      }
      if (pgeom != NULL) {
         // Se debe generar un nuevo feature con los campos de la capa destino
         OGRFeature *psecondfeature = pSecondLayer->GetNextFeature();
         OGRFeature* pnewfeature = new OGRFeature(pdestlayerfeaturedef);
         pnewfeature->SetGeometryDirectly(pgeom);

         // copio los campos de la geometria 2.
         CopyFieldsFromOrigin(psecondfeature, pnewfeature,
                              pdestlayerfeaturedef->GetFieldCount(),
                              pfirstlayerfeaturedef->GetFieldCount());
         pDestLayer->CreateFeature(pnewfeature);
         OGRFeature::DestroyFeature(pnewfeature);
      }
   }
}

} /** namespace suri */
