/*! \file EnclosureValidator.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ENCLOSUREVALIDATOR_H_
#define ENCLOSUREVALIDATOR_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/DatasourceInterface.h"
#include "suri/DatasourceValidatorInterface.h"
#include "suri/DataTypes.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * El EnclosureValidator se encarga de validar los casos en que un VectorDatasource
 * se pueda utilizar como un recinto (Enclosure)
 */
class EnclosureValidator : public suri::DatasourceValidatorInterface {
public:
   /** Constructor */
   EnclosureValidator();
   /** Destructor */
   virtual ~EnclosureValidator();
   /** Efectua la validacion del objeto en cuestion y devuelve verdadero si es valido */
   virtual bool IsValid(DatasourceInterface* pDatasource) const;
   bool IsValid(Element* pElement) const;
   /*Valida que la fuente de datos sea valida como reciento. Es decir, sea vectorial
    * y de geometria poligono */
   virtual bool IsValidDatasourceType(DatasourceInterface* pDatasource) const;
    /*Valida que la fuente de datos sea valida como reciento. Es decir, sea vectorial
    * y de geometria poligono */
   bool IsValidDatasourceType(Element* pElement) const;
  /** Nombre del campo "tipo" */
   static const std::string TypeField;
   /** Nombre del campo "descripcion" */
   static const std::string DescField;
   /** Tipo del campo "tipo" */
   typedef DataInfo<int> TypeRecord;
   /** Tipo del campo "descripcion" */
   typedef DataInfo<unsigned char> DescRecord;

private:
   /** Efectua la validacion del objeto en cuestion y devuelve verdadero si es valido */
   bool IsXmlValid(Element* pElement) const;
   /** Verifica que una fuente de datos tenga los campos que corresponden a un recito */
   bool IsFieldsValid(Element* pElement) const;
};

} /** namespace suri */

#endif /* ENCLOSUREVALIDATOR_H_ */
