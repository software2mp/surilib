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

// Include estandard
#include <string>

// Includes Suri
#include "suri/LibraryToolCommandExecutionHandler.h"
#include "suri/DataViewManager.h"
#include "suri/Command.h"
#include "suri/GenericTool.h"
#include "suri/LibraryManagerFactory.h"
#include "suri/FullFledgeLibraryEditorWidget.h"

namespace suri {

namespace core {

/** Constructor **/
SpatialReferenceLibraryCommandExecutionHandler::SpatialReferenceLibraryCommandExecutionHandler(DataViewManager* pDataView) : 
		LibraryToolCommandExecutionHandler(pDataView) {
		libraryName_ = LibraryManagerFactory::SpatialReferenceLibraryCode;
		windowTitle_ = _(caption_SPATIAL_REFERENCE_LIBRARY);
}

/** Metodo responsable de determinar si el comando puede ser manejado por la instancia **/
bool SpatialReferenceLibraryCommandExecutionHandler::CanHandleCommand(const Command* pCommand, GenericTool* pTool) {
	return (pCommand->GetId() == pTool->GetId("SpatialReferenceLibrary"));
}

/** Constructor **/
DatumLibraryCommandExecutionHandler::DatumLibraryCommandExecutionHandler(DataViewManager* pDataView) :
						LibraryToolCommandExecutionHandler(pDataView) {
	libraryName_ = LibraryManagerFactory::DatumLibraryCode;
	windowTitle_ = _(caption_DATUM_LIBRARY);
}

/** Metodo responsable de determinar si el comando puede ser manejado por la instancia **/
bool DatumLibraryCommandExecutionHandler::CanHandleCommand(const Command* pCommand, GenericTool* pTool) {
	return (pCommand->GetId() == pTool->GetId("DatumLibrary"));
}

/** Constructor **/
EllipsoidLibraryCommandExecutionHandler::EllipsoidLibraryCommandExecutionHandler(DataViewManager* pDataView) : 
						LibraryToolCommandExecutionHandler(pDataView) {
	libraryName_ = LibraryManagerFactory::EllipsoidLibraryCode;
	windowTitle_ = _(caption_ELLIPSOID_LIBRARY); 
}

/** Metodo responsable de determinar si el comando puede ser manejado por la instancia **/
bool EllipsoidLibraryCommandExecutionHandler::CanHandleCommand(const Command* pCommand, GenericTool* pTool) {
	return (pCommand->GetId() == pTool->GetId("EllipsoidLibrary"));
}

/** Constructor **/
DensitySliceLibraryCommandExecutionHandler::DensitySliceLibraryCommandExecutionHandler(DataViewManager* pDataView) :
						LibraryToolCommandExecutionHandler(pDataView) {	
	libraryName_ = LibraryManagerFactory::DensitySliceCode;
	windowTitle_ = _(caption_DENSITY_SLICE_LIBRARY);
}

/** Metodo responsable de determinar si el comando puede ser manejado por la instancia **/
bool DensitySliceLibraryCommandExecutionHandler::CanHandleCommand(const Command* pCommand, GenericTool* pTool) {
	return (pCommand->GetId() == pTool->GetId("DensitySlice"));
}

/** Constructor **/
WmsLibraryCommandExecutionHandler::WmsLibraryCommandExecutionHandler(DataViewManager* pDataView) :
						LibraryToolCommandExecutionHandler(pDataView) {
	libraryName_ = LibraryManagerFactory::WmsLibraryCode;
	windowTitle_ = _(caption_WMS_LIBRARY);
}

/** Metodo responsable de determinar si el comando puede ser manejado por la instancia **/
bool WmsLibraryCommandExecutionHandler::CanHandleCommand(const Command* pCommand, GenericTool* pTool) {
	return (pCommand->GetId() == pTool->GetId("WmsLibrary"));
}

/** Constructor **/
VectorStyleLibraryCommandExecutionHandler::VectorStyleLibraryCommandExecutionHandler(DataViewManager* pDataView): 
							LibraryToolCommandExecutionHandler(pDataView) {
	libraryName_ = LibraryManagerFactory::VectorstyleLibraryCode;
	windowTitle_ = _(caption_VECTOR_STYLE_LIBRARY);
}

/** Metodo responsable de determinar si el comando puede ser manejado por la instancia **/
bool VectorStyleLibraryCommandExecutionHandler::CanHandleCommand(const Command* pCommand, GenericTool* pTool) {
	return (pCommand->GetId() == pTool->GetId("VectorstyleLibrary"));
}

/** Constructor **/
ConvolutionFiltersLibraryCommandExecutionHandler::ConvolutionFiltersLibraryCommandExecutionHandler(DataViewManager* pDataView) :
                  LibraryToolCommandExecutionHandler(pDataView) {
   libraryName_ = LibraryManagerFactory::ConvolutionFiltersCode;
   windowTitle_ = _(caption_CONVOLUTION_FILTERS);
}

/** Metodo responsable de determinar si el comando puede ser manejado por la instancia **/
bool ConvolutionFiltersLibraryCommandExecutionHandler::CanHandleCommand(const Command* pCommand, GenericTool* pTool) {
   return (pCommand->GetId() == pTool->GetId("ConvolutionFiltersLibrary"));
}

/** Constructor **/
SpectralSignLibraryCommandExecutionHandler::SpectralSignLibraryCommandExecutionHandler(DataViewManager* pDataView) :
							LibraryToolCommandExecutionHandler(pDataView) {
	libraryName_ = LibraryManagerFactory::SpectralSignLibraryCode;
	windowTitle_ = caption_SPECTRAL_SIGN_LIBRARY;
}

/** Metodo responsable de determinar si el comando puede ser manejado por la instancia **/
bool SpectralSignLibraryCommandExecutionHandler::CanHandleCommand(const Command* pCommand, GenericTool* pTool) {
	return (pCommand->GetId() == pTool->GetId("SpectralSignLibrary"));
}

/** Constructor **/
EquationLibraryCommandExecutionHandler::EquationLibraryCommandExecutionHandler(
      DataViewManager* pDataView) : LibraryToolCommandExecutionHandler(pDataView) {
   libraryName_ = LibraryManagerFactory::EquationLibraryCode;
   windowTitle_ = caption_EQUATION_LIBRARY;
}

/** Metodo responsable de determinar si el comando puede ser manejado por la instancia **/
bool EquationLibraryCommandExecutionHandler::CanHandleCommand(const Command* pCommand, GenericTool* pTool) {
   return (pCommand->GetId() == pTool->GetId("EquationLibrary"));
}

/** Constructor **/
IndexLibraryCommandExecutionHandler::IndexLibraryCommandExecutionHandler(
      DataViewManager* pDataView) : LibraryToolCommandExecutionHandler(pDataView) {
   libraryName_ = LibraryManagerFactory::IndexLibraryCode;
   windowTitle_ = _(caption_INDEX_LIBRARY);
}

/** Metodo responsable de determinar si el comando puede ser manejado por la instancia **/
bool IndexLibraryCommandExecutionHandler::CanHandleCommand(const Command* pCommand, GenericTool* pTool) {
   return (pCommand->GetId() == pTool->GetId("IndexLibrary"));
}
/** Metodo que instancia y configura el administrador de bibliotecas a partir del nombre de la misma **/
void LibraryToolCommandExecutionHandler::ExecuteLibrary(const std::string& LibraryCode, const std::string& WindowTitle) {
	FullFledgeLibraryEditorWidget* pcontainer = 
					new FullFledgeLibraryEditorWidget(pDataView_->GetLibraryManager(), LibraryCode, WindowTitle);
	pcontainer->CreateTool();
	pcontainer->ShowModal();
}
}  /** namespace core **/
}  /** namespace suri **/
