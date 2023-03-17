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

#include "SingleLayerBandSelectionPart.h"

#include <sstream>
#include <algorithm>

#include "suri/xmlnames.h"

#include "wx/xrc/xmlres.h"
#include "wx/wx.h"
#include "wx/listctrl.h"
#include "wx/bitmap.h"
#include "wx/imaglist.h"
#include "wx/icon.h"
#include "wx/dcclient.h"

#include "resources.h"

// Tamanio de las imagenes de las listas
#define LIST_IMAGE_SIZE 16

// Posicion de la columna en la lista
#define IMAGE_COLUMN 0

// Posicion de la columna en la lista
#define INPUT_BAND_NAME_COLUMN 1

// Ancho de la columna de imagen en pixeles
#define IMAGE_COLUMN_EXTRA_WIDTH 10




// inicializa los recursos de la ventana
void InitSingleLayerBandSelectionXmlResource();

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(SingleLayerBandSelectionEvent, SingleLayerBandSelectionPart)
      IMPLEMENT_EVENT_CALLBACK(OnInputSelection, OnInputSelection(event), wxListEvent)
END_IMPLEMENT_EVENT_OBJECT

/**
 * Constructor.
 * Obtiene la cantidad y nombre de las bandas del elemento y los guarda en las
 * propiedades.
 * @param[in] pElement puntero al elemento
 */
SingleLayerBandSelectionPart::SingleLayerBandSelectionPart(RasterElement * pElement) :
      NEW_EVENT_OBJECT(SingleLayerBandSelectionEvent) {
   pElement_ = pElement;

   for (int ix = 0, lenix = pElement_->GetBandCount(); ix < lenix; ++ix) {
      std::stringstream ss;

      std::string bandname;
      pElement_->GetBandName(bandname, ix);
      ss << bandname;
      if (!pElement_->GetName().empty())
         ss << " [" << pElement_->GetName().c_str() << "]";

      bands_.push_back(BandInfo(pElement_, ix, ss.str(), 0, 0, ""));
   }
}

/**
 * Destructor
 */
SingleLayerBandSelectionPart::~SingleLayerBandSelectionPart() {
   DELETE_EVENT_OBJECT;
}

/**
 * Carga el XRC de la lista de bandas de entrada.
 * Conecta el evento UIUpdate a la lista de bandas de entrada.
 * Invoca al metodo que configura la lista de entrada.
 * @return true si pudo cargar el xrc
 * @return false si fallo la carga del xrc
 */
bool SingleLayerBandSelectionPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_SINGLE_BAND_SELECTION_PART"));
   if (!pToolWindow_) {
      return false;
   }

   // Conecta a la lista de entrada el evento doble click sobre un elemento
   wxListCtrl *plist = XRCCTRL(*pToolWindow_, wxT("ID_SINGLE_SELECTION_LIST"), wxListView);
   if (!plist) {
      return false;
   }

   plist->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
                  wxListEventHandler(SingleLayerBandSelectionEvent::OnInputSelection),
                  NULL, pEventHandler_);

   ConfigureInputBandList();

   return true;
}

/**
 * Crea dos columnas en la lista de entrada.
 * Crea la lista de iconos para las bandas.
 */
