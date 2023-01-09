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
