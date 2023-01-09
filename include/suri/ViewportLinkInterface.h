/*! \file ViewportLinkInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VIEWPORTLINKINTERFACE_H_
#define VIEWPORTLINKINTERFACE_H_

// Includes Estandar
#include <vector>

// Includes Suri
#include "suri/ViewportManagerInterface.h"
#include "suri/ViewcontextManagerInterface.h"
#include "suri/DatasourceManagerInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/*!
 *
 */
class ViewportLinkInterface {
public:
   explicit ViewportLinkInterface(bool LinkCenter = true) {
   }
   virtual ~ViewportLinkInterface() {
   }

   virtual void SetMasterViewport(const SuriObject::UuidType &MasterViewport)=0;
   virtual void SetLinkedViewports(
         const std::vector<SuriObject::UuidType> &LinkedViewports)=0;
   virtual void SetManagers(ViewportManagerInterface *pViewportManager,
                            ViewcontextManagerInterface *pViewcontextManager)=0;
   virtual bool IsCentered() const=0;
   virtual bool Execute()=0;
protected:
private:
};

} /** namespace suri */

#endif /* VIEWPORTLINKINTERFACE_H_ */
