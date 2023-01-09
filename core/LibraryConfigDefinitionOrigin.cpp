/** \file LibraryConfigDefinitionOrigin.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <string>

#include "LibraryConfigDefinitionOrigin.h"
#include "LibraryConfigDefinitionOriginSource.h"

namespace suri {

/**
 * Ctor.
 */
LibraryConfigDefinitionOrigin::LibraryConfigDefinitionOrigin() {

}

/**
 * Dtor.
 */
LibraryConfigDefinitionOrigin::~LibraryConfigDefinitionOrigin() {

}

/**
 * Devuelve el nombre del origen.
 */
std::string LibraryConfigDefinitionOrigin::GetName() const {
   return name_;
}

/**
 * Establece el nombre del origen.
 */
void LibraryConfigDefinitionOrigin::SetName(const std::string& Name) {
   name_ = Name;
}

/**
 * Devuelve los permisos disponibles sobre el origen.
 */
int LibraryConfigDefinitionOrigin::GetCapabilities() const {
   return capabilities_;
}

/**
 * Establece los permisos sobre el origen.
 */
void LibraryConfigDefinitionOrigin::SetCapabilities(int Capabilities) {
   capabilities_ = Capabilities;
}

/**
 * Devuelve el nivel del origen.
 */
int LibraryConfigDefinitionOrigin::GetLevel() const {
   return level_;
}

/**
 * Establece el nivel del origen.
 */
void LibraryConfigDefinitionOrigin::SetLevel(int Level) {
   level_ = Level;
}

/**
 * Devuelve la fuente del origen.
 */
LibraryConfigDefinitionOriginSource* LibraryConfigDefinitionOrigin::GetSource() const {
   return pSource_;
}

/**
 * Establece la fuente del origen.
 */
void LibraryConfigDefinitionOrigin::SetSource(LibraryConfigDefinitionOriginSource* pSource) {
   pSource_ = pSource;
}

}  // namespace suri
