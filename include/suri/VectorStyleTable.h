/*! \file VectorStyleTable.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTORSTYLETABLE_H_
#define VECTORSTYLETABLE_H_

// Includes standard
#include <vector>
#include <string>

// Includes suri
#include "suri/VectorStyle.h"

/** namespace suri */
namespace suri {
/** Clase que implementa una tabla de estilos */
/**
 *  Esta clase implementa una tabla de estilos y los busca
 *  por nombre o por indice.
 *  \todo Metodos de lectura/escritura a disco
 */
class VectorStyleTable {
public:
   /** Constructor */
   VectorStyleTable(const Vector::VectorType &VType, const std::string &CategoryName =
         "");

   /** Constructor */
   VectorStyleTable(const Vector::VectorType &VType, const std::string &CategoryName,
                    const std::vector<std::string> &Styles);

   /** Destructor */
   ~VectorStyleTable();

   /** Devuelve el nombre de la tabla de estilos */
   std::string GetCategoryName() const;

   /** Devuelve el tipo de vector de la tabla de estilos */
   Vector::VectorType GetVectorType() const;

   /** Cambia el nombre de la tabla */
   void ChangeCategoryName(const std::string &NewCategoryName);

   /** Devuelve la cantidad de estilos */
   size_t GetStyleCount() const;

   /** Devuelve los nombres de los estilos */
   std::vector<std::string> GetStyleNames() const;

   /** Devuelve un estilo */
   VectorStyle * GetStyle(const size_t Index) const;

   /** Devuelve un estilo por el nombre */
   VectorStyle * GetStyle(const std::string &StyleName) const;

   /** Devuelve el siguiente estilo a utilizar */
   VectorStyle * GetDefaultStyle();

   /** Agrega un estilo y toma posesion del mismo */
   bool AddStyle(VectorStyle * pVStyle);

   /** Borra un estilo */
   bool DeleteStyle(const size_t Index);

   /** Borra un estilo por el nombre */
   bool DeleteStyle(const std::string &StyleName);

   /** Clona la tabla */
   VectorStyleTable * Clone(const std::string &NewCategoryName = "") const;

protected:
   std::string categoryName_; /*! Nombre de la tabla */
   Vector::VectorType vType_; /*! Tipo de vector */
   size_t styleIndex_; /*! Indice al siguiente estilo default */
   std::vector<VectorStyle *> table_; /*! Tabla de estilos */
private:
};
} /*namespace suri*/

#endif /*VECTORSTYLETABLE_H_*/
