/*! \file LayerHtmlListItem.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LAYERHTMLLISTITEMCONVERTER_H_
#define LAYERHTMLLISTITEMCONVERTER_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "HtmlListSelectionWidget.h"

// Includes Wx
// Defines
// forwards

namespace suri {

class LayerInterface;

namespace ui {

/**
 * Clase que representa una capa como item dentro de una lista html.
 **/
class LayerHtmlListItem : public HtmlListItemInterface {
   /** ctor copia **/
   LayerHtmlListItem(const LayerHtmlListItem&);

public:
   /** ctor
    * @param[in] pLayer capa asociada al item
    **/
   explicit LayerHtmlListItem(LayerInterface* pLayer);
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

protected:
   friend class LayerHtmlListItemConverter;
   /** Capa asociada al item **/
   LayerInterface* pLayer_;
};

/**
 * Clase que genera un string en formato html que corresponde a la capa
 * que se encuentra asociada al conversor. **/
class LayerHtmlListItemConverter : public HtmlListItemStringConverterInterface {
   /** ctor copia **/
   LayerHtmlListItemConverter(const LayerHtmlListItemConverter&);

public:
   /** ctor
    * @param[in] LayoutFlags configuraciond el layout del html a generar
    **/
   LayerHtmlListItemConverter(long LayoutFlags = Checkeable);
   /** dtor **/
   virtual ~LayerHtmlListItemConverter();
   /**
    * A partir del item pasado por parametro genera un string html
    * con el contenido del mismo. **/
   virtual bool GetItemAsString(const HtmlListItemInterface* pItem,
                        std::string& ItemHtmlString) const;
};

} /** namespace ui */
} /** namespace suri */

#endif /* LAYERHTMLLISTITEMCONVERTER_H_ */
