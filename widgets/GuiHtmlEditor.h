/** \file wxGuiFunctions.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


#ifndef WXGUIFUNCTIONS_H_
#define WXGUIFUNCTIONS_H_

#include <string>

namespace suri {

class GuiHtmlEditor {
public:
   /** Ctor */
   explicit GuiHtmlEditor(const std::string &MainBody);
   /** Dtor */
   ~GuiHtmlEditor();

   /** Remplaza un TAG en html con texto que recibe por parametro */
   bool ReplaceTag(const std::string &TagName, const std::string HtmlText);
   /** Retorna Html */
   std::string GetHtml();

   /**  Modifica Text para que entre en ventana de longitud WindowWidth */
   static std::string GetSizedText(const std::string &Text, int WindowWidth);

private:
   std::string html_;
};

} /* namespace suri */

#endif /* WXGUIFUNCTIONS_H_ */
