/*! \file DataViewManager.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar

// Includes Suri
#include "suri/DataViewManager.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
DataViewManager::DataViewManager(DatasourceManagerInterface* pDatasourceManager,
                                 ViewcontextManagerInterface* pViewcontextManager,
                                 ViewportManagerInterface* pViewportManager,
                                 GroupManagerInterface* pGroupManager,
                                 TreeSelectionManagerInterface* pSelectionManager,
                                 LibraryManager* pLibraryManager,
                                 TaskManager* pTaskManager,
                                 FeatureSelection* pFeatureSelection) :
      pDatasourceManager_(pDatasourceManager), pViewcontextManager_(pViewcontextManager),
      pViewportManager_(pViewportManager), pGroupManager_(pGroupManager),
      pSelectionManager_(pSelectionManager), pLibraryManager_(pLibraryManager),
      pTaskManager_(pTaskManager), pFeatureSelection_(pFeatureSelection) {
   // Se pone al administrador de bibliotecas a escuchar las notificaciones
   // del administrador de fuente de datos
   if (pDatasourceManager_ && pLibraryManager_)
      pDatasourceManager_->Attach(pLibraryManager_);
}

/**
 * Destructor
 */
DataViewManager::~DataViewManager() {
}

/**
 * Getter de DatasourceManager
 * @return Devuelve el puntero al DatasourceManager
 */
DatasourceManagerInterface* DataViewManager::GetDatasourceManager() const {
   return pDatasourceManager_;
}

/**
 * Getter de ViewcontextManager
 * @return Devuelve el puntero al ViewcontextManager
 */
ViewcontextManagerInterface* DataViewManager::GetViewcontextManager() const {
   return pViewcontextManager_;
}

/**
 * Getter de ViewportManager
 * @return Devuelve el puntero al ViewportManager
 */
ViewportManagerInterface* DataViewManager::GetViewportManager() const {
   return pViewportManager_;
}

/**
 * Getter de GroupManager
 * @return Devuelve el puntero al GroupManager
 */
GroupManagerInterface* DataViewManager::GetGroupManager() const {
   return pGroupManager_;
}

/**
 * Getter de TreeSelectionManagerInterface
 * @return puntero al TreeSelectionManager
 */
TreeSelectionManagerInterface* DataViewManager::GetSelectionManager() const {
   return pSelectionManager_;
}

/**
 * Getter de LibraryManager
 * @return puntero al LibraryManager
 */
LibraryManager* DataViewManager::GetLibraryManager() const {
   return pLibraryManager_;
}

/** Getter de TaskManager */
TaskManager* DataViewManager::GetTaskManager() {
   return pTaskManager_;
}

/** Getter de FeatureSelection */
FeatureSelection* DataViewManager::GetFeatureSelection() {
   return pFeatureSelection_;
}

/**
 * Setter de DatasourceManager
 * @param[in]  pDatasourceManager: Puntero al DatasourceManager
 */
void DataViewManager::SetDatasourceManager(DatasourceManagerInterface* pDatasourceManager) {
   pDatasourceManager_ = pDatasourceManager;
}

/**
 * Setter de ViewcontextManager
 * @param[in]  pViewcontextManager: Puntero al ViewcontextManager
 */
void DataViewManager::SetViewcontextManager(ViewcontextManagerInterface* pViewcontextManager) {
   pViewcontextManager_ = pViewcontextManager;
}

/**
 * Setter de ViewportManager
 * @param[in]  pViewportManager: Puntero al ViewportManager
 */
void DataViewManager::SetViewportManager(ViewportManagerInterface* pViewportManager) {
   pViewportManager_ = pViewportManager;
}

/**
 * Setter de GroupManager
 * @param[in] pGroupManager: Puntero al GroupManager
 */
void DataViewManager::SetGroupManager(GroupManagerInterface* pGroupManager) {
   pGroupManager_ = pGroupManager;
}

/**
 * Setter de GroupManager
 * @param[in] pSelectionManager: Puntero al SelectionManager
 */
void DataViewManager::SetSelectionManager(TreeSelectionManagerInterface* pSelectionManager) {
   pSelectionManager_ = pSelectionManager;
}

/**
 * Setter de LibraryManager
 * @param[in] pLibraryManager: Puntero al LibraryManager
 */
void DataViewManager::SetLibraryManager(LibraryManager* pLibraryManager) {
   pLibraryManager_ = pLibraryManager;
}

/** Setter de TaskManager */
void DataViewManager::SetLibraryManager(TaskManager* pTaskManager) {
   pTaskManager_ = pTaskManager;
}

/** Setter de TaskManager */
void DataViewManager::SetFeatureSelection(FeatureSelection* pFeatureSelection) {
   pFeatureSelection_ = pFeatureSelection;
}

} /** namespace suri */
