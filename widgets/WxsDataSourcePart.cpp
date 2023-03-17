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

#include "WxsDataSourcePart.h"

// Includes standard

// Includes Suri
#include "suri/messages.h"
#include "WxsElement.h"
#include "suri/Uri.h"
#include "XrcWidget.h"
#include "suri/AuxiliaryFunctions.h"
// Includes Wx

// Includes App

// Defines

#define URL_LAYERS_FIELD_NAME "layers"

// Valores default hasta que existan las bibliotecas.
#define URL_INITIAL_VALUE "http://demo.mapserver.org/cgi-bin/wms"
#define LAYERS_INITIAL_VALUE "bluemarble"

namespace suri {
/** Ctor */
WxsDataSourcePart::WxsDataSourcePart(bool Enable, bool Modified) :
      Part(wxT("ID_WXS_DATA_SOURCE_PANEL"), _(caption_WMS_SOURCE), Enable, Modified),
      pElement_(NULL), url_(""), layers_(""), description_(""), widgetUrl_(""),
      widgetLayers_(""), widgetDescription_("") {
   url_ = URL_INITIAL_VALUE;
   layers_ = LAYERS_INITIAL_VALUE;
}

WxsDataSourcePart::WxsDataSourcePart(Element* pElement, bool Enable, bool Modified) :
      Part(wxT("ID_WXS_VISUALIZATION_PANEL"), _(caption_WMS_VISUALIZATION), Enable,
           Modified) {
   pElement_ = pElement;
   Uri uri(pElement_->GetUrl().c_str());
   std::map<std::string, std::string> queryparameters = suri::StringToMap(
         uri.GetQuery(), Uri::QUERY_SEPARATOR);
   url_ = "";
   uri.GetDatasource(url_);
   widgetUrl_ = url_;
   widgetLayers_ = layers_ = "";
   if (queryparameters.count(URL_LAYERS_FIELD_NAME)) widgetLayers_ = layers_ =
         queryparameters[URL_LAYERS_FIELD_NAME];
   widgetDescription_ = description_ = pElement_->GetDescription();
}

/** Dtor */
WxsDataSourcePart::~WxsDataSourcePart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool WxsDataSourcePart::HasChanged() {
   return !(ReadParametersFromWidget() && url_ == widgetUrl_ && layers_ == widgetLayers_
         && description_ == widgetDescription_);
}

/** Salva los cambios realizados en el Part. */
bool WxsDataSourcePart::CommitChanges() {
   if (!ReadParametersFromWidget()) return false;

   url_ = widgetUrl_;
   layers_ = widgetLayers_;
   description_ = widgetDescription_;

   if (pElement_) {
      pElement_->SetUrl(GetWxsElementUrl());
      pElement_->SetDescription(description_);
   }
   return true;
}

/** Restaura los valores originales del Part. */
bool WxsDataSourcePart::RollbackChanges() {
   widgetUrl_ = url_;
   widgetLayers_ = layers_;
   widgetDescription_ = description_;
   return UpdateParametersToWidget();
}

/** Inicializa el part */
void WxsDataSourcePart::SetInitialValues() {
   RollbackChanges();
}

/** Actualiza el estado de la parte */
void WxsDataSourcePart::Update() {
}

/** Retorna el url generado con servidor y capa */
std::string WxsDataSourcePart::GetWxsElementUrl() {
   std::string returnvalue = url_;
   if (url_.find("?") != std::string::npos)
      returnvalue += "&";
   else
      returnvalue += "?";
   return returnvalue + URL_LAYERS_FIELD_NAME + "=" + layers_;
}

/** Retorna la descripcion configurada por el usuario */
std::string WxsDataSourcePart::GetDescription() {
   return description_;
}

/**
 * Configura los parametros con datos en control
 * @return bool que indica si hubo un error
 */
bool WxsDataSourcePart::ReadParametersFromWidget() {
   bool result = GetCtrlValue(GetWidget(), wxT("ID_WMS_SOURCE_DESCRIPTION_TEXT"),
                              widgetDescription_);
   result = result && GetCtrlValue(GetWidget(), wxT("ID_WMS_URL_TEXT"), widgetUrl_);
   result = result
         && GetCtrlValue(GetWidget(), wxT("ID_LAYER_NAME_COMBOBOX"), widgetLayers_);

   return result;
}

/**
 * Actualiza el control con datos en los parametros
 * @return bool que indica si hubo un error
 */
bool WxsDataSourcePart::UpdateParametersToWidget() {
   bool result = SetCtrlValue(GetWidget(), wxT("ID_WMS_SOURCE_DESCRIPTION_TEXT"),
                              widgetDescription_);
   result = result && SetCtrlValue(GetWidget(), wxT("ID_WMS_URL_TEXT"), widgetUrl_);
   result = result
         && SetCtrlValue(GetWidget(), wxT("ID_LAYER_NAME_COMBOBOX"), widgetLayers_);

   return result;
}

/** Modifica el contenido del control con string
 * @param[in] ControlName id del control donde debe setearse el valor
 * @param[in] Value valor que debe setearse
 */
void WxsDataSourcePart::SetValue(const std::string ControlName, const std::string Value) {
   SetCtrlValue(GetWidget(), wxT(ControlName), Value);
}

}  // end namespace suri
