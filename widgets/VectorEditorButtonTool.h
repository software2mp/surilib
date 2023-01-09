/*! \file VectorEditorButtonTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTOREDITORBUTTONTOOL_H_
#define VECTOREDITORBUTTONTOOL_H_


// Includes Estandar
// Includes Suri
#include "suri/VectorEditionTask.h"
#include "suri/Table.h"
#include "suri/ViewcontextInterface.h"
#include "suri/LayerList.h"

// Includes Wx
// Defines
// forwards

namespace suri {
// forwards
class DataViewManager;
class FeatureSelection;
class DatasourceInterface;

/** Clase auxiliar para la configuracion de elementos a utilizar en los botones
 *  de edicion vectorial **/
class VectorEditorButtonTool {
public:
   /** Destructor */
   virtual ~VectorEditorButtonTool();
   static DatasourceInterface* GetSelectedDataSource(ViewcontextInterface* pViewContext,
                                                     DataViewManager* pDataViewManager);
   /** Configura la tabla para la tarea ( Task_ ) */
   static bool ConfigureTable(VectorDatasource* pDataSource, Table** pTable);
   /** Configura tarea */
   static bool ConfigureTask(VectorEditionTask* pVectorEditionTask,
                             VectorDatasource* pDataSource,
                             DataViewManager* pDataViewManager,
                             FeatureSelection* pFeatureSelection, Table** pTable);
   /** Se bloquean las fuentes activas */
   static void BlockActivatedDatasources(ViewcontextInterface* pViewContext,
                                       DataViewManager* pDataViewManager);
   /** Se desbloquean las fuentes activas */
   static void UnBlockActivatedDatasources(DataViewManager* pDataViewManager);

private:
   /** Ctor */
   VectorEditorButtonTool();
};
} // end namespace
#endif /* VECTOREDITORBUTTONTOOL_H_ */

