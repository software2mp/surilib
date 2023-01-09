/*! \file VectorCreationButton.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "VectorCreationButton.h"
#include "suri/VectorEditionTask.h"
#include "suri/VectorEditionTable.h"
#include "suri/BufferedDriver.h"
#include "VectorEditionTaskEvent.h"
#include "suri/DataViewManager.h"
#include "VectorEditorButtonTool.h"
#include "GeometryCreationEvent.h"
#include "suri/Vector.h"
#include "VectorDatasource.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/StateChangeNotification.h"
#include "suri/ToolSupport.h"
#include "suri/RasterElement.h"
#include "suri/FileManagementFunctions.h"
#include "suri/Configuration.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Clase que se encarga de capturar el fin de edicion de una geometria
 *  y finalizar correctamente la tarea
 */
class SimpleFeatureCreationEvent : public ViewportEvent {
public:
   /** Ctor */
   SimpleFeatureCreationEvent(ViewportManagerInterface *pViewportManager,
                              ViewcontextManagerInterface *pViewcontextManager,
                              VectorCreationButton* pVectorCreationButton) :
         ViewportEvent(pViewportManager, pViewcontextManager),
         pVectorCreationButton_(pVectorCreationButton) {
   }
   /** Dtor */
   virtual ~SimpleFeatureCreationEvent() {

   }

   /** Limpia el estado de la herramienta, aborta edicion */
   virtual void Reset() {

   }
   /** Se ejecuta cuando el usuario hace click sobre el visualizador. */
   virtual void SetPoint(double Pixel, double Line, wxMouseEvent &Event) {
      /**
       * Si el usuario presiono el click derecho se deberia finaliza la edicion
       * de la capa vectorial
       */
      if (Event.RightUp()) {
         pVectorCreationButton_->taskended_ = true;
      }
   }
   /** Se ejecuta cada vez que el usuario realiza un desplazamiento del mouse */
   virtual void UpdatePoint(double Pixel, double Line, wxMouseEvent &Event) {

   }
   /** Al finalizar drag se actualiza geometria en vector */
   virtual void OnEndDrag(const Coordinates &InitialCoordinate,
                          const Coordinates &CurrentCoordinate, wxMouseEvent &Event) {

   }

private:
   /** Solo retorna true para los eventos que ocurren en visualizador de creacion */
   virtual bool CanProcessEvent(wxEvent &Event) const {
      return pVectorCreationButton_->IsActive();
   }

   VectorCreationButton* pVectorCreationButton_;
};

/** constructor **/
VectorCreationButton::VectorCreationButton(wxWindow *pToolbar, ViewerWidget *pViewer,
                                           const wxString &Bitmap,
                                           GuiGeometryCreator::GeometryType GeoType,
                                           wxString ButtonTooltip,
                                           DataViewManager* pDataViewManager, int Flags,
                                           bool CreationMode) :
      VectorEditorButton(pToolbar, pViewer, Bitmap, GeoType, ButtonTooltip,
                         pDataViewManager->GetViewportManager(),
                         pDataViewManager->GetViewcontextManager(), Flags),
      pFeatureSelection_(pDataViewManager->GetFeatureSelection()),
      pCreationEventHandler_(NULL), pPaintEventHandler_(NULL), pSelectionSource_(NULL),
      pDataViewManager_(pDataViewManager), pTask_(NULL), pTable_(NULL),
      pMemoryVector_(NULL), pEditedDatasource_(NULL), geoType_(GeoType),
      creationMode_(CreationMode), taskended_(false) {
   pDataViewManager_->GetTaskManager()->AppendTask(pTask_);
}

/** Destructor **/
VectorCreationButton::~VectorCreationButton() {
}

/**
 * Metodo auxiliar que configura los eventos asociado a la tarea de edicion vectorial
 *
 */
void VectorCreationButton::ConfigureTaskEvents() {
   RasterElement* praster = ToolSupport::GetActiveRaster(
         pDataViewManager_->GetViewcontextManager(),
         pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontextId());
   DatasourceInterface* prasterds =
         pDataViewManager_->GetDatasourceManager()->GetDatasourceByElementId(
               praster->GetUid());
   pDataViewManager_->GetDatasourceManager()->BlockDatasource(prasterds->GetId());
   pDataViewManager_->GetTaskManager()->AppendTask(pTask_);
   DatasourceInterface* pds = VectorEditorButtonTool::GetSelectedDataSource(
         pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext(),
         pDataViewManager_);
   pDataViewManager_->GetDatasourceManager()->BlockDatasource(pds->GetId());
   pCreationEventHandler_ = new GeometryCreationEvent(
         pDataViewManager_->GetViewportManager(),
         pDataViewManager_->GetViewcontextManager(), pTask_, true);
   pmiddleware_ = new SimpleFeatureCreationEvent(
         pDataViewManager_->GetViewportManager(),
         pDataViewManager_->GetViewcontextManager(), this);
   pViewer_->PushMouseEvent(pmiddleware_);
   pViewer_->PushPaintEvent(pCreationEventHandler_);
   pViewer_->PushMouseEvent(pCreationEventHandler_);
   pPaintEventHandler_ = new VectorEditionTaskEvent(
         pDataViewManager_->GetViewportManager(),
         pDataViewManager_->GetViewcontextManager(), pTask_);
   pViewer_->PushPaintEvent(pPaintEventHandler_);
   pViewer_->PushMouseEvent(pPaintEventHandler_);
}

