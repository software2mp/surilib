/*! \file LutSelectionPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "LutSelectionPart.h"

// Inclues suri
#include "suri/LutArray.h"
#include "suri/messages.h"
#include "LutRenderer.h"
#include "suri/ViewcontextInterface.h"
#include "RasterLayer.h"

// Includes wx
#include "wx/xrc/xmlres.h"

/** namespace suri */
namespace suri {
/** Ctor */
LutSelectionPart::LutSelectionPart(RasterElement * pElement) :
      ChoiceSelectionPart(pElement) {
}


/** Ctor */
LutSelectionPart::LutSelectionPart(RasterLayer * pLayer) :
      ChoiceSelectionPart(pLayer) {
}

/** Dtor */
LutSelectionPart::~LutSelectionPart() {
}

/**
 * Actualiza el contenido del choice con las luts obtenidas con GetLutNames
 * y la lut del elemento(si hay una). Luego llama a ConfigureChoiceList y
 * ActivateChoice para actualizar la pantalla.
 * @return bool, indica si tuvo exito la operacion
 */
bool LutSelectionPart::ResetList() {
   // Limpio el contenido de la lista
   supportedLuts_.clear();
   choiceContent_.clear();

   // Carga el contenido del choice con las luts soportadas.
   std::list<std::string> lutnames = GetLutNames();
   std::list<std::string>::iterator it = lutnames.begin();
   for (int i = 0; it != lutnames.end(); it++, i++) {
      supportedLuts_.push_back(*it);
      choiceContent_.push_back(_(it->c_str() ));
   }
   selectedItem_ = 0;

   // Agrega lut del elemento al choice. isActive_ se actualiza en
   // AddElementLutToChoice si hay una lut activa.
   isActive_ = false;
   AddElementLutToChoice();

   // Actualiza el part
   ConfigureChoiceList();
   ActivateChoice(isActive_);
   return true;
}

/**
 * Reemplaza(o crea) el nodo LUT o tabladecolores que corresponda dentro de nodo
 * renderizacion del elemento, con la tabla que selecciono el usuario en el choice.
 * Si solo se modifico el estado de la tabla seleccionada no se recalcula a
 * menos que refresh_==true, en caso que se desactive la tabla se ignora el item
 * seleccionado y se asume que son iguales(Ver tabla de verdad en TCK #651).
 * @return informa si pudo actualizar el nodo
 */
bool LutSelectionPart::ConfigureElement() {
   // Busca el nodo lut en el elemento
   wxXmlNode* plutnode = NULL;
   bool haslut = GetLutNode(plutnode);
   bool ischoiceactive = IsChoiceActive();
   int selecteditem = GetSelectedItem();

   bool returnvalue = false;
   wxXmlNode* pnewlutnode = NULL;
   // En TCK #651 existe una tabla de verdad con las posibles situaciones
   if ((haslut && (selectedItem_ != selecteditem && ischoiceactive))
         || (!haslut && ischoiceactive)) {
      // Si se activa un elemento trata de recalcular, si no puede falla
      if (selecteditem < (int) supportedLuts_.size()) {
         returnvalue = CreateLutNode(supportedLuts_[selecteditem], ischoiceactive,
                                     pnewlutnode);
      }
   } else {
      if (refresh_) {
         // Si esta activo refresh recalcula, si no es soportado elimina el nodo
         returnvalue = true;
         if (selecteditem < (int) supportedLuts_.size()) {
            CreateLutNode(supportedLuts_[selecteditem], ischoiceactive, pnewlutnode);
         }
      } else {
         // En cualquier otro caso usa nodo anterior
         returnvalue = CreateLutNode(plutnode, ischoiceactive, pnewlutnode);
      }
   }

   // Actualiza el nodo del elemento
   if (returnvalue && !SetLutNode(pnewlutnode)) {
      delete pnewlutnode;
   }

   return returnvalue;
}

/**
 * Si encuentra un nodo lut en el elemento lo agrega al choice y lo configura
 * como seleccionado.
 * @return bool que indica si la tabla tenia nodo lut
 */
bool LutSelectionPart::AddElementLutToChoice() {
   // Busca el nodo de la lut en el elemento(GetLutNode(plutnode))
   wxXmlNode* plutnode = NULL;
   if (!GetLutNode(plutnode)) {
      return true;
   }

   // Busca el nodo entre luts registradas
   std::string lutname;
   if (!GetElementLutName(plutnode, lutname)) {
      if (lutname.empty()) {
         lutname = _(label_EMPTY_LUT);
      }
      wxString tlutname = _(lutname.c_str() );
      tlutname += _(label_ELEMENT_LUT_COMMENT);
      choiceContent_.push_back(tlutname);
      selectedItem_ = choiceContent_.size() - 1;
   } else if (!GetLutPositionInChoice(lutname, selectedItem_)) {
      REPORT_DEBUG("D: Hay una lut soportada que no esta cargada en choice");
      return false;
   }
   isActive_ = IsActive(plutnode);
   return true;
}

/**
 * Busca si en el elemento hay una LUT de tipo correcto o sin tipo y con la
 * estructura correcta.
 * @param[out] pLutNode nodo de la lut. Si no hay tabla de colores lo setea
 * en NULL. Se retorna el nodo interno, no eliminar.
 * @return bool que indica si la operacion tuvo exito
 */
bool LutSelectionPart::GetLutNode(wxXmlNode* &pLutNode) {
   pLutNode = NULL;

   // Verifico que tengo un elemento valido
   Element* pelement = NULL;
   if (pElement_ != NULL) {
      pelement = pElement_;
   } else {
      if (pRasterLayer_ != NULL) {
         pelement = pRasterLayer_->GetElement();
         if (pelement == NULL) {
            return false;
         }
      } else {
         return false;
      }
   }

   // Recorre los nodos debajo de <renderizacion> de tipo LUT
   wxString nodepath = wxString(wxT(RENDERIZATION_NODE)) + wxT(NODE_SEPARATION_TOKEN)
         + wxT(GetNodeName().c_str());
   wxXmlNode *plutnode = pelement->GetNode(nodepath);

   pLutNode = plutnode;
   return plutnode != NULL;
}

/**
 * Remplaza/agrega la lut que recibe por parametro en el nodo renderizacion del
 * elemento.
 * @param[in] pLutNode nodo con la nueva lut. Si la operacion tuvo exito el nodo
 * pasa a ser responsabilidad del elemento. Si es NULL solo se elimina
 * en nodo anterior.
 * @return bool que indica si la operacion tuvo exito
 */
bool LutSelectionPart::SetLutNode(wxXmlNode* pLutNode) {
   // Verifico que pElement este configurado
   Element* pelement = NULL;
   if (pElement_ != NULL) {
      pelement = pElement_;      
   } else {
      if (pRasterLayer_ != NULL) {
         pelement = pRasterLayer_->GetElement();
         if (pelement == NULL){
            return false;
         }
      } else {
         return false;
      }
   }

   // Obtengo el nodo de renderizacion
   // Recorre los nodos debajo de <renderizacion> de tipo LUT

   wxXmlNode *prenderzationnode = pelement->GetNode(wxString(wxT(RENDERIZATION_NODE)));
   if (!prenderzationnode) {
      REPORT_DEBUG("D: El elemento no tiene nodo de renderizacion");
      return false;
   }

   // Busca el nodo de la lut en el elemento(GetLutNode(plutnode))
   wxXmlNode* plutnode = NULL;
   if (GetLutNode(plutnode))
      pelement->ReplaceNode(prenderzationnode, pLutNode, plutnode);
   else
      SetNewLutNode(pelement, prenderzationnode, pLutNode);
   return true;
}

/**
 * Si el elemento seleccionado no es soportado inactiva boton refresh
 */
void LutSelectionPart::OnUpdate() {
   int selecteditem = GetSelectedItem();
   bool state = (selecteditem < (int) supportedLuts_.size());
   // Cambia el estado de habilitacion del boton
   if (IsChoiceActive() && USE_CONTROL(*pToolWindow_, wxT("ID_REFRESH_BUTTON"),
         wxBitmapButton, IsShown(), false)) {
      GET_CONTROL(*pToolWindow_, wxT("ID_REFRESH_BUTTON"), wxBitmapButton)
         ->Enable(state);
   }
}

/**
 * Busca la posicion de una lut en suportedLuts_ usando el nombre.
 * @param[in] Name nombre de la lut que busco en suportedLuts_
 * @param[out] Position posicion de la lut en suportedLuts_
 * @return indica si se encontro la lut en suportedLuts_.
 */
bool LutSelectionPart::GetLutPositionInChoice(const std::string &Name, int &Position) {
   int size = supportedLuts_.size();
   bool found = false;
   for (int i = 0; i < size && !found; i++)
      if (supportedLuts_[i] == Name) {
         Position = i;
         found = true;
      }

   return found;
}
}
