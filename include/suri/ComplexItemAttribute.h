/*! \file ComplexItemAttribute.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COMPLEXITEMATTRIBUTE_H_
#define COMPLEXITEMATTRIBUTE_H_

// Includes Estandar
#include <vector>
// Includes Suri
#include "LibraryItemAttribute.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 *  Atributo para item de biblioteca de tabla de intervalos.
 *  Representa un atributo complejo que viene definido por mas de un valor.
 */
class ComplexItemAttribute : public suri::LibraryItemAttribute {
   /** ctor copia **/
   ComplexItemAttribute(const ComplexItemAttribute&);
public:
   /** Constructor **/
   explicit ComplexItemAttribute(
         const std::string& Name, const std::string& ComplexValue,
         const std::string& StartAttrChar = StartAttributeChar,
         const std::string& FinishAttrChar = FinishAttributeChar,
         const std::string& AttrToken = AttributesToken);
   /** Destructor **/
   virtual ~ComplexItemAttribute();
   /** Caracter delimitador del inicio del atributo **/
   static std::string StartAttributeChar;
   /** Caracter delimitador del final del atributo **/
   static std::string FinishAttributeChar;
   /** Token que separa los valores del atributo**/
   static std::string AttributesToken;
   /** Devuelve el valor asociado al indice especificado del atributo */
   virtual std::string GetAttribute(int Index) const;
   /** Agrega un atributo al vector **/
   void AddAttribute(LibraryItemAttribute* pAttribute);
   /** Devuelve el listado de items que posee el atributo complejo
    *  en un string solo de formato {attr1,atttr2,attr3,...,attrn}
    *  @return string **/
   virtual std::string GetSimpleAttributeValue() const;

protected:
   /** A partir de un valor de atributo complejo obtiene los atributos que lo componen **/
   void LoadAttributesFromString(const std::string& ComplexValue);
   std::vector<LibraryItemAttribute*> attributes_;
   std::string startAttr_, finishAttr_, attributesToken_;
};

} /** namespace suri */

#endif /* COMPLEXITEMATTRIBUTE_H_ */
