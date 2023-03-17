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
#include <string>

// Includes Suri
#include "suri/LibraryTool.h"
#include "suri/ToolCommand.h"
#include "suri/FullFledgeLibraryEditorWidget.h"
#include "suri/LibraryManagerFactory.h"

// Includes Wx
// Defines
// forwards

namespace suri {

LibraryTool::LibraryTool(DataViewManager* pDataViewManager) :
            GenericTool(pDataViewManager) {
   configurationVariableName_ = "library";
   Initialize();
}

LibraryTool::~LibraryTool() {
   Finalize();
}

void LibraryTool::Execute(const Command *pToolCommand) {
   int id = pToolCommand->GetId();
   if (id == GetId("SpatialReferenceLibrary")) {
      ExecuteLibrary("SpatialReferenceLibrary");
   } else if (id == GetId("DatumLibrary")) {
      ExecuteLibrary("DatumLibrary");
   } else if (id == GetId("EllipsoidLibrary")) {
      ExecuteLibrary("EllipsoidLibrary");
   } else if (id == GetId("DensitySlice")) {
      ExecuteLibrary("DensitySlice");
   } else if (id == GetId("WmsLibrary")) {
      ExecuteLibrary("WmsLibrary");
   } else if (id == GetId("VectorstyleLibrary")) {
	      ExecuteLibrary("VectorstyleLibrary");
   } else if (id == GetId("SpectralSignLibrary")) {
      ExecuteLibrary("SpectralSignLibrary");
   } else if (id == GetId("ConvolutionFiltersLibrary")) {
      ExecuteLibrary("ConvolutionFiltersLibrary");
   } else if (id == GetId("EquationLibrary")) {
      ExecuteLibrary("EquationLibrary");
   }
}

/** Metodo auxiliar que inicializa el widget full fledged con la biblioteca
 *  correspondiente al command*/
void LibraryTool::ExecuteLibrary(const std::string& LibraryCommandName) const {
   std::string librarycode;
	std::string windowtitle;

   if (LibraryCommandName.compare("SpatialReferenceLibrary") == 0) {
      librarycode = LibraryManagerFactory::SpatialReferenceLibraryCode;
		windowtitle = _(caption_SPATIAL_REFERENCE_LIBRARY);
   } else if (LibraryCommandName.compare("DatumLibrary") == 0) {
      librarycode = LibraryManagerFactory::DatumLibraryCode;
		windowtitle = caption_DATUM_LIBRARY;
   } else if (LibraryCommandName.compare("EllipsoidLibrary") == 0) {
      librarycode = LibraryManagerFactory::EllipsoidLibraryCode;
		windowtitle = caption_ELLIPSOID_LIBRARY;
   } else if (LibraryCommandName.compare("DensitySlice") == 0) {
      librarycode = LibraryManagerFactory::DensitySliceCode;
		windowtitle = caption_DENSITY_SLICE_LIBRARY;
   } else if (LibraryCommandName.compare("WmsLibrary") == 0) {
      librarycode = LibraryManagerFactory::WmsLibraryCode;
   	windowtitle = caption_WMS_LIBRARY;
	} else if (LibraryCommandName.compare("VectorstyleLibrary") == 0) {
	   librarycode = LibraryManagerFactory::VectorstyleLibraryCode;
		windowtitle = caption_VECTOR_STYLE_LIBRARY;
   } else if (LibraryCommandName.compare("SpectralSignLibrary") == 0) {
      librarycode = LibraryManagerFactory::SpectralSignLibraryCode;
   	windowtitle = caption_SPECTRAL_SIGN_LIBRARY;
	} else if( LibraryCommandName.compare("ConvolutionFiltersLibrary") == 0) {
	   librarycode = LibraryManagerFactory::ConvolutionFiltersCode;
	   windowtitle = caption_CONVOLUTION_FILTERS;
	} else if( LibraryCommandName.compare("EquationLibrary") == 0) {
      librarycode = LibraryManagerFactory::EquationLibraryCode;
      windowtitle = caption_EQUATION_LIBRARY;
   }
   FullFledgeLibraryEditorWidget* pcontainer = new FullFledgeLibraryEditorWidget(
         GetLibraryManager(), librarycode, windowtitle);
   pcontainer->CreateTool();
   pcontainer->ShowModal();
}

/** */
Command *LibraryTool::CreateCommand(const std::string &CommandName) {
   Command* pcommand = new ToolCommand(this);
   pcommand->SetType("Menu");
   if (CommandName.compare("SpatialReferenceLibrary") == 0) {
      pcommand->SetLocation("ID_SR_LIBRARY_MENUTIEM");
   } else if (CommandName.compare("DatumLibrary") == 0) {
      pcommand->SetLocation("ID_DATUM_LIBRARY_MENUTIEM");
   } else if (CommandName.compare("EllipsoidLibrary") == 0) {
      pcommand->SetLocation("ID_PELLIPSOID_LIBRARY_MENUTIEM");
   } else if (CommandName.compare("DensitySlice") == 0) {
      pcommand->SetLocation("ID_DENSITY_SLICE_LIBRARY_MENUTIEM");
   } else if (CommandName.compare("WmsLibrary") == 0) {
      pcommand->SetLocation("ID_WMS_LIBRARY_MENUTIEM");
   } else if (CommandName.compare("VectorstyleLibrary") == 0) {
	   pcommand->SetLocation("ID_VECTORSTYLE_LIBRARY_MENUTIEM");
   } else if (CommandName.compare("SpectralSignLibrary") == 0) {
      pcommand->SetLocation("ID_SPECTRAL_SIGN_LIBRARY_MENUTIEM");
   } else if (CommandName.compare("ConvolutionFiltersLibrary") == 0) {
      pcommand->SetLocation("ID_CONVOLUTION_FILTERS_LIBRARY_MENUTIEM");
   } else if (CommandName.compare("EquationLibrary") == 0) {
      pcommand->SetLocation("ID_EQUATION_LIBRARY_MENUTIEM");
   }
   return pcommand;
}

/** */
void LibraryTool::DestroyCommand(Command* &pCommand) const {
   delete pCommand;
   pCommand = NULL;
}

/** */
Widget *LibraryTool::CreateWidget(const std::string &Widget) {
   // TODO(Gabriel - TCK #2468): Implementar
   return NULL;
}

} /** namespace suri */
