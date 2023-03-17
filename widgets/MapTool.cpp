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

// Includes standard
#include <math.h>
#include <string>
#include <vector>

// Includes Suri
#include "suri/MapTool.h"
#include "SpatialReference.h"
#include "suri/ViewerWidget.h"
#include "suri/LayerList.h"
#include "suri/World.h"
#include "suri/VectorElement.h"
#include "suri/Coordinates.h"
#include "suri/Configuration.h"
#include "suri/xmlnames.h"
#include "suri/VectorStyleManager.h"
#include "suri/messages.h"
#include "MapRenderer.h"
#include "MapElement.h"
#include "resources.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/dcbuffer.h"
#include "wx/xrc/xmlres.h"
#include "wx/font.h"
#include "wx/colour.h"
#include "wx/filename.h"

// Includes App

// Defines
// Indice para el wxChoice que representa 'sin grilla'
#define NO_GRID_POSSITION 0

/** Nombre de la tabla */
#define GRID_TABLE_NAME "grid"

/** Estilo para la grilla */
#define GRID_LINE_1 \
   "VECTORSTYLE[3,\"Poligono azul\",BRUSH[\"Suri-brush\",0,COLOR[0,0,0,255]],PEN[\"Suri-pen\",1,COLOR[0,255,255,255],WIDTH[1]],SYMBOL[\"Suri-symbol\",0,COLOR[0,0,0,255],SIZE[1]]]"

// Forwards
/** inicializa los recursos de la ventana */
void InitMapToolXmlResource();

