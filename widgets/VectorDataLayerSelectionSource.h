/** \file VectorDataLayerSelectionSource.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTORDATALAYERSELECTIONSOURCE_H_
#define VECTORDATALAYERSELECTIONSOURCE_H_

// Includes standard
#include <string>

// Includes suri
#include "FeatureSelectionCachedSource.h"
#include "VectorDataLayer.h"

namespace suri {

// fordwards
class VectorStyle;

/**
 * Permite seleccionar features dentro del vector en edicion.
 * Justificacion:
 *     Al editar, los cambios realizados al vector se guardan dentro de
 *     VectorDataLayerSelectionSource, por esta razon, no se puede usar el
 *     datasource directamente.
 */
class VectorDataLayerSelectionSource : public FeatureSelectionCachedSource {
public:
   /** Ctor */
   explicit VectorDataLayerSelectionSource(VectorDataLayer* pVectorDataLayer,
                                             SuriObject::UuidType DatasourceId);
   /** Dtor */
   virtual ~VectorDataLayerSelectionSource();

   /** Retorna el id del datasource donde se seleccionan features. */
   virtual SuriObject::UuidType GetWorkingDatasourceId();

   /** Selecciona los features cuya geometria intersecta el subset */
   virtual bool Select(Subset &SubsetArea, const World* pWorld);

protected:
   /** Retorna el cache asociado a un mundo */
   virtual GeometryCollection* GetSelectionCache(const World* pWorld);
   /** Indica si un feature se puede seleccionar */
   virtual bool ValidateFeature(SuriObject::UuidType DatasourceId,
                                                FeatureIdType FeatureId);

   /** VectorDataLayer que se usa como fuente de datos */
   VectorDataLayer* pVectorDataLayer_;
   /** Id del datasource editado */
   SuriObject::UuidType datasourceId_;
};

} /* namespace suri */
#endif /* VECTORDATALAYERSELECTIONSOURCE_H_ */
