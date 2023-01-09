/** \file VectorDatasourceSelectionSource.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTORDATASOURCESELECTIONSOURCE_H_
#define VECTORDATASOURCESELECTIONSOURCE_H_

// Includes standard
#include <string>

// Includes suri
#include "suri/FeatureSelectionSource.h"
#include "FilteredDatasource.h"

namespace suri {

// fordwards
class Vector;
class VectorStyle;

/**
 * Permite seleccionar features de una fuente de datos vectorial.
 * Si pFiltredDatasource que recibe en constructor != NULL, cada vez
 * que se ocurre un cambio, actualiza el filtro para que coincida 
 * con la seleccion.
 */
class VectorDatasourceSelectionSource : public FeatureSelectionSource {
public:
   /** Ctor */
   VectorDatasourceSelectionSource(VectorDatasource *pDatasource,
                                    FilteredDatasource* pFiltredDatasource);
   /** Dtor */
   virtual ~VectorDatasourceSelectionSource();

   /** Retorna el id del datasource donde se seleccionana features. */
   virtual SuriObject::UuidType GetWorkingDatasourceId();

   /** Limpia la seleccion **/
   virtual void ClearSelection();
   /** Selecciona un feature */
   virtual bool Select(FeatureIdType FeatureId,
                  SuriObject::UuidType DatasourceId = SuriObject::NullUuid);
   /** Deselecciona un feature */
   virtual bool Unselect(FeatureIdType FeatureId,
                  SuriObject::UuidType DatasourceId = SuriObject::NullUuid);

   /** Selecciona los features cuya geometria intersecta el subset */
   virtual bool Select(Subset &SubsetArea, const World* pWorld);
   /** Retorna las geometrias seleccionadas */
   virtual GeometryCollection* GetSelectedGeometries(const World* pWorld);

protected:
   /** Indica si un feature se puede seleccionar */
   virtual bool ValidateFeature(SuriObject::UuidType DatasourceId,
                                                FeatureIdType FeatureId);
   /** Actualiza la seleccion en datasource */
   bool UpdateDatasourceFilter();

   /** Fuente de datos que posee el filtro de seleccion **/
   FilteredDatasource* pFiltredDatasource_;
   /** Fuente de datos vectorial sobre el que se estan seleccionando features */
   VectorDatasource *pDatasource_;
   /** Vector asociado a fuente de datos */
   Vector* pVector_;

   /** Cache con la seleccion, evita buscar en vector si no se actualizo seleccion. */
   GeometryCollection* pSelectedGeometries_;

   /** Estilo de geometrias seleccionada */
   VectorStyle* pSelectedGeometryStyle_;

   /** Valor que se le pasa a filtro para filtrar todas las capas */
   static const int INVALID_FEATUREID = -1;
   /** nombre del campo que contiene los features ids  */
   static const std::string FEATURE_ID_FIELD_NAME;
};

} /* namespace suri */
#endif /* VECTORDATASOURCESELECTIONSOURCE_H_ */
