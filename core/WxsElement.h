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

#ifndef WXSELEMENT_H_
#define WXSELEMENT_H_

// Includes standard
#include <map>
// Includes Suri
#include "suri/RasterElement.h"
#include "suri/World.h"
// Includes Wx
// Defines

namespace suri {

/** Clase que representa imagenes remostas obtenidas mediante wms */
class WxsElement : public RasterElement {
private:
   /** Ctor de copia */
   WxsElement(const WxsElement &WxsElement);

public:
   /** Ctor */
   WxsElement();
   /** Dtor */
   ~WxsElement();

   /** Guarda el extent del raster en el nodo. */
   virtual void SetElementExtent(const Subset &WorldExtent);

   /** Devuelve los Part que se van a mostrar en las Propiedades. */
   virtual std::vector<Part *> DoGetParts(
         DataViewManager *pDataViewManager = NULL,
         LayerInterface *pLayer = NULL,
         DatasourceInterface *pDatasource = NULL);

protected:
   /** Parsea url y retorna datos de interes(url, layer, tipo de capa y estilo)*/
   bool ParseWxsFilename(const std::string &FileName,
                         std::map<std::string, std::string> &Keys);

private:
   /** Inicializa WxsElement con archivo en FileName */
   virtual RasterElement* Initialize(const std::string &FileName,
                                     const Option& Metadata = Option());

   /** Genera xml que requiere GDAL para abrir una imagen. */
   virtual std::string CreateWxsXml(const std::string &FileName, const Option& Metadata);

   /** Devuelve vector con Parts especificos de la visualizacion elemento. */
   virtual std::vector<suri::Part*> DoGetVisualizationParts(
         DataViewManager* pDataViewManager = NULL, LayerInterface *pLayer = NULL,
         DatasourceInterface *pDatasource = NULL);

};

}  //namespace

#endif /* WXSELEMENT_H_ */
