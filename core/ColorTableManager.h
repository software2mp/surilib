/*! \file ColorTableManager.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COLORTABLEMANAGER_H_
#define COLORTABLEMANAGER_H_

// Includes standar
#include <map>
#include <list>
#include <vector>
#include <utility>

// Includes suri
#include "ColorTableCategory.h"

/** namespace suri */
namespace suri {
/**
 * Singleton que administra las ColorTable reconocidas, admite manejo de
 * categorias de tablas.
 */
class ColorTableManager {
public:
   /** Ctor */
   ColorTableManager();
   /** Dtor */
   ~ColorTableManager();
   /** Retorna una lista con los ids de todas las categorias registradas */
   std::list<std::string> GetCategoryNames();
   /** Retorna una lista con los ids de todas las categorias registradas */
   std::list<std::string> GetCategoryNames(LibraryManager* pManager);
   /** Busca una categoria por nombre */
   bool GetCategory(const std::string &CategoryName, ColorTableCategory * &pCategory);
   /** Busca la categoria default */
   bool GetCategory(ColorTableCategory * &pCategory);
   /** Busca la tabla ColorTableName en la categoira CategoryName */
   bool GetColorTable(const std::string &CategoryName,
                      const std::string &ColorTableName,
                      const ColorTable * &pColorTable);
   /** Busca la tabla ColorTableName en la categoria default */
   bool GetColorTable(const std::string &ColorTableName,
                      const ColorTable * &pColorTable);
   /** Busca la tabla ColorTableName en la categoira CategoryName */
   bool GetColorTable(LibraryManager* pManager, const std::string &CategoryName,
                      const std::string &ColorTableName,
                      const ColorTable * &pColorTable);
   /** Busca la tabla ColorTableName en la categoria default */
   bool GetColorTable(LibraryManager* pManager,
                      const std::string &ColorTableName,
                      const ColorTable * &pColorTable);
   /** Agrega una categoria, si ya hay una falla. */
   bool AddCategory(const std::string &CategoryName, ColorTableCategory * pCategory);
   /** Agrega la categoria default, si ya hay una falla. */
   bool AddCategory(ColorTableCategory * pCategory);
   /** Elimina una categoria. */
   bool DeleteCategory(const std::string &CategoryName);
   /** Elimina la categoria default. */
   bool DeleteCategory();
   /** Busca en la categoria default si la tabla de colores esta cargada */
   bool IsLoaded(const std::string &CategoryName, const ColorTable* pColorTable,
                 std::string &ColorTableName);
   /** Busca en la categoria si la tabla de colores esta cargada */
   bool IsLoaded(const ColorTable * pColorTable, std::string &ColorTableName);
   /** Devuelve una instancia de la clase. */
   static ColorTableManager& Instance();

   /** Obtiene las etiquetas para cada intervalo */
   void GetColorTableLabels(const std::string& TableName,
                            std::vector<std::pair<std::string, std::string> >& Labels);
   /** Obtiene las etiquetas para cada intervalo del indice definido en biblioteca */
   void GetColorTableLabels(LibraryManager* pManager,
                            const std::string& TableName,
                            std::vector<std::pair<std::string, std::string> >& Labels);

protected:
   /** Carga tablas de colores soportadas en categoria default. */
   void LoadDefaultColorTables();
   /** Carga un vector [][4] a color table. */
   void LoadColorTablesWithVector(ColorTableCategory* pCategory, std::string Name,
                                  const double(*pColors)[4], int Rows);

private:
   std::map<std::string, ColorTableCategory*> colorTablesCategoriesMap_; /*! Permite encontrar las categorias por nombre */

   // Contiene las etiquetas para cada tabla de color segun el valor limite
   std::map<std::string, std::vector<std::pair<std::string, std::string> > > labels_;

   /** Carga las etiquetas para las tablas de colores */
   void LoadDefaultColorTablesLabels();
};
}

#endif /* COLORTABLEMANAGER_H_ */
