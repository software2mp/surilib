/*! \file GMGetCapabilitiesParser.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GMGETCAPABILITIESPARSER_H_
#define GMGETCAPABILITIESPARSER_H_

#include "WxsGetCapabilities.h"

namespace suri {

/**
 * Clase que representa un parser para el servicio TMS de Google Maps.
 */
class GMGetCapabilitiesParser : public WxsGetCapabilitiesParser {
public:
   /**
    * Ctor.
    */
   GMGetCapabilitiesParser();

   /**
    * Dtor.
    */
   virtual ~GMGetCapabilitiesParser();

protected:
   /**
    * Metodo que realiza el parseo de GetCapabilities.
    *
    * @return true en caso de realizar el parseo con exito.
    * @return false en caso de no poder realizar el parseo.
    */
   virtual bool DoParseGetCapabilities(
         const std::string& WxsUrl, WxsCapabilitesInformation& GetCapabilitesResponse,
         std::string& ServiceType, const std::string& Version = "1.1.1");

private:
   /**
    * Crea una capa para el servicio.
    */
   void CreateLayer(const std::string& Name, const std::string& Title,
                    WxsLayersInformation::WxsLayerNode& TmsLayer);
};

}  // namespace suri

#endif  // GMGETCAPABILITIESPARSER_H_
