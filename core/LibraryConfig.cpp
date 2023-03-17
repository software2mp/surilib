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

// Includes Estandard
#include <string>

// Includes Suri
#include "LibraryConfig.h"
#include "LibraryConfigDefinition.h"
#include "LibraryConfigDefinitionOrigin.h"
#include "LibraryConfigDefinitionOriginSource.h"
#include "LibraryUtils.h"
#include "suri/Configuration.h"
#include "suri/AuxiliaryFunctions.h"

// Includes wx
#include "wx/wx.h"
#include "wx/xml/xml.h"
#include "wx/stdpaths.h"
#include "wx/filename.h"

namespace suri {

/** Definicion de nombres de nodos de configuracion de bibliotecas **/
std::string LibraryConfig::LibraryConfigNodeName = "libraries-config";
std::string LibraryConfig::LibrariesDefNodeName = "libraries-definitions";
std::string LibraryConfig::LibraryDefNodeName = "library-definition";
std::string LibraryConfig::LibraryOriginsNodeName = "origins";
std::string LibraryConfig::LibraryOriginNodeName = "origin";
std::string LibraryConfig::LibraryFavIndexNodeName = "favorite-index";
std::string LibraryConfig::LibrarySourceProperty = "source";
std::string LibraryConfig::LibraryPathProperty = "path";
std::string LibraryConfig::LibraryTypeProperty = "type";
std::string LibraryConfig::LibraryNameProperty = "name";
std::string LibraryConfig::LibraryItemTypeProperty = "itemtype";
std::string LibraryConfig::LibraryCapabilitiesProperty = "capabilities";
std::string LibraryConfig::LibraryLevelProperty = "level";
std::string LibraryConfig::LibraryConfigFileName = "libraries.xml";


/**
 * Ctor.
 */
LibraryConfig::LibraryConfig() {
}

/**
 * Dtor.
 */
LibraryConfig::~LibraryConfig() {
}

/**
 * Devuelve el LibraryConfigDefinition correspondiente la nombre dado.
 */
LibraryConfigDefinition* LibraryConfig::GetDefinitionByName(
      const std::string& DefName) {

   LibraryConfigDefinition* pretDef = NULL;
   wxXmlNode* plibsconf = suri::Configuration::GetParameterNode(LibraryConfigNodeName,
                                                                NULL);
   if (plibsconf != NULL) {
      wxXmlNode* plibsdefs = plibsconf->GetChildren();
      while (plibsdefs != NULL) {
         if (plibsdefs->GetName().CompareTo(LibrariesDefNodeName.c_str()) == 0) {
            wxXmlNode* plibdef = plibsdefs->GetChildren();
            while (plibdef != NULL) {
               wxString propval;
               if (plibdef->GetName().CompareTo(LibraryDefNodeName.c_str()) == 0
                     && plibdef->GetPropVal(LibraryNameProperty.c_str(), &propval)) {
                  if (propval == wxString(DefName.c_str())) {
                     pretDef = new LibraryConfigDefinition(DefName);
                     wxXmlNode* porigins = plibdef->GetChildren();
                     while (porigins != NULL) {
                        if (porigins->GetName().CompareTo(
                              LibraryOriginsNodeName.c_str()) == 0) {
                           LoadOrigins(pretDef, porigins);
                        } else if (porigins->GetName().CompareTo(
                              LibraryFavIndexNodeName.c_str()) == 0) {
                           LoadFavorites(pretDef, porigins);
                        }
                        porigins = porigins->GetNext();
                     }
                  }
               }
               plibdef = plibdef->GetNext();
            }
         }
         plibsdefs = plibsdefs->GetNext();
      }
   }
   return pretDef;
}

/** Carga los origenes que contiene el nodo <origin> **/
void LibraryConfig::LoadOrigins(LibraryConfigDefinition* pLibConfigDef,
                                wxXmlNode* pOriginsNode) const {
   wxString propval;
   wxXmlNode* porigin = pOriginsNode->GetChildren();
   while (porigin != NULL) {
      if (porigin->GetName().CompareTo(LibraryOriginNodeName.c_str()) == 0) {
         LibraryConfigDefinitionOrigin* pdeforigin =
               new LibraryConfigDefinitionOrigin();
         if (porigin->GetPropVal(LibraryNameProperty.c_str(), &propval)) {
            pdeforigin->SetName(propval.c_str());
         }
         if (porigin->GetPropVal(LibraryCapabilitiesProperty.c_str(), &propval)) {
            pdeforigin->SetCapabilities(StringToNumber<int>(propval.c_str()));
         }
         if (porigin->GetPropVal(LibraryLevelProperty.c_str(), &propval)) {
            pdeforigin->SetLevel(StringToNumber<int>(propval.c_str()));
         }
         if (porigin->GetPropVal(LibrarySourceProperty.c_str(), &propval)) {
            LibraryConfigDefinitionOriginSource* psource =
                  new LibraryConfigDefinitionOriginSource();
            std::vector<std::string> props = split<std::string>(propval.c_str(), ";");
            bool pathdefine = false;
            for (int ix = 0, lenix = props.size(); ix < lenix; ++ix) {
               std::vector<std::string> kv = split<std::string>(props[ix], "=");
               if (kv[0].compare(LibraryNameProperty) == 0) {
                  psource->SetName(kv[1]);
               } else if (kv[0].compare(LibraryTypeProperty) == 0) {
                  psource->SetType(kv[1]);
               } else if (kv[0].compare(LibraryPathProperty) == 0) {
                  psource->SetPath(kv[1]);
                  pathdefine = true;
               } else if (kv[0].compare(LibraryItemTypeProperty) == 0) {
                  psource->SetItemType(StringToNumber<int>(kv[1]));
               }
            }
            if (!pathdefine) {
               wxString userpath = wxStandardPaths().GetUserDataDir();
               if (!wxFileName::DirExists(userpath)) {
                  wxFileName::Mkdir(userpath, 0777, wxPATH_MKDIR_FULL);
               }
               userpath.Append(wxFileName::GetPathSeparator());
               psource->SetPath(userpath.c_str());
            }
            pdeforigin->SetSource(psource);
         }
         pLibConfigDef->AddOrigin(pdeforigin);
      }
      porigin = porigin->GetNext();
   }
}

/** Carga los favoritos que se encuentran configurados en el indice
 *  de favoritos.
 *  Ejemplo de definicion de indice de favoritos
 *  <favorite-index source="name=srsfavindex.fidx;type=textfile;path=./data">
 */
void LibraryConfig::LoadFavorites(LibraryConfigDefinition* pLibConfigDef,
                                  wxXmlNode* pFavIndexNode) const {
   wxString filename, type, propval;
   std::string frompath = Configuration::GetParameter("app_data_dir", wxT("./"))
         + "libraries/";
   std::string topath;

   if (pFavIndexNode->GetPropVal(LibrarySourceProperty.c_str(), &propval)) {
      std::string filename;
      std::vector<std::string> props = split<std::string>(propval.c_str(), ";");
      for (int ix = 0, lenix = props.size(); ix < lenix; ++ix) {
         std::vector<std::string> kv = split<std::string>(props[ix], "=");
         if (kv[0].compare(LibraryNameProperty) == 0) {
            filename = kv[1];
         } else if (kv[0].compare(LibraryTypeProperty) == 0) {
         } else if (kv[0].compare(LibraryPathProperty) == 0) {
            topath = ResolvePath(kv[1]);
         }
      }

      // La idea seria para el caso de favoritos que ya vienen precargados
      // que el archivo original se copie a un directorio del usuario para
      // que el mismo tenga permisos de escritura y pueda ser modificado.
      // Si no se especifico ninguno entonces por defecto se pone el
      // directorio de datos del usuario actual.
      std::string tofile;
      if (!topath.empty()) {
         tofile = topath + filename;
      } else {
         tofile = ResolvePath(std::string(DIR_USR_DATA) + std::string("/data/libraries"))
               + filename;
      }
      EnsureFilePath(tofile);
      if (!wxFileExists(tofile))
         wxCopyFile(frompath + filename, tofile, false);
      pLibConfigDef->SetFavoritesIndexFileName(tofile);
   }
}

} // namespace suri
