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
