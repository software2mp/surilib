/*! \file BasicLibraryItemSelector.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef BASICLIBRARYITEMSELECTOR_H_
#define BASICLIBRARYITEMSELECTOR_H_

// Includes Estandar
// Includes Suri
#include "LibraryItemEditorPart.h"
#include "LibraryItemSelectionInterface.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/* Clase abstracta de tal forma que a traves de ella cualquier otra clase pueda incluir las
 * bibliotecas y capturar su comportamiento */

class BasicLibraryItemSelector : public LibraryItemEditorPart {
public:
   /* Constructor */
   BasicLibraryItemSelector(
         LibraryItemSelectionInterface* pItemSelector,
         const SuriObject::UuidType& LibraryId,
         LibraryManager* pManager,
         const std::string& WxWindowdId = "ID_LIBRARY_SELECTION_PANEL",
         const std::string& WxCguPanelId = "ID_LIBRARY_SELECTION_PANEL",
         bool FilterFavorites = false,
         const UniversalGraphicalComponentPartInterface::FeatureCombinationEnum& Feature =
               UniversalGraphicalComponentPartInterface::EditionWithFavorites,
               LibraryManager::AccessType Access = LibraryManager::ALL);
   /* Destructor */
   virtual ~BasicLibraryItemSelector();
   /**
    * Metodo auxiliar privado que es necesario implementar en las clases hijas.
    * Este metodo es el encargado de actualizar realmente la informacion en el part
    * Es llamado en el metodo SetActiveItem a modo de Template Method
    **/
   void UpdateFields(const LibraryItem* pItem, bool Modifiable = true);

protected:
   /* Puntero a LibraryItemSelector */
   LibraryItemSelectionInterface* pItemSelector_;
};

} /** namespace suri */

#endif /* BASICLIBRARYITEMSELECTOR_H_ */
