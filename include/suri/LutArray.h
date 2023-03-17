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

#ifndef LUTARRAY_H_
#define LUTARRAY_H_
// Includes estandar
#include <vector>

// Includes Suri
#include "suri/LookUpTable.h"

// Includes Wx
#include "wx/xml/xml.h"

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Clase que almacena las tablas de busqueda para una imagen */
/**
 *  Representa un array de LookUpTables.
 *
 *  Las tablas se organizan en funcion del indice de la banda de salida, cada
 * una tiene el indice de la banda de entrada asociada.
 *
 *  Define el estado de activacion de la tabla. \todo Pasar a los renderizadores
 */
class LutArray {
public:
   /** ctor */
   LutArray();
   /** dtor */
   virtual ~LutArray();

// -------------- METODOS DE SERIALIZACION DE UN ARRAY DE TABLAS ---------------
   /** Inicializa un array en funcion de un Nodo xml */
   static bool FromXml(const wxXmlNode *pLutArrayNode, LutArray &Parameters);
   /** Convierte un array a un Nodo xml */
   static bool ToXml(const LutArray &Parameters, wxXmlNode* &pLutArrayNode);
   /** Convierte el array a un Nodo xml */
   bool ToXml(wxXmlNode* &pLutArrayNode) const;
   /** Obtiene la cantidad de tablas */
   int GetCount() const;
   /** Retorna la tabla y banda de entrada para una banda de salida dada */
   LookUpTable GetLookUpTable(int OutputBand, int &InputBand) const;
   /** Agrega una tabla con su banda de entrada */
   void AddLookUpTable(const LookUpTable &Lut, const int InputBand);
   /** Retorna el nombre de la lut */
   std::string GetName() const;
   /** Modifica el nombre de la lut */
   void SetName(const std::string &LutName);

   bool active_; /*! Propiedad que habilita la tabla */
private:
   std::string lutName_; /*! contenido de la propiedad nombre en el nodo lut */
   /** Tipo que almacena la banda de salida y la tabla */
   typedef std::pair<int, LookUpTable> LookUpTablePair; /*! todo:documentar */
   /** Tipo que almacena el array */
   typedef std::vector<LookUpTablePair> LookUpTableArray;
// La posicion del vector es la salida, first es la entrada y LookUpTable la tabla
   LookUpTableArray lutArray_; /*! Array de LookUpTable */
};
}

#endif /* LUTARRAY_H_ */
