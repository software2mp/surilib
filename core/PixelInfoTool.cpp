/*! \file PixelInfoTool.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

// Includes Suri
#include "suri/PixelInfoTool.h"
#include "suri/ViewerWidget.h"
#include "suri/World.h"
#include "suri/messages.h"
#include "suri/DataTypes.h"
#include "suri/RasterSpatialModel.h"
#include "suri/RasterElement.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Dimension.h"
#include "MemoryCanvas.h"
#include "CacheRenderer.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/TerrainElement.h"
#include "Terrain.h"
#include "suri/Viewer3D.h"
#include "TerrainGLCanvas.h"
#include "suri/Configuration.h"

// Includes Wx
#include "wx/wx.h"

// Includes App

// Defines

// Forwards

/** namespace suri */
namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(PixelInfoToolEvent, PixelInfoTool)
      IMPLEMENT_EVENT_CALLBACK(OnMouse, OnMouse(event), wxMouseEvent)
      //IMPLEMENT_EVENT_CALLBACK(OnIdle, OnIdle(), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnMouseEnter, OnMouseEnter(), wxMouseEvent)
      IMPLEMENT_EVENT_CALLBACK(OnMouseLeave, OnMouseLeave(), wxMouseEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Constructor.
 * @param[in]	pWorld: puntero a la ventana del mundo.
 */
PixelInfoTool::PixelInfoTool(const World *pWorld) :
      pViewerFrame_(NULL), pViewer_(NULL), pEventHandler_(new PixelInfoToolEvent(this)),
      pWorld_(pWorld), pCT_(NULL), pPixelPosT_(NULL), onIdleConnected_(false),
      pListViewer_(this, &PixelInfoTool::ListUpdate), pRawRasterData_(NULL), pRasterElement_(NULL) {
   toolName_ = wxT("Informacion de Pixel");
   SetActive(true);
   active_ = true;
}

/**
 * Destructor.
 */
PixelInfoTool::~PixelInfoTool() {
   delete pCT_;
   delete pPixelPosT_;
   delete pEventHandler_;
}

/** Utilizo ventana en modo debug para poder tener una interfaz de control */
#ifdef __DEBUG__
/** Retorna la ventana asociada a la herramienta */
/**
 * @return Puntero a la ventana asociada a la herramienta o NULL en caso de
 *       de no tener ventana asociada.
 */
wxWindow *PixelInfoTool::GetToolWindow() const {
   return NULL;
}
#endif

/**
 * Metodo virtual para indicar que la herramienta se ha des/activado
 * @param[in]  State estado de activacion de la herramienta (no se tiene en
 *             cuenta, siempre asigna true a la propiedad active_)
 * @return retorna el valor State ya que si tratan de desactivarla
 *         la herramienta no lo permite.
 */
bool PixelInfoTool::DoSetActive(bool State) {
   active_ = true;
   // retorna false para que siempre este activo
   return false;
}

/**
 * Asigna el visualizador del contexto asociado a la herramienta
 * @param[in]	pViewer: visualizador del contexto asociado.
 */
void PixelInfoTool::SetViewer(ViewerWidget *pViewer) {
   if (pViewer) {
      // Debiera hacerse de una manera mas elegante sin casteo
      pViewerFrame_ = dynamic_cast<wxFrame*>(pViewer->GetFrame());
      assert(pViewerFrame_);
      pViewer_ = pViewer;
      pViewer_->Attach(this);
      wxWindow *pviewerwindow = pViewer->GetWindow();
      // Forma no estandar de conectarme a los eventos
      pviewerwindow->Connect(wxEVT_ENTER_WINDOW,
                             wxMouseEventHandler(PixelInfoToolEvent::OnMouseEnter),
                             NULL, pEventHandler_);
      pviewerwindow->Connect(wxEVT_LEAVE_WINDOW,
                             wxMouseEventHandler(PixelInfoToolEvent::OnMouseLeave),
                             NULL, pEventHandler_);
      pviewerwindow->Connect(wxEVT_MOTION,
                             wxMouseEventHandler(PixelInfoToolEvent::OnMouse), NULL,
                             pEventHandler_);
      // registro el visualizador para poder cambiar el elemento raster
      LayerList *plist = pViewer->GetList();
      if (plist) {
         plist->RegisterViewer(&pListViewer_);
      }
   }
}

/**
 * Evento de entrada de mouse a la ventana.
 * Conecta el evento OnIdle al visualizador.
 */
void PixelInfoTool::OnMouseEnter() {
   if ((active_) && (!onIdleConnected_)) {
      onIdleConnected_ = true;
   }
}

/**
 * Evento de salida de mouse de la ventana.
 * Desconecta el evento OnIdle al visualizador.
 * Limpia el campo del statusbar donde se muestran las coordenadas de la imagen
 */
void PixelInfoTool::OnMouseLeave() {
   if ((active_) && (onIdleConnected_)) {
      wxStatusBar *psbar = pViewerFrame_ ? pViewerFrame_->GetStatusBar() : NULL;
      if (psbar) {
         psbar->SetStatusText(wxT(""), psbar->GetFieldsCount() > 1 ? 1 : 0);
      }
      onIdleConnected_ = false;
   }
}

/**
 * Evento de mouse para actualizar los datos de posicion del puntero
 * Almacena las coordenadas de pantalla del mouse.
 * @param[in]	Event: informacion del evento.
 */
void PixelInfoTool::OnMouse(wxMouseEvent &Event) {
   viewportX_ = Event.GetX();
   viewportY_ = Event.GetY();
   // Evita que en linux se deje de actualizar el PixelInfoTool cuando se pierde evento onEnter.
   if (!active_ || !pWorld_) {
      return;
   }
   wxStatusBar *psbar = pViewerFrame_ ? pViewerFrame_->GetStatusBar() : NULL;
   Coordinates coords(viewportX_, viewportY_);
   CoordinatesTransformation* pct = pViewer_->GetTransformation();
   // convierte coordenadas de pantalla a coordenadas de mundo
   if (pct) {
      pct->Transform(coords);
   }
//   Coordinates worldcoords;
//   pWorld_->Transform(coords, worldcoords);
   Subset world;
   pWorld_->GetWorld(world);
   wxString str;
   if (!world.IsInside(coords)) {
      str = _(message_CURSOR_OUTSIDE_VALID_AREA);
   } else {
      str += GetCoordinatesString(coords);
      // Todo: Mejorar esta condicion para discriminar entre aplicativos.
      if( Configuration::GetParameter("app_short_name", "") == wxT("SoPI") ) {
         str += _(label_DASH_SEPARATOR) + GetDigitalNumberString();
#ifdef __SHOW_RGB_VALUE__
      str += _(label_DASH_SEPARATOR)+GetPixelRgbString();
#endif   // __SHOW_RGB_VALUE__
      str += _(label_DASH_SEPARATOR) + GetPixelPositionString();
      }
   }
   if (psbar) {
      psbar->SetStatusText(str, psbar->GetFieldsCount() > 1 ? 1 : 0);
   } else {
      // Si no encontro un statusbar, que use el que haya
      SHOW_STATUS(str);
   }
}

/** Metodo de actualizacion de la lista */
/**
 * Carga el cache del elemento raster activo.
 * @param pModel
 */
void PixelInfoTool::ListUpdate(Model *pModel) {
   LayerList *plist = pViewer_->GetList();
   if (!Model::IsValid(pModel) || pModel != plist || !plist) {
      return;
   }

   // Encuentra el primer raster activo
   pRasterElement_ = plist->GetActiveElement(0, 6);
   CacheRenderer::CachedElementsMap cache = CacheRenderer::GetCacheForElement(
         pRasterElement_);
   CacheRenderer::CachedElementsMap::iterator it = cache.begin();
   // toma el primer cache
   if (it != cache.end()) {
      pRawRasterData_ = it->second;
   } else {
      pRawRasterData_ = NULL;
   }
}

/**
 * Genera un string con las coordenadas del cursor en Latitud y Longitud
 * @param Position Posicion en coordenadas de Viewport para las cuales se desea
 *        el string expresado en LatLot
 * @return String con las coordenadas.
 */
wxString PixelInfoTool::GetCoordinatesString(const Coordinates &Position) {
   wxString str;
   Coordinates coords(Position), origcoords(Position);
   UpdateTransformation();
   if (pCT_) {
      // Coordenadas geograficas en formato grado sexagesimal.
      pCT_->Transform(coords);

      // Latitud.
      int degreelat = 0, minuteslat = 0, signlat = 1;
      double secondslat = 0;
      CONVERT_DEGREES(coords.y_, degreelat, minuteslat, secondslat, signlat);

      // Longitud.
      int degreelon = 0, minuteslon = 0, signlon = 1;
      double secondslon = 0;
      CONVERT_DEGREES(coords.x_, degreelon, minuteslon, secondslon, signlon);

      str = wxString::Format(
            wxString("(") + wxString(_(format_DEGREE_MINUTES_SECONDS)) + " ; "
                  + wxString(_(format_DEGREE_MINUTES_SECONDS)) + ")",
            degreelat, minuteslat, secondslat,
            (signlat > 0 ? _(label_NORTH_SHORT) : _(label_SOUTH_SHORT)), degreelon,
            minuteslon, secondslon,
            (signlon > 0 ? _(label_EAST_SHORT) : _(label_WEST_SHORT)));
   }

   if (Configuration::GetParameter("app_short_name", "") == wxT("SoPI")) {

      if (!SpatialReference::IsProjected(pWorld_->GetSpatialReference())) {
         // Coordenadas geograficas en formato decimal.
         origcoords = coords;
         str += wxString::Format(
               wxT(label_DASH_SEPARATOR "(%.2f %s; %.2f %s)"), fabs(origcoords.y_),
               origcoords.y_ >= 0 ? _(label_NORTH_SHORT) : _(label_SOUTH_SHORT),
               fabs(origcoords.x_),
               origcoords.x_ >= 0 ? _(label_EAST_SHORT) : _(label_WEST_SHORT));
      } else {
         // Con las coordenadas planas (SR proyectado) se utiliza N y E.
         if (str.length() > 0)
            str += label_DASH_SEPARATOR;
         str += wxString::Format(wxT("(N: %.2f; E: %.2f)"), origcoords.y_,
                                 origcoords.x_);
      }
   }
   return str;
}

/**
 * Genera un string con el valor del DN (Digital Number) del pixel debajo del
 * cursor.
 * @return String con la representacion del valor del DN de la imagen.
 */
wxString PixelInfoTool::GetDigitalNumberString() {
   wxString str;
   dnInfo_.SetLine(viewportX_);
   dnInfo_.SetPixel(viewportY_);
   dnInfo_.SetWorld(const_cast<World*>(pWorld_));
   dnInfo_.SetCanvas(pRawRasterData_);
   dnInfo_.SetElement(pRasterElement_);
   std::vector < std::string > dns = dnInfo_.GetBandsDnInfo();
   bool hasNoDataValue = false;
   double noDataValue = 0;
   if (pRasterElement_){
      pRasterElement_->GetNoDataValue(hasNoDataValue, noDataValue);
   }
   std::string noDataValueStr = NumberToString<double>(noDataValue);
   if (!dns.empty()) {
      for (int ix = 0, lenix = dns.size(); ix < lenix; ++ix) {
         if (str.Len() > 0) str.Append(";");
            if (noDataValueStr == dns[ix]){
               str.append(message_INVALID_DN);
            }else{
               str.append(dns[ix].c_str());
            }
      }
      str = wxT("DN:(") + str + wxString(wxT(" "))
            + _(pRawRasterData_->GetDataType().c_str()) + wxT(")");
   }

   return str;
}

/**
 * Genera un string con la posicion en Pixel y Linea del cursor.
 * @return String con coordenadas X-Y
 */
wxString PixelInfoTool::GetPixelPositionString() {
   wxString str;
   // Muestra los datos de imagen siempre que haya un elemento
   if (pRasterElement_) {
      UpdatePixelTransformation();
      bool haszaxis = false;
      if (pPixelPosT_) {
         // TODO(Gabriel - TCK #3249): Implementacion temporal para obtener
         // el modelo raster con el terreno activo en 3D
         RasterElement* prasterelement = dynamic_cast<RasterElement*>(pRasterElement_);
         RasterSpatialModel *prastermodel = NULL;
         TerrainElement* pterrain = dynamic_cast<TerrainElement*>(pRasterElement_);
         if (prasterelement) {
            prastermodel = RasterSpatialModel::Create(prasterelement->GetRasterModel());
         } else if (!prasterelement && pterrain) {
            RasterElement* ptemp = RasterElement::Create(pterrain->GetUrl().c_str());
            prastermodel = RasterSpatialModel::Create(ptemp->GetRasterModel());
            haszaxis = true;
            delete ptemp;
         }
         if (!prastermodel) {
            return str;
         }

         Coordinates pos, ul;
         pWorld_->Transform(Coordinates(viewportX_, viewportY_), pos);
         pPixelPosT_->Transform(pos, true);
         prastermodel->InverseTransform(pos);
         RasterSpatialModel::Destroy(prastermodel);
         if (!haszaxis) {
            str = wxString::Format(_(format_PIXEL_LINE_ff), pos.x_, pos.y_);
         } else {
#ifdef __DEBUG__
            suri::Viewer3D* pv3d = dynamic_cast<Viewer3D*>(this->pViewer_);
            if (pv3d) {
               TerrainGLCanvas* pterrain = pv3d->GetCanvas();
               pos = pterrain->GetPosition(viewportX_, viewportY_);
            }
            str = wxString::Format(_(format_PIXEL_LINE_fff), pos.x_, pos.y_, pos.z_);
#endif
         }

      }
   }
   return str;
}

/** Actualiza la funcion de transformacion para coordenadas pixel **/
void PixelInfoTool::UpdatePixelTransformation() {
   if (pPixelPosT_) {
        ExactCoordinatesTransformation* ptransform =
              dynamic_cast<ExactCoordinatesTransformation*>(pPixelPosT_);
      std::string wktin =
            pRasterElement_ ? pRasterElement_->GetSpatialReference().c_str() :
                              pWorld_->GetSpatialReference();
        std::string wktout = pWorld_->GetSpatialReference();
        // Si no es exacta o los sr son diferentes se elimina para regenerase
        if (!ptransform
              || (ptransform
                    && (ptransform->GetWktIn().compare(wktin)
                          || ptransform->GetWktOut().compare(wktout)))) {
           delete pPixelPosT_;
           pPixelPosT_ = NULL;
        }
     }
   if (!pPixelPosT_) {
      ParameterCollection params;
      params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                   pRasterElement_->GetSpatialReference().c_str());
      params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                   pWorld_->GetSpatialReference());
      TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
      pPixelPosT_ = pfactory->Create(TransformationFactory::kExact, params);
      TransformationFactoryBuilder::Release(pfactory);
   }
}

