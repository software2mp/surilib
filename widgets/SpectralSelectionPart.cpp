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
#include <vector>
#include <string>
#include <sstream>
#include <map>

#include "SpectralSelectionPart.h"

// Includes Suri
#include "suri/RasterElement.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/messages.h"

#include "SelectionPart.h"

// Includes wx
#include "wx/xrc/xmlres.h"
#include "wx/wx.h"
#include "wx/listctrl.h"
#include "wx/bmpbuttn.h"
#include "wx/bitmap.h"
#include "wx/imaglist.h"
#include "wx/icon.h"
#include "wx/sizer.h"

// Includes App
#include "resources.h"

// Defines
/** Tamanio de las imagenes de las listas */
#define LIST_IMAGE_SIZE 16
/** Posicion de la columna en la lista */
#define IMAGE_COLUMN 0
/** Posicion de la columna en la lista */
#define OUTPUT_BAND_INDEX_COLUMN 1
/** Posicion de la columna en la lista */
#define OUTPUT_BAND_NAME_COLUMN 2
#ifdef __UNUSED_CODE__
// Pasa a la clase base
/** Posicion de la columna en la lista */
#define INPUT_BAND_NAME_COLUMN 1
#endif
/** Ancho de la columna de imagen en pixeles */
#define IMAGE_COLUMN_EXTRA_WIDTH 10
/** Ancho de la columna de indice en pixeles */
#define BAND_INDEX_COLUMN_WIDTH 20

/** Inicializa recursos de la ventana. */
void InitSpectralSelectionXmlResource();

/** namespace suri */
namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(SpectralSelectionPartEvent, SpectralSelectionPart)
#ifdef __UNUSED_CODE__
// Pasa a la clase base
   IMPLEMENT_EVENT_CALLBACK(OnInputSelection, OnInputSelection(event), wxListEvent)
#endif
   IMPLEMENT_EVENT_CALLBACK(OnOutputSelection, OnOutputSelection(event), wxListEvent)
   IMPLEMENT_EVENT_CALLBACK(OnAddButton, OnAddButton(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnRemoveButton, OnRemoveButton(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnUpButton, OnUpButton(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnDownButton, OnDownButton(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnResize, OnResize(event), wxSizeEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/**
 * Constructor
 * Agrega el titulo a la parte y obtiene el numero de bandas. Obtiene el
 * nombre de las bandas y los agrega a la lista de bandas.
 * @param[in] pElement : elemento raster.
 */
SpectralSelectionPart::SpectralSelectionPart(SelectionPart* pSelectionPartP) :
      Part(true, false), NEW_EVENT_OBJECT(SpectralSelectionPartEvent) {
   modified_ = false;
   windowTitle_ = _(caption_SPECTRAL_SELECTION);
   pSelectionPart_ = pSelectionPartP;
   pOutputEmptyListener_ = NULL;
   pInputRequestedListener_ = NULL;
}

/**
 * Destructor
 */
SpectralSelectionPart::~SpectralSelectionPart() {
   DELETE_EVENT_OBJECT;
}

/**
 * Crea la ventana de la parte
 * @return true : si pudo cargar la parte
 * @return false : si no pudo cargar la parte
 */
bool SpectralSelectionPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_SPECTRAL_SELECTION_PART"));

   wxPanel* pcttpanel = XRCCTRL(*pToolWindow_, wxT("ID_SPECTRAL_MAIN_PANEL"), wxPanel);

   // Agrego y configuro el part para seleccionar las bandas de origen.
   if (pSelectionPart_ == NULL) return false;

   pSelectionPart_->CreatePartTool(pcttpanel);
   pSelectionPart_->SetInputBandListener(this);

   SetOutputEmptyListener(pSelectionPart_);
   SetInputRequestedListener(pSelectionPart_);

   if (!AddControl(pSelectionPart_, pcttpanel, ControlArrangement(1, 0))) return false;

   // Conecta el boton de agregar bandas
   GET_CONTROL(*pToolWindow_, wxT("ID_ADD_BUTTON"), wxBitmapButton)
      ->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                wxCommandEventHandler(SpectralSelectionPartEvent::OnAddButton), NULL,
                pEventHandler_);
   GET_CONTROL(*pToolWindow_, wxT("ID_ADD_BUTTON"), wxBitmapButton)
      ->SetToolTip(_(tooltip_ADD_BUTTON));

   // Conecta el boton de quitar bandas
   GET_CONTROL(*pToolWindow_, wxT("ID_REMOVE_BUTTON"), wxBitmapButton)
      ->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                wxCommandEventHandler(SpectralSelectionPartEvent::OnRemoveButton), NULL,
                pEventHandler_);
   GET_CONTROL(*pToolWindow_, wxT("ID_REMOVE_BUTTON"), wxBitmapButton)
      ->SetToolTip(_(tooltip_REMOVE_BUTTON));

   // Conecta el boton de subir bandas
   GET_CONTROL(*pToolWindow_, wxT("ID_UP_BUTTON"), wxBitmapButton)
      ->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                wxCommandEventHandler(SpectralSelectionPartEvent::OnUpButton), NULL,
                pEventHandler_);
   GET_CONTROL(*pToolWindow_, wxT("ID_UP_BUTTON"), wxBitmapButton)
      ->SetToolTip(_(tooltip_UP_BUTTON));

   // Conecta el boton de bajar bandas
   GET_CONTROL(*pToolWindow_, wxT("ID_DOWN_BUTTON"), wxBitmapButton)
      ->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                wxCommandEventHandler(SpectralSelectionPartEvent::OnDownButton), NULL,
                pEventHandler_);
   GET_CONTROL(*pToolWindow_, wxT("ID_DOWN_BUTTON"), wxBitmapButton)
      ->SetToolTip(_(tooltip_DOWN_BUTTON));

   // Completa las listas de bandas
   ConfigureOutputBandList();

   // Conecta a la lista de salida el evento doble click sobre un elemento
   GET_CONTROL(*pToolWindow_, wxT("ID_OUTPUT_BAND_LIST"), wxListView)
      ->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
                wxListEventHandler(SpectralSelectionPartEvent::OnOutputSelection), NULL,
                pEventHandler_);

   GET_CONTROL(*pToolWindow_, wxT("ID_OUTPUT_BAND_LIST"),
         wxListView)
      ->Connect(wxEVT_SIZE, wxSizeEventHandler(SpectralSelectionPartEvent::OnResize),
                NULL, pEventHandler_);

   return true;
}

