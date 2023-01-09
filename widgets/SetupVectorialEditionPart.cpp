/*
 * SetupVectorialEdition.cpp
 *
 *  Created on: 27/03/2014
 *      Author: MauroPisano
 */

// Includes standard
#include <sstream>
// Includes suri
#include "SetupVectorialEditionPart.h"
#include "resources.h"
#include "suri/messages.h"
#include "suri/Configuration.h"
#include "SimpleVectorStylePart.h"
#include "suri/VectorStyle.h"
#include "suri/LibraryManagerFactory.h"
// Includes wx
#include "wx/xrc/xmlres.h"
#include "wx/wx.h"
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"
#include "wx/choicebk.h"


#define DEFAULT_STYLE_STYLE "VECTORSTYLE[3,\"Poligono rojo\",BRUSH[\"Suri-brush\",1,COLOR[255,0,0,255]],PEN[\"Suri-pen\",1,COLOR[1,1,1,255],WIDTH[1]],SYMBOL[\"Suri-symbol\",0,COLOR[0,0,0,255],SIZE[1]],LABEL[\"Suri-label\",0,EXPRESSION[],COLOR[0,0,0,255],BACKCOLOR[0,0,0,0],SIZE[8],ITALIC[0],BOLD[0],UNDERLINED[0],ANGLE[0],OFFSET[0,0],ANCHOR[2]]]"

namespace suri {

SetupVectorialEditionPart::SetupVectorialEditionPart(DataViewManager* pDataViewManager):
      pDataViewManager_(pDataViewManager) {
   // TODO Auto-generated constructor stub
   windowTitle_ = wxT(window_title_VECTORIAL_EDITION);
   parameters_.push_back("geo_creation_style");
   parameters_.push_back("geo_deleting_style");
   parameters_.push_back("geo_edition_style");
   parameters_.push_back("geo_selection_style");
   panels_.push_back("ID_CREATION_GEOMETRY");
   panels_.push_back("ID_DELETING_GEOMETRY");
   panels_.push_back("ID_EDITION_GEOMETRY");
   panels_.push_back("ID_SELECTION_GEOMETRY");
}

SetupVectorialEditionPart::~SetupVectorialEditionPart() {
   // TODO Auto-generated destructor stub
}

/** Crea el part (el interior de la ventana)*/
bool SetupVectorialEditionPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_VECTORIAL_EDITION_STYLE"));

   if (!pToolWindow_) return false;
   std::string parameters[] = { "geo_creation_style", "geo_deleting_style",
                                "geo_edition_style", "geo_selection_style" };

   std::string parameters_defaults[] = { DEFAULT_GEO_CREATION_STYLE,
                                         DEFAULT_GEO_DELETING_STYLE,
                                         DEFAULT_GEO_EDITION_STYLE,
                                         DEFAULT_GEO_SELECTION_STYLE };

   std::string panels[] = { "ID_CREATION_GEOMETRY", "ID_DELETING_GEOMETRY",
                            "ID_EDITION_GEOMETRY", "ID_SELECTION_GEOMETRY" };

   std::string* wktstyles[] = { &creationStyle_, &deletingStype_, &editionStyle_,
                                &selectionStyle_ };

   LibraryClient client(pDataViewManager_->GetLibraryManager());

   const Library* plib = client.GetLibraryByCode(
         LibraryManagerFactory::VectorstyleLibraryCode);

   for (size_t i = 0; i < parameters_.size(); ++i) {
      *(wktstyles[i]) = Configuration::GetParameter(parameters[i],
                                                    parameters_defaults[i]);

      if (wktstyles[i]->empty())
         *(wktstyles[i]) = DEFAULT_STYLE_STYLE;

      ui::SimpleVectorStylePart* pstylepart = new ui::SimpleVectorStylePart(
            wktstyles[i], pDataViewManager_->GetLibraryManager(), plib->GetId());
      pstylepart->SetVectorStyleWkt(*(wktstyles[i]));

      AddControl(pstylepart->GetWidget(), wxT(panels[i].c_str()));

      styles_.push_back(pstylepart);
   }
   modified_ = false;
   return true;
}

void SetupVectorialEditionPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_GENERIC_PROPERTIES, ToolBitmap);
}
/** Inicializa los controles */
void SetupVectorialEditionPart::SetInitialValues() {
   for (size_t s = 0; s < styles_.size(); ++s)
      styles_.at(s)->SetInitialValues();
   modified_ = false;
}
/** Indica si el Part tiene cambios para salvar. */
bool SetupVectorialEditionPart::HasChanged() {
   size_t selection = XRCCTRL(*pToolWindow_, "ID_STYLE_CHOICEBOOK", wxChoicebook)->GetSelection();
   return selection > styles_.size() ? false : styles_.at(selection)->HasChanged();
}

/** Salva los cambios realizados en el Part. */
bool SetupVectorialEditionPart::CommitChanges() {
   WriteNewStyles();
   modified_ = false;
   return true;
}

/** Restaura los valores originales del Part. */
bool SetupVectorialEditionPart::RollbackChanges() {
   SetInitialValues();
   modified_ = false;
   return true;
}
/** Actualiza el estado de la parte */
void SetupVectorialEditionPart::Update() {
   return;
}
/** Actualiza las opciones de configuracion en el archivo xml. */
void SetupVectorialEditionPart::WriteNewStyles() {
  /** Escribimos los diferentes estilos en el archivo de configuracion */
   for (size_t x = 0; x<parameters_.size(); x++){
      Configuration::SetParameter(parameters_[x],styles_[x]->GetVectorStyle()->GetWkt());
   }
   return;
}


} /* namespace suri */
