/*! \file AddLayerCommandExecutionHandler.h */

/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib. 
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L. 
 *                                                                                                  
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/) 
 *  
 */


#ifndef __ADD_LAYER_COMMAND_EXEC_HANDLER_H__
#define __ADD_LAYER_COMMAND_EXEC_HANDLER_H__

// Includes Estandard
#include <string>

// Includes Suri
#include "suri/CommandExecutionHandlerInterface.h"

namespace suri {

// forwards
class DataViewManager;

namespace core {


/** Clase encargada de administrar la ejecucion del comando AddLayer **/
class AddLayerCommandExecutionHandler : public CommandExecutionHandlerInterface {

	/** constructor copia **/
	AddLayerCommandExecutionHandler(const AddLayerCommandExecutionHandler&);

public:
   typedef enum  {
      NoGroup = -1, PointGroup = 0, LineGroup = 1, PolygonGroup = 2,
      RasterGroup = 3, GeometryCollectionGroup = 4
   } LayerGroup;
   /** Constructor **/
   AddLayerCommandExecutionHandler(DataViewManager* pDataView);
   /** Destructor **/
   virtual ~AddLayerCommandExecutionHandler();
   /**
    * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
    * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    * @return true en caso de poder manejar la ejecucion del comando
    * @return false en caso contrario
    */
	virtual bool Execute(const Command* pCommand, GenericTool* pTool);

protected:
	/** Metodo auxiliar encargado de agregar de forma concreta una capa al arbol **/
	void AddLayer(const std::string& Filter);

	DataViewManager* pDataView_;
};

}  /** namespace core **/
}  /** namespace suri **/
#endif
