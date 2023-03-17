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

#include "suri/GpsTool.h"

// Includes standard
// Includes Suri
#include "suri/VectorEditor.h"
#include "suri/VectorStyleManager.h"
#include "suri/Vector.h"
#include "suri/messages.h"
#include "resources.h"
#include "CloseVectorButton.h"
#include "SaveVectorButton.h"
#include "suri/DataViewManager.h"

// Includes Wx
#include "wx/wx.h"
// Includes App
#include "PointElementEditor.h"
#include "LineElementEditor.h"
#include "PolygonElementEditor.h"
#include "GpsVectorButton.h"

// Defines
/** Nombre de la tabla GPS */
#define GPS_TABLE_NAME "gps"

// Estilos draw punto
/** Primer estilo de dibujo para el punto */
#define DRAW_POINT_1 \
   "VECTORSTYLE[1,\"Cruz violeta\",SYMBOL[\"Suri-symbol\",4,COLOR[128,0,255,255],SIZE[2]]]"
/** Segundo estilo de dibujo para el punto */
#define DRAW_POINT_2 \
   "VECTORSTYLE[1,\"Pentagono amarillo\",SYMBOL[\"Suri-symbol\",5,COLOR[255,255,0,255],SIZE[2]]]"
/** Tercer estilo de dibujo para el punto */
#define DRAW_POINT_3 \
   "VECTORSTYLE[1,\"Circulo gris\",SYMBOL[\"Suri-symbol\",1,COLOR[128,128,128,255],SIZE[2]]]"
// Estilos draw line
/** Primer estilo de dibujo para la linea */
#define DRAW_LINE_1 \
   "VECTORSTYLE[2,\"Linea amarillo\",PEN[\"Suri-pen\",1,COLOR[255,255,0,255],WIDTH[1]]]"
/** Segundo estilo de dibujo para la linea */
#define DRAW_LINE_2 "VECTORSTYLE[2,\"Linea rojo\",PEN[\"Suri-pen\",1,COLOR[255,0,0,255],WIDTH[1]]]"
/** Tercer estilo de dibujo para la linea */
#define DRAW_LINE_3 \
   "VECTORSTYLE[2,\"Linea violeta\",PEN[\"Suri-pen\",1,COLOR[128,0,255,255],WIDTH[1]]]"
// Estilos draw poligon
/** Primer estilo de dibujo para el poligono */
#define DRAW_POLYGON_1 \
   "VECTORSTYLE[3,\"Poligono cyan\",PEN[\"Suri-pen\",1,COLOR[128,255,255,255],WIDTH[1]]]"
/** Segundo estilo de dibujo para el poligono */
#define DRAW_POLYGON_2 \
   "VECTORSTYLE[3,\"Poligono lavanda\",PEN[\"Suri-pen\",1,COLOR[255,128,255,255],WIDTH[1]]]"
/** Tercer estilo de dibujo para el poligono */
#define DRAW_POLYGON_3 \
   "VECTORSTYLE[3,\"Poligono verde\",PEN[\"Suri-pen\",1,COLOR[0,255,0,255],WIDTH[1]]]"

