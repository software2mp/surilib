/*! \file ColorTableCategory.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
// Includes Estandar
#include <vector>
#include <string>
#include <map>
#include <list>
#include <limits>

// Includes suri
#include "ColorTableCategory.h"
#include "suri/LibraryManager.h"
#include "suri/LibraryManagerFactory.h"
#include "suri/LibraryClient.h"
#include "suri/LibraryItem.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/LibraryItemAttributeFactory.h"
#include "suri/RangeItemAttribute.h"

/** namespace suri */
namespace suri {
/** Ctor */
ColorTableCategory::ColorTableCategory() {
}

/** Dtor */
ColorTableCategory::~ColorTableCategory() {
   std::map<std::string, ColorTable*>::iterator it = loadedColorTables_.begin();
   for (; it != loadedColorTables_.end(); it++)
      delete it->second;

   loadedColorTables_.clear();
}

/**
 * Retorna los nombre de las tablas de colores registradas en la categoria
 * @return std::list con los nombres de todas las tablas de colores en la categoria
 */
std::list<std::string> ColorTableCategory::GetColorTableNames() {
   std::list<std::string> loadedcolortables;
   std::map<std::string, ColorTable*>::iterator it = loadedColorTables_.begin();
   for (; it != loadedColorTables_.end(); it++)
      loadedcolortables.push_back(it->first);
   return loadedcolortables;
}

/**
 * Busca la tabla de colores en la categoria usando el nombre
 * @param[in] Name nombre de la tabla de colores que quiero
 * @param[out] pColorTable tabla de colores configurada con
 * la tabla correspondiente. NULL si no se encuentra.
 * @return indica si encontro la tabla.
 */
bool ColorTableCategory::GetColorTable(const std::string &Name,
                                       const ColorTable* &pColorTable) {
   // Analizo que la tabla de colores tenga un nombre y no exista en categoria
   if (Name.empty()) {
      REPORT_DEBUG("D: La color table debe tener nombre no vacio");
      return false;
   }

   std::map<std::string, ColorTable*>::iterator it = loadedColorTables_.find(Name);
   if (it == loadedColorTables_.end()) {
      return false;
   }

   pColorTable = it->second;
   return true;
}

/**
 * Retorna los nombre de las tablas de colores registradas en la biblioteca
 * @param[in] pManager administrador de items de biblioteca
 * @return std::list con los nombres de todas las tablas de colores en la categoria
 */
std::list<std::string> ColorTableCategory::GetColorTableNames(LibraryManager* pManager) {
   std::list<std::string> loadedcolortables;
   LibraryClient client(pManager, LibraryManager::READWRITE);
   const Library* plibrary = client.GetLibraryByCode(LibraryManagerFactory::DensitySliceCode);
   if (plibrary) {
      std::vector<LibraryItemCatalog*> catalog = plibrary->GetCatalog();
      std::vector<LibraryItemCatalog*>::iterator it = catalog.begin();
      for (; it != catalog.begin() ; ++it) {
         loadedcolortables.push_back((*it)->GetPrincipalAttributeValue());
      }
      // elimino los items de catalog generados
      delete_all(catalog);
   }
   return loadedcolortables;
}

/**
 * Busca la tabla de colores en la biblioteca usando el nombre
 * @param[in] pManager administrador de items de biblioteca
 * @param[in] Name nombre de la tabla de colores que quiero
 * @param[out] pColorTable tabla de colores configurada con
 * la tabla correspondiente. NULL si no se encuentra.
 * @return indica si encontro la tabla.
 */
bool ColorTableCategory::GetColorTable(LibraryManager* pManager,
                                       const std::string &Name,
                                       const ColorTable* &pColorTable) {
   // Analizo que la tabla de colores tenga un nombre y no exista en categoria
   if (Name.empty()) {
      REPORT_DEBUG("D: La color table debe tener nombre no vacio");
      return false;
   }
   LibraryClient client(pManager, LibraryManager::READWRITE);
   const Library* plibrary = client.GetLibraryByCode(LibraryManagerFactory::DensitySliceCode);
   if (!plibrary)
      return false;
   const LibraryItem* pitem = plibrary->GetItemByPrincipal(Name);
   if (!pitem)
      return false;
   LibraryItem::MapAttribute attributes = pitem->GetAttributes();
   LibraryItem::MapAttribute::iterator it = attributes.begin();
   ColorTable* pcolortable = new ColorTable();
   pcolortable->SetName(pitem->GetPrincipalAttribute()->GetValue());
   pcolortable->Activate(true);
   for (; it != attributes.end(); ++it) {
      if (LibraryItemAttributeFactory::IsRangeItemAttribute(it->second)) {
         RangeItemAttribute* prange =
               LibraryItemAttributeFactory::CreateRangeItemAttribute(it->second);
         if (!prange)
            break;
         std::string hexacolor = prange->GetHexaColor();
         if (hexacolor.size() >= 6) {
            int red = HexaStringToLong(hexacolor.substr(0,2));
            int green = HexaStringToLong(hexacolor.substr(2, 2));
            int blue = HexaStringToLong(hexacolor.substr(4));
            double upperbound = (prange->GetFinishValue() ==
                  std::numeric_limits<double>::infinity())?
                                             std::numeric_limits<double>::max() :
                                             prange->GetFinishValue();
            pcolortable->AddColor(upperbound, red, green, blue);
         }
         delete prange;
      }
   }
   pColorTable = pcolortable;
   return true;
}

/**
 * Agrega una nueva tabla de colores en la categoria.
 * @param[in] pColorTable tabla que quiero agregar al manager. La tabla pasa a
 * ser resposabilidad de esta clase.
 */
bool ColorTableCategory::AddColorTable(ColorTable* pColorTable) {
   const ColorTable* pcolortable = NULL;
   if (GetColorTable(pColorTable->GetName(), pcolortable)) {
      REPORT_DEBUG("D: La color table ya esta en la categoria");
      return false;
   }

   loadedColorTables_[pColorTable->GetName()] = pColorTable;
   return true;
}

/**
 * Elimina tabla de colores de la categoria
 * @param[in] Name nombre de la tabla que quiero eliminar
 */
bool ColorTableCategory::DeleteColorTable(std::string Name) {
   return loadedColorTables_.erase(Name) > 0;
}

/**
 * Busca la tabla de colores primero por nombre y despues por contenido.
 * @param[in] UnknowColorTable tabla de colores que quiero analizar
 * @param[out] ColorTableName nombre de la tabla de colores. Si no se encontro
 * no se modifica
 * @return indica la tabla coincide con una registrada .
 */
bool ColorTableCategory::IsLoaded(const ColorTable* &UnknowColorTable,
                                  std::string &ColorTableName) {
   // Si tiene nombre, lo busco en categoria y solo comparo contra esa tabla
   const ColorTable* pcolortable = NULL;
   if (GetColorTable(UnknowColorTable->GetName(), pcolortable)) {
      if (*UnknowColorTable == *pcolortable) {
         ColorTableName = pcolortable->GetName();
         return true;
      }
      return false;
   }

   // Si no tiene nombre recorro la categoria buscando una tabla igual
   std::map<std::string, ColorTable*>::iterator it = loadedColorTables_.begin();
   for (; it != loadedColorTables_.end(); it++)
      if (*UnknowColorTable == *(it->second)) {
         ColorTableName = it->first;
         return true;
      }

   return false;
}
}
