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

#ifndef MULTILAYERBANDSELECTIONPART_H_
#define MULTILAYERBANDSELECTIONPART_H_

#include <vector>
#include <list>
#include <utility>
#include <cstddef>
#include <sstream>
#include <string>

// Icludes suri
#include "suri/Part.h"
#include "suri/RasterElement.h"
#include "SelectionPart.h"

// Includes wx
#include "wx/wx.h"
#include "wx/treectrl.h"

namespace suri {

/**
 * Clase para poder guardar informacion asociada al item del control wxTreeCtrl
 * (se debe utilizar asi ya que no hay consistencia en la interfaz de los controles
 * y este control en particular se maneja de esta manera)
 */
class CustomItemData : public wxTreeItemData {
public:
   /** Ctor */
   explicit CustomItemData(BandInfo pBandInfoP) { pBandInfo_ = pBandInfoP; }

   /** Dtor */
   virtual ~CustomItemData() { }

   /** Devuelve la informacion que se quiere asociar al item */
   BandInfo GetBandInfo() { return pBandInfo_; }

private:
   BandInfo pBandInfo_;  /** Informacion que se quiere guardar */
};

DECLARE_EVENT_OBJECT(MultiLayerBandSelectionEvent);

/**
 * Clase que representa un part para la seleccion de bandas desde
 * varias capas.
 */
class MultiLayerBandSelectionPart : public SelectionPart {
public:
   FRIEND_EVENT_OBJECT(MultiLayerBandSelectionEvent);

   /**
    * Ctor
    */
   explicit MultiLayerBandSelectionPart(const std::vector<RasterElement*>& ElementsP);

   /**
    * Dtor
    */
   virtual ~MultiLayerBandSelectionPart();

   /**
    * Crea la ventana de la herramienta
    */
   virtual bool CreateToolWindow();

   /**
    * Devuelve la lista de bandas seleccionadas en la lista de entrada
    */
   virtual std::vector<BandInfo> GetInputSelectedBands();

   /**
    * Devuelve el nombre de todas las banda disponibles
    */
   virtual std::vector<BandInfo> GetAvailableBands() const;

   /**
    * Realiza ajustes en el part. La mensaje se recibe desde el control
    * que lo contiene.
    */
   virtual void UpdateSourceCtrl();

   /**
    * Maneja la notificacion de que la lista (o control) de destino
    * esta vacia.
    */
   virtual void OnOutputEmpty();

   /** */
   virtual void OnInputRequested();

private:
   EVENT_OBJECT_PROPERTY(MultiLayerBandSelectionEvent);

   std::vector<RasterElement*> pElements_;
   std::vector<BandInfo> bands_;
   BandInfo pBaseBand_;
   bool hasBaseBand_;

   /**
    * Configura el control wxTreeCtrl.
    */
   void ConfigureTreeControl();

   /**
    * Carga el control con los datos de cada imagen.
    */
   void LoadTreeControl(const std::vector<RasterElement*>& pElementsP);

   /**
    * Recibe el mensaje cuando se activo un item con doble-click
    */
   void OnTreeItemActivated(wxTreeEvent &Event);

   /**
    * Actualiza el estado de los items poniendo en cursiva y deshabilitando
    * aquellos que no son compatibles.
    */
   void UpdateItemsStatus(wxTreeItemId ParentItemId);
};

}

#endif  // MULTILAYERBANDSELECTIONPART_H_
