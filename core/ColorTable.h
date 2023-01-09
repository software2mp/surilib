/*! \file ColorTable.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COLORTABLE_H_
#define COLORTABLE_H_

#include <utility>
#include <string>
#include <vector>

// Includes suri
#include "suri/LutArray.h"

/** namespace suri */
namespace suri {
/**
 * Permite configurar una tabla de colores. Tiene una sola banda de entrada y
 * tres de salida.
 */
class ColorTable {
public:
   /** Ctor */
   ColorTable();
   /** Dtor */
   ~ColorTable();
   /** Inicializa un array en funcion de un Nodo xml */
   static bool FromXml(const wxXmlNode *pColorTableNode, ColorTable &CTable);
   /** Convierte un array a un Nodo xml */
   static bool ToXml(const ColorTable &CTable, wxXmlNode* &pColorTableNode);
   /** Convierte un array a un Nodo xml */
   static bool ToXml(const ColorTable &CTable, wxXmlNode* &pColorTableNode,
                     std::vector<std::pair<std::string, std::string> > Labels);
   /** Agrega el comienzo de un intervalor y el color asociado a la tabla de colores. */
   void AddColor(double LowerBound, int Red, int Green, int Blue);
   /** Elimina el contenido de la tabla */
   void ResetColorTable();
   /** Retorna el nombre de la tabla de colores */
   std::string GetName() const;
   /** Modifica el nombre de la tabla de colores */
   void SetName(const std::string &LutName);
   /** Informa si la tabla de colores esta activa */
   bool IsActive() const;
   /** Modifica el estado de activacion de la tabla de colores */
   void Activate(bool Active);
   /** Compara valores del tipo ColorTable. */
   bool operator==(const ColorTable &ColorTableRg) const;
   /** Retorna la tabla y banda de entrada para una banda de salida dada */
   LookUpTable GetLookUpTable(int OutputBand) const;

private:
   /** Verifica que tenga una banda de entrada y tres de salida. */
   static bool VerifyLutArray(const LutArray &Parameters);
   LutArray lut_; /*! LutArray con tabla de colores. */
};

/*//! Compara valores del tipo ColorTable.
 bool operator==(const ColorTable &ColorTableLf,
 const ColorTable &ColorTableRg);*/
}

#endif /* COLORTABLE_H_ */
