/*! \file LibraryItemCatalog.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/LibraryItemCatalog.h"
#include "suri/LibraryItemAttribute.h"
#include "suri/LibraryItemOrigin.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/** constructor **/
LibraryItemCatalog::LibraryItemCatalog(const SuriObject::UuidType& ItemId,
                                       const LibraryItemAttribute* pPrincipalAttribute,
                                       const LibraryItemAttribute* pExtendedAttribute,
                                       const SuriObject::UuidType& OriginId) :
      itemId_(ItemId), pPrincipalAttribute_(pPrincipalAttribute),
      pExtendedAttribute_(pExtendedAttribute), originId_(OriginId),
      favorite_(false) {
}

/** destructor **/
LibraryItemCatalog::~LibraryItemCatalog() {
}

/**
 * Devuelve el nombre atributo principal
 * del item para la muestra del catalogo.
 * @return string con el nombre del atributo principal
 */
std::string LibraryItemCatalog::GetPrincipalAttributeName() const {
   return pPrincipalAttribute_->GetName();
}
/**
 * Devuelve el nombre del atributo extendido (atributo descriptivo del atributo principal)
 *  del item para la muestra del catalogo.
 * @return string con el nombre del atributo extendido
 */
std::string LibraryItemCatalog::GetExtendedAttributeName() const {
   std::string name;
   if (pExtendedAttribute_)
      name = pExtendedAttribute_->GetName();
   return name;
}
/**
 * Devuelve el valor atributo extendido (atributo descriptivo del atributo principal)
 *  del item para la muestra del catalogo.
 * @return string con el valor del atributo extendido
 */
std::string LibraryItemCatalog::GetExtendedAttributeValue() const {
   std::string value;
   if (pExtendedAttribute_)
      value = pExtendedAttribute_->GetValue();
   return value;
}
/**
 * Devuelve el valor del atributo principal
 *  del item para la muestra del catalogo.
 * @return string con el nombre del atributo extendido
 */
std::string LibraryItemCatalog::GetPrincipalAttributeValue() const {
   return pPrincipalAttribute_->GetValue();
}
/**
 * Devuelve el id del item asociado
 * Precondicion: Siempre se tiene id asociado a un item
 * @return string con id del item asociado
 */
SuriObject::UuidType LibraryItemCatalog::GetAssociatedItemId() const {
   return itemId_;
}

/** Obtiene el id del origen que creo el catalogo **/
SuriObject::UuidType LibraryItemCatalog::GetOriginId() const {
   return originId_;
}

/** Indica que el item al que corresponde el catalog
 *  forma parte de los favoritos
 */
void LibraryItemCatalog::SetFavoriteFlag(bool Favorite) {
   favorite_ = Favorite;
}
/** Indica si el item fue marcado como favorito  */
bool LibraryItemCatalog::IsFavorite() const {
   return favorite_;
}
} /** namespace suri */
