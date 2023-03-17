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

// Includes Estandar
#include <string>
#include <list>
#include <vector>
#include <memory>
#include <utility>
#include <map>

// Includes Suri
#include "resources.h"
#include "suri/ToolSupport.h"
#include "EnclosureValidator.h"
#include "EnclosureManipulator.h"
#include "DefaultViewcontextManager.h"
#include "ClassifiedRasterDatasourceValidator.h"
#include "ClassifiedRasterDatasourceManipulator.h"
#include "suri/Option.h"
#include "suri/messages.h"
#include "suri/DataTypes.h"
#include "suri/LayerTool.h"
#include "suri/LayerList.h"
#include "suri/ToolCommand.h"
#include "suri/Configuration.h"
#include "suri/RasterElement.h"
#include "suri/RasterProcess.h"
#include "suri/TerrainElement.h"
#include "suri/ElementManager.h"
#include "suri/LayerInterface.h"
#include "suri/LayerTreeWidget.h"
#include "suri/DatasourceInterface.h"
#include "suri/FileExporterProcess.h"
#include "suri/PartContainerWidget.h"
#include "suri/ProcessNotebookWidget.h"
#include "suri/ElementPropertiesWidget.h"
#include "suri/WxsLayerConfigurationPart.h"
#include "suri/XmlTranslatorBuilder.h"
#include "LayerElementXmlTranslator.h"
#include "DatasourceElementXmlTranslator.h"
#include "suri/Element.h"
#include "LibraryItemEditorDebugPart.h"
#include "suri/TextFileGcpLoader.h"
#include "suri/GcpList.h"
#include "TablePartConfigurator.h"
#include "suri/ProcessInterface.h"
#include "suri/FileVectorExporterProcess.h"
#include "FilteredDatasource.h"
#include "Filter.h"
#include "suri/DataViewManager.h"
#include "TableImportXyPart.h"
#include "WxsLayerImporter.h"
#include "RasterGenericImporterWidget.h"
#include "suri/VectorOperationProcess.h"
#include "SpatialReference.h"

// Includes Wx
#include "wx/arrstr.h"
#include "wx/filedlg.h"
#include "wx/xml/xml.h"
#include "wx/sstream.h"
// Defines
#define __CTX_MNG_GROUP__
#define FEATURE_ID_FIELD_NAME "FID"

// forwards

