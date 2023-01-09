/*! \file SuriObject.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SURIOBJECT_H_
#define SURIOBJECT_H_

// Includes Estandar
#include <string>

// Includes Suri

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Objeto para proveer pseudo RTTI (para factorias) e identificacion de instancias de clases.
 */
class SuriObject {
public:
   typedef std::string UuidType;             /*! Tipo para usar en los UUIDs */
   typedef long int UuidIntType;             /*! Tipo para UUIDs numericos */
   typedef std::string ClassIdType;          /*! Tipo para identificar la clase (pseudo RTTI) */
   static const UuidType NullUuid;           /*! Tipo Null */
   static const UuidIntType NullIntUuid;     /*! Tipo Null */
   static const ClassIdType NullClassId;     /*! Tipo Null */

   /** Constructor */
   SuriObject();
   /** Constructor */
   SuriObject(const ClassIdType &ClassId);
   /** Constructor */
   SuriObject(const ClassIdType &ClassId, const UuidType &Uuid);
   /** Destructor */
   virtual ~SuriObject();
   /** Retorna el Identificador Unico de la instancia */
   virtual UuidType GetId() const;
   /** Retorna el identificador de la clase */
   virtual ClassIdType GetClassNameId() const;
   /** Setea el id unico */
   virtual bool SetId(const SuriObject::UuidType& Id);
   /** Setea el id */
   virtual bool SetClassNameId(const SuriObject::ClassIdType& ClassId);
   /** Retorna un UUID unico para la aplicacion */
   static UuidType CreateId();
   /** Retorna un UUID unico para la aplicacion en tipo numerico */
   static UuidIntType CreateIntId(const UuidType &Uuid = NullUuid);

private:
   bool hasId_;            /*! Variable auxiliar para determinar si tiene id unico o no */
   bool hasClassId_;       /*! Variable auxiliar para determinar si tiene id o no */
   UuidType id_;           /*! Identificador unico de clase */
   ClassIdType classId_;   /*! Identificador de clase */
};

} /* namespace suri */

#endif /* SURIOBJECT_H_ */
