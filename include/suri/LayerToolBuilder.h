/*! \file LayerToolBuilder.h */                                                                          
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
 */    

#ifndef __LAYER_TOOL_BUILDER_H__
#define __LAYER_TOOL_BUILDER_H__

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
 * de la herramienta que acciona contra capas
 */
class LayerToolBuilder : public ToolBuilderInterface {
	/** ctor copia **/
	LayerToolBuilder(const LayerToolBuilder&);
	
public:
	/** constructor **/
	LayerToolBuilder();
	/** destructor **/
	~LayerToolBuilder();
	/** Metodo que crea y configura una herramienta con los hanlders para la ejecucion
     * (CommandExecutionHandlers)
	 * y las acciones correspondientes (Commands). **/
	virtual ToolInterface* BuildTool(DataViewManager* pDataViewManager);
};
}  /** namespace core **/
}  /** namespace suri **/

#endif
