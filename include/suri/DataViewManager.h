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
