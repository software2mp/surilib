/*! \file NavigationToolBuilder.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef NAVIGATIONTOOLBUILDER_H_
#define NAVIGATIONTOOLBUILDER_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/ToolBuilderInterface.h"

namespace suri {

// forwards
class ToolInterface;
class DataViewManager;
class Navigator;

namespace core {

// Clase encarga de realizar la creacion y configuracion de la herramienta de navegacion
class NavigationToolBuilder : public ToolBuilderInterface {
   /** Constructor Copia */
   NavigationToolBuilder(const NavigationToolBuilder&);

public:
   /** Constructor */
   explicit NavigationToolBuilder(Navigator *pNavigator);
   /** Destructor */
   virtual ~NavigationToolBuilder();
   /**
    * Metodo que crea y configura una herramienta con los hanlders para la ejecucion
    * (CommandExecutionHandlers) y las acciones correspondientes (Commands).
    */
   virtual ToolInterface* BuildTool(DataViewManager* pDataViewManager);

private:
   /** Puntero a Navigator */
   Navigator *pNavigator_;
};

} /** namespace core */
} /** namespace suri */

#endif /* NAVIGATIONTOOLBUILDER_H_ */
