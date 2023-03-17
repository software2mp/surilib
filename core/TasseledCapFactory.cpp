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

#include "TasseledCapFactory.h"

namespace suri {
namespace raster {
namespace transformation {

/**
 * Ctor (singleton).
 */
TasseledCapFactory::TasseledCapFactory() {

}

/**
 * Dtor (no es virtual porque no se deberia heredar de esta clase).
 */
TasseledCapFactory::~TasseledCapFactory() {

}

/**
 * Devuelve la unica instancia.
 */
TasseledCapFactory* TasseledCapFactory::GetInstance() {
   static TasseledCapFactory s_instance;
   return &s_instance;
}

/**
 * Crea una instancia para utilizar algun metodo de transformacion tasseled-cap.
 */
TasseledCapTransformation* TasseledCapFactory::Create(const std::string& TCTName,
                                  suri::ParameterCollection& Params) {
   std::map<std::string, CREATETCTRANSFORMATIONCB>::iterator it = factoryCbs_.find(TCTName);

   if (it != factoryCbs_.end())
      return it->second(Params);

   return NULL;
}

/**
 * Registra un nuevo tipo metodo para transformacion tasseled-cap.
 */
void TasseledCapFactory::Register(const std::string& TCTName, CREATETCTRANSFORMATIONCB pCreateCb, GETOUTPUTBANDSCB pOutputCb) {
   factoryCbs_[TCTName] = pCreateCb;
   outputCbs_[TCTName] = pOutputCb;
}

/**
 * Obtiene los nombres de los tipos de transformaciones tasseled-cap disponibles.
 */
std::vector<std::string> TasseledCapFactory::GetAvailableTCTransformations() {
   std::vector<std::string> avails;
   std::map<std::string, CREATETCTRANSFORMATIONCB>::iterator it;
   for (it = factoryCbs_.begin(); it != factoryCbs_.end(); ++it) {
      avails.push_back(it->first);
   }
   return avails;
}

/**
 * Obtiene las bandas de salida para cada transformacion soportada.
 */
std::map<int, std::string> TasseledCapFactory::GetOutputBandsForTCTransformation(
      const std::string& TCTName) {
   std::map<std::string, GETOUTPUTBANDSCB>::iterator it = outputCbs_.find(TCTName);

   if (it != outputCbs_.end())
      return it->second();

   return std::map<int, std::string>();
}

}  // namespace transformation
}  // namespace raster
}  // namespace suri
