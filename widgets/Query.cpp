/*! \file Query.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>

// Includes Suri
#include "Query.h"
#include "suri/VectorElement.h"
#include "suri/VectorEditor.h"
#include "suri/VectorStyle.h"
#include "suri/Extent.h"
#include "suri/World.h"
#include "suri/Configuration.h"
#include "PixelInformationWidget.h"
#include "DefaultDatasourceList.h"
#include "wxmacros.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/RasterElement.h"
#include "suri/RasterSpatialModel.h"

// Includes Gdal
#include "ogr_geometry.h"
#include "ogr_feature.h"
#include "ogrsf_frmts.h"

// Includes Wx

// Defines
/** Define la tolerancia en pixels para el query de vectores */
#define QUERY_TOLERANCE 4.0

// forwards

namespace suri {

/**
 * Constructor
 * @param pViewportManager
 * @param pViewcontextManager
 * @param pLibraryManager
 */
Query::Query(ViewportManagerInterface *pViewportManager,
             ViewcontextManagerInterface *pViewcontextManager,
             LibraryManager *pLibraryManager) :
            ViewportEvent(pViewportManager, pViewcontextManager),
            pLibraryManager_(pLibraryManager) {
}

/** Destructor. **/
Query::~Query() {
}

/**
 *
 */
void Query::Reset() {
   // TODO(Gabriel - TCK #2672): Implementar
}

/**
 *
 * @param Cursor
 */
void Query::GetCursor(wxCursor &Cursor) {
   // TODO(Gabriel - TCK #2672): Implementar
}

/**
 *
 * @param InitialCoordinate
 * @param CurrentCoordinate
 * @param Event
 */
void Query::OnEndDrag(const Coordinates &InitialCoordinate,
                        const Coordinates &CurrentCoordinate, wxMouseEvent &Event) {
   // TODO(Gabriel - TCK #2672): Implementar
}

/**
 *
 * @param InitialCoordinate
 * @param CurrentCoordinate
 * @param Event
 */
void Query::OnDrag(const Coordinates &InitialCoordinate,
                        const Coordinates &CurrentCoordinate, wxMouseEvent &Event) {
   // TODO(Gabriel - TCK #2672): Implementar
}

/** Metodo privado que genera un polinomio en base al punto
 * que se pasa por parametro transformado en el Sr de llegada (SrWktOut).
 * @param Point punto clickeado
 * @param SrWkt sr en el que se encuentra el punto
 * @param Tolerance tolerencia de la consulta
 * @param SrWktOut sr del sistema en el que se encuentra la geometria a consultar
 * @return geometria basada en el punto clickeado transformada en el sr de la
 * geometria a consultar
 */
OGRGeometry *Query::GetQueryPoly(const Coordinates& Point, const std::string& SrWkt,
                                 double Tolerance, const std::string& SrWktOut) {
   // TODO(Javier - TCK #700): el buffer debe estar relacionado con
   // el tamano de pixel. La documentacion no explica claramente.
   OGRPoint querypoint = OGRPoint(Point.x_, Point.y_);
   querypoint.assignSpatialReference(new OGRSpatialReference(SrWkt.c_str()));
   /** Es necesario transformar el punto hacia el sr del vector debido a
    *  que el Intersects de OGR no realiza la transformacion.  */
   querypoint.transformTo(new OGRSpatialReference(SrWktOut.c_str()));
   OGRGeometry *pquerypoly = querypoint.Buffer(Tolerance);
#ifdef __DEBUG__
   OGREnvelope env;
   pquerypoly->getEnvelope(&env);
   SHOW_ERROR("Env: %.2f,%.2f;%.2f,%.2f", env.MinX, env.MaxX, env.MinY, env.MaxY);
#endif   // __DEBUG__
   return pquerypoly;
}


VectorElement *Query::CreateSelectionVector(OGRFeature *pModelFeature,
                                            const std::string &SrWkt,
                                            Vector::VectorType Type) const {
   static char suffix = '0' - 1;
   ++suffix;
   // para que exista fuera de este metodo (LEAK)
   Vector *pvec = Vector::Open(std::string("shpmemory:query_vector_") + suffix + ".shp:" + SrWkt);
   VectorEditor vecedit;
   vecedit.OpenVector(std::string("shpmemory:query_vector_") + suffix + ".shp");
   vecedit.OpenLayer(std::string("query_layer_") + suffix, SrWkt, Type);
   vecedit.CloseVector();
   OGRFeatureDefn *pfeatdef = pModelFeature->GetDefnRef();
   OGRLayer *player = pvec->GetLayer(0);
   // copio los campos
   for (int i = 0; i < pfeatdef->GetFieldCount(); ++i) {
      player->CreateField(pfeatdef->GetFieldDefn(i));
   }

   return dynamic_cast<VectorElement*>(Element::Create("VectorElement",
                    std::string("shpmemory:query_vector_") + suffix + ".shp"));
}

void Query::AddFeatureToVector(VectorElement *pVector, OGRFeature *pFeature) const {
   Vector *pvec = Vector::Open(pVector->GetUrl().c_str());
   OGRLayer *player = pvec->GetLayer(pVector->GetActiveLayer());
   if (player->CreateFeature(pFeature) != OGRERR_NONE)
      REPORT_DEBUG("D: Error al crear feature");
}


void Query::SetPoint(double Pixel, double Line, wxMouseEvent &Event) {
   ViewcontextInterface *pviewcontext =
         GetViewcontextManager()->GetSelectedViewcontext();
   LayerList *plist = pviewcontext->GetLayerList();
   LayerList::ElementListType::iterator it = plist->GetElementIterator();
   DefaultDatasourceList querieddatasources;
   ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
   World* pviewerworld = pviewer?pviewer->GetWorld():NULL;
   std::string wkt1 = pviewerworld->GetSpatialReference();
   Coordinates mclick(Pixel, Line), click, processedpl;
   while (pviewerworld && it != plist->GetElementIterator(false)) {
      double querytolerance = Configuration::GetParameter(
            "tool_query_tolerance", QUERY_TOLERANCE);
      pviewerworld->Transform(mclick, click);

      VectorElement* pvectorelement = dynamic_cast<VectorElement*>(*it);
      // Si el elemento esta activo, es de tipo vector y tiene URL continuo
      if (pvectorelement && (*it)->IsActive()) {
         Vector* pvector = Vector::Open((*it)->GetUrl().c_str());
         if (pvector) {
            int currentlayer = pvectorelement->GetActiveLayer();
            OGRLayer *player = pvector->GetLayer(currentlayer);
            VectorElement *pselectionvector = NULL;
            if (player) {
               std::string wkt2 = pvectorelement->
                                      GetSpatialReference(currentlayer).c_str();

               Subset spatialfilterwindow;
               pviewerworld->GetWindow(spatialfilterwindow);
               // Transformacion de coordenadas
               ParameterCollection params;
               params.AddValue<std::string>(TransformationFactory::kParamWktIn, wkt1);
               params.AddValue<std::string>(TransformationFactory::kParamWktOut, wkt2);
               TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
               CoordinatesTransformation *pcoordtransform =
                     pfactory->Create(TransformationFactory::kExact, params);
               TransformationFactoryBuilder::Release(pfactory);

               if (!pcoordtransform || pcoordtransform->Transform(spatialfilterwindow) < 1) {
                  REPORT_DEBUG("D: Error al convertir a sistema de coordenadas de vector");
                  if (pcoordtransform)
                     delete pcoordtransform;
                  return;
               }

               Extent spatialfilter(spatialfilterwindow);
               player->SetSpatialFilterRect(spatialfilter.min_.x_,
                                            spatialfilter.min_.y_,
                                            spatialfilter.max_.x_,
                                            spatialfilter.max_.y_);

               OGRGeometry *pquerypoly = GetQueryPoly(click, wkt1, querytolerance, wkt2);
               // Resetea el reading de la capa para que apunte al primer feature
               player->ResetReading();
               OGRFeature *pfeature = player->GetNextFeature();  // Apunta al primer feature

               while (pfeature) {
                  /**
                   * Se verifica que cada feature tenga geometria asociada,
                   * para evitar que se abran los hotlinks asociados a registros
                   * en la tabla que no tienen geometria asociada.
                   */
                  OGRGeometry *pgeometry = pfeature->GetGeometryRef();
                  if ((pgeometry && pgeometry->Intersects(pquerypoly) == TRUE)
                        || (pquerypoly && pquerypoly->Intersects(pgeometry) == TRUE)) {
                     if (!pselectionvector) {
                        // TODO(Javier - TCK #700): obtener el tipo correcto
                        pselectionvector = CreateSelectionVector(pfeature, wkt2,
                                                           pvectorelement->GetType());
                        pselectionvector->SetName(pvectorelement->GetName());
                        pselectionvector->SetIcon(pvectorelement->GetIcon());
                        wxString fieldname =
                              pvectorelement->GetHotLinkFieldName(
                                    pvectorelement->GetActiveLayer());
                        if (!fieldname.empty())
                           pselectionvector->SetHotLinkFieldName(fieldname);
                        DatasourceInterface *pdatasource =
                              DatasourceInterface::Create("VectorDatasource", pselectionvector);
                        querieddatasources.AddDatasource(pdatasource);
                     }
                     if (pselectionvector)
                        AddFeatureToVector(pselectionvector, pfeature);
                  }
                  // Analizo el siguiente feature
                  OGRFeature::DestroyFeature(pfeature);
                  pfeature = player->GetNextFeature();
               }
               // Elimina el filtro espacial
               player->SetSpatialFilter(NULL);
               OGRGeometryFactory::destroyGeometry(pquerypoly);
            }
            // Cierro el pvector
            Vector::Close(pvector);
         } else {
            // Si no pudo crear el vector informo al usuario
            SHOW_ERROR(message_OPEN_VECTOR_ERROR);
         }
      } else {
         RasterElement* prasterelement = dynamic_cast<RasterElement*>(*it);
         if (prasterelement != NULL && prasterelement->IsActive()) {

            //*** Este es el mismo procedimiento que se utiliza en PixelInfoTool */
            // \todo (danieltesta-#3210): habria que ver la forma de unificar para
            // PixelInfoTool y este procedimiento usen el mismo codigo y que el mismo
            // no este duplicado.
            std::string rasterwkt1 = prasterelement->GetSpatialReference().c_str();
            std::string rasterwkt2 = pviewerworld->GetSpatialReference();

            ParameterCollection params;
            params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                         rasterwkt1);
            params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                         rasterwkt2);
            TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
            CoordinatesTransformation *pct = pfactory->Create(
                  TransformationFactory::kExact, params);

            TransformationFactoryBuilder::Release(pfactory);

            pviewerworld->Transform(Coordinates(mclick.x_, mclick.y_), processedpl);
            pct->Transform(processedpl, true);
            RasterSpatialModel *prastermodel = RasterSpatialModel::Create(
                  prasterelement->GetRasterModel());
            prastermodel->InverseTransform(processedpl);

            delete pct;
            pct = NULL;
            /*** */

            DatasourceInterface *pdatasource = DatasourceInterface::Create(
                  "RasterDatasource", prasterelement);
            querieddatasources.AddDatasource(pdatasource);
         }
      }
      it++;
   }
   if (!querieddatasources.GetDatasourceList().empty()) {
      PixelInformationWidget *pcontainer = new PixelInformationWidget(
                           &querieddatasources, click, wkt1, mclick, processedpl, pviewerworld,
                           pLibraryManager_);
      pcontainer->CreateTool();
      pcontainer->ShowModal();
      // TODO(Javier - TCK #700): Destruir los vectorelement y datasources
      // asociados a querieddatasources
   }
}

void Query::UpdatePoint(double Pixel, double Line, wxMouseEvent &Event) {
}

} /** namespace suri */
