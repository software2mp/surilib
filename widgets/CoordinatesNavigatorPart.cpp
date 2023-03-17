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

// Includes Estandar

// Includes suri
#include "CoordinatesNavigatorPart.h"
#include "suri/LatitudeLongitudeInputWidget.h"
#include "suri/DecimalInputWidget.h"
#include "suri/PixelLineInputWidget.h"
#include "suri/LibraryManager.h"
#include "SpatialReference.h"
#include "suri/LibraryManagerFactory.h"
#include "suri/Configuration.h"

// Includes wx
#include "wx/choicebk.h"

// Defines
#define COORD_POSITION 0
#define RASTER_POSITION 1

#define DECIMAL_ARRAY_POS 0
#define LAT_LONG_ARRAY_POS 1
#define PIXEL_ARRAY_POS 2

#define COORD_PAGE_TEXT "Sistema de coordenadas"
#define RASTER_PAGE_TEXT "Modelo Raster"
namespace suri {
	
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(CoordinatesEventHandler, CoordinatesNavigatorPart)
	IMPLEMENT_EVENT_CALLBACK(OnPageChanged,
								OnPageChanged(event), wxChoicebookEvent)
	IMPLEMENT_EVENT_CALLBACK(OnDecimalRadioSelected,
								OnDecimalRadioSelected(event), wxCommandEvent)
	IMPLEMENT_EVENT_CALLBACK(OnSexagesimalRadioSelected,
								OnSexagesimalRadioSelected(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */


/** ctor **/
CoordinatesNavigatorPart::CoordinatesNavigatorPart(LibraryManager* pManager,
                                                   bool UseLibrary) :
      NEW_EVENT_OBJECT(CoordinatesEventHandler),
      pItemSelector_(NULL),
      useLibrary_(UseLibrary),
      pClient_(pManager ? new LibraryClient(pManager, LibraryManager::READONLY) : NULL),
      currentCoordInput_(0), currentSpatialCoordInput_(0),
      pCoordPage_(NULL), pRasterCoordPage_(NULL) {
   coordsInput_.push_back(new DecimalInputWidget);
   coordsInput_.push_back(new LatitudeLongitudeInputWidget);
   coordsInput_.push_back(new PixelLineInputWidget);
   if (UseLibrary) {
      const Library* plib =
            pClient_->GetLibraryByCode(LibraryManagerFactory::SpatialReferenceLibraryCode);
      pItemSelector_ = new BasicLibraryItemSelector(this,
            plib->GetId(), pManager, "ID_LIBRARY_SELECTION_PANEL",
            "ID_LIBRARY_SELECTION_PANEL", true,
            UniversalGraphicalComponentPartInterface::ExtendedROWithFF);
   }
}	

/** dtor **/
CoordinatesNavigatorPart::~CoordinatesNavigatorPart() {
	DELETE_EVENT_OBJECT;
	if (pClient_)
	   delete pClient_;
}
			
/** Indica si el Part tiene cambios para salvar. */
bool CoordinatesNavigatorPart::HasChanged() {
	return false;
}
/** Salva los cambios realizados en el Part. */
bool CoordinatesNavigatorPart::CommitChanges() {
	return false;
}
/** Restaura los valores originales del Part. */
bool CoordinatesNavigatorPart::RollbackChanges() {
	return false;
}
/** Inicializa el part */
void CoordinatesNavigatorPart::SetInitialValues() {
   coordsInput_[LAT_LONG_ARRAY_POS]->GetInputWidget()->Hide();
   coordsInput_[DECIMAL_ARRAY_POS]->GetInputWidget()->Show();
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
       "ID_DECIMAL_RADIOBUTTON", wxRadioButton)->SetValue(true);
}
/** Se crea la ventana de herramienta en las clases derivadas */
bool CoordinatesNavigatorPart::CreateToolWindow() {
	pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_, 
					wxT("ID_COORDINATES_NAVIGATOR_PANEL"));
	if (!pToolWindow_)
		return false;
	GET_CONTROL(*(this->GetWidget()->GetWindow()),
	 "ID_COORDINATES_BOOK", wxChoicebook)->Connect(
	       wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED,
		wxChoicebookEventHandler(CoordinatesEventHandler::OnPageChanged),
								NULL, pEventHandler_);
	GET_CONTROL(*(this->GetWidget()->GetWindow()),
	 "ID_DECIMAL_RADIOBUTTON", wxRadioButton)->Connect(
		wxEVT_COMMAND_RADIOBUTTON_SELECTED,
		wxCommandEventHandler(CoordinatesEventHandler::OnDecimalRadioSelected),
								NULL, pEventHandler_);   
	GET_CONTROL(*(this->GetWidget()->GetWindow()),
	 "ID_SEXA_RADIOBUTTON", wxRadioButton)->Connect(
		wxEVT_COMMAND_RADIOBUTTON_SELECTED,
		wxCommandEventHandler(CoordinatesEventHandler::OnSexagesimalRadioSelected),
								NULL, pEventHandler_);
	if (pItemSelector_ && useLibrary_) {
	   AddControl(pItemSelector_->GetWidget(), "ID_SR_SELECTOR_PANEL");
	}
	AddControl(coordsInput_[DECIMAL_ARRAY_POS]->GetInputWidget(), "ID_COORDINATES_PANEL");
	AddControl(coordsInput_[LAT_LONG_ARRAY_POS]->GetInputWidget(), "ID_COORDINATES_PANEL");
	coordsInput_[1]->GetInputWidget()->Hide();
	AddControl(coordsInput_[PIXEL_ARRAY_POS]->GetInputWidget(), "ID_RASTER_PANEL");
	return true;
}

/** Evento que se dispara cuando se cambio de pagina del book */
void CoordinatesNavigatorPart::OnPageChanged(wxBookCtrlBaseEvent &Event) {
   if (Event.GetSelection() == RASTER_POSITION) {
      Coordinates coords;
      coordsInput_[currentCoordInput_]->GetCoordinate(coords);
      coordsInput_[PIXEL_ARRAY_POS]->SetCoordinate(coords);
      currentSpatialCoordInput_ = PIXEL_ARRAY_POS;
   } else {
      Coordinates coords;
      coordsInput_[PIXEL_ARRAY_POS]->GetCoordinate(coords);
      coordsInput_[currentCoordInput_]->SetCoordinate(coords);
      currentSpatialCoordInput_ = currentCoordInput_;
   }
}			

/** Metodo que se llama cuando se selecciona el radio
 *  de decimal **/
void CoordinatesNavigatorPart::OnDecimalRadioSelected(wxCommandEvent &Event) {
   ActiveDecimalInput();
   OnDecimalSelected();
}

/** Metodo que se llama cuando se selecciona el radio 
 *  de sexagesimal */
void CoordinatesNavigatorPart::OnSexagesimalRadioSelected(wxCommandEvent &Event) {
   ActiveSexagesimalInput();
   OnSexagesimalSelected();
}

/** retorna la coordenada que ingresaron, informa si es valida */
bool CoordinatesNavigatorPart::SetCoordinate(Coordinates Coordinate) {
   return coordsInput_[currentSpatialCoordInput_]->SetCoordinate(Coordinate);
}

/** retorna la coordenada que ingresaron, informa si es valida */
bool CoordinatesNavigatorPart::GetCoordinate(Coordinates &Coordinate) {
   if (currentSpatialCoordInput_ < 2) {
      // correccion por si se pierde el radio button activo
      bool decimal = XRCCTRL(*(this->GetWidget()->GetWindow()),
             "ID_DECIMAL_RADIOBUTTON", wxRadioButton)->GetValue();
      currentSpatialCoordInput_ = decimal ? DECIMAL_ARRAY_POS : LAT_LONG_ARRAY_POS;
   }
   return coordsInput_[currentSpatialCoordInput_]->GetCoordinate(Coordinate);
}
/** Metodo que se llama cuando se configura el sistema de referencia. Permite
 *  realizar alguna accion luego de configurar el sistema de referencia
 *  de trabajo
 */
void CoordinatesNavigatorPart::OnSpatialReferenceSet() {
   if (pItemSelector_ && useLibrary_) {
      const Library* plib = pClient_->GetLibraryByCode(
            LibraryManagerFactory::SpatialReferenceLibraryCode);
      if (plib) {
         std::string authid = SpatialReference::GetAuthorityId(spatialReference_);
         const LibraryItem* pitem = plib->GetItemByPrincipal(authid);
         if (pitem)
            pItemSelector_->SetActiveItem(pitem);
      }
   }
   UpdateCoordsInput();
}
/** Metodo auxiliar que actualiza el panel de ingreso de coordenadas
 *  en funcion del sistema de referencia
 */
void CoordinatesNavigatorPart::UpdateCoordsInput() {
   if (!pToolWindow_)
      return;
   bool enablesexa = true;
   std::string declabel = "Grados decimales";
   wxChoicebook* pchbook = XRCCTRL(*(this->GetWidget()->GetWindow()),
                        "ID_COORDINATES_BOOK", wxChoicebook);
   if (SpatialReference::IsPixelLineSpatialRef(spatialReference_)) {
      /** Esto es para evitar los casos en los que la navegacion
       * por modelo raster este deshabilitada
       */
      if (pchbook->GetPageCount() > RASTER_POSITION)
         pchbook->SetSelection(RASTER_POSITION);
   } else if (pCoordPage_) {
      pchbook->InsertPage(COORD_POSITION, pCoordPage_, _(COORD_PAGE_TEXT));
      pchbook->SetSelection(COORD_POSITION);
      pCoordPage_ = NULL;
   } else {
      pchbook->SetSelection(COORD_POSITION);
   }

   if (SpatialReference::IsProjectedSpatialRef(spatialReference_)) {
      declabel = "Coordenadas planas";
      enablesexa = false;
      ActiveDecimalInput(true);
   } else {
      ActiveSexagesimalInput();
   }
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
       "ID_DECIMAL_RADIOBUTTON", wxRadioButton)->SetLabel(declabel);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
       "ID_SEXA_RADIOBUTTON", wxRadioButton)->SetValue(enablesexa);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
       "ID_SEXA_RADIOBUTTON", wxRadioButton)->Enable(enablesexa);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
          "ID_DECIMAL_RADIOBUTTON", wxRadioButton)->SetValue(!enablesexa);
   dynamic_cast<DecimalInputWidget*>(coordsInput_[DECIMAL_ARRAY_POS])->SetSpatialReference(
         spatialReference_);
}

