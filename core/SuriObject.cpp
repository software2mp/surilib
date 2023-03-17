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

// Includes Estandar
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>

// Includes Suri
#include "suri/SuriObject.h"
#include "suri/md5.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/** Tipo Null */
const SuriObject::UuidType SuriObject::NullUuid = "null";

/** Tipo Null */
const SuriObject::UuidIntType SuriObject::NullIntUuid = 123456;

/** Tipo Null */
const SuriObject::ClassIdType SuriObject::NullClassId = "NullClassId";

/** Genera UUIDs */
SuriObject::UuidType GenerateUuid(int Seed = 0);

/**
 *  Genera un UUID que garantiza por ejecucion de la aplicacion. Solo puede
 * garantizar la unicidad si se llama siempre con Seep >=0.
 *
 * @param[in] Seed Semilla que permite regenerar el mismo UUID. Si <0, toma
 *            el resultado de clock() para realmente aleatorio.
 * @return Identificador unico. Garantizado por ejecucion.
 */
SuriObject::UuidType GenerateUuid(int Seed) {
   static int staticseed = 0;
   int seed = 0;
   if (Seed < 0) {
      srand(clock());
      seed = rand();
   } else
      seed = ++staticseed;
   char pseedstring[256] = "";
   snprintf(pseedstring, 255, "%d", seed);
   return MD5String(pseedstring);;
}

/**
 * Constructor
 */
SuriObject::SuriObject() :
      hasId_(false), hasClassId_(false), id_(GenerateUuid()), classId_(NullClassId) {
}

/**
 * Constructor
 */
SuriObject::SuriObject(const ClassIdType &ClassId) {
   hasId_ = hasClassId_ = false;
   SetId(CreateId());
   SetClassNameId(ClassId);
}

/**
 * Constructor
 */
SuriObject::SuriObject(const ClassIdType &ClassId, const UuidType &Uuid) {
   hasId_ = hasClassId_ = false;
   SetId(Uuid);
   SetClassNameId(ClassId);
}

/**
 * Destructor
 */
SuriObject::~SuriObject() {
}

/**
 * Retorna el Identificador Unico de la instancia
 */
SuriObject::UuidType SuriObject::GetId() const {
   return id_;
}

/**
 * Setea el Identificador Unico de la instancia (solo se puede setear una vez)
 */
bool SuriObject::SetId(const SuriObject::UuidType& Id) {
   if (hasId_)
      return false;
   id_ = Id;
   hasId_ = true;
   return true;
}

/**
 * Retorna el identificador de la clase
 */
SuriObject::ClassIdType SuriObject::GetClassNameId() const {
   return classId_;
}

/**
 * Setea el Identificador de la instancia (solo se puede setear una vez)
 */
bool SuriObject::SetClassNameId(const SuriObject::ClassIdType& ClassId) {
   if (hasClassId_)
      return false;
   classId_ = ClassId;
   hasClassId_ = true;
   return true;
}

/**
 * Retorna un UUID unico para la aplicacion
 */
SuriObject::UuidType SuriObject::CreateId() {
   return GenerateUuid();
}

/**
 * Retorna un UUID unico para la aplicacion en tipo numerico
 */
SuriObject::UuidIntType SuriObject::CreateIntId(const UuidType &Uuid) {
   static UuidIntType globaluuintid = NullIntUuid;
   if (Uuid == NullUuid) {
      return NullIntUuid;
   }
   static std::map<UuidType, UuidIntType> usedids;
   if (usedids.find(Uuid) != usedids.end())
      return usedids.find(Uuid)->second;
   usedids.insert(std::make_pair(Uuid, ++globaluuintid));
   return usedids.find(Uuid)->second;
}

} /* namespace suri */
