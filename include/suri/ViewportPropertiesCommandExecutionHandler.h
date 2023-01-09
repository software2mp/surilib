/*! \file ViewportPropertiesCommandExecutionHandler.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VIEWPORTPROPERTIESCOMMANDEXECUTIONHANDLER_H_
#define VIEWPORTPROPERTIESCOMMANDEXECUTIONHANDLER_H_

// Includes Estandard
#include <string>

// Includes Suri
#include "suri/CommandExecutionHandlerInterface.h"

namespace suri {

// forwards
class DataViewManager;

namespace core {

/** Clase encargada de administrar la ejecucion del comando ViewportProperties **/
class ViewportPropertiesCommandExecutionHandler : public CommandExecutionHandlerInterface {
   /** Constructor Copia **/
   ViewportPropertiesCommandExecutionHandler(const ViewportPropertiesCommandExecutionHandler&);
public:
   /** Constructor **/
   explicit ViewportPropertiesCommandExecutionHandler(DataViewManager* pDataView);
   /** Destructor **/
   virtual ~ViewportPropertiesCommandExecutionHandler();
   /**
    * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
    * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    */
   virtual bool Execute(const Command* pCommand, GenericTool* pTool);

protected:
   /** Metodo auxiliar encargado de mostrar las propiedades del visualizador **/
   void ShowProperties();

private:
   /** Puntero a DataViewManager */
   DataViewManager* pDataView_;
};

} /** namespace core */
} /** namespace suri */

#endif /* VIEWPORTPROPERTIESCOMMANDEXECUTIONHANDLER_H_ */
