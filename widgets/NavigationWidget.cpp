/*! \file NavigationWidget.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standar
#include <cmath>

// Includes Suri
#include "suri/DataViewManager.h"
#include "suri/NavigationWidget.h"
#include "suri/messages.h"
#include "resources.h"
#include "suri/ViewerWidget.h"
#include "logmacros.h"
#include "suri/LatitudeLongitudeInputWidget.h"
#include "suri/RasterElement.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/CoordinatesNavigationInputInterface.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"
#include "wx/hyperlink.h"

// forward

/** inicializa los recursos de la ventana */
void InitEditCoordinatesXmlResource();

// Defines

/** Separador decimal */
#define DECIMAL_SEPARATOR "."
/** indice del identificador del control de hipervinculo */
#define HYPERLINK_INDEX 0
/** indice del identificador del control de texto de grados */
#define DEGREE_INDEX 1
/** indice del texto de */
#define LABEL_INDEX 2
/** Los grupos estan definidos en Context */
#define RASTERELEMENT_GROUP_POSITION 6

/** namespace suri */
namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(NavigationWidgetEvent, NavigationWidget)
      IMPLEMENT_EVENT_CALLBACK(OnClose, OnClose(event), wxCloseEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/**
 * Constructor
 * Crea la ventana de la herramienta y la oculta. Crea y registra los viewers a
 * la lista de elementos.
 * @param[in] pParent ventana padre
 * @param[in] pViewer viewer asociado
 * @param[in] pInputWidget widget para el ingreso de coordenadas
 */
NavigationWidget::NavigationWidget(wxWindow *pParent, suri::ViewerWidget *pViewer,
                                   CoordinatesNavigationInputInterface* pCoordinateInput,
                                   DataViewManager* pDataViewManager) :
      Widget(pParent), pViewer_(pViewer),
      pCoordinateInputPart_(pCoordinateInput),
      NEW_EVENT_OBJECT(NavigationWidgetEvent),
      pDataViewManager_(pDataViewManager) {
   // Creo ventana
   /**
    * Estas medidas no se deberian tener en cuenta, pero hubo que fijarlas
    * porque en Ubuntu los botones de los widget de navegacion no se
    * visualisaban completos (se corta la parte de abajo).
    * \todo los widgets deberian fijar el tamanio automaticamente.
    */
   wantedWidth_ = 150;
   wantedHeight_ = 110;

   windowTitle_ = GetWindowTitle();
//   CreateTool(NULL, false);
// Al modificarse la estructura de ventanas hay punteros colgantes en este codigo
#ifdef __UNUSED_CODE__
   // Oculto la ventana
   pParentWindow_->Show(false);
   pParentWindow_->GetSizer()->Layout();
   pParentWindow_->GetParent()->GetSizer()->Layout();
#endif
   // Verifico que existan todos los elementos para la operacion
   if (pViewer_ && pViewer_->GetList()) {
      // Creo y registro los viewers
      pListViewer_ = new ListView<NavigationWidget>(this,
                                                    &NavigationWidget::ListUpdate);
      pViewer_->GetList()->RegisterViewer(pListViewer_);
   }
}


/**
 * Destructor
 * Si el widget de ingreso de coordenadas es autonomos (no se creo un wxFrame
 * para la herramienta), se destruye.
 */
NavigationWidget::~NavigationWidget() {
   // TODO(Gabriel - TCK #2344): Ver si despues se tiene que sacar
#ifdef __UNUSED_CODE__
   if (isAutonomous_) {
      pInputWidget_->GetToolWindow()->Destroy();
   }
#endif
   GetFrame() ? GetFrame()->Disconnect(
                      wxEVT_CLOSE_WINDOW,
                      wxCloseEventHandler(NavigationWidgetEvent::OnClose), NULL,
                      pEventHandler_) :
                true;
   DELETE_EVENT_OBJECT;
   pViewer_ = NULL;
}

// ------------------------------------------------------------------------------
/**
 * Carga herramieta que permite al usuario ingresar coordenadas en latitud
 * y longitud.
 * Hace connect de cerrar, click sobre iconos 'n/s' y 'e/o' y sobre eventos
 * lostfocus de los textCtrl para actualizar signo.
 * @return bool que indica si se pudo crear la ventana
 */
bool NavigationWidget::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_EDIT_COORDINATES"));
   wxWindow *ptoolbar = XRCCTRL(*pToolWindow_, wxT("ID_BUTTONS_PANEL"), wxPanel);
   if (!ptoolbar) {
      return false;
   }

   InitializeToolbar(ptoolbar, pViewer_, pViewer_ ? pViewer_->GetList() : NULL);
   if (isAutonomous_) {
      GetFrame()->Connect(wxEVT_CLOSE_WINDOW,
                          wxCloseEventHandler(NavigationWidgetEvent::OnClose), NULL,
                          pEventHandler_);
   }

   pCoordinateInputPart_->SetDataViewManager(pDataViewManager_);
   AddControl(pCoordinateInputPart_->GetInputWidget(), wxT("ID_EDIT_COORDINATES_CONTENT"));
   return true;
}

// ------------------------------------------------------------------------------
/**
 * Devuelve titulo de la ventana
 * @return string con el titulo de la ventana
 */
const wxString NavigationWidget::GetWindowTitle() const {
   return _(caption_NAVIGATION_TOOL);
}

/**
 * metodo virtual que se llama cuando se agrega boton a tool
 * Si la ventana no era visible se muestra
 * @param[in] pTool boton agregado
 */
