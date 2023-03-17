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

#ifndef RASTERDNINFO_H_
#define RASTERDNINFO_H_

#include <vector>
#include <string>

namespace suri {

class DatasourceInterface;
class MemoryCanvas;
class World;
class Element;
class CoordinatesTransformation;

/** Clase que obtiene informacion de las bandas de una capa raster **/
class RasterDnInfo {
   /** ctor copia **/
   RasterDnInfo(const RasterDnInfo&);

public:
   /** Ctor. */
   RasterDnInfo();
   /** Dtor. */
   ~RasterDnInfo();
   /**
    * Configura el pixel (de la combinacion pixel/linea).
    */
   void SetPixel(double Pixel);

   /**
    * Configura la linea (de la combinacion pixel/linea).
    */
   void SetLine(double Line);

   /**
    * Configura el origen de datos para para el cual se van a calcular los DN.
    */
   void SetDatasource(DatasourceInterface* Datasource);

   /**
    * Configura el mundo a ser utilizado.
    */
   void SetWorld(World* pWorld);

   /**
    * Configura el canvas a ser utilizado.
    */
   void SetCanvas(MemoryCanvas* pCanvas);

   /**
    * Configura el elemento a ser utilizado.
    */
   void SetElement(Element* pElement);

   /**
    * Devuelve un vecto que contiene para cada banda cual es el DN para el pixel/linea.
    */
   std::vector<std::string> GetBandsDnInfo();

private:
   double pixel_;
   double line_;
   DatasourceInterface* datasource_;
   World* pWorld_;
   MemoryCanvas* pCanvas_;
   Element* pElement_;
   /** Transformacion activa **/
   CoordinatesTransformation* pCt_;
   /** Obtiene un canvas para el elemento raster.*/
   MemoryCanvas* GetCanvas();
   /** Obtiene el elemento raster. */
   Element* GetElement();
   /** Metodo auxiliar que actualiza la transformacion **/
   void UpdateTransform();
};
} // namespace suri
#endif  // RASTERDNINFO_H_
