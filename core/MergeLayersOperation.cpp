/*! \file MergeLayersOperation.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "MergeLayersOperation.h"
#include "suri/Vector.h"
#include "VectorDatasource.h"
#include "suri/messages.h"
// Includes Wx
// Defines
// forwards
// Includes otros
#include "ogrsf_frmts.h"

namespace suri {

/** Constructor */
MergeLayersOperation::MergeLayersOperation(
      std::multimap<std::string, std::string> FieldsMultipmap) :
      fieldsmultipmap_(FieldsMultipmap) {
}

/** Destructor */
MergeLayersOperation::~MergeLayersOperation() {
}

/** Procesa las fuentes de datos de entrada generando la union de ambas en una nueva
 * fuente de datos
 * @param[in] Operation tipo de operacion a realizar
 * @param[in] Datasources fuentes de datos sobre las que se quiere operar
 * @param[out] Fuente de datos vectorial con resultado de la operacion o NULL en caso de que
 * Operation no sea la operacion correspondiente a la clase */
VectorDatasource* MergeLayersOperation::DoProcessDatasource(
      VectorOperation::OperationType& Operation,
      std::vector<DatasourceInterface*>& Datasources) {
   if (Operation != VectorOperation::MergeLayers)
         return NULL;
   if (!VerifyLayersType(Datasources)) {
      SHOW_ERROR(message_DIFFERENT_LAYER_TYPES);
      return NULL;
   }
   std::string tmpFilename = GetTempFile();
   Vector* pvector = Vector::Open(tmpFilename, Vector::ReadWrite);
   // Creo la capa destino
   Vector* ptarget = Datasources[0]->GetDatasourceAsVector();
   pvector->CreateLayer("MergeLayersOperation", ptarget->GetLayerSR(0),
                        ptarget->GetLayerType(0));
   OGRLayer* pvectorlayer = pvector->GetLayer(0);
   /*
    * TODO: A partir de la version 1.10 de gdal puede utilizarse la funcion union de la clase
    * OGRLayer. La forma utilizada para resolver la falta de la funcion debe recorrer dos veces el
    * vector de fuentes de datos. Una para crear los campos y otra para crear los features.
    */
   // Creo los campos
   MergeFields(Datasources, pvectorlayer);
   // Escribo el valor de los campos
   MergeFeatures(Datasources, pvectorlayer);
   Vector::Close(pvector);
   return VectorDatasource::Create(tmpFilename);
}

/**
 * Crea los campos seleccionados de todas las fuentes de datos en la capa destino
 * @param[in] Datasources fuentes de datos de origen
 * @param[in] pVectorLayer Capa destino
 */
void MergeLayersOperation::MergeFields(std::vector<DatasourceInterface*>& Datasources,
                                       OGRLayer* pVectorLayer) {
   Vector* ptarget = NULL;
   OGRLayer* player = NULL;
   for (std::vector<DatasourceInterface*>::iterator it = Datasources.begin();
         it != Datasources.end(); ++it) {
      ptarget = (*it)->GetDatasourceAsVector();
      player = ptarget->GetLayer(dynamic_cast<VectorDatasource*>(*it)->GetActiveLayer());
      std::pair<std::multimap<std::string, std::string>::iterator,
            std::multimap<std::string, std::string>::iterator> ret;
      ret = fieldsmultipmap_.equal_range((*it)->GetId());
      // 1. Obtengo la definicion del campo en la fd origen
      for (std::multimap<std::string, std::string>::iterator mit = ret.first;
            mit != ret.second; ++mit) {
         OGRFeatureDefn* pfeaturedef = pVectorLayer->GetLayerDefn();
         OGRFeature* pfeature = new OGRFeature(pfeaturedef);
         if (mit->first == (*it)->GetId() && pfeature->GetFieldIndex(mit->second.c_str()) < 0) {
            // 2. Creo el campo con esa def en la fg destino
            //OGRFeatureDefn* pfeaturedef = player->GetLayerDefn();
            pfeaturedef = player->GetLayerDefn();
            int fieldcount = pfeaturedef->GetFieldCount();
            //OGRFeature* pfeature = new OGRFeature(pfeaturedef);
            pfeature = new OGRFeature(pfeaturedef);
            int fieldindex = pfeature->GetFieldIndex(mit->second.c_str());
            for (int i = 0; i < fieldcount; ++i)
               if (i == fieldindex)
                  pVectorLayer->CreateField(pfeaturedef->GetFieldDefn(i));
            //delete pfeature;
         }
         delete pfeature;
      }
   }
   // 3. Agrego campo origen (en cada feature se pone el nombre de la fd origen)
   const char* origin = "Origen";
   OGRFieldDefn* pfielddefn = new OGRFieldDefn(origin, OFTString);
   pVectorLayer->CreateField(pfielddefn);
   delete pfielddefn;
}

