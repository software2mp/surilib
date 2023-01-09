/*! \file LayerHtmlListItem.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>

// Includes Suri
#include "LayerHtmlListItem.h"
#include "GuiHtmlEditor.h"
#include "resources.h"
#include "suri/Element.h"
#include "suri/LayerInterface.h"

// Includes Wx
// Defines
// Defines
#define CHECKBOXHTML \
"    <td>" \
"       <table align=\"left\" width=\"16px\">" \
"          <tr>" \
"             <td align=\"right\">" \
"                <a href=\"checkbox\">" \
"                   <img src=\"__CHOICE__\">" \
"                </a>" \
"             </td>" \
"          </tr>" \
"       </table>" \
"    </td>"

#define CHECKBOXHTML_B \
"                <a href=\"checkbox\">" \
"                   <img src=\"__CHOICE__\">" \
"                </a>"

#define CHOICE_FLAG "__CHOICE__"
#define CHECKBOXHTML_FLAG "__CHECKBOXHTML__"
#define CHECKBOXHTML_B_FLAG "__CHECKBOXHTML_B__"
/** Modelo HTML para mostrar el checkbox a   la derecha y abajo del nombre.*/
#define ELEMENT_HTML \
   "                                                        \
            <table cellspacing=\"0\" cellpadding=\"0\" __BGCOLOR__ >                                                           \
               <tr>                                                           \
                  <td><img width=\"16\" height=\"16\" src=\"__ICON__\"></td>                   \
                  <td>                                                        \
                     <table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\">                                    \
                        <tr>                                                  \
                           <td><font __FCOLOR__>__NAME__</td></font>                      \
                           <tr>                                               \
                              <table cellspacing=\"0\" cellpadding=\"0\" align= \"left\" width=\"100%\">          \
                                 <tr>                                         \
                                    <td align=\"right\"  width=\"75%\" >&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;      \
                                       <font size=1>                          \
                                       </font>                                \
                                          __CHECKBOXHTML__                    \
                                    </td>                                     \
                                    <td width=\"25%\"></td>                  \
                                 </tr>                                        \
                              </table>                                        \
                           </tr>                                              \
                        </tr>                                                 \
                     </table>                                                 \
                  </td>                                                       \
               </tr>                                                          \
            </table>"

/** Modelo HTML para mostrar el checkbox a la izquierda.*/
#define ELEMENT_HTML_LEFT \
            "<table cellspacing=\"0\" cellpadding=\"0\">                 \
                  <td><img width=\"16\" height=\"16\" src=\"__ICON__\"></td>    \
                  <td>__NAME__</td>                                              \
                  <td>__CHECKBOXHTML__</td>                                       \
            </table>"

#define NAME_FLAG "__NAME__"
#define ICON_FLAG "__ICON__"

// forwards

namespace suri {

namespace ui {

/** Constructor
 * @param[in] LayoutFlags configuraciond el layout del html a generar
 **/
LayerHtmlListItemConverter::LayerHtmlListItemConverter(long LayoutFlags) :
      HtmlListItemStringConverterInterface(LayoutFlags) {
}

/** Destructor **/
LayerHtmlListItemConverter::~LayerHtmlListItemConverter() {
}

/**
 * A partir del item pasado por parametro genera un string html
 * con el contenido del mismo
 * * @param[in] LayoutFlags configuracion del layout deseado
 * @return std::string string que representa el html asociado al item
 **/
bool LayerHtmlListItemConverter::GetItemAsString(const HtmlListItemInterface* pItem,
                                                 std::string& ItemHtmlString) const {
   HtmlListItemInterface* phitem = const_cast<HtmlListItemInterface*>(pItem);
   LayerHtmlListItem* pitem = dynamic_cast<LayerHtmlListItem*>(phitem);
   Element* pelement = pitem ? pitem->pLayer_->GetElement() : NULL;
   if (!pelement)
      return false;
   std::string nodeformat = ELEMENT_HTML;
   GuiHtmlEditor htmleditor(nodeformat);
   wxString name = pelement->GetName();
   htmleditor.ReplaceTag(NAME_FLAG, name.c_str());
   htmleditor.ReplaceTag(ICON_FLAG, pelement->GetIcon().c_str());
   if (layoutFlags_ & Checkeable) {
      htmleditor.ReplaceTag(CHECKBOXHTML_FLAG, CHECKBOXHTML);
      htmleditor.ReplaceTag(CHECKBOXHTML_B_FLAG, CHECKBOXHTML_B);
      std::string src = pItem->IsActive() ? button_CHECK_CHECKED : button_CHECK_UNCHECKED;
      htmleditor.ReplaceTag(CHOICE_FLAG, src);

   } else {
      htmleditor.ReplaceTag(CHECKBOXHTML_FLAG, "");
      htmleditor.ReplaceTag(CHECKBOXHTML_B_FLAG, "");
   }
   ItemHtmlString = htmleditor.GetHtml();
   return true;
}

/**
 *  Constructor a partir de una capa
 *  @param[in] pLayer capa asociada al item
 */
LayerHtmlListItem::LayerHtmlListItem(LayerInterface* pLayer) :
      pLayer_(pLayer) {
   itemId_ = pLayer->GetId();
}

/** Metodo que compara el elemento pasado por parametro.
 * Devuelve true si es igual al elemento de la instancia
 * sobre la cual se invoca el mismo. **/
bool LayerHtmlListItem::Equals(const HtmlListItemInterface& Item) const {
   return itemId_.compare(Item.GetId())== 0;
}

/** Metodo que marca al item como activo. **/
void LayerHtmlListItem::Active() {
   active_ = true;
}
/** Metodo que marca al item como desactivo. **/
void LayerHtmlListItem::Deactive() {
   active_ = false;
}

/**
 * A partir del item pasado por parametro genera un string html
 * con el contenido del mismo. **/
std::string LayerHtmlListItem::GetItemAsString(long LayoutFlags) const {
   LayerHtmlListItemConverter converter(LayoutFlags);
   std::string htmlstr;
   converter.GetItemAsString(this, htmlstr);
   return htmlstr;
}
} /** namespace ui */
} /** namespace suri */
