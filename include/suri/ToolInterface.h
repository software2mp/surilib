/*! \file ToolInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TOOLINTERFACE_H_
#define TOOLINTERFACE_H_

// Includes Estandar
#include <string>
#include <list>
#include <map>

// Includes Suri
#include "suri/Command.h"
#include "suri/MouseEventInterface.h"
#include "suri/PaintEventInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

// forwards
class Widget;

class ToolInterface {
   ToolInterface(const ToolInterface&);
protected:
   ToolInterface() {
   }
public:
   virtual ~ToolInterface() {
   }

   virtual bool IsActive(const Command *pToolCommand) const=0;
   virtual void SetActive(Command *pToolCommand, bool Active = true)=0;
   virtual bool IsEnabled(const Command *pToolCommand) const=0;
   virtual void SetEnabled(Command *pToolCommand, bool Enable = true)=0;

   virtual void Execute(const Command *pToolCommand)=0;

   virtual std::list<Command*> GetCommands() const=0;
   virtual Command* GetCommand(const Command::CommandIdType &Id) const=0;
   virtual std::list<MouseEventInterface*> GetMouseEvents() const=0;
   virtual std::list<PaintEventInterface*> GetPaintEvents() const=0;
   virtual std::multimap<std::string, Widget*> GetWidgets() const=0;

   /** Retorna el id del comando en la interfaz grafica */
   virtual std::string GetCommandGuiId(const Command* pCommand) const = 0;
};

} /* namespace suri */

#endif /* TOOLINTERFACE_H_ */
