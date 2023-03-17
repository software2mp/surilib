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
#include <string>
#include <list>
#include <vector>
#include <utility>

// Inclues suri
#include "ColorTableSelectionPart.h"
#include "ColorTableManager.h"
#include "suri/messages.h"
#include "suri/LibraryManagerFactory.h"
#include "BandCombinationAndEnhancementPart.h"

// Includes wx
#include "wx/xrc/xmlres.h"
#include "wx/sizer.h"

// Defines
#define ID_COLORTABLE_SELECTION_PANEL "ID_COLORTABLE_SELECTION_PANEL"
#define ID_CGU_PANEL "ID_CGU_PANEL"
#define ID_ENABLE_CHECK "ID_ENABLE_CHECK"

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ColorTableEventHandler, ColorTableSelectionPart)
      IMPLEMENT_EVENT_CALLBACK(OnColorTableSelectionCheck,
                               OnColorTableSelectionCheck(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT

/**
 *  Constructor legacy
 *  @param[in] pBandCombPart part que lo contiene
 *  @param[in] pManager administrador de items de biblioteca
 *  @param[in] LibraryId id de la biblioteca que corresponde a tabla de intervalos
 *  @param[in] pElement pElement elemento sobre el cual se podra aplicar tabla de intervalos
 *  @param[out] pElement elemento sobre el cual se podra aplicar tabla de colores
 */
ColorTableSelectionPart::ColorTableSelectionPart(BandCombinationAndEnhancementPart* pBandCombPart,
                                                 LibraryManager* pManager,
                                                 const SuriObject::UuidType& LibraryId,
                                                 RasterElement * pElement) :

      LibraryItemEditorPart(LibraryId,
                            pManager, ID_COLORTABLE_SELECTION_PANEL, ID_CGU_PANEL,
                            false,
                            UniversalGraphicalComponentPartInterface::ReadOnlyWithFullFledged),
                            NEW_EVENT_OBJECT(ColorTableEventHandler),
                            activate_(true),
                            pBandCombPart_(pBandCombPart),
                            pLayer_(NULL), pElement_(pElement),
                            pManager_(pManager) {
   windowTitle_ = _(caption_COLORTABLE_CHOICE_SELECTION);
   // el mensaje de error se asigna en el constructor porque hay uno solo
   errorMessage_ = message_COLORTABLE_CHOICE_ERROR;
}

/**
 *  Constructor
 *  @param[in] pBandCombPart part que lo contiene
 *  @param[in] pManager administrador de items de biblioteca
 *  @param[in] LibraryId id de la biblioteca que corresponde a tabla de intervalos
 *  @param[in] pLayer Capa sobre el cual se podra aplicar tabla de intervalos
 *  @param[out] pLayer Capa sobre el cual se podra aplicar tabla de colores
 */
ColorTableSelectionPart::ColorTableSelectionPart(BandCombinationAndEnhancementPart* pBandCombPart,
                                                 LibraryManager* pManager,
                                                 const SuriObject::UuidType& LibraryId,
                                                 RasterLayer * pLayer) :
      LibraryItemEditorPart(LibraryId, pManager, ID_COLORTABLE_SELECTION_PANEL,
                            ID_CGU_PANEL,
                            false,
                            UniversalGraphicalComponentPartInterface::
                            ReadOnlyWithFullFledged),
                            NEW_EVENT_OBJECT(ColorTableEventHandler),
                            activate_(true),
                            pBandCombPart_(pBandCombPart),
                            pLayer_(pLayer), pElement_(NULL),
                            pManager_(pManager) {
   windowTitle_ = _(caption_COLORTABLE_CHOICE_SELECTION);
   // el mensaje de error se asigna en el constructor porque hay uno solo
   errorMessage_ = message_COLORTABLE_CHOICE_ERROR;
}

/** Dtor */
ColorTableSelectionPart::~ColorTableSelectionPart() {
   DELETE_EVENT_OBJECT;
}

/** Inicializa el part */
void ColorTableSelectionPart::SetInitialValues() {
   pCgu_->SetInitialValues();
   Element* pelement = pLayer_ ? pLayer_->GetElement() : pElement_;
   if (pelement) {
      wxXmlNode* ptable = pelement->GetNode(RENDERIZATION_NODE NODE_SEPARATION_TOKEN COLOR_TABLE);
      wxString value;
      if (ptable && ptable->GetPropVal(NAME_NODE, &value) && pCgu_) {
         pCgu_->SelectItemByPrincipal(value.c_str());
         GET_CONTROL(*pToolWindow_, wxT(ID_ENABLE_CHECK), wxCheckBox)->SetValue(true);
      }
   }
}
/**
 * Genera una Lut soportada por el sistema. El nombre debe estar entre los que
 * devuelve GetLutNames .
 * @param[in] LutName nombre de la lut que quiero
 * @param[in] Active indica si la tabla de colores debe estar activa, si es
 * false pNewLutNode queda null.
 * @param[out] pNewLutNode puntero al nodo generado por la lut de nombre
 * indicado
 * @return informa si pudo generar la lut.
 */
bool ColorTableSelectionPart::CreateLutNode(const std::string &LutName, bool Active,
                                            wxXmlNode* &pNewLutNode) {
   pNewLutNode = NULL;
   if (!Active) {
      return true;
   }

   const ColorTable* pcolortable;
   if (!ColorTableManager::Instance().GetColorTable(pManager_, LutName, pcolortable)) {
      return false;
   }
   ColorTable colortable = *pcolortable;
   colortable.Activate(Active);
   std::vector<std::pair<std::string, std::string> > labels;
   ColorTableManager::Instance().GetColorTableLabels(pManager_, LutName, labels);
   return ColorTable::ToXml(colortable, pNewLutNode, labels);
}

/**  Metodo que obtiene a partir de los datos del editor, un vector con los atributos
 *   Este metodo es el que hace el comportamiento particular de cada editor a la hora
 *   de obtener la informacion que posee y transmitirla al sistema de bibliotecas.
 *   Es utilizado a modo de Template Method (en CommitChanges y en el notificador)
 *   @return vector con atributos
 *   @return NULL en caso de que la informnacion del part no sea valida
 *   **/
std::vector<LibraryItemAttribute*>* ColorTableSelectionPart::CreateAttributesFromFields() {
   return NULL;
}

/**
 *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
 *  Este metodo es el que realmente tienen que implementar las clases hijas para
 * lograr la especializacion a la hora de iniciar el part (metodo que se llama
 * en CreateToolWindow, a modo de Template Method).
 **/
bool ColorTableSelectionPart::ConfigureFeatures() {
   GET_CONTROL(*pToolWindow_, wxT(ID_ENABLE_CHECK), wxCheckBox)->Connect(
            wxEVT_COMMAND_CHECKBOX_CLICKED,
            wxCommandEventHandler(ColorTableEventHandler::OnColorTableSelectionCheck),
            NULL, pEventHandler_);
   pCgu_->Disable();
   modified_ = false;
   return true;
}

/**
 * Metodo auxiliar privado que es necesario implementar en las clases hijas.
 * Este metodo es el encargado de actualizar realmente la informacion en el part
 * Es llamado en el metodo SetActiveItem a modo de Template Method
 **/
void ColorTableSelectionPart::UpdateFields(const LibraryItem* pItem, bool Modifiable) {
}

/**
 * Metodo auxiliar privado que agrega comportamiento adicional al commit de la
 * clase base. Este comportamiento adicional es necesario cuando
 * el commit involucra objetos adicionales a los items de biblioteca
 * del editor (ej. reproyeccion, en donde se necesita actualizar los params
 * de reproyeccion ademas de leer el item de biblioteca)
 * @return true en caso de realizar correctamente el commit "interno"
 * @return false en caso de que ocurra algun error durante el commit
 */
bool ColorTableSelectionPart::OnCommitChanges() {
   /** nodo que se genera con la tabla de colores **/
   wxXmlNode *pnode = NULL;
   pCgu_->CommitChanges();
   const LibraryItem* pselected = pCgu_->GetSelectedItem();
   if (!pselected)
      return false;
   std::string lutname = pselected->GetPrincipalAttribute()->GetValue();
   wxCheckBox* pcheck = XRCCTRL(*pToolWindow_, wxT(ID_ENABLE_CHECK), wxCheckBox);
   bool active = pcheck->GetValue();
   Element* pelement = pLayer_ ? pLayer_->GetElement() : pElement_;
   bool returnvalue = CreateLutNode(lutname, active, pnode) && pelement;
   if (returnvalue) {
      wxXmlNode* pparent = pelement->GetNode(RENDERIZATION_NODE);
      wxXmlNode* poldnode = pelement->GetNode(COLOR_TABLE, pparent);
      pelement->RemoveNode(pparent, poldnode);
      wxXmlNode* pchild = pparent->GetChildren();
      wxXmlNode* pmulnode = NULL;
      wxXmlNode* pcastnode = NULL;
      while (pchild != NULL) {
         if (pchild->GetName().compare(DATA_CAST_NODE) == 0) {
            pcastnode = pchild;
         } else if (pchild->GetName().compare(BAND_MULTIPLEXOR_NODE) == 0) {
            pmulnode = pchild;
            break;
         }
         pchild = pchild->GetNext();
      }
      if (pnode && pmulnode)
         pparent->RemoveChild(pmulnode);
      if (pnode && pcastnode) {
         pparent->InsertChild(pnode, pcastnode);
      } else if (pnode) {
         /** En caso de que no exista el nodo de casteo se inserta luego del nodo de interpolacion**/
         wxXmlNode* pinterpolationnode =
                     pelement->GetNode(RENDERIZATION_NODE NODE_SEPARATION_TOKEN INTERPOLATION_NODE);
         if (pinterpolationnode)
            pparent->InsertChild(pnode, pinterpolationnode);
      }  
   }
   return returnvalue;
}

/** Deshabilita la herramienta de realces. */
void ColorTableSelectionPart::OnColorTableSelectionCheck(wxCommandEvent &Event) {
   ActivateSelection(Event.IsChecked());
   if (Event.IsChecked())
      pBandCombPart_->ActivatEenhancement(false);
   modified_ = true;
   Event.Skip();
}

/** Metodo que activa o desactiva los componontes que contiene el **/
void ColorTableSelectionPart::Activate(bool Activate) {
   activate_ = Activate;
   GET_CONTROL(*pToolWindow_, wxT(ID_ENABLE_CHECK), wxCheckBox)->Enable(Activate);
   ActivateSelection(Activate);
   modified_ = false;
}

/** Metodo que tilda/destilda el check */
void ColorTableSelectionPart::Check(bool Check) {
   if (activate_)
      GET_CONTROL(*pToolWindow_, wxT(ID_ENABLE_CHECK), wxCheckBox)->SetValue(Check);
   ActivateSelection(Check);
   modified_ = true;
}

/** Metodo que activa o desactiva el uso de tabla de colores **/
void ColorTableSelectionPart::ActivateSelection(bool Activate) {
   if (Activate) {
      pCgu_->Enable();
   } else {
      pCgu_->Disable();
   }
}
} /** namespace suri **/
