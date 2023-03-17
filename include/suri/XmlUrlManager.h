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

#ifndef XMLURLMANAGER_H_
#define XMLURLMANAGER_H_

#include "XmlElementManager.h"

/** namspace suri */
namespace suri {
/** Realiza operaciones de XmlElementManager sobre todos los nodos url */
/**
 * Las operaciones analizan si es un nodo url, en caso afirmativo realizan
 * las operaciones indicadas.
 * Las funciones permiten modificar el formato del url y hacerlo relativo
 * o absoluto a un directorio especifico.
 * Tambien se puede mover el archivo a otro directorio, actualizando el
 * url correspondiente
 */
class XmlUrlManager : public XmlElementManager {
public:
   /** Ctor */
   XmlUrlManager();
   /** Dtor */
   virtual ~XmlUrlManager();

   /**
    * Si Path debajo de From loa cambia a To en Nodo y mueve archivos
    * si MoveFiles=true
    */
   virtual bool Move(wxXmlNode *pXmlNode, const std::string &From,
                     const std::string &To, bool MoveFiles = false);

   /** Cambia los URL para que esten debajo de To */
   virtual bool Move(wxXmlNode *pXmlNode, const std::string &To,
                     wxArrayString *NewPaths, wxArrayString *OldPaths);

   /** Verifica que los URL en el documento sean validos */
   virtual bool Check(wxXmlNode *pXmlNode, std::list<wxXmlNode*> &BadUrls);

   /** Hace absolutos a los URLs */
   virtual bool MakeAbsolute(wxXmlNode *pXmlNode, const std::string &Path);

   /** Hace relativos a los URLs */
   virtual bool MakeRelative(wxXmlNode *pXmlNode, const std::string &Directory,
                             bool OnlySubfolders = true);

   /** Cambia el separador del path al formato pedio */
   virtual bool ChangePathFormat(wxXmlNode *pXmlNode, PathFormatType PathFormat);

private:
   /** Inicializa el manager */
   virtual XmlUrlManager* InitializeClass();

   /** Analiza si el Uri corresponde a ruta de sistema de archivos */
   bool IsFileSystemPath(std::string Uri);

};
}  // namespace suri

#endif /* XMLURLMANAGER_H_ */
