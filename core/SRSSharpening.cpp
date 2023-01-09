/*! \file SRSSharpening.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "SRSSharpening.h"

namespace suri {
namespace raster {
namespace sharpening {

/**
 * Ctor.
 */
Sharpening::Sharpening(const std::string& SrcFilename, const std::string& PanFilename,
                       const std::string& DestFilename,
                       const std::vector<int>& SrcBands, int PanBand) :
      srcFilename_(SrcFilename), panFilename_(PanFilename), destFilename_(DestFilename),
      srcBands_(SrcBands), panBand_(PanBand) {

}

/**
 * Dtor.
 */
Sharpening::~Sharpening() {

}

}  // namespace sharpening
}  // namespace raster
}  // namespace suri
