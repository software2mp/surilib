/*! \file ViewportLinkManager.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VIEWPORTLINKMANAGER_H_
#define VIEWPORTLINKMANAGER_H_

// Includes Estandar
#include <map>
#include <vector>

// Includes Suri
#include "suri/ViewportInterface.h"
#include "suri/ViewportLinkInterface.h"
#include "suri/ViewportManagerInterface.h"
#include "suri/World.h"
#include "suri/WorldView.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 *
 */
class ViewportLinkManager {
   ViewportLinkManager(const ViewportLinkManager&);

public:
   ViewportLinkManager(ViewportManagerInterface *pViewportManager,
                       ViewcontextManagerInterface *pViewcontextManager);
   virtual ~ViewportLinkManager();

   /** Agrega un viewport para controlar el enlace */
   /*!
    *  Agrega un viewport para controlar el enlace
    * @param pViewport Viewport para controlar.
    * @param Master Los viewports agregados con Master == True modificaran el
    *  World del resto. Los que sean agregados con False, no lo haran.
    * @return Indica el exito de la operacion.
    */
   virtual bool AddViewport(ViewportInterface *pViewport, bool Master = true);
   virtual bool RemoveViewport(const SuriObject::UuidType &ViewportId);
   virtual bool RemoveViewports();

   virtual ViewportLinkInterface* SetLinkCommand(ViewportLinkInterface *pLink);

   virtual std::vector<SuriObject::UuidType> GetLinkedViewports() const;
   virtual bool IsMasterViewport(const SuriObject::UuidType &ViewportId) const;

protected:
   virtual void OnMasterWorldChange(Model *pWorld);
   virtual void OnMasterWorldDelete(Model *pWorld);

private:
   ViewportManagerInterface *pViewportManager_;
   ViewcontextManagerInterface *pViewcontextManager_;
   std::map<SuriObject::UuidType, WorldView<ViewportLinkManager>*> viewportWorldViewers_;
   std::map<SuriObject::UuidType, bool> linkedViewports_;
   bool isSychronizing_;
   ViewportLinkInterface *pViewportLink_;
};

} /* namespace suri */

#endif /* VIEWPORTLINKMANAGER_H_ */
