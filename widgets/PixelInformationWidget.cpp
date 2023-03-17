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

// Includes standard
#include <string>

// Includes Suri
#include "PixelInformationWidget.h"
#include "UniversalGraphicalComponentPart.h"
#include "GeneralPixelInformationPart.h"
#include "RasterPixelInformationPart.h"
#include "resources.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/ParameterCollection.h"
#include "VectorDatasource.h"
#include "RasterDatasource.h"
#include "TablePart.h"
#include "suri/VectorEditionTable.h"
#include "suri/BufferedDriver.h"
#include "suri/VectorEditorDriver.h"
#include "MemoryVector.h"

// Includes Wx

// Includes App

// Defines
#define SRS_LIBRARY_CODE "srs"

namespace suri {

/** Ctor */
PixelInformationWidget::PixelInformationWidget(
      DatasourceContainerInterface *pDatasourceList, const Coordinates &Point,
      const std::string &PointWkt, const Coordinates &MClick, const Coordinates &ProcessedPl,
      World* pViewerWorld, LibraryManager* pLibraryManager) :
      NotebookWidget(wxEmptyString, wxNB_TOP, NotebookWidget::Notebook, SUR_BTN_OK, SUR_BTN_OK),
      pDatasourceList_(pDatasourceList), point_(Point), pointWkt_(PointWkt), mclick_(MClick),
      processedPl_(ProcessedPl), pViewerWorld_(pViewerWorld) {
   pLibraryManager_ = pLibraryManager;
   windowTitle_ = _(caption_PIXEL_INFORMATION);
}

/** Dtor */
PixelInformationWidget::~PixelInformationWidget() {
}

bool PixelInformationWidget::CreateToolWindow() {
   UniversalGraphicalComponentPart* psrsselectionpart = NULL;
   if (NotebookWidget::CreateToolWindow()) {
      Library* plibrary = pLibraryManager_->GetLibraryByCode(SRS_LIBRARY_CODE,
                                                LibraryManager::AccessKey(true));
      psrsselectionpart = new UniversalGraphicalComponentPart(
            plibrary->GetId(), pLibraryManager_, NULL,
            UniversalGraphicalComponentPart::ExtendedRoWithFFAndNoLabel);

      AddControl(psrsselectionpart, wxT("ID_RS_SELECTION_PANEL"));

      LoadCoordinatesInfo(point_, pointWkt_);

      // Agrego el tab de informacion gral. Por el momento va asi
      // porque tiene que ser el primero.
      InsertPart(CreateGeneralPixelInformationPart());

      // Agrego capas raster y vectoriales
      DatasourceContainerInterface::UuidListType datasourceids =
            pDatasourceList_->GetDatasourceList();
      DatasourceContainerInterface::UuidListType::iterator it = datasourceids.begin();
      it = datasourceids.begin();
      for (; it != datasourceids.end(); ++it) {
         Part *ppart = NULL;
         DatasourceInterface* pdatasrc = pDatasourceList_->GetDatasource(*it);
         if (dynamic_cast<VectorDatasource*>(pdatasrc) != NULL) {
            ppart = GetVectorTablePart(pdatasrc);
         } else if (dynamic_cast<RasterDatasource*>(pdatasrc) != NULL) {
            ParameterCollection info;
            info.AddValue<Coordinates>("ClickP", mclick_);
            info.AddValue<Coordinates>("CoordinatesP", processedPl_);
            info.AddValue<DatasourceInterface*>("DatasourceInterfaceP", pdatasrc);
            info.AddValue<World*>("ViewerWorld", pViewerWorld_);
            ppart = new RasterPixelInformationPart(info);
            if (pdatasrc->GetClassId() == "RasterDatasource" && psrsselectionpart) {
               std::string principal = SpatialReference::GetAuthorityId(
                     pdatasrc->GetSpatialReference());
               psrsselectionpart->SelectItemByPrincipal(principal);
            }
         }
         InsertPart(ppart);
      }
      return true;
   }
   return false;
}

wxWindow *PixelInformationWidget::GetToolWindow() {
   wxWindow *pwin = NotebookWidget::GetToolWindow();

   wxSizer *pmaincontrolsizer = USE_CONTROL(*pwin, wxT("ID_CONTAINER_WIN"),
                                      wxWindow, GetContainingSizer(), NULL);
   wxPanel *ppanel = wxXmlResource::Get()->LoadPanel(
                                      pwin, wxT("ID_PIXEL_INFORMATION_PANEL"));
   wxWindow *poldwin = XRCCTRL(*pwin, wxT("ID_CONTAINER_WIN"), wxWindow);
   pmaincontrolsizer->Replace(poldwin, ppanel, true);
   poldwin->Destroy();
   return pwin;
}

/** Retorna la el icono de la ventana */
void PixelInformationWidget::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(tool_QUERY, ToolBitmap);
}

/**
 * Crea el part que muestra info general sobre el pixel.
 *
 * @return Devuelve el part listo para ser agregado.
 */
