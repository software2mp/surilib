/** \file HelpTool.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef HELPTOOL_H_
#define HELPTOOL_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/GenericTool.h"

// Includes Wx

// Defines

// forwards
class wxHtmlHelpController;

namespace suri {

/**
 * Ejecuta los comandos correspondientes a las tareas de Ayuda
 */
class HelpTool : public GenericTool {
public:
   /** Constructor */
   explicit HelpTool(DataViewManager* pDataViewManager);
   /** Destructor */
   virtual ~HelpTool();
   /** Ejecucion del comando */
   virtual void Execute(const Command* pToolCommand);

protected:
   /** Creacion del Comando */
   virtual Command* CreateCommand(const std::string &CommandName);
   /** Destruccion del Comando */
   virtual void DestroyCommand(Command* &pCommand) const;
   /** Creacion del Widget */
   virtual Widget* CreateWidget(const std::string &Widget);

private:
   wxHtmlHelpController* pHelpController_; /*! controlador de ayuda */
};
}

#endif /* HELPTOOL_H_ */
