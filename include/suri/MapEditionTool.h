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

#ifndef MAPEDITIONTOOL_H_
#define MAPEDITIONTOOL_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/GenericTool.h"
// Includes Wx
// Defines
// forwards

namespace suri {
// forwards
class MapEditionTask;
class TaskConfig;

/*!
 *  Clase que representa una tarea de edicion de mapa.
 */
class MapEditionTool : public suri::GenericTool {
   /** ctor copia **/
   MapEditionTool(const MapEditionTool&);

public:
   /** Constructor **/
   MapEditionTool(DataViewManager* pDataViewManager, wxWindow* pParent);
   /** Destructor **/
   virtual ~MapEditionTool();
   /** Ejecucion del comando */
   virtual void Execute(const Command* pToolCommand);

protected:
   /** Creacion del Comando */
   virtual Command* CreateCommand(const std::string &CommandName);
   /** Destruccion del Comando */
   virtual void DestroyCommand(Command* &pCommand) const;
   /** Creacion del Widget */
   virtual Widget* CreateWidget(const std::string &Widget);
   /** Inicia la tarea de edicion de mapa */
   bool StartTask();
   /** Finaliza la tarea de edicion de mapa **/
   bool EndTask();

private:
   /** Metodo que se ejecuta cuando se apreta boton de mostrar leyenda */
   void ExecuteShowLegend(bool Show);
   /** Metodo que se ejecuta cuando se apreta boton de mostrar norte */
   void ExecuteShowNorth(bool Show);
   /** Metodo que se ejecuta cuando se apreta boton de mostrar escala */
   void ExecuteShowScale(bool Show);
   /** Metodo que se ejecuta cuando se apreta choice de grilla */
   void ExecuteShowGrid(const std::string& GridValue);
   /** Metodo que se ejecuta cuando se apreta boton de iniciar/finalizar tarea */
   void ExecuteTaskMenu(bool Active);
   /** Ventana para renderizar el mapa */
   wxWindow* pParent_;
   /** Referencia a la tarea */
   MapEditionTask* pMapTask_;
   /** Configurador para la tarea de mapa */
   TaskConfig* pTaskConfig_;
   /** Referencia al command de escala para poder desactivarlo/activarlo
    *  en funcion de la capa desplegada (las georreferenciados no tienen escala)
    */
   Command* pScaleCommand_;
   SuriObject::UuidType blockedDatasource_;
};

} /** namespace suri */

#endif /* MAPEDITIONTOOL_H_ */
