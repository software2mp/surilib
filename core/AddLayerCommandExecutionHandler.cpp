/*! \file AddLayerCommandExecutionHandler.cpp */

/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib. 
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L. 
 *                                                                                                  
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/) 
 *  
 */

// Includes estandard 
#include <map>

// Includes Suri
#include "suri/AddLayerCommandExecutionHandler.h"
#include "suri/DataViewManager.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/Command.h"
#include "suri/VectorElement.h"
#include "suri/GenericTool.h"
#include "XmlMetadataHandler.h"

namespace suri {

namespace core {

/** Constructor **/
AddLayerCommandExecutionHandler::AddLayerCommandExecutionHandler(DataViewManager* pDataView) : 
											pDataView_(pDataView) {
}

/** Destructor **/
AddLayerCommandExecutionHandler::~AddLayerCommandExecutionHandler() {
}

/**
 * Metodo auxiliar que devuelve el tipo de grupo que corresponde
 * la fuente de datos que se pasa por parametro
 * @param[in] pDatasource fuente de datos sobre la cual se quiere obtener su grupo
 * @return LayerGroup grupo correspondiente a la definicion de grupos definida en el enum
 * LayerGroup
 */
AddLayerCommandExecutionHandler::LayerGroup GetLayerGroup(DatasourceInterface* pDatasource) {
   AddLayerCommandExecutionHandler::LayerGroup group = AddLayerCommandExecutionHandler::NoGroup;
   if (pDatasource->GetClassId().compare("RasterDatasource") == 0) {
      group = AddLayerCommandExecutionHandler::RasterGroup;
   } else {
      Vector::VectorType vtype =
            dynamic_cast<VectorElement*>(pDatasource->GetElement())->GetType();
      switch (vtype) {
         case Vector::Point:
            group = AddLayerCommandExecutionHandler::PointGroup;
            break;
         case Vector::Line:
            group = AddLayerCommandExecutionHandler::LineGroup;
            break;
         case Vector::Polygon:
            group = AddLayerCommandExecutionHandler::PolygonGroup;
            break;
         case Vector::GeometryCollection:
            group = AddLayerCommandExecutionHandler::GeometryCollectionGroup;
            break;
         default:
            break;
      }
   }
   REPORT_DEBUG("Agregando elemento al grupo %d", group);
   return group;
}

void AddLayerCommandExecutionHandler::AddLayer(const std::string& Filter) {
	suri::DatasourceManagerInterface* dm = pDataView_->GetDatasourceManager();
   wxFileDialog filedialog(NULL, _(caption_SELECT_ELEMENT), wxT(""), wxT(""), Filter,
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
         std::multimap<LayerGroup, DatasourceInterface*> layergroup;
         for (size_t i = 0; i < pathsarray.GetCount(); i++) {
            std::string filename = std::string((pathsarray.Item(i)).mb_str(wxConvUTF8));
            const char* pdatasourcetypes[] = { "RasterDatasource", "VectorDatasource" };
            DatasourceInterface* pdatasource = NULL;
            for (size_t i = 0; i < ARRAY_SIZE(pdatasourcetypes) && pdatasource == NULL; ++i){
               pdatasource = suri::DatasourceInterface::Create(pdatasourcetypes[i], filename);
            }
            if (pdatasource != NULL) {
               MetadataImporterInterface* pimport = new XmlMetadataHandler();
               pimport->ImportMetadata(pdatasource);
               layergroup.insert(std::make_pair(GetLayerGroup(pdatasource), pdatasource));
            }
         }
         std::map<LayerGroup, DatasourceInterface*>::iterator it = layergroup.begin();
         for (; it != layergroup.end(); ++it){
            DatasourceInterface* pDatasource = it->second;
            ok = ok && dm->AddDatasource(pDatasource);
         }
         if (!ok)
            SHOW_ERROR(message_ADD_ELEMENT_ERROR);
      }
   }
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool AddLayerCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
	int id = pCommand->GetId();
	if (id == pTool->GetId("AddLayer")) {
		std::string filter = filter_VIEWER_2D_ELEMENTS;
		AddLayer(filter);
	} else
		return false;
	return true;
}


}  /** namespace core **/
}  /** namespace suri **/