/*! \file TmsCreator.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <fstream>
#include <map>
#include <sstream>
#include <string>

#include "TmsCreator.h"
#include "TmsXmlWriter.h"
#include "TmsXmlWriterBuilder.h"

#include "suri/AuxiliaryFunctions.h"
#include "suri/DataTypes.h"
#include "suri/Image.h"
#include "suri/RasterElement.h"
#include "suri/Uri.h"
#include "suri/Option.h"

#include "wx/filefn.h"
#include "wx/filename.h"

#define TMS_FILENAME_SEPARATOR "|"

namespace suri {

/**
 * Crea un raster a partir de una configuracion TMS.
 */
RasterElement* TmsCreator::Create(const std::string& Filename, const Option& Metadata) {
   std::map<std::string, std::string> keys;
   ParseTmsData(Filename, keys);

   if (Filename.find("http://") != std::string::npos
         && (keys["service"].compare("tms") == 0 || keys["service"].compare("wmts") == 0)) {
      std::string tmsxml = CreateTmsXml(Filename, keys, Metadata);
      if (tmsxml.length() > 0) {
         std::string tempfn = wxFileName::CreateTempFileName(_("")).c_str();

         std::ofstream xmlfile;
         xmlfile.open(tempfn.c_str());
         xmlfile << tmsxml;
         xmlfile.close();

         wxFileName filename(tempfn);
         filename.SetExt("xml");
         std::string finalfn = filename.GetFullPath().c_str();

         wxRenameFile(tempfn, finalfn);

         RasterElement* praster = dynamic_cast<RasterElement*>(Element::Create(
               "RasterElement", finalfn));
         praster->SetName(keys["expdesc"]);

         return praster;
      }
   }

   return NULL;
}

/**
 * Parsea url y retorna el host y los parametros en mapa(key=>value)
 * @param[in] Url: string con url que quiero parsear.
 * @param[out] Keys: mapa con pares key,value leidos.
 * @return bool que indica si el FileName tiene un formato correcto
 */
bool TmsCreator::ParseTmsData(const std::string &Filename,
                              std::map<std::string, std::string>& Keys) {
   size_t enduri = Filename.find(TMS_FILENAME_SEPARATOR);

   //Configuro uri y sus componentes
   Uri uri(Filename.substr(0, enduri));
   std::map<std::string, std::string> queryparameters = StringToMap(
         uri.GetQuery(), Uri::QUERY_SEPARATOR);

   if (queryparameters.count("service") == 0 || queryparameters.count("layers") == 0)
      return false;

   Keys["wxssource"] = uri.ToString();
   Keys["service"] = queryparameters["service"];
   std::string datasource;
   uri.GetDatasource(datasource);
   Keys["url"] = datasource;
   Keys["layers"] = queryparameters["layers"];

   if (enduri == std::string::npos) return true;

   //Configuro en mapa el extent de la fuente wxs
   std::map<std::string, std::string> extentparameters = suri::StringToMap(
         Filename.substr(enduri + 1), TMS_FILENAME_SEPARATOR);

   std::map<std::string, std::string>::iterator it = extentparameters.begin();
   for (; it != extentparameters.end(); it++)
      Keys[it->first] = it->second;

   return true;
}

/**
 * Genera xml que requiere GDAL para abrir una imagen.
 */
std::string TmsCreator::CreateTmsXml(const std::string& FileName,
                                     std::map<std::string, std::string>& Keys,
                                     const Option& Metadata) {
   std::string retval = "";

   TmsXmlWriter* pwriter = TmsXmlWriterBuilder::Build();
   if (pwriter != NULL) {
      retval = pwriter->GetXml(FileName, Keys, Metadata);
      TmsXmlWriterBuilder::Release(pwriter);
   }

   return retval;
}

}  // namespace suri
