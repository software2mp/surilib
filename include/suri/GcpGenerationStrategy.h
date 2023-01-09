/*! \file GcpGenerationStrategy.h */                                                     
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
*/ 

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
