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

#include "HtmlConfigurationData.h"

// Includes standard

// Includes Suri
#include "resources.h"

// Includes Wx

// Includes App

// Defines

namespace suri {
/** Ctor */
HtmlConfigurationData::HtmlConfigurationData(std::string Name, bool Leaf) :
      name_(Name), icon_(icon_UNDEFINED_ELEMENT), state_(OFF), isLeaf_(Leaf),
      pExecuteOnState_(NULL) {
}

/** Dtor */
HtmlConfigurationData::~HtmlConfigurationData() {
}

/** Retorna nombre de nodo */
std::string HtmlConfigurationData::GetName() const {
   return name_;
}

/** Modifica nombre del nodo//! Modifica el nombre del icono */
void HtmlConfigurationData::SetName(std::string Name) {
   this->name_ = Name;
}

/** Retorna el nombre del icono */
std::string HtmlConfigurationData::GetIcon() const {
   return icon_;
}

/** Modifica el nombre del icono */
void HtmlConfigurationData::SetIcon(std::string Icon) {
   this->icon_ = Icon;
}

/** Indica si es nodo hoja */
bool HtmlConfigurationData::IsLeaf() const {
   return isLeaf_;
}

/** Modifica indicador de si es hoja */
void HtmlConfigurationData::Leaf(bool IsLeaf) {
   this->isLeaf_ = IsLeaf;
}

/** Retorna estado del item. */
HtmlConfigurationData::State HtmlConfigurationData::GetState() const {
   return state_;
}

/** Configura estado del item. */
void HtmlConfigurationData::SetState(HtmlConfigurationData::State State) {
   this->state_ = State;
}

/** Retorna command. */
Command* HtmlConfigurationData::GetExecuteOnState() const {
   return pExecuteOnState_;
}

void HtmlConfigurationData::SetExecuteOnState(Command* pExecuteOnState) {
   this->pExecuteOnState_ = pExecuteOnState;
}
}

