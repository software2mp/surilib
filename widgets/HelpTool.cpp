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
#include "AboutPart.h"
#include "RegistrationCodeInputPart.h"
#include "suri/HelpTool.h"
#include "suri/ToolCommand.h"
#include "suri/Configuration.h"
#include "suri/PartContainerWidget.h"

// Includes Wx
#include "wx/html/helpctrl.h"

// Defines

// forwards

namespace suri {

/**
 * Constructor
 * @param[in]  pDataViewManager: Puntero al DataViewManager
 */
HelpTool::HelpTool(DataViewManager* pDataViewManager) :
      GenericTool(pDataViewManager), pHelpController_(new wxHtmlHelpController) {
   configurationVariableName_ = "help";
   Initialize();
}

/**
 * Destructor
 */
HelpTool::~HelpTool() {
   Finalize();
   delete pHelpController_;
}

/**
 * Si el comando indicado, se encuentra entre la lista de ejecutables, se ejecutara
 * @param[in]  pToolCommand: Comando a ejecutar
 */
void HelpTool::Execute(const Command* pToolCommand) {
   int id = pToolCommand->GetId();
   if (id == GetId("Help")) {
      std::string helpdir = suri::Configuration::GetParameter("app_help_dir", wxT("./"));
      helpdir += "ayuda.hhp";
      pHelpController_->AddBook(wxFileName(helpdir), false);
      pHelpController_->Display(0);
      pHelpController_->SetFrameParameters(_("Ayuda: %s"), wxSize(1024, 600));
   } else if (id == GetId("Registration")) {
      RegistrationCodeInputPart *pregistration = new RegistrationCodeInputPart;
      PartContainerWidget *pcontainer = new PartContainerWidget(pregistration,
                                                                _(caption_REGISTRATION_PART));
      pcontainer->ShowModal();
   } else if (id == GetId("About")) {
      AboutPart *pregistration = new AboutPart;
      PartContainerWidget *pcontainer = new PartContainerWidget(pregistration, caption_ABOUT_PART,
                                                                SUR_BTN_OK, SUR_BTN_OK);
      pcontainer->ShowModal();
   }
}

/**
 * Fabrica el comando si es uno de los predefinidos
 * @param[in]  CommandName: Nombre del comando a ejecutar
 * @return     Devuelve el comando creado si tuvo exito, sino NULL
 */
Command* HelpTool::CreateCommand(const std::string &CommandName) {
   Command *pcommand = NULL;
   if (CommandName == "Help") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription("Help");
      pcommand->SetDecoration("");
      pcommand->SetLocation("wxID_HELP");
   } else if (CommandName == "Registration") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription("Help");
      pcommand->SetDecoration("");
      pcommand->SetLocation("ID_REGISTRATION_MENUTIEM");
   } else if (CommandName == "About") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription("Help");
      pcommand->SetDecoration("");
      pcommand->SetLocation("wxID_ABOUT");
   }
   return pcommand;
}

/**
 * Destruye un comando previamente creado con CreateCommand
 * @param[in]  pCommand: Puntero a un Comando creado no NULL a destruir
 *
 */
void HelpTool::DestroyCommand(Command* &pCommand) const {
   delete pCommand;
   pCommand = NULL;
}

/**
 * Creador del Widget
 */
Widget* HelpTool::CreateWidget(const std::string &Widget) {
   // TODO(Sebastian - TCK #878): Deteccion de codigo sin implementar
   return NULL;
}

} /** namespace suri */