namespace suri {

/**
 * Constructor
 * @param[in]  pDataViewManager: Puntero al DataViewManager
 */
LayerTool::LayerTool(DataViewManager* pDataViewManager) :
      GenericTool(pDataViewManager), pDataViewManager_(pDataViewManager) {
   configurationVariableName_ = "layer";
   Initialize();
}

/**
 * Destructor
 */
LayerTool::~LayerTool() {
   Finalize();
}

/**
 * Accion de "Ejecutar" de AddLayer
 */
void LayerTool::ExecuteAddLayer() const {
   std::string filter = filter_VIEWER_2D_ELEMENTS;
   AddLayerElement(filter);
}

/**
 * Accion de "Ejecutar" de AddRemoteLayer
 */
void LayerTool::ExecuteAddRemoteLayer() const {
   std::string factorystring;
   Option metadata;
   WxsLayerImporter* pimportwidget = new WxsLayerImporter(pDataViewManager_,
         factorystring, metadata);
   if (pimportwidget->CreateTool() && pimportwidget->ShowModal() == wxID_OK) {

   }
   // se comenta hasta que este terminada la implementacion
   suri::DatasourceManagerInterface* dm = GetDatasourceManager();

   const char* pdatasourcetypes[] = { "RasterDatasource", "VectorDatasource" };
   DatasourceInterface* pdatasource = NULL;
   for (int i = 0; i < 2 && pdatasource == NULL; ++i) {
      pdatasource = suri::DatasourceInterface::Create(pdatasourcetypes[i],
            factorystring);
   }
   if (pdatasource != NULL && !dm->AddDatasource(pdatasource)) {
      SHOW_ERROR(message_ADD_ELEMENT_ERROR);
   }
}

/**
 * Accion de "Ejecutar" de AddCsvLayer
 */
void LayerTool::ExecuteAddCsvLayer() const {
   suri::DatasourceManagerInterface* dm = GetDatasourceManager();
   TableImportXyPart* ptableimportxy = new TableImportXyPart(pDataViewManager_->GetLibraryManager(), dm);
   ptableimportxy->CreateTool();
   ptableimportxy->ShowModal();
}

/**
 * Accion de "Ejecutar" de RemoveLayer
 */
void LayerTool::ExecuteRemoveLayer() const {
   // Elmino capas seleccionadas
   std::list<NodePath> selectednodes = GetSelectionManager()->GetSelectedNodes();
   std::list<NodePath>::iterator it = selectednodes.begin();
   bool removeresult = true;
   for (; it != selectednodes.end() && removeresult; ++it)
      if (!it->AllowsChildren())
         removeresult = GetDatasourceManager()->RemoveObject(
               it->GetLastPathNode()->GetContent());

   if (!removeresult) {
      SHOW_ERROR(_(message_LAYER_BLOCK));
      return;
   }

   // Elimino grupos seleccionados
   selectednodes = GetSelectionManager()->GetSelectedNodes();
   removeresult = true;
   it = selectednodes.begin();
   for (; it != selectednodes.end() && removeresult; ++it)
      if (it->AllowsChildren())
         removeresult = GetGroupManager()->RemoveGroup(*it);
   if (!removeresult) {
      SHOW_ERROR("No se puede elminar el grupo");
      return;
   }
}

/**
 * Accion de "Ejecutar" de TerrainLayer
 */
void LayerTool::ExecuteAddTerrainLayer() const {
   std::string filter = filter_VIEWER_3D_ELEMENTS;
   AddTerrainLayerElement(filter);
}


/** Metodo que invoca al importador generico raster **/
void LayerTool::ExecuteImportGenericRaster() const {
   RasterGenericImporterWidget* prwidget = new RasterGenericImporterWidget(
         GetDatasourceManager());
   if (prwidget->CreateTool() && prwidget->GetFrame()->EnableCloseButton()
         && prwidget->ShowModal() == wxID_OK) {
   }
}

/**
 * Metodo auxiliar para:
 * ExecuteAddTerrainLayer
 */
void LayerTool::AddTerrainLayerElement(std::string& filter) const {
   suri::DatasourceManagerInterface* dm = GetDatasourceManager();
   wxFileDialog filedialog(NULL, _(caption_SELECT_ELEMENT), wxT(""), wxT(""), filter);
   if (filedialog.ShowModal() == wxID_OK) {
      std::string filename = filedialog.GetPath().c_str();
      wxFileDialog dlg(NULL, _(caption_SELECT_TEXTURE), wxT(""), wxT(""), filter);
      if (dlg.ShowModal() == wxID_OK) {
         // valida y agrega el elemento a la lista
         filename += std::string(TEXTURE_SEPARATION_TOKEN) + dlg.GetPath().c_str();
      } else {
         SHOW_ERROR(message_ADD_TEXTURE_ERROR);
      }

      const char* pdatasourcetypes[] = { "RasterDatasource" };
      DatasourceInterface *pdatasource = NULL;
      for (size_t i = 0; i < ARRAY_SIZE(pdatasourcetypes) && pdatasource == NULL; ++i)
         pdatasource = suri::DatasourceInterface::Create(pdatasourcetypes[i], filename);
      if (!dm->AddDatasource(pdatasource))
         SHOW_ERROR(message_ADD_ELEMENT_ERROR);
   }
}

/**
 * Metodo auxiliar que devuelve el tipo de grupo que corresponde
 * la fuente de datos que se pasa por parametro
 * @param[in] pDatasource fuente de datos sobre la cual se quiere obtener su grupo
 * @return LayerGroup grupo correspondiente a la definicion de grupos definida en el enum
 * LayerGroup
 */
LayerTool::LayerGroup GetLayerGroup(DatasourceInterface* pDatasource) {
   LayerTool::LayerGroup group = LayerTool::NoGroup;
   if (pDatasource->GetClassId().compare("RasterDatasource") == 0) {
      group = LayerTool::RasterGroup;
   } else {
      Vector::VectorType vtype =
            dynamic_cast<VectorElement*>(pDatasource->GetElement())->GetType();
      switch (vtype) {
         case Vector::Point:
            group = LayerTool::PointGroup;
            break;
         case Vector::Line:
            group = LayerTool::LineGroup;
            break;
         case Vector::Polygon:
            group = LayerTool::PolygonGroup;
            break;
         default:
            break;
      }
   }
   return group;
}

/**
 * Metodo auxiliar para:
 * ExecuteAddLayer
 * ExecuteAddTerrainLayer
 */
void LayerTool::AddLayerElement(std::string& filter) const {
   suri::DatasourceManagerInterface* dm = GetDatasourceManager();
   wxFileDialog filedialog(NULL, _(caption_SELECT_ELEMENT), wxT(""), wxT(""), filter,
                           wxFD_MULTIPLE);
   if (filedialog.ShowModal() == wxID_OK) {
      wxArrayString pathsarray;
      filedialog.GetPaths(pathsarray);
      if (!pathsarray.empty()) {
         bool ok = true;
         /** Se agrega un mapa para que se realice una insercion ordenada de los
          * elementos teniendo en cuenta su naturaleza (punto, linea, poligino, raster)
          * El orden se encuentra definido en el defecto #5543.
          * http://trac.suremptec.com.ar/trac/ticket/5543
          */
         std::multimap<LayerGroup, DatasourceInterface*> layergroup = mapLayerGroup(pathsarray);
         std::map<LayerGroup, DatasourceInterface*>::iterator it = layergroup.begin();
         for (; it != layergroup.end(); ++it) {
            ok = ok && dm->AddDatasource(it->second);
         }
         if (!ok) {
            SHOW_ERROR(message_ADD_ELEMENT_ERROR);
         }
      }
   }
}

LayerTool::LayerGroupMap LayerTool::mapLayerGroup(const wxArrayString& pathsarray) const{
   std::multimap<LayerGroup, DatasourceInterface*> layergroup;
   for (size_t i = 0; i < pathsarray.GetCount(); i++) {
      std::string filename = std::string((pathsarray.Item(i)).mb_str(wxConvUTF8));
      const char* pdatasourcetypes[] = { "RasterDatasource", "VectorDatasource" };
      DatasourceInterface* pdatasource = NULL;
      for (size_t i = 0; i < ARRAY_SIZE(pdatasourcetypes) && pdatasource == NULL;
            ++i)
         pdatasource = suri::DatasourceInterface::Create(pdatasourcetypes[i],
                                                         filename);
      if (pdatasource != NULL)
         layergroup.insert(std::make_pair(GetLayerGroup(pdatasource), pdatasource));
   }
   return layergroup;
}


/**
 * Accion de "Ejecutar" de DisplayLayers
 */
void LayerTool::ExecuteDisplayLayers() const {
   ViewcontextInterface* pselectedviewcontext =
         GetViewcontextManager()->GetSelectedViewcontext();
   ActivateSelectedLayersInViewcontext(pselectedviewcontext, true);
}

/**
 * Accion de "Ejecutar" de HideLayers
 */
void LayerTool::ExecuteHideLayers() const {
   ViewcontextInterface* pselectedviewcontext =
         GetViewcontextManager()->GetSelectedViewcontext();
   ActivateSelectedLayersInViewcontext(pselectedviewcontext, false);
}

/**
 * Accion de "Ejecutar" de DisplayLayersInAllViewcontexts
 */
void LayerTool::ExecuteDisplayLayersInAllViewcontexts() const {
   ActivateLayersInAllViewcontexts(true);
}

/**
 * Accion de "Ejecutar" de HideLayersInAllViewcontexts
 */
void LayerTool::ExecuteHideLayersInAllViewcontexts() const {
   ActivateLayersInAllViewcontexts(false);
}

/**
 * Activa en todos los viewcontext las capas seleccionadas en el viewcontext seleccionado
 */
void LayerTool::ActivateLayersInAllViewcontexts(bool Activate) const {
   GetViewcontextManager()->ActivateSelectedLayersInAllViewcontexts(Activate);
}

/**
 * Metodo auxiliar para:
 * ExecuteDisplayLayersInAllViewcontexts
 * ExecuteHideLayersInAllViewcontexts
 */
void LayerTool::ActivateSelectedLayersInSelectedViewcontext(bool Activate) const {
   // Ahora hay que mostrar los elementos en todos los viewcontext
   GetViewcontextManager()->ActivateSelectedLayersInViewcontext(
         GetViewcontextManager()->GetSelectedViewcontext(), Activate);
}

/**
 * Metodo auxiliar para:
 * ExecuteDisplayLayers
 * ExecuteHideLayers
 * ActivateLayersInAllViewcontexts
 */
void LayerTool::ActivateSelectedLayersInViewcontext(ViewcontextInterface* pViewcontext,
                                                    bool Activate) const {
   GetViewcontextManager()->ActivateSelectedLayersInViewcontext(pViewcontext, Activate);
}

/**
 * Accion de "Ejecutar" de ShowProperties
 */
void LayerTool::ExecuteShowLayerProperties() const {
   ViewcontextInterface* pviewcontext =
         GetViewcontextManager()->GetSelectedViewcontext();
   std::list<SuriObject::UuidType> selectedelementlist = pviewcontext->GetSelectedLayers();
   if (selectedelementlist.empty())
      return;

   LayerInterface* player = pviewcontext->GetLayer(*selectedelementlist.begin());
   SuriObject::UuidType datasourceId = player->GetAssociatedDatasourceId();
   DatasourceInterface* pdatasource = GetDatasourceManager()->GetDatasource(
         datasourceId);

   // ppropertieswidget se elimina al cerrar el showmodal
   ElementPropertiesWidget* ppropertieswidget = new ElementPropertiesWidget(
         GetDataViewManager(), pdatasource, player);
   ppropertieswidget->ShowModal();
}

/**
 * Accion de "Ejecutar" de CreateGroup
 */
void LayerTool::ExecuteCreateGroup() const {
#ifdef __CTX_MNG_GROUP__
   // creo un dialogo para ingresar un txt
   std::string newgroupname;

   if (!GetGroupName(newgroupname))
      return;

   // Agrego grupo al final del arbol
   WorkGroupInterface* prootworkgroup = GetGroupManager()->GetRootWorkGroup();
   NodePath rootpath = prootworkgroup->GetRootPath();
   GetGroupManager()->InsertGroup(newgroupname.c_str(), rootpath,
                                  prootworkgroup->GetRootSize());
   delete prootworkgroup;
#endif
}

/**
 * Ejecuta el comando para exportar una capa
 */
void LayerTool::ExecuteExportLayer() const {
   RasterElement* praster = ToolSupport::GetRaster(GetViewcontextManager());
   Element* pelement = NULL;
   Subset viewerwindow;
   ProcessAdaptLayer* ppal =NULL;
   bool showmask = true;
   if (praster != NULL) {
      ToolSupport::GetSubset(GetViewportManager(), viewerwindow, praster);
      FileExporterProcess process(praster, viewerwindow, GetDataViewManager());
      ppal->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr,
                                                        showmask);
      process.OnProcessAdaptLayerSet();
      suri::ProcessNotebookWidget* pprocesswidget = new suri::ProcessNotebookWidget(
            &process, caption_LAYER_EXPORT, GetLibraryManager());
      if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
         DatasourceInterface* pdatasource = NULL;
         const char *pdatasourcetypes[] = { "RasterDatasource", "VectorDatasource" };
         for (int i = 0; i < 2 && pdatasource == NULL; ++i)
            pdatasource = DatasourceInterface::Create(pdatasourcetypes[i], pelement);
         if (pdatasource)
            GetDatasourceManager()->AddDatasource(pdatasource);
      }
   } else {
      SHOW_WARNING(message_PROCESS_START_ERROR);
   }
}

