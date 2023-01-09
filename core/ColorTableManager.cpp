/*! \file ColorTableManager.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <utility>
#include <map>
#include <string>
#include <list>
#include <vector>
#include <limits>

// Includes suri
#include "ColorTableManager.h"
#include "suri/LibraryClient.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/LibraryManager.h"
#include "suri/LibraryManagerFactory.h"
#include "suri/LibraryItem.h"
#include "suri/LibraryItemAttributeFactory.h"
#include "suri/RangeItemAttribute.h"

// Defines
#define DEFAULTCATEGORYNAME "default"

#define STR_HELPER(S_H_M_x) #S_H_M_x
#define STR(S_M_x) STR_HELPER(S_M_x)

#define NDVI_0_INDEX    0
#define NDVI_02_INDEX   0.2
#define NDVI_03_INDEX   0.3
#define NDVI_04_INDEX   0.4
#define NDVI_05_INDEX   0.5
#define NDVI_06_INDEX   0.6
#define NDVI_07_INDEX   0.7
#define NDVI_1_INDEX    1

#define NDWI_M03_INDEX  -0.3
#define NDWI_M02_INDEX  -0.2
#define NDWI_M01_INDEX  -0.1
#define NDWI_0_INDEX    0
#define NDWI_01_INDEX   0.1
#define NDWI_02_INDEX   0.2
#define NDWI_03_INDEX   0.3
#define NDWI_04_INDEX   0.4

#define RVI_0_INDEX     0
#define RVI_02_INDEX    0.2
#define RVI_04_INDEX    0.4
#define RVI_06_INDEX    0.6
#define RVI_08_INDEX    0.8
#define RVI_1_INDEX     1


/** Cantidad de parametros de la tabla (key, red, green, blue)(columnas de la tabla) */
#define COLOR_TABLE_PARAMETERS 4

/** Tabla de colores para el indice NDVI - {Key | Red | Green | Blue} */
const int NDVIRowCount = 8;
const double pNDVIColorTable[][COLOR_TABLE_PARAMETERS] =
      { { NDVI_0_INDEX, 255, 255, 255 }, { NDVI_02_INDEX, 129, 69, 16 },
        { NDVI_03_INDEX, 78, 30, 26 }, { NDVI_04_INDEX, 177, 124, 33 },
        { NDVI_05_INDEX, 255, 222, 32 }, { NDVI_06_INDEX, 128, 255, 0 },
        { NDVI_07_INDEX, 111, 176, 29 }, { NDVI_1_INDEX, 0, 128, 0 } };

/** Tabla de colores para el indice NDWI - {Key | Red | Green | Blue} */
const int NDWIRowCount = 8;
const double pNDWIColorTable[][COLOR_TABLE_PARAMETERS] =
      { { NDWI_M03_INDEX, 255, 255, 255 }, {NDWI_M02_INDEX, 128, 0, 0 },
        { NDWI_M01_INDEX, 255, 0, 0 }, { NDWI_0_INDEX, 223, 112, 0 },
        { NDWI_01_INDEX, 192, 192, 192 }, { NDWI_02_INDEX, 0, 255, 255 },
        { NDWI_03_INDEX, 0, 0, 255 }, { NDWI_04_INDEX, 0, 0, 128 } };

/** Tabla de colores para el indice RVI - {Key | Red | Green | Blue} */
const int RVIRowCount = 6;
const double pRVIColorTable[][COLOR_TABLE_PARAMETERS] =
      { { RVI_0_INDEX, 255, 0, 0 }, { RVI_02_INDEX, 223, 112, 0 },
        { RVI_04_INDEX, 192, 192, 192 }, { RVI_06_INDEX, 0, 255, 255 },
        { RVI_08_INDEX, 0, 0, 255 }, { RVI_1_INDEX, 0, 0, 128 } };

/** Tabla de colores para el indice SMI - {Key | Red | Green | Blue} */
const int SMIRowCount = 6;
const double pSMIColorTable[][COLOR_TABLE_PARAMETERS] =
      { { 0, 255, 0, 0 }, { 0.1, 223, 112, 0 }, { 0.2, 192, 192, 192 },
        { 0.3, 0, 255, 255 }, { 0.4, 0, 0, 255 }, { 0.5, 0, 0, 128 } };

