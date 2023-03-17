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
#include <string>

// Includes Suri
#include "ClassInformation.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/** Tipo de clase "Valor no valido" */
const std::string ClassInformation::NDVName = "VALOR NO VALIDO";

/** Indice de la clase "Valor no valido" */
const int ClassInformation::NDVIndex = 255;

/** Tipo de clase "Sin Clase" */
const std::string ClassInformation::NoClassName = "SIN CLASE";

/** Indice de la clase "Sin Clase" */
const int ClassInformation::NoClassIndex = 254;

/** Color Rojo de la clase "Sin Clase" */
const unsigned char ClassInformation::NoClassRedColor = 1;

/** Color Azul de la clase "Sin Clase" */
const unsigned char ClassInformation::NoClassBlueColor = 1;

/** Color Verde de la clase "Sin Clase" */
const unsigned char ClassInformation::NoClassGreenColor = 1;

/** Color Alfa de la clase "Sin Clase" */
const unsigned char ClassInformation::NoClassAlphaColor = 255;

/** Nombre por defecto para las clases con indices no encontrados */
const std::string ClassInformation::DefaultClassNotFoundName = "Clase Desconocida #";

/** Constructor.
 * Los parametros por defecto crean la clase "SIN CLASE"
 */
ClassInformation::ClassInformation(int Index, const std::string& Name,
                                   const VectorStyle::Color& Color,
                                   const SuriObject::UuidType& Id) :
      name_(Name), index_(Index), id_(Id), color_(Color) {
}

/**
 * Destructor
 */
ClassInformation::~ClassInformation() {
}

/**
 * Setea el nombre de la clase
 */
void ClassInformation::SetName(const std::string& Name) {
   name_ = Name;
}

/**
 * Obtiene el nombre de la clase
 */
std::string ClassInformation::GetName() const {
   return name_;
}

/**
 * Setea el indice de la clase
 */
void ClassInformation::SetIndex(IndexType Index) {
   index_ = Index;
}

/**
 * Obtiene el indice de la clase
 */
ClassInformation::IndexType ClassInformation::GetIndex() const {
   return index_;
}

/**
 * Cambia el Id del Datasource
 */
void ClassInformation::SetDatasourceId(const SuriObject::UuidType& Id) {
   id_ = Id;
}

/**
 * Obtiene el Id del Datasource
 */
SuriObject::UuidType ClassInformation::GetDatasourceId() {
   return id_;
}

/**
 * Setea el color de la clase
 */
void ClassInformation::SetColor(const VectorStyle::Color& Color) {
   color_ = Color;
}

/**
 * Obtiene el color de la clase
 */
VectorStyle::Color ClassInformation::GetColor() {
   return color_;
}

/**
 * Determina si dos class information son iguales
 */
bool ClassInformation::Equals(ClassInformation& ClassInfo) {
   bool eqid = ClassInfo.id_ == id_;
   bool eqin = ClassInfo.index_ == index_;
   bool eqnm = ClassInfo.name_.compare(name_) == 0;
   bool eqcl = ClassInfo.color_.Equals(color_);
   return eqid && eqin && eqnm && eqcl;
}

} /** namespace suri */
