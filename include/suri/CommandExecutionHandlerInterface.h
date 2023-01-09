/*! \file CommandExecutionHandlerInterface.h */                                                                          
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
 */    

#ifndef __COMMAND_EXEC_HANLDER_INTERFCE_H_
#define __COMMAND_EXEC_HANLDER_INTERFCE_H_

#include <string>

namespace suri {

// forwards
class Command;
class DataViewManager;
class GenericTool;
class ProcessInterface;
class RasterElement;
class Subset;

namespace core {


/** 
 * Interfaz que define el comportamiento esperado de una clase encargada de administrar 
 * la ejecucion de un comando que posee una herramienta. 
 **/
class CommandExecutionHandlerInterface {
   /** Constructor copia **/
	CommandExecutionHandlerInterface(const CommandExecutionHandlerInterface&);

	public:
		/** Constructor **/
		CommandExecutionHandlerInterface() {}
		/** Destructor **/
		virtual ~CommandExecutionHandlerInterface() {}
		/**
		 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
		 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
       * @return true en caso de poder manejar la ejecucion del comando
       * @return false en caso contrario
       */
		virtual bool Execute(const Command* pCommand, GenericTool* pTool)=0;
	protected:
		/** Funcion auxiliar que una vez instanciado el proyecto configurada la capa de adaptacion
		 *  y corre la ventana de configuracion del mismo para que luego corra el proceso
		 *  @param[in] pDataView administrador de informacion de la aplicacion
		 *  @param[in] Caption nombre que tendra la vetana de configuracion del proceso
		 *  @param[in] Instancia del proceso que se correra una vez configurado
		 *  @param[in] Elemento de entrada al proceso
		 **/
		void RunProcess(DataViewManager* pDataView, const std::string& Caption, ProcessInterface* pProcess);
		/** Funcion auxiliar para obtener los parametros basicos de entrada a los procesos
		 *  que operan sobre las bandas de un raster (el elemento de entrada y el subset)
		 **/
		void LoadInputProcessParameters(DataViewManager* pDataView, RasterElement* &pRaster, Subset& ViewerWindow);
};

}  /** namespace core **/
}  /** namespace suri **/

#endif
