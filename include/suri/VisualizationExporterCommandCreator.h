/*! \file VisualizationExporterCommandCreator.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VISUALIZATIONEXPORTERCOMMANDCREATOR_H_
#define VISUALIZATIONEXPORTERCOMMANDCREATOR_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/CommandCreatorInterface.h"

namespace suri {

// forwards
class Command;
class ToolInterface;

namespace core {

class VisualizationExporterCommandCreator : public CommandCreatorInterface {
   /** Constructor copia */
   VisualizationExporterCommandCreator(const VisualizationExporterCommandCreator&);
public:
   /** Constructor */
   VisualizationExporterCommandCreator();
   /** Destructor */
   virtual ~VisualizationExporterCommandCreator();

private:
   /**
    * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
    * del comando sea del dominio de la clase.
    **/
   virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

}  /** namespace core */
} /** namespace suri */

#endif /* VISUALIZATIONEXPORTERCOMMANDCREATOR_H_ */
