/*! \file DecoratedHtmlItem.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DECORATEDHTMLITEM_H_
#define DECORATEDHTMLITEM_H_

// Includes standard
#include <string>

// Includes Suri
#include "ItemId.h"

// Includes Wx

// Includes App

// Defines

namespace suri {
class DecoratedHtmlItem {
public:
   /** Ctor */
   DecoratedHtmlItem(const ItemId &Id, std::string Html);
   /** Dtor */
   ~DecoratedHtmlItem();

   /** Retorna html que representa al item */
   std::string GetHtml() const;
   /** Modifica html que representa al item */
   void SetHtml(std::string Html);

   /** Retorna identificador del item */
   ItemId GetItemId() const;

protected:
   ItemId itemId_; /*! id unico que identifica al item */
   std::string itemHtml_; /*! html que usa wxDecoratedHtmlCtrl para mostrar item */
};
}  // namespace

#endif /* DECORATEDHTMLITEM_H_ */
