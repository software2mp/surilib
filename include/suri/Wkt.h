/*! \file Wkt.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WKT_H_
#define WKT_H_

// Includes standard
#include <vector>
#include <string>

// Clase de GDAL/OGR para manejo de arboles de tipo WKTs
class OGR_SRSNode;

/** namespace suri */
namespace suri {
/** Interfaz para manejo de Well Know Strings (WKTs) */
/**
 *  Esta clase maneja Well Know Strings (WKTs)
 *  Utiliza la clase OGR_SRSNode del OGR/GDAL
 */
class Wkt {
   /** Constructor */
   Wkt() :
         pRoot_(NULL) {
   }

   /** Destructor */
   ~Wkt();

   /** Constructor de copia */
   Wkt(const Wkt &SWkt);
public:

   /** Crea el WKT a partir del WKT */
   static Wkt * Create(const std::string &SWkt);

   /** Crea el WKT a partir de un codigo y autoridad. */
   static Wkt * CreateWithAuthorityCode(const std::string &Authority,
                                        const std::string &Code);

   /** Destruye el WKT */
   static void Destroy(Wkt * &pWkt);

   /** Consulta si es valido el wkt */
   static bool IsValid(const std::string &SWkt);

   // ------------------- METODOS DE CONSULTA ----------------------------------

   /** Devuelve el WKT en formato string a partir del path referido */
   std::string GetWktString(const std::string &PathName = "") const;

   /** Devuelve el WKT en formato string "lindo" (para lectura mas simple) a partir del path referido */
   std::string GetPrettyWktString(const std::string &PathName = "") const;

   /** Devuelve el valor del nodo */
   std::string GetRootValue() const;

   /** Devuelve los valores de los hijos que tienen hijos */
   std::vector<std::string> GetNodeChildren(const std::string &PathName = "") const;

   /** Devuelve los valores de los hijos que no tienen hijos (valores) */
   std::vector<std::string> GetLeafChildren(const std::string &PathName = "") const;

   /** Retorna el nombre de la autoridad del wkt */
   std::string GetAuthorityName() const;

   /** Retorna el codigo asignado a la raiz del wkt por la autoridad */
   std::string GetAuthorityCode() const;

   // ------------------- METODOS DE MODIFICACION ------------------------------

   /** Setea el valor del nodo */
   bool SetRootValue(const std::string &RootValue);

   /** Borra el contenido a partir del path referido */
   bool Delete(const std::string &PathName = "");

   /** Inserta el path indicado */
   bool InsertPath(const std::string &PathName);

   /** Inserta los valores en el path indicado */
   bool InsertValue(const std::vector<std::string> &Values,
                    const std::string &PathName = "");

   /** Inserta el valor en el path indicado */
   bool InsertValue(const std::string &Value, const std::string &PathName = "");

   /** Inserta el Wkt en el path indicado */
   bool InsertWkt(const std::string &SWkt, const std::string &PathName = "");

   /** Devuelve el WKT en formato Proj4. */
   std::string ToProj4();

protected:
   OGR_SRSNode * pRoot_; /*! Puntero aodo raiz del arbol del WKT */
   /** Recorre el arbol siguiendo el path indicado y devuelve el puntero al nodo */
   OGR_SRSNode * FindNode(const std::string &PathName = "") const;

private:
};

/** Compara wkts a nivel string para ver si representan el mismo arbol */
bool WktCompare(const std::string &lhs, const std::string &rhs);
} /*namespace suri*/

#endif /*WKT_H_*/
