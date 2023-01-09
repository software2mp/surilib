/*! \file Viewer3D.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <sstream>
#include <vector>
#include <string>

// Includes suri
#include "suri/Viewer3D.h"
#include "suri/TerrainElement.h"
#include "suri/xmlnames.h"
#include "suri/messages.h"
#include "suri/VectorElement.h"
#include "suri/HtmlViewerPart.h"
#include "suri/PartContainerWidget.h"
#include "suri/Vector.h"
#include "suri/ToggleButton.h"
#include "suri/RasterSpatialModel.h"
#include "RenderizationManager.h"
#include "TerrainGLCanvas.h"
#include "MemoryCanvas.h"
#include "Terrain.h"
#include "resources.h"
#include "wxToggleBitmapButton.h"
#include "SaveViewButton.h"
#include "SphericalCamButton.h"
#include "EyeCamButton.h"
#include "Query3DButton.h"
#include "suri/DataTypes.h"
#include "Viewer3DWorldExtentManager.h"
#include "suri/Viewer3dTransformation.h"
#include "suri/VectorStyle.h"
#include "suri/VectorStyleManager.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/DynamicViewerWidget.h"
#include "suri/DataViewManager.h"
// Includes wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"
#include "wx/filedlg.h"
#include "wx/clrpicker.h"

// Includes ogr
#include "ogr_geometry.h"
#include "ogr_feature.h"
#include "ogrsf_frmts.h"

// Defines
#define DEFAULT_LAYER 0
/** factor de exageracion minimo */
#define MIN_VALUE 1

// Forward para no tener que crear un .h
/** Inicializa los recursos de la ventana. */
void InitC3DToolbarXmlResource();

