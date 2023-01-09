/*! \file UniversalCoordinateInputWidget.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>
#include <vector>

// Includes suri
#include "UniversalCoordinateInputPart.h"
#include "PixelLineInputPart.h"
#include "DecimalCoordinateInputPart.h"
#include "LatitudeLongitudeInputPart.h"
#include "suri/LatitudeLongitudeInputWidget.h"
#include "LibraryItemEditorNotificator.h"
#include "SpatialReference.h"
#include "suri/PixelLineInputWidget.h"
#include "suri/DecimalInputWidget.h"
#include "suri/World.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/DataViewManager.h"
#include "suri/LibraryManagerFactory.h"

// Includes wx
#include "wx/choice.h"

// defines
#define ID_RS_SELECTOR_PANEL "ID_RS_SELECTOR_PANEL"
#define ID_COORD_INPUT_PANEL "ID_COORD_INPUT_PANEL"
#define ID_UNIT_SYSTEM_CHOICE "ID_UNIT_SYSTEM_CHOICE"

/** namespace suri */
namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ChoiceSelectionEvent, UniversalCoordinateInputPart)
   IMPLEMENT_EVENT_CALLBACK(OnChoiceSelectionEventHandler, OnChoiceSelectionEventHandler(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

const std::string UniversalCoordinateInputPart::UNIVERSAL_COORDINATE_XRC_NAME = wxT(
      "ID_UNIVERSAL_COORDINATES_PANEL");
const std::string UniversalCoordinateInputPart::PIXEL_LINE_SYSTEM_UNIT = _(
      "Modelo Raster");
const std::string UniversalCoordinateInputPart::DECIMAL_SYSTEM_UNIT = _("Grados decimales");
const std::string UniversalCoordinateInputPart::FLATS_COORDS_SYSTEM_UNIT = _("Coordenadas planas");
const std::string UniversalCoordinateInputPart::SEXAGESIMAL_SYSTEM_UNIT = _(
      "Grados sexagesimal");

/** Ctor */
   UniversalCoordinateInputPart::UniversalCoordinateInputPart(LibraryManager *pManager,
                                                              int flags, const UniversalGraphicalComponentPartInterface::
                                                              FeatureCombinationEnum& Feature) :
      LibraryItemEditorPart(SuriObject::NullUuid, pManager,
                            UNIVERSAL_COORDINATE_XRC_NAME, ID_RS_SELECTOR_PANEL, true,
                            Feature),
      pLibrary_(NULL), pCoordinateTransform_(NULL), pOutputCoordinateTransform_(NULL),
      NEW_EVENT_OBJECT(ChoiceSelectionEvent),
      pNavagationInputWidget_(NULL) {
      representationflags_ = flags;
      pDataViewManager_ = NULL;

      pLibrary_ =
         pManager ? pManager->GetLibraryByCode(
                          LibraryManagerFactory::SpatialReferenceLibraryCode,
                          LibraryManager::ALL) :
         NULL;
      if (pLibrary_) {
         libraryId_ = pLibrary_->GetId();
         pCgu_->SetLibrary(libraryId_);
      }
   }

/** Dtor */
UniversalCoordinateInputPart::~UniversalCoordinateInputPart() {
   DELETE_EVENT_OBJECT
   ;
}

#ifdef __UNUSED_CODE__
/** Permite a clases que hereden modificar el widget luego de crear la ventana */
bool UniversalCoordinateInputPart::ConfigureWidget() {
   PartCollection::ConfigureWidget();
   return true;
}
#endif

/**  Metodo que obtiene a partir de los datos del editor, un vector con los atributos
 *   Este metodo es el que hace el comportamiento particular de cada editor a la hora
 *   de obtener la informacion que posee y transmitirla al sistema de bibliotecas.
 *   Es utilizado a modo de Template Method (en CommitChanges y en el notificador)
 *   @return vector con atributos
 *   @return NULL en caso de que la informnacion del part no sea valida
 *   **/
std::vector<LibraryItemAttribute*>* UniversalCoordinateInputPart::CreateAttributesFromFields() {
   return NULL;
}

/**
 *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
 *  Este metodo es el que realmente tienen que implementar las clases hijas para
 * lograr la especializacion a la hora de iniciar el part (metodo que se llama
 * en CreateToolWindow, a modo de Template Method).
 **/
bool UniversalCoordinateInputPart::ConfigureFeatures() {
   std::string authorityid = SpatialReference::GetAuthorityId(outputSpatialReference_);
   pNotificator_->NotifyItemSelectionFromEditor(authorityid);
   ConfigureChoice();
   InitializeInputWidgets();
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         ID_UNIT_SYSTEM_CHOICE, wxChoice)->Connect(
            wxEVT_COMMAND_CHOICE_SELECTED,
            wxCommandEventHandler(ChoiceSelectionEvent::OnChoiceSelectionEventHandler),
            NULL, pEventHandler_);
   UpdateCoordinatesInputWidget(true);
   return true;
}

/** Evento que se ejecuta al seleccionar un item en el combobox cambiando el
 * tooltip asociado **/
void UniversalCoordinateInputPart::OnChoiceSelectionEventHandler(
      wxCommandEvent &Event) {
   wxChoice* pchoice = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT(ID_UNIT_SYSTEM_CHOICE), wxChoice);
   if (pchoice) {
      std::string systemunit = pchoice->GetStringSelection().c_str();
      if (systemunit.compare(systemUnitSelected_) != 0) {
         systemUnitSelected_ = systemunit;
         UpdateCoordinatesInputWidget(true);
      }
      systemUnitSelected_ = systemunit;
   }
}

