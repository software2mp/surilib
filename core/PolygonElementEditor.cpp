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

#include "PolygonElementEditor.h"

// Includes standard

// Includes Suri
#include "suri/VectorEditor.h"
#include "suri/ViewerWidget.h"
#include "suri/CoordinatesTransform.h"
#include "suri/messages.h"
#include "resources.h"

// Includes Wx

// Includes App

// Includes OGR
#include "ogrsf_frmts.h"

// Defines
/** Primer estilo para dibujar poligono */
#define DRAW_POLYGON_1 \
   "VECTORSTYLE[3,\"Poligono cyan\",PEN[\"Suri-pen\",1,COLOR[128,255,255,255],WIDTH[2]]]"
/** Segundo estilo para dibujar poligono */
#define DRAW_POLYGON_2 \
   "VECTORSTYLE[3,\"Poligono lavanda\",PEN[\"Suri-pen\",1,COLOR[255,128,255,255],WIDTH[2]]]"
/** Tercer estilo para dibujar poligono */
#define DRAW_POLYGON_3 \
   "VECTORSTYLE[3,\"Poligono verde\",PEN[\"Suri-pen\",1,COLOR[0,255,0,255],WIDTH[2]]]"
/** Utilizado para definir nombre de la capa que se quiere abrir. */
#define POLYGON_FILE_NAME "Polygon_1"

/** namespace suri */
namespace suri {
/**
 * Constructor.
 * @param[in]	pViewer	Viewer sobre el cual actua.
 * @param[in]	ListGroup Grupo al que pertenece.
 * @param[in]  pVectorEditor Permite editar vectores
 * @param[in]  CategoryName categoria del vector.
 */
PolygonElementEditor::PolygonElementEditor(ViewerWidget *pViewer, int ListGroup,
      VectorEditor* pVectorEditor, DataViewManager* pDataViewManager,
       std::string CategoryName) :
      VectorElementEditor(pViewer, Vector::Polygon, CategoryName, ListGroup,
                          pVectorEditor, pDataViewManager) {
   /* cargo los estilos para que VectorElementEditor::CreateVectorElement cree la
    * tabla de estilo.
    * Limpio el vector de estilos para asegurarme de que la tabla se cargue
    * solo con los estilos que seteo.*/
   styles_.clear();
   styles_.push_back(DRAW_POLYGON_1);
   styles_.push_back(DRAW_POLYGON_2);
   styles_.push_back(DRAW_POLYGON_3);
}

/**
 * Destructor
 */
PolygonElementEditor::~PolygonElementEditor() {
}

/**
 * Abre la capa que se usara para agregar puntos
 * \pre el vector esta abierto
 * \post setea el flag de vector abierto (vectorOpened_).
 */
void PolygonElementEditor::SetupElement() {
   // Creo capa
   if (!pVectorEditor_
         || pVectorEditor_->OpenLayer(POLYGON_FILE_NAME, LATLON_SPATIAL_REFERENCE,
                                      Vector::Polygon) < 0) {
      REPORT_DEBUG("D: Error al abrir capa poligono");
   }
}

/**
 * Agrega el punto recibido por parametro al poligono, si no hay
 * un feature abierto crea uno nuevo.
 * @param[in]	Point: Punto a agregar al vector.
 * @return true en caso de haber agregado el punto.
 * @return false pCurrenVectorElement_ es invalido.
 * @return false No se pudo agregar el punto al anillo
 */
bool PolygonElementEditor::AddPointToVector(const Coordinates &Point) {
   // Si no hay un feature abierto lo abro
   if (!pCurrentVectorElement_
         || (pVectorEditor_->GetOpenFeature() < 0 && pVectorEditor_->OpenFeature() < 0)) {
      return false;
   }
   if (!pVectorEditor_->AddPoint(Point)) {
      return false;
   }
   pVectorEditor_->SyncToDisk();
   return true;
}
}  // namespace suri