/** namespace suri */
namespace suri {
// -----------------------------------------------------------------------------
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(Viewer3DEvent, Viewer3D)
   IMPLEMENT_EVENT_CALLBACK(OnSize, OnSize(), wxSizeEvent)
   IMPLEMENT_EVENT_CALLBACK(OnColourChangeButtonClick, OnColourChangeButtonClick(
               event), wxColourPickerEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/**
 * Constructor
 * @param[in] pList Lista de elementos.
 * @param[in] pParent puntero a la ventana padre.
 * @param[in] pDataViewManager administrador de info de la aplicacion 
 * @return instancia de la Clase Viewer3D
 */
Viewer3D::Viewer3D(wxWindow *pParent, DataViewManager* pDataViewManager) :
      ViewerWidget(pParent, pDataViewManager), NEW_EVENT_OBJECT(Viewer3DEvent),
      pTerrainRenderizationManager_(NULL),
      pTextureRenderizationManager_(NULL), pTerrainWorld_(NULL), pTextureWorld_(NULL),
      pTerrainMemoryCanvas_(NULL), pTextureMemoryCanvas_(NULL), pTerrainList_(NULL),
      pTextureList_(NULL), pTerrainElement_(NULL), pListViewer_(NULL), pElementViewer_(NULL),
      pGlCanvas_(NULL), pTerrain_(NULL), pColourPickerButton_(NULL), pActiveElement_(NULL),
      pExtentManagerTerrain_(NULL), pExtentManagerTexture_(NULL), pTransform_(NULL),
      pElement_(NULL) {
   windowTitle_ = caption_VIEWER3D;
   pWorld_ = new World();
   // visor de Lista
   pListViewer_ = new ListView<Viewer3D>(this, &Viewer3D::ListUpdate, &Viewer3D::ListDelete);
   // visor de elemento
   pElementViewer_ = new CallbackView<Viewer3D>(this, &Viewer3D::ElementUpdate);
   pTransform_ = new Viewer3dTransformation(this);
}

/** dtor */
Viewer3D::~Viewer3D() {
   // elimino el handler de eventos
   DELETE_EVENT_OBJECT;

   // Desregistro el ElementViewer de todos los elementos antes de eliminarlo
   suri::LayerList *playerlist = pListViewer_->GetList();
   if (Model::IsValid(playerlist)) {
      suri::LayerList::ElementListType::iterator it = playerlist->GetElementIterator();
      for (; it != playerlist->GetElementIterator(false); ++it) {
         suri::Element *pelement = *it;
         pelement->UnregisterViewer(pElementViewer_);
      }
   }
   delete pElementViewer_;
   // Elimino la listviewer. Se desregistra automaticamente
   delete pListViewer_;

   pColourPickerButton_ = NULL;
   // Le asigno NULL para que se ocupe ViewerWidget
   pWorld_ = NULL;
   // Elimino el terreno para que al resetear el viewer no se limpie el canvas
   // Caso contrario quedan punteros en estado invalido.
   // Aclaracion: ResetViewer utiliza el canvas si terrain_!=NULL,
   // para evitar que esto suceda lo elimino antes
   if (pTerrain_) {
      pTerrain_->ClearTerrainDisplayList();
      pTerrain_->ClearVectorDisplayLists();
      delete pTerrain_;
      pTerrain_ = NULL;
   }
   // Reseteo el viewer
   ResetViewer();
   if (pTransform_) delete pTransform_;
   // borra el canvas
   // pGlCanvas_->Destroy();
}

/**
 * Metodo que recibe al destruirse el modelo
 * @param[in] pModel lista que fue eliminada
 */
void Viewer3D::ListDelete(Model *pModel) {
// Al cerrar ejecuta una recursion de eventos que termina en un SIGSEGV
// Se cierra por evento wx::OnDestroy y tambien por delete, que lleva a otro
// wx::OnDestroy de un objeto destruido
#ifdef __UNUSED_CODE__
   delete this;
#endif
}

/** Creacion de la ventana
 * @return true en caso de poder crear la ventana de la herramienta.
 */
bool Viewer3D::CreateToolWindow() {
   // asigno el panel al pToolWindow_
   pGlCanvas_ = new TerrainGLCanvas(pParentWindow_);
   AddControl(pGlCanvas_, pParentWindow_);
   pToolWindow_ = pGlCanvas_;
   // llamo para que arme la ventana en caso de ser autonoma
   ViewerWidget::CreateToolWindow();
   // Conecto los eventos
   CONNECT_TOOL_EVENT(Viewer3DEvent, OnSize, wxEVT_SIZE, wxSizeEventHandler);
   return pToolWindow_ != NULL;
}

/** Se le setea la lista de elementos (vacio porque no es necesario */
void Viewer3D::DoSetElementList(LayerList *pList) {
   // me registro en la lista y mundo antes de crear el RM
   // para poder actuar antes de la renderizacion
   pList->RegisterViewer(pListViewer_);
   pWorldViewer_ = new WorldView<Viewer3D>(this, &Viewer3D::UpdateViewers);
   pWorld_->RegisterViewer(pWorldViewer_);
}

/** Resetea el View */
/**
 *  Setea el Canvas en NULL y borra los RM. Borra los canvas, el terreno y
 * setea el color del picker a blanco
 * \todo (11/06/2009 - Alejandro): las listas que se generan en metodo Create no
 * se estan eliminando. Verificar que los RenderizationManager no las borre.
 */
void Viewer3D::ResetViewer() {
   // Setea el Canvas en NULL y borra los RM. Estos destruyen los mundos
   // y las listas.
   if (pTerrainRenderizationManager_) {
      pTerrainRenderizationManager_->SetCanvas(NULL);
      delete pTerrainRenderizationManager_;
      pTerrainRenderizationManager_ = NULL;
   }
   if (pTextureRenderizationManager_) {
      pTextureRenderizationManager_->SetCanvas(NULL);
      delete pTextureRenderizationManager_;
      pTextureRenderizationManager_ = NULL;
   }

   // elimino las listas y los mundos
   delete pTerrainWorld_;
   delete pTextureWorld_;
   delete pTerrainList_;
   delete pTextureList_;
   pTerrainWorld_ = NULL;
   pTextureWorld_ = NULL;
   pTerrainList_ = NULL;
   pTextureList_ = NULL;
   // Reinicializo pWorld_ como pTerrainWorld_
   // pWorld_ = pTerrainWorld_;

   // Borro los canvas
   delete pTerrainMemoryCanvas_;
   pTerrainMemoryCanvas_ = NULL;

   delete pTextureMemoryCanvas_;
   pTextureMemoryCanvas_ = NULL;

   delete pExtentManagerTerrain_;
   pExtentManagerTerrain_ = NULL;

   delete pExtentManagerTexture_;
   pExtentManagerTexture_ = NULL;

   // Borro el terreno
   if (pTerrain_) {
      TerrainGLCanvas *pterraincanvas = GetCanvas();
      if (pterraincanvas) {
         pterraincanvas->ResetCanvas();
         pTerrain_->ClearTerrainDisplayList();
         pTerrain_->ClearVectorDisplayLists();
         pterraincanvas->Render();
         delete pTerrain_;
         pTerrain_ = NULL;
         pterraincanvas->SetTerrain(NULL);
         pterraincanvas->SetReady(false);
      }
   }

   // SETEA EL COLOR DEL PICKER A BLANCO
   if (pColourPickerButton_) {
      wxColour colour(255, 255, 255, 1);
      pColourPickerButton_->SetColour(colour);
   }
#ifdef __UNUSED_CODE__
   if (!pWorld_) {
      pWorld_ = new World();
      TerrainElement *pterrainelement = dynamic_cast<TerrainElement*>(pActiveElement_);
      if (pterrainelement && pActiveElement_) {
         pWorld_->SetSpatialReference(pActiveElement_->GetSpatialReference().c_str());
         Subset window;
         pterrainelement->GetTexture()->GetElementExtent(window);
         pWorld_->SetWindow(window);
      }
   }
#endif
   if (pWorld_) pWorld_->ResetWorld();
}


/**
 * Permite obtener la ventana con las herramientas del viewer. Si no habia
 * una configurada usa la de pParent y le carga todos los botones
 * del Viewer
 * @param[in] pParent window donde busca ID_3DTOOLBAR. Luego
 * configura toolbar con la ventana devuelta.
 * @return toolbar de la herramienta. Si ya estaba configurada no
 * la modifica, caso contrario setea la de pParent.
 */
wxWindow *Viewer3D::DoGetViewerTools(wxWindow *pParent) {
   wxWindow* ptoolbar = GetToolbar();
   // Si existe el toolbar como propiedad lo devuelve
   if (ptoolbar) {
      return ptoolbar;
   }
   // Para que actualize el pToolWindow_ y se conecten los eventos
   GetCanvas();
   // Sino lo arma y lo devuelve al final
   // CARGA EL PANEL CON EL COLOUR PICKER BUTTON
   ptoolbar = wxXmlResource::Get()->LoadPanel(pParent, wxT("ID_3DTOOLBAR"));
   InitializeToolbar(ptoolbar, this, GetList());

   // SAVE SCREEN BUTTON-----------------------------------------------
   Button* pbutton = new SaveViewButton(ptoolbar);
   pViewerToolButtons_.insert(pbutton);
   AddButtonTool(pbutton, ViewerWidget::First);

   // SPHERICAL CAM BUTTON---------------------------------------------
   SphericalCamButton* scb = new SphericalCamButton(ptoolbar, GetCanvas());
   pViewerToolButtons_.insert(scb);
   AddButtonTool(scb, Last, Navigation);
   ActivateButton(scb);

   // EYE CAM BUTTON---------------------------------------------------
   pbutton = new EyeCamButton(ptoolbar, GetCanvas());
   pViewerToolButtons_.insert(pbutton);
   AddButtonTool(pbutton, Last, Navigation);

   // QUERY BUTTON-----------------------------------------------------
   pbutton = new Query3DButton(ptoolbar, GetCanvas(), this);
   pViewerToolButtons_.insert(pbutton);
   AddButtonTool(pbutton, Last, Navigation);

   // Conecta COLOUR PICKER BUTTON EVENTS
   pColourPickerButton_ = XRCCTRL(*ptoolbar, "ID_COLOURPICKERCTRL", wxColourPickerCtrl);
   if (pColourPickerButton_) {
      pColourPickerButton_->Connect(
            wxEVT_COMMAND_COLOURPICKER_CHANGED,
            wxColourPickerEventHandler(Viewer3DEvent::OnColourChangeButtonClick), NULL,
            pEventHandler_);

      // SETEA EL COLOR DE FONDO A BLANCO
      wxColour colour(255, 255, 255, 1);
      pColourPickerButton_->SetColour(colour);
   }

   // DEVUELVE EL TOOLBAR
   return GetToolbar();
}

/**
 * Despliega un elemento (puede ser funcion de RenderizationControl)
 * @param[in] pElement Elemento que se quiere desplegar
 */
void Viewer3D::DisplayElement(Element *pElement) {
   REPORT_DEBUG("D:Muestro Elemento");
   SetWindowTitle(pElement->GetName());
}

/**
 * Oculta un elemento (puede ser funcion de RenderizationControl)
 * @param[in] pElement Elemento que se quiere ocultar
 */
void Viewer3D::HideElement(Element *pElement) {
   REPORT_DEBUG("D:Oculto Elemento");
   SetWindowTitle(caption_VIEWER3D);
}

/**
 * El elemento ha sido modificado
 * @param[in] pElement Elemento que se modifico
 */
void Viewer3D::UpdateElement(Element *pElement) {
   REPORT_DEBUG("D:Elemento modificado");
}

void Viewer3D::OnSize() {
}

/**
 * Modifica el color de fondo del canvas y tambien actualiza el nodo
 * del terreno.
 * @param[in] Event evento de seleccion de color de fondo
 */
void Viewer3D::OnColourChangeButtonClick(wxColourPickerEvent &Event) {
   wxColour colour = Event.GetColour();
   GetCanvas()->SetBackGroundColour(static_cast<float>(colour.Red()),
                                    static_cast<float>(colour.Green()),
                                    static_cast<float>(colour.Blue()));
   // Guarda color en nodo xml
   Configuration::SetParameterEx("v3d_color", colour.GetAsString(wxC2S_HTML_SYNTAX).c_str());
   GetCanvas()->Render();
}

// ------------------------ ACTUALIZACION DE MODELOS ------------------------
/**
 * Actualizacion de la lista
 * @param[in] pModel lista modificada
 */
void Viewer3D::ListUpdate(Model *pModel) {
   LayerList *plist = pListViewer_->GetList();
   if (plist != pModel) {
      return;
   }

   // Si se agregaron elementos, registro el viewer
   if (pListViewer_->ElementAdded()) {
      LayerList::ElementListType newelements = pListViewer_->GetNewElements();
      LayerList::ElementListType::iterator it = newelements.begin();
      for (; it != newelements.end(); ++it) {
         Element *pelement = *it;
         pelement->RegisterViewer(pElementViewer_);
      }
   }

   // No tengo elementos activos
   if (plist->GetActiveCount() == 0) {
      ResetViewer();
   }
}

/** Actualizacion del mundo */
/**
 * Si el elemento esta en la lista, dependiendo de que tipo de elemento sea y
 * si esta activo lo dibuja.
 * @param[in] pModel elemento modificado
 */
void Viewer3D::ElementUpdate(Model *pModel) {
   if (Configuration::GetParameter("app_short_name", "") == wxT("SoPI"))
      ElementUpdateForDynamicViewer(pModel);
   else
      ElementUpdateForViewer3D(pModel);
}

void Viewer3D::ElementUpdateForViewer3D(Model *pModel) {
   LayerList *plist = pListViewer_->GetList();

   // valido que se trata de un elemento en la lista
   Element *pelement = plist->GetElement(pModel);
   if (!pelement) return;

   int activecount =
         plist->GetActiveCount(
               plist->GetElementData(plist->GetElement(plist->GetElementCount() - 1))->group_);

   // Si es terreno y esta habilitado lo dibuja
   suri::TerrainElement* pterrain = dynamic_cast<suri::TerrainElement*>(pelement);
   if (pelement->IsActive() && dynamic_cast<suri::TerrainElement*>(pelement)) {
      // Si cambio de elemento reseteo la camara.
      if ((pterrain && pterrain->ForcedUpdate()) || (pelement != pActiveElement_)) {
         GetCanvas()->ResetCamera();
         pelement->SetShouldRender();  // fuerzo la renderizacion.
         pActiveElement_ = dynamic_cast<suri::TerrainElement*>(pelement);
         std::string srwkt = pActiveElement_->GetSpatialReference().c_str();
         Configuration::SetParameterEx("GISMode_World_Wkt", srwkt);
         CreateTerrain(pelement);
         DrawActiveVectorElements();
      }

      // seteo el paso de camara
      GetCanvas()->SetStep(
            dynamic_cast<suri::TerrainElement*>(pelement)->GetCameraStep());
      GetCanvas()->SetFocus();
      GetCanvas()->Render();
   } else if (!pelement->IsActive() && dynamic_cast<suri::TerrainElement*>(pelement)) {
      pActiveElement_ = NULL;
      ResetViewer();
      // Si es terreno y lo deshabilite cierro vectores
      LayerList *plist = pListViewer_->GetList();
      suri::LayerList::ElementListType::iterator it = plist->GetElementIterator(true);
      for (; it != plist->GetElementIterator(false); ++it) {
         Element *pelement = *it;
         pelement->Activate(false);
      }
   } else if (activecount != 0 && pActiveElement_) {
      pActiveElement_->SetShouldRender();
      UpdateTexture();
      DrawActiveVectorElements();
      GetCanvas()->SetFocus();
      GetCanvas()->Render();
   } else {
      // no pudo desplegar la capa e informa lo ocurrido
      pelement->Activate(false);
      SHOW_ERROR(message_RENDERER_ELEMENT_ERROR_s, pelement->GetName().c_str());
      return;
   }

   if (activecount == 0) {
      // Si no hay nada para habilitar resetea el viewer
      pActiveElement_ = NULL;
      ResetViewer();
      return;
   }
}

void Viewer3D::ElementUpdateForDynamicViewer(Model *pModel) {
   // Fix temporal para no realizar acciones si el visualizador activo no es 3D
   DynamicViewerWidget* pdynamic = dynamic_cast<DynamicViewerWidget*>(
         pDataViewManager_->GetViewportManager()->GetSelectedViewport());
   if (pdynamic && !dynamic_cast<Viewer3D*>(pdynamic->GetViewer()))
      return;

   LayerList *plist = pListViewer_->GetList();
   // valido que se trata de un elemento en la lista
   Element *pelement = plist->GetElement(pModel);
   pGlCanvas_->SetWorld(pWorld_);
   if (!pelement || !pElement_)
      return;

   int activecount =
         plist->GetActiveCount(
               plist->GetElementData(plist->GetElement(plist->GetElementCount() - 1))->group_);

   RemoveMultiplexorNode(pelement);
   if (dynamic_cast<suri::RasterElement*>(pelement) && pelement->IsActive()) {
      GetCanvas()->ResetCamera();
      pelement->SetShouldRender(); // fuerzo la renderizacion.
      pActiveElement_ = dynamic_cast<suri::RasterElement*>(pelement);
      std::string srwkt = pActiveElement_->GetSpatialReference().c_str();
      Configuration::SetParameterEx("GISMode_World_Wkt", srwkt);
      CreateTerrain(pelement);
      DrawActiveVectorElements();
      GetCanvas()->SetStep(Configuration::GetParameterEx("v3d_velocidad_camara", MIN_VALUE));
      GetCanvas()->SetFocus();
      GetCanvas()->Render();
   } else if ((dynamic_cast<suri::VectorElement*>(pelement) || !pelement->IsActive())
         && activecount != 0 && pActiveElement_) {
      if (!pTerrain_) {
         return;
      }
      pActiveElement_->SetShouldRender();
      UpdateTexture();
      DrawActiveVectorElements();
      GetCanvas()->SetFocus();
      GetCanvas()->Render();
   } else {
      // no pudo desplegar la capa e informa lo ocurrido
      pelement->Activate(false);
      SHOW_ERROR(message_RENDERER_ELEMENT_ERROR_s, pelement->GetName().c_str());
      return;
   }

   if (!dynamic_cast<suri::VectorElement*>(pelement))
      AddMultiplexorNode(pelement);

   if (activecount == 0) {
      // Si no hay nada para habilitar resetea el viewer
      pActiveElement_ = NULL;
      ResetViewer();
      return;
   }
}

/**
 * Recorre la LayerList y renderiza los vectores activos
 */
void Viewer3D::DrawActiveVectorElements() {
   LayerList *plist = pListViewer_->GetList();
   suri::LayerList::ElementListType::iterator it = plist->GetElementIterator(true);
   while (it != plist->GetElementIterator(false)) {
      Element *pelement = *it;
      if (pelement->IsActive() && dynamic_cast<suri::VectorElement*>(pelement)) {
         CreatePointVector(pelement);
         GetCanvas()->SetFocus();
         GetCanvas()->Render();
      }
      it++;
   }
}

/** Verifica y crea los modelos espaciales raster de las capas pasadas por parametro **/
bool CreateRasterModels(const std::vector<RasterElement*>& Rasters,
                        std::vector<RasterSpatialModel*>& Rmodels) {
   bool validrastermodels = true;
   for (size_t i = 0; validrastermodels && i < Rasters.size(); ++i) {
      RasterSpatialModel *ptexturemodel = RasterSpatialModel::Create(
            Rasters[i]->GetRasterModel());
      if (ptexturemodel)
         Rmodels.push_back(ptexturemodel);
      else
         validrastermodels = false;
   }
   return validrastermodels;
}

/** Configura el mundo del terreno 3D a desplegar
 * @param[in] pElement capa DEM a desplegar
 * @param[in] Intersection Este contiene la interseccion entre
 * el subset de las texturas y el DEM
 * @param[out] TerrainSubset subset del terreno a desplegar. i
 * @return true en caso de poder configurar correctamente el mundo
 * @return false en caso contrario
 **/
bool Viewer3D::ConfigureTerrainWorld(Element* pElement, Subset& Intersection,
                                     Subset& TerrainSubset) {
   TerrainElement *ptelement = dynamic_cast<TerrainElement*>(pElement);
   RasterSpatialModel *pterrainmodel = RasterSpatialModel::Create(
         pTerrainElement_->GetRasterModel());

   pterrainmodel->InverseTransform(TerrainSubset.ul_);
   pterrainmodel->InverseTransform(TerrainSubset.lr_);

// Defino tamano en pixeles del terreno
   int factor = Configuration::GetParameterEx("v3d_factor_terreno", MIN_VALUE);
   size_t height, width;
   if (Configuration::GetParameter("app_short_name", "") == wxT("SoPI")) {
      width = (size_t)((TerrainSubset.lr_.x_ - TerrainSubset.ul_.x_) / factor);
      height = (size_t)((TerrainSubset.lr_.y_ - TerrainSubset.ul_.y_) / factor);
   } else {
      width = (size_t)(
            (TerrainSubset.lr_.x_ - TerrainSubset.ul_.x_) / ptelement->GetTerrainFactor());
      height = (size_t)(
            (TerrainSubset.lr_.y_ - TerrainSubset.ul_.y_) / ptelement->GetTerrainFactor());
   }
   pTerrainWorld_->SetViewport(width, height);

   if (pWorld_ && pTerrainWorld_) {
      pWorld_->SetSpatialReference(pTerrainWorld_->GetSpatialReference());
      Subset window;
      pTerrainWorld_->GetWindow(window);
      pWorld_->SetWindow(window);
      pTerrainWorld_->GetWorld(window);
      pWorld_->SetWorld(window);
      oldCenter_ = window.GetCenter();
      oldSubset_ = window;
      int textwidth = 0, textheight = 0;
      pTerrainWorld_->GetViewport(textwidth, textheight);
      pWorld_->SetViewport(textwidth, textheight);
   }
   RasterSpatialModel::Destroy(pterrainmodel);

// aca se produjo 1 lectura => pTerrainMemoryCanvas_ contiene datos
// Verifico que los datos del terreno sean correctos
   if (pTerrainMemoryCanvas_->GetDataType().compare(DataInfo<void>::Name) == 0
         || pTerrainMemoryCanvas_->GetSizeX() == 0
         || pTerrainMemoryCanvas_->GetSizeY() == 0) {
      pElement->Activate(false);
      REPORT_ERROR(wxT("D: Error al renderizar. El canvas del terreno es invalido"));
      return false;
   }
   return true;
}

/**
 * Configura el mundo para la renderizacion de texturas sobre un terreno 3D (DEM)
 * @param[in] pElement
 * @param[in] Rmodels
 * @param[in] Rasters
 * @param[in] TextureSubset
 * @param[out] pTextureWorld
 * return true en caso de configurar correctamente el mundo
 * return false en caso contrario
 **/
bool Viewer3D::ConfigureTextureWorld(Element* pElement,
                                     std::vector<RasterSpatialModel*>& Rmodels,
                                     std::vector<RasterElement*>& Rasters,
                                     Subset& TextureSubset, Canvas* pTextureCanvas,
                                     World* pTextureWorld) {
   std::string application = Configuration::GetParameter("app_short_name", "");
   TerrainElement *ptelement = dynamic_cast<TerrainElement*>(pElement);
   RasterSpatialModel* ptexturemodel = Rmodels.at(0);
   ptexturemodel->InverseTransform(TextureSubset.ul_);
   ptexturemodel->InverseTransform(TextureSubset.lr_);
   // Encuentro tamanio de recuadro de Textura
   int factor = Configuration::GetParameterEx("v3d_factor_textura", MIN_VALUE);
   if (application != wxT("SoPI"))
      factor = ptelement->GetTextureFactor();
   double elemtexturex, elemtextrurey;
   elemtexturex = (TextureSubset.lr_.x_ - TextureSubset.ul_.x_) / factor;
   elemtextrurey = (TextureSubset.lr_.y_ - TextureSubset.ul_.y_) / factor;
   int texsize = pTerrain_->GetTextureSize(elemtexturex, elemtextrurey);
   for (size_t i = 0; i < Rasters.size(); ++i) {
      RasterElement* prasterelem = Rasters[i];
      if (prasterelem->IsActive() && prasterelem) {
         double elemtx = (TextureSubset.lr_.x_ - TextureSubset.ul_.x_) / factor;
         double elemty = (TextureSubset.lr_.y_ - TextureSubset.ul_.y_) / factor;
         int auxtexsize = pTerrain_->GetTextureSize(elemtx, elemty);
         texsize = auxtexsize > texsize ? auxtexsize : texsize;
      }
   }

   // Encuentro extremos en raster de recuadro de Textura
   Subset windowsubset;
   windowsubset.ul_.x_ = TextureSubset.ul_.x_;
   windowsubset.ul_.y_ = TextureSubset.ul_.y_;
   windowsubset.lr_.x_ = TextureSubset.ul_.x_ + factor * texsize;
   windowsubset.lr_.y_ = TextureSubset.ul_.y_ + factor * texsize;
   // Transformo coordenadas de raster a coord de mundo
   ptexturemodel->Transform(windowsubset.ul_);
   ptexturemodel->Transform(windowsubset.lr_);
   // Seteo viwport y window de textura con recuadro encontrado
   if (!pTextureWorld) return false;
   pTextureWorld->SetWindow(windowsubset);
   pTextureWorld->SetViewport(texsize, texsize);
   Subset window;
   pTextureWorld->GetWorld(window);
   pTextureWorld->SetWindow(window);

   // Verifico que los datos del terreno sean correctos
   if (pTextureCanvas->GetDataType().compare(DataInfo<void>::Name) == 0
         || pTextureCanvas->GetSizeX() == 0 || pTextureCanvas->GetSizeY() == 0) {
      pElement->Activate(false);
      return false;
   }
   return true;
}

/** Crea el terreno */
/**
 * Crea los raster de terreno y textura y sus respectivas listas, mundos,
 * renderizationManager, canvas.
 * Crea el Terrain, setea sus valores.
 * El window de terreno contendra la interseccion de los mundos de
 * textura y terreno
 * El window de textura tendra el tamano de un recuadro de OpenGl
 * @param[in] pElement elemento tipo TerrainElement del que se quiere el Terrain
 */
void Viewer3D::CreateTerrain(Element *pElement) {

   RasterElement *praster = dynamic_cast<RasterElement*>(pElement);
   TerrainElement *pterrain = dynamic_cast<TerrainElement*>(pElement);

   if (!praster && !pterrain)
      return;

   if (Configuration::GetParameter("app_short_name", "") == wxT("SoPI") && !pElement_)
      return;
   // Reseteo el viewer para eliminar punteros creados en Update anterior.
   ResetViewer();

   // Creo el elemento raster Terreno
   if (Configuration::GetParameter("app_short_name", "") == wxT("SoPI"))
      pTerrainElement_ = RasterElement::Create(pElement_->GetUrl().c_str());
   else
      pTerrainElement_ = RasterElement::Create(pElement->GetUrl().c_str());

   // agrego nodo data_cast
   wxXmlNode *prnode = pTerrainElement_->GetNode(RENDERIZATION_NODE);
   pTerrainElement_->AddNode(prnode, DATA_CAST_NODE, wxT(""), OUTPUT_PROPERTY,
                             DataInfo<float>::Name, true);
   // Obtengo el elemento raster de Textura
   RasterElement* ptexture = NULL;
   if (Configuration::GetParameter("app_short_name", "") == wxT("SoPI"))
      ptexture = dynamic_cast<RasterElement*>(pElement);
   else
      ptexture = pterrain->GetTexture();

   // Creo RasterSpatialModel de terreno y textura
   std::vector<RasterSpatialModel*> rmodels;
   // Agrego vectores activos a LayerList de textura antes de renderizar.
   // TODO(Ramiro - TCK #3335): Usar texturas diferentes para cada capa, con
   // OpenGL se pueden superponer.
   std::vector<RasterElement*> rasters;
   if (!pTextureList_) {
      pTextureList_ = new LayerList();
      LayerList *plist = pListViewer_->GetList();
      suri::LayerList::ElementListType::iterator it = plist->GetElementIterator(true);
      for (; it != plist->GetElementIterator(false); ++it) {
         Element *pelement = *it;
         if (dynamic_cast<suri::TerrainElement*>(pelement) || !pelement->IsActive())
            continue;
         std::string elemtype[] = { "VectorElement", "RasterElement" };
         Element* pelem = NULL;
         for (int i = 0; !pelem && i < 2; ++i)
            pelem = Element::Create(elemtype[i], pelement->GetUrl().c_str());
         if (pelem) {
            /** se reemplazan las propiedades de visualizacion **/
            pelem->AddNode(pelem->GetNode(),
                           new wxXmlNode(*(pelement->GetNode(RENDERIZATION_NODE))),
                           true);
            pTextureList_->AddElement(pelem);
            pelem->SetShouldRender();
            pelem->Activate();
            if (dynamic_cast<RasterElement*>(pelem))
               rasters.push_back(dynamic_cast<RasterElement*>(pelem));
         }
      }
   }
   /** si no hay capas de tipo raster activas se utiliza el modelo raster del dem activo **/
   if (rasters.empty()) {
      pTextureList_->AddElement(ptexture);
      ptexture->SetShouldRender();
      ptexture->Activate();
      rasters.push_back(ptexture);
   }

   if (!CreateRasterModels(rasters, rmodels))
   REPORT_AND_FAIL("D:No se pudo crear RasterSpatialModel");

   // Creo los renderization manager del terreno y textura
   InitRenderizationManager(pTerrainElement_, pTerrainRenderizationManager_,
                            pTerrainWorld_, pTerrainList_, pTerrainMemoryCanvas_,
                            pExtentManagerTerrain_);
   InitRenderizationManager(pTerrainRenderizationManager_, pTextureWorld_,
                            pTextureList_, pTextureMemoryCanvas_,
                            pExtentManagerTexture_);

   // Creo el terreno
   pTerrain_ = new Terrain(pTerrainWorld_, pTerrainMemoryCanvas_, pTextureWorld_,
                           pTextureMemoryCanvas_);
   GetCanvas()->SetTerrain(pTerrain_);
   // Factor de Textura y terreno
   if (Configuration::GetParameter("app_short_name", "") == wxT("SoPI")) {
      pTerrain_->SetTerrainFactor(Configuration::GetParameterEx("v3d_factor_terreno", MIN_VALUE));
      pTerrain_->SetTextureFactor(Configuration::GetParameterEx("v3d_factor_textura", MIN_VALUE));
      pTerrain_->SetExageration(Configuration::GetParameterEx("v3d_exageracion", MIN_VALUE));
      pTerrain_->SetPixelSize(PixelSize(pTerrainElement_));
   } else {
      pTerrain_->SetTerrainFactor(pterrain->GetTerrainFactor());
      pTerrain_->SetTextureFactor(pterrain->GetTextureFactor());
      pTerrain_->SetExageration(pterrain->GetExagerationFactor());
      pTerrain_->SetPixelSize(PixelSize(pElement));
   }

   // Guardo en los mundos la dimencion de la imagen
   pTerrainWorld_->SetChanged();
   // fuerzo una actualizacion para conocer el extent del mundo (es decir el tamanio del terreno)
   pTerrainWorld_->SendViewerUpdate();
   pTextureWorld_->SetChanged();
   // fuerzo una actualizacion para conocer el extent del mundo (es decir el tamanio de la textura)
   pTextureWorld_->SendViewerUpdate();

   // Obtengo el mundo de la textura y el terreno
   Subset terrainsubset;
   Subset texturesubset;
   pTerrainWorld_->GetWorld(terrainsubset);
   pTextureWorld_->GetWorld(texturesubset);

   // Obtengo interseccion de textura y terreno
   Subset intersection = Intersect(terrainsubset, texturesubset);
   // Remplazo el window de textura con interseccion
   pTerrainWorld_->SetWorld(intersection);
   pTextureWorld_->SetWorld(intersection);
   pTerrainWorld_->SetWindow(intersection);
   // Encuentro los extremos en PL de la interseccion en el terreno y en la textura
   terrainsubset = intersection;
   texturesubset = intersection;

   bool terrainworld = false, textureworld = false;
   terrainworld = ConfigureTerrainWorld(pElement, intersection, terrainsubset);
   textureworld = ConfigureTextureWorld(
         pElement, rmodels, rasters, texturesubset, pTextureMemoryCanvas_, pTextureWorld_);

   if (terrainworld && textureworld) {
      // Cambio color de fondo
      wxColour colour;
      if (Configuration::GetParameter("app_short_name", "") == wxT("SoPI")) {
         wxColour aux(Configuration::GetParameterEx("v3d_color", "#FFFFFF"));
         colour = aux;
      } else {
         wxColour aux(wxString(pterrain->GetBackgroundColor()));
         colour = aux;
      }
      GetCanvas()->SetBackGroundColour(static_cast<float>(colour.Red()),
                                       static_cast<float>(colour.Green()),
                                       static_cast<float>(colour.Blue()));
      if (pColourPickerButton_)
         pColourPickerButton_->SetColour(colour);

      GetCanvas()->InitTerrain();
      GetCanvas()->SetReady(true);

      // evita que vuelva a renderizar el terreno si no hay cambios que lo ameriten.
      if (pterrain)
         pterrain->SetShouldRender(false);
      if (praster)
         praster->SetShouldRender(false);
   } else {
      REPORT_ERROR(wxT("D: Error al renderizar. El canvas del terreno es invalido"));
   }

   // Destroy de modelos espaciales
   for (size_t i = 0; i < rmodels.size(); ++i) {
      RasterSpatialModel* pmodel = rmodels[i];
      RasterSpatialModel::Destroy(pmodel);
   }
}

/**
 * Crea los objetos que necesita el RenderizationManger y los asocia
 * @param[out] pRenderizationManager RenderizationManager creado
 * @param[out] pWorld Se carga con sist de referencia y extent del elemento
 * @param[out] pList Lista con elementos a renderizar
 * @param[out] pCanvas MemoryCanvas que usa el RenderizationManager
 * @param[out] pExtentManager se carga con WorldExtentManager
 */
void Viewer3D::InitRenderizationManager(RenderizationManager* &pRenderizationManager,
                                        World* &pWorld, LayerList* &pList,
                                        Canvas* &pCanvas,
                                        WorldExtentManager* pExtentManager) {
   InitRenderizationManager(NULL, pRenderizationManager, pWorld, pList, pCanvas,
                            pExtentManager);
}
/**
 * Crea los objetos que necesita el RenderizationManger y los asocia
 * @param[in] pElement Elemento del Raster a renderizar
 * @param[out] pRenderizationManager RenderizationManager creado
 * @param[out] pWorld Se carga con sist de referencia y extent del elemento
 * @param[out] pList Lista con elementos a renderizar
 * @param[out] pCanvas MemoryCanvas que usa el RenderizationManager
 * @param[out] pExtentManager se carga con WorldExtentManager
 */
void Viewer3D::InitRenderizationManager(RasterElement* pElement,
                                        RenderizationManager* &pRenderizationManager,
                                        World* &pWorld, LayerList* &pList,
                                        Canvas* &pCanvas,
                                        WorldExtentManager* pExtentManager) {
   if (!pList) pList = new LayerList();
// Creo los mundos para el terreno y la textura
   delete pWorld;
   pWorld = new World();
// Guardo en world un subset con extent de raster
   Subset world(0, 0, 0, 0);
   if (pElement) {
      pElement->GetElementExtent(world);
      pWorld->SetWorld(world);
   }
   pWorld->SetSpatialReference(Configuration::GetParameterEx("GISMode_World_Wkt", ""));
   suri::LayerList::ElementListType::iterator it = pList->GetElementIterator(true);
   for (; it != pList->GetElementIterator(false); ++it) {
      Element *pelement = *it;
      Subset elemworld;
      pelement->GetElementExtent(elemworld);
      ParameterCollection params;
      params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                   pelement->GetSpatialReference().c_str());
      params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                   pWorld->GetSpatialReference());
      TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
      CoordinatesTransformation *pcoordinatetransform = pfactory->Create(
            TransformationFactory::kExact, params);
      TransformationFactoryBuilder::Release(pfactory);
      if (pcoordinatetransform != NULL) pcoordinatetransform->Transform(elemworld);
      if (world != Subset(0, 0, 0, 0))
         world = Join(world, elemworld);
      else
         world = elemworld;
   }
   pWorld->SetWorld(world);
// Creo los RenderizationManager del terreno y de la textura
   pRenderizationManager = new RenderizationManager(pList, pWorld);
// Cargo un extent manager que no modifica el mundo al renderizar
   pExtentManager = new Viewer3DWorldExtentManager();
   pRenderizationManager->SetWorldExtentManager(pExtentManager);
   if (pElement) {
      pList->AddElement(pElement);
      pElement->Activate();
   }
// Creo los canvas
   delete pCanvas;
   pCanvas = new MemoryCanvas();
// Aniado los MemoryCanvas
   pRenderizationManager->SetCanvas(pCanvas);
}