/**
 * Configura el choice de acuerdo al SR correspondiente
 * segun el SR elegido por el usuario:
 * SR proyectado -> Decimal
 * SR Georeferenciado -> Decimal/Sexagecimal
 * SR "Raster" -> Pixel,Linea
 * @return true en caso de poder configurar correctamente
 * @return false en caso contrario
 */
bool UniversalCoordinateInputPart::ConfigureChoice() {
   wxChoice* pchoice = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT(ID_UNIT_SYSTEM_CHOICE), wxChoice);
   if (!pchoice)
      return false;
   int position = -1;
   pchoice->Clear();
   if (SpatialReference::IsPixelLineSpatialRef(outputSpatialReference_)) {
      position = pchoice->Append(PIXEL_LINE_SYSTEM_UNIT.c_str());
      systemUnitSelected_ = PIXEL_LINE_SYSTEM_UNIT;
   } else if (SpatialReference::IsProjected(outputSpatialReference_)) {
      pchoice->Append(FLATS_COORDS_SYSTEM_UNIT.c_str());
      systemUnitSelected_ = FLATS_COORDS_SYSTEM_UNIT;
   } else if (SpatialReference::IsGeographic(outputSpatialReference_)) {
      pchoice->Append(DECIMAL_SYSTEM_UNIT.c_str());
      position = pchoice->Append(SEXAGESIMAL_SYSTEM_UNIT.c_str());
      systemUnitSelected_ = SEXAGESIMAL_SYSTEM_UNIT;
   }
   if (position >= -1)
      pchoice->SetSelection(position);
   return true;
}

/**
 * Configura el NavigationInputWidget correspondiente al SRout elegido.
 * @return
 */
bool UniversalCoordinateInputPart::UpdateNavigationInputWidget() {
   Coordinates coordinates;

   bool returnvalue = (pNavagationInputWidget_
         && pNavagationInputWidget_->GetCoordinate(coordinates));

   /**Para pasar de Lat-Long a Decimal tengo que saber que navinputwidget tenia antes **/
//   if (SpatialReference::IsPixelLineSpatialRef(outputSpatialReference_)) {
//      pNavagationInputWidget_ = inputWidgets_[PIXEL_LINE_SYSTEM_UNIT];
//   } else if (SpatialReference::IsProjected(outputSpatialReference_)) {
//      pNavagationInputWidget_ = inputWidgets_[systemUnitSelected_];
//   } else if (SpatialReference::IsGeographic(outputSpatialReference_)) {
//      pNavagationInputWidget_ = inputWidgets_[systemUnitSelected_];
//   } else {
//      return false;
//   }

   pNavagationInputWidget_ = inputWidgets_[systemUnitSelected_];

   if (returnvalue && pOutputCoordinateTransform_
         && pOutputCoordinateTransform_->Transform(coordinates)) {
      returnvalue = pNavagationInputWidget_->SetCoordinate(coordinates);
   }
   return returnvalue;
}

/**
 * Inicializa los widgets para toma de datos.
 */
