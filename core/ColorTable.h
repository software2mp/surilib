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