/**
 * Agrega un punto al canvas. Devuelve el resultado de invocar a
 * TerrainGLCanvas::InitPointVector.
 * @param[in] pElement elemento del vector que se quiere agregar.
 * @return true si pudo dibujar el vector
 * @return false si no pudo dibujar el vector
 */
bool Viewer3D::CreatePointVector(Element *pElement) {
   VectorElement *pelement = dynamic_cast<VectorElement*>(pElement);
   if (!pelement) {
      return false;
   }
   return GetCanvas()->InitPointVector(pelement);
}

/**
 * Retorna el tamano de pixel en unidades de la referencia espacial
 * @param[in] pElement Elemento con modelo espacial raster
 * @return Tamanio de pixel.
 */
size_t Viewer3D::PixelSize(Element *pElement) {
   std::string rastermodel;
   wxString pathnode = GEORREFERENCE_NODE + wxString("|") + RASTER_SPATIAL_MODEL_NODE;
   wxXmlNode *pfactornode = pElement->GetNode(pathnode);
   rastermodel = pfactornode->GetNodeContent().c_str();
   RasterSpatialModel::Parameters parameters;
   parameters = RasterSpatialModel::WktToParameters(rastermodel);
   double pixelSize = parameters.pixelSizes_[0];
   return (size_t) pixelSize;
}