/**
 * Metodo configura la ventana de seleccion de bandas.
 * Agrega el icono para las bandas. Crea las columnas de ambas listas.
 * Carga la lista de entrada con los nombres de las bandas.
 */
void SpectralSelectionPart::ConfigureOutputBandList() {
   wxListCtrl *plist = XRCCTRL(*pToolWindow_, wxT("ID_OUTPUT_BAND_LIST"), wxListView);
   if (!plist) {
      return;
   }

   // Obtengo el icono para las bandas
   wxBitmap imageband;
   GET_BITMAP_RESOURCE(wxT(icon_IMAGE_BAND), imageband);

   // Creo la lista de imagenes para la lista de salida
   wxImageList *pimageoutlist = new wxImageList(LIST_IMAGE_SIZE, LIST_IMAGE_SIZE);
   pimageoutlist->Add(imageband);

   plist->AssignImageList(pimageoutlist, wxIMAGE_LIST_SMALL);

   int columnimgwidth = LIST_IMAGE_SIZE + IMAGE_COLUMN_EXTRA_WIDTH;

   // Columna imagen
   plist->InsertColumn(IMAGE_COLUMN, wxEmptyString, wxLIST_FORMAT_CENTER,
                       columnimgwidth);

   // Columna indice
   plist->InsertColumn(OUTPUT_BAND_INDEX_COLUMN, _(label_INDEX_BAND_COLUMN),
                       wxLIST_FORMAT_CENTER, BAND_INDEX_COLUMN_WIDTH);

   // Columna nombres
   plist->InsertColumn(OUTPUT_BAND_NAME_COLUMN, _(label_OUTPUT_BANDLIST),
                       wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
}

/**
 * Retorna si la parte esta habilitada (true) o no (false).
 * @return enabled_
 */
bool SpectralSelectionPart::IsEnabled() {
   return enabled_;
}

/* Indica si el Part tiene cambios para salvar.
 * @return modified_
 */
bool SpectralSelectionPart::HasChanged() {
   return modified_;
}

/**
 * Verifica si tiene datos validos. Verifica si hay cambios, si hay obtiene
 * las bandas de la lista de salida.
 * @return true si tiene datos validos
 * @return false si no tiene datos validos, no hay bandas seleccionadas
 */
bool SpectralSelectionPart::CommitChanges() {
   if (!HasValidData()) {
      SHOW_ERROR(errorMessage_);
      return false;
   }

   if (modified_) {
      selectedBands_.clear();
      selectedBands_ = GetOutputBands();
      modified_ = false;
   }
   if (pPal_) {
      pPal_->AddSerializableAttribute<BandInfo>(ProcessAdaptLayer::BandInfoKeyAttr,
                                                   selectedBands_);
      CombinationInfo* pcomb = GetCombinationInfo();
      pPal_->AddAttribute<CombinationInfo*>(ProcessAdaptLayer::CombinationInfoKeyAttr,
                                            pcomb);
   }
   return true;
}
/**
 * Pasa la ultima seleccion de bandas valida a la lista wx de salida.
 * Vacia la lista de salida. Si la seleccion estaba vacia sale. Si no
 * carga cada banda seleccionada en la lista de salida.
 * @return true siempre
 */
bool SpectralSelectionPart::RollbackChanges() {
   // Elimino todos los items de la lista y del buffer
   GET_CONTROL(*pToolWindow_, wxT("ID_OUTPUT_BAND_LIST"),
         wxListView)->DeleteAllItems();
   bandsBuffer_.clear();

   // Verifico que haya datos para el rollback
   if (selectedBands_.empty()) return true;

   for (int ix = 0, lenix = selectedBands_.size(); ix < lenix; ++ix) {
      AddOutputBand(selectedBands_[ix], GetNextInsertPosition());
   }
   if (pPal_) {
      pPal_->AddSerializableAttribute<BandInfo>(ProcessAdaptLayer::BandInfoKeyAttr,
                                                selectedBands_);
      CombinationInfo* pcomb = GetCombinationInfo();
      pPal_->AddAttribute<CombinationInfo*>(ProcessAdaptLayer::CombinationInfoKeyAttr,
                                            pcomb);
   }
   return true;
}

/**
 * La lista de bandas de salida esta vacia por defecto.
 * Vacia la lista y llama a rollbackchanges.
 */
void SpectralSelectionPart::SetInitialValues() {
   if (pSelectionPart_ != NULL) {
      selectedBands_.clear();
      for (int ix = 0, lenix = pSelectionPart_->GetAvailableBands().size(); ix < lenix; ++ix)
         selectedBands_.push_back(pSelectionPart_->GetAvailableBands()[ix]);

      pPal_->AddSerializableAttribute<BandInfo>(ProcessAdaptLayer::BandInfoKeyAttr,
                                                   selectedBands_);
      RollbackChanges();
   }
}

/**
 * Objetivo: Devuelve el icono de propiedades
 * Por ahora no tiene en cuenta el X e Y dado que devuelve el de 32x32 pixels
 * @param[in] X ancho del icono
 * @param[in] Y alto del icono.
 * @param[out] ToolBitmap icono a devolver.
 */
void SpectralSelectionPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_SPECTRAL_SELECTION, ToolBitmap);
}

