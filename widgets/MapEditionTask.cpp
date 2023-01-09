/*! \file MapEditionTask.cpp */
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
#include <vector>

// Includes Suri
#include "suri/MapEditionTask.h"
#include "suri/LayerList.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Nombre de la tarea */
const std::string MapEditionTask::TASKNAME = "MapEditionTask";
/** Nombre que se utiliza par la lista de elementos **/
const std::string MapEditionTask::ELEMENTLIST = "ElementList";

/** Constructor
 * @param pWorld mundo que posee el visualizador y sobre el cual se renderiza el mapa
 * @param pParent ventana sobre la cual se renderiza el mapa
 */
MapEditionTask::MapEditionTask(World* pWorld, wxWindow* pParent) :
      started_(false), legend_(false), scale_(false), north_(false) {
   std::vector<int> vecgroup;
   // Raster position
   vecgroup.push_back(4);
   // Vector position
   vecgroup.push_back(6);
   pMapTool_ = new MapTool(pWorld, vecgroup);
}

/** Destructor **/
MapEditionTask::~MapEditionTask() {
   delete pMapTool_;
}

/** Comienza la tarea */
bool MapEditionTask::Start() {
   started_ = true;
   return true;
}
/** Finaliza la tarea. Intenta guardar los cambios. */
bool MapEditionTask::End() {
   if (!started_)
      return false;

   ShowLegend(false);
   ShowNorth(false);
   ShowScale(false);
   ShowGrid(_(label_NO_GRID));

   started_ = false;
   return true;
}
/** Cancela la tarea sin guardar */
bool MapEditionTask::Cancel() {
   started_ = false;
   return false;
}
/** Indica si la tarea esta activa */
bool MapEditionTask::IsActive() const {
   return started_;
}

/** Muestra/oculta la leyenda*/
bool MapEditionTask::ShowLegend(bool Show) {
   if (!started_)
      return false;
   legend_ = Show;
   UpdateMapRendererParameters();
   return true;
}
/** Muestra/oculta el norte*/
bool MapEditionTask::ShowNorth(bool Show) {
   if (!started_)
      return false;
   north_ = Show;
   UpdateMapRendererParameters();
   return true;
}
/** Muestrala grilla con los grados que se indican por parametro*/
bool MapEditionTask::ShowGrid(const std::string& GridConfig) {
   if (!started_)
      return false;
   gridConfig_ = GridConfig;
   pMapTool_->UpdateGrid(GridConfig);
   UpdateMapRendererParameters();
   return true;
}
/** Muestra/oculta la escala*/
bool MapEditionTask::ShowScale(bool Show) {
   if (!started_)
      return false;
   scale_ = Show;
   UpdateMapRendererParameters();
   return true;
}

/** Indica cual es la lista de elementos activa **/
void MapEditionTask::SetActiveElementList(LayerList* pList) {
   if (pMapTool_)
      pMapTool_->SetList(pList);
}
/** Actualiza lso parametros del renderizador en funcion de las herramientas
 *  de mapas habilitadas */
void MapEditionTask::UpdateMapRendererParameters() {
   if (!pMapTool_)
      return;
   MapRenderer::Parameters params = pMapTool_->GetMapRendererParameters();
   params.legend_ = legend_;
   params.north_ = north_;
   params.scale_ = scale_;
   params.grid_ = pMapTool_->IsGridActive();
   pMapTool_->UpdateMapRendererParameters(params);
}
} /** namespace suri */
