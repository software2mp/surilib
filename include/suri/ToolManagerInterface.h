/*! \file ToolManagerInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TOOLMANAGERINTERFACE_H_
#define TOOLMANAGERINTERFACE_H_

// Includes Estandar

// Includes Suri
#include "suri/ToolInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/*!
 *
 */
class ToolManagerInterface {
public:
   ToolManagerInterface() {
   }
   virtual ~ToolManagerInterface() {
   }

   /** Agrega una herramienta */
   virtual void AddTool(ToolInterface *pTool)=0;
   /** Remueve una herramienta */
   virtual void RemoveTool()=0;

protected:
private:
};

} /* namespace suri */

#endif /* TOOLMANAGERINTERFACE_H_ */
