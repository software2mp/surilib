/*! \file EntropyResult.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "EntropyResult.h"

// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

/**
 * Destructor
 */
EntropyResult::~EntropyResult() {}

/**
 * Constructor
 */
EntropyResult::EntropyResult(double entropy, int ulx, int uly, int lrx, int lry):
      entropy_(entropy), subset_(ulx, uly, lrx, lry) {}

wxXmlNode* EntropyResult::SerializeXml() const{
   return 0;
}

} /** namespace suri */
