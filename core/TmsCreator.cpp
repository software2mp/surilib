/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