/**
 * Metodo que se ejecuta con el boton agregar.
 * Agrega el item seleccionado de la lista de entrada al final de la de salida.
 * @param[in] Event evento
 */
void SpectralSelectionPart::OnAddButton(wxCommandEvent &Event) {
   InputRequestedListener* plistener = GetInputRequestedListener();
   if (plistener != NULL) {
      plistener->EventEmitted(NULL);
   }
}

/**
 * Metodo que se ejecuta con el boton quitar
 * Elimina el item seleccionado de la lista de salida.
 * @param[in] Event evento
 */
void SpectralSelectionPart::OnRemoveButton(wxCommandEvent &Event) {
   RemoveSelectedItem();
}

/**
 * Metodo que se ejecuta con el boton subir
 * Encuentra la banda seleccionada, obtiene los datos, la elimina de su posicion
 * y la inserta una posicion antes.
 * Si hay mas de un item seleccionado no hace nada.
 * @param[in] Event evento
 */
void SpectralSelectionPart::OnUpButton(wxCommandEvent &Event) {
   if (pSelectionPart_ != NULL) {
      wxListView* plist = XRCCTRL(*pToolWindow_, wxT("ID_OUTPUT_BAND_LIST"),
                                  wxListView);

      // Tiene que haber UN SOLO item seleccionado, sino sale.
      if (plist->GetSelectedItemCount() != 1) return;

      // Obtengo el item seleccionado
      int bandoutputindex = plist->GetFirstSelected();

      // Obtiene el numero de la banda, guardado en ItemData
      // El metodo AddOutputBand contiene una explicacion.
      int bandpos = plist->GetItemData(bandoutputindex);
      BandInfo pbandinfo = bandsBuffer_[bandpos];

      // Verifico que el item seleccionado no sea el primero, y que pueda borrar
      if (bandoutputindex > 0 && RemoveItem(bandoutputindex)) {
         // Guardo la info de la banda que va a ser reemplazada al estar moviendo
         // el elemento actual (en este caso a la posicion anterior)
         BandInfo pauxband = bandsBuffer_[bandoutputindex - 1];
         bandsBuffer_[bandpos] = pauxband;
         plist->SetItemData(bandoutputindex - 1, bandpos);

         AddOutputBand(pbandinfo, bandoutputindex - 1);
         plist->Select(bandoutputindex - 1);
      }
   }
}

