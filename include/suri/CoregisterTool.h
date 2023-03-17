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

#ifndef CORREGISTERTOOL_H_
#define CORREGISTERTOOL_H_

// Includes suri
#include "suri/GenericTool.h"

namespace suri {

//fordwards
class CoregisterTask;

/** Tool con los botones de corregistro */
class CoregisterTool : public GenericTool {
public:
   /** Ctor */
   CoregisterTool(DataViewManager* pDataViewManager);
   /** Dtor */
   virtual ~CoregisterTool();

   /** Ejecucion del comando */
   virtual void Execute(const Command* pToolCommand);

protected:
   /** Creacion del Comando */
   virtual Command* CreateCommand(const std::string &CommandName);
   /** Destruccion del Comando */
   virtual void DestroyCommand(Command* &pCommand) const;
   /** Creacion del Widget */
   virtual Widget* CreateWidget(const std::string &Widget);

private:
   /** Inicia la tarea de coregistro */
   bool StartTask(const std::string &FileName);

   /** Crea una capa vectorial en memoria de tipo correcto */
   void ExecuteNewGcpFile();
   /** Genera un archivo GCP con puntos autodetectados */
   void ExecuteAutoGenerateGcpFile();
   /** Abre un archivo de gcp existente e inicia la tarea de toma de puntos**/
   void ExecuteOpenGcpFile();
   /** Inicia la tarea de edicion vectorial con vector seleccionado */
   void ExecuteStartTask();
   /** Finaliza la tarea de edicion vectorial */
   void ExecuteEndTask();

   /** Finaliza la edicion grafica de features, no finaliza la tarea. */
   void ExecuteEndAddGcps() const;
   /**
    * Flag que indica si mostrar las opciones de configuracion para el
    * algoritmo de generacion de gcps
    */
   bool gcpAutoGenerateEnabled_;

   /** Tarea de corregistro */
   CoregisterTask* pCoregisterTask_;
   /** Comando asociado a la tarea de edicion vectorial */
   Command* pTaskCommand_;
   /** TaskConfig que permite configurar el Layout */
   suri::TaskConfig *pTaskConfig_;
};

} /* namespace suri */
#endif /* CORREGISTERTOOL_H_ */
