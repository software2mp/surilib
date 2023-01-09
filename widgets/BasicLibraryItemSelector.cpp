/*! \file BasicLibraryItemSelector.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "suri/BasicLibraryItemSelector.h"
#include "suri/LibraryManager.h"
// Includes Wx
// Defines
// forwards

namespace suri {
/* Constructor */
BasicLibraryItemSelector::BasicLibraryItemSelector(
      LibraryItemSelectionInterface* pItemSelector,
      const SuriObject::UuidType& LibraryId,
      LibraryManager* pManager,
      const std::string& WxWindowdId,
      const std::string& WxCguPanelId,
      bool FilterFavourites,
      const UniversalGraphicalComponentPartInterface::FeatureCombinationEnum& Feature,
            LibraryManager::AccessType Access) :
      LibraryItemEditorPart(LibraryId, pManager, WxWindowdId, WxCguPanelId, FilterFavourites,
                                        Feature, Access), pItemSelector_(pItemSelector) {
}

/* Destructor */
BasicLibraryItemSelector::~BasicLibraryItemSelector() {
}

/**
 * Metodo auxiliar privado que es necesario implementar en las clases hijas.
 * Este metodo es el encargado de actualizar realmente la informacion en el part
 * Es llamado en el metodo SetActiveItem a modo de Template Method
 **/
void BasicLibraryItemSelector::UpdateFields(const LibraryItem* pItem, bool Modifiable) {
   pItemSelector_->SetActiveItem(pItem);
}

} /** namespace suri */
