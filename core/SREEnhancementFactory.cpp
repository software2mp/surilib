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

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "SREEnhancementFactory.h"

namespace suri {
namespace raster {
namespace enhancement {

/**
 * Ctor (singleton)
 */
EnhancementFactory::EnhancementFactory() {

}

/**
 * Dtor (no es virtual porque no se deberia heredar de esta clase)
 */
EnhancementFactory::~EnhancementFactory() {

}

/**
 * Devuelve la unica instancia.
 */
EnhancementFactory* EnhancementFactory::GetInstance() {
   static EnhancementFactory instance;
   return &instance;
}

/**
 * Crea una instancia de realce segun los parametros dados.
 */
Enhancement* EnhancementFactory::Create(const std::string& EName,
                                        suri::ParameterCollection& Params) {
   std::map<std::string, std::pair<CREATEENHANCEMENTCB, bool> >::iterator it = factoryCbs_.find(EName);

   if (it != factoryCbs_.end())
      return it->second.first(Params);

   return NULL;
}

/**
 * Registra un tipo de realce.
 */
void EnhancementFactory::Register(const std::string& EName, CREATEENHANCEMENTCB pCreateCb, bool Visible) {
   factoryCbs_[EName] = std::make_pair(pCreateCb, Visible);
}

/**
 * Obtiene los nombres de los tipos de realce disponibles.
 */
std::vector<std::string> EnhancementFactory::GetAvailableEnhancements(bool VisibleOnly) {
   std::vector<std::string> avails;
   std::map<std::string, std::pair<CREATEENHANCEMENTCB, bool> >::iterator it;
   for (it = factoryCbs_.begin(); it != factoryCbs_.end(); ++it) {
      if ((VisibleOnly == true && it->second.second == true) || VisibleOnly == false)
         avails.push_back(it->first);
   }
   return avails;
}

}  // namespace enhancement
}  // namespace raster
}  // namespace suri
