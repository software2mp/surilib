/** \file SREEnhancementUtils.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SREENHANCEMENTUTILS_H_
#define SREENHANCEMENTUTILS_H_

#include <sstream>
#include <string>

#include "suri/xmlnames.h"
#include "suri/AuxiliaryFunctions.h"

#include "wx/xml/xml.h"

namespace suri {
namespace raster {
namespace enhancement {
namespace utils {

/**
 * Persiste toda la informacion necesaria para el uso del LUT en un XML.
 */
bool LutToXml(wxXmlNode*& pXmlNode, bool Active, int BandCount, std::string& LutName,
           int* pNumBins, double* pMin, double* pMax, int** pLut);

/**
 * Lee toda la informacion necesaria para el uso del LUT desde un XML.
 */
bool LutFromXml(wxXmlNode* pXmlNode, int& BandCount, int*& pNumBins, double*& pMin,
             double*& pMax, int**& pLut, bool& Active, std::string& Name);

}  // namespace utils
}  // namespace enhancement
}  // namespace raster
}  // namespace suri

#endif  // SREENHANCEMENTUTILS_H_
