/*! \file SRSHSVSharpening.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SRSHSVSHARPENING_H_
#define SRSHSVSHARPENING_H_

#include "SRSSharpening.h"
#include "suri/ParameterCollection.h"

#include "gdal.h"

namespace suri {
namespace raster {
namespace sharpening {

/**
 * Clase que implementa el metodo de fusion de bandas con pancromatica por HSV.
 */
class HSVSharpening : public Sharpening {
public:
   static const int kDestBandCount = 3;

   /**
    * Ctor.
    */
   HSVSharpening(const std::string& SrcFilename, const std::string& PanFilename,
                 const std::string& DestFilename, const std::vector<int>& SrcBands,
                 int PanBand);

   /**
    * Dtor.
    */
   virtual ~HSVSharpening();

   /**
    * Implementa el procedimiento de fusion de bandas con pancromatica por HSV.
    */
   virtual void Transform(CBSHARPENINGPROGRESS ProgressCB = NULL, void* pProgressCBData = NULL);

   /**
    * Crea una instancia de HSVSharpening.
    */
   static Sharpening* Create(suri::ParameterCollection& Params);

private:
   /**
    * Copia informacion de proyeccion, georreferenciacion, metadatos, etc
    * desde el dataset de origen al de destino.
    */
   void CopyStandardData(GDALDatasetH SrcDs, GDALDatasetH DestDs);
};

}  // namespace sharpening
}  // namespace raster
}  // namespace suri

#endif /* SRSHSVSHARPENING_H_ */
