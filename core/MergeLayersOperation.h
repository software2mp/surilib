/*! \file MergeLayersOperation.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MERGELAYERSOPERATION_H_
#define MERGELAYERSOPERATION_H_

// Includes Estandar
// Includes Suri
#include "VectorOperation.h"
// Includes Wx
// Defines
// forwards
class OGRFeatureDefn;

namespace suri {

/**
 * Procesa las fuentes de datos que recibe generando una nueva fuente de datos con los campos
 * seleccionados. Solo aplica en fuentes de datos del mismo tipo.
 */
class MergeLayersOperation : public suri::VectorOperation {
public:
   /** Constructor */
   MergeLayersOperation(std::multimap<std::string, std::string> FieldsMultipmap);
   /** Destructor */
   virtual ~MergeLayersOperation();
private:
   /**
    * Procesa las fuentes de datos de entrada generando la union de ambas
    * en una nueva fuente de datos
    */
   virtual VectorDatasource* DoProcessDatasource(VectorOperation::OperationType& Operation,
                                                 std::vector<DatasourceInterface*>& Datasources);
   /** Crea los campos seleccionados de todas las fuentes de datos en la capa destino */
   void MergeFields(std::vector<DatasourceInterface*>& Datasources, OGRLayer* pVectorLayer);
   /**
    * Recorre todos los features de todas las fuentes de datos
    * en busca de campos seleccionados en la capa destino
    */
   void MergeFeatures(std::vector<DatasourceInterface*>& Datasources, OGRLayer* pVectorLayer);
   /**
    * Setea los valores de los campos seleccionados de todas las fuentesde datos
    * en la capa destino y crea el feature
    */
   bool CreateFeature(OGRFeature* pFeature, OGRFeature* pNewFeature,
                      std::string FieldName,
                      OGRFeatureDefn* pOriginFeatureDef,
                      OGRFeatureDefn* pDestFeatureDef);
   /**
    * Contiene los nombres de los campos seleccionados para la capa destino y
    * el id de la fuente de datos a la cual corresponden
    */
   std::multimap<std::string, std::string> fieldsmultipmap_;
};

} /** namespace suri */

#endif /* MERGELAYERSOPERATION_H_ */
