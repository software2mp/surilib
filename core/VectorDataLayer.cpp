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

// Includes standard
#include <vector>
#include <string>

// Includes suri
#include "VectorDataLayer.h"
#include "VectorDatasource.h"
#include "suri/Extent.h"
#include "GeometryTranslator.h"
#include "suri/VectorEditionTable.h"
#include "suri/BufferedDriver.h"
#include "Polygon.h"
#include "SimpleGeometryEditor.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/AuxiliaryFunctions.h"

// Includes gdal
#include "ogr_feature.h"
#include "ogrsf_frmts.h"
#include "ogr_geometry.h"

namespace suri {

/** Ctor */
VectorDataLayer::VectorDataLayer(VectorDatasource *pDatasource) : pTable_(NULL),
      pDatasource_(NULL), geometryColumn_(VectorEditorDriver::GeometryColumnName) {
   Vector* pvector = pDatasource->GetDatasourceAsVector();
   if (!pvector)
      return;
   spatialReference_ = pvector->GetLayerSR(pDatasource->GetActiveLayer());
   vectorType_ = pvector->GetLayerType(pDatasource->GetActiveLayer());
   Vector::Close(pvector);

   pDatasource_ = pDatasource;

   pTable_ =  new VectorEditionTable(pDatasource);

   VectorEditorDriver* pdriver = new VectorEditorDriver(pDatasource_);
   BufferedDriver* pbuffereddriver = new BufferedDriver(pdriver);
   pTable_->SetDriver(pbuffereddriver);
   pTable_->SetDataChangeNotifier(pbuffereddriver);

   observerId_ = pTable_->GetDataChangeNotifier()->Attach(this);

   createdFeatureId_ = -1;
}

/**
 * Constructor alternativo
 * @param pTable
 * @param GeometryColumn
 */
VectorDataLayer::VectorDataLayer(Table* pTable, const std::string& GeometryColumn) {
   pTable_ = pTable;
   observerId_ = pTable_->GetDataChangeNotifier()->Attach(this);
   geometryColumn_ = GeometryColumn;
   pDatasource_ = NULL;
}

/** Ctor */
VectorDataLayer::VectorDataLayer(Table* pTable, const std::string& VectorSrs,
                                            const std::string& GeometryColumn) {
   pTable_ = pTable;
   observerId_ = pTable_->GetDataChangeNotifier()->Attach(this);
   geometryColumn_ = GeometryColumn;
   spatialReference_ = VectorSrs;
   pDatasource_ = NULL;
}

/** Dtor */
VectorDataLayer::~VectorDataLayer() {
   pTable_->GetDataChangeNotifier()->Detach(observerId_);
   // Si pDatasource_ != NULL la tabla se creo localmente y hay que destruirla
   if (pDatasource_ != NULL)
      delete pTable_;
}

/** 
 * Retorma la tabla que permite editar los campos de la capa
 * @return puntero a la tabla de la capa
 * \attention el puntero es responsabilidad de esta clase. No eliminar
 */
Table* VectorDataLayer::GetTable() {
   return pTable_;
}

/** 
 * Retorma el id del datasource asociado al VectorDataLayer 
 * @return id del datasource
 */
SuriObject::UuidType VectorDataLayer::GetDatasourceId() {
   return pDatasource_ ? pDatasource_->GetId() : SuriObject::NullUuid;
}

/** 
 * Retorma srs de la capa asociada 
 * @return srs de la capa asociada
 */
std::string VectorDataLayer::GetLayerSrs() {
   return spatialReference_;
}

/**
 * Retorna FeatureData asociada a un FeatureId
 * @param[in] FeatureId id del feature
 * @return FeatureData del feature
 * @return NULL si no lo encuentra
 */
VectorDataLayer::FeatureData* VectorDataLayer::GetFeature(FeatureIdType FeatureId) const {
   int row = pTable_->GetRowById(FeatureId);
   Geometry* pgeometry = GetGeometry(FeatureId);
   return row >= 0 ? new FeatureData(FeatureId, pgeometry) : NULL;
}

/**
 * Retorna la geometria asociada a un feature 
 * @param[in] FeatureId id del feature cuya geometria quiero obtener
 * @return geometria del feature
 */
Geometry* VectorDataLayer::GetGeometry(FeatureIdType FeatureId) const {
   int row = pTable_->GetRowById(FeatureId);
   int column = pTable_->GetColumnByName(geometryColumn_);
   std::string cellvalue;
   if (!pTable_->GetCellValue(column, row, cellvalue))
      return NULL;

   /** Tranforma de geometry a wkt */
   Geometry* pgeom = GeometryTranslator::ImportFromWkt(cellvalue, spatialReference_);

   return pgeom;
}

/** 
 * Actualiza la geometria asociada a un feature 
 * @param[in] FeatureId id del feature que quiero actualizar
 * @param[in] Geometria que quiero configurar en capa
 * @return bool que indica si pudo actualizar feature
 */
bool VectorDataLayer::UpdateGeometry(FeatureIdType FeatureId,
                                           Geometry* pGeometry) {
   if (pGeometry->GetSpatialReference() != spatialReference_)
      return false;

   std::string wkt = GeometryTranslator::ExportToWkt(pGeometry);

   int row = pTable_->GetRowById(FeatureId);
   int column = pTable_->GetColumnByName(geometryColumn_);
   int fidcolumn = pTable_->GetColumnByName(VectorEditorDriver::FeatureIdColumnName);
   std::string fidvalue = NumberToString<long>(FeatureId);
   pTable_->SetCellValue(fidcolumn, row, fidvalue);
   return pTable_->SetCellValue(column, row, wkt);
}

/** 
 * Crea un feature y le configura la geometria
 * @param[in] pGeometry Geometria que quiero agregar en capa
 * @return FeatureId id del feature creado
 */
FeatureIdType VectorDataLayer::CreateFeature(Geometry* pGeometry) {
   if (!pGeometry)
      return -1;

   // Si en la tabla ya existia un feature nuevo, guardo la geometria en
   // ese feature. Caso contrario creo un fuature nuevo.
   if (createdFeatureId_ < 0)
      pTable_->AppendRow();

   // createdFeatureId_ se actualiza mediante una notificacion durante AppendRow
   if (createdFeatureId_ < 0)
      return -1;

   UpdateGeometry(createdFeatureId_, pGeometry);

   FeatureIdType newfeatureid = createdFeatureId_;
   createdFeatureId_ = -1;
   return newfeatureid;
}

/**
 * Retorna el feature id de las geometrias que intersecan un subset. Si el subset
 * esta en otro srs se usa CoordinatesTransformInterface::TransformCoordinates
 * para obtener aproximacion del mismo en srs de vector.
 * @param[in] SubsetArea subset que quiero intersecar
 * @param[in] SubsetSrs sistema de referencia en que esta el subset
 * @return vector con el id de los features que intersecan la geometria.
 */
std::vector<FeatureIdType> VectorDataLayer::IntersectLayer(Subset &SubsetArea,
                                                   const std::string &SubsetSrs) {
   // Si el subset esta en otro srs lo tranformo al del vector
   Subset searchsubset = SubsetArea;
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn, SubsetSrs);
   params.AddValue<std::string>(TransformationFactory::kParamWktOut, spatialReference_);
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact,
                                                     params);
   TransformationFactoryBuilder::Release(pfactory);

   if (SubsetSrs != spatialReference_
               && (!pct || !pct->Transform(searchsubset)))
      REPORT_DEBUG("D:Coordenadas incorrectas");

   // Crego geoemtria con subset transformado.
   Polygon polygon(spatialReference_);
   polygon.AppendRing(new Ring(spatialReference_));
   SimpleGeometryEditor* geomeditor = SimpleGeometryEditor::InitializeEditor(&polygon);
   geomeditor->AppendPoint(searchsubset.lr_);
   geomeditor->AppendPoint(Coordinates(searchsubset.lr_.x_, searchsubset.ul_.y_));
   geomeditor->AppendPoint(searchsubset.ul_);
   geomeditor->AppendPoint(Coordinates(searchsubset.ul_.x_, searchsubset.lr_.y_));

   delete geomeditor;

   return IntersectLayer(&polygon);
}

