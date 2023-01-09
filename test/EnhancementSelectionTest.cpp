/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
