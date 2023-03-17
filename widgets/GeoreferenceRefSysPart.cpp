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

#include "GeoreferenceRefSysPart.h"
#include "UniversalGraphicalComponentPartInterface.h"
#include "suri/LibraryManager.h"
#include "TablePart.h"
#include "SourceSrsChangeNotification.h"
#include "SpatialReference.h"

#define PRINCIPAL_PANEL_ID    "ID_GEOREFERENCE_REF_SYS_PART"
#define CGU_PANEL_ID          "ID_GEOREFERENCE_REF_SYS_REFSYS"

namespace suri {

/**
 * Ctor.
 */
GeoreferenceRefSysPart::GeoreferenceRefSysPart(const SuriObject::UuidType& LibraryId,
                                    LibraryManager* pManager, TablePart* pTablePart) :
      LibraryItemEditorPart(
            LibraryId, pManager, PRINCIPAL_PANEL_ID, CGU_PANEL_ID, true,
            UniversalGraphicalComponentPartInterface::ExtendedRoWithFFAndNoLabel,
            LibraryManager::READONLY) {
   pTablePart_ = pTablePart;
}

/**
 * Dtor.
 */
GeoreferenceRefSysPart::~GeoreferenceRefSysPart() {
}

/**
 * (METODO NO SOPORTADO)
 */
std::vector<LibraryItemAttribute*>*
GeoreferenceRefSysPart::CreateAttributesFromFields() {
   return NULL;
}

/**
 * Establece el SRS del part.
 */
void GeoreferenceRefSysPart::SetSrs(const std::string& Srs) {
   if (pCgu_ != NULL && !Srs.empty()) {
      std::string aid = SpatialReference::GetAuthorityId(Srs);
      pCgu_->SelectItemByPrincipal(aid);
      SourceSrsChangeNotification notification(Srs);
      pTablePart_->Notify(&notification);
   }
}

/**
 * Metodo de configuracion particular para el part.
 *
 * @return true Si la conf se realizo con exito.
 */
bool GeoreferenceRefSysPart::ConfigureFeatures() {
   return true;
}

/**
 * Actualiza la informacion en el part.
 *
 * \todo (danieltesta - #3392): verificar si hace falta implementar
 * este metodo. Tiene que estar porque es abstracto puro.
 */
void GeoreferenceRefSysPart::UpdateFields(const LibraryItem* pItem, bool Modifiable) {
   const LibraryItemAttribute* attr = pItem->GetAttribute("WKT");
   SourceSrsChangeNotification notification(attr->GetValue());
   pTablePart_->Notify(&notification);
}

}  // namespace suri
