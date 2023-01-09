/*! \file SpatialSelectionPart.cpp */
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
#include "SpatialReference.h"
#include "SpatialSelectionPart.h"
#include "suri/messages.h"
#include "resources.h"
#include "suri/World.h"
#include "logmacros.h"
#include "suri/LatitudeLongitudeInputWidget.h"
#include "suri/Dimension.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/DecimalInputWidget.h"
#include "suri/PixelLineInputWidget.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"

// forward

// Defines
/**
 * \see RasterOperationButton.cpp
 * \todo(13/01/2010 - Alejandro): deberia crearse un overload del metodo
 * CoordinateTransform::TransformCoordinates que tenga el parametro
 * CalculationPoints con un valor por defecto.
 */

/** namespace suri */
namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */START_IMPLEMENT_EVENT_OBJECT(SpatialSelectionPartEvent, SpatialSelectionPart)IMPLEMENT_EVENT_CALLBACK(OnImageButton, OnImageButton(event), wxCommandEvent)
IMPLEMENT_EVENT_CALLBACK(OnWindowButton, OnWindowButton(event), wxCommandEvent)
IMPLEMENT_EVENT_CALLBACK(OnUiUpdate, OnUiUpdate(event), wxUpdateUIEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/* Constructor
 * @param[in] pWorld mundo
 * @param[in] DefaultValue coordenadas default.
 * @return instancia de la clase SpatialSelectionPart
 */
SpatialSelectionPart::SpatialSelectionPart(DataViewManager* pDataviewManager,
                                           World* pWorld, const Subset &DefaultValue,
                                           int Flags) :
      Part(false, false), pLrWidget_(NULL), pUlWidget_(NULL), pOriginalWorld_(pWorld),
      appliedLatLonSubset_(DefaultValue), NEW_EVENT_OBJECT(SpatialSelectionPartEvent),
      flags_(Flags), pDataviewManager_(pDataviewManager) {
   pInternalWorld_ = new World(*pWorld);
   defaultValue_ = DefaultValue;
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                pWorld->GetSpatialReference());
   params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                pWorld->GetSpatialReference());
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   pCoordTransform_ = pfactory->Create(TransformationFactory::kExact, params);
   TransformationFactoryBuilder::Release(pfactory);
   // Creo ventana
   windowTitle_ = _(caption_SPATIAL_SELECTION);
}

/** Destructor. */
SpatialSelectionPart::~SpatialSelectionPart() {
   delete pInternalWorld_;
   if (pCoordTransform_)
      delete pCoordTransform_;
}

/* Crea la ventana de la parte
 * @return true en caso de poder crear la ventana
 * @return false en caso de poder crear la ventana
 */
bool SpatialSelectionPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(
         pParentWindow_, wxT("ID_SPATIAL_SUBSET_SELECTION_PART"));
   wxWindow *pimagebutton = XRCCTRL(*pToolWindow_, wxT("ID_IMAGE_BUTTON"), wxButton);
   wxWindow *pwindowbutton = XRCCTRL(*pToolWindow_, wxT("ID_WINDOW_BUTTON"), wxButton);

   if (!pimagebutton || !pwindowbutton) {
      return false;
   }

   // Carga el label y el tooltip del boton que carga las coordenadas de la imagen
   pimagebutton->SetLabel(wxT(_(label_IMAGE_BUTTON)));
   pimagebutton->SetToolTip(_(tooltip_IMAGE_BUTTON));
   // Carga el label y el tooltip del boton que carga las coordenadas de la ventana
   pwindowbutton->SetLabel(_(label_WINDOW_BUTTON));
   pwindowbutton->SetToolTip(_(tooltip_WINDOW_BUTTON));

   /* El boton de las coordenadas de la ventana se habilita solo si hay un
    * subset valido.  */
   pwindowbutton->Enable(defaultValue_.IsValid() && ValidateSubset(defaultValue_));

   CreateCoordinatesInputWidgets(flags_);
   // Cargo navegadores en pantalla
   AddControl(pUlWidget_, wxT("ID_UL_COORDINATES_CONTENT"));
   AddControl(pLrWidget_, wxT("ID_LR_COORDINATES_CONTENT"));
   // Conecto eventos de botones y update
   pimagebutton->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(SpatialSelectionPartEvent::OnImageButton), NULL,
         pEventHandler_);
   pwindowbutton->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(SpatialSelectionPartEvent::OnWindowButton), NULL,
         pEventHandler_);
   pToolWindow_->Connect(wxEVT_UPDATE_UI,
                         wxUpdateUIEventHandler(SpatialSelectionPartEvent::OnUiUpdate),
                         NULL, pEventHandler_);
   SetInitialValues();
   modified_ = false;
   return true;
}

