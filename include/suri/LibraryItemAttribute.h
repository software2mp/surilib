/*! \file LibraryItemAttribute.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LIBRARYITEMATTRIBUTE_H_
#define LIBRARYITEMATTRIBUTE_H_

// Includes Estandar
#include <string>

// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Clase que representa un atributo de un item de biblioteca
 */
class LibraryItemAttribute {
   LibraryItemAttribute(const LibraryItemAttribute&);

public:
   typedef enum {
      SimpleAttr = 0,
      ComplexAttr = 1,
      XmlAttr = 2
   }
   AttributeTypeEnum;

   static const std::string PrincipalAttrName;
   static const std::string ExtendedAttrName;

   /** Constructor **/
   LibraryItemAttribute(const std::string& Name, const std::string& Value = " ",
                        AttributeTypeEnum AttributeType = SimpleAttr);
   /** Destructor **/
   virtual ~LibraryItemAttribute();
   /** Enum para definir los tipos de atributos **/

   /**
    * Devuelve el nombre asociado al atributo
    * @return string con el nombre del atributo
    */
   std::string GetName() const;
   /**
    * Devuelve el tipo asociado al atributo
    * @return string con el tipo del atributo
    */
   AttributeTypeEnum GetType() const;
   /**
    * Devuelve el valor asociado al atributo
    * @return string con el valor del atributo
    */
   virtual std::string GetValue() const;
   /** Asocia un valor al atributo */
   virtual void SetValue(std::string Value);
   /** Compara si dos atributos son iguales **/
   virtual bool Equals(const LibraryItemAttribute& Attribute) const;

private:
   /** Nombre del atributo **/
   std::string name_;
   /** Tipo del atributo **/
   AttributeTypeEnum type_;
   /** Valor del atributo **/
   std::string value_;
};

} /** namespace suri */

#endif /* LIBRARYITEMATTRIBUTE_H_ */
