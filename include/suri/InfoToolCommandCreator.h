/*! \file InfoToolCommandCreator.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef INFOTOOLCOMMANDCREATOR_H_
#define INFOTOOLCOMMANDCREATOR_H_

// Includes Estandard
#include <string>

// Includes Suri
#include "suri/CommandCreatorInterface.h"

namespace suri {

// forwards
class Command;
class ToolInterface;
class DataViewManager;

namespace core {

/**
 * Clase que encargada de configurar el Command para la herramienta de informacion
 */
class InfoToolCommandCreator : public CommandCreatorInterface {
   /** Constructor copia */
   InfoToolCommandCreator(const InfoToolCommandCreator&);
public:
   /** Constructor */
   InfoToolCommandCreator(DataViewManager* pDataView);
   /** Destructor */
   virtual ~InfoToolCommandCreator();

private:
   /**
    * Creacion de un comando a partir de su nombre
    * (siempre y cuando el nombre del comando sea del dominio de la clase).
    */
   virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
   /** Puntero a DataViewManager */
   DataViewManager* pDataView_;
};

} /** namespace core */
} /** namespace suri */

#endif /* INFOTOOLCOMMANDCREATOR_H_ */