/** Metodo auxiliar para actualizar la transformacion para obtener las coordenadas**/
void PixelInfoTool::UpdateTransformation() {
   std::string wktin = pWorld_->GetSpatialReference();
   std::string wktout = !SpatialReference::IsPixelLineSpatialRef(wktin)? "" : wktin;
   if (wktin.empty() && wktout.empty())
      return;
   if (pCT_) {
      ExactCoordinatesTransformation* ptransform =
            dynamic_cast<ExactCoordinatesTransformation*>(pCT_);

      // Si no es exacta o los sr son diferentes se elimina para regenerase
      if (!ptransform
            || (ptransform
                  && (ptransform->GetWktIn().compare(wktin)))) {
         delete pCT_;
         pCT_ = NULL;
      }
   }

   if (!pCT_) {
      // TODO(Gabriel - TCK #3285): Ver si vale la pena rehacer todo el tiempo el paramcollection
      ParameterCollection params;
      params.AddValue<std::string>(TransformationFactory::kParamWktIn, wktin);
      params.AddValue<std::string>(TransformationFactory::kParamWktOut, wktout);
      TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
      pCT_ = pfactory->Create(TransformationFactory::kExact, params);
      TransformationFactoryBuilder::Release(pfactory);
   }
}


/** Metodo que desactiva la consulta de coordenadas **/
void PixelInfoTool::DisableCoordinatesInfo() {
   if (!active_)
      return;
   active_ = false;
}
/** Metodo que activa la consulta de coordenadas */
void PixelInfoTool::EnableCoordinatesInfo() {
   if (active_)
      return;
   active_ = true;
}

#ifdef __SHOW_RGB_VALUE__
/**
 * Genera un string con los valores RGB de la coordenada debajo del puntero.
 * @return String con el valor RGB.
 */
wxString PixelInfoTool::GetPixelRgbString() {
   wxString str = wxT("RGV: ");
   str += _(message_NA);
   return str;
}
#endif   // __SHOW_RGB_VALUE__
}  /** namespace suri **/
