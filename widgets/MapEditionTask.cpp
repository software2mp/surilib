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
