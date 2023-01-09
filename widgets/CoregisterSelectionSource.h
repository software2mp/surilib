/** \file CoregisterSelectionSource.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COREGISTERSELECTIONSOURCE_H_
#define COREGISTERSELECTIONSOURCE_H_

// Includes suri
#include "VectorDataLayerSelectionSource.h"
#include "suri/CoregisterTask.h"

namespace suri {

/**
 * Administra la seleccion mientras este activa la tarea de coregistro.
 */
class CoregisterSelectionSource : public FeatureSelectionCachedSource {
public:
   /** Ctor */
   explicit CoregisterSelectionSource(CoregisterTask* pTask);
   /** Dtor */
   virtual ~CoregisterSelectionSource();

   /** Retorna el id del datasource donde se seleccionan features. */
   virtual SuriObject::UuidType GetWorkingDatasourceId();

   /** Selecciona los features cuya geometria intersecta el subset */
   virtual bool Select(Subset &SubsetArea, const World* pWorld);

protected:
   /** Indica si un feature se puede seleccionar */
   virtual bool ValidateFeature(SuriObject::UuidType DatasourceId,
                                                FeatureIdType FeatureId);

   /** Retorna el cache asociado a un mundo */
   virtual GeometryCollection* GetSelectionCache(const World* pWorld);
   /** Tarea de coregistro */
   CoregisterTask* pTask_;
   /** Id con el que se identifica a la tabla de gcps */
   SuriObject::UuidType gcpTableId_;
};

} /* namespace suri */
#endif /* COREGISTERSELECTIONSOURCE_H_ */
