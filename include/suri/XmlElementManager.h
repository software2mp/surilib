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

#ifndef XMLELEMENTMANAGER_H_
#define XMLELEMENTMANAGER_H_

// Includes standard
#include <string>
#include <list>
// Includes Suri
#include "suri/FactoryHelpers.h"
// Includes Wx
#include "wx/filename.h"
// Includes App
// Defines
// forwards
class wxArrayString;
class wxXmlNode;

/** namespace suri */
namespace suri {
/** Tipos de formato de path soportados */
enum PathFormatType {
   PATH_NATIVE = 0, /*! path format para plataforma actual */
   PATH_UNIX
/*! path format para sistemas unix* */
};

/** Interfaz con las operaciones que se realizan sobre el nodo de un elemento */
/**
 * Interfaz con las operaciones que se realizan sobre el nodo de un elemento.
 * Ademas define funciones comunes a todos los manager como resolver colisiones
 * de urls, analizar si una ruta es subcarpeta de otra.
 */
class XmlElementManager {
   /** Declara metodos privados y publicos de la clase XmlElementManager. */
HAS_FACTORY(XmlElementManager, std::string);
public:

   /** Ctor */
   XmlElementManager() {
   }
   /** Dtor */
   virtual ~XmlElementManager() {
   }

   /** Mueve rutas a nuevo directorio */
   /**
    * Si Path debajo de From lo cambia a To en Nodo y mueve archivos
    * si MoveFiles=true
    * @param[in] pXmlNode nodo que quiero analizar
    * @param[in] From directorio con las rutas que quiero mover
    * @param[in] To directorio donde se mueven las rutas
    * @param[in] MoveFiles indica si tambien deben moverse los archivos
    * @return indica si se modifico la ruta correctamente
    */
   virtual bool Move(wxXmlNode *pXmlNode, const std::string &From,
                     const std::string &To, bool MoveFiles = false) {
      return false;
   }
   /** Cambia los URL para que esten debajo de To */
   /**
    * @param[in] pXmlNode nodo que quiero analizar
    * @param[in] To directorio donde se mueven las rutas
    * @param[out] NewPaths si se modifico el nodo, se agrega la nueva
    * ruta a este array
    * @param[out] OldPaths si se modifico el nodo, se agrega la
    * ruta original a este array
    * @return indica si se modifico la ruta correctamente
    */
   virtual bool Move(wxXmlNode *pXmlNode, const std::string &To,
                     wxArrayString *NewPaths, wxArrayString *OldPaths) {
      return false;
   }
   /** Verifica que los URL en el documento sean validos */
   /**
    * @param[in] pXmlNode nodo que quiero analizar
    * @param[out] BadUrls urls que son invalidos
    * @return indica si todos los urls son correctos
    */
   virtual bool Check(wxXmlNode *pXmlNode, std::list<wxXmlNode*> &BadUrls) {
      return false;
   }
   /** Hace absolutos a los URLs */
   /**
    * @param[in] pXmlNode nodo que quiero analizar
    * @param[in] Path directorio con respecto al que quiero hacer absolutos
    * los archivos
    * @return indica si se modifico la ruta correctamente.
    */
   virtual bool MakeAbsolute(wxXmlNode *pXmlNode, const std::string &Path) {
      return false;
   }
   /** Hace relativos a los URLs */
   /**
    * @param[in] pXmlNode nodo que quiero analizar
    * @param[in] Path directorio con respecto al que quiero hacer relativos
    * los archivos
    * @param[in] OnlySubfolders flag que indica si los unicos archivos que
    * tengo que hacer relativos son los que estan debajo de Path
    * @return indica si se modifico la ruta correctamente.
    */
   virtual bool MakeRelative(wxXmlNode *pXmlNode, const std::string &Path,
                             bool OnlySubfolders = true) {
      return false;
   }
   /** Cambia el separador del path al formato pedio */
   /**
    * @param[in] pXmlNode nodo que quiero analizar
    * @param[in] PathFormat tipo de formato en que quiero las rutas.
    * @return indica si se modifico la ruta correctamente.
    */
   virtual bool ChangePathFormat(wxXmlNode *pXmlNode, PathFormatType PathFormat) {
      return false;
   }

protected:
   /** Retorna WantedPath si es valido o uno correcto si esto no ocurre */
   std::string ResolveColision(const std::string &Path, wxArrayString *ExistingPaths);
// Verifica si un directorio/archivo es/esta subcarpeta de otro y trae diferencia
   bool IsSubfolder(const std::string &Url, const std::string &Directory,
                    wxArrayString &SubDirs);
// Busca nodo con nombre indicado en partenNode y lo retorna
   bool GetNode(wxXmlNode *pParentNode, const std::string &NodeName, wxXmlNode* &pNode);
};
}  // namespace suri

#endif /* XMLELEMENTMANAGER_H_ */
