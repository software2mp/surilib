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

#include "suri/Option.h"

/** namespace suri */
namespace suri {
/**
 * Constructor
 */
Option::Option() {
}

/**
 * Destructor
 */
Option::~Option() {
}

/**
 * Agrega o cambia opciones
 *  Permite almacenar opciones diversas asociadas a la imagen.
 * \post La opcion se agrega con el valor deseado o se modifica el valor de
 *  existir.
 * @param[in] Name nombre de la opcion
 * @param[in] Value valor de la opcion
 */
void Option::SetOption(const std::string &Name, const std::string &Value) {
   // inserto el valor (asumiendo que no existe)
   std::pair<OptionsMapType::iterator, bool> ret = options_.insert(
         std::make_pair(Name, Value));
   // si no se produce insercion, asumo que existe y lo modifico
   if (!ret.second) {
      options_[Name] = Value;
   }
}

/**
 * Agrega o cambia opciones
 *  Permite almacenar opciones diversas asociadas a la imagen.
 * \post Las opciones se agregan con el valor deseado o se modifican los valores
 * que existen.
 * @param[in] Options mapa con las opciones
 */
void Option::SetOption(const OptionsMapType &Options) {
   OptionsMapType::const_iterator it = Options.begin();
   while (it != Options.end()) {
      SetOption(it->first, it->second);
      it++;
   }
}

/**
 *  Permite obtener una opcion
 *  Obtiene opciones de el valor de una opcion cuyo nombre se recibe por parametro
 *  @param[in] Name nombre de la opcion
 *  @return string con el valor de la opcion, o vacio si no se encontro la opcion
 */
std::string Option::GetOption(const std::string &Name) const {
   OptionsMapType::const_iterator ret = options_.find(Name);
   if (ret != options_.end()) {
      return ret->second;
   }
   return "";
}

/**
 * Retorna todas las opciones asociadas a una imagen.
 * @return mapa con el nombre / valor de todas las opciones
 */
Option::OptionsMapType Option::GetOption() const {
   return options_;
}
}
