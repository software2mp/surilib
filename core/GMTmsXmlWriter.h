/*! \file GMTmsXmlWriter.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GMTMSXMLWRITER_H_
#define GMTMSXMLWRITER_H_

#include "TmsXmlWriter.h"

namespace suri {

/**
 * Genera el XML particular para Google Maps.
 */
class GMTmsXmlWriter : public TmsXmlWriter {
public:
   /**
    * Ctor.
    */
   GMTmsXmlWriter(TmsXmlWriter* pSuccessor = NULL);

   /**
    * Dtor.
    */
   virtual ~GMTmsXmlWriter();

   /**
    * Genera el XML correspondiente al servicio y basado en los parametros suministrados.
    */
   virtual std::string GetXml(const std::string& UrlStr,
                              std::map<std::string, std::string>& Params,
                              const Option& Metadata);

private:
   /**
    * Obtiene el ID de la capa en base al nombre de la misma.
    * map = m
    * satellite = s
    * hybrid = y
    * terrain = t
    * terrain, streets and water = p
    */
   static std::string GetLayerId(const std::string& LayerName);

   /**
    * Obtiene la descripcion de la capa en base al nombre de la misma.
    */
   static std::string GetLayerDescription(const std::string& LayerName);
};

}  // namespace suri

#endif  // GMTMSXMLWRITER_H_
