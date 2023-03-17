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
#include <vector>

// Includes suri
#include "BandCombinationSelectionPart.h"
#include "suri/xmlnames.h"
#include "suri/AuxiliaryFunctions.h"
#include "RasterLayer.h"

// Includes wx
#include "wx/xrc/xmlres.h"
#include "wx/wx.h"
#include "wx/listctrl.h"
#include "wx/bmpbuttn.h"
#include "wx/bitmap.h"
#include "wx/imaglist.h"
#include "wx/icon.h"

// Includes App
#include "resources.h"

// Defines
/** Tamanio de las imagenes de las listas */
#define LIST_IMAGE_SIZE 16
/** Posicion de la columna en la lista */
#define IMAGE_COLUMN 0
/** Posicion de la columna en la lista */
#define OUTPUT_BAND_NAME_COLUMN 1
/** Ancho de la columna de imagen en pixeles */
#define IMAGE_COLUMN_EXTRA_WIDTH 10
/** Indice para la posicion de la banda para canal rojo en la lista de bandas */
#define RED_BAND_INDEX 0
/** Indice para la posicion de la banda para canal verde en la lista de bandas */
#define GREEN_BAND_INDEX 1
/** Indice para la posicion de la banda para canal azul en la lista de bandas */
#define BLUE_BAND_INDEX 2
/** Indice para la posicion de la banda en la lista escala de grises */
#define GREY_BAND_INDEX 0
/** Indice para indicar que la banda no esta seleccionada */
#define UNSET_BAND_INDEX -1
/** Indice del icono de la banda para canal rojo en la lista de iconos */
#define RED_ICON_INDEX RED_BAND_INDEX
/** Indice del icono de la banda para canal verde en la lista de iconos */
#define GREEN_ICON_INDEX GREEN_BAND_INDEX
/** Indice del icono de la banda para canal azul en la lista de iconos */
#define BLUE_ICON_INDEX BLUE_BAND_INDEX
/** Indice del icono de la banda en la lista de iconos de escala de grises */
#define GREY_ICON_INDEX GREY_BAND_INDEX

/**
 * Si esta definido: si se agrega una banda sin que haya ninguna posicion
 * seleccionada en la lista de salida, la banda se agrega en la posicion 0.
 * Si no esta definido: si se agrega una banda sin que haya ninguna posicion
 * seleccionada en la lista de salida, la banda no se agrega.
 */
#define __DEFAULT_BAND_POSITION__

/** inicializa los recursos de la ventana */
void InitBandCombinationSelectionXmlResource();

