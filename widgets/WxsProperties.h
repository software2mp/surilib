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

#ifndef WXSPROPERTIES_H_
#define WXSPROPERTIES_H_

// Includes standard

// Includes Suri
#include "suri/Part.h"
#include "WxsRenderer.h"

// Includes Wx

// Includes App

// Defines

namespace suri {

class WxsElement;

/** Permite modificar las propiedades de una fuente wxs. */
/**
 * Las propiedades incluyen formato de archivo, sistema de referencia y
 * bounding box.
 */
class WxsProperties : public Part {
public:
   /** Ctor */
   WxsProperties(bool Enable = true, bool Modified = false);
   /** Ctor */
   WxsProperties(WxsElement* pWxsElement, bool Enable = true, bool Modified = false);
   /** Dtor */
   virtual ~WxsProperties();

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

   /** Configura los parametros ingresados por usuario */
   void ConfigureParameters(WxsRenderer::Parameters &Parameters);

protected:
   /** Configura los widgetParameters con datos en control */
   bool ReadParametersFromWidget();
   /** Actualiza el control con datos en los widgetParameters */
   bool UpdateParametersToWidget();

   WxsRenderer::Parameters parameters_;  ///< Parametros donde se guardan los
                                         ///< datos en commit
   WxsElement* pWxsElement_;  ///< Si no es NULL se le actualiza el nodo wxs en
                              ///< cada commit.
   WxsRenderer::Parameters widgetParameters_;  ///< Parametros con valores
                                               ///< ingresados en pantalla

};
}

#endif /* WXSPROPERTIES_H_ */
