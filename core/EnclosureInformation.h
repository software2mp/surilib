/*! \file EnclosureInformation.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ENCLOSUREINFORMATION_H_
#define ENCLOSUREINFORMATION_H_

// Includes Estandar
#include<string>

// Includes Suri
#include "suri/DataTypes.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Clase que encapsula la informacion de una fuente de datos como recinto
 */
class EnclosureInformation {
public:
   typedef enum { Train = 0, Test =1} EnclosureType;
   /** ctor **/
   EnclosureInformation(const std::string& LayerName, const std::string& Query,
                        EnclosureType Etype,
                        const std::string& TypeName = TypeNameDefault,
                        const std::string& DescName = DescNameDefault);
   /** dtor **/
   virtual ~EnclosureInformation();
   /** Nombre del campo tipo de area por defecto*/
   static const std::string TypeNameDefault;
   /** Nombre del campo descripcion por defecto*/
   static const std::string DescNameDefault;
   /** Obtiene el nombre de la capa*/
   std::string GetLayerName() const;
   /** Crea una query basada en los campos asociados al recinto y la condicion que recibe */
   std::string SetQuery(const std::string& Condition = " IN(0)");
   /** Obtiene la query asociada al recinto*/
   std::string GetQuery() const;
   /** Obtiene el nombre del campo tipo de area*/
   std::string GetTypeAreaName() const;
   /** Obtiene el tipo de dato del campo tipo de area*/
   std::string GetTypeAreaFieldType() const;
   /**Devuelve el nombre del campo descripcion del area */
   std::string GetDescAreaName() const;
   /**  Obtiene el tipo de dato asociado al campo descripcion tipo de area */
   std::string GetDescFieldType() const;
   /** Devuelve el tipo de recinto (entrenamiento o test). **/
   EnclosureType GetEnclosureType() const;
   /** Tipo del campo "tipo" */
   typedef DataInfo<int> TypeRecord;
   /** Tipo del campo "descripcion" */
   typedef DataInfo<unsigned char> DescRecord;

private:
   std::string layerName_;
   std::string query_;
   EnclosureType enclosureType_;
   class AreaField {
   public:
      std::string name_, type_;
      AreaField(const std::string& Name, const std::string& Type) : name_(Name), type_(Type) {
      }
   };
   AreaField typeArea_;
   AreaField descArea_;
};

} /** namespace suri */

#endif /* ENCLOSUREINFORMATION_H_ */
