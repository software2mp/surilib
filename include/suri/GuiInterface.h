/*! \file GuiInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GUIINTERFACE_H_
#define GUIINTERFACE_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/Command.h"

// Includes Wx
#include "wx/wx.h"
// Defines
// forwards

namespace suri {

// forwards
class Widget;
class ToolInterface;

/*!
 * Interfaz que representa el comportamiento comun
 * a todos los administradores de la aplicacion
 * (a nivel GUI)
 */
class GuiInterface {
public:
   GuiInterface(Widget *pWidget) {
   }
   virtual ~GuiInterface() {
   }
   /** Agrega la herramienta en la ubicacion de los Commands asociados**/
   virtual bool AddTool(ToolInterface* pTool) = 0;
   /** Elimina la herramienta de las ubicaciones del os Commandas asociados **/
   virtual bool RemoveTool(ToolInterface* pTool) = 0;
   virtual void AddToolbar(Widget *pWidget)=0;
   /** Elimina el Toolbar en la ubicacion indicada por parametro**/
   virtual void RemoveToolbar(const std::string& LocationId)=0;
protected:
   /** Metodo que agrega un menu en la ubicacion descripta por el command**/
   virtual void AddMenu(Command *pCommand, const std::string& MenuLocation)=0;
   /** Elimina un menu **/
   virtual void RemoveMenu(const std::string& MenuLocation)=0;
   /** Agrega widget **/
   virtual void AddWidget(Widget *pWidget)=0;
   /** Elimina widget **/
   virtual void RemoveWidget()=0;

private:
};

} /* namespace suri */

#endif /* GUIINTERFACE_H_ */