// namespace suri
namespace suri {
// ------------------------------- MAP  RENDERER -------------------------------
/**
 *  \cond MACRO_IMPLEMENT_EVENT_OBJECT
 */
START_IMPLEMENT_EVENT_OBJECT(MapEvent, MapTool)
      IMPLEMENT_EVENT_CALLBACK(OnGridComboChanged, OnGridComboChanged(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Constructor.
 * @param[in]	pWorld puntero al mundo.
 * @param[in]	VecGroup grupo sobre el que se dibujan las referencias.
 * @param[in]	pParent puntero a la ventana padre.
 */
MapTool::MapTool(const World *pWorld, std::vector<int> VecGroup, wxWindow *pParent) :
      Widget(pParent), NEW_EVENT_OBJECT(MapEvent),
      pWorld_(pWorld),
      pMapElement_(NULL), pGridElement_(NULL), vecGroup_(VecGroup), pNorthButton_(NULL),
      pScaleButton_(NULL), pLegendButton_(NULL), pcoordtrans_(NULL) {
   windowTitle_ = GetWindowTitle();
   wantedWidth_ = 200;
   wantedHeight_ = 0;
   toolName_ = GetWindowTitle().c_str();
}

/**
 * Destructor.
 */
MapTool::~MapTool() {
   if (Model::IsValid(pList_)) {
      pList_->DelElement(pMapElement_);
      pList_->DelElement(pGridElement_);
   }
   pMapElement_ = NULL;
   pGridElement_ = NULL;
   // elimino el helper de eventos
   DELETE_EVENT_OBJECT;
}

/**
 * Carga la ventana diseniada. Asocia los eventos a los metodos.
 * @return true en caso de poder crear la ventana. false C.O.C.
 */
bool MapTool::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_, wxT("ID_MAP_TOOL"));
   if (!pToolWindow_) {
      REPORT_AND_FAIL_VALUE("D:No hay viewer, lista o ventana inicializado", false);
   }

   wxChoice* pchoice = XRCCTRL(*pToolWindow_, wxT("ID_CMB_GRID"), wxChoice);
   if (pchoice) {
      pchoice->Connect(wxEVT_COMMAND_CHOICE_SELECTED,
                       wxCommandEventHandler(MapEvent::OnGridComboChanged), NULL,
                       pEventHandler_);

      // Solo cargo las grillas si su vector existe
      std::string url;
      pchoice->Clear();
      pchoice->Append(_(label_NO_GRID));
      if (GetGridUrl(_(label_1_DEGREE), url)) {
         pchoice->Append(_(label_1_DEGREE));
      }
      if (GetGridUrl(_(label_2_DEGREES), url)) {
         pchoice->Append(_(label_2_DEGREES));
      }
      if (GetGridUrl(_(label_5_DEGREES), url)) {
         pchoice->Append(_(label_5_DEGREES));
      }
      if (GetGridUrl(_(label_10_DEGREES), url)) {
         pchoice->Append(_(label_10_DEGREES));
      }
      if (GetGridUrl(_(label_20_DEGREES), url)) {
         pchoice->Append(_(label_20_DEGREES));
      }
      if (GetGridUrl(_(label_30_DEGREES), url)) {
         pchoice->Append(_(label_30_DEGREES));
      }

      pchoice->Select(NO_GRID_POSSITION);
   }

   InitializeToolbar(pToolWindow_, pViewer_, pList_);
   std::string northbitmap = button_MAP_NORTH_ARROW;
   std::string scalebitmap = button_MAP_SCALE;
   std::string legendbitmap = button_MAP_LEGEND;
   if (Configuration::GetParameter("app_short_name", "") != wxT("SoPI")) {
      northbitmap = button_SIMUR_MAP_NORTH_ARROW;
      scalebitmap = button_SIMUR_MAP_SCALE;
      legendbitmap = button_SIMUR_MAP_LEGEND;
   }
   pNorthButton_ = new ToggleButton(pToolWindow_, wxT(northbitmap.c_str()),
                                    wxT(caption_NORTH_BUTTON));
   AddButtonTool(pNorthButton_, First);

   pScaleButton_ = new ToggleButton(pToolWindow_, wxT(scalebitmap.c_str()),
                                    wxT(caption_SCALE_BUTTON));
   AddButtonTool(pScaleButton_, First);

   pLegendButton_ = new ToggleButton(pToolWindow_, wxT(legendbitmap.c_str()),
                                     wxT(caption_LEGEND_BUTTON));
   AddButtonTool(pLegendButton_, First);

   return true;
}

/**
 * @return wxString con titulo de la ventana.
 */
const wxString MapTool::GetWindowTitle() const {
   return _(caption_MAP_TOOL);
}

/**
 * Al activarse la herramienta:
 * \pre Ventana activa (active_ == true)
 * - Crea MapElement y lo agrega a la lista
 * - Activa pMapElement_
 * - Activa pGridElement_
 * \pre Ventana inactiva (active_ == false)
 * - Desactiva pMapElement_ y pGridElement_
 * @param[in] Active estado de activacion de la herramienta
 * @return informa si la operacion se pudo realizar
 */
bool MapTool::DoSetActive(bool Active) {
   if (!pList_) {
      REPORT_DEBUG("D:Lista no inicializada.");
      return false;
   }

   // Si existe Activa / Desactiva el elemento de mapa que maneja la herramienta.
   if (pList_->GetElement(pMapElement_)) {
      pMapElement_->Activate(Active);
   } else {
      // Si no existe y pide que se active lo crea.
      pMapElement_ = NULL;
      if (Active) {
         CreateMapElement();
      }
   }

   // Activa / Desactiva la grilla
   if (pGridElement_) {
      pGridElement_->Activate(Active);
   }

   return true;
}

/**
 * Des/Activa los distintos elementos de la herramienta (norte, escala, leyenda)
 * @param[in] Event evento click sobre el boton
 */
void MapTool::DoOnButtonClicked(wxCommandEvent &Event) {
   if (!pList_->GetElement(pMapElement_)) {
      CreateMapElement();
   }

   MapRenderer::Parameters params = MapRenderer::GetParameters(
         MapRenderer::GetXmlNode(pMapElement_));

   params.legend_ = pLegendButton_->GetValue();
   params.north_ = pNorthButton_->GetValue();
   params.scale_ = pScaleButton_->GetValue();
   params.grid_ = Event.IsChecked();

   UpdateMapRendererParameters(params);
}

/** Actualiza los parametros del render de mapa */
void MapTool::UpdateMapRendererParameters(MapRenderer::Parameters& Parameters) {
   if (!pMapElement_)
      CreateMapElement();
   if (MapRenderer::SetXmlNode(pMapElement_, MapRenderer::GetXmlNode(Parameters))
         && !pMapElement_->IsActive()) {
      pMapElement_->Activate();
   }
}

/** Obtiene los parametros actuales del renderizador de mapa */
MapRenderer::Parameters MapTool::GetMapRendererParameters() {
   return MapRenderer::GetParameters(MapRenderer::GetXmlNode(pMapElement_));
}
/**
 * Verifica si hay elementos activos en la lista de elemntos, en dicho caso
 * activa(restaura) los elementos del mapa.
 * @return true si hay elementos activos. false C.O.C.
 */
bool MapTool::IsEnabled() {
   if (!pWorld_) {
      return false;
   }
   std::string wkt = pWorld_->GetSpatialReference();
   std::string paramwkt;
   params_.GetValue(TransformationFactory::kParamWktIn, paramwkt);
   if (wkt.compare(paramwkt) != 0 || pcoordtrans_ == NULL) {
      if (pcoordtrans_ != NULL)
         delete pcoordtrans_;
      params_.AddValue<std::string>(TransformationFactory::kParamWktIn, wkt);
      params_.AddValue<std::string>(TransformationFactory::kParamWktOut, wkt);
      TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
      pcoordtrans_ = pfactory->Create(TransformationFactory::kExact, params_);
      TransformationFactoryBuilder::Release(pfactory);
   }
   // Si el mundo (no)tiene referencia espacial (des)habilita norte y grilla
   bool projection = true;
   if (!pcoordtrans_) {
      projection = false;
   }

   if (pLegendButton_->GetWindow()->IsEnabled() != projection) {
      pLegendButton_->SetEnabled(projection);
   }

   if (pNorthButton_->GetWindow()->IsEnabled() != projection) {
      pNorthButton_->SetEnabled(projection);
   }

   if (USE_CONTROL(*pToolWindow_, wxT("ID_CMB_GRID"), wxChoice, IsEnabled(),
         false) != projection) {
      USE_CONTROL(*pToolWindow_, wxT("ID_CMB_GRID"), wxChoice, Enable(projection),
                  false);
   }

   // Si el mundo (no)es proyectado (des)habilita escala
   if ((pcoordtrans_) && (!SpatialReference::IsProjectedSpatialRef(wkt))) {
      projection = false;
   }

   if (pScaleButton_->GetWindow()->IsEnabled() != projection) {
      pScaleButton_->SetEnabled(projection);
   }

   // devuelve booleano indicando true si hay elementos activos
   // y false sino
   return pList_ && pList_->GetActiveCount() != 0;
}

/**
 * Este metodo se ejecuta al cuando se cambia la seleccion (del item) en el
 * combo de la Grilla.
 * \todo Este metodo deberia actualizar el nodo de renderizacion del elemento
 *   Mapa MapElement para agregar la grilla que se desea usar. Termina siendo
 *   responsabilidad del renderizador, levantar dicho dato y manifestarlo.
 * @param[in]	Event: evento
 */
void MapTool::OnGridComboChanged(wxCommandEvent &Event) {
   // Obtiene la seleccion del wxChoice
   wxString selection =
         (dynamic_cast<wxChoice*>(Event.GetEventObject()))->GetStringSelection();

   UpdateGrid(selection.c_str());
}

/**
 * Metodo que actualiza la grilla del mapa en funcion de los grados indicados por parametro
 * @param GridValue configuracion de la grilla
 */
void MapTool::UpdateGrid(const std::string& GridValue) {
   // Elimina (y por lo tanto oculta) elemento grilla anterior si existe
     if (pGridElement_) {
        pList_->DelElement(pGridElement_);
        pGridElement_ = NULL;
     }

     // si no se selecciona ninguna retorna
     if (GridValue.compare(_(label_NO_GRID)) == 0) {
        return;
     }

     // crea el elemento grilla y lo agrega oculto
     std::string griddir = "";

     if (GetGridUrl(GridValue.c_str(), griddir)) {
        pGridElement_ = dynamic_cast<VectorElement*>(Element::Create("VectorElement",
                                                                     griddir));
     }

     if (!pGridElement_) {
        SHOW_ERROR(_(message_GRID_VECTOR_CREATION_ERROR));
        return;
     }

     pGridElement_->SetName(_(label_GRID));
     // obtengo el estilo para la grilla. si no existe, agrego la tabla al VectorStyleManager.
     VectorStyle *pvecstyle = VectorStyleManager::Instance().GetDefaultStyle(
           GRID_TABLE_NAME, Vector::Polygon);
     if (!pvecstyle) {
        std::vector<std::string> styles;
        styles.push_back(GRID_LINE_1);
        VectorStyleTable *pstyletable = new VectorStyleTable(Vector::Polygon,
                                                             GRID_TABLE_NAME, styles);
        VectorStyleManager::Instance().AddTable(GRID_TABLE_NAME, pstyletable);
        pvecstyle = VectorStyleManager::Instance().GetDefaultStyle(GRID_TABLE_NAME,
                                                                   Vector::Polygon);
        if (!pvecstyle) {
           delete pGridElement_;
           pGridElement_ = NULL;
           REPORT_AND_FAIL("D: no existe estilo para el vector.");
        }
     }
     pGridElement_->SetStyle(pvecstyle->GetWkt());
     pList_->AddElement(pGridElement_, -1);
     pGridElement_->Activate(true);
}

/**
 * Crea el elemento de mapa que maneja la herramienta.
 * \attention pMapElement no debe ser eliminado ya que su dueno es la lista.
 */
void MapTool::CreateMapElement() {
   if (pList_->GetElement(pMapElement_)) {
      return;
   }

   /* \attention: del puntero pMapElement_ se hace cargo la lista. No eliminar */
   pMapElement_ = MapElement::Create();
   pMapElement_->Activate();

   // asigno la lista de elementos a la subclase Parametros,
   // del MapRenderer,  para poder dibujar las referencias.
   MapRenderer::Parameters::pStaticList_ = pList_;

   MapRenderer::staticElementGroupVector_.assign(vecGroup_.begin(), vecGroup_.end());
   pList_->AddElement(pMapElement_, -1);

   // asigno la lista de elementos a la subclase Parametros,
   // del MapRenderer,  para poder dibujar las referencias.
   MapRenderer::Parameters::pStaticList_ = NULL;
}

/**
 * Obtiene el url del vector de una grilla.
 * @param[in] GridId id de la grilla de la que se dea el URL. Los id estan
 * definidos en messages.h y corresponde al texto del wxgrid
 * @param[out] Url del elemento. Solo es valido si el metodo retorna true;
 * @return informa si el archivo existe en disco
 */
bool MapTool::GetGridUrl(const std::string &GridId, std::string &Url) {
   // genera el path al elemento grilla
   std::string url = Configuration::GetParameter("app_map_grid_dir", wxT("./"));
   if (GridId.compare(_(label_1_DEGREE)) == 0) {
      url += "WGS-84_Grid_1deg.shp";
   }

   if (GridId.compare(_(label_2_DEGREES)) == 0) {
      url += "WGS-84_Grid_2deg.shp";
   }

   if (GridId.compare(_(label_5_DEGREES)) == 0) {
      url += "WGS-84_Grid_5deg.shp";
   }

   if (GridId.compare(_(label_10_DEGREES)) == 0) {
      url += "WGS-84_Grid_10deg.shp";
   }

   if (GridId.compare(_(label_20_DEGREES)) == 0) {
      url += "WGS-84_Grid_20deg.shp";
   }

   if (GridId.compare(_(label_30_DEGREES)) == 0) {
      url += "WGS-84_Grid_30deg.shp";
   }

   if (!wxFileName::FileExists(url.c_str())) {
      return false;
   }

   Url = url;
   return true;
}

/**
 *
 */
wxWindow *MapTool::GetToolWindow() const {
   return (const_cast<MapTool*>(this))->GetWindow();
}

/** Indica si la grilla se encuentra activa */
bool MapTool::IsGridActive() const {
   return pGridElement_ != NULL;
}
}  // end namespace suri

