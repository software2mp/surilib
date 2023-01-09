/*! \file SRSSharpening.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SRSSHARPENING_H_
#define SRSSHARPENING_H_

#include <string>
#include <vector>

namespace suri {
namespace raster {
namespace sharpening {

typedef int (*CBSHARPENINGPROGRESS)(double Complete, const char* pMessage,
                                    void* pProgressArg);

/**
 * Clase base para los distintos metodos de fusion de bandas con pancromatica.
 */
class Sharpening {
public:
   /**
    * Ctor.
    */
   Sharpening(const std::string& SrcFilename, const std::string& PanFilename,
              const std::string& DestFilename, const std::vector<int>& SrcBands,
              int PanBand);

   /**
    * Dtor.
    */
   virtual ~Sharpening();

   /**
    * Metodo a implementar en cada clase hija para ejecutar el
    * procedimiento de fusion de bandas con pancromatica.
    */
   virtual void Transform(CBSHARPENINGPROGRESS ProgressCB = NULL,
                          void* pProgressCBData = NULL) = 0;

protected:
   std::string srcFilename_;
   std::string panFilename_;
   std::string destFilename_;
   std::vector<int> srcBands_;
   int panBand_;
};

}  // namespace sharpening
}  // namespace raster
}  // namespace suri

#endif  // SRSSHARPENING_H_
