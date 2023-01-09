/*! \file ItemId.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ITEMID_H_
#define ITEMID_H_

// Includes standard
#include <string>
// Includes Suri
#include "suri/SuriObject.h"

// Includes Wx

// Includes App

// Defines
namespace suri {
class ItemId {
protected:
   /** Ctor */
   ItemId(std::string Id);
public:
   /** Ctor de a partir de suriobject */
   explicit ItemId(const SuriObject &Id);
   /** Ctor de copia */
   ItemId(const ItemId &Id);
   /** Dtor */
   virtual ~ItemId();

   ItemId& operator=(const ItemId& Id);

   /** Compara el id contra el del ItemId que recibe por parametro */
   virtual int Compare(const ItemId &OtherItemId) const;
   /** Retorna id en formato string. Este metodo no se deberia usar fuera de wx. */
   std::string GetId() const;

protected:
   std::string id_; /*! id de elemento */
};
}  // namespace

#endif /* ITEMID_H_ */
