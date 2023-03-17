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

#ifndef TERRAINPROPERTIESPART_H_
#define TERRAINPROPERTIESPART_H_

// Includes suri
#include "suri/Part.h"
#include "suri/TerrainElement.h"

/** namespace suri */
namespace suri {
/** Declara la clase TerrainPropertiesPartEvent */
DECLARE_EVENT_OBJECT(TerrainPropertiesPartEvent);

/** Part con controles para las propiedades de un terreno */
/**
 * Tiene los controles calidad, exageracion y velocidad camara.
 * \note extiende Part para guardar los cambios en propiedades del terreno
 */
class TerrainPropertiesPart : public Part {
   /** Ctor. de Copia. */
   TerrainPropertiesPart(const TerrainPropertiesPart &TerrainPropertiesPart);

public:
   /** Declara friend la clase TerrainPropertiesPartEvent */
   FRIEND_EVENT_OBJECT(TerrainPropertiesPartEvent);
   /** Ctor. */
   TerrainPropertiesPart(TerrainElement *pElement);
   /** Dtor. */
   virtual ~TerrainPropertiesPart();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
   /** Inicializa los controles */
   virtual void SetInitialValues();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Obtiene el icono del part */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
   /** Actualiza el estado de la parte */
   virtual void Update() {
   }
protected:
private:
   /** Evento de movimiento de sliders */
   void OnSliderScroll(wxScrollEvent &Event);

   TerrainElement *pElement_; /*! elemento Terreno. */
   EVENT_OBJECT_PROPERTY(TerrainPropertiesPartEvent); /*! Objeto que conecta */
   /* los eventos */
};
}  // namespace suri

#endif /*TERRAINPROPERTIESPART_H_*/
