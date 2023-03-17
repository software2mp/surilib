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
