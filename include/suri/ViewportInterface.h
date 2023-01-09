/*! \file ViewportInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VIEWPORTINTERFACE_H_
#define VIEWPORTINTERFACE_H_

// Includes Estandar
// Includes Suri
#include "suri/SuriObject.h"
#include "suri/ViewcontextManagerInterface.h"

// Includes Wx
// Defines
/** Macro que castea al viewport a un viewer **/
#define AS_VW(ViewportInterface) (dynamic_cast<suri::ViewerWidget*>(ViewportInterface))



namespace suri {
// forwards
class CoordinatesTransformation;

/*!
 *  Interface que representa el comportamiento de un viewport.
 */
class ViewportInterface {
   /** ctor copia **/
   ViewportInterface(const ViewportInterface&);

public:
   /** constructor **/
   ViewportInterface() {
   }
   /** destructor **/
   virtual ~ViewportInterface() {
   }
   /** Obtiene el id asociado al viewport **/
   virtual SuriObject::UuidType GetId() const=0;
   /** Obtiene el id entero asociado al viewport**/
   virtual SuriObject::UuidIntType GetIntId() const=0;
   /** Obtiene el id del viewcontext asociado**/
   virtual SuriObject::UuidType GetViewcontextUuid() const=0;
   /** Indica/asocia el viewcontext a utilizar **/
   virtual bool SetViewcontext(ViewcontextInterface *pViewcontext)=0;
   /** Obtiene la transformacion asociada **/
   virtual CoordinatesTransformation* GetTransformation()=0;

protected:
private:
};

} /* namespace suri */

#endif /* VIEWPORTINTERFACE_H_ */
