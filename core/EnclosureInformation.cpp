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
#include<string>

// Includes Suri
#include "EnclosureInformation.h"

// Includes Wx

// Defines

// forwards

namespace suri {

const std::string EnclosureInformation::TypeNameDefault = "tipo_Area";
const std::string EnclosureInformation::DescNameDefault = "desc_Area";

/**
 * Constructor
 */
EnclosureInformation::EnclosureInformation(const std::string& LayerName,
                                           const std::string& Query,
                                           EnclosureType Etype,
                                           const std::string& TypeName,
                                           const std::string& DescName) :
      layerName_(LayerName), query_(Query), enclosureType_(Etype),
      typeArea_(TypeName, TypeRecord::Name),
      descArea_(DescName, DescRecord::Name) {
}

/**
 * Destructor
 */
EnclosureInformation::~EnclosureInformation() {
}


/*
 * Devuelve el nombre de la capa
 * @return string con el nombre de la capa
 */
std::string EnclosureInformation::GetLayerName() const {
   return layerName_;
}

/**
 * A partir de una condicion devuelve una query asociada
 * @param[in] Condition condicion para obtener la query
 * @return string con la query formada
 */
std::string EnclosureInformation::SetQuery(const std::string& Condition) {
   std::string query = "SELECT FID, * FROM";
   query += layerName_;
   query += " WHERE ";
   query += typeArea_.name_;
   query += Condition;
   query_ = query;
   return query;
}

/**
 *  Obtiene la query asociada al recinto
 *  @return string con el query
 */
std::string EnclosureInformation::GetQuery() const {
   return query_;
}

/**
 *  Obtiene el tipo de dato asociado al campo tipo de area
 *  @return string con el tipo
 */
std::string EnclosureInformation::GetTypeAreaFieldType() const {
   return typeArea_.type_;
}

/*
 * Devuelve el nombre del campo tipo de area
 * @return string con el nombre
 */
std::string EnclosureInformation::GetTypeAreaName() const {
   return typeArea_.name_;
}

/**
 *  Obtiene el tipo de dato asociado al campo descripcion tipo de area
 *  @return string con el tipo
 */
std::string EnclosureInformation::GetDescFieldType() const {
   return descArea_.type_;
}

/*
 * Devuelve el nombre del campo descripcion del area
 * @return string con el nombre
 */
std::string EnclosureInformation::GetDescAreaName() const {
   return descArea_.name_;
}

/** Devuelve el tipo de recinto (entrenamiento o test). **/
EnclosureInformation::EnclosureType EnclosureInformation::GetEnclosureType() const {
   return enclosureType_;
}

} /** namespace suri */
