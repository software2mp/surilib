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

#ifndef LOOKUPTABLE_H_
#define LOOKUPTABLE_H_

// Includes estandar
#include <map>
#include <limits>

// Includes Suri
#include "logmacros.h"

// Includes Wx
#include "wx/xml/xml.h"

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Clase que representa una tabla de busqueda de valores */
/**
 *  Esta clase representa una tabla de busqueda de valores.
 *  Conceptualmente se define como una serie de limites, que definen intervalos
 * para los cuales se tiene un valor de salida asociado.
 */
class LookUpTable {
public:
   /** Tipo que se utiliza para guardar la tabla */
   typedef std::map<double, double> LutType;
   /** ctor */
   LookUpTable();
   /** dtor */
   ~LookUpTable();

// ------------------- METODOS DE SERIALIZACION DE UNA TABLA -------------------
   /** Metodo que lee un xml y carga la tabla */
   static bool FromXml(const wxXmlNode *pLutNode, LookUpTable &Parameters);
   /** Metodo para convertir una tabla a xml */
   static bool ToXml(const LookUpTable &Parameters, wxXmlNode* &pLutNode);
   /** Convierte la tabla local en xml */
   bool ToXml(wxXmlNode *pLutNode) const;

// -------------------- METODO QUE APLICA LA TRANSFORMACION --------------------
   /** Metodo que aplica la tabla para un dato de entrada (tipo de dato nativo) */
   double ApplyLut(const double InputData) const;
   /** Template que aplica la tabla convirtiendo los tipos de datos */
   template<class T>
   T ApplyLut(const T InputData) const {
      return static_cast<T>(ApplyLut(static_cast<double>(InputData)));
   }
// ---------------------------- METODOS DE ACCESO ------------------------------
   /** Agrega un limite a la tabla con su salida correspondiente (tipo de dato nativo) */
   bool AddBin(const double LowerBound, const double Output);
   /** Template que agrega un limite convirtiendo los tipos de datos */
   template<class T1, class T2>
   bool AddBin(const T1 LowerBound, const T2 Output) {
      return AddBin(static_cast<double>(LowerBound), static_cast<double>(Output));
   }
   /** Carga la tabla interna o la inicializa en vacio */
   void SetTable(const LutType &Table = LutType());
   /** Obtiene una copia de la tabla interna */
   void GetTable(LutType &Table) const;
private:
   double defaultValue_; /*! Valor default de la tabla no se guarda en XML */
   LutType lookUpTable_; /*! tabla de busqueda */
};
}  // namespace suri

#endif /* LOOKUPTABLE_H_ */
