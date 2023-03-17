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

#ifndef ACTIVERASTERWORLDEXTENTMANAGER_H_
#define ACTIVERASTERWORLDEXTENTMANAGER_H_

// Includes standard

// Includes suri
#include "WorldExtentManager.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {

// Forwards
class LayerList;
class World;

/** Clase que permite generar un subset con los extents de todos raster activos */
/**
 * Clase que recibe una lista de elementos y un mundo, y asigna al extent del
 * mundo un subset que contenga los extent de todos los elementos activos de la
 * lista.
 */
class ActiveRasterWorldExtentManager : public WorldExtentManager {
public:
   /** Constructor */
   ActiveRasterWorldExtentManager(LayerList* pLayerList, World* pWorld);
   /** Destructor */
   virtual ~ActiveRasterWorldExtentManager();
   /** Metodo que actualiza el mundo */
   virtual void SetExtent();
   /** Devuelve un puntero a LayerList */
   virtual LayerList* GetLayerList();
   /** Devuelve un puntero a World */
   virtual World* GetWorld();

private:
   LayerList* pLayerList_; /*! Lista con elementos que generan el world extent */
   World* pWorld_; /*! Mundo donde se carga el nuevo extent */
};
}

#endif /*ACTIVERASTERWORLDEXTENTMANAGER_H_*/
