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
#include <algorithm>
#include <list>
#include <set>
#include <map>
#include <utility>

// Includes Suri
#include "suri/VectorEditionTool.h"
#include "TablePartConfigurator.h"
#include "suri/BufferedDriver.h"
#include "TablePart.h"
#include "suri/VectorEditionTable.h"
#include "suri/LayerHtmlTreeNodeHandler.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/ViewcontextManagerInterface.h"
#include "suri/GroupManagerInterface.h"
#include "suri/Configuration.h"
#include "resources.h"
#include "GuiHtmlEditor.h"
#include "logmacros.h"
#include "TreeNodeAuxiliaryFunctions.h"
#include "suri/ElementPropertiesWidget.h"
#include "VectorDatasource.h"
#include "TreeNodeDoubleClickNotification.h"
#include "suri/DataViewManager.h"
#include "EnclosureManipulator.h"
#include "EnclosureValidator.h"
#include "VectorEditorButtonTool.h"
#include "DefaultTreeNode.h"
#include "suri/LayerStateChangeNotification.h"
#include "VectorTablesPart.h"
#include "suri/ManualEnhancementPart.h"
#include "RasterDatasource.h"
#include "suri/LayerInterface.h"
#include "suri/TerrainElement.h"
#include "Event.h"

#include "RasterLayer.h"
#include "VectorLayer.h"

#include "wx/wx.h"
#include "wx/xrc/xmlres.h"
#include "wx/fs_mem.h"

// Longitud de pixeles x caracter, si se cambia el font, tomar en cuenta cambiar este valor.
#ifdef __WINDOWS__
#define PIXEL_LENGTH 8
#else
#define PIXEL_LENGTH 9
#endif

/** Right Mode */
#define GROUP_HTML \
"<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\"\"  __BGCOLOR__>" \
"  <tr>" \
"    <td>" \
"      <table border=\"0\"  cellspacing=\"0\" cellpadding=\"0\">" \
"        <tr>" \
"           <td>" \
"             <a href=\"expand\">" \
"               <img src=\"__ICON__\">" \
"             </a>" \
"           </td>" \
"        </tr>" \
"      </table>" \
"    </td>" \
"    <td width=\"100%\">" \
"       <table border=\"0\" width=\"100%\" cellspacing=\"0\" cellpadding=\"0\">" \
"          <tr>" \
"             <td>" \
"                <font  __FCOLOR__ > &nbsp;&nbsp;__NAME__ </font>" \
"             </td>" \
"          </tr>" \
"       </table>" \
"    </td>" \
"    __CHECKBOXHTML__" \
"  </tr>" \
"</table>"

#define SOPI_ELEMENT_HTML \
"<table width=\"100%\" border=\"0\" cellspacing=\"1\" cellpadding=\"1\" __BGCOLOR__ >" \
"<tr>" \
"    <td nowrap width=\"24\" height=\"24\">" \
"       <img width=\"16\" height=\"16\" src=\"__ICON__\">" \
"    </td>" \
"    <td width=\"100%\">" \
"       <font  __FCOLOR__ > __NAME__ </font>" \
"    </td>" \
"    __CHECKBOXHTML__" \
"    __NODE_INFORMATION__ "\
"</tr>" \
"</table>"

/** Modelo HTML para mostrar el checkbox a la derecha y abajo del nombre.*/
#define SIMUR_ELEMENT_HTML \
"<table width=\"100%\" border=\"0\" cellspacing=\"1\" cellpadding=\"1\" __BGCOLOR__ >" \
"<tr>" \
"    <td nowrap width=\"32\" height=\"32\">" \
"       <img width=\"32\" height=\"32\" src=\"__ICON__\">" \
"    </td>" \
"    <td width=\"100%\">" \
"       <font  __FCOLOR__ > __NAME__ </font>" \
"    </td>" \
"    __CHECKBOXHTML__" \
"</tr>" \
"</table>"

#define CHECKBOXHTML \
"    <td nowrap width=\"24\" height=\"24\">" \
"       <a href=\"checkbox\">" \
"         <img src=\"__CHOICE__\">" \
"       </a>" \
"    </td>"

#define CHECKBOXHTML_B \
"                <a href=\"checkbox\">" \
"                   <img width=\"16\" height=\"16\" src=\"__CHOICE__\">" \
"                </a>&nbsp;"

/** Se define un nivel para que un nodo hoja pueda tener uno varios subitems asociados
 *  sin la necesidad de que sea un nodo padre (no seria necesariamente un nodo hijo sino 
 *  un dato anexo al item)
 */ 
#define NODE_INFORMATION_TAG "__NODE_INFORMATION__"

#define SUBITEM \
"<tr>"\
"     <!-- SubItem-->" \
"     <table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">" \
"        <tr>" \
"        <td>" \
"           <table width=\"16px\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\"></table>" \
"        </td>" \
"        <td width=\"100%\">__SUBITEM_NODE__" \
"        </td>" \
"     </tr>" \
"  </table>"\
"</tr> "

#define SUBITEM_NODE "__SUBITEM_NODE__"   

#define VECTOR_SELECTION_SUB_ITEM_NODE \
"<table border=\"0\" width=\"100%\" cellspacing=\"0\" cellpadding=\"0\"> " \
"  <tr> " \
"    <td> "\
"         <img width=\"16\" height=\"16\" src=\"__SELECTION_ICON_SRC__\" > " \
"    </td> "\
"    <td width=\"100%\"> "\
"        &nbsp;&nbsp;<font  __FCOLOR__ > Seleccion </font>" \
"     </td>" \
"  </tr>" \
"</table>"

#define SELECTION_ICON_SRC          "__SELECTION_ICON_SRC__"

#define BACKGROUND_COLOR_DISABLED   "bgcolor=\"#DBDBDB\""
#define FONT_COLOR_DISABLED         "color=\"#B0B0B0\""

#define NAME_FLAG                   "__NAME__"
#define ICON_FLAG                   "__ICON__"
#define CHECKBOXHTML_FLAG           "__CHECKBOXHTML__"
#define CHECKBOXHTML_B_FLAG         "__CHECKBOXHTML_B__"

