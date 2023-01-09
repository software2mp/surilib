/*! \file Filter.cpp */
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
#include <set>
#include <utility>
#include <vector>

// Includes Suri
#include "Filter.h"
#include "suri/AuxiliaryFunctions.h"

// Includes Wx

// Defines
#define SQL_SIMPLE_SELECT_STRING "SELECT * FROM"
#define SQL_SELECT_STRING "SELECT FID, * FROM"
#define SQ_SELECT_WHERE_CLAUSE "WHERE"
#define SQL_ID_FIELD_NAME "FID"
// forwards

namespace suri {

/** Ctor */
Filter::Filter() {
}
/** ctor copia */
Filter::Filter(const Filter& OtherFilter) {
   equalConstraints_ = OtherFilter.equalConstraints_;
   minorConstraints_ = OtherFilter.minorConstraints_;
   greaterConstraints_ = OtherFilter.greaterConstraints_;
   filteredName_ = OtherFilter.filteredName_;
   idFieldName_ = OtherFilter.idFieldName_;
}

/** Constructor a partir de un string **/
Filter::Filter(const std::string &Query) :
      query_(Query) {
   if (!Query.find("LIKE") == std::string::npos) {
      // Obtiene el nombre de la capa sobre la cual se filtra
      size_t frompos = query_.find("FROM");
      size_t wherepos = query_.find("WHERE");
      if (wherepos != std::string::npos && frompos != std::string::npos) {
         std::string sub = query_.substr(frompos + sizeof("FROM"),
                                         wherepos - frompos - sizeof("FROM"));
         filteredName_ = trim(sub, " ");
      }
      // Obtiene el campo sobre el cual se aplica el filtrado
      size_t inpos = query_.find("IN");
      if (wherepos != std::string::npos && inpos != std::string::npos) {
         std::string sub = query_.substr(wherepos + sizeof("WHERE"),
                                        inpos - wherepos - sizeof("WHERE"));
         idFieldName_ = trim(sub, " ");
      }
      ParseEqualConditions(query_);
   }
}

/** Dtor */
Filter::~Filter() {
}

/**
 *
 * Genera la consulta en base a las restricciones cargadas en el filtro
 * Obtiene string asociado al query.
 * @return
 */
std::string Filter::GenerateQueryAsString()  const {
   if (equalConstraints_.size() == 0) {
      return query_;
   }
   std::string query = SQL_SIMPLE_SELECT_STRING;
   query += " ";
   query += filteredName_;
   query += " WHERE ";
   query += idFieldName_;
   query += " IN(";
   ConstraintSet::const_iterator it = equalConstraints_.begin();
   for (; it != equalConstraints_.end(); ++it) {
      query += NumberToString<int>(*it);
      ++it;
      if (it != equalConstraints_.end()) {
         query += ",";
      }
      --it;
   }
   query += ")";
   return query;
}


/** Agrega una restriccion de igual aplicada a un id determinado
 *  @param[in] Id id sobre el cual se agrega la restriccion
 *  @return true en caso de agregar correctamente la restrticcion
 *  @return false en caso contrario.
 */
bool Filter::AddEqualConstraint(int Id) {
   std::pair<ConstraintSet::iterator, bool> returnvalue;
   returnvalue = equalConstraints_.insert(Id);
   return (returnvalue.second != false);
}

/** Elimina la restriccion de igual aplicada sobre un id determinado
 *  @param[in] Id id a buscar sobre la restriccion de igual
 *  @return true en caso de eliminar la restriccion de igual aplicada al id
 *  @return false en caso contrario
 */
bool Filter::RemoveEqualConstraint(int Id) {
   return equalConstraints_.erase(Id) > 0;
}

/** Agrega una restriccion de mayor aplicada a un id determinado
 *  @param[in] Id id sobre el cual se agrega la restriccion
 *  @return true en caso de agregar correctamente la restrticcion
 *  @return false en caso contrario.
 */
bool Filter::AddGreaterConstraint(int Id) {
   std::pair<ConstraintSet::iterator, bool> returnvalue;
   returnvalue = greaterConstraints_.insert(Id);
   return (returnvalue.second != false);
}

/** Elimina la restriccion de mayor aplicada sobre un id determinado
 *  @param[in] Id id a buscar sobre la restriccion de mayor
 *  @return true en caso de eliminar la restriccion de mayor aplicada al id
 *  @return false en caso contrario
 */
bool Filter::RemoveGreaterConstraint(int Id) {
   return greaterConstraints_.erase(Id) > 0;
}

/** Agrega una restriccion de menor aplicada a un id determinado
 *  @param[in] Id id sobre el cual se agrega la restriccion
 *  @return true en caso de agregar correctamente la restrticcion
 *  @return false en caso contrario.
 */
bool Filter::AddMinorConstraint(int Id) {
   std::pair<ConstraintSet::iterator, bool> returnvalue;
   returnvalue = minorConstraints_.insert(Id);
   return (returnvalue.second != false);
}

/** Elimina la restriccion de menor aplicada sobre un id determinado
 *  @param[in] Id id a buscar sobre la restriccion de menor
 *  @return true en caso de eliminar la restriccion de menor aplicada al id
 *  @return false en caso contrario
 */
bool Filter::RemoveMinorConstraint(int Id) {
   return minorConstraints_.erase(Id) > 0;
}

/**
 * Retorna cual es el nombre del objeto a filtrar (clasula FROM)
 * @return nombre del objeto a filtrar.
 **/
std::string Filter::GetFilteredName() const {
   return filteredName_;
}

/**
 * Configura cual es el nombre del objeto a filtrar (clasula FROM)
 *  @param[in] nombre del objeto sobre el cual se aplica el filtro (ej. nombre capa)
 */
void Filter::SetFilteredName(const std::string& FilteredName) {
   // Es necesario agregar comillas simple por si el nombre de la capa tiene espacios
   filteredName_ = "'" + FilteredName + "'";
}

/**
 * Configura cual es el nombre del campo correspondiente al id (clasula WHERE)
 *  @param[in] IdFieldName nombre del campo correspondiente al id
 */
void Filter::SetIdFieldName(const std::string& IdFieldName) {
   idFieldName_ = IdFieldName;
}

/** Elimina todas las restricciones impuestas al filtro. */
void Filter::ClearFilter() {
   equalConstraints_.clear();
   minorConstraints_.clear();
   greaterConstraints_.clear();
}

/** Metodo auxiliar para parsear las condiciones de = o IN **/
void Filter::ParseEqualConditions(const std::string& Query) {
   // Parseo las condiciones de igual
   size_t startinpos = Query.find("IN(");
   size_t finishinpos = Query.find(")");
   if (startinpos != std::string::npos && finishinpos != std::string::npos) {
      std::string equals = Query.substr(startinpos + sizeof("IN(") - 1,
                                        finishinpos - startinpos - sizeof(")") - 1);
      equals = trim(equals, " ");
      std::vector<std::string> eqcons = tokenizer(equals, ",");
      std::vector<std::string>::iterator it = eqcons.begin();
      for (; it != eqcons.end(); ++it) {
         equalConstraints_.insert(StringToNumber<int>(*it));
      }
   }
}

/** Metodo auxiliar para parsear las condiciones de Like **/
void Filter::ParseLikeConditions(const std::string& Query) {
     // TODO (Federico - TCK #4649): es necesario implementar un sql parser
     // por la complejidad de las consultas
}

} /** namespace suri */