/** Ejecuta el comando para exportar una capa vectorial **/
void LayerTool::ExecuteExportVectorLayer() const {
    Element* pelement = NULL;
    Subset viewerwindow;
    bool showmask = false;
    bool showspatial = false;
   VectorDatasource* pvector = ToolSupport::GetVectorDatasource(
         pDataViewManager_->GetDatasourceManager(),
         pDataViewManager_->GetViewcontextManager());
   World* pworld = ToolSupport::GetWorld(GetViewportManager());
   if (pvector) {
      if (!pworld) {
         pworld = new World();
         pworld->SetSpatialReference(pvector->GetSpatialReference());
         Subset extent;
         pvector->GetElement()->GetElementExtent(extent);
         pworld->SetWorld(extent);
         pworld->SetWindow(extent);
      }
      pworld->GetWindow(viewerwindow);
      std::vector<DatasourceInterface*> datasources;
      datasources.push_back(pvector);
      FileVectorExporterProcess process(datasources, viewerwindow, pelement,
                                               pDataViewManager_);
      ProcessAdaptLayer* ppal = process.GetProcessAdaptLayer();
      ppal->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr,
                                                   showmask);
      ppal->AddAttribute<bool>(ProcessAdaptLayer::IncludeSpatialSelKeyAttr,
                                   showspatial);
      process.OnProcessAdaptLayerSet();
      suri::ProcessNotebookWidget* pprocesswidget = new suri::ProcessNotebookWidget(
            &process, caption_VECTOR_LAYER_EXPORT, GetLibraryManager());
      if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
         DatasourceInterface* pdatasource = NULL;
         const char *pdatasourcetypes[] = { "RasterDatasource", "VectorDatasource" };
         for (int i = 0; i < 2 && pdatasource == NULL; ++i)
            pdatasource = DatasourceInterface::Create(pdatasourcetypes[i], pelement);
         if (pdatasource)
            GetDatasourceManager()->AddDatasource(pdatasource);
      }
   } else {
      SHOW_WARNING(message_PROCESS_EXPORT_VECTOR_LAYER_START_ERROR);
   }
}

/** Ejecuta el comando para exportar la seleccion de una capa */
void LayerTool::ExecuteExportLayerSelection() const {
   VectorDatasource* pvector = ToolSupport::GetVectorDatasource(
         pDataViewManager_->GetDatasourceManager(),
         pDataViewManager_->GetViewcontextManager());
   World* pworld = ToolSupport::GetWorld(GetViewportManager());
   Element* pelement = NULL;
   ProcessAdaptLayer* ppal=NULL;
   if (pvector) {
      bool showspatial = false;
      Subset viewerwindow;
      if (!pworld) {
         pworld = new World();
         pworld->SetSpatialReference(pvector->GetSpatialReference());
         Subset extent;
         pvector->GetElement()->GetElementExtent(extent);
         pworld->SetWorld(extent);
         pworld->SetWindow(extent);
      }
      pworld->GetWindow(viewerwindow);
      std::vector<DatasourceInterface*> datasources;
      FilteredDatasource* pvectorselection = new FilteredDatasource();
      pvectorselection->SetDatasource(pvector);
      Filter* pfilter = new Filter();
      pfilter->SetFilteredName(pvector->GetActiveLayerName());
      pfilter->SetIdFieldName(FEATURE_ID_FIELD_NAME);
      std::vector<FeatureIdType> fids =
            pDataViewManager_->GetFeatureSelection()->GetSelectedFeatures(
                  pvector->GetId());
      std::vector<FeatureIdType>::iterator it = fids.begin();
      for (; it != fids.end(); ++it) {
         pfilter->AddEqualConstraint(*it);
      }
      pvectorselection->SetFilter(pfilter);
      datasources.push_back(pvectorselection);
      FileVectorExporterProcess process(datasources, viewerwindow, pelement,
                                               pDataViewManager_);
      ppal->AddAttribute<bool>(ProcessAdaptLayer::IncludeSpatialSelKeyAttr,
                                    showspatial);
      process.OnProcessAdaptLayerSet();
      suri::ProcessNotebookWidget* pprocesswidget = new suri::ProcessNotebookWidget(
            &process, caption_VECTOR_LAYER_SELECTION_EXPORT, GetLibraryManager());
      if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
         DatasourceInterface* pdatasource = NULL;
         const char *pdatasourcetypes[] = { "VectorDatasource" };
         for (int i = 0; i < 1 && pdatasource == NULL; ++i)
            pdatasource = DatasourceInterface::Create(pdatasourcetypes[i], pelement);
         if (pdatasource)
            GetDatasourceManager()->AddDatasource(pdatasource);
      }
   } else {
      SHOW_WARNING(message_SELECTION_EXPORT_PROCESS_START_ERROR);
   }
}

/**
 * Ejecuta el comando para apilar bandas desde distintas fuentes
 * de datos.
 */
void LayerTool::ExecuteStackBands() const {
   std::vector<RasterElement*> rasters = ToolSupport::GetAllRaster(
         GetViewcontextManager());
   if (rasters.size() == 0){
     SHOW_WARNING(message_PROCESS_START_ERROR);
      return;
   }
   Subset viewerwindow;
   ToolSupport::GetSubset(GetViewportManager(), viewerwindow, rasters[0]);
   std::vector<Element*> elements;
   for (int ix = 0, lenix = rasters.size(); ix < lenix; ++ix) {
      elements.push_back(dynamic_cast<Element*>(rasters[ix]));
   }
   FileExporterProcess process(elements, viewerwindow, GetDataViewManager());
   process.OnProcessAdaptLayerSet();
   suri::ProcessNotebookWidget* pprocesswidget = new suri::ProcessNotebookWidget(
         &process, caption_STACK_BANDS, GetLibraryManager());
   if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
      Element* pNewElement = NULL;
      process.GetProcessAdaptLayer()->GetAttribute<Element*>(
                  ProcessAdaptLayer::OutputElementKeyAttr, pNewElement);

      DatasourceInterface* pdatasource = DatasourceInterface::Create("RasterDatasource",
                                                                     pNewElement);
      GetDatasourceManager()->AddDatasource(pdatasource);
   }
}

