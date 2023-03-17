/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
