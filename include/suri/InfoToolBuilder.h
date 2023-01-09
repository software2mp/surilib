/*! \file InfoToolBuilder.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef INFOTOOLBUILDER_H_
#define INFOTOOLBUILDER_H_

// Include Estandard
#include <string>

// Includes suri
#include "suri/ToolBuilderInterface.h"

namespace suri {

// forwards
class ToolInterface;
class DataViewManager;
class MeassureWidget;

namespace core {

// Clase encarga de realizar la creacion y configuracion de la herramienta de informacion
class InfoToolBuilder : public ToolBuilderInterface {
   /** Constructor Copia */
   InfoToolBuilder(const InfoToolBuilder&);

public:
   /** Constructor */
   explicit InfoToolBuilder(suri::MeassureWidget* pQuickMeasure);
   /** Destructor */
   virtual ~InfoToolBuilder();
   /**
    * Metodo que crea y configura una herramienta con los hanlders para la ejecucion
    * (CommandExecutionHandlers) y las acciones correspondientes (Commands).
    */
   virtual ToolInterface* BuildTool(DataViewManager* pDataViewManager);

private:
   /** Puntero a MeassureWidget */
   suri::MeassureWidget* pQuickMeasure_;
};

} /** namespace core */
} /** namespace suri */

#endif /* INFOTOOLBUILDER_H_ */
