/** \file GeoreferenceRefSysPart.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
