/** \file GeneralPixelInformationPart.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <sstream>
#include <cstdio>

#include "GeneralPixelInformationPart.h"
#include "suri/ParameterCollection.h"

namespace suri {

const std::string GeneralPixelInformationPart::kGeneralPixelInformationXrcName =
      wxT("ID_GENERAL_PIXEL_INFORMATION_PART");
const std::string GeneralPixelInformationPart::kGeneralPixelInformationTitle =
      "General";

/**
 * Ctor
 */
GeneralPixelInformationPart::GeneralPixelInformationPart(ParameterCollection& Params) :
      Part(kGeneralPixelInformationXrcName, kGeneralPixelInformationTitle),
      layersCount_(0), layersWlCount_(0), rasterCount_(0), vectorCount_(0) {
   Params.GetValue<int>("LayersCount", layersCount_);
   Params.GetValue<int>("LayersHlCount", layersWlCount_);
   Params.GetValue<int>("RasterCount", rasterCount_);
   Params.GetValue<int>("VectorCount", vectorCount_);
}

/**
 * Dtor
 */
GeneralPixelInformationPart::~GeneralPixelInformationPart() {
}

/**
 * Indica si el Part tiene cambios para salvar.
 */
bool GeneralPixelInformationPart::HasChanged() {
   return false;
}

/**
 * Salva los cambios realizados en el Part.
 */
bool GeneralPixelInformationPart::CommitChanges() {
   return true;
}

/**
 * Restaura los valores originales del Part.
 */
bool GeneralPixelInformationPart::RollbackChanges() {
   return true;
}

/**
 * Inicializa el Part.
 */
void GeneralPixelInformationPart::SetInitialValues() {
   char buff[11] = { 0 };
   wxWindow* pwin = this->GetWidget()->GetWindow();

   sprintf(buff, "%d", layersCount_);
   wxStaticText* pstatic = XRCCTRL(*pwin, wxT("ID_GENERAL_PINFO_LAYERS_COUNT"),
                                   wxStaticText);
   pstatic->SetLabel(wxT(buff));

   sprintf(buff, "%d", layersWlCount_);
   pstatic = XRCCTRL(*pwin, wxT("ID_GENERAL_PINFO_LAYERS_WH_COUNT"), wxStaticText);
   pstatic->SetLabel(wxT(buff));

   sprintf(buff, "%d", rasterCount_);
   pstatic = XRCCTRL(*pwin, wxT("ID_GENERAL_PINFO_RASTER_COUNT"), wxStaticText);
   pstatic->SetLabel(wxT(buff));

   sprintf(buff, "%d", vectorCount_);
   pstatic = XRCCTRL(*pwin, wxT("ID_GENERAL_PINFO_VECTOR_COUNT"), wxStaticText);
   pstatic->SetLabel(wxT(buff));
}

/**
 * Actualiza el estado del Part.
 */
void GeneralPixelInformationPart::Update() {
}

}  // namespace suri
