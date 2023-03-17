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

#ifndef VISUALIZATIONEXPORTERPROCESS_H_
#define VISUALIZATIONEXPORTERPROCESS_H_

// Includes Suri
#include "ReprojectionProcess.h"

namespace suri {

class MapElement;

/** Exporta un elemento que contenga todas las capas desplegadas en el visualizador */
class VisualizationExporterProcess : public FileExporterProcess {
   /** Constructor copia */
   VisualizationExporterProcess(const VisualizationExporterProcess&);
public:
   /** Nombre del proceso **/
   static const std::string kProcessName;
   /** Constructor */
   VisualizationExporterProcess(RasterElement* pRasterElement,
                                std::vector<Element*> InputElements,
                                const Subset &ViewerSubset,
                                DataViewManager* pDataViewManager);
   /** Destructor */
   virtual ~VisualizationExporterProcess();

protected:
   /** Obtiene la lista de elementos que se a renderizar con el proceso */
   virtual LayerList *GetElementList();
   /** Devuelve el Modelo Raster de llegada */
   virtual RasterSpatialModel* GetOutputRasterSpatialModel();
   /** Pemite configurar el RasterElement que se va a renderizar */
   virtual bool ConfigureRaster(RasterElement *pRasterElement);
   /** Retorna el nodo datacast configurado con el tipo de dato de salida */
   wxXmlNode* GetDataCastRenderizationNode();
   /** Metodo que se llama cuando se configura el ProcessAdaptLayer del proceso */
   virtual void DoOnProcessAdaptLayerSet();
   /** Retorna el elemento de entrada */
   virtual Element* GetInputElement() const { return pRasterElement_; }
   /** Configura el elemento creado con la herramienta */
   virtual bool ConfigureOutput();
   /** Configura el proceso para que la salida sea el raster configurado */
   virtual bool ConfigureProcess();
   /** Configura el subset espacial */
   virtual World *GetWorld();
   /** Devuelve un elemento de tipo mapa con la misma configuracion del que se encuentra activo */
   MapElement* GetMapElement(Element* pElement);
   /** Elemento de entrada */
   RasterElement* pRasterElement_;
   /** Vector con elementos activos */
   std::vector<Element*> InputElements_;
   /** Parametros de reproyeccion */
   ReprojectionProcess::ReprojectionParameters* pReprojectionParameters_;
};

} /** namespace suri */

#endif /* VISUALIZATIONEXPORTERPROCESS_H_ */