/** Metodo auxiliar que crea los NavigationInputWidget correspondientes
 *  al Sistema de ingreso de coordenadas seleccionado */
bool SpatialSelectionPart::CreateCoordinatesInputWidgets(int Flags) {
   if (Flags & SEXAGESIMAL_SUBSET) {
      // Cargo herramienta para ingresar lat-long en NavigationWidget
      pUlWidget_ = new suri::LatitudeLongitudeInputWidget(false);
      // Cargo herramienta para ingresar lat-long en NavigationWidget
      pLrWidget_ = new suri::LatitudeLongitudeInputWidget(false);
   } else if (Flags & DECIMAL_SUBSET) {
      // Cargo herramienta para ingresar lat-long en NavigationWidget
      pUlWidget_ = new DecimalInputWidget(false);
      // Cargo herramienta para ingresar lat-long en NavigationWidget
      pLrWidget_ = new DecimalInputWidget(false);
   } else if (Flags & PIXEL_LINE_SUBSET) {
      // Cargo herramienta para ingresar lat-long en NavigationWidget
      pUlWidget_ = new suri::PixelLineInputWidget();
      // Cargo herramienta para ingresar lat-long en NavigationWidget
      pLrWidget_ = new suri::PixelLineInputWidget();
   } else {
      return false;
   }
   pUlWidget_->SetDataViewManager(pDataviewManager_);
   pLrWidget_->SetDataViewManager(pDataviewManager_);
   return true;
}
/* Devuelve el estado del Part.
 * @return true si el part esta habilitado
 * @return false si el part no esta habilitado
 */
bool SpatialSelectionPart::IsEnabled() {
   std::string wkt = pInternalWorld_->GetSpatialReference();
   if (!SpatialReference::IsGeographicSpatialRef(wkt)
         && !SpatialReference::IsProjectedSpatialRef(wkt)) {
      return false;
   }
   return true;
}

/* Indica si el Part tiene cambios para salvar.
 * Devuelve el valor de la propiedad modified_
 * @return true si el part esta modificado
 * @return false si el part no esta modificado
 */
bool SpatialSelectionPart::HasChanged() {
   return modified_;
}

/* Salva los cambios realizados en el Part.
 * @return true en caso de poder salvar los cambios
 * @return false en caso de no poder salvar los cambios
 */
bool SpatialSelectionPart::CommitChanges() {
   if (!HasValidData()) {
      SHOW_ERROR(errorMessage_);
      return false;
   }

   if (HasChanged()) {
      Subset newsubset;

      // Obtengo subset en coordenadas de mundo
      if (!GetInternalSubset(newsubset)) {
         return false;
      }

      // Actualizo el subset aplicado en lat long
      if (!GetWidgetSubset(appliedLatLonSubset_)) {
         return false;
      }

      // Actualizo mundo que obtiene el usuario
      pInternalWorld_->SetWindow(newsubset);

      pPal_->AddAttribute<Subset>(ProcessAdaptLayer::SpatialBoundsKeyAttr, newsubset);

      modified_ = false;
   }
   return true;
}

/* Restaura los valores a los ultimos validos.
 * @return devuelve el resultado de invocar a SetInternalSubset(subset)
 */
bool SpatialSelectionPart::RollbackChanges() {
   Subset subset;
   pInternalWorld_->GetWindow(subset);
   return SetInternalSubset(subset);
}

/** Restaura los valores a los del mundo original */
void SpatialSelectionPart::SetInitialValues() {
   Subset subset;
   pOriginalWorld_->GetWorld(subset);
   // Actualizo mundo del usuario
   pInternalWorld_->SetWindow(subset);
   // Actualizo variables internas
   SetInternalSubset(subset);
   // Considero que el subset inicial esta aplicado
   GetWidgetSubset(appliedLatLonSubset_);

   RollbackChanges();
}

/* Devuelve el icono de la parte
 * @param[in] X ancho del icono
 * @param[in] Y alto del icono
 * @param[out] ToolBitmap icono a devolver.
 */
void SpatialSelectionPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_SPATIAL_SELECTION, ToolBitmap);
}

/* Elimina las modificaciones realizadas a la ventana. Invoca a SetInternalSubset
 * con el subset del mundo original.
 * @param[in] Event evento
 */
void SpatialSelectionPart::OnImageButton(wxCommandEvent &Event) {
   Subset subset;
   pOriginalWorld_->GetWorld(subset);
   SetInternalSubset(subset);
}

/* Muestra por pantalla las coordenadas default cargadas en constructor.
 * Invoca a SetInternalSubset con el subset default (defaultValue_)
 * @param[in] Event evento
 */
void SpatialSelectionPart::OnWindowButton(wxCommandEvent &Event) {
   SetInternalSubset(defaultValue_);
}