void SingleLayerBandSelectionPart::ConfigureInputBandList() {
   // Obtengo el icono para las bandas
   wxBitmap imageband;
   GET_BITMAP_RESOURCE(wxT(icon_IMAGE_BAND), imageband);

   // Creo la lista de imagenes para la lista de entrada
   wxImageList *pimageinlist = new wxImageList(LIST_IMAGE_SIZE, LIST_IMAGE_SIZE);
   pimageinlist->Add(imageband);
   wxBitmap invalidimageband;
   GET_BITMAP_RESOURCE(wxT(icon_IMAGE_INVALID_BAND), invalidimageband);
   pimageinlist->Add(invalidimageband);
   // Agrego la lista de imagenes para iconos de las bandas
   wxListCtrl *plist = XRCCTRL(*pToolWindow_, wxT("ID_SINGLE_SELECTION_LIST"), wxListView);

   // Si no puede acceder al control elimina la lista de imagenes
   if (!plist) {
      delete pimageinlist;
      return;
   }
   plist->AssignImageList(pimageinlist, wxIMAGE_LIST_SMALL);

   // creo las columnas de la lista de entrada
   int imgwidth = LIST_IMAGE_SIZE + IMAGE_COLUMN_EXTRA_WIDTH;

   // Columna imagen
   plist->InsertColumn(IMAGE_COLUMN, wxEmptyString, wxLIST_FORMAT_CENTER, imgwidth);

   // Columna nombres
   plist->InsertColumn(INPUT_BAND_NAME_COLUMN, _(label_INPUT_BANDLIST),
                       wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
   LoadInputBands();
}

/**
 * Carga las bandas del elemento en la lista de bandas de entrada
 */
void SingleLayerBandSelectionPart::LoadInputBands() {
   wxListCtrl * plist = XRCCTRL(*pToolWindow_, wxT("ID_SINGLE_SELECTION_LIST"), wxListView);
   if (!plist) {
      return;
   }

   // Cargo las bandas
   for (int ix = 0, lenix = bands_.size(); ix < lenix; ++ix) {
      int valid = 1;
      pElement_->GetBandValidity(valid, bands_[ix].GetPosition());
      plist->InsertItem(ix, wxEmptyString, valid == 1? 0 : 1);
      plist->SetItem(ix, INPUT_BAND_NAME_COLUMN, bands_[ix].GetName());

      // Esto hay que hacerlo asi porque los campeones de wxWidgets
      // metieron un SetItemPtrData pero no pusieron el GetItemPtrData :)
      plist->SetItemData(ix, ix);
   }
}

/**
 * Metodo que se ejecuta cuando se hace doble click sobre una banda de la lista
 * de entrada, invoca al metodo que agrega las bandas seleccionadas.
 * @param[in] Event evento doble click sobre un item de la lista de entrada
 */
void SingleLayerBandSelectionPart::OnInputSelection(wxListEvent &Event) {
   EventListener* plistener = GetInputBandListener();
   if (plistener != NULL)
      plistener->EventEmitted(this);
}

/**
 * Obtiene las bandas seleccionadas de la lista de entrada y las inserta
 * en un vector.
 * @return bands vector con las bandas selecionadas de la lista de entrada.
 */
std::vector<BandInfo> SingleLayerBandSelectionPart::GetInputSelectedBands() {
   wxListView* plist = XRCCTRL(*pToolWindow_, wxT("ID_SINGLE_SELECTION_LIST"), wxListView);

   std::vector<BandInfo> bands;

   int bandinputindex;

   // Obtiene el indice al primer item seleccionado.
   bandinputindex = plist->GetFirstSelected();
   while (bandinputindex > -1) {
      int itemdatapos = plist->GetItemData(bandinputindex);
      int valid = 1;
      pElement_->GetBandValidity(valid, bands_[itemdatapos].GetPosition());
      if (valid == 0)
         SHOW_WARNING(message_INVALID_BAND_NAME_SELECTED, bands_[itemdatapos].GetName().c_str());
      else
         bands.push_back(bands_[itemdatapos]);
      bandinputindex = plist->GetNextSelected(bandinputindex);
   }
   return bands;
}

/**
 * Devuelve el nombre de todas las banda disponibles
 *
 * @return Array de wxString con los nombres de todas las bandas disponibles.
 */
std::vector<BandInfo> SingleLayerBandSelectionPart::GetAvailableBands() const {
   std::vector<BandInfo> bands;
    for (int ix = 0, lenix = bands_.size(); ix < lenix; ++ix) {
      int valid = 1;
      pElement_->GetBandValidity(valid, bands_[ix].GetPosition());
      if (valid != 0)
         bands.push_back(bands_[ix]);
   } 
   return bands;
}

/**
 * Ajusta el ancho de la ultima columna para que ocupe el espacio
 * libre en el cotrol lista. Se llama desde el OnResize de la clase
 * que lo contiene.
 */
void SingleLayerBandSelectionPart::UpdateSourceCtrl() {
   wxListCtrl* plist = XRCCTRL(*pToolWindow_, wxT("ID_SINGLE_SELECTION_LIST"), wxListView);
   if (plist != NULL)
      SelectionPart::AdjustLastColumn(plist);
}

} // namespace suri