/** namespace suri */
namespace suri {
/** Ctor */
ColorTableManager::ColorTableManager() {
   LoadDefaultColorTables();
   LoadDefaultColorTablesLabels();
}

/** Dtor */
ColorTableManager::~ColorTableManager() {
}

/**
 * retorna una lista con los ids de todas las categorias registradas
 * @return std::list con el nombre de todas las categorias registradas.
 */
std::list<std::string> ColorTableManager::GetCategoryNames() {
   std::list<std::string> loadedcategories;
   std::map<std::string, ColorTableCategory*>::iterator it =
         colorTablesCategoriesMap_.begin();
   for (; it != colorTablesCategoriesMap_.end(); it++)
      loadedcategories.push_back(it->first);
   return loadedcategories;
}

/**
 * retorna una lista con los ids de todas las categorias registradas
 * @return std::list con el nombre de todas las categorias registradas.
 */
std::list<std::string> ColorTableManager::GetCategoryNames(LibraryManager* pManager) {
   std::list<std::string> loadedcategories;
   std::map<std::string, ColorTableCategory*>::iterator it =
         colorTablesCategoriesMap_.begin();
   if (it != colorTablesCategoriesMap_.end())
      loadedcategories = it->second->GetColorTableNames(pManager);
   return loadedcategories;
}

/**
 * Busca una categoria por nombre
 * @param[in] CategoryName nombre de la categoria
 * @param[out] pCategory puntero a ColorTableCategory
 * con la categoria elegida. El objeto es responsabilidad de esta clase
 * @return bool que indica si encontro la categoria. setea NULL el puntero
 * si retorno falso.
 */
bool ColorTableManager::GetCategory(const std::string &CategoryName,
                                    ColorTableCategory* &pCategory) {
   std::map<std::string, ColorTableCategory*>::iterator it =
         colorTablesCategoriesMap_.find(CategoryName);
   if (it == colorTablesCategoriesMap_.end()) {
      return false;
   }

   pCategory = it->second;
   return true;
}

/**
 * Busca la categoria default
 * @param[out] pCategory puntero a ColorTableCategory con la categoria elegida.
 * @return bool que indica si encontro la categoria. setea NULL el
 * puntero si retorno falso.
 */
bool ColorTableManager::GetCategory(ColorTableCategory * &pCategory) {
   return GetCategory(DEFAULTCATEGORYNAME, pCategory);
}

/**
 * Busca la tabla de colores ColorTableName en la categoira CategoryName
 * @param[in] CategoryName nombre de la categoria donde busca a la tabla
 * @param[in] ColorTableName nombre de la tabla de colores que quiero
 * @param[out] pColorTable tabla de colores configurada
 * con la tabla deseada. NULL si no pudo encontrala. El objeto es
 * responsabilidad de esta clase.
 * @return indica si encontro la tabla.
 */
bool ColorTableManager::GetColorTable(const std::string &CategoryName,
                                      const std::string &ColorTableName,
                                      const ColorTable * &pColorTable) {
   ColorTableCategory* pcategory;
   pColorTable = NULL;
   if (!GetCategory(CategoryName, pcategory)
         || !pcategory->GetColorTable(ColorTableName, pColorTable)) {
      return false;
   }

   return true;
}

/**
 * Busca la tabla de colores ColorTableName en la categoria default
 * @param[in] ColorTableName nombre de la tabla de colores que quiero
 * @param[out] pColorTable tabla de colores configurada
 * con la tabla deseada. NULL si no pudo encontrala. El objeto es
 * responsabilidad de esta clase.
 * @return indica si encontro la tabla.
 */
bool ColorTableManager::GetColorTable(const std::string &ColorTableName,
                                      const ColorTable * &pColorTable) {
   return GetColorTable(DEFAULTCATEGORYNAME, ColorTableName, pColorTable);
}

/**
 * Busca la tabla de colores ColorTableName en la categoira CategoryName
 * @param[in] pManager administrador de items de biblioteca
 * @param[in] CategoryName nombre de la categoria donde busca a la tabla
 * @param[in] ColorTableName nombre de la tabla de colores que quiero
 * @param[out] pColorTable tabla de colores configurada
 * con la tabla deseada. NULL si no pudo encontrala. El objeto es
 * responsabilidad de esta clase.
 * @return indica si encontro la tabla.
 */
bool ColorTableManager::GetColorTable(LibraryManager* pManager,
                                      const std::string &CategoryName,
                                      const std::string &ColorTableName,
                                      const ColorTable * &pColorTable) {
   ColorTableCategory* pcategory;
   pColorTable = NULL;
   if (!GetCategory(CategoryName, pcategory)
         || !pcategory->GetColorTable(pManager, ColorTableName, pColorTable)) {
      return false;
   }

   return true;
}

/**
 * Busca la tabla de colores ColorTableName en la categoria default
 * @param[in] pManager administrador de items de biblioteca
 * @param[in] ColorTableName nombre de la tabla de colores que quiero
 * @param[out] pColorTable tabla de colores configurada
 * con la tabla deseada. NULL si no pudo encontrala. El objeto es
 * responsabilidad de esta clase.
 * @return indica si encontro la tabla.
 */
bool ColorTableManager::GetColorTable(LibraryManager* pManager,
                                      const std::string &ColorTableName,
                                      const ColorTable * &pColorTable) {
   return GetColorTable(pManager, DEFAULTCATEGORYNAME, ColorTableName, pColorTable);
}
/**
 * Registra una categoria. No puede haber dos categorias con el mismo nombre
 * @param[in] CategoryName nombre de la categoria donde quiero agregar la tabla
 * @param[in] pCategory puntero a la categoria que quiero agregar. La categoria
 * pasa a ser responsabilidad de esta clase
 */
bool ColorTableManager::AddCategory(const std::string &CategoryName,
                                    ColorTableCategory * pCategory) {
   ColorTableCategory* pcategory;
   if (GetCategory(CategoryName, pcategory)) {
      return false;
   }

   colorTablesCategoriesMap_[CategoryName] = pCategory;
   return true;
}

/**
 * Agrega la categoria default, si ya hay una falla.
 * @param[in] pCategory puntero a la categoriatabla que quiero agregar.
 * @return informa si pudo agregar la categoria
 * La categoria pasa a ser responsabilidad de esta clase
 */
bool ColorTableManager::AddCategory(ColorTableCategory * pCategory) {
   return AddCategory(DEFAULTCATEGORYNAME, pCategory);
}

/**
 * Elimina una categoria.
 * @param[in] CategoryName nombre de la categoria que quiero eliminar
 * @return informa si pudo eliminar la categoria
 */
bool ColorTableManager::DeleteCategory(const std::string &CategoryName) {
   return colorTablesCategoriesMap_.erase(CategoryName) > 0;
}

/**
 * Elimina la categoria default.
 * @return informa si pudo eliminar la categoria
 */
bool ColorTableManager::DeleteCategory() {
   return DeleteCategory(DEFAULTCATEGORYNAME);
}

/**
 * Busca en la categoria CategoryName si la tabla de colores esta cargada
 * (busca por nombre y contenido).
 * @param[in] CategoryName Nombre de la categoria en donde quiero buscar
 * @param[in] pColorTable tabla de colores que quiero analizar
 * @param[out] ColorTableName nombre de la tabla de colores. Si no se encontro
 * no se modifica
 * @return indica si la tabla coincide con una registrada .
 */
bool ColorTableManager::IsLoaded(const std::string &CategoryName,
                                 const ColorTable* pColorTable,
                                 std::string &ColorTableName) {
   ColorTableCategory* pcategory;
   if (!GetCategory(CategoryName, pcategory)) {
      return false;
   }
   return pcategory->IsLoaded(pColorTable, ColorTableName);
}

/**
 * Busca en la categoria default si la tabla de colores esta cargada
 * (busca por nombre y contenido).
 * @param[in] pColorTable tabla de colores que quiero analizar
 * @param[out] ColorTableName nombre de la tabla de colores. Si no se encontro
 * no se modifica
 * @return bool, indica si la tabla coincide con una registrada .
 */
bool ColorTableManager::IsLoaded(const ColorTable * pColorTable,
                                 std::string &ColorTableName) {
   return IsLoaded(DEFAULTCATEGORYNAME, pColorTable, ColorTableName);
}

/**
 * Retorna la instancia de ColorTableManager(Singleton).
 * @return la unica instancia de ColorTableManager.
 */
ColorTableManager& ColorTableManager::Instance() {
   static ColorTableManager *pcolortablemanager = new ColorTableManager();
   return *pcolortablemanager;
}

/**
 * Obtiene las etiquetas para cada intervalo.
 */
void ColorTableManager::GetColorTableLabels(const std::string& TableName,
      std::vector<std::pair<std::string, std::string> >& Labels) {
   if (labels_.find(TableName) != labels_.end())
      Labels = labels_[TableName];
}


/** Obtiene las etiquetas para cada intervalo del indice definido en biblioteca */
void ColorTableManager::GetColorTableLabels(
      LibraryManager* pManager, const std::string& TableName,
      std::vector<std::pair<std::string, std::string> >& Labels) {
   LibraryClient client(pManager, LibraryManager::READONLY);
   const Library* plib = client.GetLibraryByCode(LibraryManagerFactory::DensitySliceCode);
   if (plib) {
      const LibraryItem* pitem = plib->GetItemByPrincipal(TableName);
      if (pitem) {
         LibraryItem::MapAttribute attributes = pitem->GetAttributes();
         LibraryItem::MapAttribute::iterator it = attributes.begin();
         for (; it != attributes.end(); ++it) {
            LibraryItemAttribute* pattr = it->second;
            if (LibraryItemAttributeFactory::IsRangeItemAttribute(pattr)) {
               RangeItemAttribute* prange =
                     LibraryItemAttributeFactory::CreateRangeItemAttribute(pattr);
               if (prange) {
                  std::string name = prange->GetRangeName();
                  double upperbound = (prange->GetFinishValue() ==
                        std::numeric_limits<double>::infinity())?
                                                   std::numeric_limits<double>::max() :
                                                   prange->GetFinishValue();
                  std::string bound = DoubleToString(upperbound, JOIN_DOUBLE_PRECISION);
                  Labels.push_back(std::make_pair<std::string, std::string>(bound, name));
                  delete prange;
               }
            }
         }
      }
   }
}

/**
 * Carga mapa con tablas de colores soportadas en categoria.
 * (NDVI, NDWI, SMI y RVI). Agrega la categoria como default(AddCategory).
 */
void ColorTableManager::LoadDefaultColorTables() {
   ColorTableCategory* pcategory = new ColorTableCategory();
   LoadColorTablesWithVector(pcategory, "NDVI", pNDVIColorTable, NDVIRowCount);
   LoadColorTablesWithVector(pcategory, "NDWI", pNDWIColorTable, NDWIRowCount);
   LoadColorTablesWithVector(pcategory, "RVI", pRVIColorTable, RVIRowCount);
   LoadColorTablesWithVector(pcategory, "SMI", pSMIColorTable, SMIRowCount);
   AddCategory(pcategory);
}

/**
 * Crea color table con pColors y lo agrega a la categoria.
 * @param[out] pCategory catgoria donde quiero agregar el vector
 * @param[in] Name nombre de la tabla de colores
 * @param[in] pColors tabla con los colores
 * @param[in] Rows cantidad de filas en tabla de colores
 */
void ColorTableManager::LoadColorTablesWithVector(ColorTableCategory* pCategory,
                                                  std::string Name,
                                                  const double(*pColors)[4], int Rows) {
   ColorTable* pcolortable = new ColorTable();
   pcolortable->SetName(Name);
   pcolortable->Activate(true);
   for (int i = 0; i < Rows; i++)
      pcolortable->AddColor(pColors[i][0], SURI_ROUND(int, pColors[i][1]),
                            SURI_ROUND(int, pColors[i][2]),
                            SURI_ROUND(int, pColors[i][3]));
   pCategory->AddColorTable(pcolortable);
}

/**
 * Carga las etiquetas para las tablas de colores.
 */
void ColorTableManager::LoadDefaultColorTablesLabels() {
   std::vector<std::pair<std::string, std::string> > lbls;

   lbls.push_back(std::pair<std::string, std::string>(STR(NDVI_0_INDEX),
                                                      label_NDVI_0_SLICE_LABEL));
   lbls.push_back(std::pair<std::string, std::string>(STR(NDVI_02_INDEX),
                                                      label_NDVI_02_SLICE_LABEL));
   lbls.push_back(std::pair<std::string, std::string>(STR(NDVI_03_INDEX),
                                                      label_NDVI_03_SLICE_LABEL));
   lbls.push_back(std::pair<std::string, std::string>(STR(NDVI_04_INDEX),
                                                      label_NDVI_04_SLICE_LABEL));
   lbls.push_back(std::pair<std::string, std::string>(STR(NDVI_05_INDEX),
                                                      label_NDVI_05_SLICE_LABEL));
   lbls.push_back(std::pair<std::string, std::string>(STR(NDVI_06_INDEX),
                                                      label_NDVI_06_SLICE_LABEL));
   lbls.push_back(std::pair<std::string, std::string>(STR(NDVI_07_INDEX),
                                                      label_NDVI_07_SLICE_LABEL));
   lbls.push_back(std::pair<std::string, std::string>(STR(NDVI_1_INDEX),
                                                      label_NDVI_1_SLICE_LABEL));
   labels_["NDVI"] = lbls;

   lbls.push_back(std::pair<std::string, std::string>(STR(NDWI_M03_INDEX),
                                                      label_NDWI_M03_SLICE_LABEL));
   lbls.push_back(std::pair<std::string, std::string>(STR(NDWI_M02_INDEX),
                                                      label_NDWI_M02_SLICE_LABEL));
   lbls.push_back(std::pair<std::string, std::string>(STR(NDWI_M01_INDEX),
                                                      label_NDWI_M01_SLICE_LABEL));
   lbls.push_back(std::pair<std::string, std::string>(STR(NDWI_0_INDEX),
                                                      label_NDWI_0_SLICE_LABEL));
   lbls.push_back(std::pair<std::string, std::string>(STR(NDWI_01_INDEX),
                                                      label_NDWI_01_SLICE_LABEL));
   lbls.push_back(std::pair<std::string, std::string>(STR(NDWI_02_INDEX),
                                                      label_NDWI_02_SLICE_LABEL));
   lbls.push_back(std::pair<std::string, std::string>(STR(NDWI_03_INDEX),
                                                      label_NDWI_03_SLICE_LABEL));
   lbls.push_back(std::pair<std::string, std::string>(STR(NDWI_04_INDEX),
                                                      label_NDWI_04_SLICE_LABEL));
   labels_["NDWI"] = lbls;

   // Se reutilizan las etiquetas de NDVI para RVI
   lbls.push_back(std::pair<std::string, std::string>(STR(RVI_0_INDEX),
                                                      label_NDVI_0_SLICE_LABEL));
   lbls.push_back(std::pair<std::string, std::string>(STR(RVI_02_INDEX),
                                                      label_NDVI_02_SLICE_LABEL));
   lbls.push_back(std::pair<std::string, std::string>(STR(RVI_04_INDEX),
                                                      label_NDVI_04_SLICE_LABEL));
   lbls.push_back(std::pair<std::string, std::string>(STR(RVI_06_INDEX),
                                                      label_NDVI_06_SLICE_LABEL));
   lbls.push_back(std::pair<std::string, std::string>(STR(RVI_08_INDEX),
                                                      label_NDVI_07_SLICE_LABEL));
   lbls.push_back(std::pair<std::string, std::string>(STR(RVI_1_INDEX),
                                                      label_NDVI_1_SLICE_LABEL));
   labels_["RVI"] = lbls;
}
}

