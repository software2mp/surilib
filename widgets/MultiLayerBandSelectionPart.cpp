/** \file MultiLayerBandSelectionPart.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "MultiLayerBandSelectionPart.h"

#include <string>
#include <vector>

#include "wx/imaglist.h"
#include "wx/font.h"

#include "resources.h"

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(MultiLayerBandSelectionEvent, MultiLayerBandSelectionPart)
      IMPLEMENT_EVENT_CALLBACK(OnTreeItemActivated, OnTreeItemActivated(event), wxTreeEvent)
END_IMPLEMENT_EVENT_OBJECT

/**
 * Ctor
 */
MultiLayerBandSelectionPart::MultiLayerBandSelectionPart(
      const std::vector<RasterElement*>& ElementsP) :
      NEW_EVENT_OBJECT(MultiLayerBandSelectionEvent) {
   this->hasBaseBand_ = false;
   this->pElements_ = ElementsP;
}

/**
 * Dtor
 */
MultiLayerBandSelectionPart::~MultiLayerBandSelectionPart() {
   //DELETE_EVENT_OBJECT  Va?
}

/**
 * Crea la ventana de la herramienta
 *
 * @return true si pudo crear el part.
 */
bool MultiLayerBandSelectionPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_MULTI_BAND_SELECTION_PART"));
   if (!pToolWindow_) {
      return false;
   }

   ConfigureTreeControl();

   LoadTreeControl(pElements_);

   return true;
}


/**
 * Devuelve la lista de bandas seleccionadas en la lista de entrada
 *
 * @return Devuelve todas las bandas seleccionadas por el usuario.
 * @see MultiLayerBandSelectionPart::GetAvailableBands
 */
std::vector<BandInfo> MultiLayerBandSelectionPart::GetInputSelectedBands() {
   wxTreeCtrl* ptree = XRCCTRL(*pToolWindow_, wxT("ID_MULTI_SELECTION_TREE"),
                               wxTreeCtrl);

   std::vector<BandInfo> retbands;

   wxTreeItemId selitem = ptree->GetSelection();
   wxTreeItemData* pdata = ptree->GetItemData(selitem);
   if (pdata != NULL) {
      BandInfo info = static_cast<CustomItemData*>(pdata)->GetBandInfo();
      int valid = 1;
      info.GetElement()->GetBandValidity(valid, info.GetPosition());
      if (valid == 0)
         SHOW_WARNING(message_INVALID_BAND_NAME_SELECTED, info.GetName().c_str());
      else 
         retbands.push_back(info);
   }

   return retbands;
}

/**
 * Devuelve el nombre de todas las banda disponibles.
 *
 * @return Devuelve todas las bandas disponibles para usar.
 *
 * \todo (danieltesta-#2961): Todas las bandas disponibles dependen de la
 * la primera que se seleccione ya que para poder hacer el apilamiento
 * todas las bandas tiene que tener las mismas dimensiones y tipo.
 */
std::vector<BandInfo> MultiLayerBandSelectionPart::GetAvailableBands() const {
   return std::vector<BandInfo>();
}

/**
 * Realiza ajustes en el part. La mensaje se recibe desde el control
 * que lo contiene.
 */
void MultiLayerBandSelectionPart::UpdateSourceCtrl() {
}

/**
 * Maneja la notificacion de que la lista (o control) de destino
 * esta vacia.
 */
void MultiLayerBandSelectionPart::OnOutputEmpty() {
   hasBaseBand_ = false;
   UpdateItemsStatus(
         XRCCTRL(*pToolWindow_, wxT("ID_MULTI_SELECTION_TREE"), wxTreeCtrl)->GetRootItem());
}

/**
 * Maneja la notificacion de que el control de destino esta
 * solicitando elementos de entrada.
 */
void MultiLayerBandSelectionPart::OnInputRequested() {
   wxTreeEvent Event; // necesario para la llamada al metodo.
   OnTreeItemActivated(Event);
}

/**
 * Configura el control wxTreeCtrl.
 */
void MultiLayerBandSelectionPart::ConfigureTreeControl() {
   wxTreeCtrl* ptree = XRCCTRL(*pToolWindow_, wxT("ID_MULTI_SELECTION_TREE"),
                               wxTreeCtrl);
   if (ptree != NULL) {
      wxBitmap imageband;
      wxImageList *pimageinlist = new wxImageList(16, 16);

      GET_BITMAP_RESOURCE(wxT(icon_DRIVE_16), imageband);
      pimageinlist->Add(imageband);

      GET_BITMAP_RESOURCE(wxT(icon_RASTER_ELEMENT_16), imageband);
      pimageinlist->Add(imageband);

      GET_BITMAP_RESOURCE(wxT(icon_IMAGE_BAND_GREYSCALE), imageband);
      pimageinlist->Add(imageband);
      wxBitmap invalidimageband;
      GET_BITMAP_RESOURCE(wxT(icon_IMAGE_INVALID_BAND_GREY_SCALE), invalidimageband);
      pimageinlist->Add(invalidimageband);

      ptree->AssignImageList(pimageinlist);

      ptree->Connect(
            wxEVT_COMMAND_TREE_ITEM_ACTIVATED,
            wxTreeEventHandler(MultiLayerBandSelectionEvent::OnTreeItemActivated), NULL,
            pEventHandler_);
   }
}

