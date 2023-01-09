/*! \file DataViewManager.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DATAVIEWMANAGER_H_
#define DATAVIEWMANAGER_H_

// Includes Estandar

// Includes Suri
#include "DatasourceManagerInterface.h"
#include "ViewportManagerInterface.h"
#include "ViewcontextManagerInterface.h"
#include "GroupManagerInterface.h"
#include "TreeSelectionManagerInterface.h"
#include "LibraryManager.h"
#include "TaskManager.h"
#include "FeatureSelection.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * DataViewManager encapsula DatasourceManager, ViewportManager, 
 * ViewcontextManager y GroupManager.
 */
class DataViewManager {
public:
   /** Constructor */
   DataViewManager(DatasourceManagerInterface* pDatasourceManager,
                   ViewcontextManagerInterface* pViewcontextManager,
                   ViewportManagerInterface* pViewportManager,
                   GroupManagerInterface* pGroupManager,
                   TreeSelectionManagerInterface* pSelectionManager,
                   LibraryManager* pLibraryManager,
                   TaskManager* pTaskManager,
                   FeatureSelection* pFeatureSelection);
   /** Destructor */
   virtual ~DataViewManager();
   /** Getter de DatasourceManager */
   DatasourceManagerInterface* GetDatasourceManager() const;
   /** Getter de ViewcontextManager */
   ViewcontextManagerInterface* GetViewcontextManager() const;
   /** Getter de ViewportManager */
   ViewportManagerInterface* GetViewportManager() const;
   /** Getter de GroupManager */
   GroupManagerInterface* GetGroupManager() const;
   /** Getter de TreeSelectionManagerInterface */
   TreeSelectionManagerInterface* GetSelectionManager() const;
   /** Getter de LibraryManager */
   LibraryManager* GetLibraryManager() const;
   /** Getter de TaskManager */
   TaskManager* GetTaskManager();
   /** Getter de FeatureSelection */
   FeatureSelection* GetFeatureSelection();

private:
   /** Setter de DatasourceManager */
   void SetDatasourceManager(DatasourceManagerInterface* pDatasourceManager);
   /** Setter de ViewcontextManager */
   void SetViewcontextManager(ViewcontextManagerInterface* pViewcontextManager);
   /** Setter de ViewportManager */
   void SetViewportManager(ViewportManagerInterface* pViewportManager);
   /** Setter de GroupManager */
   void SetGroupManager(GroupManagerInterface* pGroupManager);
   /** Setter de TreeSelectionManagerInterface */
   void SetSelectionManager(TreeSelectionManagerInterface* pGroupManager);
   /** Setter de LibraryManager */
   void SetLibraryManager(LibraryManager* pLibraryManager);
   /** Setter de TaskManager */
   void SetLibraryManager(TaskManager* pTaskManager);
   /** Setter de TaskManager */
   void SetFeatureSelection(FeatureSelection* pFeatureSelection);

   DatasourceManagerInterface* pDatasourceManager_;   /*! Puntero a DatasourceManager */
   ViewcontextManagerInterface* pViewcontextManager_; /*! Puntero a ViewcontextManager */
   ViewportManagerInterface* pViewportManager_;       /*! Puntero a ViewportManager */
   GroupManagerInterface* pGroupManager_;             /*! Puntero a GroupManager */
   TreeSelectionManagerInterface* pSelectionManager_; /*! Puntero a TreeSelectionManager */
   LibraryManager* pLibraryManager_;                  /*! Puntero a LibraryManager */
   TaskManager* pTaskManager_;                        /*! Puntero a TaskManager */
   FeatureSelection* pFeatureSelection_;              /*! Puntero a FeatureSelection */
};

} /** namespace suri */

#endif /* DATAVIEWMANAGER_H_ */
