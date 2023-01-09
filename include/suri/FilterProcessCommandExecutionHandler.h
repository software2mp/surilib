/*! \file FilterProcessCommandExecutionHandler.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FILTERPROCESSCOMMANDEXECUTIONHANDLER_H_
#define FILTERPROCESSCOMMANDEXECUTIONHANDLER_H_

#include "CommandExecutionHandlerInterface.h"

// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {
class DataViewManager;

namespace core {

/**
 * Clase encargada de administrar la ejecucion del comando
 * ApplyFilter
 **/
class FilterProcessCommandExecutionHandler : public CommandExecutionHandlerInterface {

   /** constructor copia **/
   FilterProcessCommandExecutionHandler(const FilterProcessCommandExecutionHandler&);

public:
   /** Constructor **/
   FilterProcessCommandExecutionHandler(DataViewManager* pDataView);
   /** Destructor **/
   virtual ~FilterProcessCommandExecutionHandler();
   /**
    * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
    * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    * @return true en caso de poder manejar la ejecucion del comando
    * @return false en caso contrario
    */
   virtual bool Execute(const Command* pCommand, GenericTool* pTool);

private:
   DataViewManager* pDataView_;
};

/** Clase encargada de administrar la ejecucion del comando FilterLibrary **/
class FilterLibraryCommandExecutionHandler : public CommandExecutionHandlerInterface {

   /** constructor copia **/
   FilterLibraryCommandExecutionHandler(const FilterLibraryCommandExecutionHandler&);

public:
   /** Constructor **/
   FilterLibraryCommandExecutionHandler(DataViewManager* pDataView);
   /** Destructor **/
   virtual ~FilterLibraryCommandExecutionHandler();
   /**
    * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
    * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    * @return true en caso de poder manejar la ejecucion del comando
    * @return false en caso contrario
    */
   virtual bool Execute(const Command* pCommand, GenericTool* pTool);

private:
   DataViewManager* pDataView_;
};
} /** namespace core */
} /** namespace suri */

#endif /* FILTERPROCESSCOMMANDEXECUTIONHANDLER_H_ */
