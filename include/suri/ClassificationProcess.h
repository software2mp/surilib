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

#ifndef CLASSIFICATIONEXPORTERWIDGET_H_
#define CLASSIFICATIONEXPORTERWIDGET_H_

// Includes standard
#include <string>
#include <vector>
// Includes Suri
#include "suri/FileExporterProcess.h"
#include "ClassInformation.h"

/** namespace suri */
namespace suri {
// foward
class ViewcontextManagerInterface;
class DatasourceManagerInterface;

/** Clase para clasificar usando poligonos */
/**
 * Utiliza una lista con elementos vectoriales de tipo poligono (representan
 * clases) para clasificar un elemento.
 * Agrega un Part para seleccion de parametros del clasificador.
 * Crea un elemento nuevo con el resultado de la clasificacion, que tiene una
 * tabla de color con los colores de las clases.
 */
class ClassificationProcess : public FileExporterProcess {
   /** Ctor. de copia */
   ClassificationProcess(const ClassificationProcess &);

public:
   /** Nombre del proceso **/
   static const std::string kProcessName;
   /** Ctor. */
   ClassificationProcess(LayerList *pLayerList, Element *pInputElement,
                         const Subset &WindowSubset,
                         std::string ClassificationType,
                         DataViewManager* pDataViewManager,
                         bool ShowSpectralPart = false);
   /** Dtor. */
   virtual ~ClassificationProcess();
   /** Clave para atributo de tipo de clasificacion**/
   static const std::string ClassificationType;
   /** Clave para la obtencion de funcion encargada de obtener las propiedades
    *  de las clases y el nodo xml resultantes de la clasificacion*/
   static const std::string ClassGetterAttrKey;

protected:
   /** Metodo que especializa la configuracion del ProcessAdaptLayer para
    *  los procesos especificos
    */
   virtual void DoOnProcessAdaptLayerSet();
   /** Configura el nodo antes de renderizar */
   virtual bool ConfigureRaster(RasterElement *pRasterElement);
   /** Configura el elemento creado con la herramienta */
   virtual bool ConfigureOutput();
   /** Otiene el tamanio del objeto a salvar */
   wxLongLong GetNeededSpace();
   /** 
    * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
    * agreguen sus atributos especificos
    * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
    * @return vector vacio en caso de que no tenga parametros adicionales
   **/
   virtual std::vector<wxXmlNode*> DoGetXmlParameters() const; 

private:
   /*! Layer list donde estan los poligonos con clases */
   LayerList *pLayerList_;
   DatasourceManagerInterface* pDatasourceManager_;
   ViewcontextManagerInterface* pViewcontextManager_;
   std::string classificationType_;
};
}  // suri

#endif /* CLASSIFICATIONEXPORTERWIDGET_H_ */
