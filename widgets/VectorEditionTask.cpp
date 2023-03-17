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

#include <algorithm>
#include <utility>
#include <string>
#include <vector>
#include <set>

// Includes suri
#include "suri/VectorEditionTask.h"
#include "GuiGeometryEditor.h"
#include "GuiGeometryCreator.h"
#include "VectorDataLayerSelectionSource.h"
#include "VectorDatasourceSelectionSource.h"
#include "GeometryColumnEditionState.h"
#include "VectorDataLayer.h"
#include "suri/ViewportManagerInterface.h"
#include "suri/SnapTool.h"
#include "suri/Configuration.h"
namespace suri {

/** Nombre del campo que tiene la geometria */
const std::string VectorEditionTask::GEOMETRY_COLUMN_NAME = "the_geom";
/** Nombre asignado a la tarea */
const std::string VectorEditionTask::TASKNAME = "VectorEditionTask";

/** Ctor */
VectorEditionTask::VectorEditionTask() : vectorType_(Vector::Undefined),
                                           pWorld_(NULL),
                                           pGeometryCreator_(NULL),
                                           pGeometryEditor_(NULL),
                                           pModifiedGeometries_(NULL),
                                           pDeletedGeometries_(NULL),
                                           pViewportManager_(NULL),
                                           pSnapTool_(NULL) {
   pGeometryCreator_ = new GuiGeometryCreator();
   pGeometryEditor_ = new GuiGeometryEditor();
}

/** Dtor */
VectorEditionTask::~VectorEditionTask() {
   delete pGeometryCreator_;
   delete pGeometryEditor_;
   Cancel();
}


/**
 * Configura el mundo donde se desea editar el vector
 * @param[in] pWorld mundo que se usa para editar los puntos.
 */
bool VectorEditionTask::SetWorld(World* pWorld) {
   if (IsActive())
      return false;
   pWorld_ = pWorld;
   return true;
}

/**
 * Retorna el mundo donde se realiza la edicion
 * @return mundo que se usa para iniciar la edicion.
 */
World* VectorEditionTask::GetWorld() const {
   return pWorld_;
}

/** 
 * Configura el srs y el tipo de vector editado.
 * @param[in] pVectorDatasource fuente de datos asociada a la tabla
 * que se edita.
 */
void VectorEditionTask::ConfigureVectorData(VectorDatasource* pVectorDatasource) {
   Vector* pvector = pVectorDatasource->GetDatasourceAsVector();
   vectorSrs_ = pvector->GetLayerSR(pVectorDatasource->GetActiveLayer());
   vectorType_ = pvector->GetLayerType(pVectorDatasource->GetActiveLayer());
   Vector::Close(pvector);
   datasourceId_ = pVectorDatasource->GetId();
}

/** Comienza la tarea de edicion vectorial */
bool VectorEditionTask::Start() {
   if (!TableEditionTask::Start())
      return false;
   ClearModifiedFeatures();
   return true;
}

/**
 * Finaliza la tarea. Intenta guardar los cambios.
 */
bool VectorEditionTask::End() {
   if (!TableEditionTask::End()) return false;

   delete pDeletedGeometries_;
   pDeletedGeometries_ = NULL;

   return true;
}

/** 
 * Inicia la edicion de un feature existente 
 * @param[in] FeatureId id del feature a editar
 * @return bool que indica si se pudo iniciar la edicion.
 */
bool VectorEditionTask::StartFeatureEdition(FeatureIdType FeatureId) {
   if (pTable_->GetTableReadOnlyState()) {
      SHOW_ERROR(message_ERROR_EDITING_READ_ONLY_TABLE);
      return false;
   }
   if (!TableEditionTask::StartFeatureEdition(FeatureId))
      return false;

   if (!EditedFeatureHasGeometry()) {
      GuiGeometryCreator::GeometryType creatortype = GuiGeometryCreator::Polygon;
      switch (vectorType_) {
      case Vector::Polygon:
         creatortype = GuiGeometryCreator::Polygon;
         break;
      case Vector::Line:
         creatortype =  GuiGeometryCreator::Line;
         break;
      case Vector::Point:
         creatortype =  GuiGeometryCreator::Point;
         break;
      default:
         return false;
      }

      pGeometryCreator_->Start(creatortype, vectorSrs_, pWorld_);
   } else {
      pGeometryEditor_->Start(GetEditedGeometry(GEOMETRY_COLUMN_NAME), pWorld_);
   }
   return true;
}

/** 
 * Finaliza la edicion de la geometria editada.
 * @param[in] SaveChanges flag que indica si se deben guardar los cambios
 * @return bool que indica si se pudo finalizar la edicion
 * correctamente.
 */
bool VectorEditionTask::EndFeatureEdition(bool SaveChanges) {
   if (pTable_->GetTableReadOnlyState()) {
      SHOW_ERROR(message_ERROR_EDITING_READ_ONLY_TABLE);
      return false;
   }
   if (!IsEditingFeature())
      return true;

   if (SaveChanges && pGeometryCreator_->IsEditing()) {
      // Si se creo una geometria en la gui la configuro antes de cerrar la edicion
      Geometry* pnewgeom = pGeometryCreator_->GetCreatedGeometry();
      if (pnewgeom && pnewgeom->IsValid()) {
         SetEditedGeometry(GEOMETRY_COLUMN_NAME, pnewgeom);
         AddModifiedFeature(GetEditedFeatureId());
      }
   }

   if (pGeometryEditor_->IsEditing() && pGeometryEditor_->HasChanged())
      AddModifiedFeature(GetEditedFeatureId());

   pGeometryEditor_->End();
   pGeometryCreator_->End();

   bool returnvalue = TableEditionTask::EndFeatureEdition(SaveChanges);

   if (pSnapTool_ && returnvalue)
      pSnapTool_->AddGeometry();

   return returnvalue;
}

/** 
 * Retorna la geometria que se esta editando 
 * @param[in] GeometryColumnName nombre de la columna asociada a la geometria
 * @return geometria en edicion.
 */
Geometry* VectorEditionTask::GetEditedGeometry(std::string GeometryColumnName) {
   if (GeometryColumnName == GEOMETRY_COLUMN_NAME && pGeometryCreator_->IsEditing())
      return pGeometryCreator_->GetCreatedGeometry();

   return TableEditionTask::GetEditedGeometry(GeometryColumnName);
}

/**
 * Retorna puntero a GuiGeometryEditorInterface
 * @return GuiGeometryCreatorInterface si el feature tiene geometria
 * @return NULL si el feature no tiene geometria
 */
GuiGeometryEditorInterface* VectorEditionTask::GetFeatureEditor() {
   return (pGeometryEditor_->IsEditing()) ? pGeometryEditor_ : NULL;
}

/**
 * Retorna puntero a GuiGeometryCreatorInterface
 * @return GuiGeometryCreatorInterface si el feature no tiene geometria
 * @return NULL si el feature tiene geometria
 */
GuiGeometryCreatorInterface* VectorEditionTask::GetFeatureCreator() {
   return (pGeometryCreator_->IsEditing()) ? pGeometryCreator_ : NULL;
}

/** Indica si el feature en edicion tiene una geometria. */
bool VectorEditionTask::EditedFeatureHasGeometry() {
   return GetEditedGeometry(GEOMETRY_COLUMN_NAME) != NULL;
}


/**
 * Elimina un feature de la capa en edicion
 * @param[in] FeatureId id del feature a eliminar
 * @return bool que indica si se pudo eliminar el feature.
 */
bool VectorEditionTask::DeleteFeature(FeatureIdType FeatureId) {
   if (pTable_->GetTableReadOnlyState()) {
      SHOW_ERROR(message_ERROR_EDITING_READ_ONLY_TABLE);
      return false;
   }

   if (!IsActive())
      return false;

   int rownumber = pTable_->GetRowById(FeatureId);
   if (rownumber < 0)
      return false;

   if (!pTable_->DeleteRow(rownumber))
      return false;

   // Saco FeatureId de modifiedFeatures_
   RemoveModifiedFeature(FeatureId);

   return true;
}

/** Retorna GeometryCollection con las geometrias modificadas por el usuario. */
GeometryCollection* VectorEditionTask::GetModifiedGeometries() {
   if (IsActive() && !pModifiedGeometries_) {
      VectorDataLayer datalayer(pTable_, vectorSrs_, GEOMETRY_COLUMN_NAME);
      pModifiedGeometries_ = new GeometryCollection(vectorSrs_);
      VectorStyle* pgeomstyle = VectorStyle::Create
            ("VECTORSTYLE[3,\"Poligono amarillo\","
                  "BRUSH[\"Suri-brush\",0,COLOR[0,0,0,255]],PEN[\"Suri-pen\","
                  "1,COLOR[255,255,0,255],WIDTH[1]],"
                  "SYMBOL[\"Suri-symbol\",4,COLOR[255,255,0,255],SIZE[2]]]");
      pModifiedGeometries_->SetStyle(pgeomstyle);
      std::set<FeatureIdType>::iterator it = modifiedFeatures_.begin();
      for (; it != modifiedFeatures_.end(); ++it)
         pModifiedGeometries_->AppendGeometry(datalayer.GetGeometry(*it));
   }
   return pModifiedGeometries_;
}

/** Retorna GeometryCollection con las geometrias modificadas por el usuario. */
GeometryCollection* VectorEditionTask::GetDeletedGeometries() {
   return pDeletedGeometries_;
}

/** Retorna el ViewportManager */
ViewportManagerInterface* VectorEditionTask::GetViewportManager() {
   return pViewportManager_;
}

/** Setea el ViewportManager */
void VectorEditionTask::SetViewportManager(ViewportManagerInterface* pViewportManager) {
   pViewportManager_ = pViewportManager;
}

/** Actualiza los visualizadores */
void VectorEditionTask::UpdateViewers() {
   if (pViewportManager_) {
      std::vector<SuriObject::UuidType> Ids = pViewportManager_->GetViewportIds();
      std::vector<SuriObject::UuidType>::iterator it = Ids.begin();
      for ( ; it != Ids.end(); ++it) {
         if (pViewportManager_->IsViewportActive(*it)) {
            ViewerWidget* ptempviewer = AS_VW(pViewportManager_->GetViewport(*it));
            ptempviewer->GetWindow()->Refresh();
         }
      }
   }
}

/**
 * Configura las columnas de tipo geometria que estan en la tabla.
 * @return bool que indica si tuvo exito
 */
bool VectorEditionTask::ConfigureGeometryColumnEditors() {
   pGeometryColumnEditorMap_.insert(
         std::pair<std::string, GeometryColumnEditionState*>(GEOMETRY_COLUMN_NAME,
                new GeometryColumnEditionState(pTable_, vectorSrs_, GEOMETRY_COLUMN_NAME)));
   return true;
}

/** Funcion que indica si se puede iniciar la edicion */
bool VectorEditionTask::CanStart() {
   return TableEditionTask::CanStart() && pWorld_ != NULL
                                       && vectorType_ != Vector::Undefined;
}

/** Crea el SelectionSource que hay que configurar al iniciar la tarea */
FeatureSelectionSource* VectorEditionTask::CreateFeatureSelectionSource() {
   VectorDataLayer* pdatalayer = new VectorDataLayer(
                           pTable_, vectorSrs_, GEOMETRY_COLUMN_NAME);
   return new VectorDataLayerSelectionSource(pdatalayer, datasourceId_);
}

/**
 * Agrega feature id a listado con las geometrias editadas/modificadas.
 * @param[in] FeatureId id de feature modificado
 */
void VectorEditionTask::AddModifiedFeature(FeatureIdType FeatureId) {
   modifiedFeatures_.insert(FeatureId);
   delete pModifiedGeometries_;
   pModifiedGeometries_ = NULL;
}

void VectorEditionTask::AddDeletedFeature(FeatureIdType FeatureId) {
   if (pDeletedGeometries_ == NULL) {
      pDeletedGeometries_ = new GeometryCollection(vectorSrs_);
      std::string style = Configuration::GetParameter("geo_deleting_style",
                                                      DEFAULT_GEO_DELETING_STYLE);
      VectorStyle* pgeomstyle = VectorStyle::Create(style);
      pDeletedGeometries_->SetStyle(pgeomstyle);
   }

   VectorDataLayer datalayer(pTable_, vectorSrs_, GEOMETRY_COLUMN_NAME);
   pDeletedGeometries_->AppendGeometry(datalayer.GetGeometry(FeatureId));
}

/**
 * Busca el feature especificado en la lista de features modificados
 * @param[in] FeatureIdType tipo de feature
 * @return true si encontro el feature
 */
bool VectorEditionTask::IsModifiedFeature(FeatureIdType FeatureId) {
   std::set<FeatureIdType>::iterator retit;
   retit = modifiedFeatures_.find(FeatureId);
   if (retit == modifiedFeatures_.end())
      return false;
   return true;
}

/** 
 * Elimina feature id de listado con las geometrias editadas/modificadas. 
 * @param[in] FeatureId id de feature a eliminar
 */
void VectorEditionTask::RemoveModifiedFeature(FeatureIdType FeatureId) {
   modifiedFeatures_.erase(FeatureId);
   delete pModifiedGeometries_;
   pModifiedGeometries_ = NULL;
}

/** Limpia listado con las geometrias editadas/modificadas. */
void VectorEditionTask::ClearModifiedFeatures() {
   modifiedFeatures_.clear();
   delete pModifiedGeometries_;
   pModifiedGeometries_ = NULL;
}

/** Limpia listado con las geometrias eliminadas */
void VectorEditionTask::ClearDeletedFeatures() {
   delete pDeletedGeometries_;
   pDeletedGeometries_ = NULL;
}

/** Setea un puntero a la herramienta de snapping existente */
void VectorEditionTask::SetSnapTool(SnapTool* Tool) {
   pSnapTool_ = Tool;
}

/** Setea un puntero a la herramienta de snapping existente */
SnapTool* VectorEditionTask::GetSnapTool() {
   return pSnapTool_;
}

} /* namespace suri */
