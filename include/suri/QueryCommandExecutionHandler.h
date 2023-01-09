/*! \file QueryCommandExecutionHandler.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef QUERYCOMMANDEXECUTIONHANDLER_H_
#define QUERYCOMMANDEXECUTIONHANDLER_H_

// Includes Estandar
#include "CommandExecutionHandlerInterface.h"

namespace suri {

// forwards
class DataViewManager;
class Query;

namespace core {

/** Clase encargada de administrar la ejecucion del comando Query **/
class QueryCommandExecutionHandler : public CommandExecutionHandlerInterface {
   /** Constructor copia */
   QueryCommandExecutionHandler(const QueryCommandExecutionHandler&);
public:
   /** Constructor */
   explicit QueryCommandExecutionHandler(DataViewManager* pDataView);
   /** Destructor */
   virtual ~QueryCommandExecutionHandler();
   /**
    * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
    * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    */
   virtual bool Execute(const Command* pCommand, GenericTool* pTool);

private:
   /** Puntero a DataViewManager */
   DataViewManager* pDataView_;
   /** Puntero a Query */
   Query* pQuery_;
};

} /** namespace core */
} /** namespace suri */

#endif /* QUERYCOMMANDEXECUTIONHANDLER_H_ */
