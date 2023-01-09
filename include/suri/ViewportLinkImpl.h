/*! \file ViewportLinkImpl.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VIEWPORTLINKIMPL_H_
#define VIEWPORTLINKIMPL_H_

// Includes Estandar
#include <vector>

// Includes Suri
#include "suri/ViewportLinkInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/*!
 *
 */
class ViewportLinkImpl : public ViewportLinkInterface {
public:
   ViewportLinkImpl();
   virtual ~ViewportLinkImpl();

   virtual void SetMasterViewport(const SuriObject::UuidType &MasterViewport);
   virtual void SetLinkedViewports(
         const std::vector<SuriObject::UuidType> &LinkedViewports);
   virtual void SetManagers(ViewportManagerInterface *pViewportManager,
                            ViewcontextManagerInterface *pViewcontextManager);

protected:
   virtual ViewportInterface *GetMasterViewport() const;
   virtual std::vector<ViewportInterface*> GetLinkedViewports() const;
   virtual ViewportManagerInterface *GetViewportManager() const;
   virtual ViewcontextManagerInterface *GetViewcontextManager() const;

private:
   SuriObject::UuidType masterViewportId_;
   std::vector<SuriObject::UuidType> linkedViewportIds_;
   ViewportManagerInterface *pViewportManager_;
   ViewcontextManagerInterface *pViewcontextManager_;
};

} /** namespace suri */

#endif /* VIEWPORTLINKIMPL_H_ */
