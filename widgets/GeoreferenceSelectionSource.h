/** \file GeoreferenceSelectionSource.h
 *
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GEOREFERENCESELECTIONSOURCE_H_
#define GEOREFERENCESELECTIONSOURCE_H_

#include "FeatureSelectionCachedSource.h"
#include "suri/SuriObject.h"
#include "VectorDataLayer.h"
#include "GeometryCollection.h"
#include "suri/GeoreferenceTask.h"

namespace suri {

class Subset;
class World;

/**
 * Administra la seleccion mientras este activa la tarea de georreferenciacion.
 */
class GeoreferenceSelectionSource : public FeatureSelectionCachedSource {
public:
   /**
    * Ctor.
    */
   explicit GeoreferenceSelectionSource(GeoreferenceTask* pTask);

   /**
    * Dtor.
    */
   virtual ~GeoreferenceSelectionSource();

   /**
    * Retorna el id del datasource donde se seleccionan features.
    */
   virtual SuriObject::UuidType GetWorkingDatasourceId();

   /**
    * Selecciona los features cuya geometria intersecta el subset.
    */
   virtual bool Select(Subset& SubsetArea, const World* pWorld);

protected:
   GeoreferenceTask* pTask_;  // Tarea de georreferenciacion
   SuriObject::UuidType gcpTableId_;  // Id con el que se identifica a la tabla de gcps

   /**
    * Indica si un feature se puede seleccionar.
    */
   virtual bool ValidateFeature(SuriObject::UuidType DatasourceId,
                                FeatureIdType FeatureId);

   /**
    * Retorna el cache asociado a un mundo.
    */
   virtual GeometryCollection* GetSelectionCache(const World* pWorld);
};

}

#endif  // GEOREFERENCESELECTIONSOURCE_H_