/**
 * Carga el control con los datos de cada imagen.
 *
 * @param[in] pElementsP Vector de RasterElements desde los cuales se toman los datos
 *    para cargar el control.
 */
void MultiLayerBandSelectionPart::LoadTreeControl(
      const std::vector<RasterElement*>& pElementsP) {
   wxTreeCtrl* ptree = XRCCTRL(*pToolWindow_, wxT("ID_MULTI_SELECTION_TREE"),
                               wxTreeCtrl);
   if (ptree != NULL) {
      wxTreeItemId rootnodeix = ptree->AddRoot(wxT("Imagenes seleccionadas"), 0);
      for (int ix = 0, lenix = pElementsP.size(); ix < lenix; ++ix) {
         wxString name = pElementsP[ix]->GetName();
         wxTreeItemId parentNodeIndex = ptree->AppendItem(rootnodeix, name, 1);
         ptree->SetItemData(parentNodeIndex, NULL);
         for (int jx = 0, lenjx = pElementsP[ix]->GetBandCount(); jx < lenjx; ++jx) {
            std::stringstream ss;

            std::string bandname;
            pElementsP[ix]->GetBandName(bandname, jx);
            int valid = 1;
            pElementsP[ix]->GetBandValidity(valid, jx);
            ss << bandname << " [" << pElementsP[ix]->GetName().c_str() << "]";

            wxTreeItemId lastpost = ptree->AppendItem(parentNodeIndex,
                                                      wxT(bandname.c_str()), valid == 1 ? 2 : 3);

            int width = 0, height = 0;
            pElementsP[ix]->GetRasterSize(width, height);

            std::string type = BandInfo::ExtractType(pElementsP[ix]);

            BandInfo pbandinfo(pElementsP[ix], jx, ss.str(), width,
                                               height, type);
            CustomItemData* pitemdata = new CustomItemData(pbandinfo);

            ptree->SetItemData(lastpost, pitemdata);
         }
      }
      ptree->ExpandAll();
   }
}

/**
 * Recibe el mensaje cuando se activo un item con doble-click.
 *
 * @param[in] Event Datos del evento.
 */
void MultiLayerBandSelectionPart::OnTreeItemActivated(wxTreeEvent &Event) {
   std::vector<BandInfo> bands = GetInputSelectedBands();
   if (!bands.empty() && hasBaseBand_) {
      if (pBaseBand_.GetWidth() != bands[0].GetWidth() ||
            pBaseBand_.GetHeight() != bands[0].GetHeight() ||
            pBaseBand_.GetType().compare(bands[0].GetType()) != 0) {
         return;
      }
   }


   EventListener* plistener = GetInputBandListener();
   if (plistener != NULL) plistener->EventEmitted(this);

   if (!bands.empty() && !hasBaseBand_) {
      pBaseBand_ = bands[0];
      hasBaseBand_ = true;

      wxTreeCtrl* ptree = XRCCTRL(*pToolWindow_, wxT("ID_MULTI_SELECTION_TREE"),
                                  wxTreeCtrl);

      wxTreeItemId rootid = ptree->GetRootItem();
      UpdateItemsStatus(rootid);
   }
}

/**
 * Actualiza el estado de los items poniendo en cursiva y deshabilitando
 * aquellos que no son compatibles.
 *
 * @param[in] ParentItemId Item desde el cual chequear el estado.
 */
void MultiLayerBandSelectionPart::UpdateItemsStatus(wxTreeItemId ParentItemId) {
   wxTreeCtrl* ptree = XRCCTRL(*pToolWindow_, wxT("ID_MULTI_SELECTION_TREE"),
                                  wxTreeCtrl);

   wxTreeItemIdValue cookie; // Esta variable es necesaria para que funciones wx
   wxTreeItemId firstchildid;
   wxTreeItemId nextsiblingid;
   wxTreeItemId parentid;

   // Si tiene hijo verifico los hijos primero
   if (ptree->ItemHasChildren(ParentItemId)) {
      firstchildid = ptree->GetFirstChild(ParentItemId, cookie);
      UpdateItemsStatus(firstchildid);
   }

   // Verifico el item actual. La activacion, o desactivacion, se hace sobre
   // el item y su padre (Ver comportamiento en SuriPluma)
   wxTreeItemData* prawdata = ptree->GetItemData(ParentItemId);
   if (prawdata != NULL) {
      CustomItemData* pcustdata = dynamic_cast<CustomItemData*>(prawdata);
      if (pcustdata != NULL) {
         wxFont font;
         parentid = ptree->GetItemParent(ParentItemId);
         if (hasBaseBand_ && (pBaseBand_.GetWidth() != pcustdata->GetBandInfo().GetWidth() ||
               pBaseBand_.GetHeight() != pcustdata->GetBandInfo().GetHeight() ||
               pBaseBand_.GetType().compare(pcustdata->GetBandInfo().GetType()) != 0)) {
            font.SetStyle(wxFONTSTYLE_ITALIC);
         } else {
            font.SetStyle(wxFONTSTYLE_NORMAL);
         }
         ptree->SetItemFont(ParentItemId, font);
         ptree->SetItemFont(parentid, font);
      }
   }

   // Chequea hermanos.
   nextsiblingid = ptree->GetNextSibling(ParentItemId);
   if (nextsiblingid > 0) {
      UpdateItemsStatus(nextsiblingid);
   }
}

}  // namespace suri