/**
 * Metodo que se ejecuta con el boton bajar.
 * Encuentra la banda seleccionada, obtiene los datos, la elimina de su posicion
 * y la inserta una posicion despues.
 * Si hay mas de un item seleccionado no hace nada.
 * @param[in] Event evento
 */
void SpectralSelectionPart::OnDownButton(wxCommandEvent &Event) {
   if (pSelectionPart_ != NULL) {
      wxListView* plist = XRCCTRL(*pToolWindow_, wxT("ID_OUTPUT_BAND_LIST"),
                                  wxListView);

      // Tiene que haber UN SOLO item seleccionado, sino sale.
      if (plist->GetSelectedItemCount() != 1) return;

      // Obtengo el item seleccionado
      int bandoutputindex = plist->GetFirstSelected();

      // Obtengo la cantidad de items de la lista
      int bandcount = plist->GetItemCount();

      // Obtiene el numero de la banda, guardado en ItemData
      // El metodo AddOutputBand contiene una explicacion.
      int bandpos = plist->GetItemData(bandoutputindex);
      BandInfo pbandinfo = bandsBuffer_[bandpos];

      // Verifico que el item seleccionado no sea el ultimo, y que pueda borrar
      if ((bandoutputindex < (bandcount - 1)) && RemoveItem(bandoutputindex)) {
         // Guardo la info de la banda que va a ser reemplazada al estar moviendo
         // el elemento actual (en este caso a la posicion siguiente)
         BandInfo pnextband = bandsBuffer_[bandoutputindex + 1];
         bandsBuffer_[bandpos] = pnextband;
         plist->SetItemData(bandoutputindex, bandpos);

         AddOutputBand(pbandinfo, bandoutputindex + 1);
         plist->Select(bandoutputindex + 1);
      }
   }
}

/**
 * Metodo que se ejecuta cuando se hace doble click sobre un elemento de la lista
 * de entrada o salida.
 * @param[in] Event evento
 */
void SpectralSelectionPart::OnOutputSelection(wxListEvent &Event) {
   RemoveSelectedItem();
}

/**
 * Verifica que la lista de bandas de salida no este vacia
 * return true : por lo menos una banda seleccionada
 * return false : lista de salida vacia
 */
bool SpectralSelectionPart::HasValidData() {
   int bandcount = USE_CONTROL(*pToolWindow_, wxT("ID_OUTPUT_BAND_LIST"), wxListView,
         GetItemCount(), wxNOT_FOUND);

   return (bandcount > 0) ? true : false;
}

/**
 * Obtiene el numero de banda para la lista de bandas de salida, almacenado
 * como clientDada.
 * @return bandList_
 */
std::vector<BandInfo> SpectralSelectionPart::GetSelectedBands() {
   return selectedBands_;
}

/**
 * Elimina los items seleccionados de la lista de salida. Encuentra los items
 * seleccionados e invoca a RemoveItem por cadda uno. Marca como seleccionado
 * el elemento mas cercano al ultimo eliminado.
 */
