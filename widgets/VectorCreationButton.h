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

#ifndef VECTORCREATIONBUTTON_H_
#define VECTORCREATIONBUTTON_H_

// Includes Estandar
// Includes Suri
#include "VectorEditorButton.h"
#include "VectorEditionTaskEvent.h"
#include "VectorEditionTaskEvent.h"
#include "GeometryCreationEvent.h"
#include "VectorEditorButtonTool.h"
#include "suri/ObserverInterface.h"
#include "suri/StateChangeNotification.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class VectorDatasourceSelectionSource;
class VectorEditionTask;
class Vector;
class VectorDatasource;
class SimpleFeatureCreationEvent;


/*!
 * Clase que representa el boton que activa la creacion de nuevas capas
 * vectoriales haciendo uso del nuevo sistema de renderizacion vectorial
 */
class VectorCreationButton : public suri::VectorEditorButton {
   /** ctor copia **/
   VectorCreationButton(const VectorCreationButton&);

public:
   VectorCreationButton(wxWindow *pToolbar, ViewerWidget *pViewer,
                        const wxString &Bitmap,
                        GuiGeometryCreator::GeometryType GeoType,
                        wxString ButtonTooltip,
                        DataViewManager* pDataViewManager = NULL, int Flags =
                              BUTTON_NULL, bool CreationMode = true);
   virtual ~VectorCreationButton();
   /** Inicio de la edicion */
   void Start();
   /** Finaliza la edicion */
   void End();
   /** Indica si se encuentra activo el editor **/
   virtual bool IsActive() const;
   /**  Metodo que se llama cuando finaliza la edicion vectorial.*/
   virtual void OnVectorEditionFinished() {
      if (IsActive()) {
         DoSetValue(false);
         End();
         pDataViewManager_->GetDatasourceManager()->UnblockAllDatasource();
      }
   }
   /** Evento que habilita el boton segun el retorno de IsEnabled */
   virtual void OnUIUpdate(wxUpdateUIEvent &Event) {
      VectorEditorButton::OnUIUpdate(Event);
      if (taskended_ && pTask_) {
         OnVectorEditionFinished();
         StateChangeNotification notif("Deactivate", "null");
         Notify(&notif);
      }
   }

private:
   /** Setea el estado del boton. */
   virtual bool DoSetValue(bool State);
   /**
    * Crea un vector en memoria del tipo indicado.
    * @param[in] MemUrl url del vector temporal a crear
    * @param[in] NewVectorType tipo de vector a crear
    * @return puntero a vector en memoria
    */
   Vector* CreateMemoryVector(const std::string& MemUrl,
                              Vector::VectorType NewVectorType) const;
   /**
    * Inicia la tarea de edicion vectorial
    * @param[in] pDatasource datasource de trabajo
    * @return bool que indica si tuvo exito.
    */
   bool StartTask(VectorDatasource* pDatasource);
   /**
    * Abre una ventana de dialogo para seleccionar la ruta y nombre para guardar
    * un vector.
    * @param[out] Filename ruta y nombre del archivo a salvar.
    * @return false si el usuario cancelo
    */
   bool GetOutputFileName(std::string &Filename);

   /**
    * Copia el datasource en memoria a disco
    * @param[in] Filename nombre del shapefile en disco.
    * @param[in] Name nombre que se desea configurar a la fuente de datos
    */
   bool SaveMemoryDatasourceToDisk(const std::string &Filename,
                                   const std::string& Name = "");
   /**
    * Elimina un datasource vectorial y todos los archivos asociados.
    * @param[in] Filename url del shapefile que quiero eliminar
    * @return true si pudo borrar el datasource (todos los archivos asociados).
    * \todo este metodo deberia estar en vector.cpp
    */
   bool DeleteVectorDatasource(const std::string &Filename);
   /**
    * Analiza si se puede eliminar la fuente de datos vectorial y sus archivos asociados
    * Trata de abrir el datasource con el filename recibido, si no lo puede abrir
    * retorna false.
    * Recorre las capas, si en alguna no tiene permiso de escritura retorna false
    * @param[in] Filename nombre del archivo que se desea eliminar
    * @return informa si se puede eliminar archivo
    */
   bool CanDeleteVectorDatasource(const std::string &Filename);
   /**
    * Metodo auxiliar que guarda la fuente de datos en edicion
    * y la agrega a la lista
    */
   void SaveDatasource();
   /**
    * Metodo auxiliar que configura los eventos asociado a la tarea de edicion vectorial
    */
   void ConfigureTaskEvents();
   ViewportManagerInterface *pViewportManager_;
   FeatureSelection* pFeatureSelection_;
   GeometryCreationEvent* pCreationEventHandler_;
   VectorEditionTaskEvent* pPaintEventHandler_;
   VectorDatasourceSelectionSource* pSelectionSource_;
   DataViewManager* pDataViewManager_;
   VectorEditionTask* pTask_;
   Table* pTable_;
   /** Guarda referencia a vector en memoria mientras se edita. */
   Vector* pMemoryVector_;
   /** Fuente de datos que se esta creando*/
   VectorDatasource* pEditedDatasource_;
   GuiGeometryCreator::GeometryType geoType_;
   /** bool que indica que se activa el modo de creacion de nueva fuente de datos*/
   bool creationMode_;
   friend class SimpleFeatureCreationEvent;
   SimpleFeatureCreationEvent* pmiddleware_;
   bool taskended_;
};

} /** namespace suri */

#endif /* VECTORCREATIONBUTTON_H_ */
