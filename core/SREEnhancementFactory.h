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

#ifndef SREENHANCEMENTFACTORY_H_
#define SREENHANCEMENTFACTORY_H_

#include <map>
#include <string>
#include <vector>
#include <utility>

#include "SREEnhancement.h"
#include "suri/ParameterCollection.h"

#define REGISTER_ENHANCEMENT(M_EName, M_EVisible) \
namespace { \
class _M_Register_Enhancement_ { \
public: \
   _M_Register_Enhancement_() { \
      suri::raster::enhancement::EnhancementFactory::GetInstance()->Register(#M_EName, &M_EName::Create, M_EVisible); \
   } \
}; \
_M_Register_Enhancement_ _m_register_enhancement_; \
} \

namespace suri {
namespace raster {
namespace enhancement {

typedef Enhancement* (*CREATEENHANCEMENTCB)(suri::ParameterCollection& Params);

class EnhancementFactory {
public:
   /**
    * Dtor (no es virtual porque no se deberia heredar de esta clase)
    */
   ~EnhancementFactory();

   /**
    * Devuelve la unica instancia.
    */
   static EnhancementFactory* GetInstance();

   /**
    * Crea una instancia de realce segun los parametros dados.
    */
   Enhancement* Create(const std::string& EName, suri::ParameterCollection& Params);

   /**
    * Registra un nuevo realce.
    */
   void Register(const std::string& EName, CREATEENHANCEMENTCB pCreateCb, bool Visible = true);

   /**
    * Obtiene los nombres de los tipos de realce disponibles.
    */
   std::vector<std::string> GetAvailableEnhancements(bool VisibleOnly = true);

private:
   /**
    * Ctor (singleton)
    */
   EnhancementFactory();

   std::map<std::string, std::pair<CREATEENHANCEMENTCB, bool> > factoryCbs_;
};

}  // namespace enhancement
}  // namespace raster
}  // namespace suri

#endif  // SREENHANCEMENTFACTORY_H_
