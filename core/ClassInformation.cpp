/*! \file ClassInformation.cpp */
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
