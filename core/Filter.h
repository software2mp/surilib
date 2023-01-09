/*! \file Filter.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FILTER_H_
#define FILTER_H_

// Includes Estandar
#include <string>
#include <set>

// Includes Suri

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Clase encargada de generar un filtro en base a las restricciones indicadas sobre ids.
 */
class Filter {

public:
   /** Ctor */
   Filter();
   /** Ctor a partir de un string*/
   explicit Filter(const std::string &Query);
   /** ctor copia */
   Filter(const Filter&);
   /** Dtor */
   virtual ~Filter();
   /** Genera la consulta en base a las restricciones cargadas en el filtro en un string **/
   std::string GenerateQueryAsString() const;
   /** Agrega una restriccion de igual aplicada a un id determinado*/
   bool AddEqualConstraint(int Id);
   /** Elimina la restriccion de igual aplicada sobre un id determinado */
   bool RemoveEqualConstraint(int Id);
   /** Agrega una restriccion de mayor aplicada a un id determinado */
   bool AddGreaterConstraint(int id);
   /** Elimina la restriccion de mayor aplicada sobre un id determinado */
   bool RemoveGreaterConstraint(int Id);
   /** Agrega una restriccion de menor aplicada a un id determinado */
   bool AddMinorConstraint(int Id);
   /** Elimina la restriccion de menor aplicada sobre un id determinado */
   bool RemoveMinorConstraint(int Id);
   /** Retorna cual es el nombre del objeto a filtrar (clasula FROM)**/
   std::string GetFilteredName() const;
   /** Configura cual es el nombre del objeto a filtrar (clasula FROM)**/
   void SetFilteredName(const std::string& FilteredName);
   /** Configura cual es el nombre del campo correspondiente al id */
   void SetIdFieldName(const std::string& IdFieldName);
   /** Elimina todas las restricciones impuestas al filtro.  */
   void ClearFilter();

private:
   /** Metodo auxiliar para parsear las condiciones de = o IN **/
   void ParseEqualConditions(const std::string& Query);
   /** Metodo auxiliar para parsear las condiciones de Like **/
   void ParseLikeConditions(const std::string& Query);
   /** nombre del objeto sobre el cual se aplica el filtro **/
   std::string filteredName_;
   /** nombre del campo en donde se aplica el filtro*/
   std::string idFieldName_;
   typedef std::set<int> ConstraintSet;
   /** id con restricciones de igual */
   ConstraintSet equalConstraints_;
   /** id con restricciones de menor */
   ConstraintSet minorConstraints_;
   /** id con restricciones de mayor */
   ConstraintSet greaterConstraints_;
   /** texto con restricciones de like **/
   std::set<std::string> likeConstraints_;
   /** string que contiene el query **/
   std::string query_;
};

} /** namespace suri */

#endif /* FILTER_H_ */