/**
 * Accion de "Ejecutar" de Visualization
 * \TODO(Sebastian - TCK #846): Implementar
 */
void LayerTool::ExecuteVisualization() const {
   return;
}

/**
 * Muestra pantalla para que el usuario ingrese un nombre de grupo.
 * @param[out] GroupName nombre del grupo ingresado por el usuario
 * @return bool que que indica si tuvo exito.
 */
bool LayerTool::GetGroupName(std::string &GroupName) const {
   wxDialog dialog(NULL, wxID_ANY, _(caption_NEW_GROUP_INPUT), wxDefaultPosition,
                   wxDefaultSize, wxCAPTION | wxSYSTEM_MENU);
   dialog.SetSizer(new wxBoxSizer(wxVERTICAL));
   wxTextCtrl *ptxtctrl = new wxTextCtrl(&dialog, wxID_ANY,
                                         _(content_DEFAULT_GROUP_NAME),
                                         wxDefaultPosition, wxSize(250, -1));
   dialog.GetSizer()->Add(ptxtctrl, 0, wxEXPAND | wxALL, 5);
   dialog.GetSizer()->Add(dialog.CreateButtonSizer(wxOK), 0, wxEXPAND | wxALIGN_BOTTOM);
   dialog.Fit();

   // Valor por default
   wxString newgroupname = _(content_DEFAULT_GROUP_NAME);
   ptxtctrl->SetValidator(wxTextValidator(wxFILTER_NONE, &newgroupname));
   ptxtctrl->SelectAll();
   ptxtctrl->SetFocusFromKbd();

   // muestro el dialogo
   if (dialog.ShowModal() != wxID_OK)
      return false;

   GroupName = newgroupname;

   return true;
}

/** importador de capa como area de entrenamiento **/
void LayerTool::ExecuteClassConverter() const {
   EnclosureManipulator manipulator;
   if (pDataViewManager_) {
      VectorDatasource* pvectords = ToolSupport::GetVectorDatasource(
            pDataViewManager_->GetDatasourceManager(),
            pDataViewManager_->GetViewcontextManager());
      if (!pvectords || !manipulator.AddNature(pvectords)) {
         SHOW_ERROR(_(message_CANT_CONVERT_TO_CLASS));
      }
      /** TODO(Gabriel - TCK #6127)
       * Se debe replicar el addnature a las capas para que de esta
       *  forma no importa que capa tome para guardar los proyectos
       *  se encuentre la metadata en el xml que indica que es valida
       *  como area de clasificacion
       */
      std::vector<SuriObject::UuidType> layerids =
            pDataViewManager_->GetViewcontextManager()->GetAssociatedLayer(pvectords->GetId());
      std::vector<SuriObject::UuidType>::iterator it = layerids.begin();
      for (; it != layerids.end(); ++it) {
         LayerInterface* player =
               pDataViewManager_->GetViewcontextManager()->GetLayer(*it);
         manipulator.AddNature(player->GetElement());
      }
   } else {
      SHOW_ERROR(_(message_CANT_CONVERT_TO_CLASS));
   }
}

/** Ejecuta el comando para calcular el buffer para capas vectoriales **/
void LayerTool::ExecuteExportBufferVectorLayer() const {
   VectorDatasource* pvector = ToolSupport::GetVectorDatasource(
         pDataViewManager_->GetDatasourceManager(),
         pDataViewManager_->GetViewcontextManager());
   RasterElement* praster = ToolSupport::GetActiveRaster(
         GetViewcontextManager(),
         GetViewcontextManager()->GetSelectedViewcontextId());
   if (!praster
         || SpatialReference::IsPixelLineSpatialRef(
               praster->GetSpatialReference().c_str())) {
      SHOW_WARNING(message_CANT_START_PROCESS_WITHOUT_SR);
      return;
   }
   World* pworld = ToolSupport::GetWorld(GetViewportManager());
   Element* pelement = NULL;
   ProcessAdaptLayer* ppal=NULL;
   if (pvector) {
      bool showspatial = false;
      bool showbuffer = true;
      Subset viewerwindow;
      if (!pworld) {
         pworld = new World();
         pworld->SetSpatialReference(pvector->GetSpatialReference());
         Subset extent;
         pvector->GetElement()->GetElementExtent(extent);
         pworld->SetWorld(extent);
         pworld->SetWindow(extent);
      }
      pworld->GetWorld(viewerwindow);
      std::vector<DatasourceInterface*> datasources;
      VectorDatasource* pvector = ToolSupport::GetVectorDatasource(
            pDataViewManager_->GetDatasourceManager(),
            pDataViewManager_->GetViewcontextManager());
      datasources.push_back(pvector);

      VectorOperationProcess process(datasources, viewerwindow, pelement,
                                               pDataViewManager_);
      ppal=process.GetProcessAdaptLayer();
      ppal->AddAttribute<bool>(ProcessAdaptLayer::IncludeBufferKeyAttr,
                                      showbuffer);
      ppal->AddAttribute<bool>(ProcessAdaptLayer::IncludeSpatialSelKeyAttr,
                                   showspatial);
      ppal->AddAttribute<std::vector<DatasourceInterface*> >(
                       ProcessAdaptLayer::VectorOperationDatasourcesKeyAttr, datasources);
      process.OnProcessAdaptLayerSet();
      suri::ProcessNotebookWidget* pprocesswidget = new suri::ProcessNotebookWidget(
            &process, caption_BUFFER_CALCULATION, GetLibraryManager());
      if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
         DatasourceInterface* pdatasource = NULL;
         const char *pdatasourcetypes[] = { "VectorDatasource" };
         for (int i = 0; i < 1 && pdatasource == NULL; ++i)
            pdatasource = DatasourceInterface::Create(pdatasourcetypes[i], pelement);
         if (pdatasource)
            GetDatasourceManager()->AddDatasource(pdatasource);
      }
   } else {
      SHOW_WARNING(message_PROCESS_EXPORT_VECTOR_LAYER_START_ERROR);
   }
}

