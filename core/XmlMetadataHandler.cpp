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

// Includes Estandar
// Includes Suri
#include "XmlMetadataHandler.h"
#include "suri/xmlnames.h"

// Includes Wx
#include "wx/file.h"
#include "wx/filename.h"
#include "wx/xml/xml.h"

// Defines
// forwards

namespace suri {

/**
 * Constructor
 * @param[in] Xml del elemento
 * @param[in] OutputFilename path donde debe guardarse el xml
 */
XmlMetadataHandler::XmlMetadataHandler() {
}

/** Desstructor */
XmlMetadataHandler::~XmlMetadataHandler() {
}

/**
 * Realiza la lectura de un archivo xml de metadatos
 * @param[in] pDatasource fuente de datos que posee el elemento donde
 * @return true en caso de que exista el archivo con metadatos y pueda abrirlo, caso contrario false
 */
bool XmlMetadataHandler::ImportMetadata(DatasourceInterface* pDatasource) {
   wxFileName xmlinput(wxT(pDatasource->GetUrl()));
   xmlinput.SetExt(wxT(extension_XML));
   wxXmlDocument doc;
   if (!xmlinput.FileExists() || !doc.Load(xmlinput.GetFullPath()))
       return false;
   wxXmlNode* pchild = doc.GetRoot()->GetChildren();
   wxXmlNode* pparent = pDatasource->GetElement()->GetNode();
   while (pchild != NULL) {
      if (pchild->GetName() != URL_NODE || pchild->GetName() != DEPENDENCY_NODE
            || pchild->GetName() != NAME_NODE)
         pDatasource->GetElement()->AddNode(pparent, new wxXmlNode(*pchild), true);
      pchild = pchild->GetNext();
   }
   return true;
}

/**
 * Realiza la escritura de los metadatos en formato xml
 * @param[in] pDatasource fuente de datos que posee el elemento con los metadatos a exportar
 * @return true en caso de que pueda abrir y escribir el archivo, caso contrario false
 */
bool XmlMetadataHandler::ExportMetadata(DatasourceInterface* pDatasource, std::string Filename) {
   wxFile wxfile;
   wxFileName xmloutput(wxT(Filename));
   xmloutput.SetExt(wxT(extension_XML));
   wxString path = xmloutput.GetFullPath();
   if (!wxfile.Open(path, wxFile::write))
      return false;
   return wxfile.Write(pDatasource->GetElement()->GetXmlAsString());
}
} /** namespace suri */
