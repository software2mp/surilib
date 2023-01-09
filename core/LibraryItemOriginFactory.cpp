/** \file LibraryItemOriginFactory.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
