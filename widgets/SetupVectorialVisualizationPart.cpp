/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

#include <vector>
#include "SetupVectorialVisualizationPart.h"
#include "resources.h"
#include "suri/Configuration.h"
#include "wx/choicebk.h"

#define SOLID_FILL_LINE_STYLE \
   "VECTORSTYLE[2,\"Linea celeste\",PEN[\"Suri-pen\",1,COLOR[0,0,255,255],WIDTH[1]],SYMBOL[\"Suri-symbol\",0,COLOR[0,0,0,255],SIZE[1]],LABEL[\"Suri-label\",0,EXPRESSION[],COLOR[0,0,0,255],BACKCOLOR[0,0,0,0],SIZE[8],ITALIC[0],BOLD[0],UNDERLINED[0],ANGLE[0],OFFSET[0,0],ANCHOR[4]]]"

namespace suri {

SetupVectorialVisualizationPart::SetupVectorialVisualizationPart() {
   windowTitle_ = wxT(window_title_VECTORIAL_VISUALIZATION);
}

SetupVectorialVisualizationPart::~SetupVectorialVisualizationPart() {
}

/** Crea el part (el interior de la ventana)*/
bool SetupVectorialVisualizationPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
         wxT("ID_VISUALIZATION_STYLE"));
   if (!pToolWindow_) return false;

   std::string parameters[] = { "geo_polygon_style", "geo_line_style",
         "geo_point_style" };
   std::string panels[] = { "ID_POLYGON_VECTOR_STYLE_PANEL","ID_LINE_VECTOR_STYLE_PANEL",
                           "ID_POINT_VECTOR_STYLE_PANEL" };
   std::string* wktstyles[] = { &PolygonStyle_,&LineStyle_,&PointStyle_};

   for ( size_t i = 0; i < 3; ++i) {
      *(wktstyles[i]) = Configuration::GetParameter(parameters[i], "");
      if((*wktstyles[i]).empty() )
         (*wktstyles[i]) = SOLID_FILL_LINE_STYLE;
      ui::SimpleVectorStylePart* pstylepart = new ui::SimpleVectorStylePart(wktstyles[i]);
      pstylepart->SetVectorStyleWkt(*(wktstyles[i]));
      AddControl(pstylepart->GetWidget(), wxT(panels[i].c_str()));
      pStyles_.push_back(pstylepart);
      pstylepart->SetInitialValues();

   }

   modified_ = false;
   return true;
}
/** Asigna el icono para el part.  */
void SetupVectorialVisualizationPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_GENERIC_PROPERTIES, ToolBitmap);
}
/** Inicializa los controles */
void SetupVectorialVisualizationPart::SetInitialValues() {
   modified_ = false;
}
/** Indica si el Part tiene cambios para salvar. */
bool SetupVectorialVisualizationPart::HasChanged() {
   size_t selection = XRCCTRL(*pToolWindow_, "ID_STYLE_CHOICEBOOK", wxChoicebook)->GetSelection();
   return selection > pStyles_.size() ? false : pStyles_.at(selection)->HasChanged();
}
/** Salva los cambios realizados en el Part. */
bool SetupVectorialVisualizationPart::CommitChanges() {
   WriteNewStyles();
   return false;
}
/** Restaura los valores originales del Part. */
bool SetupVectorialVisualizationPart::RollbackChanges() {
   SetInitialValues();
   return true;
}
/** Actualiza el estado de la parte */
void SetupVectorialVisualizationPart::Update() {
}

/** Actualiza las opciones de configuración en el archivo xml. */
void SetupVectorialVisualizationPart::WriteNewStyles() {
  /** Escribimos los diferentes estilos en el archivo de configuración */
   Configuration::SetParameter("geo_polygon_style", PolygonStyle_);
   Configuration::SetParameter("geo_line_style", LineStyle_);
   Configuration::SetParameter("geo_point_style", PointStyle_);
}

} /* namespace suri */
