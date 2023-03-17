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

#include "EnhancementSelectionTest.h"

// Inclues suri
#include "suri/RasterElement.h"
#include "suri/PartContainerWidget.h"
#include "EnhancementSelectionPart.h"
#include "suri/messages.h"
#include "suri/Configuration.h"
#include "Enhancement.h"

/** namespace suri */
namespace suri {
/**
 * Constructor
 */
EnhancementSelectionTest::EnhancementSelectionTest() {
}

/**
 * Destructor
 */
EnhancementSelectionTest::~EnhancementSelectionTest() {
}

/**
 * Crea un elemento raster, crea una parte de seleccion de realce, y crea un
 * PartContainer para mostrar la parte.
 * Se pregunta al usuario si aparecio la lista de realces.
 * @return true si el usuario contesta SI
 * @return false si el usuario contesta NO.
 */
bool EnhancementSelectionTest::CreateElementPropertiesWindow() {
   bool returnvalue = false;

   RasterElement * pelement = RasterElement::Create("c:/EnhancementListTest.tif");
   if (!pelement) {
      return false;
   }
   Part *ppart = new EnhancementSelectionPart(pelement);
   PartContainerWidget * pwidget = new PartContainerWidget(ppart);

   pwidget->ShowModal();
   switch(SHOWQUERY(NULL, wxT("Aparecio la lista de realces?"),
                      wxYES|wxNO|wxICON_QUESTION))
         {
            case wxID_YES:
            returnvalue = true;
            break;
            case wxID_NO:
            returnvalue = false;
            break;
         }
   delete pelement;
   CPPUNIT_ASSERT_MESSAGE("No aparecio la lista de realces", returnvalue);
   return returnvalue;
}
}