/** Ejecuta el comando para calcular operaciones vectoriales **/
void LayerTool::ExecuteMergeLayers() const {
   VectorDatasource* pvector = ToolSupport::GetVectorDatasource(
         pDataViewManager_->GetDatasourceManager(),
         pDataViewManager_->GetViewcontextManager());
   RasterElement* praster = ToolSupport::GetActiveRaster(
         GetViewcontextManager(),
         GetViewcontextManager()->GetSelectedViewcontextId());
   if (!praster
         || SpatialReference::IsPixelLineSpatialRef(
               praster->GetSpatialReference().c_str())) {
      SHOW_WARNING(message_CANT_START_PROCESS_WITHOUT_SR);
      return;
   }
   World* pworld = ToolSupport::GetWorld(GetViewportManager());
   Element* pelement = NULL;
   ProcessAdaptLayer* ppal=NULL;
   bool hasdatasources = false;
   if (pvector) {
      bool showspatial = false;
      bool showvectoroperation = true;
      Subset viewerwindow;
      if (!pworld) {
         pworld = new World();
         pworld->SetSpatialReference(pvector->GetSpatialReference());
         Subset extent;
         pvector->GetElement()->GetElementExtent(extent);
         pworld->SetWorld(extent);
         pworld->SetWindow(extent);
      }
      pworld->GetWorld(viewerwindow);
      std::vector<DatasourceInterface*> datasources;

      // Agrego las fuentes de datos al vector
      std::list<NodePath> selectednodes =
            pDataViewManager_->GetSelectionManager()->GetSelectedNodes();
      std::list<suri::NodePath>::iterator it = selectednodes.begin();
      for (; it != selectednodes.end(); ++it) {
         DatasourceInterface* pds = GetDatasourceManager()->GetDatasource(
               it->GetLastPathNode()->GetContent());
         if (pds != NULL && pds->GetClassId().compare("VectorDatasource") == 0) {
            datasources.push_back(pds);
         }
      }

      // Para iniciar el proceso necesito al menos dos fuentes de datos
      if (datasources.size() > 1)
         hasdatasources = true;
      VectorOperationProcess process(datasources, viewerwindow, pelement,
                                            pDataViewManager_);
      ppal->AddAttribute<bool>(ProcessAdaptLayer::IncludeMergeLayersKeyAttr,
                                  showvectoroperation);
      ppal->AddAttribute<bool>(ProcessAdaptLayer::IncludeSpatialSelKeyAttr,
                                    showspatial);
      ppal->AddAttribute<std::vector<DatasourceInterface*> >(
                     ProcessAdaptLayer::VectorOperationDatasourcesKeyAttr, datasources);
      process.OnProcessAdaptLayerSet();
      if (hasdatasources) {
         suri::ProcessNotebookWidget* pprocesswidget = new suri::ProcessNotebookWidget(
               &process, caption_MERGE_LAYERS, GetLibraryManager());
         if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
            DatasourceInterface* pdatasource = NULL;
            const char *pdatasourcetypes[] = { "VectorDatasource" };
            for (int i = 0; i < 1 && pdatasource == NULL; ++i)
               pdatasource = DatasourceInterface::Create(pdatasourcetypes[i], pelement);
            if (pdatasource)
               GetDatasourceManager()->AddDatasource(pdatasource);
         }
      } else {
         SHOW_WARNING(message_PROCESS_MERGE_LAYERS_START_ERROR);
      }
   }else{
     SHOW_WARNING(message_PROCESS_MERGE_LAYERS_START_ERROR);
   }
}

/** Ejecuta el comando para fusionar capas vectoriales **/
void LayerTool::ExecuteExportVectorOperationLayer() const {
   VectorDatasource* pvector = ToolSupport::GetVectorDatasource(
         pDataViewManager_->GetDatasourceManager(),
         pDataViewManager_->GetViewcontextManager());
   RasterElement* praster = ToolSupport::GetActiveRaster(
         GetViewcontextManager(),
         GetViewcontextManager()->GetSelectedViewcontextId());
   if (!praster
         || SpatialReference::IsPixelLineSpatialRef(
               praster->GetSpatialReference().c_str())) {
      SHOW_WARNING(message_CANT_START_PROCESS_WITHOUT_SR);
      return;
   }
   World* pworld = ToolSupport::GetWorld(GetViewportManager());
   Element* pelement = NULL;
   ProcessAdaptLayer* ppal=NULL;
   bool hasdatasources = false;
   if (pvector) {
      bool showspatial = false;
      bool showvectoroperation = true;
      Subset viewerwindow;
      if (!pworld) {
         pworld = new World();
         pworld->SetSpatialReference(pvector->GetSpatialReference());
         Subset extent;
         pvector->GetElement()->GetElementExtent(extent);
         pworld->SetWorld(extent);
         pworld->SetWindow(extent);
      }
      pworld->GetWorld(viewerwindow);
      std::vector<DatasourceInterface*> datasources;

      ViewcontextInterface* pviewcontext =
            pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();
      LayerList* playerlist = pviewcontext->GetLayerList();
      int elementcount = playerlist->GetElementCount();
      for (int i = 0; i < elementcount; i++) {
         Element* pelement = playerlist->GetElement(i);
         LayerInterface* player = pviewcontext->GetLayerByElementId(pelement->GetUid());
         SuriObject::UuidType dsid = player->GetAssociatedDatasourceId();
         DatasourceInterface* pvector =
               dynamic_cast<VectorDatasource*>(
                     pDataViewManager_->GetDatasourceManager()->GetDatasource(dsid));
         if (pvector)
            datasources.push_back(pvector);
      }

      if (datasources.size() > 1)
         hasdatasources = true;
      VectorOperationProcess process(datasources, viewerwindow, pelement,
                                               pDataViewManager_);
      ppal->AddAttribute<bool>(ProcessAdaptLayer::IncludeVectorOperationKeyAttr,
                                showvectoroperation);
      ppal->AddAttribute<bool>(ProcessAdaptLayer::IncludeSpatialSelKeyAttr,
                                  showspatial);
      ppal->AddAttribute<std::vector<DatasourceInterface*> >(
                         ProcessAdaptLayer::InputDatasourcesKeyAttr, datasources);
      process.OnProcessAdaptLayerSet();
      if (hasdatasources) {
         process.OnProcessAdaptLayerSet();
         suri::ProcessNotebookWidget* pprocesswidget = new suri::ProcessNotebookWidget(
               &process, caption_VECTOR_OPERATION, GetLibraryManager());
         if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
            DatasourceInterface* pdatasource = NULL;
            const char *pdatasourcetypes[] = { "VectorDatasource" };
            for (int i = 0; i < 1 && pdatasource == NULL; ++i)
               pdatasource = DatasourceInterface::Create(pdatasourcetypes[i], pelement);
            if (pdatasource)
               GetDatasourceManager()->AddDatasource(pdatasource);
         }
      } else {
         SHOW_WARNING(message_PROCESS_EXPORT_VECTOR_OPERATION_LAYER_START_ERROR);
      }
   } else {
      SHOW_WARNING(message_PROCESS_EXPORT_VECTOR_OPERATION_LAYER_START_ERROR);
   }
}

#ifdef __DEBUG__
/**
 * Accion de "Ejecutar" de AddEnclosure
 */
void LayerTool::ExecuteAddEnclosure() const {
   std::string filter = filter_VIEWER_2D_ELEMENTS;
   suri::DatasourceManagerInterface* dm = GetDatasourceManager();
   wxFileDialog filedialog(NULL, _(caption_SELECT_ELEMENT), wxT(""), wxT(""), filter);
   if (filedialog.ShowModal() == wxID_OK) {
      std::string filename = filedialog.GetPath().c_str();
      suri::DatasourceInterface* pdatasource = NULL;
      pdatasource = suri::DatasourceInterface::Create("VectorDatasource", filename);
      EnclosureManipulator manipulator;
      if (pdatasource == NULL) {
         Option option;
         option.SetOption(EnclosureManipulator::DriverOptionName, "ESRI Shapefile");
         option.SetOption(EnclosureManipulator::SpatialReferenceOptionName, "");
         pdatasource = manipulator.Create(filename, option);
      } else {
         manipulator.AddNature(pdatasource);
      }
      EnclosureValidator validator;
      bool ok = false;
      if (validator.IsValid(pdatasource))
         ok = dm->AddDatasource(pdatasource);
      if (!ok)
         SHOW_ERROR(message_ADD_ELEMENT_ERROR);
   }
}