/** Inicio de la edicion */
void VectorCreationButton::Start() {
   ViewcontextInterface* pviewcontext =
         pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();
   if (!pviewcontext)
      return;

   if (IsActive())
      return;

   if (!pDataViewManager_->GetTaskManager()->GetActiveTaskIds().empty())
      return;

   if (!pTask_) {
      pTask_ = new VectorEditionTask;
   }
   taskended_ = false;
   pTask_->SetFeatureSelector(pFeatureSelection_);
   pTask_->SetViewportManager(pDataViewManager_->GetViewportManager());

   if (creationMode_) {
      Vector::VectorType type = Vector::Polygon;
      switch (geoType_) {
         case GuiGeometryCreator::Polygon:
            type = Vector::Polygon;
            break;
         case GuiGeometryCreator::Line:
            type = Vector::Line;
            break;
         case GuiGeometryCreator::Point:
            type = Vector::Point;
            break;
         default:
            type = Vector::Polygon;
            break;
      }
      static int vectornumber = 0;
      ViewerWidget *pviewer =
            AS_VW(pDataViewManager_->GetViewportManager()->GetSelectedViewport());
      std::string ftemp = "shpmemory:NewVector" + NumberToString(vectornumber);
      std::string memoryvectorurl = ftemp + ":"
            + pviewer->GetWorld()->GetSpatialReference();
      ++vectornumber;
      pMemoryVector_ = CreateMemoryVector(memoryvectorurl, type);
      if (!pMemoryVector_) {
         return;
      }
      pEditedDatasource_ = VectorDatasource::Create(ftemp);
   } else {
      pEditedDatasource_ =
            dynamic_cast<VectorDatasource*>(VectorEditorButtonTool::GetSelectedDataSource(
                  pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext(),
                  pDataViewManager_));
      if (!pEditedDatasource_) {
         SHOW_ERROR("Error al intentar obtener el vector desde la fuente de datos.");
         End();
         return;
      }
      pMemoryVector_ = NULL;
      /** Se bloquea la fuente de datos en edicion **/
      pDataViewManager_->GetDatasourceManager()->BlockDatasource(
            pEditedDatasource_->GetId());
   }
   if (StartTask(pEditedDatasource_)) {
      ConfigureTaskEvents();
      StateChangeNotification notif("Activate", "null");
      Notify(&notif);
   }
}

/**
 * Metodo auxiliar que guarda la fuente de datos en edicion
 * y la agrega a la lista
 */
void VectorCreationButton::SaveDatasource() {
   if (pMemoryVector_ && creationMode_) {
      // Trata de guardarlo a disco
      std::string filename;
      if (GetOutputFileName(filename)) {
         SaveMemoryDatasourceToDisk(filename);
      } else if (pEditedDatasource_) {
         // se utiliza la fuente de datos temporal
         std::string geomtype =
               dynamic_cast<VectorElement*>(pEditedDatasource_->GetElement())->GetStringType();
         pEditedDatasource_->SetName(geomtype);
         std::string pathtempaux = suri::Configuration::GetParameter(
               "app_vector_base_dir_volatile", wxT(""));
         pathtempaux = suri::GetNextPath(pathtempaux);
         std::string ftemp = wxFileName::CreateTempFileName(pathtempaux).c_str();
         ftemp += ".shp";
         SaveMemoryDatasourceToDisk(ftemp, geomtype);
      }
      delete pEditedDatasource_;
      pEditedDatasource_ = NULL;
      Vector::Close(pMemoryVector_);
   } else if (pEditedDatasource_) {
      /** actualiza la renderizacion de la capa para limpiar lo que quedo de la edicion
       cuando se edita una capa ya existente **/
      ViewcontextInterface *pactivevc =
            pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();
      LayerInterface* player = pactivevc->GetAssociatedLayer(
            pEditedDatasource_->GetId());
      player->GetElement()->SetChanged();
      player->GetElement()->SendViewerUpdate();
   }
}

