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

#include "SaveViewButton.h"
#include "suri/ViewerWidget.h"

// namespace
namespace suri {

/** Ctor */
SaveViewButton::SaveViewButton(wxWindow *pToolbar) :
         Button(pToolbar, wxT(button_SIMUR_SAVE_VIEW)) {
      windowTitle_ = _(tooltip_SAVE_VIEW);
}
/** Guarda la imagen mostrada por pantalla en formato elegido */
void SaveViewButton::OnButtonClicked(wxCommandEvent &Event) {
   wxDC *pscreendc = new wxClientDC(pViewer_->GetWindow());
   wxMemoryDC *ptargetdc = new wxMemoryDC;
   int width, height;
   pscreendc->GetSize(&width, &height);
   wxBitmap outputbmp(width, height);
   ptargetdc->SelectObject(outputbmp);
   ptargetdc->Blit(0, 0, width, height, pscreendc, 0, 0);
   ptargetdc->SelectObject(wxNullBitmap);
   delete ptargetdc;
   delete pscreendc;
   std::string visualizationpath = Configuration::GetParameter("app_visualization_dir","");
   wxFileDialog dlg(NULL, _(caption_SAVE_VISUALIZATION), wxT(visualizationpath), wxT(""),
   filter_SAVE_VISUALIZATION,
                    wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
   if (dlg.ShowModal() == wxID_OK) {
         outputbmp.ConvertToImage().SaveFile(dlg.GetPath());
   }
}

} // namespace suri.
