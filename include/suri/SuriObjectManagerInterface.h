/** \file SuriObjectManagerInterface.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


#ifndef SURIOBJECTMANAGERINTERFACE_H_
#define SURIOBJECTMANAGERINTERFACE_H_

// Includes standard
#include <vector>
// Includes Suri
#include "suri/Subject.h"
#include "suri/SuriObject.h"
// Includes Wx
// Includes App
// Defines

namespace suri {

class SuriObjectManagerInterface : public Subject {
public:
   /**
    * Mueve un elemento a la posicion del objeto destino
    *  @param[in] Id identificador del elemento a mover. Debe existir en manager.
    *  @param[in] Id identificador del elemento destino. Debe existir en manager.
    *  @return bool que indica si fue exitoso
    */
   virtual bool MoveObject(const SuriObject::UuidType& Id,
                            const SuriObject::UuidType& DestinationId) = 0;

   /**
    * Mueve un elemento al final
    * @param[in] Id identificador del elemento a mover. Debe existir en manager.
    * @return bool que indica si fue exitoso
    */
   virtual bool MoveObjectToEnd(const SuriObject::UuidType& Id) = 0;

   /**
    * Devuelve una lista con los Ids de la fuente de datos ordenados
    * @return std::vector con los ids ordenados
    */
   virtual std::vector<SuriObject::UuidType> GetOrderedIds() const = 0;

   /** Quita un objeto del manager */
   virtual bool RemoveObject(const SuriObject::UuidType& Id) = 0;
   /** Verifica si puede quitar un objeto del manager */
   virtual bool CanRemoveObject(const SuriObject::UuidType& Id) = 0;
};

}  /* namespace suri */

#endif /* SURIOBJECTMANAGERINTERFACE_H_ */
