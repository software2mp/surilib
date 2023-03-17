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

// Includes Suri
#include "suri/MultimediaHtmlRepresentationGetter.h"
#include "suri/Element.h"
#include "MultimediaElement.h"
#include "suri/Configuration.h"

namespace suri {

/** ctor default **/
MultimediaHtmlRepresentationGetter::MultimediaHtmlRepresentationGetter() {
}
/** dtor */
MultimediaHtmlRepresentationGetter::~MultimediaHtmlRepresentationGetter() {
}

/**
 * Objetivo: Verifica que exista el directorio donde se encuentran los templates
 * html para visualizar la informacion del PAT.
 * En caso de que el directorio no exista lo crea y extrae los templates.
 * @param[in]	Path: path del directorio a verificar.
 * @return si existe o si puede crearlo retorna true; otro caso false.
 */
bool MultimediaHtmlRepresentationGetter::CheckDataDirectory(const wxString &Path) const {
   wxFileName fn(Path);

   if (!fn.Normalize())
   REPORT_AND_FAIL_VALUE("D:Falla normalizacion", false);
   if (!fn.IsDir())
      REPORT_AND_FAIL_VALUE("D:%s no es un directorio valido", false,
                            fn.GetFullPath().c_str());
   if (!fn.DirExists())
   //Trata de crear el directorio. Si ya existe no hace nada.
      if (!fn.Mkdir(fn.GetFullPath().c_str(), 0755, wxPATH_MKDIR_FULL))
      REPORT_AND_FAIL_VALUE("D:Error al intentar crear directorio de templates", false);
   //Verifico que exista el template html.
   if (!wxFileName::FileExists(Path + "imagen.html")) {
      SHOW_ERROR(message_HTML_FILES_NOT_FOUND);
      return false;
   }
   return true;
}
/** Metodo polimorfico encargado de obtener la representacion html de la capa que se
 *  encuentra por parametro.
 * @param[in] pElement elemento sobre el que se obtendra su representacion html
 * @param[out] Representation representacion en formato html del elemento
 * @return true en caso de poder obtener correctamente la interpretacion html del elemento
 * @return false en caso contrario 
**/
bool MultimediaHtmlRepresentationGetter::DoGetRepresentation(const Element* pElement,
                                       std::string& Representation) const {
   if (!dynamic_cast<const MultimediaElement*>(pElement))
      return false;
   wxString html = wxT("");
   std::string htmlphotopath = suri::Configuration::GetParameter(wxT("app_data_dir"),
                                                                 wxT("./")) + "fotografia/";
   const MultimediaElement* pphoto = dynamic_cast<const MultimediaElement*>(pElement);
   if (!CheckDataDirectory(htmlphotopath) || !pphoto->GetHtmlInfo(html))  
      return false;
   Representation = html.c_str();
   return true;
}

}  /** namespace suri */
