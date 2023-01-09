/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "LatitudeLongitudeInputWidgetTest.h"
#include "suri/Coordinates.h"
#include "wx/window.h"

/** namespace suri */
namespace suri {
/**
 * Constructor
 * @param[in] pWorld puntero al mundo de la ventana
 */
LatitudeLongitudeInputWidgetTest::LatitudeLongitudeInputWidgetTest(
      suri::World* pWorld) {
   pWorld_ = pWorld;
}

/**
 * Verifica que el valor inicial no cambie. Asigna una coordenada al widget, y
 * luego obtiene la coordenada cargada en el widget. Compara ambas coordenadas.
 */
void LatitudeLongitudeInputWidgetTest::TestSetGet() {
   suri::Coordinates coord(3.123456789, 6.987654321);
   suri::Coordinates coordretorno;
   suri::LatitudeLongitudeInputWidget* lliw = new suri::LatitudeLongitudeInputWidget(
         pWorld_);
   lliw->CreateTool();
   lliw->SetCoordinate(coord);
   lliw->GetCoordinate(coordretorno);

   CPPUNIT_ASSERT_MESSAGE("Error en SetGet de LatitudeLongitudeWidget",
                          coord==coordretorno);

   lliw->GetWindow()->GetParent()->Close();
}
}  // namespace suri