/**
 * Retorna el Canvas del viewer. Si no existe lo crea.
 * @return canvas del viewer
 */
TerrainGLCanvas *Viewer3D::GetCanvas() {
   return pGlCanvas_;
}

/** Obtiene la transformacion asociada **/
CoordinatesTransformation* Viewer3D::GetTransformation() {
   return pTransform_;
}

/**
 * Obtiene el estilo vectorial configurado en la capa ( XML )
 * teniendo en cuenta la configuracion vieja que posee
 * el nodo <estilo>
 */
VectorStyle* Viewer3D::GetVectorStyle(Element* pElement) {
   VectorStyle* pvectorstyle = VectorStyle::Create(
         dynamic_cast<suri::VectorElement*>(pElement)->GetStyle(DEFAULT_LAYER));
   if (!pvectorstyle) {
      wxXmlNode* prnode = pElement->GetNode(wxT(RENDERIZATION_NODE), NULL);
      wxXmlNode* pchild = prnode->GetChildren();
      while (pchild && pchild->GetName().compare(LAYERS_NODE) != 0)
         pchild = pchild->GetNext();
      std::string wktstyle;
      pchild = pchild->GetChildren() ? pchild->GetChildren()->GetChildren() : NULL;
      if (pchild) {
         wktstyle = pchild->GetContent().c_str();
      }
      pvectorstyle = VectorStyle::Create(wktstyle);
      if (!pvectorstyle) return NULL;
   }
   return pvectorstyle;
}