void UniversalCoordinateInputPart::InitializeInputWidgets() {
   if (pToolWindow_ != NULL) {
      PixelLineInputWidget* pplw = new PixelLineInputWidget();
      pplw->SetDataViewManager(GetDataViewManager());

      NavigationInputWidget* ppixelline = pplw;
      NavigationInputWidget* pdecimal = new DecimalInputWidget();
      NavigationInputWidget* platlong = new LatitudeLongitudeInputWidget();

      inputWidgets_[PIXEL_LINE_SYSTEM_UNIT] = ppixelline;
      inputWidgets_[DECIMAL_SYSTEM_UNIT] = pdecimal;
      inputWidgets_[SEXAGESIMAL_SYSTEM_UNIT] = platlong;

      AddControl(ppixelline, ID_COORD_INPUT_PANEL);
      AddControl(pdecimal, ID_COORD_INPUT_PANEL);
      AddControl(platlong, ID_COORD_INPUT_PANEL);

      ppixelline->Hide();
      pdecimal->Hide();
      platlong->Hide();
   }
}

/**
 * Metodo auxiliar privado que es necesario implementar en las clases hijas.
 * Este metodo es el encargado de actualizar realmente la informacion en el part
 * Es llamado en el metodo SetActiveItem a modo de Template Method
 **/
void UniversalCoordinateInputPart::UpdateFields(const LibraryItem* pItem,
                                                bool Modifiable) {
   if (pItem) {
      const LibraryItemAttribute* pattr = pItem->GetAttribute("WKT");
      // veo que sea un SR diferente
      if (pattr && pattr->GetValue().compare(outputSpatialReference_) != 0) {
         delete pOutputCoordinateTransform_;
         ParameterCollection params;
         params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                      outputSpatialReference_);
         params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                      pattr->GetValue());
         TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
         pOutputCoordinateTransform_ = pfactory->Create(TransformationFactory::kExact,
                                                        params);
         TransformationFactoryBuilder::Release(pfactory);
         outputSpatialReference_ = pattr->GetValue();
         UpdateCoordinatesTransform();
         if (pToolWindow_) {
            ConfigureChoice();
            InitializeInputWidgets();
            UpdateCoordinatesInputWidget(true);
         }
      }
      // invalidar working (coordenadas)
   }
}

/**
 * Metodo auxiliar privado que agrega comportamiento adicional al commit de la
 * clase base. Este comportamiento adicional es necesario cuando
 * el commit involucra objetos adicionales a los items de biblioteca
 * del editor (ej. reproyeccion, en donde se necesita actualizar los params
 * de reproyeccion ademas de leer el item de biblioteca)
 * @return true en caso de realizar correctamente el commit "interno"
 * @return false en caso de que ocurra algun error durante el commit
 */
bool UniversalCoordinateInputPart::OnCommitChanges() {
   return true;
}
/**
 * Metodo auxiliar privado que agrega comportamiento adicional al rollback de la
 * clase base. Este comportamiento adicional es necesario cuando
 * el rollback involucra objetos adicionales a los items de biblioteca
 * del editor (ej. parts compuesto en donde se necesita llamar a los rollbacks de
 * los mismos)
 * @return true en caso de realizar correctamente el rollback "interno"
 * @return false en caso de que ocurra algun error durante el rollback
 */
bool UniversalCoordinateInputPart::OnRollback() {
   return true;
}

/** Metodo auxiliar que actualiza el widget correspondiente a el ingreso de coordenadas
 *  que corresponda*/
bool UniversalCoordinateInputPart::UpdateCoordinatesInputWidget(bool ReplaceWidget) {
   NavigationInputWidget* plastnavwidget = pNavagationInputWidget_;
   UpdateNavigationInputWidget();
   if (ReplaceWidget && pToolWindow_) {
      if (plastnavwidget)
         plastnavwidget->Hide();

      if (pNavagationInputWidget_)
         pNavagationInputWidget_->Show();
   } else if (ReplaceWidget && !pToolWindow_) {
      return false;
   }
   return true;
}

/** retorna la coordenada que ingresaron, informa si es valida */
bool UniversalCoordinateInputPart::GetCoordinate(Coordinates &Coordinate) {
   if (!pNavagationInputWidget_)
      return false;
   UpdateCoordinatesTransform();
   Coordinates widgetcoordinates;
   pNavagationInputWidget_->GetCoordinate(widgetcoordinates);
   Coordinate = widgetcoordinates;
   // Si sigue siendo NULL o no puede transformar la coordenaeda
   if (!pCoordinateTransform_
         || !pCoordinateTransform_->Transform(widgetcoordinates))
      return false;
   return true;
}

