/*! \file QuickMeassureCommandExecutionHandler.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef QUICKMEASSURECOMMANDEXECUTIONHANDLER_H_
#define QUICKMEASSURECOMMANDEXECUTIONHANDLER_H_

// Includes Estandard
#include <string>

// Includes Suri
#include "suri/CommandExecutionHandlerInterface.h"

namespace suri {

// forwards
class DataViewManager;
class MeassureWidget;

namespace core {

/** Clase encargada de administrar la ejecucion del comando QuickMeassure **/
class QuickMeassureCommandExecutionHandler : public CommandExecutionHandlerInterface {
   /** Constructor Copia **/
   QuickMeassureCommandExecutionHandler(const QuickMeassureCommandExecutionHandler&);
public:
   /** Constructor **/
   QuickMeassureCommandExecutionHandler(
         DataViewManager* pDataView, suri::MeassureWidget* pQuickMeasure);
   /** Destructor **/
   virtual ~QuickMeassureCommandExecutionHandler();
   /**
    * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
    * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    */
   virtual bool Execute(const Command* pCommand, GenericTool* pTool);

private:
   /** Puntero a DataViewManager */
   DataViewManager* pDataView_;
   /** Puntero a MeassureWidget */
   suri::MeassureWidget* pQuickMeasure_;
};

} /** namespace core */
} /** namespace suri */

#endif /* QUICKMEASSURECOMMANDEXECUTIONHANDLER_H_ */
