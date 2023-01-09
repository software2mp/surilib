/*! \file ColorTableCategory.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COLORTABLECATEGORY_H_
#define COLORTABLECATEGORY_H_

// Includes standar
#include <map>
#include <list>

// Includes suri
#include "ColorTable.h"

/** namespace suri */
namespace suri {

// Forwards
class LibraryManager;

/**
 * Agrupa las tablas de colores en categorias. Permite buscar una
 * tabla de colores sin nombre recorriendo todas las tablas registradas y
 * comparando el contenido(IsLoaded).
 */
class ColorTableCategory {
public:
   /** Ctor */
   ColorTableCategory();
   /** Dtor */
   ~ColorTableCategory();
   /** retorna los nombres de las tablas de colores en la categoria */
   std::list<std::string> GetColorTableNames();
   /** Busca tabla de colores Name en el mapa. Retorna puntero interno */
   bool GetColorTable(const std::string &Name, const ColorTable* &pColorTable);
   /** Agrega tabla de colores a la categoria.El puntero pasa a ser de la clase */
   bool AddColorTable(ColorTable* pColorTable);
   /** retorna los nombres de las tablas de colores en la categoria */
   std::list<std::string> GetColorTableNames(LibraryManager* pManager);
   /** Busca tabla de colores Name en el mapa. Retorna puntero interno */
   bool GetColorTable(LibraryManager* pManager, const std::string &Name, const ColorTable* &pColorTable);
   /** Elimina tabla de colores de la categoria */
   bool DeleteColorTable(std::string Name);
   /** Busca la tabla de colores primero por nombre y despues por contenido. */
   bool IsLoaded(const ColorTable* &UnknowColorTable, std::string &ColorTableName);

private:

   std::map<std::string, ColorTable*> loadedColorTables_; /*! Tablas de colores */
   /* cargadas en la categoria */
};
}

#endif /* COLORTABLECATEGORY_H_ */
