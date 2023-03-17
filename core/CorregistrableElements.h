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

#ifndef CORREGISTRABLEELEMENTS_H
#define CORREGISTRABLEELEMENTS_H

// Includes Estandar
#include <string>
// Includes Suri
#include "suri/Subset.h"
#include "suri/GcpList.h"
// Includes Wx
// Defines
// forwards

namespace suri{

class RasterElement;

class CorregistrableElements{
public:
   /**
    * Constructor
    */
   CorregistrableElements(RasterElement* pBase, RasterElement* pWarp, RasterElement* pCandidates,
                          const std::string& seedGcpPath);
   /**
    * Constructor
    */
   CorregistrableElements(RasterElement* pBase, RasterElement* pWarp, RasterElement* pCandidates,
                          const std::string& seedGcpPath, const std::vector<int>& BaseBands,
                          const std::vector<int>& WarpBands);
   /**
    * Destructor
    */
   ~CorregistrableElements();
   /**
    * Crea para el par de imagenes un par corregistrable donde se recorta y
    * redimensionan la imagen base y warp para que contengan solo la parte que se
    * solapa entre ellos
    */
   void CreateCorregistrableElements();
   /**
    * Devuelve el raster base escalado y recortado
    */
   RasterElement* GetCorregistrableBase();
   /**
    * Devuelve el raster warp escalado y recortado
    */
   RasterElement* GetCorregistrableWarp();
   /**
    * Devuelve los candidatos de la base escalados y recortados
    */
   RasterElement* GetCorregistrableCandidates();
   /**
    * Devuelve los gcps usados como semilla
    */
   GcpList GetSeedGcps();
private:
   /**
    * Inicializa paths y elementos
    */
   void InitializeElements();
   /**
    * Crea una copia del warp corregistrada en el sistema de referencia de la base
    */
   void CreateCorregisteredWarp();
   /**
    * Recarga los elementos raster
    */
   void RefreshRasterElements();
   /**
    * Devuelve el extente con la interseccion entre la base y warp
    */
   Subset GetIntersection(RasterElement* base, RasterElement* warp);
   RasterElement* originalBase_;
   RasterElement* originalCandidates_;
   RasterElement* originalWarp_;
   // Versiones ajustadas y escaladas de las imágenes originales
   RasterElement* corregistrableBase_;
   RasterElement* corregistrableCandidates_;
   RasterElement* corregistrableWarp_;
   // Gcps para hacer una reproyección grosera
   GcpList gcpList;
   // Bandas que serán renderizadas en las versiones corregistrables
   std::vector<int> baseBands, warpBands, candidateBands;
   // Archivo temporal con el warp corregistrado groseramente
   std::string tempFilePath_, corregisteredWarpPath_;
   std::string corregistrableBasePath_, corregistrableWarpPath_, corrCandidatesPath_;
};

} // namespace suri

#endif // CORREGISTRABLEELEMENTS_H
