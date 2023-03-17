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
#include <vector>
#include <list>
#include <utility>

// Includes Suri
#include "suri/GuiImpl.h"
#include "suri/Command.h"
#include "suri/CommandMenu.h"
#include "suri/Widget.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/ToolInterface.h"
#include "suri/CommandToggleButton.h"
#include "suri/CommandButton.h"
#include "suri/SliderCommand.h"
#include "suri/ChoiceCommand.h"
#include "suri/GuiStateChangeMediator.h"
#include "suri/DynCommandButton.h"

// Includes Wx
#include "wx/menuitem.h"
#include "wx/xrc/xmlres.h"
#include "wx/window.h"
#include "wx/wx.h"

// Defines

// forwards

namespace suri {

const std::string GuiImpl::CallerLocationToken = "|";
const std::string GuiImpl::PositionLocationToken = ";";
const int GuiImpl::LocationPosition = 1;
const int GuiImpl::LocationDescriptionPosition = 0;

GuiImpl::GuiImpl(suri::Widget *pGuiWidget) :
      GuiInterface(pGuiWidget), pGuiWidget_(pGuiWidget),
                                 pGuiStateMediator_(NULL), isMenuEnable_(true),
                                 isRemoteLayer_(false) {
   validtools_.push_back("ID_ZOOM_IN_MENUTIEM");
   validtools_.push_back("ID_ZOOM_OUT_MENUTIEM");
   validtools_.push_back("ID_NAVIGATION_MENUTIEM");
   validtools_.push_back("ID_MEASSURE_MENUTIEM");
   validtools_.push_back("ID_PIXEL_QUERY_MENUTIEM");
   validtools_.push_back("ID_CONFIG_OPTIONS_MENUITEM");
   validtools_.push_back("ID_RASTER_GENERIC_IMPORTER_MENUTIEM");
   validtools_.push_back("ID_ADD_CSV_LAYER_MENUTIEM");
   validtools_.push_back("ID_EDIT_MAP_MENUTIEM");
}

GuiImpl::~GuiImpl() {
   delete_second_all(toolGroupMap_);
}

/**
 * Agrega la herramienta en la ubicacion de los Commands asociados
 * Precondicion: Se asume que las ubicaciones de las commands de la herramienta
 * a agregar vienen dadas de forma correcta y que respetan el mismo orden que el de los
 * tipos definidos en los Commands asociados
 * @param pTool
 * @return
 */
bool GuiImpl::AddTool(ToolInterface* pTool) {
   std::list<suri::Command*> commands = pTool->GetCommands();
   std::list<suri::Command*>::iterator it = commands.begin();
   for (; it != commands.end(); ++it) {
      std::vector < std::string > types = suri::tokenizer((*it)->GetType(), "|");
      std::vector < std::string > locations =
            suri::tokenizer((*it)->GetLocation(), "|");

      std::vector<std::string>::iterator typeit = types.begin();
      unsigned int locationposition = 0;

      for (; typeit != types.end(); ++typeit, ++locationposition) {
         std::string location = (locations.size() > 0) ?
               locations[locationposition % locations.size()] : "";
         if ((*typeit).compare("Menu") == 0) {
            AddMenu(*it, location);
         } else {
            ToolGroupToolAddition(*it, *typeit, location);
         }
      }

      if (pGuiStateMediator_) {
         pGuiStateMediator_->RegisterCommand(pTool->GetCommandGuiId(*it), *it);
         (*it)->Attach(pGuiStateMediator_);
      }
   }
   return true;
}

/**
 * Metodo auxilar que dada una ubicacion agrega el command a todos los
 * toolgroupmanagerasociados
 * @param pCommand
 * @param CommandType
 * @param Location
 */
void GuiImpl::ToolGroupToolAddition(Command *pCommand, const std::string& CommandType,
                                    const std::string& Location) {
   size_t tokenfound = Location.find(PositionLocationToken);
   std::vector<std::string> locationdesc = suri::tokenizer(Location, PositionLocationToken);
   std::string location = Location;
   int position = suri::ToolGroupManager::Last;
   if (tokenfound != std::string::npos && (locationdesc.size() == 2)) {
      position = StringToNumber<int>(locationdesc[LocationPosition]);
      location = locationdesc[LocationDescriptionPosition];
   }
   ToolGroupMapType::iterator findit = toolGroupMap_.find(location);
   ToolGroupManager* ptgm = NULL;
   if (findit != toolGroupMap_.end()) {
      ptgm = findit->second;
      ManageToolAddition(pCommand, ptgm, CommandType, location, position);
   } else {
      std::pair < CallerToolGroupMapType::iterator, CallerToolGroupMapType::iterator
            > equalrange = callerToolGroupMultiMap_.equal_range(Location);
      CallerToolGroupMapType::iterator multifindit = equalrange.first;
      for (; multifindit != equalrange.second; ++multifindit) {
         ManageToolAddition(pCommand, multifindit->second, CommandType, location,
                            position);
      }
   }
}

/**
 * Metodo auxiliar que maneja de una herramienta en funcion al command asociado
 * @param pCommand
 * @param pTgm
 * @param CommandType
 * @param Location
 * @param Position entero que indica la posicion en la cual se agrega la herramienta
 */
void GuiImpl::ManageToolAddition(Command *pCommand, ToolGroupManager* pTgm,
                                 const std::string& CommandType,
                                 const std::string& Location, int Position) {
   int group = pCommand->GetGroupId();
   // \todo: Leak y SIGSEGV si se elimina *it (pCommand)
   if (CommandType.compare("ToggleButton") == 0 && pTgm) {
      pTgm->AddButtonTool(new suri::CommandToggleButton(pCommand), Position, group);
   } else if (CommandType.compare("Button") == 0 && pTgm) {
      pTgm->AddButtonTool(new suri::CommandButton(pCommand), Position, group);
   } else if (CommandType.compare("Choice") == 0 && pTgm) {
      pTgm->AddChoiceTool(new suri::ChoiceCommand(pCommand), Position);
   } else if (CommandType.compare("Slider") == 0 && pTgm) {
      pTgm->AddSliderTool(new suri::SliderCommand(pCommand), Position);
   } else if (CommandType.compare("DynButton") == 0 && pTgm) {
      pTgm->AddButtonTool(new suri::DynCommandButton(pCommand), Position);
   }
}
/**
 * Elimina la herramienta de las ubicaciones del os Commandas asociados
 * @param pTool
 * @return
 */
bool GuiImpl::RemoveTool(ToolInterface* pTool) {
   return true;
}

/**
 * Metodo que agrega un menu en la ubicacion descripta por el command
 * @param pCommand
 * @param MenuLocation
 */
void GuiImpl::AddMenu(suri::Command *pCommand, const std::string& MenuLocation) {
   wxMenuItem *pitem = CreateMenuFromCommand(pCommand, MenuLocation);
   // \todo: Leak y SIGSEGV si se elimina pCommand
   pGuiWidget_->PushCommandEvent(new CommandMenu(pCommand, pitem));
}
void GuiImpl::DisableMenuNoElement() {

   if (isMenuEnable_) {
      std::vector<std::string> options;
      options.push_back("ID_CONFIG_OPTIONS_MENUITEM");
      EnableTopMenu(false, 1, options);
      EnableTopMenu(false, 2, options);
      isMenuEnable_ = false;
   }
}

void GuiImpl::EnableMenuRemoteLayer(bool Enable) {
   if (isRemoteLayer_ == Enable) {
      for (int x = 1; x < 3; x++) {
         isRemoteLayer_ = !Enable;
         EnableTopMenu(Enable, x, validtools_);
      }
   }
}

void GuiImpl::EnableMenu(bool Enable) {
   if (isMenuEnable_ == Enable) return;
   wxFrame *pframe = dynamic_cast<wxFrame*>(pGuiWidget_->GetFrame());
   wxMenuBar *pmenubar = pframe->GetMenuBar();
   for (unsigned int x = 0; x < pmenubar->GetMenuCount(); x++) {
      pmenubar->EnableTop(x, Enable);
      wxMenu * pmenu = pmenubar->GetMenu(x);
      wxMenuItemList menulist = pmenu->GetMenuItems();
      wxMenuItemList::iterator it = menulist.begin();
      for (; it != menulist.end(); ++it) {
         (*it)->Enable(Enable);
      }
   }
   isMenuEnable_ = Enable;
}

void GuiImpl::EnableTopMenu(bool Enable, int Position, const std::vector<std::string>& ValidNames) {
   wxFrame *pframe = dynamic_cast<wxFrame*>(pGuiWidget_->GetFrame());
   wxMenuBar *pmenubar = pframe->GetMenuBar();
   if (!pmenubar) return;
   wxMenu * pmenu = pmenubar->GetMenu(Position);
   wxMenuItemList menulist = pmenu->GetMenuItems();
   wxMenuItemList::iterator it = menulist.begin();
   for (; it != menulist.end(); ++it) {
      std::vector<std::string>::const_iterator itvalid = ValidNames.begin();
      bool validName = false;
      for (; itvalid != ValidNames.end(); ++itvalid) {
         wxMenuItem * pmenuitem = dynamic_cast<wxMenuItem*>((*it));
         int id = GetLocationId(*itvalid);
         if (pmenuitem->GetId() == id) {
            validName = true;
         }
      }
      if (!validName) (*it)->Enable(Enable);
   }
}

/**
 *  Elimina un menu
 *  @param[in] MenuLocation
 */
void GuiImpl::RemoveMenu(const std::string& MenuLocation) {
}

/**
 * Inicializa el ToolGroupManager con la wxWindow y ubicacion pasada por parametro
 * @param pToolBar
 * @param LocationId
 */
void GuiImpl::InitializeToolbar(wxWindow *pToolBar, const std::string& LocationId) {
   ToolGroupMapType::iterator findit = toolGroupMap_.find(LocationId);
   if (findit == toolGroupMap_.end()) {
      std::string windowlocation;
      size_t tokenfound = LocationId.find(CallerLocationToken);
      std::vector < std::string > locationdesc =
                                      suri::tokenizer(LocationId, CallerLocationToken);
      bool complexid = false;
      if (tokenfound != std::string::npos && (locationdesc.size() == 2)) {
         windowlocation = locationdesc[0];
         complexid = true;
      } else {
         windowlocation = LocationId;
      }
      ToolGroupManager* ptgm = new ToolGroupManager();
      ptgm->InitializeToolbar(XRCCTRL(*pToolBar, windowlocation.c_str(),
                                      wxWindow), NULL, NULL);
      // linkeo el nuevo toolgroupmanager con el anterior
      if (toolGroupMap_.size() > 0) {
         ToolGroupMapType::iterator linkit = toolGroupMap_.end();
         --linkit;
         linkit->second->Link(ptgm);
      }
      toolGroupMap_.insert(std::make_pair(LocationId, ptgm));
      if (complexid)
         callerToolGroupMultiMap_.insert(std::make_pair(windowlocation, ptgm));
   }
}

/** 
 * Configura el GuiChangeMediator. 
 * @param[in] pGuiStateMediator mediador donde se deben registrar los command creados.
 */
void GuiImpl::SetGuiStateChangeMediator(GuiStateChangeMediator* pGuiStateMediator) {
   pGuiStateMediator_ = pGuiStateMediator;
}


/**
 *
 * @param pWidget
 */
void GuiImpl::AddToolbar(Widget *pWidget) {
}

/**
 * Elimina el toolgroup manager asociado a la toolbar con la ubicacion
 * pasada por parametro
 * @param[in] LocationId string con el id de XRC que corresponde
 * a la toolbar a borrar
 */
void GuiImpl::RemoveToolbar(const std::string& LocationId) {
   std::vector < std::string > locationdesc =
                                   suri::tokenizer(LocationId, CallerLocationToken);
   size_t tokenfound = LocationId.find(CallerLocationToken);
   if (tokenfound != std::string::npos && (locationdesc.size() == 2)) {
      std::pair < CallerToolGroupMapType::iterator, CallerToolGroupMapType::iterator
            > equalrange = callerToolGroupMultiMap_.equal_range(locationdesc[0]);
      callerToolGroupMultiMap_.erase(equalrange.first, equalrange.second);
   }
   ToolGroupMapType::iterator findit = toolGroupMap_.find(LocationId);
   if (findit != toolGroupMap_.end()) {
      ToolGroupManager* ptgm = findit->second;
      toolGroupMap_.erase(findit);
      ptgm->Unlink();
      delete ptgm;
   }
}

/**
 *
 * @param pWidget
 */
void GuiImpl::AddWidget(suri::Widget *pWidget) {
}

/**
 *
 */
void GuiImpl::RemoveWidget() {
}

/*!
 * Obtiene el Id del para un item de menu en funcion de Location. El Id puede
 * provenir de un XRCID, levantado de configuracion o generado.
 * \todo: Faltan el levantado de configuracion y el generado.
 * \todo: Esta buscando entre todos los strings separados por | cual da un
 *   ID de XRC valido. Modificar para que Location tenga que estar sincronizado
 *   con Type en Command.
 */
int GuiImpl::GetLocationId(const std::string &Location) {
   std::vector<std::string> possiblelocations = suri::tokenizer(Location, "|");
   std::vector<std::string>::iterator locationit = possiblelocations.begin();
   int location = -2;
   while (locationit != possiblelocations.end() && location == -2) {
      location = wxXmlResource::Get()->GetXRCID((*locationit).c_str(), -2);
      ++locationit;
   }
   return location;
}

/*!
 * \todo: En caso de no encontrar el item a partir de location, parsear location
 *        para determinar donde agregar y no en GetMenu(0). (*)
 * \todo: Usar menu contextual para el caso de que el widget no sea autonomo.
 * \todo: No agregar compulsivamente en el frame asociado al widget, solo
 *        si es autonomo.
 */
wxMenuItem *GuiImpl::CreateMenuFromCommand(suri::Command *pCommand,
                                           const std::string& MenuLocation) {
   wxFrame *pframe = dynamic_cast<wxFrame*>(pGuiWidget_->GetFrame());
   wxMenuBar *pmenubar = pframe->GetMenuBar();
   int itemid = GetLocationId(MenuLocation);
   wxMenuItem *pitem = pmenubar->FindItem(itemid);
   wxMenu *pmenu = NULL;
   if (!pitem) {
      // \todo: ver (*)
      pmenu = pmenubar->GetMenu(0);
      wxString itemlabel = MenuLocation.c_str();
      pitem = pmenu->Append(wxID_ANY, itemlabel);
      itemid = pitem->GetId();
   }
   pitem->Enable(pCommand->IsEnabled());
   pmenu = pitem->GetMenu();
   if (pitem->GetKind() == wxITEM_NORMAL && !pitem->GetBitmap().IsOk()
         && !pCommand->GetDecoration().empty()) {
      wxBitmap bitmap;
      size_t itempos = 0;
      while (pmenu->FindItemByPosition(itempos++)->GetId() != itemid) {
      }
      pmenu->Remove(pitem);
      GET_BITMAP_RESOURCE(pCommand->GetDecoration().c_str(), bitmap);
      ESCALE_BITMAP(bitmap, 16, 16);
      pitem->SetBitmap(bitmap);
      pmenu->Insert(itempos - 1, pitem);
   }
   pitem->SetHelp(_(pCommand->GetDescription().c_str()));
   return pitem;
}

} /* namespace suri */
