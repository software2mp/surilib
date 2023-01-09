/*! \file LibraryItemAttribute.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/LibraryItemAttribute.h"

// Includes Wx

// Defines

// forwards

namespace suri {


const std::string LibraryItemAttribute::PrincipalAttrName = "Principal";
const std::string LibraryItemAttribute::ExtendedAttrName = "Extended";


/** Constructor por defecto de la instancia
 * @param[in] Name nombre del atributo
 * @param[in] Value valor del atributo
 * @param[in] AttributeType tipo del atributo
 **/
LibraryItemAttribute::LibraryItemAttribute(const std::string& Name,
                                           const std::string& Value,
                                           AttributeTypeEnum AttributeType) :
      name_(Name), type_(AttributeType), value_(Value) {
}

/** Destructor **/
LibraryItemAttribute::~LibraryItemAttribute() {
}

/**
 * Devuelve el nombre asociado al atributo
 * @return string con el nombre del atributo
 */
std::string LibraryItemAttribute::GetName() const {
   return name_;
}

/**
 * Devuelve el tipo asociado al atributo
 * @return string con el tipo del atributo
 */
LibraryItemAttribute::AttributeTypeEnum LibraryItemAttribute::GetType() const {
   return type_;
}

/**
 * Devuelve el valor asociado al atributo
 * @return string con el valor del atributo
 */
std::string LibraryItemAttribute::GetValue() const {
   return value_;
}

/** Asocia un valor al atributo */
void LibraryItemAttribute::SetValue(std::string Value) {
   value_ = Value;
}

/** Compara si dos atributos son iguales **/
bool LibraryItemAttribute::Equals(const LibraryItemAttribute& Attribute) const {
   return value_.compare(Attribute.GetValue()) == 0;
}

} /** namespace suri */
