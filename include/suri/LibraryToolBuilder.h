/*! \file LibraryToolBuilder.h */                                                                          
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
 */    

#ifndef __LIBRARY_TOOL_BUILDER_H__
#define __LIBRARY_TOOL_BUILDER_H__

// Include Estandard
#include <string>

// Includes suri 
#include "suri/ToolBuilderInterface.h"

namespace suri {

class ToolInterface;
class DataViewManager;

namespace core {

/** 
 * Clase encarga de realizar la creacion y configuracion
 * de la herramientas que operan aplicando operaciones sobre las bandas de un raster
 */
class LibraryToolBuilder : public ToolBuilderInterface {
	/** ctor copia **/
	LibraryToolBuilder(const LibraryToolBuilder&);
	
public:
	/** constructor **/
	LibraryToolBuilder();
	/** destructor **/
	~LibraryToolBuilder();
	/** Metodo que crea y configura una herramienta con los hanlders para la ejecucion
     * (CommandExecutionHandlers)
	 * y las acciones correspondientes (Commands). **/
	virtual ToolInterface* BuildTool(DataViewManager* pDataViewManager);
};
}  /** namespace core **/
}  /** namespace suri **/

#endif
