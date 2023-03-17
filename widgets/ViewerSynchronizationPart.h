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

#ifndef VIEWERSYNCHRONIZATIONPART_H_
#define VIEWERSYNCHRONIZATIONPART_H_

// Includes standard

// Includes Suri
#include "suri/Part.h"
#include "suri/ViewportManagerInterface.h"
#include "suri/ViewportLinkManager.h"

// Includes Wx

// Includes App

// Defines

namespace suri {

/** Part que permite sincronizar visualizadores */
/**
 * Se pueden elegir los visualizadores a sincronizar, el tipo de
 * sincronizacion(geografica, proyectada, imagen) y si solo se sincroniza
 * el centro o el bounding box completo.
 */
class ViewerSynchronizationPart : public Part {
public:
   /** Ctor */
   ViewerSynchronizationPart(ViewportManagerInterface *pViewportManager,
                             ViewcontextManagerInterface *pViewcontextManager,
                             ViewportLinkManager *pViewportLink, bool Enable = true,
                             bool Modified = false);
   /** Dtor */
   virtual ~ViewerSynchronizationPart();

   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valor's originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Modifica el nombre del viewer. */
   virtual bool ConfigureWidget();
   /** Devuelve si la parte tiene datos validos */
   virtual bool HasValidData();

private:
   ViewportManagerInterface *pViewportManager_;
   ViewcontextManagerInterface *pViewcontextManager_;
   ViewportLinkManager* pViewportLink_;
};

}

#endif /* VIEWERSYNCHRONIZATIONPART_H_ */
