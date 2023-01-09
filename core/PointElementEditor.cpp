/*! \file PointElementEditor.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "PointElementEditor.h"

// Includes standard

// Includes Suri
#include "suri/VectorEditor.h"
#include "suri/ViewerWidget.h"
#include "suri/messages.h"
#include "suri/CoordinatesTransform.h"
#include "resources.h"

// Includes Wx

// Includes App

// Includes OGR
#include "ogrsf_frmts.h"

// Defines
/** Primer estilo para dibujar linea */
#define DRAW_POINT_1 \
   "VECTORSTYLE[1,\"Cruz violeta\",SYMBOL[\"Suri-symbol\",4,COLOR[128,0,255,255],SIZE[2]]]"
/** Segundo estilo para dibujar linea */
#define DRAW_POINT_2 \
   "VECTORSTYLE[1,\"Pentagono amarillo\",SYMBOL[\"Suri-symbol\",5,COLOR[255,255,0,255],SIZE[2]]]"
/** Tercer estilo para dibujar linea */
#define DRAW_POINT_3 \
   "VECTORSTYLE[1,\"Circulo gris\",SYMBOL[\"Suri-symbol\",1,COLOR[128,128,128,255],SIZE[2]]]"
/** Utilizado para definir nombre de la capa que se quiere abrir. */
#define POINT_FILE_NAME "Point_1"

/** namespace suri */
namespace suri {
/**
 * Constructor.
 * @param[in]	pViewer	Viewer sobre el cual actua.
 * @param[in]	ListGroup	Grupo al que pertenece.
 * @param[in]  pVectorEditor Permite editar vectores
 * @param[in]  CategoryName categoria del vector.
 */
PointElementEditor::PointElementEditor(ViewerWidget *pViewer, int ListGroup,
                                       VectorEditor* pVectorEditor,
                                       DataViewManager* pDataViewManager,
                                       std::string CategoryName) :
      VectorElementEditor(pViewer, Vector::Point, CategoryName, ListGroup,
                          pVectorEditor, pDataViewManager) {
   /* cargo los estilos para que VectorElementEditor::CreateVectorElement cree la
    * tabla de estilo.
    * Limpio el vector de estilos para asegurarme de que la tabla se cargue
    * solo con los estilos que seteo.*/
   styles_.clear();
   styles_.push_back(DRAW_POINT_1);
   styles_.push_back(DRAW_POINT_2);
   styles_.push_back(DRAW_POINT_3);
}

/**
 * Destructor.
 */
PointElementEditor::~PointElementEditor() {
}

/**
 * Abre capa y crea elemento HotLinkVectorElement
 * \pre El vector esta abierto
 *	\post Setea el flag de vector abierto.
 * \todo No utilizar el nombre del campo hardcodeado.
 * \todo Considerar utilizar propiedad o define para el tamanio del campo.
 * \warning El nombre del campo debe ser menos a 10 caracteres ASCII-127
 */
void PointElementEditor::SetupElement() {
   // Creo capa (antes de crear elemento para que se cree archivo)
   if (!pVectorEditor_
         || pVectorEditor_->OpenLayer(POINT_FILE_NAME, LATLON_SPATIAL_REFERENCE,
                                      Vector::Point) < 0) {
      REPORT_DEBUG("D: Error al abrir capa punto");
      return;
   }

   // Sincronizo a disco antes de crear el HotLinkVectorElement
   pVectorEditor_->SyncToDisk();
   Element *photlinkelement = Element::Create("HotLinkVectorElement", elementUrl_);
   if (photlinkelement && dynamic_cast<VectorElement*>(photlinkelement)) {
      // Guardo HotLinkElement para que VectorElementEditor no cree VectorElement
      pCurrentVectorElement_ = dynamic_cast<VectorElement*>(photlinkelement);
   } else {
      REPORT_AND_FAIL("D:No se pudo configurar el vector");
   }
}

/**
 * Agrega el punto recibido por parametro al vector, si no hay
 * un feature abierto crea uno nuevo.
 * Luego de agregar el punto setea flags de vector como cerrados.
 * \post vectorOpened_ pasan a false
 * @param[in] Point: punto a agregar.
 * @return true en caso de haber podido agregar el punto
 * @return false pCurrentVectorElement_ es invalido.
 * @return false No se pudo crear el feature
 * @return false No se pudo agregar el punto
 */
bool PointElementEditor::AddPointToVector(const Coordinates &Point) {
   // Si no hay un feature abierto lo abro
   if (!pCurrentVectorElement_
         || (pVectorEditor_->GetOpenFeature() < 0 && pVectorEditor_->OpenFeature() < 0)) {
      return false;
   }
   if (!pVectorEditor_->SetPoint(Point)) {
      return false;
   }
   CloseFeature();
   return true;
}
}  // namespace suri
