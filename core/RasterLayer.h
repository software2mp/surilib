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

#ifndef RASTERLAYER_H_
#define RASTERLAYER_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/LayerInterface.h"
#include "suri/Element.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/***
 *  RasterLayer modela una capa de tipo raster
 */
class RasterLayer : public LayerInterface {
public:
   /** Constructor \deprecated */
   RasterLayer(DatasourceInterface *pDatasource, Element *pElement);
   /** Constructor*/
   explicit RasterLayer(DatasourceInterface *pDatasource);
   /** Destructor */
   virtual ~RasterLayer();
   /** Obtiene la Datasource asociada a la Layer. Trazabilidad entre Datasource Layer */
   virtual SuriObject::UuidType GetAssociatedDatasourceId() const;
   /** Devuelve el estilo de renderizacion del layer */
   virtual void GetRenderizationStyle(const SuriObject::UuidType &LayerId) const;
   /** Devuelve el elemento \deprecated */
   virtual Element* GetElement();
   /** Devuelve la cantidad de bandas del raster */
   virtual int GetBandCount() const;
   /** Devuelve el nombre de una banda */
   virtual bool GetBandName(std::string &BandName, int BandNumber) const;
   /** Compara un Datasource contra un Layer */
   virtual bool Equals(DatasourceInterface &Datasource);
   /** Compara dos tipos diferentes de Layers */
   virtual bool Equals(LayerInterface &Layer);
   /** Compara un Layer contra un Element \deprecated */
   virtual bool Equals(Element &Element);
   /** Oculta */
   virtual void Hide(bool Hide = true);
   /** Verifica si esta oculto */
   virtual bool IsHidden() const;
   /** Setea el nombre de la capa **/
   virtual void SetName(const std::string& Name);
   /** Setea el titulo de la capa **/
   virtual void SetTitle(const std::string& Title);
   /** Configura el viewcontext al cual pertenece **/
   virtual void SetViewcontext(ViewcontextInterface* pViewcontext);
   /** Obtiene la instancia de fuente de datos asociada a la capa **/
   virtual DatasourceInterface* FetchAssociatedDatasource();
   /** Devuelve el viewcontext */
   virtual ViewcontextInterface* GetViewcontext();

private:
   /** Constructor de copia */
   RasterLayer(const RasterLayer &);
   /** Referencia al elemento que encapsula */
   DatasourceInterface *pDatasource_;
   /** Referencia al elemento \deprecated */
   Element *pElement_;
   /** Variable Ocultacion */
   bool hidden_;
   /** nombre y titulo de la capa **/
   std::string name_, title_;
   /** Viewcontext al que pertenece **/
   ViewcontextInterface* pViewcontext_;
};
} /** namespace suri */

#endif /* RASTERLAYER_H_ */
