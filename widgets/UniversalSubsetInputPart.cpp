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
// Includes Suri
#include "UniversalSubsetInputPart.h"
#include "SpatialReference.h"

// Includes Wx
// Defines
// forwards

namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */START_IMPLEMENT_EVENT_OBJECT(UniversalSubsetInputPartEvent, UniversalSubsetInputPart)IMPLEMENT_EVENT_CALLBACK(OnCheckboxEventHandler,
		OnCheckboxEventHandler(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/**
 * Constructor
 * @param pManager
 * @param pWorld
 */
UniversalSubsetInputPart::UniversalSubsetInputPart(
		DataViewManager* pDataviewManager, LibraryManager *pManager,
		World* pWorld, const Subset& InputSubset,
		ProcessAdaptLayer* pProcessAdaptLayer) :
		UniversalCoordinateInputPart(pManager,
				SHOW_RS_SELECTOR | SHOW_UNIT_SYSTEM_SELECTOR,
				UniversalGraphicalComponentPartInterface::RoWithFFAndNoLabel), pSubsetSelectionPart_(
				new SpatialSelectionPart(pDataviewManager, pWorld,
						InputSubset)), pWorld_(pWorld), inputSubset_(
				InputSubset), pProcessAdaptLayer_(pProcessAdaptLayer), pCheckEventHandler_(
				new UniversalSubsetInputPartEvent(this)), pRasterSubsetSelectionPart_(
				new SpatialSelectionPart(pDataviewManager, pWorld, InputSubset,
						SpatialSelectionPart::PIXEL_LINE_SUBSET)), raster_(
				false) {
	windowTitle_ = _(caption_SPATIAL_SELECTION);
	pSubsetSelectionPart_->SetProcessLayer(pProcessAdaptLayer_);
	pRasterSubsetSelectionPart_->SetProcessLayer(pProcessAdaptLayer_);
}

/** Dtor **/
UniversalSubsetInputPart::~UniversalSubsetInputPart() {
	delete pCheckEventHandler_;
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
bool UniversalSubsetInputPart::OnCommitChanges() {
	return pSubsetSelectionPart_->CommitChanges()
			&& pRasterSubsetSelectionPart_->CommitChanges();
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
bool UniversalSubsetInputPart::OnRollback() {
	bool result = pSubsetSelectionPart_->RollbackChanges()
			&& pRasterSubsetSelectionPart_->RollbackChanges();
#ifdef __WXMSW__
	FixSizers(pParentWindow_, "");
#endif
	pParentWindow_->Update();
	return result;
}

/**
 * Metodo auxiliar que actualiza el widget correspondiente a el ingreso de coordenadas
 * que corresponda
 */
bool UniversalSubsetInputPart::UpdateCoordinatesInputWidget(
		bool ReplaceWidget) {
	/*Para pasar de Lat-Long a Decimal tengo que saber que navinputwidget tenia antes**/
	int flag = SpatialSelectionPart::SEXAGESIMAL_SUBSET;
	if (ReplaceWidget && pToolWindow_) {
		if (SpatialReference::IsPixelLineSpatialRef(outputSpatialReference_)) {
			flag = SpatialSelectionPart::PIXEL_LINE_SUBSET;
		} else if (SpatialReference::IsProjected(outputSpatialReference_)) {
			flag = SpatialSelectionPart::DECIMAL_SUBSET;
		} else if (SpatialReference::IsGeographic(outputSpatialReference_)) {
			if (systemUnitSelected_.compare(DECIMAL_SYSTEM_UNIT) == 0)
				flag = SpatialSelectionPart::DECIMAL_SUBSET;
		} else {
			return false;
		}
		// Actualizo el subset en funcion de la transformacion que se encuentra vigente
		Subset lastsubset;
		GetSubset(lastsubset);
		RemoveControl(pSubsetSelectionPart_);
		pSubsetSelectionPart_ = new SpatialSelectionPart(pDataViewManager_,
				pWorld_, lastsubset, flag);
		pSubsetSelectionPart_->SetProcessLayer(pProcessAdaptLayer_);
		AddControl(pSubsetSelectionPart_, wxT("ID_COORD_INPUT_PANEL"));
		SetSubset(lastsubset);
		pSubsetSelectionPart_->RollbackChanges();
		modified_ = false;
	}

	return true;
}

/** retorna la coordenada que ingresaron, informa si es valida */
bool UniversalSubsetInputPart::GetCoordinate(Coordinates& Coordinate) {
	return false;
}

/** Configura la coordenada que ingresaron, informa si es valida */
bool UniversalSubsetInputPart::SetCoordinate(Coordinates Coordinate) {
	return false;
}

/** retorna la coordenada que ingresaron en srs de trabajo, informa si es valida */
bool UniversalSubsetInputPart::GetSubset(Subset& OutputSubset) {
	if (!pSubsetSelectionPart_)
		return false;
	UpdateCoordinatesTransform();
	Subset spatialsubset;
	if (!raster_)
		pSubsetSelectionPart_->GetInputWindow(spatialsubset);
	else
		pRasterSubsetSelectionPart_->GetInputWindow(spatialsubset);
	// Si sigue siendo NULL o no puede transformar la coordenaeda
	if (!pCoordinateTransform_
			|| pCoordinateTransform_->Transform(spatialsubset,
					DEFAULT_CALCULATION_POINTS, true) <= 0)
		return false;
	OutputSubset = spatialsubset;
	return true;
}

/** Configura el subset que ingresaron en srs de trabajo, informa si es valida */
bool UniversalSubsetInputPart::SetSubset(Subset& NewSubset) {
	Subset newsubset = NewSubset;
	if (!pSubsetSelectionPart_)
		return false;
	UpdateCoordinatesTransform();
	// Si sigue siendo NULL o no puede transformar la coordenaeda
	if (!pCoordinateTransform_
			|| pCoordinateTransform_->Transform(newsubset) < 0)
		return false;

	pSubsetSelectionPart_->SetInternalSubset(newsubset);
	pRasterSubsetSelectionPart_->SetInternalSubset(newsubset);
	return true;
}

/** Devuelve el world que se encuentra activo en el part **/
World* UniversalSubsetInputPart::GetWorld() {
	return pSubsetSelectionPart_->GetWorld();
}

/** Devuelve el icono de la parte */
void UniversalSubsetInputPart::GetWindowIcon(wxBitmap& ToolBitmap, int X,
		int Y) const {
	pSubsetSelectionPart_->GetWindowIcon(ToolBitmap, X, Y);
}

/** Informa si el subset seleccionado es valido */
bool UniversalSubsetInputPart::HasValidData() {
   if (raster_)
      return pRasterSubsetSelectionPart_->HasValidData();

	return pSubsetSelectionPart_->HasValidData();
}

/** Indica si el Part tiene cambios para salvar. */
bool UniversalSubsetInputPart::HasChanged() {
   if (raster_)
      return pRasterSubsetSelectionPart_->HasChanged();

	return pSubsetSelectionPart_->HasChanged();
}

/** Inicializa el part */
void UniversalSubsetInputPart::SetInitialValues() {
	SetOutputSpatialReference(pWorld_->GetSpatialReference());
}

/**
 *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
 *  Este metodo es el que realmente tienen que implementar las clases hijas para
 * lograr la especializacion a la hora de iniciar el part (metodo que se llama
 * en CreateToolWindow, a modo de Template Method).
 **/
bool UniversalSubsetInputPart::ConfigureFeatures() {
	UniversalCoordinateInputPart::ConfigureFeatures();
	GET_CONTROL(*(this->GetWidget()->GetWindow()),
			"ID_RASTER_MODEL_CHECKBOX", wxCheckBox)->Connect(
				wxEVT_COMMAND_CHECKBOX_CLICKED,
				wxCommandEventHandler(
						UniversalSubsetInputPartEvent::OnCheckboxEventHandler),
				NULL, pCheckEventHandler_);
	AddControl(pRasterSubsetSelectionPart_, wxT("ID_RASTER_INPUT_PANEL"));
	GET_CONTROL(*(this->GetWidget()->GetWindow()),
			"ID_RASTER_INPUT_PANEL", wxPanel)->Show(false);
	return true;
}

/** Evento que se ejecuta al des/activar el checkbox **/
void UniversalSubsetInputPart::OnCheckboxEventHandler(wxCommandEvent &Event) {
	bool raster = XRCCTRL(*(this->GetWidget()->GetWindow()),
	"ID_RASTER_MODEL_CHECKBOX", wxCheckBox)->GetValue();
	GET_CONTROL(*(this->GetWidget()->GetWindow()),
			"ID_RASTER_INPUT_PANEL", wxPanel)->Show(raster);
	GET_CONTROL(*(this->GetWidget()->GetWindow()),
			"ID_RS_SELECTOR_PANEL", wxPanel)->Show(!raster);
	GET_CONTROL(*(this->GetWidget()->GetWindow()),
			"ID_COORD_INPUT_PANEL", wxPanel)->Show(!raster);
	GET_CONTROL(*(this->GetWidget()->GetWindow()),
			"ID_UNIT_SYSTEM_CHOICE", wxChoice)->Show(!raster);
	raster_ = raster;
	pToolWindow_->Update();
	pToolWindow_->Refresh();
	pToolWindow_->Fit();
}}/** namespace suri */