/**
 * Retorna el mundo que se va a modificar
 * @return mundo con subset ingresado en la ventana
 * \attention el mundo es responsabilidad de esta clase. No eliminar fuera de
 * esta clase
 */
World* SpatialSelectionPart::GetWorld() {
   return pInternalWorld_;
}

/* Informa si el subset seleccionado es valido. Devuelve el resultado de invocar
 * a ValidateSubset.
 * @return true si el subset es valido
 * @return false si el subset no es valido
 */
bool SpatialSelectionPart::HasValidData() {
   Subset newsubset;
   if (!GetInternalSubset(newsubset)) {
      errorMessage_ = message_LATLONG_POSITION_ERROR;
      return false;
   }

   return ValidateSubset(newsubset);
}

/**
 * Verifica que el subset recibido sea valido, que no exista una coordenada del
 * mismo por fuera de las coordenadas de la imagen.
 * @param[in]   SubsetIn subset en coordenadas del mundo.
 * @return true en caso de que el subset sea valido.
 * @return false en caso de que el subset no sea valido.
 */
bool SpatialSelectionPart::ValidateSubset(const Subset &SubsetIn) {
   // Verifico que las coordenadas ingresadas tengan misma direccion que
   // las del mundo original
   Subset world;
   pOriginalWorld_->GetWorld(world);
   bool equalx, equaly;
   CompareSigns(world, SubsetIn, equalx, equaly);
   if (!equalx || !equaly) {
      errorMessage_ = message_SUBSET_ORDER_ERROR;
      return false;
   }

   // Verifico que las coordenadas esten dentro del mundo
   if (!world.IsInside(SubsetIn.ul_) || !world.IsInside(SubsetIn.lr_)) {
      errorMessage_ = message_LATLONG_POSITION_ERROR;
      return false;
   }

   return true;
}

// ------------------------------------------------------------------------------
/**
 * Este metodo responde a los eventos UIUpdate(que se disparan cuando no hay
 * eventos de usuario en la cola de eventos de wx).
 * Setea flag de Part modificado verificando el estado de las textctrl.
 * \todo No deberia usarse un UiUpdate para setear datos, el UiUpdate solo debe
 *  usarse para actualizar la interfaz de usuario!
 * @param[in]   Event: evento
 */
void SpatialSelectionPart::OnUiUpdate(wxUpdateUIEvent &Event) {
   modified_ = false;

   Subset newsubset;
   if (!GetWidgetSubset(newsubset) || newsubset != appliedLatLonSubset_) {
      modified_ = true;
   }

   Event.Skip();
}

/** Obtiene el subset que esta en herramienta grafica */
/**
 * Devuelve subset interno en las coordenadas del mundo. Invoca a GetWidgetSubset
 * para obtener los valores cargados en la herramienta grafica y en caso de que
 * hayan sido modificados, los tranforma a coordenadas del mundo y devuelve
 * dicho subset. Luego de transformar los extremos del subset puede que reordene las
 * coordenadas de mundo para que sean ul y lr.
 * @param[out] SubsetOut subset en coordenadas del mundo.
 * @return true en caso de poder obtener y setear el subset.
 * @return false en caso de no poder obtener o setear el subset.
 */
bool SpatialSelectionPart::GetInternalSubset(Subset &SubsetOut) {
   SubsetOut = currentSRSubset_;
   if (HasChanged()) {
      GetWidgetSubset(SubsetOut);

      // para evitar que vuelva a convertir un subset que ya convirtio.
      if (SubsetOut == currentLatLonSubset_) {
         SubsetOut = currentSRSubset_;
         return true;
      }

      if (pCoordTransform_) {
         currentLatLonSubset_ = SubsetOut;
         pCoordTransform_->Transform(SubsetOut.ul_, true);
         pCoordTransform_->Transform(SubsetOut.lr_, true);

         // Si las coordenadas estan fuera de orden las ordeno
         Subset world;
         pOriginalWorld_->GetWorld(world);
         bool equalx, equaly;
         CompareSigns(world, SubsetOut, equalx, equaly);
         if (!equalx || !equaly) {
            Subset subsettemp = SubsetOut;
            if (!equalx) {
               SubsetOut.lr_.x_ = subsettemp.ul_.x_;
               SubsetOut.ul_.x_ = subsettemp.lr_.x_;
            }
            if (!equaly) {
               SubsetOut.lr_.y_ = subsettemp.ul_.y_;
               SubsetOut.ul_.y_ = subsettemp.lr_.y_;
            }
         }

         currentSRSubset_ = SubsetOut;
      }
   }
   return true;
}

