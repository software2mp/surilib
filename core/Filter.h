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

#ifndef FILTER_H_
#define FILTER_H_

// Includes Estandar
#include <string>
#include <set>

// Includes Suri

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Clase encargada de generar un filtro en base a las restricciones indicadas sobre ids.
 */
class Filter {

public:
   /** Ctor */
   Filter();
   /** Ctor a partir de un string*/
   explicit Filter(const std::string &Query);
   /** ctor copia */
   Filter(const Filter&);
   /** Dtor */
   virtual ~Filter();
   /** Genera la consulta en base a las restricciones cargadas en el filtro en un string **/
   std::string GenerateQueryAsString() const;
   /** Agrega una restriccion de igual aplicada a un id determinado*/
   bool AddEqualConstraint(int Id);
   /** Elimina la restriccion de igual aplicada sobre un id determinado */
   bool RemoveEqualConstraint(int Id);
   /** Agrega una restriccion de mayor aplicada a un id determinado */
   bool AddGreaterConstraint(int id);
   /** Elimina la restriccion de mayor aplicada sobre un id determinado */
   bool RemoveGreaterConstraint(int Id);
   /** Agrega una restriccion de menor aplicada a un id determinado */
   bool AddMinorConstraint(int Id);
   /** Elimina la restriccion de menor aplicada sobre un id determinado */
   bool RemoveMinorConstraint(int Id);
   /** Retorna cual es el nombre del objeto a filtrar (clasula FROM)**/
   std::string GetFilteredName() const;
   /** Configura cual es el nombre del objeto a filtrar (clasula FROM)**/
   void SetFilteredName(const std::string& FilteredName);
   /** Configura cual es el nombre del campo correspondiente al id */
   void SetIdFieldName(const std::string& IdFieldName);
   /** Elimina todas las restricciones impuestas al filtro.  */
   void ClearFilter();

private:
   /** Metodo auxiliar para parsear las condiciones de = o IN **/
   void ParseEqualConditions(const std::string& Query);
   /** Metodo auxiliar para parsear las condiciones de Like **/
   void ParseLikeConditions(const std::string& Query);
   /** nombre del objeto sobre el cual se aplica el filtro **/
   std::string filteredName_;
   /** nombre del campo en donde se aplica el filtro*/
   std::string idFieldName_;
   typedef std::set<int> ConstraintSet;
   /** id con restricciones de igual */
   ConstraintSet equalConstraints_;
   /** id con restricciones de menor */
   ConstraintSet minorConstraints_;
   /** id con restricciones de mayor */
   ConstraintSet greaterConstraints_;
   /** texto con restricciones de like **/
   std::set<std::string> likeConstraints_;
   /** string que contiene el query **/
   std::string query_;
};

} /** namespace suri */

#endif /* FILTER_H_ */