void CoordinatesNavigatorPart::SetDataViewManager(DataViewManager* pDataViewManager) {
   CoordinatesNavigationInputInterface::SetDataViewManager(pDataViewManager);
   for (size_t i = 0; i < coordsInput_.size(); ++i)
      coordsInput_.at(i)->SetDataViewManager(pDataViewManager);
}

/** Habilita/deshabilita la navegacion pixel/linea **/
void CoordinatesNavigatorPart::EnablePixelLineNavigation(bool Enable) {
   if (pToolWindow_ && !Enable) {
      wxChoicebook* pchbook = XRCCTRL(*(this->GetWidget()->GetWindow()),
                           "ID_COORDINATES_BOOK", wxChoicebook);

      // Fix para ticket #6284
      if (Configuration::GetParameter("app_short_name", "") != wxT("SoPI")) {
         GET_CONTROL(*(this->GetWidget()->GetWindow()),
               "ID_DECIMAL_RADIOBUTTON", wxRadioButton)->Show(false);
         GET_CONTROL(*(this->GetWidget()->GetWindow()),
               "ID_SEXA_RADIOBUTTON", wxRadioButton)->Show(false);
         return;
      }

      if (pRasterCoordPage_ == NULL) {
         pRasterCoordPage_ = pchbook->GetPage(RASTER_POSITION);
         pchbook->RemovePage(RASTER_POSITION);
         wxWindow* pcoordpage = pchbook->GetPage(COORD_POSITION);
         pchbook->RemovePage(COORD_POSITION);
         pchbook->InsertPage(COORD_POSITION, pcoordpage, _(COORD_PAGE_TEXT));
      }
      pchbook->SetSelection(COORD_POSITION);
   } else if (pToolWindow_ && pRasterCoordPage_) {
      wxChoicebook* pchbook = XRCCTRL(*(this->GetWidget()->GetWindow()),
                           "ID_COORDINATES_BOOK", wxChoicebook);
      pchbook->InsertPage(RASTER_POSITION, pRasterCoordPage_, RASTER_PAGE_TEXT);
      pRasterCoordPage_ = NULL;
      pchbook->SetSelection(0);
   }
}