/**
 * Accion de "Ejecutar" de AddClassifiedRaster
 */
void LayerTool::ExecuteAddClassifiedRaster() const {
   std::string filter = filter_VIEWER_2D_ELEMENTS;
   suri::DatasourceManagerInterface* dm = GetDatasourceManager();
   wxFileDialog filedialog(NULL, _(caption_SELECT_ELEMENT), wxT(""), wxT(""), filter);
   if (filedialog.ShowModal() == wxID_OK) {
      std::string filename = filedialog.GetPath().c_str();
      DatasourceInterface* pdatasource = suri::DatasourceInterface::Create(
            "RasterDatasource", filename);
      // Inicio Codigo __DEBUG__
      ClassifiedRasterDatasourceManipulator manipulator;
      if (pdatasource == NULL) {
         Option option;
         option.SetOption(ClassifiedRasterDatasourceManipulator::DriverOptionName,
                          "GTiff");
         option.SetOption(ClassifiedRasterDatasourceManipulator::SizeXOptionName,
                          "100");
         option.SetOption(ClassifiedRasterDatasourceManipulator::SizeYOptionName,
                          "100");
         pdatasource = manipulator.Create(filename, option);
         pdatasource = manipulator.Create(filename, option);
      } else {
         manipulator.AddNature(pdatasource);
      }
      ClassifiedRasterDatasourceValidator validator;
      bool ok = false;
      if (validator.IsValid(pdatasource))
         ok = dm->AddDatasource(pdatasource);
      // Fin Codigo __DEBUG__
      if (!ok)
         SHOW_ERROR(message_ADD_ELEMENT_ERROR);
   }
}

/**
 * Ejecuta el comando para limpiar una recinto
 */
void LayerTool::ExecuteCleanEnclosure() const {
   ViewcontextInterface* pviewcontext =
         GetViewcontextManager()->GetSelectedViewcontext();
   LayerList* playerlist = pviewcontext->GetLayerList();
   Element* pelement = playerlist->GetElement(
         playerlist->GetSelectedElementUid(suri::LayerList::AllGroups));
   LayerInterface* player = pviewcontext->GetLayerByElementId(pelement->GetUid());
   SuriObject::UuidType datasourceId = player->GetAssociatedDatasourceId();
   DatasourceManagerInterface *pdsmanager = GetDatasourceManager();
   DatasourceInterface* pdatasource = pdsmanager->GetDatasource(datasourceId);
   // Si no tengo al menos uno seleccionado, no hago nada
   if (playerlist->GetSelectedCount() < 1)
      return;
   // Valido y Manipulo
   EnclosureValidator validator;
   EnclosureManipulator manipulator;
   if (pdatasource != NULL && validator.IsValid(pdatasource)) {
      manipulator.CleanNature(pdatasource);
   }
}

/**
 * Ejecuta el comando para limpiar un raster clasificado
 */
void LayerTool::ExecuteCleanClassifiedRaster() const {
   ViewcontextInterface* pviewcontext =
         GetViewcontextManager()->GetSelectedViewcontext();
   LayerList* playerlist = pviewcontext->GetLayerList();
   Element* pelement = playerlist->GetElement(
         playerlist->GetSelectedElementUid(suri::LayerList::AllGroups));
   LayerInterface* player = pviewcontext->GetLayerByElementId(pelement->GetUid());
   SuriObject::UuidType datasourceId = player->GetAssociatedDatasourceId();
   DatasourceManagerInterface* pdsmanager = GetDatasourceManager();
   DatasourceInterface* pdatasource = pdsmanager->GetDatasource(datasourceId);
   // Si no tengo al menos uno seleccionado, no hago nada
   if (playerlist->GetSelectedCount() < 1)
      return;
   // Valido y Manipulo
   ClassifiedRasterDatasourceValidator validator;
   ClassifiedRasterDatasourceManipulator manipulator;
   if (pdatasource != NULL && validator.IsValid(pdatasource)) {
      manipulator.CleanNature(pdatasource);
   }
}

/**
 * Ejecuta el comando para validar una recinto
 */
void LayerTool::ExecuteValidateEnclosure() const {
   ViewcontextInterface* pviewcontext =
         GetViewcontextManager()->GetSelectedViewcontext();
   LayerList* playerlist = pviewcontext->GetLayerList();
   Element* pelement = playerlist->GetElement(
         playerlist->GetSelectedElementUid(suri::LayerList::AllGroups));
   LayerInterface* player = pviewcontext->GetLayerByElementId(pelement->GetUid());
   SuriObject::UuidType datasourceId = player->GetAssociatedDatasourceId();
   DatasourceManagerInterface* pdsmanager = GetDatasourceManager();
   DatasourceInterface *pdatasource = pdsmanager->GetDatasource(datasourceId);
   // Si no tengo al menos uno seleccionado, no hago nada
   if (playerlist->GetSelectedCount() < 1)
      return;
   // Valido y Manipulo
   EnclosureValidator validator;
   if (pdatasource != NULL && validator.IsValid(pdatasource)) {
      SHOW_STATUS("Recinto: VALIDADO");
   } else {
      SHOW_STATUS("Recinto: NO VALIDADO");
   }
}

/**
 * Ejecuta el comando para validar un raster clasificado
 */
void LayerTool::ExecuteValidateClassifiedRaster() const {
   ViewcontextInterface* pviewcontext =
         GetViewcontextManager()->GetSelectedViewcontext();
   LayerList* playerlist = pviewcontext->GetLayerList();
   Element* pelement = playerlist->GetElement(
         playerlist->GetSelectedElementUid(suri::LayerList::AllGroups));
   LayerInterface* player = pviewcontext->GetLayerByElementId(pelement->GetUid());
   SuriObject::UuidType datasourceId = player->GetAssociatedDatasourceId();
   DatasourceManagerInterface* pdsmanager = GetDatasourceManager();
   DatasourceInterface* pdatasource = pdsmanager->GetDatasource(datasourceId);
   // Si no tengo al menos uno seleccionado, no hago nada
   if (playerlist->GetSelectedCount() < 1)
      return;
   // Valido y Manipulo
   ClassifiedRasterDatasourceValidator validator;
   if (pdatasource != NULL && validator.IsValid(pdatasource)) {
      SHOW_STATUS("Raster Clasificado: VALIDADO");
   } else {
      SHOW_STATUS("Raster Clasificado: NO VALIDADO");
   }
}

/** Ejecuta la traduccion layer-Element **/
void LayerTool::ExecuteLayerElementTranslation(bool Direct, bool Apply) const {
   ExecuteTranslation(0, Direct, Apply);
}

/** Ejecuta la traduccion Datasource-Element **/
void LayerTool::ExecuteDatasourceElementTranslation(bool Direct, bool Apply) const {
   ExecuteTranslation(1, Direct, Apply);
}

/** Ejecuta la traduccion que corresponde al id pasado por parametro
 * @param[in] IdTranslation id de la traduccion a realizar
 * id 0 = LayerElement, 1 Datasource-Element
 * @param[in] Direct booleano que indica si se realiza traduccion directa (true)
 * o indirecta (false)
 */
