/*! \file TmsXmlWriter.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <map>
#include <string>

#include "TmsXmlWriter.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Uri.h"

namespace suri {

/**
 * Ctor.
 */
TmsXmlWriter::TmsXmlWriter(TmsXmlWriter* pSuccessor) : pSuccessor_(pSuccessor) {

}

/**
 * Dtor.
 */
TmsXmlWriter::~TmsXmlWriter() {
   if (pSuccessor_ != NULL) {
      delete pSuccessor_;
      pSuccessor_ = NULL;
   }
}

/**
 * Obtiene al sucesor.
 */
TmsXmlWriter* TmsXmlWriter::GetSuccessor() {
   return pSuccessor_;
}

}  // namespace suri
