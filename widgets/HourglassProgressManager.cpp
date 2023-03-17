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

#include "HourglassProgressManager.h"

// Includes Suri
#include "logmacros.h"
#include "suri/DialogWidget.h"
#include "resources.h"
// Includes Wx
#include "wx/progdlg.h"
#include "wx/gauge.h"
// Includes App

// Defines
/** Mensaje para mostrar en la barra de progreso */
#define message_OPERATION_IN_PROGRESS "Operacion en progreso"

#ifdef __WXMSW__
/** Workaround to restore the busy cursor after wxSafeYield and wxExecute */
bool RestoreBusy()
{
   if ( wxIsBusy() )
   {
      ::SetCursor( (HCURSOR) wxHOURGLASS_CURSOR->GetHCURSOR() );
      return true;
   }
   return false;
}
#endif

/** namespace suri */
namespace suri {
/** Muestra una barra que avanza de a poco */
/**
 * Luego de crear la ventana se espera un tiempo en caso que la operacion
 * se termine rapidamente, luego se muestra una ventana con una barra que
 * avanza un poco cada vez que se llama al metodo Pulse.
 */
class ProgressWidget : public DialogWidget {
public:
   /** Ctor */
   ProgressWidget() :
         DialogWidget(NULL) {
      windowTitle_ = _(caption_OPERATION_IN_PROGRESS);
      wantedWidth_ = 400;
      wantedHeight_ = -1;
      pGauge_ = NULL;
      CreateTool(NULL, false);
      GetFrame()->Centre();
   }
   /** Dtor. */
   virtual ~ProgressWidget() {
   }
   /** Crea la ventana de la barra de progreso */
   virtual bool CreateToolWindow() {
      pToolWindow_ = new wxPanel(pParentWindow_, wxID_ANY);
      pToolWindow_->SetMinSize(wxSize(wantedWidth_, wantedHeight_));
      pToolWindow_->SetSizer(new wxBoxSizer(wxHORIZONTAL));
      pGauge_ = new wxGauge(pToolWindow_, wxID_ANY, 100);
      pToolWindow_->GetSizer()->Add(pGauge_, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
      return true;
   }

   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
      GET_BITMAP_RESOURCE(icon_HOURGLASS, ToolBitmap);
   }
   /** Metodo update */
   bool Update(int Cicle) {
      return true;
   }
   /** Inicializa los recursos XRC */
   virtual bool InitializeXmlResources() const {
      return true;
   }
   /** Muestra un progreso en la barra */
   bool Pulse() {
      pGauge_->Pulse();
      return true;
   }
private:
   wxGauge *pGauge_; /*! Control que indica progreso */
};

/** Ctor. */
HourglassProgressManager::HourglassProgressManager() :
#ifdef __USE_PROGRESSBAR__
     pProgressWidget_(NULL)
#endif
{}

/** Dtor */
HourglassProgressManager::~HourglassProgressManager() {
}

/**
 * Pone el cursor como relog y desactiva las ventanas de la aplicacion
 * @param[in] Cycles cantidad de ciclos de la tarea
 * @param[in] Description descripcion de la tarea
 * @return retorna el identificador asignado a la tarea
 */
int HourglassProgressManager::GetProgressId(int Cycles,
                                            const std::string &Description) {
   int id = ProgressManager::GetProgressId(Cycles, Description);
   if (progressQueue_.size() == 1) {
      wxBeginBusyCursor();
#ifdef __USE_PROGRESSBAR__
      // Genera la barra, solo se muestra al ocurrir Update despues de 1seg
      timer_.Start(1000, wxTIMER_ONE_SHOT);
      // delete pProgressWidget_;
      if (pProgressWidget_) {
         pProgressWidget_->GetFrame()->Destroy();
      }
      pProgressWidget_ = new ProgressWidget();
#endif
   }
   return id;
}
/**
 * Si no quedan procesos pendientes activa las ventanas de la aplicacion y
 * muestra flecha
 * @param[in] ProgressId identificador de la tarea que se libera
 * @return true si pudo liberar la tarea, false en otro caso
 */
bool HourglassProgressManager::ReleaseProgressId(int ProgressId) {
   bool retval = ProgressManager::ReleaseProgressId(ProgressId);
   if (progressQueue_.size() == 0) {
#ifdef __USE_PROGRESSBAR__
      // Elimina el frame de la barra.
      timer_.Stop();
      pProgressWidget_->Update(100);
      if (pProgressWidget_) {
         pProgressWidget_->GetFrame()->Destroy();
      }
      pProgressWidget_ = NULL;
#endif
      wxEndBusyCursor();
   }
   return retval;
}
/**
 * Hace un yiald
 * @param[in] ProgressId identificador de la tarea
 */
void HourglassProgressManager::Update(int ProgressId) const {
   REPORT_DEBUG("D: Updating Id #%d", ProgressId);
#ifdef __USE_PROGRESSBAR__
   if (!timer_.IsRunning()) {
      // Si finalizo el timer despliega la barra
      REPORT_DEBUG("D: Showing bar");
      FLUSH();
      pProgressWidget_->GetFrame()->Show();
      pProgressWidget_->Pulse();
      REPORT_DEBUG("D: BarUpdate");
      FLUSH();
   }
#endif
   wxTheApp->Yield();
#ifdef __WXMSW__
   RestoreBusy();
#endif
}

/**
 * Metodo que devuelve una instancia de HourglassProgressManager
 * @return instancia de HourglassProgressManager
 */
ProgressManager *WxProgressManager() {
   return new HourglassProgressManager;
}
}

