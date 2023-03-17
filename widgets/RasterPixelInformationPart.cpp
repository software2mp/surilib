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

#include <cstdio>

#include "RasterPixelInformationPart.h"
#include "TablePartConfigurator.h"
#include "TablePart.h"
#include "suri/ParameterCollection.h"
#include "suri/Coordinates.h"
#include "suri/DatasourceInterface.h"
#include "suri/Part.h"
#include "RasterDatasource.h"
#include "DefaultTable.h"
#include "RasterDnDriver.h"
#include "suri/BufferedDriver.h"
#include "RasterDnInfo.h"
#include "suri/World.h"

#include "suri/DatasourceManagerInterface.h"
#include "SuriObjectGroupManager.h"
#include "suri/ViewcontextManagerInterface.h"
#include "suri/ViewcontextTreeSelectionManager.h"
#include "suri/LibraryManager.h"
#include "suri/TaskManager.h"
#include "suri/FeatureSelection.h"
#include "suri/DataViewManager.h"
#include "suri/LibraryManagerFactory.h"
#include "DefaultDatasourceManager.h"
#include "DefaultViewcontextManager.h"

namespace suri {

const std::string RasterPixelInformationPart::kRasterPixelInformationXrcName =
      wxT("ID_RASTER_PIXEL_INFORMATION_PART");
const std::string RasterPixelInformationPart::kRasterPixelInformationTitle = "";

START_IMPLEMENT_EVENT_OBJECT(RasterPixelInformationEvent, RasterPixelInformationPart)
   IMPLEMENT_EVENT_CALLBACK(OnResize, OnResize(event), wxSizeEvent)
END_IMPLEMENT_EVENT_OBJECT


/** Ctor */
RasterPixelInformationPart::RasterPixelInformationPart(ParameterCollection& Params) :
   NEW_EVENT_OBJECT(RasterPixelInformationEvent), pRasterDs_(NULL), pViewerWorld_(NULL) {
   DatasourceInterface* pinterface = NULL;

   Params.GetValue<Coordinates>("ClickP", click_);
   Params.GetValue<Coordinates>("CoordinatesP", coords_);
   Params.GetValue<DatasourceInterface*>("DatasourceInterfaceP", pinterface);
   Params.GetValue<World*>("ViewerWorld", pViewerWorld_);

   pRasterDs_ = dynamic_cast<RasterDatasource*>(pinterface);
}

/** Dtor */
RasterPixelInformationPart::~RasterPixelInformationPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool RasterPixelInformationPart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool RasterPixelInformationPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool RasterPixelInformationPart::RollbackChanges() {
   return true;
}

/** Inicializa el Part */
void RasterPixelInformationPart::SetInitialValues() {
}

/** Actualiza el estado del Part */
void RasterPixelInformationPart::Update() {
}

/**
 * Implementacion del metodo de creacion de ventana de Widget.
 *
 * @return true si la ventana se creo con exito.
 */
bool RasterPixelInformationPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(
         pParentWindow_, kRasterPixelInformationXrcName);
   windowTitle_ = pRasterDs_->GetName();

   char buff[19] = { 0 };

   sprintf(buff, "%f", coords_.x_);
   wxStaticText* pstatic = XRCCTRL(*pToolWindow_, wxT("ID_RASTER_PINFO_LINE"), wxStaticText);
   pstatic->SetLabel(wxT(buff));

   sprintf(buff, "%f", coords_.y_);
   pstatic = XRCCTRL(*pToolWindow_, wxT("ID_RASTER_PINFO_PIXEL"), wxStaticText);
   pstatic->SetLabel(wxT(buff));

   // agrego la tabla
   RasterDnInfo* pdninfo = new RasterDnInfo();
   pdninfo->SetLine(click_.x_);
   pdninfo->SetPixel(click_.y_);
   pdninfo->SetDatasource(pRasterDs_);
   pdninfo->SetWorld(pViewerWorld_);

   suri::TablePartConfigurator tableconf;
   suri::Option opt = suri::TablePartConfigurator::GetRasterDnTableEditorConfig();

   DefaultTable* ptable = new DefaultTable();
   RasterDnDriver* pdriver = new RasterDnDriver(pdninfo);
   BufferedDriver* pbuffer = new BufferedDriver(pdriver);
   ptable->SetDriver(pbuffer);
   TablePart* ptablepart = tableconf.Initialize(GetDataView(), ptable, opt);

   AddControl(ptablepart, wxT("ID_RASTER_PINFO_TABLE_CONTAINER"));

   pToolWindow_->Connect(wxEVT_SIZE, wxSizeEventHandler(RasterPixelInformationEvent::OnResize),
                     NULL, pEventHandler_);

   return true;
}

/**
 * Retorna el icono de la herramienta.
 */
void RasterPixelInformationPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   if (X < 0 && Y < 0) {
      ToolBitmap = wxArtProvider::GetBitmap(wxART_MISSING_IMAGE);
      return;
   }
   GET_BITMAP_RESOURCE(pRasterDs_->GetElement()->GetIcon().c_str(), ToolBitmap);
   ESCALE_BITMAP(ToolBitmap, X, Y);
}

/**
 * Obtiene un dataviewmanager para el table part utilizando
 * componentes con valores por defecto.
 *
 * @return punto a un nuevo DataViewManager.
 *
 * \todo (danieltesta-#3216): verificar quien tiene la responsabilidad
 * de liberar la memoria del dataviewmanager.
 */
DataViewManager* RasterPixelInformationPart::GetDataView() {
   suri::DatasourceManagerInterface* pdm = new suri::DefaultDatasourceManager();
   suri::SuriObjectGroupManager* pgroupmanager = new suri::SuriObjectGroupManager(pdm);
   suri::ViewcontextManagerInterface* pvm = new suri::DefaultViewcontextManager(pdm);
   suri::ViewcontextTreeSelectionManager* pselectionmanager =
         new suri::ViewcontextTreeSelectionManager(pgroupmanager, pvm);
   suri::LibraryManager* plibrarymanager =
                     suri::LibraryManagerFactory::CreateLibraryManager();
   suri::TaskManager* ptaskmanager = new suri::TaskManager();
   suri::FeatureSelection* pfeatureselection = new suri::FeatureSelection();

   DataViewManager* pmanager = new suri::DataViewManager(pdm, pvm, NULL,
                                                         pgroupmanager,
                                                         pselectionmanager,
                                                         plibrarymanager, ptaskmanager,
                                                         pfeatureselection);

   return pmanager;
}

/** Lanzado cuando cambia el tamanio de la ventana */
void RasterPixelInformationPart::OnResize(wxSizeEvent &Event) {
#ifdef __WXMSW__
   FixSizers(pToolWindow_, "");
#endif
}

}  // namespace suri
