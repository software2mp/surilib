/*! \file MapToolSetupPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>

// Includes suri
#include "suri/MapToolSetupPart.h"
#include "resources.h"
#include "suri/Configuration.h"
#include "suri/AuxiliaryFunctions.h"
#include "MapRenderer.h"

// Includes wx
#include "wx/filepicker.h"
#include "wx/fontpicker.h"
#include "wx/clrpicker.h"

// Defines
#define NORTH_FILE_CTRL_ID "ID_NORTH_ICON_FILEPICKCTRL"
#define LEGEND_FONT_CTRL_ID "ID_LEGEND_FONTCTRL"
#define LEGEND_TITLE_CTRL_ID "ID_LEGEND_TITLE_TEXT"
#define LEGEND_BORDER_COLOR_CTRL_ID "ID_LEGEND_BORDER_COLOR_CTRL"
#define LEGEND_BORDER_BCK_CTRL_ID "ID_LEGEND_BACKGROUND_COLOR_CTRL"

/** Tipo de letra del titulo de la leyenda */
#define LEGEND_FONT_STYLE wxFONTSTYLE_NORMAL
#define LEGEND_TITLE_FONT_FAMILY LEGEND_FONT_FAMILY
/** Tipo de letra de la leyenda */
#define LEGEND_FONT_FAMILY wxFONTFAMILY_DEFAULT
/** Color de fondo de la leyenda (canal red) */
#define LEGEND_BACKGROUND_RED 255
/** Color de fondo de la leyenda (canal green) */
#define LEGEND_BACKGROUND_GREEN 255
/** Color de fondo de la leyenda (canal blue) */
#define LEGEND_BACKGROUND_BLUE 255
/** Ancho de la fuente de la leyenda */
#define LEGEND_FONT_WEIGHT wxFONTWEIGHT_NORMAL

namespace suri {

/** Ctor. */
MapToolSetupPart::MapToolSetupPart(DataViewManager* pDataViewManager) {
   windowTitle_ = _(caption_MAP_TOOL_PROPERTIES);
   pDataViewManager_ = pDataViewManager;
}

/** Destructor. */
MapToolSetupPart::~MapToolSetupPart() {
}

/** Crea el part (el interior de la ventana)*/
bool MapToolSetupPart::CreateToolWindow() {
   if (pToolWindow_)
      return false;
   pToolWindow_ =
         wxXmlResource::Get()->LoadPanel(pParentWindow_, wxT("ID_MAPTOOLSETUP_PANEL"));
   return true;
}

/** Asigna el icono para el part.  */
void MapToolSetupPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_GENERIC_PROPERTIES, ToolBitmap);
}

/** Inicializa los controles */
void MapToolSetupPart::SetInitialValues() {
   // configuracion inicial para texto del titulo
   std::string titletext = Configuration::GetParameter(MapRenderer::MapLegendTitleConfigTag, "");
   titleText_ = titletext.empty() ? label_LEGEND : titletext;
   XRCCTRL(*pToolWindow_, LEGEND_TITLE_CTRL_ID, wxTextCtrl)->SetValue(titleText_);
   nativeFontInfo_ = Configuration::GetParameter(MapRenderer::MapLegendNativeFontInfo, "");
   // configuracion del font picker
   wxFont font;
   if (!nativeFontInfo_.empty())
      font.SetNativeFontInfo(nativeFontInfo_);
   if ( font.IsOk() )
      XRCCTRL(*pToolWindow_, LEGEND_FONT_CTRL_ID, wxFontPickerCtrl)->SetSelectedFont(font);
   // configuracion inicial para el color de borde de la leyenda
   std::string bordercolour = Configuration::GetParameter
         (MapRenderer::MapLegendBorderConfigTag, "");
   legendBorderColour_ = bordercolour.empty() ? wxColour(1, 1, 1).GetAsString() :
                        wxColour(bordercolour).GetAsString();
   GET_CONTROL(*pToolWindow_, LEGEND_BORDER_COLOR_CTRL_ID,
            wxColourPickerCtrl)->SetColour(legendBorderColour_);
   // configuracion inicial del color de fondo para la leyenda
   std::string backgroundcolor = Configuration::GetParameter
                                                (MapRenderer::MapLegendBackgroundConfigTag, "");
   legendBackgroundColour_ = backgroundcolor.empty() ?
               wxColour(LEGEND_BACKGROUND_RED, LEGEND_BACKGROUND_GREEN,
                        LEGEND_BACKGROUND_BLUE).GetAsString() :
                           wxColour(backgroundcolor).GetAsString();
   GET_CONTROL(*pToolWindow_, LEGEND_BORDER_BCK_CTRL_ID,
               wxColourPickerCtrl)->SetColour(legendBackgroundColour_);
   // configuracion inicial para el icono asociado al norte del mapa
   std::string northfilename = Configuration::GetParameter(MapRenderer::MapNorthIconConfigTag, "");
   northPath_ = northfilename;
   if (!northfilename.empty())
      GET_CONTROL(*pToolWindow_, NORTH_FILE_CTRL_ID, wxFilePickerCtrl)->SetPath(northPath_); 
}

