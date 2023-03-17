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

#ifndef MAPEDITIONTASK_H_
#define MAPEDITIONTASK_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "suri/TaskInterface.h"
#include "suri/MapTool.h"
#include "suri/World.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class LayerList;
class RasterElement;

/*!
 * Clase que representa la tarea de creacion de mapas
 */
class MapEditionTask : public TaskInterface {
   /** Ctor. copia **/
   MapEditionTask(const MapEditionTask&);

public:
   /** Constructor **/
   MapEditionTask(World* pWorld, wxWindow* pParent);
   /** Destructor **/
   virtual ~MapEditionTask();
   /** Comienza la tarea */
   virtual bool Start();
   /** Finaliza la tarea. Intenta guardar los cambios. */
   virtual bool End();
   /** Cancela la tarea sin guardar */
   virtual bool Cancel();
   /** Indica si la tarea esta activa */
   virtual bool IsActive() const;
   /** Muestra/oculta la leyenda*/
   bool ShowLegend(bool Show);
   /** Muestra/oculta el norte*/
   bool ShowNorth(bool Show);
   /** Muestrala grilla con los grados que se indican por parametro*/
   bool ShowGrid(const std::string& GridConfig);
   /** Muestra/oculta la escala*/
   bool ShowScale(bool Show);
   /** Indica cual es la lista de elementos activa **/
   void SetActiveElementList(LayerList* pList);
   /** Nombre de la tarea */
   static const std::string TASKNAME;
   /** Nombre clave que se utiliza par la lista de elementos **/
   static const std::string ELEMENTLIST;

private:
   /** bool que indica si comenzo la tarea **/
   bool started_;
   /** Herramienta para la creacion de mapas **/
   MapTool* pMapTool_;
   /** Actualiza lso parametros del renderizador en funcion de las herramientas
    *  de mapas habilitadas */
   void UpdateMapRendererParameters();
   /** bool que indica si se muestra la legenda en el mapa */
   bool legend_;
   /** bool que indica si se muestra la escala en el mapa */
   bool scale_;
   /** bool que indica si se muestra el norte en el mapa */
   bool north_;
   /** String con la configuracion actual de la grilla **/
   std::string gridConfig_;
};

} /** namespace suri */

#endif /* MAPEDITIONTASK_H_ */