void Viewer3D::EraseCrosshair() {
   Coordinates point(-1, -1);
   DrawCrosshair(point);
}

void Viewer3D::DrawCrosshair(Coordinates Point) {
   pTerrain_->SetSearchedPoint(Point);
   UpdateTexture();
}


/**
 * Actualiza las texturas en funcion de la activacion/desactivacion de
 * elementos en la lista de capas
 **/
bool Viewer3D::UpdateTexture() {
   if (!pTerrain_ || !pActiveElement_)
      return false;

   LayerList* ptexturelist = new LayerList;
   World* ptextureworld = NULL;
   Canvas* ptexturecanvas = NULL;
   std::vector<RasterElement*> rasters;
   std::vector<RasterSpatialModel*> rmodels;
   LayerList *plist = pListViewer_->GetList();
   suri::LayerList::ElementListType::iterator it = plist->GetElementIterator(true);
   for (; it != plist->GetElementIterator(false); ++it) {
      Element *pelement = *it;
      if (dynamic_cast<suri::TerrainElement*>(pelement) || !pelement->IsActive())
         continue;
      std::string elemtype[] = { "VectorElement", "RasterElement" };
      Element* pelem = NULL;
      for (int i = 0; !pelem && i < 2; ++i)
         pelem = Element::Create(elemtype[i], pelement->GetUrl().c_str());
      if (pelem) {
         /** se reemplazan las propiedades de visualizacion **/
         pelem->AddNode(pelem->GetNode(),
                        new wxXmlNode(*(pelement->GetNode(RENDERIZATION_NODE))), true);
         ptexturelist->AddElement(pelem);
         pelem->SetShouldRender();
         pelem->Activate();
         if (dynamic_cast<RasterElement*>(pelem))
            rasters.push_back(dynamic_cast<RasterElement*>(pelem));
      }
   }
   /** si no hay capas de tipo raster activas se utiliza el modelo raster del dem activo **/
   if (rasters.empty()) {
      // Obtengo el elemento raster de Textura
      RasterElement* ptexture =
            dynamic_cast<TerrainElement*>(pActiveElement_)->GetTexture();
      ptexturelist->AddElement(ptexture);
      ptexture->SetShouldRender();
      ptexture->Activate();
      rasters.push_back(ptexture);
   }
   if (!CreateRasterModels(rasters, rmodels)) {
      REPORT_DEBUG("D:No se pudo crear RasterSpatialModel");
      return false;
   }

   InitRenderizationManager(pTerrainRenderizationManager_, ptextureworld,
                            ptexturelist, ptexturecanvas,
                            pExtentManagerTexture_);
// Guardo en los mundos la dimencion de la imagen
   ptextureworld->SetChanged();  // fuerzo una actualizacion para conocer el extent del mundo (es
// decir el tamanio de la textura)
   ptextureworld->SendViewerUpdate();

// Obtengo el mundo de la textura y el terreno
   Subset terrainsubset;
   Subset texturesubset;
   pTerrainWorld_->GetWorld(terrainsubset);
   ptextureworld->GetWorld(texturesubset);

// Obtengo interseccion de textura y terreno
   Subset intersection = Intersect(terrainsubset, texturesubset);
// Remplazo el window de textura con interseccion
   ptextureworld->SetWorld(intersection);
// Encuentro los extremos en PL de la interseccion en el terreno y en la textura
   terrainsubset = intersection;
   texturesubset = intersection;
   bool success = true;
   if (ConfigureTextureWorld(pActiveElement_, rmodels, rasters, texturesubset,
                             ptexturecanvas, ptextureworld)) {
      pTerrain_->UpdateTexture(ptextureworld, ptexturecanvas);
      delete pTextureWorld_;
      delete pTextureList_;
      pTextureWorld_ = ptextureworld;
      pTextureMemoryCanvas_ = ptexturecanvas;
      pTextureList_ = ptexturelist;
   } else {
      success = true;
      delete ptextureworld;
      delete ptexturecanvas;
   }

   // Destroy de modelos espaciales
   for (size_t i = 0; i < rmodels.size(); ++i) {
      RasterSpatialModel* pmodel = rmodels[i];
      RasterSpatialModel::Destroy(pmodel);
   }
   return success;
}

