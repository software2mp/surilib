/*! \file FeatureSelection.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FEATURESELECTION_H_
#define FEATURESELECTION_H_

// Includes Estandar
#include <set>

// Includes Suri
#include "suri/Subject.h"
#include "suri/FeatureSelectionSource.h"
#include "GeometryCollection.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/*!
 * Administra la seleccion de features en la aplicacion.
 * Existen dos modos de seleccion:
 * i. Para seleccionar un feature hay que pasar el id de la fuente de datos
 * y el id del feature dentro de la fuente de datos.
 * i. Para seleccionar un feature solo hay que pasar el FeatureId, el
 * FeatureSelectionSource sabe en que datasource buscar.
 * 
 * Cada FeatureSelectionSource pueden soportar uno o los dos modos.
 */
class FeatureSelection : public suri::Subject {
public:
   /** ctor **/
   FeatureSelection();
   /** dtor **/
   virtual ~FeatureSelection();

   /** Configura el GeometrySelectionSource que maneja la seleccion */
   bool Configure(FeatureSelectionSource* pSelectionSource);
   /** Retorna el id del datasource donde se seleccionana features */
   SuriObject::UuidType GetWorkingDatasourceId();

   /** Limpia la seleccion **/
   virtual void ClearSelection();
   /** Actualiza la seleccion existente notificando a los interesados **/
   virtual void UpdateSelection(const std::set<FeatureIdType>& FeaturesSelected,
                  const SuriObject::UuidType &DatasourceId = SuriObject::NullUuid);
   /** Selecciona un feature */
   virtual bool Select(FeatureIdType FeatureId,
                  const SuriObject::UuidType &DatasourceId = SuriObject::NullUuid);
   /** Deselecciona un feature */
   virtual bool Unselect(FeatureIdType FeatureId,
                  const SuriObject::UuidType &DatasourceId = SuriObject::NullUuid);
   /** Retorna vector con el id de los features seleccionados de DatasourceId */
   virtual std::vector<FeatureIdType> GetSelectedFeatures(
                  const SuriObject::UuidType &DatasourceId = SuriObject::NullUuid);

   /** Selecciona todos los features cuya geometria intersectan el subset */
   virtual bool Select(Subset &SubsetArea, const World* pWorld);
   /** Retorna las geometrias seleccionadas */
   virtual GeometryCollection* GetSelectedGeometries(const World* pWorld);

private:
   /** Clase que administra la seleccion, patron strategy */
   FeatureSelectionSource* pSelectionSource_;
};

} /** namespace suri */

#endif /* FEATURESELECTION_H_ */
