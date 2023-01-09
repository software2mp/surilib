/*! \file WmtsXmlWriter.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WMTSXMLWRITER_H_
#define WMTSXMLWRITER_H_

// Includes Estandar
// Includes Suri
#include "TmsXmlWriter.h"

// Includes Wx
// Defines
// forwards

namespace suri {

class WmtsXmlWriter : public TmsXmlWriter {
public:
   /** Constructor */
   WmtsXmlWriter(TmsXmlWriter* pSuccessor);
   /** Destructor */
   virtual ~WmtsXmlWriter();
   /** Genera el XML correspondiente al servicio y basado en los parametros suministrados. */
   virtual std::string GetXml(const std::string& UrlStr,
                              std::map<std::string, std::string>& Params,
                              const Option& Metadata);
private:
   /** Valida el sistema de referencia ingresado, antes de escribirlo en el XML */
   bool validReferenceSystem(std::string referenceSystem);
   /** Obtiene el sistema de referencia por el que se debe reemplazar a uno no soportado */
   std::string correctReferenceSystem(std::string referenceSystem);
};

} /** namespace suri */

#endif /* WMTSXMLWRITER_H_ */
