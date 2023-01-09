/*! \file DualViewerSelectionPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
// Includes Suri
#include "DualViewerSelectionPart.h"
// Includes Wx

// Includes App

// Defines

/** Genera un part que permite editar las opciones de algebra de bandas */
/** Namespace suri */
namespace suri {
const std::string DualViewerSelectionPart::DUAL_VIEWER_SELECTION_XRC_NAME =
      wxT("ID_DUAL_VIEWER_SELECTION_PANEL"); /*! Id del panel */
/** Ctor */
DualViewerSelectionPart::DualViewerSelectionPart() :
      Part(DualViewerSelectionPart::DUAL_VIEWER_SELECTION_XRC_NAME,
           wxT(caption_DUAL_VIEWER_PART)) {
}
/** Dtor */
DualViewerSelectionPart::~DualViewerSelectionPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool DualViewerSelectionPart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool DualViewerSelectionPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool DualViewerSelectionPart::RollbackChanges() {
   return true;
}

/** Inicializa el part */
void DualViewerSelectionPart::SetInitialValues() {
}

/** Actualiza el estado de la parte */
void DualViewerSelectionPart::Update() {
}
}  // namespace suri