void LayerTool::ExecuteTranslation(int IdTranslation, bool Direct, bool Apply) const {
   ViewcontextInterface* pviewcontext =
         GetViewcontextManager()->GetSelectedViewcontext();
   LayerList* playerlist = pviewcontext->GetLayerList();
   Element* pelement = playerlist->GetElement(
         playerlist->GetSelectedElementUid(suri::LayerList::AllGroups));
   LayerInterface* player = pviewcontext->GetLayerByElementId(pelement->GetUid());
   SHOW_WARNING("Nombre Datasource= %s",
                player->FetchAssociatedDatasource()->GetName().c_str());
   XmlTranslatorInterface* ptranslator = NULL;
   if (IdTranslation == 1) {
      suri::DatasourceManagerInterface* dm = GetDatasourceManager();
      DatasourceInterface* pdatasource = dm->GetDatasource(
            player->GetAssociatedDatasourceId());
      ptranslator = suri::XmlTranslatorBuilder::GetDatasourceElementXmlTranslator(
            pdatasource, NULL);
   } else {
      ptranslator = suri::XmlTranslatorBuilder::GetLayerElementXmlTranslator(player,
                                                                             NULL);
   }
   if (!Apply) {
      wxXmlNode* ptranslationnode = ptranslator->GenerateXmlTranslation(Direct);
      wxXmlDocument doc;
      doc.SetRoot(ptranslationnode);
      doc.SetEncoding(suri::XmlElement::xmlEncoding_);
      doc.SetFileEncoding(suri::XmlElement::xmlEncoding_);
      wxString temp;
      wxStringOutputStream ss(&temp);
      doc.Save(ss);
      doc.DetachRoot();
      delete ptranslationnode;
      SHOW_WARNING(temp);
   } else {
      ptranslator->ApplyTranslation(Direct);
   }
   delete ptranslator;
}

/** Aplica dos traducciones, de forma de poder cruzar las fuente de datos asociadas
 *  entre dos capas
 */
void LayerTool::ExecuteSwitchTranslation() const {
   ViewcontextInterface* pviewcontext =
         GetViewcontextManager()->GetSelectedViewcontext();
   LayerList* playerlist = pviewcontext->GetLayerList();
   Element* pfirstelement = playerlist->GetElement(0, true);
   Element* psecondelement = playerlist->GetElement(1, true);
   if (pfirstelement && psecondelement) {
      Element* pcopyfirst = Element::Create(pfirstelement->GetClassId(),
                                            pfirstelement->GetNode());
      Element* pcopysecond = Element::Create(psecondelement->GetClassId(),
                                             psecondelement->GetNode());
      LayerInterface* pfirstlayer = pviewcontext->GetLayerByElementId(
            pfirstelement->GetUid());
      LayerInterface* psecondlayer = pviewcontext->GetLayerByElementId(
            psecondelement->GetUid());
      XmlTranslatorInterface* pfirsttranslator =
            suri::XmlTranslatorBuilder::GetLayerElementXmlTranslator(pfirstlayer,
                                                                     pcopysecond);
      XmlTranslatorInterface* psecondtranslator =
            suri::XmlTranslatorBuilder::GetLayerElementXmlTranslator(psecondlayer,
                                                                     pcopyfirst);
      pfirsttranslator->ApplyTranslation(false);
      psecondtranslator->ApplyTranslation(false);
      delete pfirsttranslator;
      delete psecondtranslator;
      Element::Destroy(pcopyfirst);
      Element::Destroy(pcopysecond);
   }
}

/**
 * Ejecuta el componente CGU en modo ReadOnly o ReadWrite.
 * Este metodo hace de editor in situ. Es decir, del componente
 * que invocaria y configuraria el cgu
 */
void LayerTool::ExecuteShowCgu(bool ReadOnly) const {
//   static int numberofcalls = 0;
   static LibraryManager* pmanager = new LibraryManager();
   // TODO(Gabriel - TCK #2249): Ver donde hacer
   // la carga de biblioteca tendria que estar en
   // otro lado
   Library* plibrary = new Library();
   LibraryClient libraryclient(pmanager, suri::LibraryManager::ALL);
   libraryclient.AddLibrary(plibrary);
   suri::LibraryItemEditorDebugPart* peditor = new suri::LibraryItemEditorDebugPart(
         plibrary->GetId(), pmanager);
   long visiblebuttons = SUR_BTN_OK | SUR_BTN_CANCEL | SUR_BTN_APPLY;
   suri::PartContainerWidget* ppartcontainer = new suri::PartContainerWidget(
         peditor, wxT(""), visiblebuttons);
   ppartcontainer->ShowModal(true);
}

/** Metodo que carga una lista de gcp a partir de un archivo*/
void LayerTool::ExecuteGcpLoaderFromTxt() const {
   std::string path = Configuration::GetParameter("app_data_dir", wxT("./"));
   path.append("/gcpenvi.txt");
   suri::TablePartConfigurator tableconf;
   suri::Option config = suri::TablePartConfigurator::GetCorregisterTableEditorConfig();
   config.SetOption(TablePartConfigurator::TablePartFileNameKey, path);
   suri::TablePart* ptablepart = tableconf.Initialize(pDataViewManager_, config);
   if (ptablepart == NULL) {
      return;
   }
   PartContainerWidget* pcontainer = new PartContainerWidget(ptablepart);
   pcontainer->ShowModal();
}

/** Activa/Desactiva los botones de la toolbar**/
void LayerTool::ExecuteActiveCommands(const std::string& CommandNameCaller) const {
   static bool active = false;
   std::list<std::string> commandnames = GetCommandNames();
   std::list<std::string>::iterator it = commandnames.begin();
   while (it != commandnames.end()) {
      std::string name = *it;
      if (name != CommandNameCaller) {
         Command *pcommand = GetCommand(GetId(name));
         if (pcommand)
            pcommand->SetActive(active);
      }
      ++it;
   }
   active = !active;
}
/** Habilita/Deshabilita los botones de la toolbar**/
void LayerTool::ExecuteEnableCommands(const std::string& CommandNameCaller) const {
   static bool enable = false;
   std::list<std::string> commandnames = GetCommandNames();
   std::list<std::string>::iterator it = commandnames.begin();
   while (it != commandnames.end()) {
      if (*it != CommandNameCaller) {
         Command *pcommand = GetCommand(GetId(*it));
         if (pcommand)
            pcommand->SetEnabled(enable);
      }
      ++it;
   }
   enable = !enable;
}
#endif
/**
 * Si el comando indicado, se encuentra entre la lista de ejecutables, se ejecutara
 * @param[in]  pToolCommand: Comando a ejecutar
 */
