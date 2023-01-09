/*! \file ToolBuilderInterface.h */                                                                          
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
 */    

#ifndef __TOOL_BUILDER_INTERFACE_H__
#define __TOOL_BUILDER_INTERFACE_H__
 
namespace suri {

// forwards
class ToolInterface;
class DataViewManager;

namespace core {

/** Interfaz que define el comportamiento de un configurador de herramientas. 
 * El objetivo de este tipo de clases es el de encapsular, 
 *  simplificar la creacion y configuracion de las herramientas. 
 **/ 
class ToolBuilderInterface {
		/** ctor copia **/
		ToolBuilderInterface(const ToolBuilderInterface&);

	public:
		/** constructor **/
		ToolBuilderInterface() {}
		/** destructor **/
		virtual ~ToolBuilderInterface() {}
		/** Metodo que crea y configura una herramienta con los hanlders para la ejecucion
        * (CommandExecutionHandlers)
		  * y las acciones correspondientes (Commands). **/
		virtual ToolInterface* BuildTool(DataViewManager* pDataView)=0;		
};

}  /** namespace core **/
}  /** namespace suri **/

#endif
