/*! \file ReprojectionTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef REPROJECTIONTOOL_H_
#define REPROJECTIONTOOL_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "GenericTool.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Ejecuta los comandos relacionados a las transformaciones de reproyeccion
 */
class ReprojectionTool : public suri::GenericTool {
public:
   /** Constructor */
   explicit ReprojectionTool(DataViewManager* pDataViewManager);
   /** Destructor */
   virtual ~ReprojectionTool();
   /** Ejecucion del comando */
   virtual void Execute(const Command* pToolCommand);
   /** Despliega el widget para corregistro */
   void ExecuteCoregister() const;
   /** Despliega el widget para georreferenciacion */
   void ExecuteGeoreference() const;
   /** Despliega el widget para reproyeccion */
   void ExecuteReproject() const;

protected:
   /** Creacion del Comando */
   virtual Command* CreateCommand(const std::string &CommandName);
   /** Destruccion del Comando */
   virtual void DestroyCommand(Command* &pCommand) const;
   /** Creacion del Widget */
   virtual Widget* CreateWidget(const std::string &Widget);

private:

};

} /** namespace suri */

#endif /* REPROJECTIONTOOL_H_ */
