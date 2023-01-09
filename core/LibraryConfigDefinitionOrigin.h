/** \file LibraryConfigDefinitionOrigin.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LIBRARYCONFIGDEFINITIONORIGIN_H_
#define LIBRARYCONFIGDEFINITIONORIGIN_H_

#include <string>

namespace suri {

class LibraryConfigDefinitionOriginSource;

/**
 * Contiene la informacion necesaria para la carga del origen de datos.
 */
class LibraryConfigDefinitionOrigin {
public:
   static const int kCapabilitiesRead = 1;
   static const int kCapabilitiesWrite = 2;

   static const int kLevelSystem = 1;
   static const int kLevelUser = 2;
   static const int kLevelProject = 3;

   /**
    * Ctor.
    */
   LibraryConfigDefinitionOrigin();

   /**
    * Dtor.
    */
   ~LibraryConfigDefinitionOrigin();

   /**
    * Devuelve el nombre del origen.
    */
   std::string GetName() const;

   /**
    * Establece el nombre del origen.
    */
   void SetName(const std::string& Name);

   /**
    * Devuelve los permisos disponibles sobre el origen.
    */
   int GetCapabilities() const;

   /**
    * Establece los permisos sobre el origen.
    */
   void SetCapabilities(int Capabilities);

   /**
    * Devuelve el nivel del origen.
    */
   int GetLevel() const;

   /**
    * Establece el nivel del origen.
    */
   void SetLevel(int Level);

   /**
    * Devuelve la fuente del origen.
    */
   LibraryConfigDefinitionOriginSource* GetSource() const;

   /**
    * Establece la fuente del origen.
    */
   void SetSource(LibraryConfigDefinitionOriginSource* pSource);

private:
   std::string name_;  // nombre del origen
   int capabilities_;  // permisos (lectura y/o escritura)
   int level_;  // nivel (usuario, sistema o proyecto)
   LibraryConfigDefinitionOriginSource* pSource_;  // fuente de los datos
};

}  // namespace suri

#endif  // LIBRARYCONFIGDEFINITIONORIGIN_H_