Part* PixelInformationWidget::CreateGeneralPixelInformationPart() {
   int layerscount = 0;
   int layershlcount = 0;
   int rastercount = 0;
   int vectorcount = 0;
   ParameterCollection paramsinfo;

   DatasourceContainerInterface::UuidListType datasourceids =
         pDatasourceList_->GetDatasourceList();
   DatasourceContainerInterface::UuidListType::iterator it = datasourceids.begin();
   for (; it != datasourceids.end(); ++it) {
      DatasourceInterface* pdatasrc = pDatasourceList_->GetDatasource(*it);
      VectorDatasource* pvectordatasrc = dynamic_cast<VectorDatasource*>(pdatasrc);
      if (pvectordatasrc != NULL) {
         ++vectorcount;
         if (pvectordatasrc->HasHotlink() == true)
            ++layershlcount;
      }
      else if (dynamic_cast<RasterDatasource*>(pdatasrc) != NULL) ++rastercount;
      ++layerscount;
   }
   paramsinfo.AddValue<int>("LayersCount", layerscount);
   paramsinfo.AddValue<int>("LayersHlCount", layershlcount);
   paramsinfo.AddValue<int>("RasterCount", rastercount);
   paramsinfo.AddValue<int>("VectorCount", vectorcount);

   return new GeneralPixelInformationPart(paramsinfo);
}

/**
 * Metodo que obtiene y configura el part para pagina de consulta de pixel para una
 * fuente de datos vectorial
 * @param pVectorDatasource
 * @return
 */
Part* PixelInformationWidget::GetVectorTablePart(DatasourceInterface* pVectorDatasource) {
   // Preparo los datos en memory para que los pueda usar el driver.
   std::string url = MEMORY + "layertablepart_" + GET_NAME(pVectorDatasource->GetUrl());
   Vector* pmemoryvector = Vector::Open(url, Vector::ReadWrite);
   Vector* pdiskvector = Vector::Open(pVectorDatasource->GetUrl());
   if (pmemoryvector == NULL || pdiskvector == NULL) {
      REPORT_DEBUG("DEBUG: Fallo al crear vector en disco y/o memoria");
      return NULL;
   }
   pdiskvector->Copy(pmemoryvector);
   Vector::Close(pdiskvector);

   // Actualizo el nombre del datasource para que lo pueda usar el driver.
   pVectorDatasource->SetUrl(url);
   VectorEditionTable* ptable = new VectorEditionTable(pVectorDatasource);
   VectorEditorDriver* pdriver = new VectorEditorDriver(pVectorDatasource);
   ptable->SetDriver(pdriver);
   TablePart* ptablepart = new TablePart(ptable);
   ptablepart->SetWindowIcon(pVectorDatasource->GetElement()->GetIcon().c_str());
   ptablepart->SetSelectionNotifier(NULL);
   ptablepart->SetWindowTitle(pVectorDatasource->GetName());
   return ptablepart;
}

/**
 * Carga la informacion de coordenadas.
 */
void PixelInformationWidget::LoadCoordinatesInfo(const Coordinates& Coords,
                                                 const std::string& CoordsWkt) {
   Coordinates point = Coords;

   wxStaticText* platNoth = XRCCTRL(*pToolWindow_, wxT("ID_LATITUDE_TEXT"), wxStaticText);
   wxStaticText* plonEast = XRCCTRL(*pToolWindow_, wxT("ID_LONGITUDE_TEXT"), wxStaticText);

   wxStaticText* platNothStt = XRCCTRL(*pToolWindow_, wxT("ID_LATITUDE_STATIC"), wxStaticText);
   wxStaticText* plonEastStt = XRCCTRL(*pToolWindow_, wxT("ID_LONGITUDE_STATIC"), wxStaticText);

   if (!SpatialReference::IsProjected(CoordsWkt)) {
      double sec = 0.0;
      int deg = 0, min = 0, sign = 0;

      ParameterCollection params;
      params.AddValue<std::string>(TransformationFactory::kParamWktIn, CoordsWkt);
      params.AddValue<std::string>(TransformationFactory::kParamWktOut, wxT(""));

      TransformationFactory* pfactory = TransformationFactoryBuilder::Build();

      CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact,
                                                        params);
      TransformationFactoryBuilder::Release(pfactory);
      if (pct) {
         pct->Transform(point);
         delete pct;
      }

      // Latitud.
      CONVERT_DEGREES(point.y_, deg, min, sec, sign);
      wxString pointstr = wxString::Format(
            _(format_DEGREE_MINUTES_SECONDS), deg, min, sec,
            (sign > 0 ? _(label_NORTH_SHORT) : _(label_SOUTH_SHORT)));
      platNoth->SetLabel(pointstr);

      // Longitud.
      CONVERT_DEGREES(point.x_, deg, min, sec, sign);
      pointstr = wxString::Format(
            _(format_DEGREE_MINUTES_SECONDS), deg, min, sec,
            (sign > 0 ? _(label_EAST_SHORT) : _(label_WEST_SHORT)));
      plonEast->SetLabel(pointstr);
   } else {
      // Norte.
      wxString pointstr = wxString::Format(wxT("%.2f"), Coords.y_);
      platNoth->SetLabel(pointstr);
      platNothStt->SetLabel("Norte: ");

      // Este.
      pointstr = wxString::Format(wxT("%.2f"), Coords.x_);
      plonEast->SetLabel(pointstr);
      plonEastStt->SetLabel("Este: ");
   }
}

}  // namespace suri
