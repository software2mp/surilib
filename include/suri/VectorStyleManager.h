/*! \file VectorStyleManager.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTORSTYLEMANAGER_H_
#define VECTORSTYLEMANAGER_H_

// Includes standard
#include <vector>
#include <string>

// Includes suri
#include "suri/VectorStyleTable.h"

/** Defines */
/** namespace suri */
namespace suri {
/** Clase que maneja las tablas de estilos vectoriales */
/**
 *  Esta clase maneja las tablas de estilos vectoriales
 *  Brinda estilos por defecto
 *  Maneja las tablas de estilos
 *  \todo Implementacion tipo singleton???
 */
class VectorStyleManager {
   /** Constructor */
   VectorStyleManager();
   /** Destructor */
   ~VectorStyleManager();
   /** Constructor de copia */
   VectorStyleManager(const VectorStyleManager &pVStyleMgr);
public:
   /** Devuelve una instancia de la clase. */
   static VectorStyleManager& Instance();
// ------------------- METODOS DE CONSULTA ------------------------------
   /** Devuelve la cantidad de total tablas de estilos */
   size_t GetTablesCount();
   /** Devuelve los nombres de las tablas de estilos */
   std::vector<std::string> GetTablesNames();
   /** Devuelve una tabla de estilo */
   VectorStyleTable * GetTable(const size_t StyleTableIndex);
   /** Devuelve una tabla de estilo */
   VectorStyleTable * GetTable(const std::string &CategoryName,
                               Vector::VectorType Type);
   /** Devuelve una tabla de estilo */
   VectorStyleTable * GetTable(Vector::VectorType Type);
   /** Agrega una tabla de estilos y toma posesion de la mismo */
   bool AddTable(const std::string &CategoryName, VectorStyleTable *pVStyleTable);
   /** Agrega una tabla de estilos y toma posesion de la mismo */
   bool AddTable(VectorStyleTable * pVStyleTable);
   /** Borra una tabla de estilos */
   bool DeleteTable(const size_t StyleTableIndex);
   /** Borra una tabla de estilos */
   bool DeleteTable(const std::string &CategoryName, Vector::VectorType Type);
   /** Borra una tabla de estilos */
   bool DeleteTable(Vector::VectorType Type);
   /** Devuelve un estilo default */
   VectorStyle * GetDefaultStyle(const std::string &CategoryName,
                                 Vector::VectorType Type);
   /** Devuelve un estilo default */
   VectorStyle * GetDefaultStyle(Vector::VectorType Type);
   /** Devuelve un estilo particular. */
   VectorStyle * GetStyle(const std::string &CategoryName,
                          Vector::VectorType VectorType, size_t StyleId);


protected:
   /** Carga los estilos del sistema */
   void LoadDefaultStyles();

   std::string defaultCategory_; /*! categoria default (layer) */
   std::multimap<std::string, VectorStyleTable *> styleTablesMap_; /*! Tablas */
   /* de estilos */
private:
};
} /*namespace suri*/

#endif /*VECTORSTYLEMANAGER_H_*/
