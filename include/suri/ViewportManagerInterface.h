/*! \file ViewportManagerInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VIEWPORTMANAGERINTERFACE_H_
#define VIEWPORTMANAGERINTERFACE_H_

// Includes Estandar
#include <vector>

// Includes Suri
#include "suri/ViewerWidget.h"
#include "SuriObject.h"

// Includes Wx

// Defines

// forwards

namespace suri {
class ViewportInterface;
class ViewportManagerInterface {
   ViewportManagerInterface(const ViewportManagerInterface&);
protected:
   ViewportManagerInterface() {
   }
public:
   virtual ~ViewportManagerInterface() {
   }

   virtual SuriObject::UuidType CreateViewport(
         const SuriObject::ClassIdType &ViewportType)=0;
   virtual std::vector<SuriObject::UuidType> GetViewportIds() const=0;
   virtual void SelectViewport(const SuriObject::UuidType &ViewportId)=0;
   virtual ViewportInterface* GetSelectedViewport() const=0;
   virtual ViewportInterface* GetViewport(
         const SuriObject::UuidType &ViewportId) const=0;
   virtual void SetViewportActive(const SuriObject::UuidType &ViewportId, bool Active =
         true)=0;
   virtual bool IsViewportActive(const SuriObject::UuidType &ViewportId) const=0;
protected:
private:
};

} /* namespace suri */

#endif /* VIEWPORTMANAGERINTERFACE_H_ */