void SpectralSelectionPart::RemoveSelectedItem() {
   wxListView* plist = XRCCTRL(*pToolWindow_, wxT("ID_OUTPUT_BAND_LIST"), wxListView);

   // Variable para guardar el indice del ultimo item eliminado de la lista
   int lastdeleted = -1;
   int bandoutputindex = plist->GetFirstSelected();

   while (bandoutputindex > -1) {
      RemoveItem(bandoutputindex, true);
      lastdeleted = bandoutputindex;
      // Obtiene el indice del item seleccionado siguiente
      bandoutputindex = plist->GetFirstSelected();
   }

   // Obtengo la cantidad de items
   int itemscount = plist->GetItemCount();

   // Si quedan items en la lista
   if (itemscount > 0 && lastdeleted != -1) {
      // Si el que elimine era el ultimo selecciono el anterior
      if (lastdeleted == itemscount) {
         plist->Select(itemscount - 1);
      }
      // Si no selecciono el elemento en la misma posicion al borrado
      else {
         plist->Select(lastdeleted);
      }
   }

   // Si despues de sacar el item la lista queda vacia entonces hay
   // que avisarle al part de origen de datos para que actualize el
   // estado de los items si es necesario.
   if (itemscount == 0) {
      OutputEmptyListener* plistener = GetOutputEmptyListener();
      if (plistener != NULL)
         plistener->EventEmitted(NULL);
   }
}

/**
 * Agrega el item indicado por la posicion en la lista de entrada, en la posicion
 * especifica en la lista de salida.
 * @param[in] Item : posicion del item en la lista de entrada
 * @param[out] Position: posicion en la que se inserta el item en la lista de salida
 * @return true : si pudo insertar el item.
 * @return false : no pudo insertar el item.
 */
bool SpectralSelectionPart::AddOutputBand(BandInfo bandInfoP, int Position) {
   if (pSelectionPart_ != NULL) {
      int outputitemcount = USE_CONTROL(*pToolWindow_, wxT("ID_OUTPUT_BAND_LIST"),
            wxListView, GetItemCount(), wxNOT_FOUND);

      if ((Position > outputitemcount) || (Position < 0)) {
         return false;
      }

      wxString index;
      index.sprintf("%d", Position + 1);
      int imageindex = 0;

      // Lo agrego al mapa de bandas. Esto es para simular la falta de un metodo
      // SetItemData que acepte algo mas que un long en el wxListCtrl. Existe un
      // SetItemPtrData pero no existe un GetItemPtrData.
      bandsBuffer_[Position] = bandInfoP;

      GET_CONTROL(*pToolWindow_, wxT("ID_OUTPUT_BAND_LIST"),
            wxListView)->InsertItem(Position, "", imageindex);

      GET_CONTROL(*pToolWindow_, wxT("ID_OUTPUT_BAND_LIST"), wxListView)
         ->SetItem(Position, OUTPUT_BAND_INDEX_COLUMN, index);

      GET_CONTROL(*pToolWindow_, wxT("ID_OUTPUT_BAND_LIST"), wxListView)
         ->SetItem(Position, OUTPUT_BAND_NAME_COLUMN,
                   bandInfoP.GetName());

      GET_CONTROL(*pToolWindow_, wxT("ID_OUTPUT_BAND_LIST"),
            wxListView)->SetItemData(Position, Position);

      UpdateBandIndex();

      return true;
   }
   return false;
}

/**
 * Elimina el item de la lista de salida
 *
 * @param[in] Item : indice del elemento a eliminar.
 * @param[in] UpdateInternalBufferP Indica si se debe actualiza el buffer interno
 *    que contiene la informacion de las bandas.
 * @return true : si pudo eliminar el item
 * @return false : si no pudo obtener el elemento seleccionado o no hay.
 */
bool SpectralSelectionPart::RemoveItem(const int &Item, bool UpdateInternalBufferP) {
   wxListView* plist = XRCCTRL(*pToolWindow_, wxT("ID_OUTPUT_BAND_LIST"), wxListView);

   // Obtengo la cantidad de items
   int itemscount = plist->GetItemCount();

   // Si no obtuvo la cantidad de items o el item es mayor a la cantidad
   if (itemscount == 0 || Item >= itemscount) return false;

   // Elimina el item
   modified_ = true;
   plist->DeleteItem(Item);

   // Lo borro tambien del mapa de bandas que usa para simular
   // la falta de una correcta implementacion del SetItemData
   // del wxListCtrl. Ver metodo AddOutputBand
   bandsBuffer_.erase(Item);

   // Actualiza el buffer interno.
   if (UpdateInternalBufferP) {
      for (int ix = Item, lenix = plist->GetItemCount(); ix < lenix; ++ix) {
         int bandpos = plist->GetItemData(ix);
         BandInfo paux = bandsBuffer_[bandpos];
         bandsBuffer_.erase(bandpos);
         plist->SetItemData(ix, bandpos - 1);
         bandsBuffer_[bandpos - 1] = paux;
      }
   }

   UpdateBandIndex();

   return true;
}

