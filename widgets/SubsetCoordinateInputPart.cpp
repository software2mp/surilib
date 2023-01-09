/*! \file SubsetCoordinateInputPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "SubsetCoordinateInputPart.h"

// Includes standard
// Includes Suri
// Includes Wx
// Defines

/** namespace suri */
namespace suri {
const std::string SubsetCoordinateInputPart::SUBSET_COORDINATE_XRC_NAME =
      wxT("ID_SUBSET_COORD_INPUT_PANEL");

SubsetCoordinateInputPart::SubsetCoordinateInputPart() :
      PartCollection(SUBSET_COORDINATE_XRC_NAME) {
   pULDegreeCorner_ = new LatitudeLongitudeInputPart();
   pLRDegreeCorner_ = new LatitudeLongitudeInputPart();
   pULDecimalCorner_ = new DecimalCoordinateInputPart();
   pLRDecimalCorner_ = new DecimalCoordinateInputPart();
   AddPart(pULDegreeCorner_, wxT("ID_UL_DEGREE_COORD_PANEL"));
   AddPart(pLRDegreeCorner_, wxT("ID_LR_DEGREE_COORD_PANEL"));
   AddPart(pULDecimalCorner_, wxT("ID_UL_DECIMAL_COORD_PANEL"));
   AddPart(pLRDecimalCorner_, wxT("ID_LR_DECIMAL_COORD_PANEL"));
}

SubsetCoordinateInputPart::~SubsetCoordinateInputPart() {
}

bool SubsetCoordinateInputPart::SetInternalSubset(const Subset &SubsetIn) {
   // seteo coordenadas en controles.
   if (!pLRDegreeCorner_->SetCoordinate(SubsetIn.lr_)
         || !pULDegreeCorner_->SetCoordinate(SubsetIn.ul_)) {
      return false;
   }

   return true;
}

/** Indica si el Part tiene cambios para salvar. */
bool SubsetCoordinateInputPart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool SubsetCoordinateInputPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool SubsetCoordinateInputPart::RollbackChanges() {
   return true;
}

/** Inicializa el part */
void SubsetCoordinateInputPart::SetInitialValues() {
}

/** Actualiza el estado de la parte */
void SubsetCoordinateInputPart::Update() {
}

/** Permite a clases que hereden modificar el widget luego de crear la ventana */
bool SubsetCoordinateInputPart::ConfigureWidget() {
   PartCollection::ConfigureWidget();

   return true;
}
}
