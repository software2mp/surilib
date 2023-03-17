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

// Includes standard
#include <string>
#include <vector>

// Includes Suri
#include "EllipsoidEditionPart.h"
#include "UniversalGraphicalComponentPartInterface.h"
#include "suri/LibraryManager.h"
#include "GdalSrsItemOrigin.h"
#include "SpatialReference.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/LibraryItemAttribute.h"

// Defines

// Forwards

namespace suri {

const std::string EllipsoidEditionPart::ELLIPSOID_EDITION_XRC_NAME =
      wxT("ID_ELLIPSOID_EDITION_PANEL");
const std::string EllipsoidEditionPart::ELLIPSOID_CGU_XRC_NAME =
      wxT("ID_ELLIPSOID_NAME_PANEL");

EllipsoidEditionPart::EllipsoidEditionPart(
      const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
      UniversalGraphicalComponentPartInterface::FeatureCombinationEnum Feature) :
      LibraryItemEditorPart(LibraryId, pManager, ELLIPSOID_EDITION_XRC_NAME,
                            ELLIPSOID_CGU_XRC_NAME, Feature) {
}

EllipsoidEditionPart::~EllipsoidEditionPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool EllipsoidEditionPart::HasChanged() {
   bool textchanged = XRCCTRL(*(this->GetWidget()->GetWindow()),
            wxT("ID_AUTHORITY_NAME_TEXT"), wxTextCtrl)->IsModified() ||
   XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_AUTHORITY_CODE_TEXT"), wxTextCtrl)->IsModified() ||
   XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_SEMIMAYOR_AXIS_TEXT"), wxTextCtrl)->IsModified() ||
   XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_FLATENING_FACTOR_TEXT"), wxTextCtrl)->IsModified() ||
   XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_FLATENING_FACTOR_TEXT"), wxTextCtrl)->IsModified();

   return textchanged || pCgu_->HasChanged();
}

/** Inicializa el part */
void EllipsoidEditionPart::SetInitialValues() {
}

/** Actualiza el estado de la parte */
void EllipsoidEditionPart::Update() {
}

/**  Metodo que obtiene a partir de los datos del editor, un vector con los atributos
 *   Este metodo es el que hace el comportamiento particular de cada editor a la hora
 *   de obtener la informacion que posee y transmitirla al sistema de bibliotecas.
 *   Es utilizado a modo de Template Method (en CommitChanges y en el notificador)
 *   @return vector con atributos
 *   @return NULL en caso de que la informnacion del part no sea valida
 *   **/
std::vector<LibraryItemAttribute*>* EllipsoidEditionPart::CreateAttributesFromFields() {
   /** genera un wkt del estilo
    *  wkt SPHEROID["Danish 1876",6377019.27,300,AUTHORITY["EPSG","7051"]] **/
   std::string wkt;
   std::string name = pCgu_->GetSelectedItem()->GetPrincipalAttribute()->GetValue();
   std::string authority = XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_AUTHORITY_NAME_TEXT"), wxTextCtrl)->GetValue().c_str();
   std::string code = XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_AUTHORITY_CODE_TEXT"), wxTextCtrl)->GetValue().c_str();
   std::string semimajoraxis = XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_SEMIMAYOR_AXIS_TEXT"), wxTextCtrl)->GetValue().c_str();

   bool semiminoraxis = XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_SEMI_MINOR_AXIS"), wxRadioButton)->GetValue();
   bool flatfactor = XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_FLATT_FACTOR"), wxRadioButton)->GetValue();
   std::string axisorfactorstr;
   if (semiminoraxis) {
      axisorfactorstr = XRCCTRL(*(this->GetWidget()->GetWindow()),
            wxT("ID_SEMIMINOR_AXIS_TEXT"), wxTextCtrl)->GetValue().c_str();

   } else if (flatfactor) {
      axisorfactorstr = XRCCTRL(*(this->GetWidget()->GetWindow()),
            wxT("ID_FLATENING_FACTOR_TEXT"), wxTextCtrl)->GetValue().c_str();
   }
   wkt = "SPHEROID[\"" + name + "\"," + semimajoraxis + "," + axisorfactorstr + ",";
   wkt += "AUTHORITY[\"" + authority + "\"," + "\"" + code + "\"]]";
   std::vector<LibraryItemAttribute*>* pattrs = new std::vector<LibraryItemAttribute*>;
   pattrs->push_back(
         new LibraryItemAttribute(GdalSrsItemOrigin::WKT_ATTRIBUTE_NAME, wkt));
   XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_AUTHORITY_NAME_TEXT"), wxTextCtrl)->SetModified(false);
   XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_AUTHORITY_CODE_TEXT"), wxTextCtrl)->SetModified(false);
   XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_SEMIMAYOR_AXIS_TEXT"), wxTextCtrl)->SetModified(false);
   XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_FLATENING_FACTOR_TEXT"), wxTextCtrl)->SetModified(false);
   XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_FLATENING_FACTOR_TEXT"), wxTextCtrl)->SetModified(false);
   return pattrs;
}

/**
 *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
 *  Este metodo es el que realmente tienen que implementar las clases hijas para
 * lograr la especializacion a la hora de iniciar el part (metodo que se llama
 * en CreateToolWindow, a modo de Template Method).
 **/
bool EllipsoidEditionPart::ConfigureFeatures() {
   return true;
}

/**
 * Metodo auxiliar privado que es necesario implementar en las clases hijas.
 * Este metodo es el encargado de actualizar realmente la informacion en el part
 * Es llamado en el metodo SetActiveItem a modo de Template Method
 **/
void EllipsoidEditionPart::UpdateFields(const LibraryItem* pItem, bool Modifiable) {
   const LibraryItemAttribute* pattr =
         pItem ? pItem->GetAttribute(GdalSrsItemOrigin::WKT_ATTRIBUTE_NAME) : NULL;
   std::string srwkt = pattr ? pattr->GetValue() : "";
   std::string authorityid = SpatialReference::GetAuthorityId(srwkt);
   std::vector<std::string> authority = tokenizer(authorityid, ":");
   std::string aut, code;
   if (authority.size() == 2) {
      aut = authority[0].c_str();
      code = authority[1].c_str();
   }
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT("ID_AUTHORITY_NAME_TEXT"), wxTextCtrl)->SetValue(aut.c_str());
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT("ID_AUTHORITY_NAME_TEXT"), wxTextCtrl)->Enable(Modifiable);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT("ID_AUTHORITY_CODE_TEXT"), wxTextCtrl)->SetValue(code.c_str());
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT("ID_AUTHORITY_CODE_TEXT"), wxTextCtrl)->Enable(Modifiable);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT("ID_SEMIMAYOR_AXIS_TEXT"), wxTextCtrl)->SetValue(
         NumberToString<double>(SpatialReference::GetSpheroidSemiMajorAxis(srwkt)).c_str());
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT("ID_SEMIMAYOR_AXIS_TEXT"), wxTextCtrl)->Enable(Modifiable);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT("ID_FLATENING_FACTOR_TEXT"), wxTextCtrl)->SetValue(
         NumberToString<double>(
               SpatialReference::GetSpheroidInverseFlatteningFactor(srwkt)).c_str());
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT("ID_FLATENING_FACTOR_TEXT"), wxTextCtrl)->Enable(Modifiable);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT("ID_FLATT_FACTOR"), wxRadioButton)->SetValue(true);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT("ID_SEMI_MINOR_AXIS"), wxRadioButton)->Enable(false);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT("ID_SEMIMINOR_AXIS_TEXT"), wxTextCtrl)->Enable(false);
}
}  /** namespace suri */
