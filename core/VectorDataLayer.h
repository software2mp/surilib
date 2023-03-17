/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

#ifndef VECTORDATALAYER_H_
#define VECTORDATALAYER_H_

// Includes standard
#include <vector>
#include <string>

// Includes suri
#include "Geometry.h"
#include "suri/Subset.h"
#include "suri/Vector.h"
#include "suri/Table.h"
#include "VectorDatasource.h"
#include "suri/VectorEditorDriver.h"
#include "NewRowNotification.h"
#include "suri/ObserverInterface.h"

// fordwards
class OGRLayer;

namespace suri {

// fordwardsWS
class VectorDatasource;
class BufferedDriver;

/** Tipo campo FeatureId en capa */
typedef long FeatureIdType;

/**
 * Clase que representa una 'capa de datos' vectorial. Tiene asociado una
 * fuente de datos de tipo vectorial.
 * Nota: No confundir 'capa de datos' con 'capa' que tiene datos de
 * visualizacion en un visualizador.
 */
class VectorDataLayer : public ObserverInterface {
public:
   /** Clase que guarda un par FeatureId - Geometria */
   class FeatureData {
   public:
      /** Ctor */
      FeatureData(FeatureIdType FeatureId, Geometry* pGeometry) :
                              featureId_(FeatureId), pGeometry_(pGeometry) {}
      /** Dtor */
      ~FeatureData() {
         delete pGeometry_;
         pGeometry_ = NULL;
      }
      /** Id del feature */
      FeatureIdType featureId_;
      /** Puntero a la geometria */
      Geometry* pGeometry_;
   };

   /** Ctor */
   explicit VectorDataLayer(VectorDatasource *pDatasource);
   /** constructor alternativo */
   VectorDataLayer(Table* pTable, const std::string& GeometryColumn);
   /** Ctor */
   VectorDataLayer(Table* pTable, const std::string& VectorSrs,
                                             const std::string& GeometryColumn);

   /** Dtor */
   virtual ~VectorDataLayer();

   /** Retorma la tabla que permite editar los campos de la capa */
   Table* GetTable();
   /** Retorma el id del datasource asociado al VectorDataLayer */
   SuriObject::UuidType GetDatasourceId();
   /** Retorma srs de la capa asociada */
   std::string GetLayerSrs();

   /** Retorna FeatureData asociada a un FeatureId */
   FeatureData* GetFeature(FeatureIdType FeatureId) const;
   /** Retorna la geometria asociada a un feature */
   Geometry* GetGeometry(FeatureIdType FeatureId) const;
   /** Actualiza la geometria asociada a un feature */
   bool UpdateGeometry(FeatureIdType FeatureId, Geometry* pGeometry);
   /** Crea un feature y le configura la geometria */
   FeatureIdType CreateFeature(Geometry* pGeometry);

   /** Retorma el feature id de las geometrias que intersecan un subset */
   std::vector<FeatureIdType> IntersectLayer(Subset &SubsetArea,
                                             const std::string &SubsetSrs);
   /** Retorma el feature id de las geometrias que intersecan una geometria */
   std::vector<FeatureIdType> IntersectLayer(Geometry* pGeometry);

   /** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
   virtual void Update(NotificationInterface* pNotification);

private:
   /** Table que se usa para actualizar la geometria */
   Table* pTable_;
   /** Fuente de datos donde pertenece la capa */
   VectorDatasource *pDatasource_;
   /** Sistema de referencia espacial de la capa */
   std::string spatialReference_;
   /** Tipo de geometrias contenidas en la capa */
   Vector::VectorType vectorType_;
   /** */
   std::string geometryColumn_;

   //  ----  Atributos que permiten obtener id de fila creada ----
   /** Id del feature que se esta creando */
   FeatureIdType createdFeatureId_;
   /** Identificador del observer en BufferedDriver */
   int observerId_;

   friend class NewRowNotification;
};

} /* namespace suri */

#endif /* VECTORDATALAYER_H_ */
