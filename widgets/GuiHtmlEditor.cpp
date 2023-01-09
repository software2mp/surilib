/** \file wxGuiFunctions.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <string>

// Includes suri
#include "GuiHtmlEditor.h"
#include "logmacros.h"

// Includes wx
#include "wx/dcmemory.h"

namespace suri {

/** Ctor */
GuiHtmlEditor::GuiHtmlEditor(const std::string &MainBody) {
   html_ = MainBody;
}

/** Dtor */
GuiHtmlEditor::~GuiHtmlEditor() {
}

/**
 * Remplaza un TAG en html con texto que recibe por parametro
 * @param[in] TagName nombre del tag que quiero remplazar
 * @param[in] HtmlText Texto por el que quiero remplazar el tag.
 */
bool GuiHtmlEditor::ReplaceTag(const std::string &TagName, const std::string HtmlText) {
   size_t position = html_.find(TagName.c_str());
   if (position == std::string::npos)
      return false;
   html_.replace(position, TagName.size(), HtmlText.c_str());
   return true;
}

/** Retorna Html */
std::string GuiHtmlEditor::GetHtml() {
   REPORT_DEBUG("NODO GRUPO: %s", html_.c_str());
   return html_;
}

/**
 * Modifica Text para que entre en ventana de longitud WindowWidth
 * @param[in] Text cadena de caracteres que quiero graficar
 * @param[in] WindowWidth Ancho en pixels de la ventana donde quiero graficar
 * @return cadena de texto que entra en WindowWidth
 */
std::string GuiHtmlEditor::GetSizedText(const std::string &Text, int WindowWidth) {
   if (WindowWidth <= 0) {
      return "";
   }

   wxMemoryDC *pdc = new wxMemoryDC();

   size_t index = 1;
   std::string sizedtext = Text;
   int sizedtextlenght = pdc->GetTextExtent(sizedtext).GetWidth();
   while (sizedtextlenght > WindowWidth && index < Text.size()) {
      sizedtext = "...";
      sizedtext += Text.substr(index);
      sizedtextlenght = pdc->GetTextExtent(sizedtext).GetWidth();
      index++;
   }

   delete pdc;
   return sizedtext;
}

} /* namespace suri */
