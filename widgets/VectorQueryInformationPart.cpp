/*! \file VectorQueryInformationPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <string>

// Includes Suri
#include "VectorQueryInformationPart.h"
#include "LayerTablePart.h"

#include "TablePartConfigurator.h"
#include "TablePart.h"
#include "suri/DatasourceInterface.h"
#include "suri/Part.h"
#include "DefaultTable.h"
#include "suri/BufferedDriver.h"
#include "suri/World.h"

#include "suri/DatasourceManagerInterface.h"
#include "SuriObjectGroupManager.h"
#include "suri/ViewcontextManagerInterface.h"
#include "suri/ViewcontextTreeSelectionManager.h"
#include "suri/LibraryManager.h"
#include "suri/TaskManager.h"
#include "suri/FeatureSelection.h"
#include "suri/DataViewManager.h"
#include "suri/LibraryManagerFactory.h"
#include "DefaultDatasourceManager.h"
#include "DefaultViewcontextManager.h"
#include "suri/VectorEditionTable.h"
#include "MemoryVector.h"
#include "suri/Vector.h"

namespace suri {


/** Ctor */
VectorQueryInformationPart::VectorQueryInformationPart(DatasourceInterface *pDatasource):
      pDatasource_(pDatasource), pLayerTablePart_(NULL) {
}

/** Dtor */
VectorQueryInformationPart::~VectorQueryInformationPart() {
}

bool VectorQueryInformationPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                      wxT("ID_VECTOR_QUERY_INFORMATION_PANEL"));

   // Preparo los datos en memory para que los pueda usar el driver.
   Vector* pmemoryvector = Vector::Open(GetMemoryUrl(pDatasource_->GetUrl()), Vector::ReadWrite);
   Vector* pdiskvector = Vector::Open(pDatasource_->GetUrl());
   if (pmemoryvector == NULL || pdiskvector == NULL) {
      REPORT_DEBUG("DEBUG: Fallo al crear vector en disco y/o memoria");
      return false;
   }
   pdiskvector->Copy(pmemoryvector);
   Vector::Close(pdiskvector);

   // Actualizo el nombre del datasource para que lo pueda usar el driver.
   pDatasource_->SetUrl(GetMemoryUrl(pDatasource_->GetUrl()));

   // Preparo la tabla con la configuracion y el driver.
   windowTitle_ = pDatasource_->GetName();
   suri::TablePartConfigurator tableconf;
   suri::Option opt = suri::TablePartConfigurator::GetVectorTableEditorConfig();

   DefaultTable* ptable = new DefaultTable();
   VectorEditorDriver* pdriver = new VectorEditorDriver(pDatasource_);
   BufferedDriver* pbuffer = new BufferedDriver(pdriver);
   ptable->SetDriver(pbuffer);
   TablePart* ptablepart = tableconf.Initialize(GetDataView(), ptable, opt);

   AddControl(ptablepart, wxT("ID_VECTOR_TABLE_PANEL"));

   Vector::Close(pmemoryvector);

   return true;
}

/** Indica si el Part tiene cambios para salvar. */
bool VectorQueryInformationPart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool VectorQueryInformationPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool VectorQueryInformationPart::RollbackChanges() {
   return true;
}

/** Inicializa el Part */
void VectorQueryInformationPart::SetInitialValues() {
}

/** Actualiza el estado del Part */
void VectorQueryInformationPart::Update() {
}

/**
 * Obtiene un dataviewmanager para el table part utilizando
 * componentes con valores por defecto.
 *
 * @return punto a un nuevo DataViewManager.
 *
 * \todo (danieltesta-#3270): verificar quien tiene la responsabilidad
 * de liberar la memoria del dataviewmanager.
 */
DataViewManager* VectorQueryInformationPart::GetDataView() {
   suri::DatasourceManagerInterface* pdm = new suri::DefaultDatasourceManager();
   suri::SuriObjectGroupManager* pgroupmanager = new suri::SuriObjectGroupManager(pdm);
   suri::ViewcontextManagerInterface* pvm = new suri::DefaultViewcontextManager(pdm);
   suri::ViewcontextTreeSelectionManager* pselectionmanager =
         new suri::ViewcontextTreeSelectionManager(pgroupmanager, pvm);
   suri::LibraryManager* plibrarymanager =
                     suri::LibraryManagerFactory::CreateLibraryManager();
   suri::TaskManager* ptaskmanager = new suri::TaskManager();
   suri::FeatureSelection* pfeatureselection = new suri::FeatureSelection();

   DataViewManager* pmanager = new suri::DataViewManager(pdm, pvm, NULL,
                                                         pgroupmanager,
                                                         pselectionmanager,
                                                         plibrarymanager, ptaskmanager,
                                                         pfeatureselection);

   return pmanager;
}

/**
 * Obtiene el nombre del vector en memoria.
 *
 * @param[in] FileNameP Nombre para usar como base del nombre del archivo en memoria.
 * @return Nombre del archivo en memoria.
 */
std::string VectorQueryInformationPart::GetMemoryUrl(const std::string& FileNameP) {
   return MEMORY + "layertablepart_" + GET_NAME(FileNameP);
}

/**
 * Retorna un bitmap con el icono de la ventana. Si se pasa solo X o solo Y el otro
 * parametro tomara un valor que conserve la relacion entre lados con la imagen. Al
 * final de ser necesario hace Rescale.
 *
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho del icono
 * @param[in] Y alto del icono
 */
void VectorQueryInformationPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   if (X < 0 && Y < 0) {
      ToolBitmap = wxArtProvider::GetBitmap(wxART_MISSING_IMAGE);
      return;
   }
   GET_BITMAP_RESOURCE(pDatasource_->GetElement()->GetIcon().c_str(), ToolBitmap);
   ESCALE_BITMAP(ToolBitmap, X, Y);
}

}  // namespace suri