void LayerTool::Execute(const Command* pToolCommand) {
   int id = pToolCommand->GetId();
   if (id == GetId("AddLayer")) {
      ExecuteAddLayer();
   } else if (id == GetId("AddRemoteLayer")) {
      ExecuteAddRemoteLayer();
   } else if (id == GetId("AddCsvLayer")) {
      ExecuteAddCsvLayer();
   } else if (id == GetId("AddTerrainLayer")) {
      ExecuteAddTerrainLayer();
   } else if (id == GetId("RemoveLayer")) {
      ExecuteRemoveLayer();
   } else if (id == GetId("DisplayLayers")) {
      ExecuteDisplayLayers();
   } else if (id == GetId("HideLayers")) {
      ExecuteHideLayers();
   } else if (id == GetId("DisplayLayersInAllViewcontexts")) {
      ExecuteDisplayLayersInAllViewcontexts();
   } else if (id == GetId("HideLayersInAllViewcontexts")) {
      ExecuteHideLayersInAllViewcontexts();
   } else if (id == GetId("ShowLayerProperties")) {
      ExecuteShowLayerProperties();
   } else if (id == GetId("CreateGroup")) {
      ExecuteCreateGroup();
   } else if (id == GetId("Visualization")) {
      ExecuteVisualization();
   } else if (id == GetId("ExportLayer")) {
      ExecuteExportLayer();
   } else if (id == GetId("ExportVectorLayer")) {
      ExecuteExportVectorLayer();
   } else if (id == GetId("ExportLayerSelection")) {
      ExecuteExportLayerSelection();
   } else if (id == GetId("StackBands")) {
      ExecuteStackBands();
   } else if (id == GetId("ClassConverter")) {
      ExecuteClassConverter();
   } else if (id == GetId("ExportBufferVectorLayer")) {
      ExecuteExportBufferVectorLayer();
   } else if (id == GetId("MergeLayers")) {
      ExecuteMergeLayers();
   }  else if (id == GetId("RasterGenericImporter")) {
      ExecuteImportGenericRaster();
   } else if (id == GetId("ExportVectorOperationLayer")) {
      ExecuteExportVectorOperationLayer();
   }
}

/**
 * Fabrica el comando si es uno de los predefinidos
 * @param[in]  CommandName: Nombre del comando a ejecutar
 * @return     Devuelve el comando creado si tuvo exito, sino NULL
 */
Command* LayerTool::CreateCommand(const std::string &CommandName) {
   Command* pcommand = NULL;
   if (CommandName == "AddLayer") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu|Button");
      pcommand->SetDescription(tooltip_LAYER_ADD_BUTTON);
      pcommand->SetDecoration(tool_LAYER_ADD);
      pcommand->SetLocation("ID_ADD_LAYER_MENUTIEM|ID_CONATINERLEFT");
//      pcommand->SetLocation("ID_ADD_LAYER_MENUTIEM");
   } else if (CommandName == "AddRemoteLayer") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_LAYER_ADD_REMOTE_BUTTON);
      pcommand->SetDecoration(tool_LAYER_ADD_REMOTE);
      pcommand->SetLocation("ID_ADD_REMOTE_LAYER_MENUTIEM");
   } else if (CommandName == "AddCsvLayer") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_LAYER_ADD_CSV_BUTTON);
      pcommand->SetDecoration(tool_LAYER_ADD);
      pcommand->SetLocation("ID_ADD_CSV_LAYER_MENUTIEM");
   }  else if (CommandName == "AddTerrainLayer") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_LAYER_ADD_TERRAIN_BUTTON);
      pcommand->SetLocation("ID_ADD_TERRAIN_LAYER_MENUTIEM");
   } else if (CommandName == "RemoveLayer") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu|Button");
      pcommand->SetDescription(tooltip_LAYER_REMOVE_BUTTON);
      pcommand->SetDecoration(tool_LAYER_REMOVE);
      pcommand->SetLocation("ID_REMOVE_LAYER_MENUTIEM|ID_CONATINERLEFT");
      pcommand->SetEnabled(true);
   } else if (CommandName == "DisplayLayers") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_LAYER_DISPLAY_BUTTON);
      pcommand->SetDecoration(tool_LAYER_DISPLAY);
      pcommand->SetLocation("ID_SHOW_LAYER_MENUTIEM");
   } else if (CommandName == "HideLayers") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_LAYER_HIDE_BUTTON);
      pcommand->SetDecoration(tool_LAYER_HIDE);
      pcommand->SetLocation("ID_HIDE_LAYER_MENUTIEM");
   } else if (CommandName == "ShowLayerProperties") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_LAYER_SHOW_PROPERTIES_BUTTON);
      pcommand->SetDecoration(tool_LAYER_SHOW_PROPERTIES);
      pcommand->SetLocation("ID_LAYER_PROPERTIES_MENUTIEM");
   } else if (CommandName == "DisplayLayersInAllViewcontexts") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_LAYER_DISPLAY_ALL_BUTTON);
      pcommand->SetLocation("ID_SHOW_LAYER_ALL_MENUTIEM");
   } else if (CommandName == "HideLayersInAllViewcontexts") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_LAYER_HIDE_ALL_BUTTON);
      pcommand->SetLocation("ID_HIDE_LAYER_ALL_MENUTIEM");
   } else if (CommandName == "CreateGroup") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_LAYER_CREATE_GROUP_BUTTON);
      pcommand->SetLocation("ID_CREATE_GROUP_MENUTIEM");
   } else if (CommandName == "ExportLayer") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_LAYER_EXPORT_LAYER_BUTTON);
      pcommand->SetDecoration(tool_LAYER_EXPORT);
      pcommand->SetLocation("ID_EXPORT_LAYER_MENUTIEM");
   } else if (CommandName == "ExportVectorLayer") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_LAYER_EXPORT_LAYER_BUTTON);
      pcommand->SetDecoration(tool_LAYER_EXPORT);
      pcommand->SetLocation("ID_EXPORT_VECTOR_LAYER_MENUTIEM");
   } else if (CommandName == "ExportLayerSelection") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_LAYER_EXPORT_LAYER_BUTTON);
      pcommand->SetLocation("ID_EXPORT_LAYER_SELECTION_MENUTIEM");
   } else if (CommandName == "StackBands") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetLocation("ID_BAND_STACKING_MENUTIEM");
      pcommand->SetEnabled(true);
   } else if (CommandName == "Visualization") {
      pcommand = NULL;
   } else if (CommandName == "ClassConverter") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetLocation("ID_CONVERT_AREA_MENUTIEM");
      pcommand->SetEnabled(true);
   } else if (CommandName == "ExportBufferVectorLayer") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(caption_BUFFER_CALCULATION);
      pcommand->SetDecoration(tool_LAYER_EXPORT);
      pcommand->SetLocation("ID_EXPORT_BUFFER_VECTOR_LAYER_MENUTIEM");
   } else if (CommandName == "MergeLayers") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(caption_MERGE_LAYERS);
      pcommand->SetLocation("ID_MERGE_LAYERS_MENUTIEM");
   }  else if (CommandName == "RasterGenericImporter") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(caption_RASTER_IMPORTER);
      pcommand->SetLocation("ID_RASTER_GENERIC_IMPORTER_MENUTIEM");
   } else if (CommandName == "ExportVectorOperationLayer") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(caption_BUFFER_CALCULATION);
      pcommand->SetDecoration(tool_LAYER_EXPORT);
      pcommand->SetLocation("ID_EXPORT_VECTOR_OPERATION_LAYER_MENUTIEM");
   }
   return pcommand;
}

/**
 * Destruye un comando previamente creado con CreateCommand
 * @param[in]  pCommand: Puntero a un Comando creado no NULL a destruir
 *
 */
void LayerTool::DestroyCommand(Command* &pCommand) const {
   delete pCommand;
   pCommand = NULL;
}

/**
 * Creador del Widget
 */
Widget* LayerTool::CreateWidget(const std::string &Widget) {
   // TODO(Sebastian - TCK #878): Deteccion de codigo sin implementar
   return NULL;
}
} /** namespace suri */