/**
 * Modifica el campo que muestra el numero de banda en la lista de bandas seleccionadas.
 * Este metodo permite que se mantenga el orden ante una eliminacion o subir/bajar algun
 * item.
 */
void SpectralSelectionPart::UpdateBandIndex() {
   wxListView* plist = XRCCTRL(*pToolWindow_, wxT("ID_OUTPUT_BAND_LIST"), wxListView);

   wxString index;

   for (int ix = 0, lenix = plist->GetItemCount(); ix < lenix; ++ix) {
      index.sprintf("%d", ix + 1);
      plist->SetItem(ix, OUTPUT_BAND_INDEX_COLUMN, index);
   }
}

/**
 *	Recorre la lista de bandas de salida y guarda para cada una el numero de
 *	banda.
 *	@return bandlist : string con las bandas separadas por espacio en blanco.
 */
std::vector<BandInfo> SpectralSelectionPart::GetOutputBands() {
   std::vector<BandInfo> bandlist;
   wxListView* plist = XRCCTRL(*pToolWindow_, wxT("ID_OUTPUT_BAND_LIST"), wxListView);

   if (pSelectionPart_ != NULL) {
      for (int ix = 0, lenix = plist->GetItemCount(); ix < lenix; ++ix) {
         int bandpos = plist->GetItemData(ix);
         bandlist.push_back(bandsBuffer_[bandpos]);
      }
   }
   return bandlist;
}

/**
 * Metodo que devuelve la posicion para insertar el proximo elemento.
 * Siempre devuelve la ultima posicion de la lista. Obtiene la cantidad de bandas
 * de la lista y devuelve ese valor.
 * @return int con la posicion para insertar el proximo elemento.
 */
int SpectralSelectionPart::GetNextInsertPosition() {
   int itemcount = USE_CONTROL(*pToolWindow_, wxT("ID_OUTPUT_BAND_LIST"),
         wxListView, GetItemCount(), wxNOT_FOUND);
   return itemcount;
}

/**
 * Este metodo se ejecuta cuando la lista recibe el evento de resize. Se invoca
 * al metodo BandSelectionPart::AdjustLastColumn para ajustar la ultima columna
 * de la lista.
 * @param[in]	Event: evento
 */
void SpectralSelectionPart::OnResize(wxSizeEvent &Event) {
   SelectionPart::AdjustLastColumn(dynamic_cast<wxListCtrl*>(Event.GetEventObject()));

   // Hack para actualizar el control lista (particularmente) del origen de datos.
   if (pSelectionPart_ != NULL)
      pSelectionPart_->UpdateSourceCtrl();
}

void SpectralSelectionPart::OnBandSelected(std::vector<BandInfo> BandIxsP) {
   modified_ = true;
   for (int ix = 0, lenix = BandIxsP.size(); ix < lenix; ++ix) {
      if (IsValidItem(BandIxsP[ix]))
         AddOutputBand(BandIxsP[ix], GetNextInsertPosition());
   }
}

/**
 * Agrega a los vectores la informacion de la combinacion correspondiente
 * a cada uno.
 *
 * @param[out] UrlsP Elementos que participan en la combinacion.
 * @param[out] CombinationsP Bandas que participan en la combinacion.
 * @param[in] GenIdP Identificador de la combinacion.
 * @param[in] UrlP Url del elemento del que se van a extraer las bandas.
 * @param[in] BandsP Bandas que se van a extraer del elemento.
 */
