/*! \file EnclosureInformation.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */



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