/**
 * Setea subset interno en las coordenadas del mundo. Convierte el subset
 * recibido a coordenadas LatLon e invoca a SetWidgetSubset para setear los
 * valores en la herramienta grafica.
 * @param[in] SubsetIn subset en coordenadas de mundo.
 * @return true
 */
bool SpatialSelectionPart::SetInternalSubset(const Subset &SubsetIn) {
   if (SubsetIn == currentSRSubset_) {
      return true;
   }

   Subset subsetaux = SubsetIn;

   if (pCoordTransform_) {
      pCoordTransform_->Transform(subsetaux.ul_);
      pCoordTransform_->Transform(subsetaux.lr_);
   }

#ifdef __UNUSED_CODE_
   /**
    * Se deja de realzar la comprobacion de si los puntos ul y lr de la imagen
    * estan invertidos, porque esta solucion no funcionaba en todos los casos
    * (por ejemplo en imagenes con coordenadas polares, invertia los puntos
    * cuando no debia).
    * Los valores Esquina superior izquierda y esquina inferior derecha
    * corresponden al los puntos ul y lr de la imagen.
    */

   // Si el sentido de algun eje no coicinde vuelvo a calcular la transformacion
   // utilizando otras esquinas de origen
   bool equalx, equaly;
   Subset latlongsubset(-180, 90, 180, -90);
   CompareSigns(latlongsubset, subsetaux, equalx, equaly);
   if ( !equalx || !equaly ) {
      subsetaux = SubsetIn;
      if ( !equalx ) {
         subsetaux.lr_.x_=SubsetIn.ul_.x_;
         subsetaux.ul_.x_=SubsetIn.lr_.x_;
      }
      if ( !equaly ) {
         subsetaux.lr_.y_=SubsetIn.ul_.y_;
         subsetaux.ul_.y_=SubsetIn.lr_.y_;
      }
      if ( pCoordTransform_ ) {
         pCoordTransform_->TransformCoordinates(subsetaux.ul_);
         pCoordTransform_->TransformCoordinates(subsetaux.lr_);
      }
   }
#endif

   if (SetWidgetSubset(subsetaux)) {
      currentLatLonSubset_ = subsetaux;
      currentSRSubset_ = SubsetIn; // guarda subset en coord. de mundo.
   }

   return true;
}

/**
 * Obtiene los valores seteados en el widget y devuelve un subset en coordenadas
 * LatLon.
 * @param[out] SubsetOut en coordenadas LatLon.
 * @return true en caso de poder obtener los valores de las coordenadas.
 * @return false en caso de no poder obtener los valores de las coordenadas.
 */
bool SpatialSelectionPart::GetWidgetSubset(Subset &SubsetOut) {
   Coordinates ulcoord;
   Coordinates lrcoord;
   if (!pToolWindow_)
      return false;
   if (!pUlWidget_->GetCoordinate(ulcoord) || !pLrWidget_->GetCoordinate(lrcoord))
      return false;

   SubsetOut.lr_ = lrcoord;
   SubsetOut.ul_ = ulcoord;
   return true;
}

/**
 * Recibe subset en coordenadas LatLon y lo setea en el widget.
 * @param[in] SubsetIn subset en coordenadas LatLon.
 * @return true en caso de haber podido setear los valores en los controles.
 * @return false en caso de no haber podido setear los valores en los controles.
 */
bool SpatialSelectionPart::SetWidgetSubset(const Subset &SubsetIn) {
   // seteo coordenadas en controles.
   if (!pToolWindow_)
      return false;
   if (!pLrWidget_->SetCoordinate(SubsetIn.lr_)
         || !pUlWidget_->SetCoordinate(SubsetIn.ul_)) {
      return false;
   }

   return true;
}

/* Compara el sentido de los ejes de dos subsets
 * @param[in] Subset1 subset 1
 * @param[in] Subset2 subset 2
 * @param[out] EqualX true si los signos de los ejes X son iguales. False si son
 * distintos.
 * @param[out] EqualY true si los signos de los ejes Y son iguales. False si son
 * distintos.
 */
void SpatialSelectionPart::CompareSigns(const Subset &Subset1, const Subset &Subset2,
                                        bool &EqualX, bool &EqualY) {
   // Verifico que las coordenadas esten en orden correcto
   Dimension dimension1(Subset1);
   Dimension dimension2(Subset2);
   int x1, y1, x2, y2;
   dimension1.GetSign(x1, y1);
   dimension2.GetSign(x2, y2);
   EqualX = (x1 == x2);
   EqualY = (y1 == y2);
}

/** Retorna las coordenadas de la ventana en sistema window
 *  De esta forma no se esta devolviendo el World directamente**/
void SpatialSelectionPart::GetInputWindow(Subset& window) {
   pInternalWorld_->GetWindow(window);
}
}
