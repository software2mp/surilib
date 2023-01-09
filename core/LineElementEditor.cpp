/*! \file LineElementEditor.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "LineElementEditor.h"

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
/** Primer estilo de dibujo para la linea */
#define DRAW_LINE_1 \
   "VECTORSTYLE[2,\"Linea amarillo\",PEN[\"Suri-pen\",1,COLOR[255,255,0,255],WIDTH[2]]]"
/** Segundo estilo de dibujo para la linea */
#define DRAW_LINE_2 "VECTORSTYLE[2,\"Linea rojo\",PEN[\"Suri-pen\",1,COLOR[255,0,0,255],WIDTH[2]]]"
/** Tercer estilo de dibujo para la linea */
#define DRAW_LINE_3 \
   "VECTORSTYLE[2,\"Linea violeta\",PEN[\"Suri-pen\",1,COLOR[128,0,255,255],WIDTH[2]]]"
/** Nombre por defecto para el elemento */
#define LINE_FILE_NAME "Line_1"

/** namespace suri */
namespace suri {
/**
 * Constructor.
 * @param[in]	pViewer:	Viewer sobre el cual actua.
 * @param[in]	ListGroup:	Grupo al que pertenece.
 * @param[in]  pVectorEditor: Permite editar vectores
 * @param[in]  CategoryName
 */
LineElementEditor::LineElementEditor(ViewerWidget *pViewer, int ListGroup,
      VectorEditor* pVectorEditor, DataViewManager* pDataViewManager,
      std::string CategoryName) :
            VectorElementEditor(pViewer, Vector::Line, CategoryName, ListGroup,
                  pVectorEditor, pDataViewManager) {
   /* cargo los estilos para que VectorElementEditor::CreateVectorElement cree la
    * tabla de estilo.
    * Limpio el vector de estilos para asegurarme de que la tabla se cargue
    * solo con los estilos que seteo.*/
   styles_.clear();
   styles_.push_back(DRAW_LINE_1);
   styles_.push_back(DRAW_LINE_2);
   styles_.push_back(DRAW_LINE_3);
}

/**
 * Destructor.
 */
LineElementEditor::~LineElementEditor() {
}

/**
 * Configura el vector con una capa de tipo linea.
 * \pre pCurrentVectorElement_ debe ser valido
 * \pre El vector debe estar abierto
 * \post setea el flag de vector abierto (vectorOpened_).
 */
void LineElementEditor::SetupElement() {
   // Creo capa
   if (!pVectorEditor_
         || pVectorEditor_->OpenLayer(LINE_FILE_NAME, LATLON_SPATIAL_REFERENCE,
                                      Vector::Line) < 0) {
      REPORT_DEBUG("D: Error al abrir capa linea");
   }
}

/**
 * Agrega el punto recibido por parametro a la linea, si no hay
 * un feature abierto crea uno nuevo.
 * @param[in]	Point: Punto a agregar al vector.
 * @return true en caso de haber agregado el punto.
 * @return false pCurrentVectorElement_ es invalido.
 * @return false No se pudo agregar punto.
 */
bool LineElementEditor::AddPointToVector(const Coordinates &Point) {
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