/** namespace suri */
namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(BandCombinationSelectionPartEvent, BandCombinationSelectionPart)IMPLEMENT_EVENT_CALLBACK(OnGreyScaleChecked, OnGreyScaleChecked(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnResize, OnResize(event), wxSizeEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Constructor
 * Guarda en una propiedad el elemento recibido, sobre el cual se efectua
 * el cambio en la combinacion de bandas.
 * @param[in] pElement puntero al elemento
 */
BandCombinationSelectionPart::BandCombinationSelectionPart(RasterElement * pElement) :
      BandSelectionPart(pElement), NEW_EVENT_OBJECT(BandCombinationSelectionPartEvent),
      pElement_(pElement), pRasterLayer_(NULL),
      pOutputList_(NULL) {
   windowTitle_ = _(caption_BAND_COMBINATION_SELECTION);
}


/** Guarda en una propiedad el elemento recibido, sobre el cual se efectua
 * el cambio en la combinacion de bandas.
 * @param[in] pRasterLayer puntero al elemento
 */
BandCombinationSelectionPart::BandCombinationSelectionPart(RasterLayer* pRasterLayer) :
      BandSelectionPart(pRasterLayer),
      NEW_EVENT_OBJECT(BandCombinationSelectionPartEvent), pElement_(NULL)
      , pRasterLayer_(pRasterLayer), pOutputList_(NULL) {
   windowTitle_ = _(caption_BAND_COMBINATION_SELECTION);
}

/**
 * Destructor
 */
BandCombinationSelectionPart::~BandCombinationSelectionPart() {
   DELETE_EVENT_OBJECT
   ;
}

/**
 * Agrega el panel con la lista de salida RGB y la lista en escala de grises.
 * @return true si puede crear la ventana
 * @return false si no puede crear la ventana
 *
 */
bool BandCombinationSelectionPart::CreateToolWindow() {
   if (!BandSelectionPart::CreateToolWindow()) {
      return false;
   }
   wxWindow * pparent = XRCCTRL(*pToolWindow_, wxT("ID_OUTPUT_BANDS_PANEL"),
         wxPanel);
   wxWindow * ptool = wxXmlResource::Get()->LoadPanel(
         pparent, wxT("ID_BAND_COMBINATION_PART"));

   if (!AddControl(ptool, pparent)) {
      return false;
   }

   ConfigureOutputBandList();
   SelectRgbBandList();

   // Conecta el evento al checkbox de escala de grises
   GET_CONTROL(*pToolWindow_, wxT("ID_GREYSCALE_CHECKBOX"), wxCheckBox)
      ->Connect(
            wxEVT_COMMAND_CHECKBOX_CLICKED,
            wxCommandEventHandler(BandCombinationSelectionPartEvent::OnGreyScaleChecked),
            NULL, pEventHandler_);

   GET_CONTROL(*pToolWindow_, wxT("ID_RGB_LIST"),
         wxListView)
      ->Connect(wxEVT_SIZE,
                wxSizeEventHandler(BandCombinationSelectionPartEvent::OnResize), NULL,
                pEventHandler_);

   GET_CONTROL(*pToolWindow_, wxT("ID_GREYSCALE_LIST"),
         wxListView)
      ->Connect(wxEVT_SIZE,
                wxSizeEventHandler(BandCombinationSelectionPartEvent::OnResize), NULL,
                pEventHandler_);
   return true;
}

/**
 * Devuelve el icono de la parte
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho del icono
 * @param[in] Y alto del icono
 */
void BandCombinationSelectionPart::GetWindowIcon(wxBitmap& ToolBitmap, int X,
                                                 int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_BAND_COMBINATION, ToolBitmap);
}

/**
 * Configura la lista de bandas de salida.
 * Crea las listas de iconos para asociar a las listas de bandas.
 * Agrega dos columnas en cada lista, una para iconos y una para las bandas.
 * Inserta 3 items en la lista RGB y un item en la lista escala de grises.
 */
void BandCombinationSelectionPart::ConfigureOutputBandList() {
   wxListCtrl *prgblist = XRCCTRL(*pToolWindow_, wxT("ID_RGB_LIST"), wxListView);
   if (!prgblist) {
      return;
   }

   // Creo la lista de imagenes para la lista de salida
   wxImageList *pimagergblist = new wxImageList(LIST_IMAGE_SIZE, LIST_IMAGE_SIZE);
   wxBitmap imageband;
   GET_BITMAP_RESOURCE(wxT(icon_IMAGE_BAND_RED), imageband);
   pimagergblist->Add(imageband);
   GET_BITMAP_RESOURCE(wxT(icon_IMAGE_BAND_GREEN), imageband);
   pimagergblist->Add(imageband);
   GET_BITMAP_RESOURCE(wxT(icon_IMAGE_BAND_BLUE), imageband);
   pimagergblist->Add(imageband);

   prgblist->AssignImageList(pimagergblist, wxIMAGE_LIST_SMALL);

   // ancho de la columna de imagenes de ambas listas.
   int columnimgwidth = LIST_IMAGE_SIZE + IMAGE_COLUMN_EXTRA_WIDTH;

   // Columna imagen
   prgblist->InsertColumn(IMAGE_COLUMN, wxEmptyString, wxLIST_FORMAT_CENTER,
                          columnimgwidth);

   // Columna nombres
   prgblist->InsertColumn(OUTPUT_BAND_NAME_COLUMN, _(label_OUTPUT_BANDLIST),
                          wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);

   prgblist->InsertItem(RED_BAND_INDEX, wxEmptyString, RED_ICON_INDEX);
   prgblist->InsertItem(GREEN_BAND_INDEX, wxEmptyString, GREEN_ICON_INDEX);
   prgblist->InsertItem(BLUE_BAND_INDEX, wxEmptyString, BLUE_ICON_INDEX);

   // Inserta un item en la lista de escala de grises
   wxListCtrl *pgreylist = XRCCTRL(*pToolWindow_, wxT("ID_GREYSCALE_LIST"), wxListView);
   if (!pgreylist) {
      return;
   }

   // Creo la lista de imagenes para la lista de salida
   GET_BITMAP_RESOURCE(wxT(icon_IMAGE_BAND_GREYSCALE), imageband);
   wxImageList *pimagegreylist = new wxImageList(LIST_IMAGE_SIZE, LIST_IMAGE_SIZE);
   pimagegreylist->Add(imageband);

   pgreylist->AssignImageList(pimagegreylist, wxIMAGE_LIST_SMALL);

   // Columna imagen
   pgreylist->InsertColumn(IMAGE_COLUMN, wxEmptyString, wxLIST_FORMAT_CENTER,
                           columnimgwidth);

   // Columna nombres
   pgreylist->InsertColumn(OUTPUT_BAND_NAME_COLUMN, wxEmptyString, wxLIST_FORMAT_LEFT,
                           wxLIST_AUTOSIZE);
   pgreylist->InsertItem(GREY_BAND_INDEX, wxEmptyString, GREY_ICON_INDEX);
}

/**
 * Vacia la lista con las bandas seleccionadas, e invoca a Rollback.
 */
void BandCombinationSelectionPart::SetInitialValues() {
   bandList_.clear();
   RollbackChanges();
}

/**
 * Limpia las dos listas (RGB y Escala de grises), poniendo los textos en blanco
 * y los itemdata con la seleccion de bandas en -1.
 * Si la propiedad con la lista de bandas tiene 1 solo item implica que hay
 * que seleccionar escala de grises. En otro caso (si tiene 3 items  o si no
 * tiene ningun item tambien) se selecciona la lista RGB.
 * Pasa los valores de la propiedad con las bandas seleccionadas a la lista wx.
 * @return true pudo pasar las bandas de la propiedad al control wx
 * @return false fallo pasar las bandas de la propiedad al control wx
 */
bool BandCombinationSelectionPart::RollbackChanges() {
   // Cantidad de items de la lista RGB, siempre 3
   size_t bandcountrgb = USE_CONTROL(*pToolWindow_, wxT("ID_RGB_LIST"),
         wxListView, GetItemCount(), wxNOT_FOUND);
   // Limpia la lista, nombre de banda en blanco e itemdata = -1 (UNSET_BAND_INDEX)
   for (size_t i = 0; i < bandcountrgb; i++) {
      GET_CONTROL(*pToolWindow_, wxT("ID_RGB_LIST"), wxListView)
         ->SetItem(i, OUTPUT_BAND_NAME_COLUMN, wxT(
               ""));
      GET_CONTROL(*pToolWindow_, wxT("ID_RGB_LIST"),
            wxListView)->SetItemData(i, UNSET_BAND_INDEX);
   }

   // Cantidad de items de la lista escala de grises, siempre 1
   size_t bandcountgrey = USE_CONTROL(*pToolWindow_, wxT("ID_GREYSCALE_LIST"),
         wxListView, GetItemCount(), wxNOT_FOUND);

   // Limpia la lista, nombre de banda en blanco e itemdata = -1 (UNSET_BAND_INDEX)
   for (size_t j = 0; j < bandcountgrey; j++) {
      GET_CONTROL(*pToolWindow_, wxT("ID_GREYSCALE_LIST"), wxListView)
         ->SetItem(j, OUTPUT_BAND_NAME_COLUMN, wxT(""));
      GET_CONTROL(*pToolWindow_, wxT("ID_GREYSCALE_LIST"),
            wxListView)->SetItemData(j, UNSET_BAND_INDEX);
   }

   // Obtiene combinacion de bandas del elemento y la guarda en la propiedad
   bandList_ = GetElementBandCombination();

   // Cantidad de items es igual a la de la lista escala de grises
   // Activa la lista escala de grises
   if (bandList_.size() == bandcountgrey) {
      GET_CONTROL(*pToolWindow_, wxT("ID_GREYSCALE_CHECKBOX"),
            wxCheckBox)->SetValue(true);
      SelectGreyScaleBandList();
   }
   // En cualquier otro caso activa la lista RGB
   else {
      SelectRgbBandList();
      GET_CONTROL(*pToolWindow_, wxT("ID_GREYSCALE_CHECKBOX"),
            wxCheckBox)->SetValue(false);
   }

   std::vector<int>::iterator iter = bandList_.begin();

   // Pasa los valores de la propiedad con la lista de bandas seleccionadas al control wx
   for (; iter != bandList_.end(); iter++)
      if (!AddOutputBand(*iter, GetNextInsertPosition())) {
         return false;
      }
   return true;
}

/**
 * Verifica que la cantidad de bandas seleccionadas en el control wx sea igual
 * a las requeridas segun la lista seleccionada (3 para RGB, 1 para escala de
 * grises)
 * @return true la cantidad de bandas es igual a la requerida por el control
 * @return false la cantidad de bandas es distinta a la requerida por el control
 */
bool BandCombinationSelectionPart::HasValidData() {
   unsigned int bandscount = pOutputList_->GetItemCount();
   std::vector<int> bandlist = GetOutputBands();

   if (bandlist.size() != bandscount) {
      return false;
   }
   return true;
}

/**
 * Devuelve si la parte tuvo cambios
 * @return modified_
 */
bool BandCombinationSelectionPart::HasChanged() {
   return modified_;
}

/**
 * Obtiene la lista de bandas seleccionadas en el control wx y las guarda en la
 * propiedad bandList_.
 * Invoca a SetBandCombinationNode, para que cuando se aplican los cambios estos
 * pasen al elemento.
 * Actualiza el viewer.
 * @return true si la parte tiene datos validos
 * @return false si la parte no tiene datos validos
 */
bool BandCombinationSelectionPart::CommitChanges() {
   if (!HasValidData()) {
      SHOW_ERROR(errorMessage_);
      return false;
   }

   if (modified_) {
      bandList_ = GetOutputBands();
   }

   SetBandCombinationNode();

   Element* pelement =
         pRasterLayer_ ? pRasterLayer_->GetElement() : pElement_;

   RemoveEnhancement(pelement);

   pelement->SetChanged();
   pelement->SendViewerUpdate();

   modified_ = false;

   return true;
}

/**
 * Metodo que se ejecuta ante el evento de seleccionar el CheckBox de escala
 * de grises. Invoca al metodo correspondiente para cambiar el puntero a la lista
 * activa (RGB o Escala de grises)
 * @param[in] Event evento seleccionar el checkbox
 */
void BandCombinationSelectionPart::OnGreyScaleChecked(wxCommandEvent &Event) {
   bool ischecked = USE_CONTROL(*pToolWindow_, wxT("ID_GREYSCALE_CHECKBOX"),
         wxCheckBox, IsChecked(), false);
   if (ischecked) {
      SelectGreyScaleBandList();
   } else {
      SelectRgbBandList();
   }
}

/**
 * Devuelve la posicion seleccionada de la lista, que es el indice donde se
 * inserta la siguiente banda.
 * Si no hay ninguna posicion seleccionada en la lista de salida el
 * comportamiento varia segun este definida DEFAULT_BAND_POSITION. Si esta
 * definida devuelve 0 y la banda se agrega en la primera posicion de la lista.
 * Si no esta definida devuelve -1 y la banda no se agrega.
 * @return position indice del item seleccionado
 */
int BandCombinationSelectionPart::GetNextInsertPosition() {
   int position = pOutputList_->GetFirstSelected();
#ifdef __DEFAULT_BAND_POSITION__
   if (position < 0) {
      position = 0;
      pOutputList_->Select(0);
   }
#endif
   return position;
}

/**
 * Agrega una banda en la posicion indicada. Agregar una banda implica setear el
 * texto con el nombre de la banda, y el itemdata con el numero de banda.
 * Invoca al metodo que selecciona la proxima posicion donde insertar.
 * @param[in] BandIndex numero de la banda que se agrega a la lista de salida
 * @param[in] Position posicion en la lista de salida en la que se agrega la banda
 * @return true si puede agregar.
 * @return false si la posicion para agregar es menor a cero.
 */
bool BandCombinationSelectionPart::AddOutputBand(int BandIndex, int Position) {
   if (Position < 0) {
      return false;
   }
   pOutputList_->SetItem(Position, OUTPUT_BAND_NAME_COLUMN, bandNames_[BandIndex]);
   pOutputList_->SetItemData(Position, BandIndex);
   SelectNextItem();

   return true;
}

/**
 * Obtiene el indice del item seleccionado y selecciona el item siguiente. Tiene
 * comportamiento circular, si el primero seleccionado es el ultimo de la lista
 * selecciona el primer item de la lista.
 */
void BandCombinationSelectionPart::SelectNextItem() {
   int position = pOutputList_->GetFirstSelected();
   int maxbands = pOutputList_->GetItemCount();
   position++;
   if (maxbands > 0) {
      pOutputList_->Select(position % maxbands);
   }
}

/**
 * Obtiene el nodo de renderizacion del elemento, si no existe lo crea.
 * Agrega el nodo combinacion con la combinacion de bandas seleccionadas,
 * si el nodo ya existe lo reemplaza.
 */
void BandCombinationSelectionPart::SetBandCombinationNode() {
   Element* pelement =
         pRasterLayer_ ? pRasterLayer_->GetElement() : pElement_;
   wxXmlNode *prnode = pelement->GetNode(RENDERIZATION_NODE);
   if (!prnode) {
      prnode = pElement_->AddNode(pelement->GetNode(wxT("")), RENDERIZATION_NODE);
   }
   wxXmlNode *pcombinationnode = CreateBandCombinationNode();
   // Agrega el nodo combinacion al nodo renderizacion
   pelement->AddNode(prnode, pcombinationnode);
}

/**
 * Obtiene la cantidad de items de la lista, segun la lista activa (RGB o Escala
 * de Grises). Obiene el numero de cada banda seleccionada y las agrega a un
 * vector de enteros. Si alguna banda tiene el numero UNSET_BAND_INDEX limpia
 * el vector y devuelve un vector vacio.
 * @return bandselection vector con las bandas seleccionadas (vacio si alguna
 * banda no esta seleccionada).
 */
std::vector<int> BandCombinationSelectionPart::GetOutputBands() {
   std::vector<int> bandselection;
   int bandcount = pOutputList_->GetItemCount();
   int bandselected = UNSET_BAND_INDEX;
   for (int bandchannel = 0; bandchannel < bandcount; bandchannel++) {
      bandselected = pOutputList_->GetItemData(bandchannel);
      if (bandselected == UNSET_BAND_INDEX) {
         bandselection.clear();
         break;
      }
      bandselection.push_back(bandselected);
   }

   return bandselection;
}

/**
 * Este metodo se ejecuta cuando alguna de las listas recibe el evento de
 * resize. Se invoca al metodo BandSelectionPart::AdjustLastColumn para ajustar
 * la ultima columna de ambas listas.
 * @param[in]	Event: evento
 * \note no utilizo el metodo wxEvent::GetEventObject ya que necesito ajustar
 * las columnas de ambas lista y no solo la que recibe el evento.
 */
void BandCombinationSelectionPart::OnResize(wxSizeEvent &Event) {
   AdjustLastColumn(XRCCTRL(*pToolWindow_, wxT("ID_RGB_LIST"), wxListView));
   AdjustLastColumn(XRCCTRL(*pToolWindow_, wxT("ID_GREYSCALE_LIST"), wxListView));
}

/** Informa si esta en escala de grises */
bool BandCombinationSelectionPart::IsGreyScale() {
   return USE_CONTROL(*pToolWindow_, wxT("ID_GREYSCALE_CHECKBOX"),
         wxCheckBox, IsChecked(), false);
}

/**
 * Metodo que se ejecuta para cambiar la lista activa, cuando se destilda
 * el checkbox escala de grises.
 * Asigna al puntero de la lista la lista activa.
 * Activa el control wx de la lista activa.
 * Desactiva el control wx de la lista inactiva.
 * Selecciona el primer elemento de la lista.
 */
void BandCombinationSelectionPart::SelectRgbBandList() {
   pOutputList_ = XRCCTRL(*pToolWindow_, "ID_RGB_LIST", wxListView);
   GET_CONTROL(*pToolWindow_, wxT("ID_GREYSCALE_LIST"), wxListView)->Enable(false);
   GET_CONTROL(*pToolWindow_, wxT("ID_RGB_LIST"), wxListView)->Enable(true);
   pOutputList_->Select(0);
}

/**
 * Metodo que se ejecuta para cambiar la lista activa, cuando se tilda
 * el checkbox escala de grises.
 * Asigna al puntero de la lista la lista activa.
 * Activa el control wx de la lista activa.
 * Desactiva el control wx de la lista inactiva.
 * Selecciona el primer elemento de la lista.
 */
void BandCombinationSelectionPart::SelectGreyScaleBandList() {
   pOutputList_ = XRCCTRL(*pToolWindow_, "ID_GREYSCALE_LIST", wxListView);
   GET_CONTROL(*pToolWindow_, wxT("ID_RGB_LIST"), wxListView)->Enable(false);
   GET_CONTROL(*pToolWindow_, wxT("ID_GREYSCALE_LIST"), wxListView)->Enable(true);
   pOutputList_->Select(0);
}

/**
 * Obtiene el nodo combinacion de bandas del elemento, obtiene la lista de
 * bandas, las transforma a entero y las guarda en un vector.
 * @return bandcombination vector de enteros con las bandas del nodo combinacion
 */
std::vector<int> BandCombinationSelectionPart::GetElementBandCombination() {
   std::vector<int> bandcombination;
   // Renderizo por si se eliminaron features de las capas.
   Element* pelement =
         pRasterLayer_ ? pRasterLayer_->GetElement() : pElement_;
   wxXmlNode *prnode = pelement->GetNode(RENDERIZATION_NODE);
   wxXmlNode *pcombinationnode = pelement->GetNode(BAND_COMBINATION_NODE, prnode);

   if (pcombinationnode) {
      wxXmlNode *pbandsnode = pcombinationnode->GetChildren();
      if (pbandsnode) {
         std::string bands = pbandsnode->GetContent().c_str();
         std::vector<std::string> bandlist = tokenizer(bands, " ");
         std::vector<std::string>::iterator iter = bandlist.begin();
         for (; iter != bandlist.end(); iter++)
            bandcombination.push_back(atoi((*iter).c_str()));
      }
   }
   return bandcombination;
}

/**
 * Elimina el tag de realce si existe.
 */
void BandCombinationSelectionPart::RemoveEnhancement(Element* pElement) {
   wxXmlNode* prender = pElement->GetNode(RENDERIZATION_NODE);
   if (prender != NULL) {
      std::stringstream ss;
      ss << RENDERIZATION_NODE << "|" << ENHANCEMENT_NODE;
      wxXmlNode* penhance = pElement->GetNode(ss.str().c_str());
      if (penhance != NULL) {
         pElement->RemoveNode(prender, penhance);
      }
   }
}

}
