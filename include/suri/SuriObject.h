/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
