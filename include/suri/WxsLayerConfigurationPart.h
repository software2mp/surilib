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

#ifndef WXSLAYERCONFIGURATIONPART_H_
#define WXSLAYERCONFIGURATIONPART_H_

// Includes standard
#include <string>

// Includes Suri
#include "suri/PartCollection.h"
#include "suri/LibraryItemSelectionInterface.h"
#include "BasicLibraryItemSelector.h"
#include "suri/DataViewManager.h"
// Includes Wx

// Includes App

// Defines

namespace suri {

class WxsDataSourcePart;
class WxsProperties;
class WxsVisualizationPart;

/** Permite agregar una capa wxs al proyecto */
/**
 * Para agregar fuentes wxs se pueden usar las que vienen precargadas o ingresar
 * una nueva a mano.
 */
class WxsLayerConfigurationPart : public PartCollection, public LibraryItemSelectionInterface {
   /** Ctor */
public:
   WxsLayerConfigurationPart(std::string* pWxsElementFactoryString,
                             DataViewManager* pDataViewManager);
   /** Dtor */
   virtual ~WxsLayerConfigurationPart();

   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Actualiza el TextCtrl del part con el valor de la biblioteca */
   void SetActiveItem(const LibraryItem* pItem);

protected:
   /** Actualiza el string que configura el elemento wms con los datos en parts */
   void UpdateFactoryString();

   std::string* pWxsElementFactoryString_;  ///< Clase donde se configuran los
                                            ///< datos ingresados por el usuario
   WxsDataSourcePart* pWxsDataSourcePart_;        ///< Part donde se carga la fuente wms
   WxsProperties* pWxsProperties_;  ///< Part donde se cargan las
                                    ///< propiedades wms
   WxsVisualizationPart* pWxsVisualizationPart_;  ///< Part donde se cargan las
   ///< propiedades de visualizacion
   BasicLibraryItemSelector* pItemSelector_; ///Puntero al selector de items de la biblioteca
};
}

#endif /* WXSLAYERCONFIGURATIONPART_H_ */
