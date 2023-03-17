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

#ifndef GCP_GENERATION_STRATEGY_H
#define GCP_GENERATION_STRATEGY_H

// Includes Estandar
#include <string>
#include <vector>
// Includes Suri
#include "GcpList.h"
#include "CorregistrableElements.h"
// Includes Wx
// Defines
// forwards

namespace suri {

class Image;
class RasterElement;
class CoordinatesTransformation;

class GcpGenerationStrategy{
public:
   /**
    * Constructor
    */
   GcpGenerationStrategy(RasterElement* pBaseElement, RasterElement* pBaseCandidates,
                         RasterElement* pWarpElement, const std::string& seedGcpPath);
   /**
    * Destructor
    */
   ~GcpGenerationStrategy();
   /**
    * Devuelve una referencia a la lista de GCPs generados en el proceso.
    */
   GcpList& RunProcess();
   /**
    * Indica el nivel de agresividad del algoritmo.
    */
   void SetAggressionLevel(int AgressionLevel);
   /**
    * Indica la banda de la imagen base con la que corregistro
    */
   void SetBaseBands(const std::vector<int>& BaseBands);
   /**
    * Indica la banda de la imagen warp con la que corregistro
    */
   void SetWarpBands(const std::vector<int>& WarpBands);

private:
   /**
    * Busca un par de gcps a partir de las coordenadas del gcp candidato.
    */
   Image *pBaseCandidates_, *pCorregistrableBase_, *pCorregistrableWarp_;
   RasterElement *pOriginalBase_, *pOriginalWarp_;
   CorregistrableElements elements;
   GcpList gcpList_;
   /**
    * Nivel de agresividad del algoritmo, que se usa para ajustar algunas variables
    * del proceso y regular la cantidad de gcps generados.
    */
   int aggressionLevel_;
   /**
    * Bandas a corregistrar
    */
   std::vector<int> baseBands_, warpBands_;
   /**
    * Asigno modelo raster, sistema de referencia, etc.
    */
   void InitializeGcpList(const RasterElement* pBaseElement, const RasterElement* pWarpElement);
   /**
    * Crea un transformador de coordenadas
    */
   void InitializeTransformer(const GcpList& SeedGcps);
   /**
    * Transforma las coordenadas del warp corregistrable a las de la imagen original
    */
   CoordinatesTransformation* pCoordTransform;
   /** Orden de la transformacion **/
   static const int kGcpTransformOrder;
};

} // namespace suri
#endif // GCP_GENERATION_STRATEGY_H
