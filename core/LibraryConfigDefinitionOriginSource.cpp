/** \file LibraryConfigDefinitionOriginSource.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <string>

#include "LibraryConfigDefinitionOriginSource.h"

namespace suri {

const std::string LibraryConfigDefinitionOriginSource::kTypeTextfile = "textfile";
const std::string LibraryConfigDefinitionOriginSource::kTypeGdal = "gdal" ;

/**
 * Ctor.
 */
LibraryConfigDefinitionOriginSource::LibraryConfigDefinitionOriginSource() {

}

/**
 * Dtor.
 */
LibraryConfigDefinitionOriginSource::~LibraryConfigDefinitionOriginSource() {

}

/**
 * Devuelve el nombre de la fuente de datos. Ej: de ser un archivos
 * de texto plano devolveria el nombre del mismo.
 */
std::string LibraryConfigDefinitionOriginSource::GetName() const {
   return name_;
}

/**
 * Establece el nombre del origen de datos.
 */
void LibraryConfigDefinitionOriginSource::SetName(const std::string& Name) {
   name_ = Name;
}

/**
 * Devuelve el tipo de la fuente de datos.
 */
std::string LibraryConfigDefinitionOriginSource::GetType() const {
   return type_;
}

/**
 * Establece el tipo del origen de datos.
 */
void LibraryConfigDefinitionOriginSource::SetType(const std::string& Type) {
   type_ = Type;
}

/**
 * Devuelve la ruta de la fuente de datos. Ej: de ser un archivo
 * de texto plano seria la ruta en el sistema de archivos.
 */
std::string LibraryConfigDefinitionOriginSource::GetPath() const {
   return path_;
}

/**
 * Establece la ruta del origen de datos.
 */
void LibraryConfigDefinitionOriginSource::SetPath(const std::string& Path) {
   path_ = Path;
}

/**
 * Devuelve el tipo de item. Ej: si es sist. georreferenciado,
 * proyectado, datum o elipsoide.
 */
int LibraryConfigDefinitionOriginSource::GetItemType() const {
   return itemType_;
}

/**
 * Establece el tipo de item.
 */
void LibraryConfigDefinitionOriginSource::SetItemType(int ItemType) {
   itemType_ = ItemType;
}

}  // namespace suri