/** Indica si el Part tiene cambios para salvar. */
bool MapToolSetupPart::HasChanged() {
   wxFont font = XRCCTRL(*pToolWindow_, LEGEND_FONT_CTRL_ID, wxFontPickerCtrl)->GetSelectedFont();
   wxString actualFontInfo;
   if (font.IsOk()) {
      actualFontInfo = font.GetNativeFontInfoDesc();
   }
   bool fontchanged = actualFontInfo != nativeFontInfo_;
   if (fontchanged) {
      nativeFontInfo_ = actualFontInfo;
   }
   std::string northpath = USE_CONTROL(*pToolWindow_, NORTH_FILE_CTRL_ID,
                           wxFilePickerCtrl, GetPath(), "").c_str();
   std::string titletext =
      USE_CONTROL(*pToolWindow_, LEGEND_TITLE_CTRL_ID, wxTextCtrl, GetValue(), "").c_str();
   std::string bordercolour = USE_CONTROL(*pToolWindow_, LEGEND_BORDER_COLOR_CTRL_ID,
                           wxColourPickerCtrl, GetColour(), "").GetAsString().c_str();
   std::string backgroundcolor = USE_CONTROL(*pToolWindow_, LEGEND_BORDER_BCK_CTRL_ID,
                           wxColourPickerCtrl, GetColour(), "").GetAsString().c_str();

   return fontchanged || northpath.compare(northPath_) != 0 || titletext.compare(titleText_) != 0
            || bordercolour.compare(legendBorderColour_) != 0
            || backgroundcolor.compare(legendBackgroundColour_) !=0;
}

/** Salva los cambios realizados en el Part. */
bool MapToolSetupPart::CommitChanges() {
   std::string titletext =
      USE_CONTROL(*pToolWindow_, LEGEND_TITLE_CTRL_ID, wxTextCtrl, GetValue(), "").c_str();
   Configuration::SetParameter(MapRenderer::MapLegendTitleConfigTag, titletext);

   wxFontPickerCtrl* pfontpicker = XRCCTRL(*pToolWindow_, LEGEND_FONT_CTRL_ID, wxFontPickerCtrl);
   wxFont font = pfontpicker->GetSelectedFont();
   Configuration::SetParameter(MapRenderer::MapLegendNativeFontInfo, nativeFontInfo_.c_str());
   std::string bordercolour =
      USE_CONTROL(*pToolWindow_, LEGEND_BORDER_COLOR_CTRL_ID, wxColourPickerCtrl,
                  GetColour(), "").GetAsString().c_str();
   Configuration::SetParameter(MapRenderer::MapLegendBorderConfigTag, bordercolour);
   std::string backgroundcolor =
      USE_CONTROL(*pToolWindow_, LEGEND_BORDER_BCK_CTRL_ID, wxColourPickerCtrl,
                  GetColour(), "").GetAsString().c_str();
   Configuration::SetParameter(MapRenderer::MapLegendBackgroundConfigTag, backgroundcolor);

   std::string northfilename =
      USE_CONTROL(*pToolWindow_, NORTH_FILE_CTRL_ID, wxFilePickerCtrl, GetPath(), "").c_str();
   if (!northfilename.empty())
      Configuration::SetParameter(MapRenderer::MapNorthIconConfigTag, northfilename);
   if (pDataViewManager_) {
      ViewcontextInterface* pview = pDataViewManager_->GetViewcontextManager()->
                                                                     GetSelectedViewcontext();
      LayerList* player = pview->GetLayerList();
      std::vector<Element*> elements = player->GetRenderizationOrderList();
      if (elements.size() > 0) {
         elements[0]->SetChanged();
         elements[0]->SendViewerUpdate();
      }
   }
   return true;
}

/** Restaura los valores originales del Part. */
bool MapToolSetupPart::RollbackChanges() {
   SetInitialValues();
   return true;
}

}  /** namespace suri **/

