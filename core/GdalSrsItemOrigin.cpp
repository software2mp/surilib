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

// Includes standar
#include <string>
#include <map>
#include <vector>
#include <sstream>

// Includes suri
#include "suri/Configuration.h"
#include "GdalSrsItemOrigin.h"
#include "suri/Wkt.h"
#include "IndexFile.h"
#include "logmacros.h"
#include "SpatialReference.h"
#include "suri/LibraryItem.h"
#include "suri/AuxiliaryFunctions.h"
#include "FavoritesIndexFile.h"

namespace suri {

/** Path con los indices en disco. Relativos a data */
const std::string PROJECTED_SRS_INDEX = "libraries/projected.idx";
const std::string GEOGRAFIC_SRS_INDEX = "libraries/geografic.idx";
const std::string DATUM_SRS_INDEX = "libraries/datum.idx";
const std::string ELLIPSOID_SRS_INDEX = "libraries/ellipsoid.idx";
std::map<int, std::string> FILE_NAMES;

/** Nombre del atributo que guarda WKT en item */
const std::string GdalSrsItemOrigin::WKT_ATTRIBUTE_NAME = "WKT";
/** nombre del atributo que indica si el sr es proyectado o geo **/
const std::string GdalSrsItemOrigin::ProjectedAttr = "PROJECTED";

/**
 * Constructor
 */
GdalSrsItemOrigin::GdalSrsItemOrigin(const SuriObject::UuidType& LibraryId,
                                     GdalSrsItemOrigin::SrsItemType OriginItemType,
                                     const std::string& FavoritesIndexFileName) :
      LibraryItemOrigin(LibraryId, SYSTEM,
                        std::vector<LibraryItemOrigin::CapabilityType>()) {
   AddCapability(READ);
   libraryItemType_ = OriginItemType;
   switch (OriginItemType) {
      case PROJECTED_SRS:
         name_ = "Sistemas Proyectados";
         break;
      case GEOGRAFIC_SRS:
         name_ = "Sistemas Georrefenciados";
         break;
      case DATUM:
         name_ = "Datum";
         break;
      case ELLIPSOID:
         name_ = "Elipsoide";
         break;
      default:
         name_ = "GdalSrsItemOrigin";
         break;
   }
   favoritesIndexFileName_ = FavoritesIndexFileName;
   LoadItems();
}

/** Dtor **/
GdalSrsItemOrigin::~GdalSrsItemOrigin() {
}

/**
 * Esta funcion inicializa los srs, datums y elipsoides soportados por gdal.
 */
void GdalSrsItemOrigin::InitializeGdalSrsIndexs() {
   IndexFile projected_idxfile(GetIndexFileName(PROJECTED_SRS));
   IndexFile geografic_idxfile(GetIndexFileName(GEOGRAFIC_SRS));
   IndexFile datum_idxfile(GetIndexFileName(DATUM));
   IndexFile ellipsoid_idxfile(GetIndexFileName(ELLIPSOID));

   std::string key, data;
   for (int i = 0; i <= 65000; i++) {
      if ( i % 500 == 0 ) {
         REPORT_DEBUG("EPSG count: %d", i);
         FLUSH();
      }

      std::stringstream ss;
      ss << i;

      std::string wktstr = SpatialReference::GetWktWithAuthority("EPSG", ss.str());
      if (!wktstr.empty()) {
         REPORT_DEBUG("WKT: %s", wktstr.c_str());

         std::string key = SpatialReference::GetAuthorityId(wktstr);

         if (SpatialReference::IsProjected(wktstr)) {
            std::string name = SpatialReference::GetProjCoordinateSystemName(wktstr);
            projected_idxfile.AppendRegister(key, name);
         } else if (SpatialReference::IsGeographic(wktstr)) {
            std::string name = SpatialReference::GetGeoCoordinateSystemName(wktstr);
            geografic_idxfile.AppendRegister(key, name);
         }

         std::string datum = SpatialReference::GetGeoCoordinateSystemDatum(wktstr);
         std::string datumkey = SpatialReference::GetAuthorityId(datum).c_str();
         if (!datum_idxfile.GetRegisterData(datumkey, data)) {
            std::string name = SpatialReference::GetGeoCoordinateSystemDatumName(wktstr);
            datum_idxfile.AppendRegister(datumkey, datum);
         }

         std::string ellipsoid = SpatialReference::GetGeoCoordinateSystemSpheroid(wktstr);
         std::string ellipsoidkey = SpatialReference::GetAuthorityId(ellipsoid).c_str();
         if (!ellipsoid_idxfile.GetRegisterData(ellipsoidkey, data)) {
            std::string name = SpatialReference::GetGeoCoordinateSystemSpheroidName(wktstr);
            ellipsoid_idxfile.AppendRegister(ellipsoidkey, ellipsoid);
         }
      }
   }
}




/**
 * Agrega el item pasado por parametro a la fuente de datos del origen.
 * Metodo que tiene un comportamiento particular para cada tipo de origen.
 * (una clase concreta de origen tiene que reimplementar este metodo)
 */
bool GdalSrsItemOrigin::AddItemToSource(LibraryItem* pItem) {
    int proj = libraryItemType_ == PROJECTED_SRS ? 1 : 0;
    pItem->AddAttribute(ProjectedAttr, LibraryItemAttribute::SimpleAttr,
                       NumberToString<int>(proj));
    return false;
}

/**
 * Elimina el item de la fuente de datos del origen correspondiente al codigo
 * pasado por parametro
 * Metodo que tiene un comportamiento particular para cada tipo de origen.
 * (una clase concreta de origen tiene que reimplementar este metodo)
 */
bool GdalSrsItemOrigin::RemoveItemFromSourceByCode(const std::string& Code) {
   return false;
}

/**
 * Elimina el item de la fuente de datos del origen correspondiente al id
 * que se pasa por parametro
 * Metodo que tiene un comportamiento particular para cada tipo de origen.
 * (una clase concreta de origen tiene que reimplementar este metodo)
 */
bool GdalSrsItemOrigin::RemoveItemFromSource(const SuriObject::UuidType& ItemId) {
   return false;
}

/**
 * Obtiene el item pasado por parametro a la fuente de datos del origen
 * Metodo que tiene un comportamiento particular para cada tipo de origen.
 * (una clase concreta de origen tiene que reimplementar este metodo)
 */
const LibraryItem* GdalSrsItemOrigin::GetItemByCodeFromSource(const std::string& Code) {
   LibraryItem* pitem = new LibraryItem(libraryId_, Code);
   pitem->ConfigureOriginId(GetId());
   std::string wkt;
   if (libraryItemType_ == PROJECTED_SRS || libraryItemType_ == GEOGRAFIC_SRS) {
      wkt = SpatialReference::GetWktWithAuthorityId(Code);
   } else if (libraryItemType_ == DATUM || libraryItemType_ == ELLIPSOID) {
      IndexFile idxfile(GetIndexFileName(libraryItemType_));
      idxfile.GetRegisterData(Code, wkt);
   }

   if (!ConfigureLibraryItem(wkt, pitem)) {
      delete pitem;
      pitem = NULL;
   }

   return pitem;
}

/**
 * Reemplaza un item de la fuente de datos del origen
 * Metodo que tiene un comportamiento particular para cada tipo de origen.
 * (una clase concreta de origen tiene que reimplementar este metodo)
 */
bool GdalSrsItemOrigin::ReplaceItemFromSource(LibraryItem* pItem) {
   return false;
}

/**
 * Permite actualizar el item en memoria en caso que este incompleto o
 * haya quedado desactualizado con el origen de datos concreto.
 * (una clase concreta de origen tiene que reimplementar este metodo)
 * @param[in] pItem item que quiero actualizar
 * @return indica si se pudo actualizar el item.
 */
bool GdalSrsItemOrigin::UpdateItem(LibraryItem* pItem) const {
   if (!pItem || pItem->GetPrincipalAttribute() == NULL)
      return false;

   std::string wkt = SpatialReference::GetWktWithAuthorityId(
                              pItem->GetPrincipalAttribute()->GetValue());
   return ConfigureLibraryItem(wkt, pItem);
}

/** Metodo auxiliar que se encarga de cargar los items del origen **/
void GdalSrsItemOrigin::LoadItems() {
   std::string key, data;
   IndexFile idxfile(GetIndexFileName(libraryItemType_));
   idxfile.OpenIndex();
   while (idxfile.GetNextRegister(key, data)) {
      LibraryItem* pitem = new LibraryItem(libraryId_, key);
      pitem->ConfigureOriginId(GetId());
      if (libraryItemType_ == PROJECTED_SRS || libraryItemType_ == GEOGRAFIC_SRS) {
         pitem->AddAttribute(LibraryItemAttribute::ExtendedAttrName,
                                LibraryItemAttribute::SimpleAttr, data);
      } else if (libraryItemType_ == DATUM || libraryItemType_ == ELLIPSOID) {
         ConfigureLibraryItem(data, pitem);
      }
      AddItem(pitem);
   }
   idxfile.CloseIndex();
   if (!favoritesIndexFileName_.empty())
      LoadFavoritesIndex();
}

/**
 * Configura LibraryItem con datos en wkt
 * @param[in] ItemWkt wkt con el que quiero configurar el item.
 * @param[out] pLibraryItem item que quiero configurar
 * @return bool que indica si pudo configurar el item.
 */
bool GdalSrsItemOrigin::ConfigureLibraryItem(const std::string &ItemWkt,
                                                 LibraryItem *pLibraryItem) const {
   if (!pLibraryItem || !pLibraryItem->GetPrincipalAttribute() || ItemWkt.empty())
      return false;

   // Agrego nodo wkt
   if (pLibraryItem->GetAttribute(WKT_ATTRIBUTE_NAME) == NULL)
      pLibraryItem->AddAttribute(WKT_ATTRIBUTE_NAME,
                              LibraryItemAttribute::SimpleAttr, ItemWkt);

   // Agrego nodo principal
   if (pLibraryItem->GetAttribute(LibraryItemAttribute::ExtendedAttrName) == NULL) {
      Wkt* pwkt = Wkt::Create(ItemWkt);
      if (pwkt) {
         std::vector<std::string> children = pwkt->GetLeafChildren(pwkt->GetRootValue());
         pLibraryItem->AddAttribute(LibraryItemAttribute::ExtendedAttrName,
                                    LibraryItemAttribute::SimpleAttr,
                                    children[0]);
         Wkt::Destroy(pwkt);
      }
   }

   return true;
}

/** Metodo auxiliar que retorna nobmre del archivo indice asociado a tipo de biblioteca **/
std::string GdalSrsItemOrigin::GetIndexFileName(GdalSrsItemOrigin::SrsItemType Type) {
   if (FILE_NAMES.size() == 0) {
      FILE_NAMES[GdalSrsItemOrigin::PROJECTED_SRS] = PROJECTED_SRS_INDEX;
      FILE_NAMES[GdalSrsItemOrigin::GEOGRAFIC_SRS] = GEOGRAFIC_SRS_INDEX;
      FILE_NAMES[GdalSrsItemOrigin::DATUM] = DATUM_SRS_INDEX;
      FILE_NAMES[GdalSrsItemOrigin::ELLIPSOID] = ELLIPSOID_SRS_INDEX;
   }
   std::map<int, std::string>::const_iterator it = FILE_NAMES.find(Type);
   return Configuration::GetParameter("app_data_dir", wxT("./")) +
                                             std::string("/") + it->second;
}
} /* namespace suri */

