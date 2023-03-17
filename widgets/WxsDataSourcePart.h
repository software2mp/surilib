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

#ifndef WXSDATASOURCEPART_H_
#define WXSDATASOURCEPART_H_

// Includes standard

// Includes Suri
#include "suri/Part.h"

// Includes Wx

// Includes App

// Defines

namespace suri {

class Element;

/** Permite cargar una fuente wxs. */
/**
 * Se debe ingresar el url del servidor y el nombre de la capa.
 */
class WxsDataSourcePart : public Part {
public:
   /** Ctor */
	WxsDataSourcePart(bool Enable = true,
			bool Modified = false);
   /** Ctor */
   WxsDataSourcePart(Element* pElement, bool Enable = true, bool Modified = false);
   /** Dtor */
   virtual ~WxsDataSourcePart();

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

   /** Retorna el url construida con el nombre del servidor y la capa */
   std::string GetWxsElementUrl();
   /** Retorna la descripcion configurada por el usuario */
   std::string GetDescription();
   /* Modifica el contenido del control con string */
   void SetValue(std::string ControlName, const std::string Value);

protected:
   /** Configura los widgetParameters con datos en control */
   bool ReadParametersFromWidget();
   /** Actualiza el control con datos en los widgetParameters */
   bool UpdateParametersToWidget();

   Element* pElement_;  ///< Si no es NULL se le actualiza el url y la
                        ///< descripcion en cada commit.

   std::string url_;  ///< Servidor de la fuente wxs
   std::string layers_;  ///< Capas que se desea desplegar
   std::string description_;  ///< Descripcion de las capas

   std::string widgetUrl_;  ///< Servidor de la fuente wxs en control
   std::string widgetLayers_;  ///< Capas que se desea desplegar en control
   std::string widgetDescription_;  ///< Descripcion de las capas en control
};

}  //namespace

#endif /* WXSDATASOURCEPART_H_ */
