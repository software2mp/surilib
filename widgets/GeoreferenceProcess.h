/** \file GeoreferenceProcess.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GEOREFERENCEPROCESS_H_
#define GEOREFERENCEPROCESS_H_

#include <string>
#include <vector>

#include "suri/ProcessInterface.h"
#include "suri/Element.h"
#include "suri/Subset.h"
#include "suri/DataViewManager.h"
#include "ReprojectionProcess.h"
#include "BandInfo.h"
#include "suri/Subset.h"

#include "gdal_priv.h"

namespace suri {

class GeoreferenceProcess : public ReprojectionProcess {
public:
   /** Nombre del proceso **/
   static const std::string kProcessName;
   static const std::string kOutputElementType;

   /**
    * Ctor.
    */
   GeoreferenceProcess(Element* pInputElement, const Subset& ViewportSubset,
                       Element*& pOutputElement, const SuriObject::UuidType& LibraryId,
                       DataViewManager* pDataViewManager, bool AskGcpFile = true);

   /**
    * Dtor.
    */
   virtual ~GeoreferenceProcess();

   /**
    * Corre el proceso y genera la salida usando los metodos de configuracion.
    */
   virtual bool RunProcess();

   /**
    * Permite configurar la salida del proceso antes de correrlo.
    */
   virtual bool ConfigureProcess();

   /**
    * Una vez ejecutado el proceso hace visible la salida.
    */
   virtual bool ConfigureOutput();

protected:
   Element* pInputElement_;
   Subset viewportSubset_;
   Element*& pOutputElement_;
   SuriObject::UuidType libraryId_;
   DataViewManager* pDataViewManager_;

   /**
    * Copia informacion de una banda.
    */
   static void CopyBandInfo(GDALRasterBand * poSrcBand, GDALRasterBand * poDstBand,
                            int bCanCopyStatsMetadata, int bCopyScale, int bCopyNoData);

   /**
    * Gdal translate.
    */
   bool Translate(const std::string& SrcFile, const std::string& DestFile,
                  const std::string& Format, const GDAL_GCP* pGCPs, int GCPsCount,
                  const std::vector<BandInfo>& Bands, int Pixels, int Lines,
                  const Subset& GeoBounds, void* pProgressData = NULL);

   /**
    * Crea el archivo de salida.
    */
   static GDALDatasetH GDALWarpCreateOutput(GDALDatasetH hSrcDS,
                                            const char *pszFilename,
                                            const char *pszFormat,
                                            const char *pszSourceSRS,
                                            const char *pszTargetSRS, bool UseGCPs,
                                            int nOrder, double PixelWidth,
                                            double PixelHeight,
                                            char **papszCreateOptions);

   /**
    * Gdal warp.
    */
   bool Warp(const std::string& SrcFile, const std::string& DestFile,
             const std::string& Format, const std::string& InWkt, bool UseGCPs,
             int Order, double PixelWidth = 0.0, double PixelHeight = 0.0,
             void* pProgressData = NULL);

   /**
    * Obtiene los puntos de control del archivo especificado junto con la cantidad
    * y SRS.
    */
   void GetGdalGcps(GDAL_GCP*& pGdalGcps, int& Count, std::string& SrcWkt);

   /**
    * Libera la memoria reservada para los puntos de control.
    */
   void ReleaseGdalGcps(GDAL_GCP* pGdalGcps, int Count);   
   /** 
    * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
    * agreguen sus atributos especificos
    * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
    * @return vector vacio en caso de que no tenga parametros adicionales
   **/
   virtual std::vector<wxXmlNode*> DoGetXmlParameters() const; 
};

}  // namespace suri

#endif  // GEOREFERENCEPROCESS_H_
