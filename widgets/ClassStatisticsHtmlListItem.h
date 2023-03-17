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

#ifndef CLASSSTATISTICSHTMLLISTITEM_H_
#define CLASSSTATISTICSHTMLLISTITEM_H_

// Includes Estandar
// Includes Suri
#include "HtmlListSelectionWidget.h"
#include "suri/ClassStatisticsFileParser.h"

// Includes Wx
// Defines
// forwards

namespace suri {

namespace ui {
/**
 * Clase que representa un conjunto de parametros estadísticos que definen una clase valida
 * para el proceso de clasificacion supervisada
 */
class ClassStatisticsHtmlListItem : public HtmlListItemInterface {
   /** Constructor copia */
   ClassStatisticsHtmlListItem(ClassStatisticsHtmlListItem const&);

public:
   /** Constructor */
   explicit ClassStatisticsHtmlListItem(
         ClassStatisticsFileParser::ClassStatisticsInfo& ClassInfo);
   /** Metodo que compara el elemento pasado por parametro.
    * Devuelve true si es igual al elemento de la instancia
    * sobre la cual se invoca el mismo. **/
   virtual bool Equals(const HtmlListItemInterface& Item) const;
   /** Metodo que marca al item como activo. **/
   virtual void Active();
   /** Metodo que marca al item como desactivo. **/
   virtual void Deactive();
   /**
    * A partir del item pasado por parametro genera un string html
    * con el contenido del mismo.
    * @param[in] LayoutFlags configuracion del layout deseado
    * @return std::string string que representa el html asociado al item
    **/
   virtual std::string GetItemAsString(long LayoutFlags =
         HtmlListItemStringConverterInterface::Checkeable) const;

   ClassStatisticsFileParser::ClassStatisticsInfo GetClassInfo() {
      return classInfo_;
   }

protected:
   friend class ClassStatisticsFileHtmlListItemConverter;
   /** Nombre de la clase */
   ClassStatisticsFileParser::ClassStatisticsInfo classInfo_;
};

/**
 * Genera un string en formato html que corresponde a la capa que se encuentra
 * asociada al conversor
 **/
class ClassStatisticsFileHtmlListItemConverter : public HtmlListItemStringConverterInterface {
   /** ctor copia **/
   ClassStatisticsFileHtmlListItemConverter(const ClassStatisticsFileHtmlListItemConverter&);

public:
   /** Constructor */
   ClassStatisticsFileHtmlListItemConverter(long LayoutFlags = Checkeable);
   /** Destructor */
   virtual ~ClassStatisticsFileHtmlListItemConverter();
   /** A partir del item pasado por parametro genera un string html con el contenido del mismo. **/
   virtual bool GetItemAsString(const HtmlListItemInterface* pItem,
                        std::string& ItemHtmlString) const;
};

} /** namespace ui */
} /** namespace suri */

#endif /* CLASSSTATISTICSHTMLLISTITEM_H_ */
