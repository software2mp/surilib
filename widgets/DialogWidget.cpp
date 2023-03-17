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

#include "suri/DialogWidget.h"

// Includes wx
#include "wx/wx.h"

/** namespace suri */
namespace suri {
/**
 * Constructor
 * @param[in] pParent puntero a la ventana padre
 */
DialogWidget::DialogWidget(wxWindow *pParent) :
      Widget(pParent), pParentDialog_(NULL) {
}

/**
 * Destructor
 */
DialogWidget::~DialogWidget() {
}

/**
 *  Crea la herramienta
 *   Usa el metodo virtual CreateToolWindow y agrega la ventana al padre
 *   Si el padre es nulo, crea una ventana simple y le agrega la herramienta
 * @param[in] Show determina si se muestra o no la herramienta al ser creada
 * @param[in] AutoAdd autoadd
 * @param[in] pParentFrame: puntero al frame padre de la ventana.
 * @return devuelve el resultado del metodo CreateTool de la clase base
 */
bool DialogWidget::CreateTool(wxTopLevelWindow *pParentFrame, bool Show) {
   return Widget::CreateTool(pParentFrame, false);
}

/**
 * Retorna el frame dentro del cual se encuentra la ventana
 * @return frame dentro del cual se encuentra la ventana
 */
wxTopLevelWindow *DialogWidget::GetFrame() const {
   if (pParentDialog_) {
      return pParentDialog_;
   }
   return Widget::GetFrame();
}

/** Muestra el dialogo en forma modal */
/**
 *  Este metodo muestra en forma modal el dialogo creado.
 * \pre En caso de no haber dialogo, se debe llamar a pParentWindow_->Show().
 * @param[in] Show Valor a pasar al metodo llamado.
 * @return boton que se apreto (wxID_OK o wxID_CANCEL)
 */
int DialogWidget::ShowModal(bool Show) {
   int returnvalue = wxID_OK;
   if (pParentDialog_) {
      pParentDialog_->Center();
      returnvalue = pParentDialog_->ShowModal();
   } else {
      if (pParentWindow_)
         pParentWindow_->Show(Show);
      else
         returnvalue = wxID_ABORT;
   }
   /**
    * TODO: verificar si pierde memoria, en algunos casos ocasiona double free
    * 
   if (pToolWindow_) {
      pToolWindow_->Destroy();
      pToolWindow_ = 0;
   }*/
   return returnvalue;
}

/**
 * Termina el dialogo modal
 * @param[in] ReturnCode valor retornado por ShowModal
 */

void DialogWidget::EndModal(int ReturnCode) {
   if (pParentDialog_) {
      pParentDialog_->EndModal(ReturnCode);
      pParentDialog_->Destroy();
      pParentDialog_ = NULL;
   } else {
      return;
   }
}

/**
 * Crea el wxTopLevelWindow
 * @param[in] pParentFrame puntero a la ventana padre
 * @return ventana dialogo de la herramienta
 */
wxTopLevelWindow* DialogWidget::CreateContainingWindow(wxTopLevelWindow *pParentFrame) {
   // Creo el dialogo
   pParentDialog_ = new wxDialog;
   wxTopLevelWindow *pframe = pParentDialog_;

   // TCK #8274: cambio que partio de un arreglo para Simur.
   pParentDialog_->Create(pParentFrame, wxID_ANY, GetWindowTitle(), wxDefaultPosition,
                          wxSize(0, 0),
                          (wxDEFAULT_DIALOG_STYLE /* & ~wxCLOSE_BOX */) | wxRESIZE_BORDER,
                          wxT("ID_DLGEMPTY"));

   // Creo y agrego la ventana de la herramienta
   //  para que lo pueda usar la clase derivada en CreateToolWindow()
   pParentWindow_ = pframe;
   pToolFrame_ = NULL;

   /*-------------------------------------------------------------------------
    * Se agregan (otra vez!!) las lineas:
    *    wxSize size = pParentDialog_->GetClientSize();
    *		wantedWidth_ += size.GetWidth();
    *    wantedHeight_ += size.GetHeight();
    * ya que de lo contrario cuando se crea un NotebookWidget con varios Part
    * no se muestran los Part agregados; es decir, se muestra el frame del
    * PartContainerWidget y en el ClientArea nada (como si fuese un panel vacio).
    * -----------------------------------------------------------------------*/
   wxSize size = pParentDialog_->GetClientSize();
   wantedWidth_ += size.GetWidth();
   wantedHeight_ += size.GetHeight();
   return pParentDialog_;
}
}  // namespace name

