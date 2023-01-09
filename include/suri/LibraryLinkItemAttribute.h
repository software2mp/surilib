/*! \file LibraryLinkItemAttribute.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LIBRARYLINKITEMATTRIBUTE_H_
#define LIBRARYLINKITEMATTRIBUTE_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/LibraryItemAttribute.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Clase que representa un atributo de tipo link a biblioteca externa
 */
class LibraryLinkItemAttribute : public suri::LibraryItemAttribute {
   /** ctor copia**/
   LibraryLinkItemAttribute(const LibraryLinkItemAttribute&);

public:
   /** Constructor */
   LibraryLinkItemAttribute(const std::string& Id,
                            const std::string& LibraryCode);
   /** Destructor **/
   virtual ~LibraryLinkItemAttribute();
   /** Obtiene el id del item al cual hace referencia **/
   std::string GetItemId() const;
   /** Obtiene el codigo de la biblioteca al cual pertenece el item **/
   std::string GetLibraryCode() const;
   /** String que representa la clave para el id del item al cual hace referencia**/
   static const std::string ItemIdKey;
   /** String que representa la clave para el codigo de biblioteca al que pertence el item **/
   static const std::string CodeKey;
   typedef enum {
      ItemIdPosition = 0,
      LibraryCodePosition = 1,
   } AtributeValuesPosition;

private:
   /** id que posee el item **/
   std::string itemId_;
   /** codigo de la biblioteca **/
   std::string libcode_;
};

} /** namespace suri */

#endif /* LIBRARYLINKITEMATTRIBUTE_H_ */

