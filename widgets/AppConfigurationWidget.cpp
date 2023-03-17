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

#include "suri/AppConfigurationWidget.h"
#include "suri/Configuration.h"
#include "suri/Part.h"
#include "suri/DialogWidget.h"

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(AppConfigurationWidgetEvent, AppConfigurationWidget)
         IMPLEMENT_EVENT_CALLBACK(OnButtonOk, OnButtonOk(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT


/* Constructor. */
AppConfigurationWidget::AppConfigurationWidget(
      AppConfigPartsProviderInterface* pappconfigparts, wxString Titulo,
      DataViewManager* pDataviewmanager) :
      NotebookWidget(Titulo), pAppconfigparts_(pappconfigparts), pDataviewmanager_(
            pDataviewmanager), NEW_EVENT_OBJECT(AppConfigurationWidgetEvent) {
   CreateTool();
}

/* Destructor */
AppConfigurationWidget::~AppConfigurationWidget() {
}

/** Commit de los cambios efectuados. */
bool AppConfigurationWidget::ApplyChanges() {
   if (!PartContainerWidget::ApplyChanges()){
      Configuration::DiscardChanges();
      return false;
   }
   return true;
}

/** Inserta los parts necesarios para la configuracion de la aplicacion */
bool AppConfigurationWidget::InsertParts() {
   std::vector<Part *> parts = pAppconfigparts_->GetConfigParts();
   std::vector<Part *>::iterator part = parts.begin();

   for (; part != parts.end(); ++part) {
      if (!InsertPart(*part))
            return false;
   }
   return true;
}
void AppConfigurationWidget::OnButtonOk(wxCommandEvent &Event) {
   Event.Skip(false);
   if (ApplyChanges()) {
      Configuration::Save();
      EndModal(wxID_OK);
   }
}

} // End namespace suri
