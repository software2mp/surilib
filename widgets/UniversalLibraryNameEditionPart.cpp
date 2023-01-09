/*! \file UniversalLibraryNameEditionPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "UniversalLibraryNameEditionPart.h"

namespace suri {
const std::string UniversalLibraryNameEditionPart::LIBRARY_NAME_EDITION_XRC_NAME =
      wxT("ID_LIBRARY_NAME_EDITION_PANEL");

UniversalLibraryNameEditionPart::UniversalLibraryNameEditionPart(int flags) :
      Part(LIBRARY_NAME_EDITION_XRC_NAME) {
   representationflags_ = flags;
}

UniversalLibraryNameEditionPart::~UniversalLibraryNameEditionPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool UniversalLibraryNameEditionPart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool UniversalLibraryNameEditionPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool UniversalLibraryNameEditionPart::RollbackChanges() {
   return true;
}

/** Inicializa el part */
void UniversalLibraryNameEditionPart::SetInitialValues() {
}

/** Actualiza el estado de la parte */
void UniversalLibraryNameEditionPart::Update() {
}

/** Permite a clases que hereden modificar el widget luego de crear la ventana */
bool UniversalLibraryNameEditionPart::ConfigureWidget() {
   if ((representationflags_ & SHOW_NAME) == 0) {
      pWidget_->Hide("ID_NAME_EDITION_TAG");
   }

   if ((representationflags_ & SHOW_DELETE_BUTTON) == 0) {
      pWidget_->Hide("ID_DELETE_ITEM_BUTTON");
   }

   if ((representationflags_ & SHOW_FAVOURITES) == 0) {
      pWidget_->Hide("ID_SHOW_FAVORITES_CHECK");
   }

   if ((representationflags_ & READ_ONLY) != 0) {
      pWidget_->Disable("ID_SHOW_FAVORITES_CHECK");
      pWidget_->Disable("ID_NAME_EDITION_COMBO");
   }

   return true;
}
}