/** 
 * Retorma el feature id de las geometrias que intersecan una geometria.
 * El Intersect se hace en el sistema de referencia del vector.
 * @param[in] pGeometry geometria que quiero intersecar.
 * @return vector con el id de los features que intersecan la geometria.
 */
std::vector<FeatureIdType> VectorDataLayer::IntersectLayer(Geometry* pGeometry) {
   std::vector<FeatureIdType> returnvalue;

   OGRSpatialReference vector_srs(spatialReference_.c_str());
   OGRGeometry* pogrgeom = NULL;
   OGRGeometry* preferencegeom = NULL;

   // Genero OGRGeometry a partir de pGeometry.
   OGRSpatialReference reference_srs(pGeometry->GetSpatialReference().c_str());
   std::string pgeomasstring = GeometryTranslator::ExportToWkt(pGeometry);
   char* pgeomwkt = (char*)pgeomasstring.c_str();
   if (OGRGeometryFactory::createFromWkt(&pgeomwkt, &reference_srs, &preferencegeom) != OGRERR_NONE)
      return returnvalue;

   // Transformo la geometria al srs del vector.
   if (vector_srs.IsSame(&reference_srs) == FALSE) {
      if (preferencegeom->transformTo(&vector_srs) != OGRERR_NONE) {
         OGRGeometryFactory::destroyGeometry(preferencegeom);
         return returnvalue;
      }
   }

   // Busco geometrias que intersectan con pGeometry
   std::string cellvalue;
   int column = pTable_->GetColumnByName(geometryColumn_);
   int rowcount = pTable_->GetRows();
   for (int i = 0; i < rowcount; i++) {
      pTable_->GetCellValue(column, i, cellvalue);
      char* pwkt = (char*)cellvalue.c_str();
      if (OGRGeometryFactory::createFromWkt(&pwkt, &vector_srs, &pogrgeom) == OGRERR_NONE) {
         if (pogrgeom->Intersects(preferencegeom))
            returnvalue.push_back(pTable_->GetRowId(i));
         OGRGeometryFactory::destroyGeometry(pogrgeom);
      }
   }

   OGRGeometryFactory::destroyGeometry(preferencegeom);
   return returnvalue;
}

/** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
void VectorDataLayer::Update(NotificationInterface* pNotification) {
   pNotification->ApplyNotification(this);
}

} /* namespace suri */