void NavigationWidget::DoOnAddButton(Button *pTool) {
   pParentWindow_->Show(true);
   pParentWindow_->GetSizer()->Layout();
   pParentWindow_->GetParent()->GetSizer()->Layout();
}

/**
 * metodo virtual que se llama cuando se elimina boton a tool
 * Si no quedan botones se esconde el tool.
 * @param[in] pTool boton eliminado
 */
void NavigationWidget::DoOnRemoveButton(Button *pTool) {
   if (GetButtonCount() == 0) {
      pParentWindow_->Show(false);
   }

   pParentWindow_->GetSizer()->Layout();
   pParentWindow_->GetParent()->GetSizer()->Layout();
}

/**
 * retorna la coordenada que ingresaron e informa si es valida. Si la
 * coordenada es valida actualiza el signo del control llamado a
 * UpdateControlSign.
 * @param[out] LatLongCoordinate coordenada que se ingreso en control
 * @return bool que indica si la coordenada en control es valida
 */
bool NavigationWidget::GetCoordinate(Coordinates &LatLongCoordinate) {
   return pCoordinateInputPart_->GetCoordinate(LatLongCoordinate);
}

/**
 * retorna la coordenada que ingresaron e informa si es valida en el sistema de
 * referencia que se pasa por parametro. Si la coordenada es valida actualiza el
 * signo del control llamado a UpdateControlSign.
 * @param[out] LatLongCoordinate coordenada que se ingreso en control
 * @param[in]  OutputSpatialReference Sistema de Referencia sobre el cual se quiere
 * obtener los puntos
 * @return bool que indica si la coordenada en control es valida
 */
bool NavigationWidget::GetCoordinate(Coordinates &LatLongCoordinate,
                                     const std::string& OutputSpatialReference) {
   if (!pCoordinateInputPart_)
      return false;
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn, OutputSpatialReference);
   params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                pCoordinateInputPart_->GetSpatialReference());
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact,
                                                     params);
   TransformationFactoryBuilder::Release(pfactory);

   pCoordinateInputPart_->GetCoordinate(LatLongCoordinate);
   bool returnvalue = true;
   if (!pct || !pct->Transform(LatLongCoordinate))
      returnvalue = false;
   if (pct)
      delete pct;
   return returnvalue;
}

/**
 * cambia la herramienta para ingresar coordenadas
 * @param[in] pNavigationWidget nueva herramienta para ingresar coordenadas
 * @return bool que indica si el pNavigationWidget recibido es valido (o NULL)
 * \attention El widget sera responsabilidad de esta clase. No eliminar
 */
bool NavigationWidget::SetInputWidget(CoordinatesNavigationInputInterface* pNavigationWidget) {
   if (pNavigationWidget) {
      RemoveControl(pCoordinateInputPart_->GetInputWidget());
      pCoordinateInputPart_ = pNavigationWidget;
      AddControl(pCoordinateInputPart_->GetInputWidget(), wxT("ID_EDIT_COORDINATES_CONTENT"));
      pCoordinateInputPart_->GetInputWidget()->CreateTool();
      pParentWindow_->Fit();
   }
   return (pNavigationWidget != NULL);
}

/**
 * Devuelve la herramienta para ingtresar coordenadas
 * @return pInputWidget_ herramienta para ingresar coordenadas
 * \attention El widget devuelto es responsabilidad de esta clase.
 *             No eliminar externamente.
 */
CoordinatesNavigationInputInterface* NavigationWidget::GetInputWidget() {
   return pCoordinateInputPart_;
}
/**
 * Si el NavigationWidget tiene su propia ventana se oculta
 * cuando se cierra, en vez de eliminar.
 * @param[in] Event evento
 */
void NavigationWidget::OnClose(wxCloseEvent &Event) {
   if (isAutonomous_ && pToolWindow_ && pToolWindow_->GetParent()) {
      pToolWindow_->GetParent()->Show(false);
      Event.Skip();
   }
}

/**
 * Objetivo: Devuelve el icono de propiedades
 * Por ahora no tiene en cuenta el X e Y dado que devuelve el de 32x32 pixels
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho del icono
 * @param[in] Y alto del icono
 */
void NavigationWidget::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_NAVIGATION_WIDGET, ToolBitmap);
}

/**
 * Metodo que se ejecuta ante cambios en la lista de elementos. Obtiene el
 * elemento raster activo, si puede habilida el control wx, si falla lo
 * deshabilita.
 * @param[in] pModel model
 */
void NavigationWidget::ListUpdate(Model *pModel) {
   if (!pViewer_) return;
	bool enable = true;
// Se comenta ya que esto hace que se desactive la lista y no la vuelva a activar
#ifdef __LISTUPDATE_PROBLEM_SOLVED__
   LayerList* plist = pViewer_->GetList();
	// Si hay un elemento raster activo en la lista se activa el control wx
   if (!plist
         || !dynamic_cast<RasterElement*>(plist->GetActiveElement(
               0, RASTERELEMENT_GROUP_POSITION))) {
      enable = false;
   }
#endif
   // Verifico que existan todos los elementos para la operacion
   if (pToolWindow_) {
      pToolWindow_->Enable(enable);
   }
}
/** Metodo temporal. Devuelve el admin de la aplicacion que posee el widget**/
DataViewManager* NavigationWidget::GetDataViewManager() {
   return pDataViewManager_;
}
}

