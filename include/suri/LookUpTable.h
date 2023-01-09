/*! \file LookUpTable.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#ifndef LOOKUPTABLE_H_
#define LOOKUPTABLE_H_

// Includes estandar
#include <map>
#include <limits>

// Includes Suri
#include "logmacros.h"

// Includes Wx
#include "wx/xml/xml.h"

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Clase que representa una tabla de busqueda de valores */
/**
 *  Esta clase representa una tabla de busqueda de valores.
 *  Conceptualmente se define como una serie de limites, que definen intervalos
 * para los cuales se tiene un valor de salida asociado.
 */
class LookUpTable {
public:
   /** Tipo que se utiliza para guardar la tabla */
   typedef std::map<double, double> LutType;
   /** ctor */
   LookUpTable();
   /** dtor */
   ~LookUpTable();

// ------------------- METODOS DE SERIALIZACION DE UNA TABLA -------------------
   /** Metodo que lee un xml y carga la tabla */
   static bool FromXml(const wxXmlNode *pLutNode, LookUpTable &Parameters);
   /** Metodo para convertir una tabla a xml */
   static bool ToXml(const LookUpTable &Parameters, wxXmlNode* &pLutNode);
   /** Convierte la tabla local en xml */
   bool ToXml(wxXmlNode *pLutNode) const;

// -------------------- METODO QUE APLICA LA TRANSFORMACION --------------------
   /** Metodo que aplica la tabla para un dato de entrada (tipo de dato nativo) */
   double ApplyLut(const double InputData) const;
   /** Template que aplica la tabla convirtiendo los tipos de datos */
   template<class T>
   T ApplyLut(const T InputData) const {
      return static_cast<T>(ApplyLut(static_cast<double>(InputData)));
   }
// ---------------------------- METODOS DE ACCESO ------------------------------
   /** Agrega un limite a la tabla con su salida correspondiente (tipo de dato nativo) */
   bool AddBin(const double LowerBound, const double Output);
   /** Template que agrega un limite convirtiendo los tipos de datos */
   template<class T1, class T2>
   bool AddBin(const T1 LowerBound, const T2 Output) {
      return AddBin(static_cast<double>(LowerBound), static_cast<double>(Output));
   }
   /** Carga la tabla interna o la inicializa en vacio */
   void SetTable(const LutType &Table = LutType());
   /** Obtiene una copia de la tabla interna */
   void GetTable(LutType &Table) const;
private:
   double defaultValue_; /*! Valor default de la tabla no se guarda en XML */
   LutType lookUpTable_; /*! tabla de busqueda */
};
}  // namespace suri

#endif /* LOOKUPTABLE_H_ */
