/*! \file DecoratedHtmlItem.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "DecoratedHtmlItem.h"

namespace suri {
/**
 * Configura el ItemId y el Html del item
 * @param[in] Id id del item
 * @param[in] Html std::string con html del item
 */
DecoratedHtmlItem::DecoratedHtmlItem(const ItemId &Id, std::string Html) :
      itemId_(Id), itemHtml_(Html) {
}

/** Dtor */
DecoratedHtmlItem::~DecoratedHtmlItem() {
}

/**
 * Retorna html que representa al item
 * return std::string con html del item
 */
std::string DecoratedHtmlItem::GetHtml() const {
   return itemHtml_;
}

/**
 * Modifica html que representa al item
 * @param[in] nuevo html del item
 */
void DecoratedHtmlItem::SetHtml(std::string Html) {
   itemHtml_ = Html;
}

/**
 * Retorna identificador item
 * return ItemId identificador del item
 */
ItemId DecoratedHtmlItem::GetItemId() const {
   return itemId_;
}
}  // namespace
