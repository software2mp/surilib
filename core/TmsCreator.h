/*! \file TmsElement.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TMSCREATOR_H_
#define TMSCREATOR_H_

#include <map>
#include <string>

namespace suri {

class RasterElement;
class Option;

/**
 * Clase que representa un servicio TMS.
 */
class TmsCreator {
public:
   /**
    * Dtor.
    */
   virtual ~TmsCreator() { }

   /**
    * Crea un raster a partir de una configuracion TMS.
    */
   static RasterElement* Create(const std::string& Filename, const Option& Metadata);

   /**
    * Parsea url y retorna el host y los parametros en mapa.
    */
   static bool ParseTmsData(const std::string &Filename,
                            std::map<std::string, std::string>& Keys);

private:
   /**
    * Ctor.
    */
   TmsCreator() { }

   /**
    * Genera xml que requiere GDAL para abrir una imagen.
    */
   static std::string CreateTmsXml(const std::string& FileName,
                                   std::map<std::string, std::string>& Keys,
                                   const Option& Metadata);
};

}  // namespace suri

#endif  // TMSCREATOR_H_