/** Configura el ingreso de coordenadas en modo decimal */
void CoordinatesNavigatorPart::ActiveDecimalInput(bool IsProj) {
   coordsInput_[LAT_LONG_ARRAY_POS]->GetInputWidget()->Hide();
   Coordinates coords;
   coordsInput_[LAT_LONG_ARRAY_POS]->GetCoordinate(coords);
   currentCoordInput_ = DECIMAL_ARRAY_POS;
   coordsInput_[currentCoordInput_]->SetProj(IsProj);
   coordsInput_[currentCoordInput_]->SetCoordinate(coords);
   coordsInput_[currentCoordInput_]->GetInputWidget()->Show();
}

/** Configura el ingreso de coordenadas en modo sexagesimal */
void CoordinatesNavigatorPart::ActiveSexagesimalInput() {
   coordsInput_[DECIMAL_ARRAY_POS]->GetInputWidget()->Hide();
   Coordinates coords;
   coordsInput_[DECIMAL_ARRAY_POS]->GetCoordinate(coords);
   currentCoordInput_ = LAT_LONG_ARRAY_POS;
   coordsInput_[currentCoordInput_]->SetCoordinate(coords);
   coordsInput_[currentCoordInput_]->GetInputWidget()->Show();
}
}  /** namespace suri **/