/**
 * Recorre todos los features de todas las fuentes de datos
 * en busca de campos seleccionados en la capa destino
 */
void MergeLayersOperation::MergeFeatures(std::vector<DatasourceInterface*>& Datasources,
                                         OGRLayer* pVectorLayer) {
   std::vector<DatasourceInterface*>::iterator it = Datasources.begin();
   //
   for (int dsindex = 0; it != Datasources.end(); ++it, ++dsindex) {
      Vector* poriginvector = (*it)->GetDatasourceAsVector();
      OGRLayer* poriginlayer = poriginvector->GetLayer(
            dynamic_cast<VectorDatasource*>(*it)->GetActiveLayer());

      std::pair<std::multimap<std::string, std::string>::iterator,
            std::multimap<std::string, std::string>::iterator> ret;

      ret = fieldsmultipmap_.equal_range((*it)->GetId());

      // TODO: Optimizacion: si equal_range no devuelve ninguna coincidencia => continue
      OGRFeature* pfeature = poriginlayer->GetNextFeature();
      OGRFeatureDefn* pdestfeaturedef = pVectorLayer->GetLayerDefn();
      OGRFeatureDefn* poriginfeaturedef = poriginlayer->GetLayerDefn();

      // Se copia desde el feature origen los campos seleccionados al feature destino
      while (pfeature) {
         OGRFeature* pnewfeature =  new OGRFeature(pdestfeaturedef);
         bool hasfields = false;
         for (std::multimap<std::string, std::string>::iterator fieldselectionit = ret.first;
               fieldselectionit != ret.second; ++fieldselectionit) {
            std::string fieldname = fieldselectionit->second;
            hasfields = CreateFeature(pfeature, pnewfeature, fieldname,
                                           poriginfeaturedef, pdestfeaturedef);
         }
         // Solo creo el feature si se seteo algun campo en la capa destino
         if (hasfields) {
            pnewfeature->SetField(pdestfeaturedef->GetFieldIndex("Origen"),
                                  (*it)->GetName().c_str());
            pnewfeature->SetGeometry(pfeature->GetGeometryRef());
            // Guardo el feature en la capa destino
            pVectorLayer->CreateFeature(pnewfeature);
         }
         delete pnewfeature;
         pfeature = poriginlayer->GetNextFeature();
      }
   }
}

/**
 * Setea los valores de los campos seleccionados de todas las fuentes de datos
 * en la capa destino
 */
bool MergeLayersOperation::CreateFeature(OGRFeature* pFeature, OGRFeature* pNewFeature,
                                         std::string FieldName,
                                         OGRFeatureDefn* pOriginFeatureDef,
                                         OGRFeatureDefn* pDestFeatureDef) {
   bool hasfields = false;
   int originindex = pFeature->GetFieldIndex(FieldName.c_str());
   OGRFieldType fieldtype = pOriginFeatureDef->GetFieldDefn(originindex)->GetType();
   int destinyindex = pDestFeatureDef->GetFieldIndex(FieldName.c_str());
   switch (fieldtype) {
      case OFTInteger:
      {
         int fieldcontent = pFeature->GetFieldAsInteger(originindex);
         pNewFeature->SetField(destinyindex, fieldcontent);
         hasfields = true;
      }
         break;
      case OFTString:
      {
         std::string fieldcontent = pFeature->GetFieldAsString(originindex);
         pNewFeature->SetField(destinyindex, fieldcontent.c_str());
         hasfields = true;
      }
         break;
      case OFTReal:
      {
         double fieldcontent = pFeature->GetFieldAsDouble(originindex);
         pNewFeature->SetField(destinyindex, fieldcontent);
         hasfields = true;
      }
         break;
      default:
      {
         std::string fieldcontent = pFeature->GetFieldAsString(originindex);
         pNewFeature->SetField(destinyindex, fieldcontent.c_str());
         hasfields = true;
      }
         break;
   }
   return hasfields;
}
} /** namespace suri */
