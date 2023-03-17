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

#include "ClassificationClassSelectionPart.h"

// Includes standard

// Includes Suri
#include "HtmlTreePart.h"

// Includes Wx
#include "wx/xrc/xmlres.h"
#include "wx/wx.h"
// Includes App

// Defines

namespace suri {
/** Ctor */
ClassificationClassSelectionPart::ClassificationClassSelectionPart(
      LayerList* pLayerList, bool NonClassifiedClassParametersSelector) :
      PartCollection(wxT("ID_CLASS_SELECTION_PANEL"), caption_CLASS_SELECTION_PART) {
   Part *phtmltree = new HtmlTreePart(pLayerList);
   AddPart(phtmltree, wxT("ID_LAYER_LIST_PANEL"));

   // Seteo flag para ocultar part con propiedades de clase 'Sin clase'
   nonClassifiedClassParametersSelector_ = NonClassifiedClassParametersSelector;
}

/** Dtor */
ClassificationClassSelectionPart::~ClassificationClassSelectionPart() {
}

/* Oculto propiedades de clase 'sin clase'
 * Es necesario para el caso de estadisticas. */
bool ClassificationClassSelectionPart::ConfigureWidget() {
   PartCollection::ConfigureWidget();
   if (!nonClassifiedClassParametersSelector_) {
      GetWidget()->Hide("ID_NONCLASSIFIEDCLASS_PROPERTIES_STATICBOX");
   }

   return true;
}
}
