/*! \file TmsXmlWriter.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TMSXMLWRITER_H_
#define TMSXMLWRITER_H_

#include <map>
#include <string>

namespace suri {

class Option;

/**
 * Clase base para la cadena de responsabilidad para la generacion
 * del XML para cada servicio TMS.
 */
class TmsXmlWriter {
public:
   /**
    * Ctor.
    */
   TmsXmlWriter(TmsXmlWriter* pSuccessor = NULL);

   /**
    * Dtor.
    */
   virtual ~TmsXmlWriter();

   /**
    * Genera el XML correspondiente al servicio y basado en los parametros suministrados.
    */
   virtual std::string GetXml(const std::string& UrlStr,
                              std::map<std::string, std::string>& Params,
                              const Option& Metadata)=0;

protected:
   /**
    * Obtiene al sucesor.
    */
   TmsXmlWriter* GetSuccessor();

private:
   TmsXmlWriter* pSuccessor_;
};

} /** namespace suri */

#endif  // TMSXMLWRITER_H_
