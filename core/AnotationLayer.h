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

#ifndef ANOTATIONLAYER_H_
#define ANOTATIONLAYER_H_

// Includes Estandar

// Includes Suri
#include "VectorLayer.h"
#include "suri/Element.h"
#include "suri/DatasourceInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * AnotationLayer
 */
class AnotationLayer : public suri::VectorLayer {
public:
   /** Constructor */
   explicit AnotationLayer(DatasourceInterface *pDatasource);
   /** Constructor \deprecated */
   AnotationLayer(DatasourceInterface *pDatasource, Element *pElement);
   /** Destructor */
   virtual ~AnotationLayer();
   /** Obtiene el id del datasource asociado */
   virtual SuriObject::UuidType GetAssociatedDatasourceId() const;
   /** Devuelve el estilo de renderizacion del layer */
   virtual void GetRenderizationStyle(const SuriObject::UuidType &LayerId) const;
   /** Devuelve el elemento \deprecated */
   virtual Element* GetElement();
   /** Compara un Datasource contra un Layer*/
   virtual bool Equals(DatasourceInterface &Datasource);
   /** Compara dos tipos diferentes de Layers */
   virtual bool Equals(LayerInterface &Layer);
   /** Compara un Layer contra un Element */
   virtual bool Equals(Element &Element);

private:
   /** Constructor de Copia */
   AnotationLayer(const AnotationLayer&);
   /** Puntero al datasource */
   DatasourceInterface *pDatasource_;
   /** Puntero al elemento */
   Element *pElement_;
};

} /** namespace suri */

#endif /* ANOTATIONLAYER_H_ */
