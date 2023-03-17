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

#include <cstdlib>
#include <memory>
#include <string>

#include "MeassureWidget.h"

#include "MeasureAreaButton.h"
#include "MeasureChangedNotification.h"
#include "MeasureDeletionRequestedNotification.h"
#include "MeasureDistanceButton.h"
#include "MeassureDistanceElementEditor.h"
#include "suri/Button.h"
#include "suri/Configuration.h"
#include "suri/DatasourceInterface.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/DataViewManager.h"
#include "suri/FileManagementFunctions.h"
#include "suri/LabelText.h"
#include "suri/messages.h"
#include "suri/RasterElement.h"
#include "suri/SuriObject.h"
#include "suri/ToolSupport.h"
#include "suri/Vector.h"
#include "suri/VectorEditor.h"
#include "suri/ViewcontextManagerInterface.h"
#include "suri/ViewerWidget.h"
#include "suri/ViewportManagerInterface.h"

#include "logmacros.h"
#include "resources.h"
#include "wxmacros.h"

#include "wx/bmpbuttn.h"
#include "wx/strconv.h"
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"

// Define
#define MEASSURE_LAYER "Medicion"

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(MeassureWidgetEvent, MeassureWidget)
      IMPLEMENT_EVENT_CALLBACK(OnDeleteClicked, OnDeleteClicked(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnChoisePerimDistClicked, OnChoisePerimDistClicked(event),
         wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnChoiseAreaClicked, OnChoiseAreaClicked(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnSaveMeassureClicked, OnSaveMeassureClicked(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT

/**
 * Ctor.
 */
MeassureWidget::MeassureWidget(wxWindow* pParent, ViewerWidget* pViewer,
                               int VectorGroup, VectorEditor* pVectorEditor,
                               Widget* pPart, DataViewManager* pDataViewManager) :
      Widget(pParent), area(0), NEW_EVENT_OBJECT(MeassureWidgetEvent),
      pViewer_(pViewer), vectorGroup_(VectorGroup), pVectorEditor_(pVectorEditor),
      pPart_(pPart), pDataViewManager_(pDataViewManager), pActivatedButton_(NULL) {
}

/**
 * Dtor.
 */
MeassureWidget::~MeassureWidget() {
}

/**
 * Carga el widget de medicion.
 */
bool MeassureWidget::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_, "ID_MEASSUREWIDGET_PANEL");

   // todo provisorio Definir como va a funcionar boton y menu antes de eliminar
   wxWindow *ptoolbar = XRCCTRL(*pToolWindow_, wxT("ID_MEASSURE_BUTTON_PANEL"), wxPanel);
   if (!ptoolbar) {
      return false;
   }

   InitializeToolbar(ptoolbar, pViewer_, pViewer_ ? pViewer_->GetList() : NULL);

   // TCK #6316:
   // Se elimino esta linea para desvincular el boton quick meassure de los botones de edicion
   // del la clase meassurewidget.
   // pViewer_->Link(this);

   //*** Area ***//
   MeassureAreaElementEditor* peditorarea = new MeassureAreaElementEditor(
         pViewer_, vectorGroup_,
         pDataViewManager_ ? pDataViewManager_->GetViewportManager() : NULL,
         pDataViewManager_ ? pDataViewManager_->GetViewcontextManager() : NULL);

   peditorarea->Attach(this);
   this->Attach(peditorarea);

   MeasureAreaButton* pareabutton = new MeasureAreaButton(
         pToolWindow_, wxT(tool_MEASURE_AREA), caption_AREA,
         BUTTON_LEFT_UP | BUTTON_RIGHT_UP, peditorarea);

   AddButtonTool(pareabutton, First, ToolGroupManager::Edition | ToolGroupManager::Default);

   //*** Distancia ***//
   MeassureDistanceElementEditor* peditordis = new MeassureDistanceElementEditor(
         pViewer_, vectorGroup_,
         pDataViewManager_ ? pDataViewManager_->GetViewportManager() : NULL,
         pDataViewManager_ ? pDataViewManager_->GetViewcontextManager() : NULL);

   peditordis->Attach(this);
   this->Attach(peditordis);

   MeasureDistanceButton* pdistancebutton = new MeasureDistanceButton(
         pToolWindow_, wxT(tool_MEASURE_DISTANCE), caption_DISTANCE,
         BUTTON_LEFT_UP | BUTTON_RIGHT_UP, peditordis);

   AddButtonTool(pdistancebutton, First,
                 ToolGroupManager::Edition | ToolGroupManager::Default);

   //*** Borrar ***//
   wxBitmapButton* pdelete = XRCCTRL(*pToolWindow_,
                                     wxT("ID_MEASSUREWIDGET_PANEL_DELETE"),
                                     wxBitmapButton);

   pdelete->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                          wxCommandEventHandler(MeassureWidgetEvent::OnDeleteClicked),
                          NULL, pEventHandler_);

   XRCCTRL(*pToolWindow_, wxT("ID_SAVE_MEASSURE"), wxBitmapButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(MeassureWidgetEvent::OnSaveMeassureClicked),
         NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_,
           wxT("ID_MEASSUREWIDGET_PANEL_CHS_AREA"),
           wxChoice)->Connect(wxEVT_COMMAND_CHOICE_SELECTED , wxCommandEventHandler(
            MeassureWidgetEvent::OnChoiseAreaClicked), NULL, pEventHandler_);

   GET_CONTROL(*pToolWindow_,
           wxT("ID_CHOICE4"),
           wxChoice)->Connect(wxEVT_COMMAND_CHOICE_SELECTED , wxCommandEventHandler(
            MeassureWidgetEvent::OnChoisePerimDistClicked), NULL, pEventHandler_);

   return (pToolWindow_ != NULL);
}

/**
 * Recibe notificaciones de las medidas tomadas.
 */
void MeassureWidget::Update(NotificationInterface* pNotification) {
   MeasureChangedNotification* pmnot = dynamic_cast<MeasureChangedNotification*>(pNotification);
   if (pmnot) {
      wxChoice* pchsPerimDist = XRCCTRL(*pToolWindow_, wxT("ID_CHOICE4"), wxChoice);
      wxChoice* pchsArea = XRCCTRL(*pToolWindow_, wxT("ID_MEASSUREWIDGET_PANEL_CHS_AREA"), wxChoice);
      if (pmnot->GetType() & MeasureChangedNotification::kArea) {
         wxStaticText *parea = XRCCTRL(*pToolWindow_,
                                       wxT("ID_MEASSUREWIDGET_PANEL_AREA"),
                                       wxStaticText);
         area = pmnot->GetArea();
         parea->SetLabel(wxString::Format("%.2f", CalculateSelection(area,
            pchsArea->GetCurrentSelection(), MeasureChangedNotification::kArea)));
      }
      if (pmnot->GetType() & MeasureChangedNotification::kPerimeter) {
         wxStaticText *pperim = XRCCTRL(*pToolWindow_,
                                       wxT("ID_MEASSUREWIDGET_PANEL_PERIM_DIST"),
                                       wxStaticText);
         perimdist = pmnot->GetPerimeter();
         pperim->SetLabel(wxString::Format("%.2f", CalculateSelection(perimdist,
            pchsPerimDist->GetCurrentSelection(), MeasureChangedNotification::kPerimeter)));
      }
      if (pmnot->GetType() & MeasureChangedNotification::kDistance) {
         wxStaticText *pdistance = XRCCTRL(*pToolWindow_,
                                        wxT("ID_MEASSUREWIDGET_PANEL_PERIM_DIST"),
                                        wxStaticText);
         perimdist = pmnot->GetDistance();
         pdistance->SetLabel(wxString::Format("%.2f", CalculateSelection(perimdist,
            pchsPerimDist->GetCurrentSelection(), MeasureChangedNotification::kDistance)));
      }
      geometry_ = pmnot->GetMeassureGeometry();
   }
}

/**
 * Despliega el panel para mostrar las mediciones.
 */
void MeassureWidget::ShowWidget() {
   pPart_->Show(wxT("ID_MEASSURE_TOOL"));
}

/**
 * Oculta el panel para mostrar las mediciones.
 */
void MeassureWidget::HideWidget() {
   pPart_->Hide(wxT("ID_MEASSURE_TOOL"));
   if (pActivatedButton_ != NULL) {
      DeactivateButton(pActivatedButton_);
   }
}

/**
 * Metodo que se llama cuando el boton se activo correctamente.
 *
 * @param[in] pButton Boton que fue activado.
 */
void MeassureWidget::ButtonActivated(Button* pButton) {
   MeasureAreaButton* parea = dynamic_cast<MeasureAreaButton*>(pButton);
   if (parea != NULL) {
      ConfigControls(kAreaMode);
      pActivatedButton_ = parea;
   } else {
      MeasureDistanceButton* pdistance = dynamic_cast<MeasureDistanceButton*>(pButton);
      if (pdistance != NULL) {
         ConfigControls(kDistanceMode);
         pActivatedButton_ = pdistance;
      }
   }
}

/**
 * Configura el widget para mostrar medidas de area y perimetro o
 * para mostrar medidas de distancia.
 *
 * @param[in] Mode Alguno de los siguientes valores: kAreaMode o kDistanceMode
 */
void MeassureWidget::ConfigControls(int Mode) {
   wxStaticText* pstt = NULL;
   wxChoice* pchs = NULL;
   if (Mode == kAreaMode) {
      pstt = XRCCTRL(*pToolWindow_, wxT("ID_MEASSUREWIDGET_PANEL_LBL_PERIM_DIST"), wxStaticText);
      pstt->SetLabel(_("Perimetro: "));

      pstt = XRCCTRL(*pToolWindow_, wxT("ID_MEASSUREWIDGET_PANEL_LBL_AREA"), wxStaticText);
      pstt->Enable();

      pstt = XRCCTRL(*pToolWindow_, wxT("ID_MEASSUREWIDGET_PANEL_AREA"), wxStaticText);
      pstt->Enable();

      pchs = XRCCTRL(*pToolWindow_, wxT("ID_MEASSUREWIDGET_PANEL_CHS_AREA"),
                     wxChoice);
      pchs->Enable();
   } else if (Mode == kDistanceMode) {
      pstt = XRCCTRL(*pToolWindow_, wxT("ID_MEASSUREWIDGET_PANEL_LBL_PERIM_DIST"),
                     wxStaticText);
      pstt->SetLabel(_("Distancia: "));

      pstt = XRCCTRL(*pToolWindow_, wxT("ID_MEASSUREWIDGET_PANEL_LBL_AREA"), wxStaticText);
      pstt->Disable();

      pstt = XRCCTRL(*pToolWindow_, wxT("ID_MEASSUREWIDGET_PANEL_AREA"), wxStaticText);
      pstt->Disable();

      pchs = XRCCTRL(*pToolWindow_, wxT("ID_MEASSUREWIDGET_PANEL_CHS_AREA"),
                     wxChoice);
      pchs->Disable();
   }
}

/**
 * Maneja el evento de click en el boton para borrar geometria.
 */
void MeassureWidget::OnDeleteClicked(wxCommandEvent &Event) {
   std::auto_ptr<MeasureDeletionRequestedNotification>
      delnot(new MeasureDeletionRequestedNotification());
   Notify(delnot.get());
}

/**
 * Manejo de evento que guarda la medicion realizada.
 */
void MeassureWidget::OnSaveMeassureClicked(wxCommandEvent &Event) {
   VectorEditor editor;
   std::string filename = GetOutputFileName();
   /** precision hardcode **/
   Vector::VectorType vtype = area > 0.0000000001 ? Vector::Polygon : Vector::Line;
   bool success = true;
   SuriObject::UuidType vid = pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontextId();
   RasterElement* praster = ToolSupport::GetActiveRaster(
         pDataViewManager_->GetViewcontextManager(), vid);
   if (!filename.empty() && !geometry_.empty() && editor.OpenVector(filename)
         && editor.OpenLayer(MEASSURE_LAYER, praster->GetSpatialReference().c_str(),
                             vtype) >= 0) {
      long fid = editor.OpenFeature();
      if (fid >= 0 && editor.SetGeometryByWkt(fid, geometry_)) {
         editor.CloseVector();
         DatasourceInterface* pdatasource = DatasourceInterface::Create("VectorDatasource", filename);
         std::string dsname;

         if (Configuration::GetParameter("app_short_name", "") != wxT("SoPI")) {
            wxString stemp;
            stemp = _("Medicion") + wxString(" [");
            if (vtype == Vector::Polygon) {
               wxStaticText* parea = XRCCTRL(*pToolWindow_,
                                             wxT("ID_MEASSUREWIDGET_PANEL_AREA"),
                                             wxStaticText);
               stemp += _("Area") + wxString("=");
               stemp += parea->GetLabel().c_str();
               stemp += XRCCTRL(*pToolWindow_, wxT("ID_MEASSUREWIDGET_PANEL_CHS_AREA"),
                                 wxChoice)->GetStringSelection().c_str();
               stemp += "; ";
            }
            wxStaticText *pperimdist = XRCCTRL(
                  *pToolWindow_, wxT("ID_MEASSUREWIDGET_PANEL_PERIM_DIST"),
                  wxStaticText);
            stemp += area > 0 ? (_("Perimetro") + wxString("=")) : wxString("Distancia=");
            stemp += pperimdist->GetLabel().c_str();
            stemp +=
                  XRCCTRL(*pToolWindow_, wxT("ID_CHOICE4"), wxChoice)->GetStringSelection().c_str();
            stemp += "]";

            dsname = stemp.mb_str(wxConvUTF8);
         } else {
            dsname = "Medicion [";
            if (vtype == Vector::Polygon) {
               wxStaticText* parea = XRCCTRL(*pToolWindow_,
                                             wxT("ID_MEASSUREWIDGET_PANEL_AREA"),
                                             wxStaticText);
               dsname += "Area=";
               dsname += parea->GetLabel().c_str();
               dsname += XRCCTRL(*pToolWindow_, wxT("ID_MEASSUREWIDGET_PANEL_CHS_AREA"),
                                 wxChoice)->GetStringSelection().c_str();
               dsname += "; ";
            }
            wxStaticText *pperimdist = XRCCTRL(
                  *pToolWindow_, wxT("ID_MEASSUREWIDGET_PANEL_PERIM_DIST"),
                  wxStaticText);
            dsname += area > 0 ? "Perimetro=" : "Distancia=";
            dsname += pperimdist->GetLabel().c_str();
            dsname +=
                  XRCCTRL(*pToolWindow_, wxT("ID_CHOICE4"), wxChoice)->GetStringSelection().c_str();
            dsname += "]";
         }

         pdatasource->SetName(dsname);
         if (Configuration::GetParameter("app_short_name", "") != wxT("SoPI")) {
            pDataViewManager_->GetDatasourceManager()->AddDatasourceWithNotification(
                  pdatasource, vid);
         } else {
            pDataViewManager_->GetDatasourceManager()->AddDatasource(pdatasource);
         }
      } else {
         success = false;
      }
   } else {
      success = false;
   }

   if (!success) {
      SHOW_ERROR(_(message_MEASSURE_ERROR));
   }
}

/** Metodo auxiliar que obtiene el archivo destino en donde se guardara la medicion **/
std::string MeassureWidget::GetOutputFileName() {
   std::string filename;
   if (Configuration::GetParameter("app_short_name", "") != wxT("SoPI")) {
      std::string pathtempaux = suri::Configuration::GetParameter(
            "app_vector_base_dir_volatile", wxT(""));
      pathtempaux = suri::GetNextPath(pathtempaux);
      std::string ftemp = wxFileName::CreateTempFileName(pathtempaux).c_str();
      ftemp += ".shp";
      filename = ftemp;
   } else {
      wxFileDialog dlg(NULL, _(caption_SAVE_VECTOR), wxT(""), wxT(""),
      filter_SAVE_VECTOR,
                       wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

      if (dlg.ShowModal() == wxID_OK) {
         filename = dlg.GetPath();
      }
   }
   return filename;
}

/**
 * Maneja el evento de click en el choise para seleccionar unidad de distancia/perimetro.
 */
void MeassureWidget::OnChoisePerimDistClicked(wxCommandEvent &Event) {
   wxChoice* pchsPerimDist = XRCCTRL(*pToolWindow_, wxT("ID_CHOICE4"), wxChoice);
   wxStaticText *pperimdist = XRCCTRL(*pToolWindow_,
                                  wxT("ID_MEASSUREWIDGET_PANEL_PERIM_DIST"),
                                  wxStaticText);

   if (!IsZero(pperimdist)) {
    pperimdist->SetLabel(wxString::Format("%.2f",
      CalculateSelection(perimdist, pchsPerimDist->GetCurrentSelection(),
         MeasureChangedNotification::kPerimeter)));
   }
}

/**
 * Maneja el evento de click en el choise para seleccionar unidad del area.
 */
void MeassureWidget::OnChoiseAreaClicked(wxCommandEvent &Event) {
   wxChoice* pchsArea = XRCCTRL(*pToolWindow_, wxT("ID_MEASSUREWIDGET_PANEL_CHS_AREA"), wxChoice);
   wxStaticText* parea = XRCCTRL(*pToolWindow_,
                                  wxT("ID_MEASSUREWIDGET_PANEL_AREA"),
                                  wxStaticText);
   parea->SetLabel(wxString::Format("%.2f",
      CalculateSelection(area, pchsArea->GetCurrentSelection(),
         MeasureChangedNotification::kArea)));
}

/**
 * Calcula la medida tomada en la unidad seleccionada.
 * @param[in] value valor de la medida tomada
 * @param[in] unit indice de la unidad seleccionada en el choise
 * @param[in] mode tipo de medicion
 * @return valor de la medida tomada en la unidad correspondiente
 */
double MeassureWidget::CalculateSelection(double value, int unit, int mode) {
   // Para el area el valor en "value" viene en metros cuadrados
   if (mode == MeasureChangedNotification::kArea) {
      double auxArea = value;
         if (unit == km2)
            return auxArea * 0.001 * 0.001;
         else if (unit == m2)
            return auxArea;
         else if (unit == ha)
            return auxArea * 0.0001;
   }
   if (mode == MeasureChangedNotification::kPerimeter ||
      mode == MeasureChangedNotification::kDistance) {
      double auxPerimdist = value;
      if (unit == km)
         return auxPerimdist * 0.001;
      else if (unit == m)
         return auxPerimdist;
      else if (unit == mi)
         return auxPerimdist * 0.000621371192237334;
   }
   return 0;
}
bool MeassureWidget::IsZero(wxStaticText *pperimdist) {
	   char strvalor[24] = {0x00};
	   int valor = 0;
	   strcpy( strvalor, pperimdist->GetLabel().c_str());
	   valor = atoi(strvalor);

	   return (valor == 0);
}
}  /** namespace suri */
