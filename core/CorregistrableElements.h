/*
 * Biblioteca de procesamiento GeoEspacial SuriLib.
 * Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 * Este producto contiene software desarrollado por
 * SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
