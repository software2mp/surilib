/*! \file ClassStatisticsHtmlListItem.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
