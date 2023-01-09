/*! \file HistogramMatchingExecutionHandler.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef  __HISTOGRAM_MATCHING_COMMAND_EXEC_HANDLER_H__
#define __HISTOGRAM_MATCHING_COMMAND_EXEC_HANDLER_H__

// Includes Estandard
#include <string>

// Includes Suri
#include "suri/CommandExecutionHandlerInterface.h"
#include "suri/ProcessAdaptLayer.h"

namespace suri {

// forwards
class DataViewManager;
class Element;

namespace core {

/** Clase encargada de administrar la ejecucion del comando  **/
class HistogramMatchingCommandExecutionHandler : public CommandExecutionHandlerInterface {
   /** Constructor copia **/
   HistogramMatchingCommandExecutionHandler(const HistogramMatchingCommandExecutionHandler&);

public:
   /** Constructor **/
   HistogramMatchingCommandExecutionHandler(DataViewManager* pDataView);
   /** Destructor **/
   virtual ~HistogramMatchingCommandExecutionHandler();
   /**
    * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
    * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    * @return true en caso de poder manejar la ejecucion del comando
    * @return false en caso contrario
    */
   virtual bool Execute(const Command* pCommand, GenericTool* pTool);

private:
   /**
    * Retorna un vector con los rasters de la lista de capas ubicando el raster seleccionado
    * en la posicion 0
    */
   std::vector<Element*> GetRasterElements(Element* pElement);
   /** Puntero a DataViewManager */
   DataViewManager* pDataView_;
};

}  /** namespace core **/
}  /** namespace suri **/
#endif