/** Finaliza la edicion */
void VectorCreationButton::End() {
   if (!IsActive())
      return;
   if (!pTask_ || (!pTask_->IsActive()))
      return;
   if (!pTask_->End())
      SHOW_ERROR(_(message_CANT_FINISH_TASK));
   pDataViewManager_->GetTaskManager()->DeleteTask(pTask_->GetId());
   pTask_ = NULL;

   if(!pPaintEventHandler_->GetClosedWithoutEdition())
      SaveDatasource();
   pViewer_->PopPaintEvent(pPaintEventHandler_);
   pViewer_->PopMouseEvent(pPaintEventHandler_);
   pViewer_->PopPaintEvent(pCreationEventHandler_);
   pViewer_->PopMouseEvent(pCreationEventHandler_);
   delete pCreationEventHandler_;
   pCreationEventHandler_ = NULL;
   delete pPaintEventHandler_;
   pPaintEventHandler_ = NULL;
   ViewcontextInterface* pviewcontext =
         pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();
   if (pviewcontext) {
      pviewcontext->GetLayerList()->SetChanged();
      pviewcontext->GetLayerList()->SendViewerUpdate();
   }
}

bool VectorCreationButton::DoSetValue(bool State) {
   if (State) {
      DatasourceInterface* pds = VectorEditorButtonTool::GetSelectedDataSource(
            pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext(),
            pDataViewManager_);
      if (pds == NULL) {
         SHOW_ERROR("Debe seleccionar una imagen o un mapa para poder iniciar la tarea.");
         return false;
      }
      Start();
   }
   return ToggleButton::DoSetValue(State);
}

/** Indica si se encuentra activo el editor **/
bool VectorCreationButton::IsActive() const {
   return pCreationEventHandler_ != NULL && pPaintEventHandler_ != NULL;
}

/**
 * Crea un vector en memoria del tipo indicado.
 * @param[in] MemUrl url del vector temporal a crear
 * @param[in] NewVectorType tipo de vector a crear
 * @return puntero a vector en memoria
 */
Vector* VectorCreationButton::CreateMemoryVector(
      const std::string& MemUrl, Vector::VectorType NewVectorType) const {
   Vector* pmemoryvector = NULL;
   ViewerWidget *pviewer =
   AS_VW(pDataViewManager_->GetViewportManager()->GetSelectedViewport());
   pmemoryvector = Vector::Open(MemUrl, Vector::ReadWrite, 0, NewVectorType);
   if (pmemoryvector) {
      pmemoryvector->CreateLayer("NewLayer", pviewer->GetWorld()->GetSpatialReference(),
                                 NewVectorType);
   }
   return pmemoryvector;
}

/**
 * Inicia la tarea de edicion vectorial
 * @param[in] pDatasource datasource de trabajo
 * @return bool que indica si tuvo exito.
 */
bool VectorCreationButton::StartTask(VectorDatasource* pDatasource) {
   // config tabla
   if (!pDatasource || !VectorEditorButtonTool::ConfigureTable(pDatasource, &pTable_)) {
      SHOW_ERROR("Error al intentar configurar la tabla del shapefile.");
      return false;
   }

   // config tarea
   if (!VectorEditorButtonTool::ConfigureTask(pTask_, pDatasource, pDataViewManager_,
                                              pFeatureSelection_, &pTable_)) {
      SHOW_ERROR("Error al intentar configurar la tarea.");
      return false;
   }

   return pTask_->Start();
}

/**
 * Abre una ventana de dialogo para seleccionar la ruta y nombre para guardar
 * un vector.
 * @param[out] Filename ruta y nombre del archivo a salvar.
 * @return false si el usuario cancelo
 */
