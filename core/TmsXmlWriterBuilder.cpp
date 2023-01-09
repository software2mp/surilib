/*! \file TmsXmlWriterBuilder.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "TmsXmlWriterBuilder.h"

#include "GMTmsXmlWriter.h"
#include "TmsXmlWriter.h"
#include "WmtsXmlWriter.h"

namespace suri {

/**
 * Construye la cadena de reponsabilidad con todas las TmsXmlWriter disponibles.
 */
TmsXmlWriter* TmsXmlWriterBuilder::Build() {
   TmsXmlWriter* ptmsxmlwriter = new GMTmsXmlWriter(NULL);
   ptmsxmlwriter = new WmtsXmlWriter(ptmsxmlwriter);
   return ptmsxmlwriter;
}

/**
 * Libera la memoria reservada por la cadena (metodo utilizado por el codigo cliente).
 */
void TmsXmlWriterBuilder::Release(TmsXmlWriter*& pTmsXmlWriter) {
   if (pTmsXmlWriter != NULL) {
      delete pTmsXmlWriter;
      pTmsXmlWriter = NULL;
   }
}

}  // namespace suri
