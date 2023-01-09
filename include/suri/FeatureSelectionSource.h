/** \file FeatureSelectionSource.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FEATURESELECTIONSOURCE_H_
#define FEATURESELECTIONSOURCE_H_

// Includes standard
#include <vector>
#include <string>

// Includes suri
#include "GeometryCollection.h"
#include "VectorDataLayer.h"
#include "suri/Subset.h"
#include "suri/World.h"

namespace suri {

// fordwards
class VectorStyle;

/**
 * Permite definir una estrategia de seleccion. Lo usa FeatureSelection
 * (patron strategy).
 */
class FeatureSelectionSource {
public:
   /** Ctor */
   FeatureSelectionSource();
   /** Dtor */
   virtual ~FeatureSelectionSource();

   /**
    * Retorna el id del datasource donde se seleccionan features.
    * NULL si el FeatureSelectionSource concreto no tiene un datasource default.
    */
   virtual SuriObject::UuidType GetWorkingDatasourceId() = 0;

   /** Limpia la seleccion **/
   virtual void ClearSelection();
   /** Selecciona un feature */
   virtual bool Select(FeatureIdType FeatureId,
                           SuriObject::UuidType DatasourceId);
   /** Deselecciona un feature */
   virtual bool Unselect(FeatureIdType FeatureId,
                           SuriObject::UuidType DatasourceId);
   /** Retorna vector con el id de los features seleccionados */
   std::vector<FeatureIdType> GetSelectedFeatures(
                        const SuriObject::UuidType &DatasourceId);

   /** Selecciona los features cuya geometria intersecta el subset */
   virtual bool Select(Subset &SubsetArea, const World* pWorld) = 0;
   /** Retorna las geometrias seleccionadas */
   virtual GeometryCollection* GetSelectedGeometries(const World* pWorld) = 0;

protected:
   struct FeatureUuid {
      FeatureUuid(SuriObject::UuidType DatasourceId, FeatureIdType FeatureId) {
         datasourceId_ = DatasourceId;
         featureId_ = FeatureId;
      }
      SuriObject::UuidType datasourceId_;
      FeatureIdType featureId_;
   };

   /** Indica si un feature se puede seleccionar */
   virtual bool ValidateFeature(SuriObject::UuidType DatasourceId,
                                                FeatureIdType FeatureId) = 0;
   /** Retorna iterador correspondiente a feature */
   std::vector<FeatureUuid>::iterator FindFeature(SuriObject::UuidType DatasourceId,
                                                              FeatureIdType FeatureId);

   /** Vector con el id de los features seleccionados */
   std::vector<FeatureUuid> selectedFeatures_;
   /** Estilo de geometrias seleccionada */
   VectorStyle* pSelectedGeometryStyle_;
};

} /* namespace suri */
#endif /* FEATURESELECTIONSOURCE_H_ */
