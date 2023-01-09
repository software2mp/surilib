/** \file LibraryConfig.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LIBRARYCONFIG_H_
#define LIBRARYCONFIG_H_

#include <string>

// forward wx
class wxXmlDocument;
class wxXmlNode;

namespace suri {

class LibraryConfigDefinition;

/**
 * Carga la configuracion de bibliotecas para que este disponible para la aplicacion.
 */
class LibraryConfig {
public:
   /**
    * Devuelve el LibraryConfigDefinition correspondiente la nombre dado.
    */
   LibraryConfigDefinition* GetDefinitionByName(const std::string& DefName);
   /** Definicion de nombres de nodos de configuracion de bibliotecas **/
   static std::string LibraryConfigNodeName;
   static std::string LibrariesDefNodeName;
   static std::string LibraryDefNodeName;
   static std::string LibraryOriginsNodeName;
   static std::string LibraryOriginNodeName;
   static std::string LibraryFavIndexNodeName;
   static std::string LibrarySourceProperty;
   static std::string LibraryPathProperty;
   static std::string LibraryTypeProperty;
   static std::string LibraryNameProperty;
   static std::string LibraryItemTypeProperty;
   static std::string LibraryCapabilitiesProperty;
   static std::string LibraryLevelProperty;
   static std::string LibraryConfigFileName;

   static LibraryConfig* GetInstance() {
      static LibraryConfig* plibconfig = new LibraryConfig();
      return plibconfig;
   }

private:
   /** Ctor. */
   LibraryConfig();

   /** Dtor. */
   ~LibraryConfig();
   /** Carga los origenes que contiene el nodo <origin> **/
   void LoadOrigins(LibraryConfigDefinition* pLibConfigDef,
                    wxXmlNode* pOriginsNode) const;
   /** Carga los favoritos que se encuentran configurados en el indice
    *  de favoritos
    */
   void LoadFavorites(LibraryConfigDefinition* pLibConfigDef,
                                   wxXmlNode* pOriginsNode) const;
};

}  // namespace suri

#endif  // LIBRARYCONFIG_H_
