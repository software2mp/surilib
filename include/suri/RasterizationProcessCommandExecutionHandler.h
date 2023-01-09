#ifndef __RASTERIZATION_COMMAND_EXEC_HANDLER_H__
#define __RASTERIZATION_COMMAND_EXEC_HANDLER_H__

// Includes Estandard
#include <string>

// Includes Suri
#include "suri/CommandExecutionHandlerInterface.h"
#include "suri/ProcessAdaptLayer.h"
namespace suri {

// forwards
class DataViewManager;

namespace core {


/** Clase encargada de administrar la ejecucion del comando Mosaic **/
class RasterizationCommandExecutionHandler : public CommandExecutionHandlerInterface {

	/** constructor copia **/
   RasterizationCommandExecutionHandler(const RasterizationCommandExecutionHandler&);

public:
	/** Constructor **/
   RasterizationCommandExecutionHandler(DataViewManager* pDataView);
	/** Destructor **/
	virtual ~RasterizationCommandExecutionHandler();
	/**
	 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
	 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    * @return true en caso de poder manejar la ejecucion del comando
    * @return false en caso contrario
    */
	virtual bool Execute(const Command* pCommand, GenericTool* pTool);

protected:
	void SetAttributes(ProcessAdaptLayer &Pal);
	DataViewManager* pDataView_;
};

}  /** namespace core **/
}  /** namespace suri **/
#endif
