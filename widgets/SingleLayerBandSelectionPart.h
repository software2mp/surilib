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

#ifndef SINGLELAYERBANDSELECTIONPART_H_
#define SINGLELAYERBANDSELECTIONPART_H_

// Includes standar
#include <vector>
#include <list>
#include <utility>

// Icludes suri
#include "suri/Part.h"
#include "suri/RasterElement.h"

// Includes wx
#include "wx/wx.h"
#include "wx/listctrl.h"

#include "SelectionPart.h"

namespace suri {

DECLARE_EVENT_OBJECT(SingleLayerBandSelectionEvent);

/**
 * Clase que representa un part para la seleccion de bandas desde
 * una sola capa.
 */
class SingleLayerBandSelectionPart : public SelectionPart {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(SingleLayerBandSelectionEvent);

   /** Ctor */
   SingleLayerBandSelectionPart(RasterElement * pElement);

   /** Dtor */
   virtual ~SingleLayerBandSelectionPart();

   /** Crea la ventana de la herramienta */
   virtual bool CreateToolWindow();

   /** Devuelve la lista de bandas seleccionadas en la lista de entrada */
   virtual std::vector<BandInfo> GetInputSelectedBands();

   /** Devuelve el nombre de todas las banda disponibles */
   virtual std::vector<BandInfo> GetAvailableBands() const;

   /**
    * Ajusta el largo de la ultima columna para que ocupe todo el espacio
    * libre en el cotrol lista. Se llama desde el OnResize de la clase
    * que lo contiene.
    */
   virtual void UpdateSourceCtrl();

private:
   EVENT_OBJECT_PROPERTY(SingleLayerBandSelectionEvent); /*! Objeto para conectar eventos */

   RasterElement* pElement_;
   std::vector<BandInfo> bands_;

   /** Configura la lista donde se cargan las bandas de entrada */
   void ConfigureInputBandList();

   /** Carga la lista de bandas de entrada */
   void LoadInputBands();

   /** Metodo que se ejecuta al hacer doble click en la lista de entrada */
   void OnInputSelection(wxListEvent &Event);

};

}  // namespace suri

#endif  // SINGLELAYERBANDSELECTIONPART_H_
