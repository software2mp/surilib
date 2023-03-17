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

// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards



#include "RasterizationPart.h"

// Includes standard
// Includes Suri
#include "suri/Part.h"
#include "suri/Configuration.h"
#include "suri/AuxiliaryFunctions.h"
#include "VectorDatasource.h"
#include "suri/VectorEditionTable.h"
#include "suri/VectorEditorDriver.h"
#include "suri/BufferedDriver.h"
// Includes wx
#include "wx/xrc/xmlres.h"
#include "wx/wx.h"

// Includes App
#include "suri/messages.h"
#include "resources.h"

// Defines


/** namespace suri */
namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(RasterizationPartEvent, RasterizationPart)
   IMPLEMENT_EVENT_CALLBACK(OnUiUpdate, OnUiUpdate(event), wxUpdateUIEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Constructor
 */
RasterizationPart::RasterizationPart(std::vector<Element*> Selected) :
      Part(false, false), NEW_EVENT_OBJECT(RasterizationPartEvent) {
   windowTitle_ = _(caption_MOSAIC_CONFIGURATION);
}
/**
 * Destructor
 */
RasterizationPart::~RasterizationPart() {
   DELETE_EVENT_OBJECT;
}


/**
 * Crea la ventana de la parte
 * @return false : si no pudo agregar el elemento XRC
 * @return true : si crea la ventana
 */
bool RasterizationPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_RASTERIZATION_PART"));
   pDNChoice_ = XRCCTRL(*pToolWindow_, wxT("ID_CHOICE1"), wxChoice);
   pDataTypeChoice_ = XRCCTRL(*pToolWindow_, wxT("ID_DATATYPE_CHOICE"), wxChoice);
   ConfigureDataTypeChoice();
   if (!pToolWindow_) {
      return false;
   }
   return true;
}


/**
 * Devuelve el estado de la parte
 * @return true cuando la parte esta habilitada
 * @return false cuado la parte esta desabilitada
 */
bool RasterizationPart::IsEnabled() {
   return enabled_;
}

/**
 * Informa si la parte tiene cambios
 * @return true tiene cambios
 * @return false no tiene cambios
 */
bool RasterizationPart::HasChanged() {
   attributeSelection_ = pDNChoice_->GetStringSelection();

   if(pDataTypeChoice_->GetSelection() > -1)
      typeSelection_ = dataTypeNames_[pDataTypeChoice_->GetSelection()];
   if ( (attributeSelection_ != "") && typeSelection_ != "") {
        return true;
   }
   return false;
}



/**
 * Si los datos de los controles wx son validos los guarda en las variables.
 * Si el archivo existe confirma sobreescritura.
 * Si no son validos muestra el mensaje de error que devuelve HasValidData.
 */
bool RasterizationPart::CommitChanges() {
   if (!HasValidData()) {
      REPORT_ERROR(errorMessage_);
      return false;
   }
   pPal_->AddAttribute<std::string>(
         ProcessAdaptLayer::RasterizationAttrSelectionAttr, attributeSelection_);
   pPal_->AddAttribute<std::string>(
            ProcessAdaptLayer::RasterizationTypeSelectionAttr, typeSelection_);
   return true;
}


bool RasterizationPart::RollbackChanges() {
   VectorDatasource* pVectorDatasource = NULL;
   pDNChoice_->Clear();
   pPal_->GetAttribute<VectorDatasource*>(
               ProcessAdaptLayer::RasterizationVectorDatasourceAttr, pVectorDatasource);
   VectorEditionTable vTable(pVectorDatasource);
   VectorEditorDriver* pdriver = new VectorEditorDriver(pVectorDatasource);
   BufferedDriver* pbuffereddriver = new BufferedDriver(pdriver);
   vTable.SetDriver(pbuffereddriver);
   for (int x = 0; x < vTable.GetColumns(); x++) {
      Table::ColumnType type = vTable.GetColumnType(x);
      if ( (type == Table::INT) || (type == Table::FLOAT))
         pDNChoice_->Append(vTable.GetColumnName(x));
   }
   return true;
}


/**
 * Configura los tipos de dato que puede tener la imagen de salida
 * El tamanio del tipo de dato se obtiene usando DataInfo<unsigned char>::Size
 * por si modifica para algun compilador/arquitectura.
 */
void RasterizationPart::ConfigureDataTypeChoice() {
   wxArrayString pdatatypes;

   dataTypeNames_.push_back(DataInfo<unsigned char>::Name);
   pdatatypes.Add(
         wxString::Format(_(content_UNSIGNEDINTEGEROF_d), DataInfo<unsigned char>::Size * 8));

   dataTypeNames_.push_back(DataInfo<unsigned short>::Name);
   pdatatypes.Add(
         wxString::Format(_(content_UNSIGNEDINTEGEROF_d), DataInfo<unsigned short>::Size * 8));

   dataTypeNames_.push_back(DataInfo<unsigned int>::Name);
   pdatatypes.Add(
         wxString::Format(_(content_UNSIGNEDINTEGEROF_d), DataInfo<unsigned int>::Size * 8));

   dataTypeNames_.push_back(DataInfo<short>::Name);
   pdatatypes.Add(wxString::Format(_(content_INTEGEROF_d), DataInfo<short>::Size * 8));

   dataTypeNames_.push_back(DataInfo<int>::Name);
   pdatatypes.Add(wxString::Format(_(content_INTEGEROF_d), DataInfo<int>::Size * 8));

   dataTypeNames_.push_back(DataInfo<float>::Name);
   pdatatypes.Add(wxString::Format(_(content_FLOATOF_d), DataInfo<float>::Size * 8));

   dataTypeNames_.push_back(DataInfo<double>::Name);
   pdatatypes.Add(wxString::Format(_(content_FLOATOF_d), DataInfo<double>::Size * 8));

   GET_CONTROL(*GetWidget()->GetWindow(), wxT("ID_DATATYPE_CHOICE"),
         wxChoice)->Clear();
   GET_CONTROL(*GetWidget()->GetWindow(), wxT("ID_DATATYPE_CHOICE"),
         wxChoice)->Append(pdatatypes);
}

void RasterizationPart::SetInitialValues() {
   RollbackChanges();
}




/**
 * Objetivo: Devuelve el icono de propiedades
 * Por ahora no tiene en cuenta el X e Y dado que devuelve el de 32x32 pixels
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho del icono de la ventana
 * @param[in] Y alto del icono de la ventana
 */
void RasterizationPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_IMAGE_FORMAT_SELECTION, ToolBitmap);
}

/**
 * Compara los valores de los controles wx con los valores de las propiedades,
 * y modifica el valor de la propiedad modified_
 * @param[in] Event evento UIUpdate
 */
void RasterizationPart::OnUiUpdate(wxUpdateUIEvent &Event) {
   modified_ = false;
   Event.Skip();
}


bool RasterizationPart::HasValidData() {
   bool result = true;
   return result;
}

}


