/** \file LibraryConfigDefinitionOriginSource.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LIBRARYCONFIGDEFINITIONORIGINSOURCE_H_
#define LIBRARYCONFIGDEFINITIONORIGINSOURCE_H_

#include <string>

namespace suri {

/**
 * Contiene la informacion para poder leer los datos para el origen en particular.
 */
class LibraryConfigDefinitionOriginSource {
public:
   static const std::string kTypeTextfile;
   static const std::string kTypeGdal;

   /**
    * Ctor.
    */
   LibraryConfigDefinitionOriginSource();

   /**
    * Dtor.
    */
   ~LibraryConfigDefinitionOriginSource();

   /**
    * Devuelve el nombre de la fuente de datos. Ej: de ser un archivos
    * de texto plano devolveria el nombre del mismo.
    */
   std::string GetName() const;

   /**
    * Establece el nombre del origen de datos.
    */
   void SetName(const std::string& Name);

   /**
    * Devuelve el tipo de la fuente de datos.
    */
   std::string GetType() const;

   /**
    * Establece el tipo del origen de datos.
    */
   void SetType(const std::string& Type);

   /**
    * Devuelve la ruta de la fuente de datos. Ej: de ser un archivo
    * de texto plano seria la ruta en el sistema de archivos.
    */
   std::string GetPath() const;

   /**
    * Establece la ruta del origen de datos.
    */
   void SetPath(const std::string& Path);

   /**
    * Devuelve el tipo de item. Ej: si es sist. georreferenciado,
    * proyectado, datum o elipsoide.
    */
   int GetItemType() const;

   /**
    * Establece el tipo de item.
    */
   void SetItemType(int ItemType);

private:
   std::string name_;
   std::string type_;
   std::string path_;
   int itemType_;

};

}  // namespace suri

#endif  // LIBRARYCONFIGDEFINITIONORIGINSOURCE_H_
