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

#ifndef FILTEREDVECTOR_H_
#define FILTEREDVECTOR_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/Vector.h"
#include "FilterInterpreterInterface.h"
#include "Filter.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Clase que representa un vector sobre el cual se aplica un filtro
 * (funciona a modo de adapter). Contiene una capa sobre la cual se aplica
 * un filtro sql
 */
class FilteredVector : public suri::Vector, public suri::FilterInterpreterInterface {
   /** ctor copia **/
   FilteredVector(const FilteredVector&);

public:
   /** ctor */
   FilteredVector(Vector* pVector, Filter* pFilter);
   /** dtor */
   virtual ~FilteredVector();
   /** Acceso a la capa por indice
   * @param LayerIndex
   * @return capa filtrada
   * @return NULL en caso de que el filtro no coincida con ningun registro*/
   virtual OGRLayer *GetLayer(int LayerIndex) const;
   /** Devuelve el indice a la capa por nombre (-1 si falla) */
   virtual int GetLayerIndex(const std::string &LayerName) const;
   /** Configura el filtro a aplicar */
   void SetFilter(Filter* pFilter);
   /**  Interpreta el filtro pasado por parametro.  */
   virtual bool Interpret(Filter* pFilter);
   /** Devuelve el resultado de la interpretacion del filtro por un Vector*/
   virtual Vector* GetInterpretationAsVector();
   /** Devuelve el resultado de la interpretacion del filtro por un Raster*/
   virtual Image* GetInterpretationAsRaster();
   /** Devuelve el resultado de la interpretacion del filtro representado por una Tabla*/
   virtual Table* GetInterpretationAsTable();

private:
   /** Metodo auxiliar que actualiza la capa filtrada en funcion del filtro*/
   void UpdateFilteredLayer();
   /** Vector sobre  el cual aplicara el filtro **/
   Vector* pVector_;
   /** Filtro a aplicar */
   Filter* pFilter_;
   /** Capa resultante del aplicado del filtro al vector **/
   OGRLayer* pFilteredLayer_;
};

} /** namespace suri */

#endif /* FILTEREDVECTOR_H_ */
