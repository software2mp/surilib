/** \file GeometryEditionState.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GEOMETRYEDITIONSTATE_H_
#define GEOMETRYEDITIONSTATE_H_

// Includes standard
#include <string>

// Includes suri
#include "suri/Table.h"
#include "VectorDataLayer.h"

namespace suri {

/**
 * Clase que administra la edicion de geometrias de una tabla. No se puede
 * editar mas de una geometria al mismo tiempo.
 * No tiene logica de commit, por lo que el cliente es el encargado
 * de persistir los cambios en la tabla.
 */
class GeometryColumnEditionState {
public:
   /** Ctor */
   GeometryColumnEditionState(Table* pTable, const std::string &LayerSrs,
                                       const std::string &GeometryColumnName);
   /** Dtor */
   ~GeometryColumnEditionState();

   /** Inicia la edicion de un feature */
   bool StartFeatureEdition(FeatureIdType FeatureId);
   /** Finaliza la edicion */
   bool EndFeatureEdition(bool SaveChanges);
   /** Retorna la geometria en el feature que se esta editando/creando */
   Geometry* GetEditedGeometry();
   /** Configura la geometria en el feature que se esta editando/creando */
   bool SetEditedGeometry(Geometry* pGeometry);

   /** Indica si se esta editando */
   bool IsEditing() const;
   /** Indica si la geometria editada es valida */
   bool IsValid() const;

private:
   /** DataLayer que permite obtener geometrias de capa */
   VectorDataLayer* pDataLayer_;
   /** Feature que se esta editando */
   VectorDataLayer::FeatureData* pEditedFeature_;
};

} /* namespace suri */
#endif /* GEOMETRYEDITIONSTATE_H_ */
