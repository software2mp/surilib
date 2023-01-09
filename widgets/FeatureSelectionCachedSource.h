/** \file FeatureSelectionCachedSource.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FEATURESELECTIONCACHEDSOURCE_H_
#define FEATURESELECTIONCACHEDSOURCE_H_

// Includes standard
#include <map>

// Include suri
#include "suri/FeatureSelectionSource.h"

namespace suri {

class FeatureSelectionCachedSource : public FeatureSelectionSource {
public:
   /** ctor */
   FeatureSelectionCachedSource();
   /** Dtor */
   virtual ~FeatureSelectionCachedSource();

   /** Limpia la seleccion **/
   virtual void ClearSelection();
   /** Selecciona un feature */
   virtual bool Select(FeatureIdType FeatureId,
                  SuriObject::UuidType DatasourceId);
   /** Deselecciona un feature */
   virtual bool Unselect(FeatureIdType FeatureId,
                  SuriObject::UuidType DatasourceId);

   /** Retorna las geometrias seleccionadas */
   virtual GeometryCollection* GetSelectedGeometries(const World* pWorld);

protected:
   /** Retorna el cache asociado a un mundo */
   virtual GeometryCollection* GetSelectionCache(const World* pWorld) = 0;
   /** Limpia el cache, GetSelectedGeometries buscara las geometrias en vector */
   virtual void ClearSelectionCache();

   /** Mapa con el cache asignado a cada mundo. */
   std::map<const World*, GeometryCollection*> cache_;
};

} /* namespace suri */
#endif /* FEATURESELECTIONCACHEDSOURCE_H_ */
