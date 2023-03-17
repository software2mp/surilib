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

#include "suri/PartCollection.h"

// Includes standard

// Includes Suri

// Includes Wx
#include "wx/wx.h"

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Ctor */
PartCollection::PartCollection() :
      Part() {
}

/** Ctor */
PartCollection::PartCollection(Widget* pWidget) :
      Part(pWidget) {
}

/** Ctor */
PartCollection::PartCollection(const std::string &XrcId, const std::string &PartName) :
      Part(XrcId, PartName, true, false) {
}

/** Dtor */
PartCollection::~PartCollection() {
}

/**
 * Agrega un part a la coleccion
 * @param[in] pNewPart part que se quiere agregar a la coleccion.
 * @param[in] ParentWidgetId es el id del panel donde se debe agregar la
 * ventana del widget asociado a pNewPart.
 */
void PartCollection::AddPart(Part* pNewPart, const std::string &ParentWidgetId) {
   partCollection_.push_back(std::pair<Part*, std::string>(pNewPart, ParentWidgetId));
}

/**
 * Remueve un part de la coleccion
 * @param[in] pPart Part que se quiere remover de la coleccion.
 */
void PartCollection::RemovePart(Part* pPart) {
   // STUB
}

/**
 * Realiza la difusion de HasChanged a todos los parts que componen la
 * coleccion interna.
 * @return bool que indica OR de los resultados particulares de cada part.
 */
bool PartCollection::HasChanged() {
   bool haschanged = false;

   std::list<std::pair<Part*, std::string> >::iterator it = partCollection_.begin();
   while (it != partCollection_.end() && !haschanged) {
      haschanged = haschanged || it->first->HasChanged();
      it++;
   }

   return haschanged;
}

/**
 * Realiza la difusion del metodo commit a todos los parts que componen la
 * coleccion interna.
 * @return bool que indica AND de los Commit particulares de cada part.
 */
bool PartCollection::CommitChanges() {
   bool success = true;

   std::list<std::pair<Part*, std::string> >::iterator it = partCollection_.begin();
   while (it != partCollection_.end() && success) {
      success = success && it->first->CommitChanges();
      it++;
   }

   return success;
}

/**
 * Realiza la difusion de Rollback a todos los parts que componen la coleccion
 * interna.
 * @return bool que indica AND de los Rollback particulares de cada part.
 */
bool PartCollection::RollbackChanges() {
   bool success = true;

   std::list<std::pair<Part*, std::string> >::iterator it = partCollection_.begin();
   while (it != partCollection_.end() && success) {
      success = success && it->first->RollbackChanges();
      it++;
   }

   return success;
}

/**
 * Realiza la difusion de SetInitialValues a todos los parts que componen la
 * coleccion interna.
 */
void PartCollection::SetInitialValues() {
   std::list<std::pair<Part*, std::string> >::iterator it = partCollection_.begin();
   while (it != partCollection_.end()) {
      it->first->SetInitialValues();
      it++;
   }
}

/**
 * Realiza la difusion de Update a todos los parts que componen la coleccion
 * interna.
 */
void PartCollection::Update() {
   std::list<std::pair<Part*, std::string> >::iterator it = partCollection_.begin();
   while (it != partCollection_.end()) {
      it->first->Update();
      it++;
   }
}

/**
 * Agrega a pWidget los controles de todos los parts en partColection_.
 * @return bool que indica si se pudo configurar la ventana correctamente.
 */
bool PartCollection::ConfigureWidget() {
   bool addcontrolok = true;
   std::list<std::pair<Part*, std::string> >::iterator it;
   for (it = partCollection_.begin(); it != partCollection_.end(); it++) {
      if (!GetWidget()->AddControl(it->first->GetWidget(), it->second)) {
         addcontrolok = false;
      }
   }

   return addcontrolok;
}
}