#define CHOICE_FLAG                 "__CHOICE__"
#define FONT_COLOR_TAG              "__FCOLOR__"
#define BG_COLOR_TAG                "__BGCOLOR__"

#define LEVELOFFSET 16
#if defined(__LINUX__)
#define NODEOFFSET 32
#else
#define NODEOFFSET 28
#endif

#define CHECKOFFSET        20
#define ICON_SIZE          16
#define SIMUR_ICON_SIZE    32

namespace {

std::string GetMultilineHtmlText_Sopi(const std::string& Text, int ChrsPerLine) {
   std::string retstr;

   int textix = 0;
   int lineix = 0;
   int textlen = static_cast<int>(Text.size());
   std::string line;
   while (textix < textlen) {
      if (lineix == ChrsPerLine) {
         retstr.append(line);
         retstr.append("<br />");
         line = "";
         lineix = 0;
      }
      line.push_back(Text[textix]);
      ++textix;
      ++lineix;
   }
   retstr.append(line);

   return retstr;
}

std::string NormalizeText_Simur(const std::string& Text, int ChrsPerLine) {
   std::string retstr;

   std::vector<std::string> words = SplitEx(Text, " ");

   for (int ix = 0, ixlen = words.size(); ix < ixlen; ++ix) {
      if (retstr.length() > 0)
         retstr.append(" ");

      if (static_cast<int>(words[ix].length()) > ChrsPerLine) {
         int start = 0;
         std::string buff;
         double times = static_cast<double>(words[ix].length()) / static_cast<double>(ChrsPerLine);
         int jxlen = static_cast<int>(std::ceil(times));
         for (int jx = 0; jx < jxlen; ++jx) {
            if (buff.length() > 0) buff.append(" ");
            buff.append(words[ix].substr(start, ChrsPerLine));
            start += ChrsPerLine;
         }
         retstr.append(buff);
      } else {
         retstr.append(words[ix]);
      }
   }

   return retstr;
}

std::string GetMultilineHtmlText_Simur(const std::string& Text, int ChrsPerLine) {
   std::string retstr;

   std::string normalizedtext = NormalizeText_Simur(Text, ChrsPerLine);

   std::vector<std::string> words = SplitEx(normalizedtext, " ");

   int wordsix = 0;
   std::string line;

   while (wordsix < static_cast<int>(words.size())) {
      if (static_cast<int>(line.length() + words[wordsix].length()) <= ChrsPerLine) {
         if (line.length() > 0) line.append(" ");
         line.append(words[wordsix]);
         ++wordsix;
      } else {
         if (retstr.length() > 0) retstr.append("<br />");
         retstr.append(line);
         line.clear();
      }
   }

   if (wordsix == static_cast<int>(words.size())) {
      if (retstr.length() > 0) retstr.append("<br />");
      retstr.append(line);
   }

   return retstr;
}

std::string GetMultilineHtmlText(const std::string& Text, int ChrsPerLine) {
   if (ChrsPerLine > 0) {
      bool simur = !(suri::Configuration::GetParameter("app_short_name", "") == wxT("SoPI"));
      return
            simur ? GetMultilineHtmlText_Simur(Text, ChrsPerLine) :
                    GetMultilineHtmlText_Sopi(Text, ChrsPerLine);
   }

   return "";
}

}  // namespace anonimo

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(LayerHtmlTreeNodeHandlerEvent, LayerHtmlTreeNodeHandler)
   IMPLEMENT_EVENT_CALLBACK(OnContextMenu, OnContextMenu(event), wxMenuEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** Ctor */
LayerHtmlTreeNodeHandler::LayerHtmlTreeNodeHandler(DataViewManager* pDataViewManager,
                                                   GroupManagerInterface* pGroupManager,
                                                   GuiTreeModel* pGuiTreeModel,
                                                   long Flags) :
      pDataViewManager_(pDataViewManager),
      pViewcontextManager_(pDataViewManager->GetViewcontextManager()),
      pDatasourceManager_(pDataViewManager->GetDatasourceManager()),
      pGroupManager_(pGroupManager), flags_(Flags), pGuiTreeModel_(pGuiTreeModel),
      lastMenuItemId_(NodePath(NULL, NULL)),
      NEW_EVENT_OBJECT(LayerHtmlTreeNodeHandlerEvent) {
}

/** Dtor */
LayerHtmlTreeNodeHandler::~LayerHtmlTreeNodeHandler() {
}

/** 
 *  Metodo que obtiene el html asociado a la informacion de una capa vectorial 
 *  que se desea mostrar de forma visual en el arbol de capas. 
 *  Por el momento se tomara como unico subitem de una capa vectorial 
 *  la seleccion configurada sobre la misma. Esto aparece en el xml de la misma
 *  @param[in] pLayer capa sobre la cual se consultara la informacion a 
 *  mostrar en el arbol de capas
 *  @return string html que representa la informacion de la capa.
 */
std::string LayerHtmlTreeNodeHandler::AddSubItemInformation(LayerInterface*& pLayer) {
   VectorLayer* pvlayer = dynamic_cast<VectorLayer*>(pLayer);
   std::string subitemhtml;
   if (pvlayer) {
      Element* pelement = pLayer->GetElement();
      if (pelement->GetNode(VECTOR_SELECTION_NODE) != NULL) {
         subitemhtml = SUBITEM;
         GuiHtmlEditor htmleditor(subitemhtml);
         htmleditor.ReplaceTag(SUBITEM_NODE, VECTOR_SELECTION_SUB_ITEM_NODE);
         htmleditor.ReplaceTag(SELECTION_ICON_SRC, icon_SELECT_GEOMETRY);
         subitemhtml = htmleditor.GetHtml();
      }
   }
   return subitemhtml;
}

/**
 * Retorna el html de un nodo
 * @param[in] Path path al nodo del arbol
 * @param[in] Width Cantidad de pixeles de ancho disponibles
 * @return html de nodo
 */
std::string LayerHtmlTreeNodeHandler::GetHtml(const NodePath &Path, int Width) {
   if (!Path.IsValid()) {
      REPORT_DEBUG("D: ERROR Se llamo GetGroupHtml con NodePath invalido");
      return "";
   }

   // Configuro variables segun tipo de nodo
   std::string iconsrc, nodename, nodeformat;
   std::string bkgcolor, fontcolor, subitemshtml;
   if (Path.AllowsChildren()) {
      iconsrc = GetGroupIconSrc(Path);
      nodeformat = GROUP_HTML;
      nodename = Path.GetLastPathNode()->GetName();
   } else {
      LayerInterface* player = GetNodeLayer(Path);
      subitemshtml = AddSubItemInformation(player);
      if (!player || !player->GetElement()) {
         REPORT_DEBUG("D: ERROR No se pudo obtener el nodo asociado al elemento");
         return "";
      }

      iconsrc = GetLeafIconSrc(Path);
      bool simur = !(Configuration::GetParameter("app_short_name", "") == wxT("SoPI"));
      nodeformat = simur ? SIMUR_ELEMENT_HTML : SOPI_ELEMENT_HTML;
      nodename = player->GetElement()->GetName().c_str();

      if (player->IsBlocked()) {
         bkgcolor = BACKGROUND_COLOR_DISABLED;
         fontcolor = FONT_COLOR_DISABLED;
      }
   }

   GuiHtmlEditor htmleditor(nodeformat);

   unsigned int charsByLine = ((Width - 48) / PIXEL_LENGTH);
   htmleditor.ReplaceTag(NAME_FLAG, GetMultilineHtmlText(nodename, charsByLine).c_str());

   // Configuro icono para expandir/contraer nodo
   std::string src = pGuiTreeModel_->IsExpanded(Path) ? button_EXPANDEDNODE : button_COLLAPSEDNODE;
   htmleditor.ReplaceTag(ICON_FLAG, iconsrc);

   htmleditor.ReplaceTag(BG_COLOR_TAG, bkgcolor);
   htmleditor.ReplaceTag(FONT_COLOR_TAG, fontcolor);

   // Configuro check
   if ((flags_ & CheckableHtmlNode) != 0) {
      NodeState nodestate = GetState(Path);
      htmleditor.ReplaceTag(CHECKBOXHTML_FLAG, CHECKBOXHTML);
      htmleditor.ReplaceTag(CHECKBOXHTML_B_FLAG, CHECKBOXHTML_B);
      std::string src = button_CHECK_UNCHECKED;
      if (nodestate == LayerHtmlTreeNodeHandler::ON)
         src = button_CHECK_CHECKED;
      if (nodestate == LayerHtmlTreeNodeHandler::BOTH)
         src = button_CHECK_BOTH;

      htmleditor.ReplaceTag(CHOICE_FLAG, src);
   } else {
      htmleditor.ReplaceTag(CHECKBOXHTML_FLAG, "");
      htmleditor.ReplaceTag(CHECKBOXHTML_B_FLAG, "");
   }
   htmleditor.ReplaceTag(NODE_INFORMATION_TAG, subitemshtml);
   return htmleditor.GetHtml();
}

/**
 * Muestra menu que corresponde al nodo en la posicion indicada.
 * @param[in] Path path al nodo del arbol
 * @param[in] pWidget widget sobre el que hay que renderizar el menu
 * @param[in] X pixel donde debe estar el menu
 * @param[in] Y linea donde debe estar el menu
 */
void LayerHtmlTreeNodeHandler::CreateContextMenu(const NodePath &Path, Widget* pWidget,
                                                 int X, int Y) {
   if (!Path.IsValid()) {
      REPORT_DEBUG("D: ERROR Se llamo CreateContextMenu con NodePath invalido");
      return;
   }

   if ((flags_ & ContextMenu) == 0)
      return;

   bool removedisplay = true, removehide = true, removeshowtable = true,
         removedisplayall = true, removehideall = true; 
   if (!Path.AllowsChildren()) {
      wxMenu *pmenu = wxXmlResource::Get()->LoadMenu(wxT("ID_ELEMENT_MENU"));
      // Si no es una capa vectorial oculto item mostar tabla.
      LayerInterface* player = GetNodeLayer(Path);
      if (!player || player->IsBlocked())
         return;
      DatasourceInterface *pdatasource = pViewcontextManager_->GetAssociatedDatasource(
            player->GetId());
      VectorDatasource* pvectords = dynamic_cast<VectorDatasource*>(pdatasource);

      NodeState nodestate = GetLeafState(Path);
      removedisplay = (nodestate == ON);
      removehide = (nodestate == OFF);
      removeshowtable = (pvectords == NULL);
      removedisplayall = false;
      removehideall = false;
      bool simur = !(Configuration::GetParameter("app_short_name", "") == wxT("SoPI"));
      EnclosureValidator validator;
      bool showtrainclassconvert = false, showtestclassconvert = false;
      /** Se muestra la opciones de convertir a areas de entrenamiento y test si 
       *  todas las capas seleccionadas son poligonos
      **/
      ViewcontextInterface* pviewcontext = pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();
      std::list<SuriObject::UuidType> selectedlayers = pviewcontext->GetSelectedLayers();
      bool validclassselection = true, classconvertpending = false;
      for (std::list<SuriObject::UuidType>::iterator layer = selectedlayers.begin();
            layer != selectedlayers.end(); ++layer) {
         DatasourceInterface* pds = pViewcontextManager_->GetAssociatedDatasource(*layer);
         bool validds = pds && validator.IsValidDatasourceType(pds);
         validclassselection = validclassselection && validds;
         /** si es valida y no se encuentra configurada como area de clasificacion entonces
          *  se cuenta como clase pendiente de convertir
         **/
         bool isenclosure = validator.IsValid(pds);
         classconvertpending = classconvertpending || !isenclosure;
         if (validds && isenclosure) {
            EnclosureManipulator manipulator;
            EnclosureInformation einfo = manipulator.GetEnclosureInformation(pds);
            showtrainclassconvert = showtrainclassconvert ||
                                    einfo.GetEnclosureType() != EnclosureInformation::Train;
            showtestclassconvert = showtestclassconvert ||
                                    einfo.GetEnclosureType() != EnclosureInformation::Test;
         }
      }
      showtrainclassconvert = validclassselection && (showtrainclassconvert || classconvertpending);
      showtestclassconvert = validclassselection && (showtestclassconvert || classconvertpending); 
      // Configuro el menu

      if (removedisplay) {
         wxMenuItem *pitemdisplay = pmenu->FindItem(pmenu->FindItem(_(menu_DISPLAY)));
         if (pitemdisplay)
            pmenu->Remove(pitemdisplay);
      }

      if (removehide) {
         wxMenuItem *pitemhide = pmenu->FindItem(pmenu->FindItem(_(menu_HIDE)));
         if (pitemhide)
            pmenu->Remove(pitemhide);
      }

      if (removedisplayall) {
         wxMenuItem *pitemdisplay = pmenu->FindItem(
               pmenu->FindItem(_(menu_DISPLAY_ALL)));
         if (pitemdisplay)
            pmenu->Remove(pitemdisplay);
      }

      if (removehideall) {
         wxMenuItem *pitemhide = pmenu->FindItem(pmenu->FindItem(_(menu_HIDE_ALL)));
         if (pitemhide)
            pmenu->Remove(pitemhide);
      }

      if (removeshowtable) {
         wxMenuItem *pitemdisplay = pmenu->FindItem(pmenu->FindItem(_(menu_SHOWTABLE)));
         if (pitemdisplay)
            pmenu->Remove(pitemdisplay);
      }
      if (!pvectords || player->GetElement()->GetNode(VECTOR_SELECTION_NODE) == NULL) {
         wxMenuItem *pitemdisplay = pmenu->FindItem(pmenu->FindItem(_(menu_CLEAN_VECTOR_SELECTION)));
         if (pitemdisplay)
            pmenu->Remove(pitemdisplay);
      }
      if (!showtrainclassconvert) {
         wxMenuItem *pitemdisplay = pmenu->FindItem(
               pmenu->FindItem(_(menu_CONVERTO_TRAIN_CLASS)));
         if (pitemdisplay)
            pmenu->Remove(pitemdisplay);         
      }
      if (!showtestclassconvert) {
         wxMenuItem* pitemdisplay = pmenu->FindItem(pmenu->FindItem(_(menu_CONVERTO_TEST_CLASS)));
         if (pitemdisplay)
            pmenu->Remove(pitemdisplay);
      }
      
      if (simur) {
         LayerInterface* player = GetNodeLayer(Path);
         DatasourceInterface *pdatasource =
               pViewcontextManager_->GetAssociatedDatasource(player->GetId());
         std::string type = pdatasource->GetElement()->GetPropertyType().c_str();
         // TCK #5964 : Fix temporal para fusionar aspecto y propiedades.
         // Si no es sopi esta opcion se esconde
         wxMenuItem *pitemdisplay = pmenu->FindItem(pmenu->FindItem(_(menu_ASPECT)));
         if (pitemdisplay)
            pmenu->Remove(pitemdisplay);
         pitemdisplay = pmenu->FindItem(pmenu->FindItem(_(menu_CONVERTO_TEST_CLASS)));
         if (pitemdisplay)
            pmenu->Remove(pitemdisplay);
         pitemdisplay = pmenu->FindItem(pmenu->FindItem(_(menu_CONVERTO_TRAIN_CLASS)));
         if (pitemdisplay)
            pmenu->Remove(pitemdisplay);
         // TODO(Federico - TCK #4386): Esconder los contextos en funcion de los booleans
#ifdef __HIDE_NOT_COMPATIBLE_CONTEXT__
         ViewcontextInterface* pviewcontext =
               pViewcontextManager_->GetSelectedViewcontext();
         bool hideimagecontext = false, hidemapcontext = false, hide3dcontext = false,
               hidedocumentscontext = false, hidexcecisescontext = false;
         if (pviewcontext->GetType() == ViewcontextInterface::V2D) {
            hideimagecontext = hidedocumentscontext = hidexcecisescontext = true;
         } else if (pviewcontext->GetType() == ViewcontextInterface::Map) {
            hidemapcontext = hidedocumentscontext = hidexcecisescontext = true;
         } else if (pviewcontext->GetType() == ViewcontextInterface::V3D) {
            hide3dcontext = hidedocumentscontext = hidexcecisescontext = true;
         } else if (pviewcontext->GetType() == ViewcontextInterface::Documents) {
            hideimagecontext = hide3dcontext = hidemapcontext = hidedocumentscontext =
                  true;
         } else if (pviewcontext->GetType() == ViewcontextInterface::Exercises) {
            hideimagecontext = hide3dcontext = hidemapcontext = hidexcecisescontext =
                  true;
         }
#endif
         pitemdisplay = pmenu->FindItem(pmenu->FindItem(_("&Eliminar")));
         if (pitemdisplay) pitemdisplay->SetText(_("Quitar elemento"));
      } else {
         /* En SoPI no se permite copiar entre contextos **/
         wxMenuItem* pitemdisplay = pmenu->FindItem(pmenu->FindItem(_(menu_COPYTO)));
         if (pitemdisplay)
            pmenu->Remove(pitemdisplay);
      }

      lastMenuItemId_ = Path;
      // Despliego el menu
      pmenu->Connect(wxEVT_COMMAND_MENU_SELECTED, wxMenuEventHandler(
            LayerHtmlTreeNodeHandlerEvent::OnContextMenu),
                     NULL, pEventHandler_);
      wxMenuItem* pitem = pmenu->FindItem(pmenu->FindItem(_(menu_COPYTO_IMAGE)));
      if (pitem) {
         pitem->GetMenu()->Connect(wxEVT_COMMAND_MENU_SELECTED, wxMenuEventHandler(
                              LayerHtmlTreeNodeHandlerEvent::OnContextMenu),
                              NULL, pEventHandler_);
      }
      pWidget->GetWindow()->PopupMenu(pmenu, X, Y);
      delete pmenu;
   }
}

/**
 * Se llama cuando el usuario hace doble click sobre un nodo.
 * @param[in] Path path al nodo del arbol
 */
void LayerHtmlTreeNodeHandler::ExecuteDoubleClick(const NodePath &Path) {
   // Antes de capturar el doble click y ocultar o desplegar una capa
   // Se fija si el nodo es una capa y si se encuentra bloqueda
   if (!Path.AllowsChildren()) {
      LayerInterface* player = GetNodeLayer(Path);
      if (player && player->IsBlocked())
         return;
   }
   TreeNodeDoubleClickNotification doubleclicknotif(Path);
   Notify(&doubleclicknotif);
   if ((flags_ & CheckableHtmlNode) != 0)
      InvertItemState(Path);

   pGuiTreeModel_->ResetSelection();
   pGuiTreeModel_->ChangeSelection(Path, true);
}

/**
 * Se llama cuando el usuario ejecuta un item del menu contextual.
 * Ejecuta la accion que corresponda.
 * @param[in] Path path al nodo del arbol
 * @param[in] LinkId id del item de menu seleccionado
 */
void LayerHtmlTreeNodeHandler::ExecuteHtmlLink(const NodePath &Path,
                                               const std::string &LinkId) {
   // Antes de capturar el doble click y ocultar o desplegar una capa
   // Se fija si el nodo es una capa y si se encuentra bloqueda
   if (!Path.AllowsChildren()) {
      LayerInterface* player = GetNodeLayer(Path);
      if (player && player->IsBlocked())
         return;
   }
   if (LinkId == "checkbox") {
      InvertItemState(Path);
   }

   if (LinkId == "expand") {
      bool expand = pGuiTreeModel_->IsExpanded(Path);
      pGuiTreeModel_->ExpandGroup(Path, !expand);
   }

   pGuiTreeModel_->ResetSelection();
   pGuiTreeModel_->ChangeSelection(Path, true);
}

/**
 * Retorna el estado del nodo hoja.
 * @param[in] Path path al nodo del arbol
 * @return estado de activacion del nodo. Puede ser ON, OFF o BOTH
 */
LayerHtmlTreeNodeHandler::NodeState LayerHtmlTreeNodeHandler::GetState(
      const NodePath &Path) {
   if (Path.AllowsChildren())
      return GetGroupState(Path);
   else
      return GetLeafState(Path);
}

/** Se llama cuando se selecciona item del menu contextual */
void LayerHtmlTreeNodeHandler::OnContextMenu(wxMenuEvent &Event) {
   if (!lastMenuItemId_.IsValid())
      return;

   if (lastMenuItemId_.AllowsChildren()) {
      if (Event.GetId() == XRCID("ID_ELEMENT_CLOSE"))
      pGroupManager_->RemoveGroup(lastMenuItemId_);
   } else {
      LayerInterface* player = GetNodeLayer(lastMenuItemId_);
      DatasourceInterface *pdatasource = pViewcontextManager_->
      GetAssociatedDatasource(player->GetId());
#ifdef __SHOW_PROPERTIES_DURING_EDITION__
      bool tempdatasourcecreated = false;
      if (!pdatasource || !player) {
         tempdatasourcecreated = true;
         const char *pdatasourcetypes[] = {"RasterDatasource", "VectorDatasource"};
         for (int i = 0; i < 2 && pdatasource == NULL; ++i)
         pdatasource = DatasourceInterface::Create(pdatasourcetypes[i], pelemlayer);
         if (pdatasource == NULL)
         return;
         player = LayerFactory::CreateLayer(pdatasource);
      }
#endif
      if (!pdatasource)
      return;         
      ViewcontextInterface* pviewcontext = pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();
      std::list<SuriObject::UuidType> selectedlayers = pviewcontext->GetSelectedLayers();

      // Menu de propiedades
      if (Event.GetId() == XRCID("ID_ELEMENT_PROPERTIES")) {
         ElementPropertiesWidget *ppropertieswidget = new ElementPropertiesWidget(
               pDataViewManager_, pdatasource, player);

         int retval = ppropertieswidget->ShowModal();
         if (retval != wxID_CANCEL) {
            Element* pelemlayer = player->GetElement();
            pelemlayer->SetChanged();
            pelemlayer->SendViewerUpdate();
         }
      } else if (Event.GetId() == XRCID("ID_ELEMENT_CLOSE")) {
         bool issimur = !(Configuration::GetParameter("app_short_name", "") == wxT("SoPI"));
         if (issimur == true) {
            suri::Event::GetInstance()->Call("SEVT_DELETED_ELEMENT", NULL);
         } else { // SoPI
            for (std::list<SuriObject::UuidType>::const_iterator l =
                  selectedlayers.begin(); l != selectedlayers.end(); ++l) {
               DatasourceInterface* pdatasourceremove =
                     pviewcontext->GetAssociatedDatasource(*l);
               if (pdatasourceremove) {
                  pDatasourceManager_->RemoveObject(pdatasourceremove->GetId());
               }
            }
         }
      } else if (Event.GetId() == XRCID("ID_ELEMENT_DISPLAY")) {
         std::list<NodePath> selectednodes = pGuiTreeModel_->GetSelectedNodes();
         for (std::list<NodePath>::const_iterator node = selectednodes.begin();
            node != selectednodes.end(); ++node) {
            LayerInterface* pdisplayer = GetNodeLayer(*node);
            if (pdisplayer != NULL && !pdisplayer->IsHidden())
               SetLeafState(*node, LayerHtmlTreeNodeHandler::ON);
         }
      } else if (Event.GetId() == XRCID("ID_ELEMENT_HIDE")) {
         std::list<NodePath> selectednodes = pGuiTreeModel_->GetSelectedNodes();
         for (std::list<NodePath>::const_iterator node = selectednodes.begin();
            node != selectednodes.end(); ++node) {
            LayerInterface* pdisplayer = GetNodeLayer(*node);
            if (pdisplayer != NULL && !pdisplayer->IsHidden())
               SetLeafState(*node, LayerHtmlTreeNodeHandler::OFF);
         }
      } else if (Event.GetId() == XRCID("ID_CONVERT_TRAIN_AREA_MENUTIEM") ||
                        Event.GetId() == XRCID("ID_CONVERT_TEST_AREA_MENUTIEM")) {
         EnclosureManipulator manipulator(Event.GetId() == XRCID("ID_CONVERT_TRAIN_AREA_MENUTIEM"));
         for (std::list<SuriObject::UuidType>::const_iterator l = selectedlayers.begin();
                  l != selectedlayers.end(); ++l) {
            DatasourceInterface* pdatasourceconvert = pviewcontext->GetAssociatedDatasource(*l);
            manipulator.AddNature(pdatasourceconvert);            
            /** TODO(Gabriel - TCK #6127): Se debe replicar el addnature a las capas para que de esta
             *  forma no importa que capa tome para guardar los proyectos
             *  se encuentre la metadata en el xml que indica que es valida
             *  como area de clasificacion
             */
           std::vector<SuriObject::UuidType> layerids =
               pDataViewManager_->GetViewcontextManager()->GetAssociatedLayer(pdatasourceconvert->GetId());
            std::vector<SuriObject::UuidType>::iterator it = layerids.begin();
            for (; it != layerids.end(); ++it) {
               LayerInterface* player =
                  pDataViewManager_->GetViewcontextManager()->GetLayer(*it);
               manipulator.AddNature(player->GetElement());
            }
           
         }
      } else if (Event.GetId() == XRCID("ID_ELEMENT_VISUALIZATION")) {
         if (player != NULL && !player->IsHidden()) {
             ElementPropertiesWidget *ppropertieswidget = new ElementPropertiesWidget(
                  pDataViewManager_, pdatasource, player, true);
            int retval = ppropertieswidget->ShowModal();
            if (retval != wxID_CANCEL) {
               Element* pelemlayer = player->GetElement();
               pelemlayer->SetChanged();
               pelemlayer->SendViewerUpdate();
            }
         }
      } else if (Event.GetId() == XRCID("ID_CLEAN_VECTOR_SELECTION")) {
         wxXmlNode* pparentnode = player->GetElement()->GetNode("");
         wxXmlNode* pselectionode = player->GetElement()->GetNode(VECTOR_SELECTION_NODE);
         player->GetElement()->RemoveNode(pparentnode, pselectionode);
      } else if (Event.GetId() == XRCID("ID_SENDTO_IMAGE")) {
         SendToOtherContext(ViewcontextInterface::V2D);
      } else if (Event.GetId() == XRCID("ID_SENDTO_MAP")) {
         SendToOtherContext(ViewcontextInterface::Map);
      } else if (Event.GetId() == XRCID("ID_SENDTO_3D")) {
         SendToOtherContext(ViewcontextInterface::V3D);
      } else if (Event.GetId() == XRCID("ID_SENDTO_DOCUMENTS")) {
         SendToOtherContext(ViewcontextInterface::Documents);
      } else if (Event.GetId() == XRCID("ID_SENDTO_ACTIVITIES")) {
         SendToOtherContext(ViewcontextInterface::Exercises);
      } else if (Event.GetId() == XRCID("ID_SHOW_ELEMENT_TABLE")) {
         NotebookWidget* pnotebookwidget =
               new NotebookWidget(_(caption_ELEMENT_PROPERTIES), wxNB_LEFT, NotebookWidget::Listbook);
         VectorDatasource* pvectordatasource = dynamic_cast<VectorDatasource*>(pdatasource);        
         if (pvectordatasource) {
            TablePartConfigurator tableconf;
            Option opt =TablePartConfigurator::GetVectorTableEditorConfig();
            VectorEditionTable *ptable =  new VectorEditionTable(pvectordatasource);
            VectorEditorDriver* pdriver = new VectorEditorDriver(pvectordatasource);
            BufferedDriver* pbuffereddriver = new BufferedDriver(pdriver);
            ptable->SetDriver(pbuffereddriver);
            ptable->SetDataChangeNotifier(pbuffereddriver);
            TablePart* ptablepart = tableconf.Initialize(pDataViewManager_, ptable, opt);
            ptablepart->SetWindowTitle(label_DATA_TABLE);
            ptablepart->SetWindowIcon(icon_VECTOR_TABLE_EDITOR);
            //TableTool* edt=new VectorEditionTool(pDataViewManager_);
            //ptablepart->SetTableTool(edt);
            pnotebookwidget->CreateTool();
            pnotebookwidget->InsertPart(ptablepart);
            pnotebookwidget->ShowModal();
            delete ptable;
           }
      }  
#ifdef __SHOW_PROPERTIES_DURING_EDITION__
      if (tempdatasourcecreated) {
         DatasourceInterface::Destroy(pdatasource);
         delete player;
      }
#endif
      /** Fix para el ticket #6364
       * Notificamos que hay un cambio de status en el elemento.
       */
      LayerStateChangeNotification changenotif =
            LayerStateChangeNotification();
      pDatasourceManager_->Notify(&changenotif);
   }
}

/**
 * Invierte el estado de activacion del item asociado al path en el
 * viewcontext seleccionado
 * @param[in] Path path del nodo en el arbol
 */
void LayerHtmlTreeNodeHandler::InvertItemState(const NodePath &Path) {
   if (!Path.IsValid() || Path.AllowsChildren())
      return;
   NodeState newstate = (GetLeafState(Path) == ON) ? OFF : ON;
   SetLeafState(Path, newstate);
}

/**
 * Retorna el estado del grupo en el visualizador activo
 * @param[in] Path path del grupo en el arbol.
 * @return estado de activacion del grupo. Puede ser ON, OFF o BOTH
 */
LayerHtmlTreeNodeHandler::NodeState LayerHtmlTreeNodeHandler::GetGroupState(
      const NodePath &Path) {
   if (!Path.IsValid() || !Path.AllowsChildren()) {
      REPORT_DEBUG("D: Se llamo a GetGroupState con nodo invalido");
      return LayerHtmlTreeNodeHandler::OFF;
   }
   int activelayerscount = 0;
   int grouplayerscount = 0;
   TreeIterator it = pGuiTreeModel_->GetLeafIterator(Path);
   for (; it.IsValid(); it.NextNode()) {
      NodePath itempath = it.GetPath();
      grouplayerscount++;
      NodeState leafstate = GetLeafState(itempath);
      if (leafstate == LayerHtmlTreeNodeHandler::ON)
         activelayerscount++;
   }

   NodeState state = LayerHtmlTreeNodeHandler::ON;
   if (activelayerscount == 0)
      state = LayerHtmlTreeNodeHandler::OFF;
   else if (activelayerscount < grouplayerscount)
      state = LayerHtmlTreeNodeHandler::BOTH;
   return state;
}

/**
 * Retorna el icono del grupo indicado.
 * @param[in] Path path del nodo en el arbol
 * @return icono del grupo
 */
std::string LayerHtmlTreeNodeHandler::GetGroupIconSrc(const NodePath &Path) {
   return pGuiTreeModel_->IsExpanded(Path) ? button_EXPANDEDNODE : button_COLLAPSEDNODE;
}

/**
 * Retorna el dibujo que hay que mostrar junto a la hoja. Guarda imagen
 * en memoria
 * @param[in] Path path del nodo hoja en el arbol
 * @return string que representa el icono asociado al elemento
 */
std::string LayerHtmlTreeNodeHandler::GetLeafIconSrc(const NodePath &Node) {
   LayerInterface* player = GetNodeLayer(Node);
   if ((flags_ & ShowPreviews) == 0)
      return player->GetElement()->GetIcon().c_str();

   // Si no existe el elemento en mapa creo el nombre del archivo
   std::string name;
   std::map<SuriObject::UuidType, std::string>::iterator it = layerPreviews_.find(
         player->GetId());
   if (it != layerPreviews_.end()) {
      name = it->second;
   } else {
      Element* pelement = player->GetElement();
      int previewsize = layerPreviews_.size();
      name = wxString::Format("LeafIcon%d", previewsize);
      layerPreviews_.insert(
            std::pair<SuriObject::UuidType, std::string>(player->GetId(),
                                                         name.c_str()));
      wxBitmap preview;
      int width =
            (Configuration::GetParameter("app_short_name", "") != wxT("SoPI")) ? SIMUR_ICON_SIZE :
                                                                                 ICON_SIZE;
      int height = width;
      pelement->GetPreview(preview, width, height);
      wxFileSystem fs;
      wxFSFile* pfile = fs.OpenFile((std::string("memory:") + name).c_str());
      if (pfile) {
         delete pfile;
         wxMemoryFSHandler::RemoveFile(name.c_str());
      }
      wxMemoryFSHandler::AddFile(name.c_str(), preview, wxBITMAP_TYPE_PNG);
   }

   std::string memoryname = std::string("memory:") + name;
   return memoryname.c_str();
}

/** Retorna la capa asociada al nodo. */
LayerInterface* LayerHtmlTreeNodeHandler::GetNodeLayer(const NodePath &Node) {
   if (!Node.IsValid())
      return NULL;
   TreeNodeInterface* pnode = Node.GetLastPathNode();
   SuriObject::UuidType datasourceid = pnode->GetContent();
   ViewcontextInterface* pviewcontext = pViewcontextManager_->GetSelectedViewcontext();

   return pviewcontext ? pviewcontext->GetAssociatedLayer(datasourceid) : NULL;
}

/**
 * Retorna listado de nodos hoja activos.
 * @return listado de nodos hoja activos.
 */
std::set<SuriObject::UuidType> LayerHtmlTreeNodeHandler::GetActiveLeafs() {
   if ((flags_ & IndependentState) != 0)
      return activeNodes_;

   std::set<SuriObject::UuidType> result;
   TreeIterator it = pGuiTreeModel_->GetLeafIterator();
   for (; it.IsValid(); it.NextNode()) {
      NodePath path = it.GetPath();
      LayerInterface* player = GetNodeLayer(path);
      if (player && player->GetElement()->IsActive())
         result.insert(path.GetLastPathNode()->GetId());
   }
   return result;
}

/**
 * Retorna el estado del nodo hoja.
 * @param[in] Path path de la hoja en el arbol.
 * @return estado de activacion. Puede ser ON, OFF
 */
LayerHtmlTreeNodeHandler::NodeState LayerHtmlTreeNodeHandler::GetLeafState(
      const NodePath &Path) {
   if (!Path.IsValid() || Path.AllowsChildren())
      return LayerHtmlTreeNodeHandler::OFF;

   NodeState state = LayerHtmlTreeNodeHandler::OFF;

   // Parche que permite tener un estado independiente de las capas
   if ((flags_ & IndependentState) != 0) {
      if (activeNodes_.find(Path.GetLastPathNode()->GetId()) != activeNodes_.end())
         state = LayerHtmlTreeNodeHandler::ON;
      return state;
   }

   LayerInterface* player = GetNodeLayer(Path);
   if (player && player->GetElement()->IsActive())
      state = LayerHtmlTreeNodeHandler::ON;
   return state;
}

/**
 * Configura el estado del nodo hoja.
 * @param[in] Path path de la hoja en el arbol.
 * @param[in] NewState nuevo estado de la hoja en el arbol.
 */
void LayerHtmlTreeNodeHandler::SetLeafState(
      const NodePath &Path, LayerHtmlTreeNodeHandler::NodeState NewState) {

   ViewcontextInterface* pviewcontext = pViewcontextManager_->GetSelectedViewcontext();

   bool issimur = Configuration::GetParameter("app_short_name", "") != wxT("SoPI");
   bool is3d = (pviewcontext) ? (pviewcontext->GetType() == ViewcontextInterface::V3D) : false;

   if (issimur && is3d)
      SetLeafStateForSimur3D(Path, NewState);
   else
      SetLeafStateStandard(Path, NewState);
}

/** Configura el estado del nodo hoja. */
void LayerHtmlTreeNodeHandler::SetLeafStateStandard(const NodePath &Path, NodeState NewState) {
   if (!Path.IsValid() || Path.AllowsChildren()) return;

   // Parche que permite tener un estado independiente de las capas
   if ((flags_ & IndependentState) != 0) {
      // Actualizo nodos activos
      std::set<SuriObject::UuidType> oldactive = activeNodes_;
      activeNodes_.clear();

      if ((flags_ & CheckableHtmlNodeExclusive) == 0) {
         TreeIterator it = pGuiTreeModel_->GetLeafIterator();
         for (; it.IsValid(); it.NextNode()) {
            SuriObject::UuidType nodeid = it.GetPath().GetLastPathNode()->GetId();
            if (oldactive.find(nodeid) != oldactive.end()) activeNodes_.insert(nodeid);
         }
      }

      // Agrego/elimino el nuevo nodo
      SuriObject::UuidType nodeid = Path.GetLastPathNode()->GetId();
      if (NewState == ON)
         activeNodes_.insert(nodeid);
      else if (activeNodes_.find(nodeid) != activeNodes_.end())
         activeNodes_.erase(nodeid);
      return;
   }

   LayerInterface* player = GetNodeLayer(Path);
   ViewcontextInterface* pviewcontext = pViewcontextManager_->GetSelectedViewcontext();
   if (!player || !pviewcontext) {
      REPORT_DEBUG("No se pudo actualizar el estado del nodo hoja");
      return;
   }

   bool newstate = (NewState == ON) ? true : false;
   pviewcontext->ActivateLayer(player->GetId(), newstate);
}

/** Configura el estado del nodo hoja. */
void LayerHtmlTreeNodeHandler::SetLeafStateForSimur3D(const NodePath &Path, NodeState NewState) {
   if (!Path.IsValid() || Path.AllowsChildren()) return;

   LayerInterface* player = GetNodeLayer(Path);
   ViewcontextInterface* pviewcontext = pViewcontextManager_->GetSelectedViewcontext();

   if (!player || !pviewcontext) {
      REPORT_DEBUG("No se pudo actualizar el estado del nodo hoja");
      return;
   }

   bool newstate = (NewState == ON) ? true : false;

   if (player != NULL) {
      RasterElement* pelement = dynamic_cast<RasterElement*>(player->GetElement());
      if (pelement != NULL) {
         LayerList* plist = pviewcontext->GetLayerList();
         if (plist != NULL) {
            LayerList::ElementListType::iterator beginit = plist->GetElementIterator();
            LayerList::ElementListType::iterator endit = plist->GetElementIterator(false);
            for (; beginit != endit; ++beginit) {
               TerrainElement* pterrain = dynamic_cast<TerrainElement*>(*beginit);
               if (pterrain != NULL && pterrain->IsActive()) {
                  if (newstate == true) // ON
                     pterrain->AddTexture(pelement->GetUrl().c_str());
                  else
                     pterrain->RemoveTexture(pelement->GetUrl().c_str());
               }
            }
         }
      } else {
         TerrainElement* pterrain = dynamic_cast<TerrainElement*>(player->GetElement());
         if (pterrain != NULL && newstate == false) {
            LayerList* plist = pviewcontext->GetLayerList();
            LayerList::ElementListType::iterator beginit = plist->GetElementIterator();
            LayerList::ElementListType::iterator endit = plist->GetElementIterator(false);
            for (; beginit != endit; ++beginit) {
               RasterElement* praster = dynamic_cast<RasterElement*>(*beginit);
               if (praster != NULL && praster->IsActive()) {
                  pterrain->RemoveTexture(praster->GetUrl().c_str());
               }
            }
         }
      }
   }

   pviewcontext->ActivateLayer(player->GetId(), newstate);
}

/** Copia un elemento a otro context */
void LayerHtmlTreeNodeHandler::SendToOtherContext(suri::ViewcontextInterface::ViewcontextType Type) {
   ViewcontextInterface* porigviewcontext = pViewcontextManager_->GetSelectedViewcontext();
   ViewcontextInterface* pviewcontext = pViewcontextManager_->GetViewcontextByType(Type);
   DatasourceInterface* pdatasource = VectorEditorButtonTool::GetSelectedDataSource(porigviewcontext, pDataViewManager_);
   WorkGroupInterface* prootgroup = pGroupManager_->GetRootWorkGroup();
   NodePath npath = prootgroup->SearchContent(pdatasource->GetId());
   if (npath.IsValid()) {
      DefaultTreeNode* ptreenode = dynamic_cast<DefaultTreeNode*>(npath.GetLastPathNode());
      while (ptreenode) {
         ptreenode->AddAssociatedId(pviewcontext->GetViewcontextId());
         ptreenode = dynamic_cast<DefaultTreeNode*>(ptreenode->GetParent());
      }
   }
   if (!pDatasourceManager_->AddDatasourceWithNotification(pdatasource,
         pviewcontext->GetViewcontextId())) {
      SHOW_ERROR(message_SEND_ELEMENT_TO_ERROR, pdatasource->GetName().c_str());
   }
}

/** Reformatea el string del nombre del elemento dependiendo de el tamanio
 * de la ventana de la lista (izquierda).*/
std::string LayerHtmlTreeNodeHandler::Reformat(std::string Name,
                                               unsigned int charsbyLine) {
   size_t steps = Name.size() / (charsbyLine + 4);
   for (size_t i = 0; i < steps; ++i) {
      size_t pos = (charsbyLine * (i + 1));
      Name.insert(pos, "<br>");
   }
   return Name;
}

} /* namespace suri */
