/*! \file TreeNodeId.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TREENODEID_H_
#define TREENODEID_H_

// Includes standard

// Includes Suri
#include "ItemId.h"

// Includes Wx

// Includes App

// Defines
#define INVALIDTREEID TreeNodeId("")

namespace suri {
class TreeNodeId : public ItemId {
public:
   /** Ctor */
   TreeNodeId(const ItemId &Id);
   /** Ctor */
   TreeNodeId(std::string Id);
   /** Dtor */
   virtual ~TreeNodeId();

   /** Compara dos ids usando la jerarquia del arbol. */
   virtual int Compare(const ItemId &OtherItemId) const;

   /** Se usa para generar ids de subnodos. */
   void AppendStringToId(std::string SubnodeId);
};
}

#endif /* TREENODEID_H_ */
