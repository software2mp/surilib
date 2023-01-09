/*! \file SetupMeasurementPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes suri
#include "suri/messages.h"
#include "suri/LibraryManagerFactory.h"
#include "suri/Configuration.h"
#include "SetupMeasurementPart.h"
#include "resources.h"
// Includes wx
#include "wx/clrpicker.h"
#include "wx/choicebk.h"


#define SOLID_POLYGON_STYLE \
   "VECTORSTYLE[3,\"Poligono azul\",BRUSH[\"Suri-brush\",1,COLOR[255,0,0,255]],PEN[\"Suri-pen\",1,COLOR[1,1,1,255],WIDTH[1]],SYMBOL[\"Suri-symbol\",0,COLOR[0,0,0,255],SIZE[1]],LABEL[\"Suri-label\",0,EXPRESSION[],COLOR[0,0,0,255],BACKCOLOR[0,0,0,0],SIZE[8],ITALIC[0],BOLD[0],UNDERLINED[0],ANGLE[0],OFFSET[0,0],ANCHOR[4]]]"

namespace suri {

SetupMeasurementPart::SetupMeasurementPart(DataViewManager* pDataViewManager):pDataViewManager_(pDataViewManager) {
   // TODO Auto-generated constructor stub
   windowTitle_ = wxT(window_title_MEASUREMENT);
   parameters_.push_back("geo_area_style");
   parameters_.push_back("geo_distance_style");
   panels_.push_back("ID_AREA_STYLE");
   panels_.push_back("ID_DISTANCE_STYLE");
}

SetupMeasurementPart::~SetupMeasurementPart() {
   // TODO Auto-generated destructor stub
}

/** Crea el part (el interior de la ventana) */
bool SetupMeasurementPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_, wxT("ID_MEASUREMENT_STYLE"));

   if (!pToolWindow_)
      return false;


   std::string* wktstyles[]      = {&areaStyle_, &distanceStyle_};
   LibraryClient client(pDataViewManager_->GetLibraryManager());
      const Library* plib = client.GetLibraryByCode(
             LibraryManagerFactory::VectorstyleLibraryCode);
   for(int i = 0; i < 2; ++i) {
      *(wktstyles[i]) = Configuration::GetParameter(parameters_[i], "");
      if((*wktstyles[i]).empty() )
         (*wktstyles[i]) = SOLID_POLYGON_STYLE;
      ui::SimpleVectorStylePart* pstylepart = new ui::SimpleVectorStylePart(wktstyles[i],
                                           pDataViewManager_->GetLibraryManager(), plib->GetId());
      pstylepart->SetVectorStyleWkt(*(wktstyles[i]));
      AddControl(pstylepart->GetWidget(), wxT(panels_[i].c_str()));
      pStyles_.push_back(pstylepart);
      pstylepart->SetInitialValues();
   }

   modified_ = false;
   return true;
}
/** Asigna el icono para el part.  */
void SetupMeasurementPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_GENERIC_PROPERTIES, ToolBitmap); // Agregar el icono indicado.
}
/** Inicializa los controles */
void SetupMeasurementPart::SetInitialValues() {
   for (size_t s = 0; s < pStyles_.size(); ++s)
      pStyles_.at(s)->SetInitialValues();
}
/** Indica si el Part tiene cambios para salvar. */
bool SetupMeasurementPart::HasChanged() {
   size_t selection = XRCCTRL(*pToolWindow_, "ID_STYLE_CHOICEBOOK", wxChoicebook)->GetSelection();
   return selection > pStyles_.size() ? false : pStyles_.at(selection)->HasChanged();
}
/** Salva los cambios realizados en el Part. */
bool SetupMeasurementPart::CommitChanges() {
   WriteNewStyles();

   modified_ = false;
   return true;
}
/** Restaura los valores originales del Part. */
bool SetupMeasurementPart::RollbackChanges() {
   SetInitialValues();
   return false;
}
/** Actualiza el estado de la parte */
void SetupMeasurementPart::Update() {
   return;
}

/** Actualiza las opciones de configuracion en el archivo xml. */
void SetupMeasurementPart::WriteNewStyles() {
  /** Escribimos los diferentes estilos en el archivo de configuracion */
   for (size_t x = 0; x<parameters_.size(); x++){
        Configuration::SetParameter(parameters_[x], pStyles_[x]->GetVectorStyle()->GetWkt());
   }
   return;
}


} /* namespace suri */
