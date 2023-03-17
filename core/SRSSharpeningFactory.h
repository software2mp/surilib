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

#ifndef SRSSHARPENINGFACTORY_H_
#define SRSSHARPENINGFACTORY_H_

#include "SRSSharpening.h"
#include "suri/ParameterCollection.h"

#define REGISTER_SHARPENING(M_SName) \
namespace { \
class _M_Register_Sharpening_ { \
public: \
   _M_Register_Sharpening_() { \
      suri::raster::sharpening::SharpeningFactory::GetInstance()->Register(#M_SName, &M_SName::Create); \
   } \
}; \
_M_Register_Sharpening_ _m_register_Sharpening_; \
} \

namespace suri {
namespace raster {
namespace sharpening {

typedef Sharpening* (*CREATESHARPENINGCB)(suri::ParameterCollection& Params);

/**
 *
 */
class SharpeningFactory {
public:
   /**
    * Dtor (no es virtual porque no se deberia heredar de esta clase)
    */
   ~SharpeningFactory();

   /**
    * Devuelve la unica instancia.
    */
   static SharpeningFactory* GetInstance();

   /**
    * Crea una instancia para utilizar algun metodo para fusion de bandas
    * con pancromatica segun los parametros dados.
    */
   Sharpening* Create(const std::string& SName, suri::ParameterCollection& Params);

   /**
    * Registra un nuevo tipo metodo para fusion de bandas con pancromatica.
    */
   void Register(const std::string& SName, CREATESHARPENINGCB pCreateCb);

private:
   /**
    * Ctor (singleton)
    */
   SharpeningFactory();

   std::map<std::string, CREATESHARPENINGCB> factoryCbs_;
};

}  // namespace sharpening
}  // namespace raster
}  // namespace suri

#endif /* SRSSHARPENINGFACTORY_H_ */
