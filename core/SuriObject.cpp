/*! \file SuriObject.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