bool VectorCreationButton::GetOutputFileName(std::string &Filename) {
#ifdef __FILENAME_SELECTION_ENABLE__
   wxFileDialog dlg(NULL, _(caption_SAVE_VECTOR), wxT(""), wxT(""),
         filter_SAVE_VECTOR, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

   if (dlg.ShowModal() == wxID_OK) {
      Filename = dlg.GetPath();
      return true;
   }
#endif
   return false;
}

/**
 * Copia el datasource en memoria a disco
 * @param[in] Filename nombre del shapefile en disco.
 * @param[in] Name nombre que se desea configurar a la fuente de datos
 */
bool VectorCreationButton::SaveMemoryDatasourceToDisk(const std::string &Filename,
                                                      const std::string& Name) {
   bool result = true;
   if (wxFileName::FileExists(Filename)) {
      if (!CanDeleteVectorDatasource(Filename)) {
         SHOW_ERROR(message_CANT_DELETE_VECTOR);
         return false;
      }

      // Prueba eliminarlo de la lista, por si ademas de existir esta abierto
      // Crea un elemento para obtener el UID
      Element *pelement = VectorElement::Create(Filename);
      if (pelement) {
         Element::UidType uid = pelement->GetUid();
         Element::Destroy(pelement);
         pDataViewManager_->GetDatasourceManager()->RemoveDatasourceByElementId(uid);
      }
      if (!DeleteVectorDatasource(Filename)) {
         SHOW_ERROR(message_VECTOR_DELETION_ERROR);
         return false;
      }
   }

   if (!Filename.empty() && !wxFileName::FileExists(Filename)
         && pMemoryVector_->Save(Filename, "ESRI Shapefile")) {
      DatasourceInterface* pdatasource = suri::DatasourceInterface::Create(
            "VectorDatasource", Filename);
      if (pdatasource) {
         if (!Name.empty())
            pdatasource->SetName(Name);
         suri::SuriObject::UuidType viewcontextid =
               pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontextId();
         DatasourceManagerInterface* pdsmgr = pDataViewManager_->GetDatasourceManager();
         std::vector<SuriObject::UuidType> ids = pdsmgr->GetIds();                                  
         int lastvectorpos = -1;                                                                    
         for (size_t id = 0; id < ids.size(); ++id) {
            DatasourceInterface* pds = pdsmgr->GetDatasource(ids[id]);
            lastvectorpos = pds && pds->GetClassId().compare("VectorDatasource") == 0 ? 
                           id : lastvectorpos;
         }                                                                 
         result = pdsmgr->AddDatasourceWithNotification(pdatasource, viewcontextid);

         if (result) {
            if (lastvectorpos >= 0 && lastvectorpos >= static_cast<int>((ids.size() - 1))) {
               // si no es el ultimo lo mueve luego del ultimo vector
               pdsmgr->MoveObjectToEnd(pdatasource->GetId());
            } else if (lastvectorpos >= 0) {
              if (Configuration::GetParameter("app_short_name", "") != wxT("SoPI")) {
                  pdsmgr->MoveObjectToEnd(pdatasource->GetId());
              } else {
                  pdsmgr->MoveObject(pdatasource->GetId(), ids[lastvectorpos+1]);
              }
            }
            ViewcontextInterface *pactivevc =
                  pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();
            LayerInterface* player = pactivevc->GetAssociatedLayer(
                  pdatasource->GetId());
            player->GetElement()->Activate(true);
         }
      }
   } else {
      SHOW_ERROR("Fallo al guardar el vector");
      return false;
   }
   return result;
}

/**
 * Elimina un datasource vectorial y todos los archivos asociados.
 * @param[in] Filename url del shapefile que quiero eliminar
 * @return true si pudo borrar el datasource (todos los archivos asociados).
 * \todo este metodo deberia estar en vector.cpp
 */
bool VectorCreationButton::DeleteVectorDatasource(const std::string &Filename) {
   /** Elimino shapefile */
   if (!CanDeleteVectorDatasource(Filename)) {
      return false;
   }

   OGRDataSource *pdatasource = OGRSFDriverRegistrar::Open(Filename.c_str());
   if (!pdatasource) {
      return false;
   }
   OGRSFDriver *pdriver = pdatasource->GetDriver();
   OGRDataSource::DestroyDataSource(pdatasource);

   /** Elimino archivo */
   if (pdriver->DeleteDataSource(Filename.c_str()) != OGRERR_NONE) {
      return false;
   }

   return true;
}

/**
 * Analiza si se puede eliminar la fuente de datos vectorial y sus archivos asociados
 * Trata de abrir el datasource con el filename recibido, si no lo puede abrir
 * retorna false.
 * Recorre las capas, si en alguna no tiene permiso de escritura retorna false
 * @param[in] Filename nombre del archivo que se desea eliminar
 * @return informa si se puede eliminar archivo
 */
bool VectorCreationButton::CanDeleteVectorDatasource(const std::string &Filename) {
   // Verifico que sea un vector valido
   OGRDataSource *pdatasource = OGRSFDriverRegistrar::Open(Filename.c_str());
   if (!pdatasource) {
      return false;
   }

   OGRSFDriver *pdriver = pdatasource->GetDriver();
   if (!pdriver || !pdriver->TestCapability(ODrCDeleteDataSource)) {
      return false;
   }

   OGRDataSource::DestroyDataSource(pdatasource);

   // Verifico que el elemento no este abierto
   VectorEditor vectoreditor;
   vectoreditor.OpenVector(Filename);
   int layerindex = 0;
   while (vectoreditor.OpenLayer(layerindex)) {
      if (!vectoreditor.CanWrite()) {
         vectoreditor.CloseVector();
         return false;
      }
      layerindex++;
   }
   vectoreditor.CloseVector();

   return true;
}
} /** namespace suri */
