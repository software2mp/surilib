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

#ifndef INDEXEXPORTERWIDGET_H_
#define INDEXEXPORTERWIDGET_H_

// Includes standar
#include <string>
#include <vector>

// Includes suri
#include "suri/FileExporterProcess.h"
#include "suri/RasterElement.h"

/** namespace suri */
namespace suri {
// Forward
class DataViewManager;
/** Clase que crea una herramienta para el calculo de indices. */
/**
 * Esta clase recibe un elemento y un subset. A las partes que agrega la clase
 * base (seleccion de subset espacial, y nombre y formato de archivo) agrega una
 * parte para seleccion de indices y bandas.
 * Con los datos obtenidos de estas partes configura el elemento para aplicar
 * la herramienta de indice.
 * Calcula el espacio requerido por el elemento creado.
 * FileExporterWidget normalmente incluye un Part de seleccion espectral, como
 * esta funcionalidad no es requerida en FileExporterWidget se elimina
 * impelmentando CreateSpectralSectionPart y dejandolo vacio.
 * En el futuro se podria separar la logica de archivo de la de seleccion
 * espectral para que esto no sea necesario.
 */
class IndexProcess : public FileExporterProcess {
   /** Ctor. de copia */
   IndexProcess(const IndexProcess &IndexExporterWidget);

public:
   /** Nombre del proceso **/
   static const std::string kProcessName;
   /** Constructor */
   IndexProcess(Element *pInputElement, const Subset &ViewerSubset,
                DataViewManager* pDataViewManager);
   /** Destructor */
   virtual ~IndexProcess();
   /** Clave para atributo de seleccion de indice **/
   static const std::string IndexKeyAttr;
   /** Clave para el atributo de ecuaciones */
   static const std::string EquationKey;

protected:
   /** 
    * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
    * agreguen sus atributos especificos
    * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
    * @return vector vacio en caso de que no tenga parametros adicionales
    **/
   std::vector<wxXmlNode*> DoGetXmlParameters() const;
   // ------ Metodos que permiten configurar el proceso raster ------
   /** Permite configurar la salida del proceso antes de correrlo */
   virtual bool ConfigureRaster(RasterElement *pRasterElement);
   /** Otiene el tamano del objeto a salvar */
   virtual wxLongLong GetNeededSpace();

   // ------ Metodos que permiten redefinir bandas de entrada ------
   /** Retorna las bandas selecciondas por el usuario */
   virtual void GetSelectedRasterBands(std::vector<int>& SelectedBands);
   /** Indica si las bandas se configuraron correctamente */
   virtual bool HasValidSpectralData();

private:
   /** Crea el nodo renderizacion con el indice */
   wxXmlNode* GetIndexRenderizationNode();
};

}  // namespace suri

#endif /* INDEXEXPORTERWIDGET_H_ */