/** Hace ZoomIn */
void Viewer3D::ZoomIn(int X, int Y, int Factor, int Z) {
}

/** Hace ZoomOut */
void Viewer3D::ZoomOut(int X, int Y, int Factor, int Z) {
}

/** Cambia la ventana del mundo */
void Viewer3D::ChangeWindow(const Subset &NewWindow, bool PreventUpdate) {
   Subset window = NewWindow;
      TRY {
         pWorld_->PreventNextViewerUpdate(PreventUpdate);
         pWorld_->World::SetWindow(window);
      } CATCH {
         // Aca se estaba volviendo al subset anterior lo que traia problemas
         // con el zoom cuando en realidad no habia ningun problema en ningun
         // lado. Si hicieron varias pruebas con imagenes con SRS geograficos
         // y proyectados, con y sin vectores, y no hubo problemas.
         // - Deshacer cambio si hace falta -
      }
}

/** Setea el WorldExtentManager */
void Viewer3D::SetWorldExtentManager(WorldExtentManager* pExtentManager) {
}
/** Retorna el WorldExtentManager */
WorldExtentManager* Viewer3D::GetWorldExtentManager() {
      return NULL;
}

/** Metodo de actualizacion de la lista, el mundo y/o los elementos. */
void Viewer3D::UpdateViewers(Model *pModel) {
   if (!pGlCanvas_->GetUpdate()) {
      pGlCanvas_->SetUpdate(true);
      return;
   }
   float radius = pTerrain_->GetTerrainRadius()*2;  // radio de la camara (eje z)
   Subset window;
   Subset world;
   int vwidth = 0;
   int vheight = 0;
   Coordinates cworld;
   pTerrainWorld_->GetViewport(vwidth, vheight);
   pWorld_->GetWindow(window);
   Dimension newD(window);
   if ((newD.GetHeight() == 0) || (newD.GetHeight() == 0)) return;
   if (window.GetCenter() != oldCenter_) {
      Coordinates viewOld;
      Coordinates viewNew;
      pWorld_->InverseTransform(oldCenter_, viewOld);
      pWorld_->InverseTransform(window.GetCenter(), viewNew);
      Dimension oldD(oldSubset_);
      float factorH = (newD.GetHeight() / oldD.GetHeight());
      float factorW = (newD.GetWidth() / oldD.GetWidth());
      // Si la dimension inicial es 0, guardo el subset y seteo los factores de amplificacion a 1
      if (oldD.GetHeight() == 0) {
         factorH = 1;
         factorW = 1;
         oldSubset_ = window;
      }
      pGlCanvas_->InitCamera(radius, 90, -1);
      double normalizationConstantX = 0;
      double normalizationConstantY = 0;
      pTerrain_->GetNormalizationConstant(normalizationConstantX, normalizationConstantY);
      // Calculo la distancia con la posicion inicial
      float distx = (-viewNew.x_ + viewOld.x_);
      float disty = (-viewNew.y_ + viewOld.y_);
      // Normalizo las distancias
      float normalx = distx / normalizationConstantX;
      float normaly = disty / normalizationConstantY;
      // multiplico por el factor de amplificacion
      float xGlViewCoord = normalx*factorW;
      float yGlViewCoord = normaly*factorH;
      if (factorH > 1.1) {
         yGlViewCoord = normaly * 1 / factorH;
      }
      if (factorW > 1.1) {
         xGlViewCoord = normalx * 1 / factorW;
      }
      float zGlViewCoord = -(factorH * radius - radius);
      // Evito mostrar pantalla en blanco por traspasar elevacion
      if (zGlViewCoord > radius - 6 * pTerrain_->GetExageration()) {
         zGlViewCoord = radius - 6 * pTerrain_->GetExageration();
      }
      pGlCanvas_->GetCamera()->Translate(xGlViewCoord, -yGlViewCoord, zGlViewCoord);
      GetCanvas()->Render();
   }
}