/**
 *  Metodo auxiliar que actualiza la transformacion de coordenadas
 */
void UniversalCoordinateInputPart::UpdateCoordinatesTransform() {
   // si no hay transformacion o cambio el SRout
   if (pCoordinateTransform_)
      delete pCoordinateTransform_;
      std::string srout =
            (outputSpatialReference_.empty()) ? workingSpatialReference_ :
                                                outputSpatialReference_;
      ParameterCollection params;
      std::string srin = workingSpatialReference_.empty() ?
                                       outputSpatialReference_ : workingSpatialReference_;

      params.AddValue<std::string>(TransformationFactory::kParamWktIn, srin);
      params.AddValue<std::string>(TransformationFactory::kParamWktOut, srout);
      TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
      pCoordinateTransform_ = pfactory->Create(TransformationFactory::kExact, params);
      TransformationFactoryBuilder::Release(pfactory);
}

/** retorna la coordenada que ingresaron, informa si es valida */
bool UniversalCoordinateInputPart::SetCoordinate(Coordinates Coordinate) {
   UpdateCoordinatesTransform();
   // Si sigue siendo NULL o no puede transformar la coordenaeda
   if (!pCoordinateTransform_
         || !pCoordinateTransform_->Transform(Coordinate, true))
      return false;
   return
         (pNavagationInputWidget_) ? pNavagationInputWidget_->SetCoordinate(
                                           Coordinate) :
                                     false;
}

/** retorna la coordenada que ingresaron, informa si es valida */
bool UniversalCoordinateInputPart::GetSubset(Subset& OutputSubset) {
   return false;
}
/** retorna la coordenada que ingresaron, informa si es valida */
bool UniversalCoordinateInputPart::SetSubset(Subset& NewSubset) {
   return false;
}

/** Devuelve el sistema de referencia de trabajo */
std::string UniversalCoordinateInputPart::GetWorkingSpatialReference() const {
   return workingSpatialReference_;
}

/** Indica cual el sistema de referencia de trabajo */
bool UniversalCoordinateInputPart::SetWorkingSpatialReference(
      const std::string& WorkingSpatialReference) {
   workingSpatialReference_ = WorkingSpatialReference;
   if (outputSpatialReference_.empty()) {
      outputSpatialReference_ = workingSpatialReference_;
      delete pOutputCoordinateTransform_;
      ParameterCollection params;
      params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                   outputSpatialReference_);
      params.AddValue<std::string>(TransformationFactory::kParamWktOut, outputSpatialReference_);
      TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
      pCoordinateTransform_ = pfactory->Create(TransformationFactory::kExact, params);
      TransformationFactoryBuilder::Release(pfactory);
   }
   delete pCoordinateTransform_;
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                workingSpatialReference_);
   params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                outputSpatialReference_);
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   pCoordinateTransform_ = pfactory->Create(TransformationFactory::kExact, params);
   TransformationFactoryBuilder::Release(pfactory);
   if (pCoordinateTransform_ != NULL) {
      std::string id = SpatialReference::GetAuthorityId(workingSpatialReference_);
      pCgu_->UpdateCatalogList();
      pCgu_->SelectItemByPrincipal(id);
      return true;
   }
   return false;
}

/** Devuelve el sistema de referencia de salida */
std::string UniversalCoordinateInputPart::GetOutputSpatialReference() const {
   return outputSpatialReference_;
}

/** Indica cual el sistema de referencia de salida */
bool UniversalCoordinateInputPart::SetOutputSpatialReference(
      const std::string& OutputSpatialReference) {
   delete pOutputCoordinateTransform_;
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                outputSpatialReference_);
   params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                OutputSpatialReference);
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   pOutputCoordinateTransform_ = pfactory->Create(TransformationFactory::kExact, params);
   outputSpatialReference_ = OutputSpatialReference;
   UpdateCoordinatesTransform();
#ifdef __UNUSED_CODE__
   // La llamada a este metodo genera una ventana top level nueva
   ConfigureChoice();
#endif
   UpdateCoordinatesInputWidget(true);

   std::string authorityid = SpatialReference::GetAuthorityId(outputSpatialReference_);
   pNotificator_->NotifyItemSelectionFromEditor(authorityid);
   return (pCoordinateTransform_ != NULL);
}
}
