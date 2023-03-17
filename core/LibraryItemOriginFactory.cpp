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

#include <cstring>
#include <string>

#include "GdalSrsItemOrigin.h"
#include "LibraryItemOriginFactory.h"
#include "LibraryConfigDefinitionOrigin.h"
#include "LibraryConfigDefinitionOriginSource.h"
#include "LibraryUtils.h"
#include "suri/Configuration.h"
#include "suri/Library.h"
#include "suri/LibraryItemOrigin.h"
#include "suri/TextFileLibraryItemOrigin.h"

#include <wx/stdpaths.h>
#include <wx/strconv.h>
#include <wx/filename.h>

namespace suri {

/**
 * Crea una nueva instancia de LibraryItemOrigin basado en la definicion del origen.
 */
LibraryItemOrigin* LibraryItemOriginFactory::Create(
      Library* pLibrary, const LibraryConfigDefinitionOrigin* pOrigin,
      const std::string& FavoritesIndexFileName) {

   LibraryItemOrigin* pretorigin = NULL;

   if (pOrigin != NULL) {

      std::string type = pOrigin->GetSource()->GetType();
      if (type.compare(LibraryConfigDefinitionOriginSource::kTypeTextfile) == 0) {
         pretorigin = CreateTextfile(pLibrary, pOrigin, FavoritesIndexFileName);
      } else if (type.compare(LibraryConfigDefinitionOriginSource::kTypeGdal) == 0) {
         pretorigin = CreateGdal(pLibrary, pOrigin, FavoritesIndexFileName);
      }
   }
   return pretorigin;
}

/**
 * Crea una nueva instancia de LibraryItemOrigin de tipo texto plano.
 */
LibraryItemOrigin* LibraryItemOriginFactory::CreateTextfile(
      Library* pLibrary, const LibraryConfigDefinitionOrigin* pOrigin,
      const std::string& FavoritesIndexFileName) {
   //
   std::string filename = pOrigin->GetSource()->GetName();
   std::string filepath = ResolvePath(pOrigin->GetSource()->GetPath());
   std::string fullpath = filepath + filename;

   //
   LibraryItemOrigin::OriginCategoryType category = LibraryItemOrigin::NONE;
   if (pOrigin->GetLevel() == LibraryConfigDefinitionOrigin::kLevelSystem)
      category = LibraryItemOrigin::SYSTEM;
   else if (pOrigin->GetLevel() == LibraryConfigDefinitionOrigin::kLevelUser)
      category = LibraryItemOrigin::USER;

   //
   std::string originname = pOrigin->GetName();

   //
   std::vector<LibraryItemOrigin::CapabilityType> capabilites;
   if (pOrigin->GetCapabilities() & LibraryConfigDefinitionOrigin::kCapabilitiesRead) {
      capabilites.push_back(LibraryItemOrigin::READ);
   }

   if (pOrigin->GetCapabilities() & LibraryConfigDefinitionOrigin::kCapabilitiesWrite) {
      capabilites.push_back(LibraryItemOrigin::WRITE);
      capabilites.push_back(LibraryItemOrigin::APPEND);
   }

   return new TextFileLibraryItemOrigin(pLibrary->GetId(), capabilites, fullpath,
                                        pOrigin->GetName(), category,
                                        FavoritesIndexFileName);
}

/**
 * Crea una nueva instancia de LibraryItemOrigin para carga de datos desde gdal.
 */
LibraryItemOrigin* LibraryItemOriginFactory::CreateGdal(
      Library* pLibrary, const LibraryConfigDefinitionOrigin* pOrigin,
      const std::string& FavoritesIndexFileName) {
   GdalSrsItemOrigin::SrsItemType itemType;

   switch (pOrigin->GetSource()->GetItemType()) {
      case 0:
         itemType = GdalSrsItemOrigin::PROJECTED_SRS;
         break;
      case 1:
         itemType = GdalSrsItemOrigin::GEOGRAFIC_SRS;
         break;
      case 2:
         itemType = GdalSrsItemOrigin::DATUM;
         break;
      case 3:
         itemType = GdalSrsItemOrigin::ELLIPSOID;
         break;
      default:
         itemType = GdalSrsItemOrigin::UNKNOWN;
         break;
   }

   return new GdalSrsItemOrigin(pLibrary->GetId(), itemType, FavoritesIndexFileName);
}

} // namespace suri