/** Verifica el estado de la renderizacion. */
void Viewer3D::CheckRenderizationStatus() {
}

/** Setea el elemento raster del terreno */
void Viewer3D::SetTerrain(RasterElement* pRasterElement) {
   if (!pRasterElement) {
      AddMultiplexorNode(pElement_);
      pElement_ = NULL;
      return;
   }
   pElement_ = pRasterElement;
}

/**
 * Borra el nodo multiplexor de las capas raster de una banda
 * @param[in] pElement elemento
 * @return true si pudo borrar el nodo
 * @return false si no pudo borrar el nodo
 */
void Viewer3D::RemoveMultiplexorNode(Element* pElement) {
   RasterElement* praster = dynamic_cast<RasterElement*>(pElement);
   if (!praster || (praster && praster->GetBandCount() != 1))
      return;
   wxXmlNode* pmultnode = pElement->GetNode(
         RENDERIZATION_NODE NODE_SEPARATION_TOKEN BAND_MULTIPLEXOR_NODE);
   if (!pmultnode)
      return;
   pElement->RemoveNode(pElement->GetNode(RENDERIZATION_NODE), pmultnode);
}

/**
 * Agrega a las capas raster el nodo multiplexor borrado
 * @param[in] pElement elemento
 * @param[in] HasMultiplexorNode indica si fue borrado el nodo del elemento
 */
void Viewer3D::AddMultiplexorNode(Element* pElement) {
   RasterElement* praster = dynamic_cast<RasterElement*>(pElement);
   if (!praster || (praster && praster->GetBandCount() != 1))
      return;
   wxXmlNode* pmultnode = pElement->GetNode(
         RENDERIZATION_NODE NODE_SEPARATION_TOKEN BAND_MULTIPLEXOR_NODE);
   if (pmultnode)
      return;
   wxXmlNode* pmulnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, BAND_MULTIPLEXOR_NODE);
   wxXmlNode* prendernode = pElement->GetNode(RENDERIZATION_NODE);
   wxXmlNode* pchild = prendernode ? prendernode->GetChildren() : NULL;
   while (pchild && pchild->GetNext())
      pchild = pchild->GetNext();
   prendernode->InsertChildAfter(pmulnode, pchild);
}
}  // namespace suri