/** namespce suri */
namespace suri {

/**
 * Constructor
 * @param VectorGroup
 * @param pVectorEditor
 * @param pDataViewManager
 * @param pParent
 */
GpsTool::GpsTool(int VectorGroup, VectorEditor* pVectorEditor,
                 DataViewManager* pDataViewManager,
                 const suri::SuriObject::UuidType& ViewcontextId,
                 wxWindow *pParent) :
      DrawTool(pParent, pDataViewManager ? pDataViewManager : NULL, ViewcontextId),
      vectorGroup_(VectorGroup), pVectorEditor_(pVectorEditor),
      pDataViewManager_(pDataViewManager),
      viewcontextId_(ViewcontextId) {
   windowTitle_ = _(caption_GPS_TOOL);
   toolName_ = GetWindowTitle();
   wantedHeight_ = BUTTON_SIZE;

   // Cargo los estilos de los vectores de dibujo
   /* cargo los estilos para que VectorElementEditor::CreateVectorElement cree la
    * tabla de estilo.
    * Limpio el vector de estilos para asegurarme de que la tabla se cargue
    * solo con los estilos que seteo.*/
   // Point
   std::vector<std::string> Styles;
   Styles.clear();
   Styles.push_back(DRAW_POINT_1);
   Styles.push_back(DRAW_POINT_2);
   Styles.push_back(DRAW_POINT_3);
   VectorStyleManager::Instance().AddTable(
         GPS_TABLE_NAME, new VectorStyleTable(Vector::Point, GPS_TABLE_NAME, Styles));

   // Line
   Styles.clear();
   Styles.push_back(DRAW_LINE_1);
   Styles.push_back(DRAW_LINE_2);
   Styles.push_back(DRAW_LINE_3);
   VectorStyleManager::Instance().AddTable(
         GPS_TABLE_NAME, new VectorStyleTable(Vector::Line, GPS_TABLE_NAME, Styles));
   // Polygon
   Styles.clear();
   Styles.push_back(DRAW_POLYGON_1);
   Styles.push_back(DRAW_POLYGON_2);
   Styles.push_back(DRAW_POLYGON_3);
   VectorStyleManager::Instance().AddTable(
         GPS_TABLE_NAME, new VectorStyleTable(Vector::Polygon, GPS_TABLE_NAME, Styles));
}

/**
 * Destructor.
 */
GpsTool::~GpsTool() {
}

/**
 * Carga la ventana diseniada y asocia los eventos.
 * @return true.
 */
bool GpsTool::CreateToolWindow() {
	pDatasourceManager_ =
			pDataViewManager_ ?
					pDataViewManager_->GetDatasourceManager() : NULL;
   pToolWindow_ = new wxPanel(pParentWindow_, wxID_ANY);
   pToolWindow_->SetSizer(new wxBoxSizer(wxHORIZONTAL));
   if (!pViewer_ || !pToolWindow_) {
      REPORT_AND_FAIL_VALUE("D:No hay viewer, lista o ventana inicializado", false);
   }
   /** Parche para que los botones funcionen */
   pList_ = pViewer_->GetList();

   InitializeToolbar(pToolWindow_, pViewer_, pList_);

   // ---------- PUNTOS
   // Crea editor de elementos tipo punto y se lo pasa a boton Gps.
   // Se pasa BUTTON_MARK para que solo muestre boton de marcar
   VectorElementEditor *pelementeditor = new PointElementEditor(pViewer_, vectorGroup_,
                                                                pVectorEditor_,
                                                                pDataViewManager_,
                                                                GPS_TABLE_NAME);
   GpsVectorButton *ptool = new GpsVectorButton(pToolWindow_, pViewer_, wxT(
                                                tool_GPS_POINT),
                                                pelementeditor, pDataViewManager_,
                                                tooltip_GPS_POINT,
                                                BUTTON_MARK);
   AddButtonTool(ptool, Last, ToolGroupManager::Edition);
   toolButtons_.insert(ptool);

   // ---------- LINEAS
   // Crea editor de elementos tipo linea y se lo pasa a boton Gps.
   pelementeditor = new LineElementEditor(pViewer_, vectorGroup_, pVectorEditor_,
                                          pDataViewManager_, GPS_TABLE_NAME);
   ptool = new GpsVectorButton(pToolWindow_, pViewer_, wxT(tool_GPS_LINE),
                               pelementeditor, pDataViewManager_,
                               tooltip_GPS_LINE);
   AddButtonTool(ptool, Last, ToolGroupManager::Edition);
   toolButtons_.insert(ptool);

   // ---------- POLIGONOS
   // Crea editor de elementos tipo poligono y se lo pasa a boton Gps.
   pelementeditor = new PolygonElementEditor(pViewer_, vectorGroup_, pVectorEditor_,
                                             pDataViewManager_, GPS_TABLE_NAME);
   ptool = new GpsVectorButton(pToolWindow_, pViewer_, wxT(tool_GPS_POLYGON),
                               pelementeditor, pDataViewManager_,
                               tooltip_GPS_POLYGON);
   AddButtonTool(ptool, Last, ToolGroupManager::Edition);
   toolButtons_.insert(ptool);

   pViewer_->Link(this);
   return true;
}
}  // namespace suri
