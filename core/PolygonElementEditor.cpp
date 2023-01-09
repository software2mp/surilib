/*! \file PolygonElementEditor.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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