void SpectralSelectionPart::AppendCombinationInfo(
      std::vector<wxXmlNode*>& UrlsP, std::vector<wxXmlNode*>& CombinationsP,
      int GenIdP, std::string& UrlP, std::string& BandsP) {
   // Genero el ID para la URL
   std::stringstream ssid;
   ssid << GenIdP;

   // Creo el nodo URL con el ID
   wxXmlProperty* pprop = NULL;
   if (GenIdP > 0) {
      pprop = new wxXmlProperty(wxT(BAND_STACKING_ID), wxT(ssid.str().c_str()));
   }

   wxXmlNode* purl = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(URL_NODE),
                                   wxEmptyString, pprop);
   wxXmlNode* purlcontent = new wxXmlNode(NULL, wxXML_TEXT_NODE, wxEmptyString,
                                          wxT(UrlP.c_str()));
   purl->AddChild(purlcontent);

   UrlsP.push_back(purl);

   // Creo el nodo COMBINACION con el ID
   wxXmlProperty* ppropband = NULL;
   if (GenIdP > 0) {
      ppropband = new wxXmlProperty(wxT(BAND_STACKING_ID), wxT(ssid.str().c_str()));
   }

   wxXmlNode* pband = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                    wxT(BAND_COMBINATION_NODE), wxEmptyString,
                                    ppropband);
   wxXmlNode* pbandcontent = new wxXmlNode(NULL, wxXML_TEXT_NODE, wxEmptyString,
                                           wxT(BandsP.c_str()));
   pband->AddChild(pbandcontent);

   CombinationsP.push_back(pband);
}

/**
 * Devuelve la informacion de combinacion de bandas.
 *
 * @return Informacion de combinacion de bandas.
 */
CombinationInfo* SpectralSelectionPart::GetCombinationInfo() {
   std::vector<wxXmlNode*> urls;
   std::vector<wxXmlNode*> combinations;

   int genid = 0;
   std::string urlcurr = selectedBands_[0].GetElement()->GetUrl().c_str();
   std::string urlbands;

   for (int ix = 0, lenix = selectedBands_.size(); ix < lenix; ++ix) {
      if (urlcurr.compare(selectedBands_[ix].GetElement()->GetUrl().c_str()) != 0) {
         AppendCombinationInfo(urls, combinations, genid, urlcurr, urlbands);

         // Limpio/ajusto las variables que hacen falta para la proxima URL
         urlcurr = selectedBands_[ix].GetElement()->GetUrl().c_str();
         urlbands.clear();
         ++genid;
      }

      if (urlbands.size() > 0)
         urlbands.append(" ");

      std::stringstream ss;
      ss << selectedBands_[ix].GetPosition();
      urlbands.append(ss.str());
   }

   AppendCombinationInfo(urls, combinations, genid, urlcurr, urlbands);

   return new CombinationInfo(urls, combinations);
}

/**
 * Verifica si el item es valido para agregar comprobando que no sea
 * un item que ya existe en el buffer interno.
 *
 * @param[in] pItemP Item a verificar.
 * @return true si el item es valido y se puede agregar.
 */
bool SpectralSelectionPart::IsValidItem(BandInfo pItemP) {
   std::map<int, BandInfo>::iterator it = bandsBuffer_.begin();
   for (; it != bandsBuffer_.end(); ++it) {
      if ((*it).second.GetName().compare(pItemP.GetName()) == 0) return false;
   }
   return true;
}

/**
 * Configura el objeto que va a recibir las notificacion de que
 * se el control de destino esta vacio
 *
 * @param[in] pOutputEmptyListener Objeto que va a recibir las notificaciones
 * de salida vacia.
 */
void SpectralSelectionPart::SetOutputEmptyListener(OutputEmptyListener* pOutputEmptyListener) {
   pOutputEmptyListener_ = pOutputEmptyListener;
}

/**
 * Devuelve el objeto que va a recibir las notificacion de que
 * el control de destino esta vacio.
 *
 * @return Objeto configurado para recibir notificaciones de salida vacia.
 */
OutputEmptyListener* SpectralSelectionPart::GetOutputEmptyListener() const {
   return pOutputEmptyListener_;
}

/**
 * Configura el objeto que va a recibir las notificacion de que
 * el control esta solicitando elementos de entrada.
 */
void SpectralSelectionPart::SetInputRequestedListener(
      InputRequestedListener* pInputRequestedListener) {
   pInputRequestedListener_ = pInputRequestedListener;
}

/**
 * Devuelve el objeto que va a recibir las notificacion de que
 * el control esta solicitando elementos de entrada.
 */
InputRequestedListener* SpectralSelectionPart::GetInputRequestedListener() const {
   return pInputRequestedListener_;
}

}  // namespace suri
