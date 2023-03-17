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

#ifndef WORLDEXTENTMANAGER_H_
#define WORLDEXTENTMANAGER_H_

// Includes standard

// Includes suri

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {

// Forwards
class LayerList;
class World;

/** Interfaz que permite definir la forma en que se modifica la ventana. */
/**
 * Renderization manager utiliza SetExtent cada vez que se
 * renderiza para determinar el extent del mundo.
 * El objetivo de esta clase es permitir mayor control sobre el
 * algoritmo que se usa a las clases externas.
 * Se puede utilizar WorldExtentManager directamente para que
 * no se modifique el mundo al renderizar
 */
class WorldExtentManager {
public:
   /** Constructor */
   WorldExtentManager() {};
   virtual ~WorldExtentManager() {};
   /** Metodo que actualiza el mundo */
   virtual void SetExtent()=0;
   /** Devuelve un puntero a LayerList */
   virtual LayerList* GetLayerList()=0;
   /** Devuelve un puntero a World */
   virtual World* GetWorld()=0;
};

}

#endif /*WORLDEXTENTMANAGER_H_*/
