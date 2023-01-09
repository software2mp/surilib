/*! \file LibraryItemCatalog.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LIBRARYITEMCATALOG_H_
#define LIBRARYITEMCATALOG_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/SuriObject.h"
#include "LibraryItemOrigin.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class LibraryItemAttribute;

/*!
 * Clase que representa la informacion que brinda un item de biblioteca en el catalogo.
 */
class LibraryItemCatalog {
   /** ctor copia **/
   LibraryItemCatalog(const LibraryItemCatalog&);

public:
   /** ctor **/
   LibraryItemCatalog(const SuriObject::UuidType& ItemId,
                      const LibraryItemAttribute* pPrincipalAttribute,
                      const LibraryItemAttribute* pExtendedAttribute = NULL,
                      const SuriObject::UuidType& OriginId = SuriObject::NullUuid);
   /** dtor */
   virtual ~LibraryItemCatalog();
   /**
    * Devuelve el nombre atributo principal
    * del item para la muestra del catalogo.
    * @return string con el nombre del atributo principal
    */
   std::string GetPrincipalAttributeName() const;
   /**
    * Devuelve el nombre del atributo extendido (atributo descriptivo del atributo principal)
    *  del item para la muestra del catalogo.
    * @return string con el nombre del atributo extendido
    */
   std::string GetExtendedAttributeName() const;
   /**
    * Devuelve el valor atributo extendido (atributo descriptivo del atributo principal)
    *  del item para la muestra del catalogo.
    * @return string con el valor del atributo extendido
    */
   std::string GetExtendedAttributeValue() const;
   /**
    * Devuelve el valor del atributo principal
    *  del item para la muestra del catalogo.
    * @return string con el nombre del atributo extendido
    */
   std::string GetPrincipalAttributeValue() const;
   /**
    * Devuelve el id del item asociado
    * Precondicion: Siempre se tiene id asociado a un item
    * @return string con id del item asociado
    */
   SuriObject::UuidType GetAssociatedItemId() const;
   /** Obtiene el id del origen que creo el catalogo **/
   SuriObject::UuidType GetOriginId() const;
   /** Indica que el item al que corresponde el catalog
    *  forma parte de los favoritos
    */
   void SetFavoriteFlag(bool Favorite);
   /** Indica si el item fue marcado como favorito  */
   bool IsFavorite() const;

private:
   SuriObject::UuidType itemId_;
   /** Atributo principal del item**/
   const LibraryItemAttribute* pPrincipalAttribute_;
   /** Atributo descriptivo del atributo principal **/
   const LibraryItemAttribute* pExtendedAttribute_;
   /** id del origen en el que se encuentra el item*/
   SuriObject::UuidType originId_;
   bool favorite_;
};

} /** namespace suri */

#endif /* LIBRARYITEMCATALOG_H_ */
