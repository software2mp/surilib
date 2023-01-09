/*! \file ProjectionSystemEditionPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#include "ProjectionSystemEditionPart.h"

namespace suri {
/** Nombre del XRC que representa la ventana. */
const std::string ProjectionSystemEditionPart::PROJECTION_SYSTEM_EDITION_XRC_NAME =
      wxT("ID_PROJECTION_SYSTEM_EDITION_PANEL");

/** Ctor. Carga el XRC de la pantalla, e instancia los subpart */
ProjectionSystemEditionPart::ProjectionSystemEditionPart() :
      PartCollection(PROJECTION_SYSTEM_EDITION_XRC_NAME) {

   Part *pnameeditor = new UniversalLibraryNameEditionPart(
         UniversalLibraryNameEditionPart::SHOW_FAVOURITES);
   AddPart(pnameeditor, wxT("ID_PROJECTED_SYSTEM_NAME_PANEL"));
}

/** Dtor. */
ProjectionSystemEditionPart::~ProjectionSystemEditionPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool ProjectionSystemEditionPart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool ProjectionSystemEditionPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool ProjectionSystemEditionPart::RollbackChanges() {
   return true;
}

/** Inicializa el part */
void ProjectionSystemEditionPart::SetInitialValues() {
}

/** Actualiza el estado de la parte */
void ProjectionSystemEditionPart::Update() {
}
}  // end namespace suri

