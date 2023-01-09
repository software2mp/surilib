/*! \file InfoTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef INFOTOOL_H_
#define INFOTOOL_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/GenericTool.h"

// Includes Wx

// Defines

// forwards

namespace suri {

// forwards
class Query;

/*!
 * Ejecuta los comandos de informacion
 */
class InfoTool : public GenericTool {
public:
   /** Constructor */
   explicit InfoTool(DataViewManager* pDataViewManager);
   /** Destructor */
   virtual ~InfoTool();
   /** Ejecuta un comando */
   virtual void Execute(const Command* pToolCommand);

protected:
   /** Crea un comando */
   virtual Command* CreateCommand(const std::string &Command);
   /** Destruye un comando */
   virtual void DestroyCommand(Command* &pCommand) const;
   /** Crea el widget */
   virtual Widget* CreateWidget(const std::string &Widget);

private:
   Query* pQuery_; /*! Puntero a query */
};

} /* namespace suri */

#endif /* INFOTOOL_H_ */
