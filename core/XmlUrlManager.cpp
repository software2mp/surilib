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

#include "suri/XmlUrlManager.h"

// Includes standard
// Includes Suri
#include "suri/xmlnames.h"
#include "suri/FactoryHelpers.h"
#include "suri/Element.h"
#include "logmacros.h"
#include "suri/Uri.h"
// Includes Wx
#include "wx/filefn.h"
#include "wx/xml/xml.h"
#include "wx/filename.h"
// Includes App
// Defines

/**
 * Como en un terreno el url de la textura esta en el elemento interno y en la
 * dependencia se debe actualizar el nodo url en textura cuando se actualiza
 * la dependencia
 * En el futuro se deberia eliminar la dependencia.
 */
#define __TERRAIN_ELEMENT_TEXTURE_URL_CODE__

/** namespace suri */
namespace suri {
/** registra la clase XmlUrlManager */
AUTO_REGISTER_CLASS(XmlElementManager, XmlUrlManager, 0)

XmlUrlManager::XmlUrlManager() {
}

/** Dtor */
XmlUrlManager::~XmlUrlManager() {
}

/**
 * Si Path debajo de From lo cambia a To y mueve archivos
 * si MoveFiles=true.
 * Mueve nodo con todas sus dependencias
 * El nodo debe contener un nodo url y otro de dependencias(opcional).
 * Muevo todas las dependencias debajo de From al mismo directorio
 * que url si su path esta por debajo del path de url. Si hay colision muestro
 * msg de error, esto no deberia ocurrir nunca.
 * @param[in] pXmlNode nodo cuyo URL se intentara mover
 * @param[in] From directorio bajo el cual debe estar el url para que se mueva
 * @param[in] To directorio donde se movera el url
 * @param[in] MoveFiles indica si ademas se debe tratar de copiar el archivo.
 * @return indica si se debe continuar con los hijos de este nodo.
 */
bool XmlUrlManager::Move(wxXmlNode *pXmlNode, const std::string &From,
                         const std::string &To, bool MoveFiles) {
   wxXmlNode* purlnode = NULL;
   if ((!pXmlNode) || (!GetNode(pXmlNode, wxT(URL_NODE), purlnode))) {
      return false;
   }

   if (!IsFileSystemPath(purlnode->GetNodeContent().c_str())) return true;

   wxFileName fnnode(purlnode->GetNodeContent().c_str(), "");
   if (!fnnode.DirExists()) {
      fnnode.Assign(purlnode->GetNodeContent().c_str());
   }

   // Muevo nodo url
   wxFileName fnto(To, "");
   wxArrayString subdirs;
   if (IsSubfolder(fnnode.GetPathWithSep().c_str(), From, subdirs)) {
      // Agrego subcarpetas entre From y Node
      for (unsigned int position = 0; position < subdirs.Count(); position++)
         fnto.AppendDir(subdirs[position]);

      fnto.SetFullName(fnnode.GetFullName());

      // Verifico/Soluciono si hay colision
      if ((!fnto.IsDir() && (fnto.FileExists()))
            || (fnto.IsDir() && (fnto.DirExists()))) {
         fnto.Assign(ResolveColision(fnto.GetFullPath().c_str(), NULL));
      }

      // Guardo cambios
      purlnode->GetChildren()->SetContent(fnto.GetFullPath());
      if (MoveFiles) {
         // Muevo carpeta.
         if (!fnto.DirExists()) {
            wxFileName::Mkdir(fnto.GetPathWithSep(), 0777, wxPATH_MKDIR_FULL);
         }
         if (!fnto.IsDir()) {
            wxRenameFile(fnnode.GetFullPath(), fnto.GetFullPath());
         }
      }
   }
   // Busco si hay nodo dependencias y muevo paths al mismo directorio
   wxXmlNode* pdependencies;
   if (!GetNode(pXmlNode, wxT(DEPENDENCY_NODE), pdependencies)) {
      return true;
   }
   purlnode = pdependencies->GetChildren();
   while (purlnode != NULL) {
      // Genero nombre destino y lo agrego a lista de paths existentes.
      wxFileName fnurl(purlnode->GetNodeContent().c_str());
      wxArrayString subdirsdep;
      if (IsSubfolder(fnurl.GetPathWithSep().c_str(), From, subdirsdep)) {
         // Muevo nodo url
         wxFileName fntodep(To, "");
         // Si es subcarpeta de archivo url muevo a mismo to
         wxArrayString subdirsdifdepurl;
         if (IsSubfolder(fnurl.GetPathWithSep().c_str(),
                         fnnode.GetPathWithSep().c_str(), subdirsdifdepurl)) {
            fntodep.Assign(fnto);
            // Agrego subcarpetas entre From y Dep
            for (unsigned int position = 0; position < subdirsdifdepurl.Count();
                  position++)
               fntodep.AppendDir(subdirsdifdepurl[position]);
         } else {
            // Agrego subcarpetas entre From y Dep
            for (unsigned int position = 0; position < subdirsdep.Count(); position++)
               fntodep.AppendDir(subdirsdep[position]);
         }

         fntodep.SetFullName(fnurl.GetFullName());

         // Verifico si hay colision
         if ((!fntodep.IsDir() && (fntodep.FileExists()))
               || (fntodep.IsDir() && (fntodep.DirExists()))) {
            fntodep.Assign(ResolveColision(fntodep.GetFullPath().c_str(), NULL));
         }

         if (fntodep.FileExists()) {
            REPORT_ERROR("Colision al copiar dependencia");
         }
         // Guardo cambios
         purlnode->GetChildren()->SetContent(fntodep.GetFullPath());

#ifdef __TERRAIN_ELEMENT_TEXTURE_URL_CODE__

         Element* pterrain = Element::Create("TerrainElement", pXmlNode);
         // Si es un TerrainElement tambien cambio el url de la textura
         if (pterrain) {
            wxXmlNode* pgenericnode = pXmlNode;
            GetNode(pgenericnode, wxT("renderizacion"), pgenericnode);
            if (pgenericnode) {
               GetNode(pgenericnode, wxT("textura"), pgenericnode);
               if (pgenericnode) {
                  GetNode(pgenericnode, wxT("elemento"), pgenericnode);
                  if (pgenericnode) {
                     GetNode(pgenericnode, wxT(URL_NODE), pgenericnode);
                     if (pgenericnode) {
                        pgenericnode->GetChildren()->SetContent(fntodep.GetFullPath());
                     }
                  }
               }
            }
         }
#endif

         if (MoveFiles) {
            // Si hay colision creo la subcarpeta.
            if (!fntodep.DirExists()) {
               wxFileName::Mkdir(fntodep.GetPathWithSep(), 0777, wxPATH_MKDIR_FULL);
            }
            if (!fntodep.IsDir()) {
               wxRenameFile(fnurl.GetFullPath(), fntodep.GetFullPath());
            }
         }
      }
      purlnode = purlnode->GetNext();
   }

   return true;
}

/**
 * Cambia los URL para que esten debajo de To. Primero busca si el nodo
 * contiene otro de nombre URL, en caso afirmativo lo mueve a To. Luego busca
 * si hay nodo dependencias y mueve los archivos a la misma direccion.
 * @param[in] pXmlNode nodo cuyo URL se intentara mover
 * @param[in] To directorio donde se movera el url
 * @param[out] NewPaths array con las nuevas rutas
 * @param[out] OldPaths array con rutas originales
 * @return indica si se debe continuar con los hijos de este nodo.
 */
bool XmlUrlManager::Move(wxXmlNode *pXmlNode, const std::string &To,
                         wxArrayString *NewPaths, wxArrayString *OldPaths) {
   // Si nodo incorrecto o no se encontro URL falla
   wxXmlNode* purlnode = NULL;
   if ((!pXmlNode) || (!GetNode(pXmlNode, wxT(URL_NODE), purlnode))) {
      return false;
   }

   if (!IsFileSystemPath(purlnode->GetNodeContent().c_str())) return true;

   wxFileName fnfrom(purlnode->GetNodeContent().c_str());
   OldPaths->Add(purlnode->GetNodeContent().c_str());
   // Genero nombre destino
   wxFileName fnto(To, "");
   fnto.SetFullName(fnfrom.GetFullName());
   purlnode->GetChildren()->SetContent(fnto.GetFullPath());
   // Genero directorio para que tenga posicion unica
   fnto.Assign(ResolveColision(fnto.GetFullPath().c_str(), NewPaths));
   // cargo nuevo URL en lista de Paths.
   NewPaths->Add(fnto.GetFullPath());

   // Busco si hay nodo dependencias y agrego paths al mismo directorio
   wxXmlNode* dependencies;
   if (!GetNode(pXmlNode, wxT(DEPENDENCY_NODE), dependencies)) {
      return true;
   }
   purlnode = dependencies->GetChildren();
   while (purlnode != NULL) {
      // Genero nombre destino y lo agrego a lista de paths existentes.
      wxFileName fnurl(purlnode->GetNodeContent().c_str());
      OldPaths->Add(purlnode->GetNodeContent().c_str());
      fnto.SetFullName(fnurl.GetFullName());
      NewPaths->Add(fnto.GetFullPath());
      purlnode->GetChildren()->SetContent(fnto.GetFullPath());
      purlnode = purlnode->GetNext();
   }

   return true;
}

/**
 * Verifica que los URL en el documento sean validos
 * @param[in] pXmlNode nodo cuyo URL se intentara encontrar
 * @param[out] BadUrls Si no se encuentra el path se agrega a esta lista.
 * @return indica si se debe continuar con los hijos de este nodo.
 * \attention los nodos en BadUrls son los mismos que estan en pXmlNode y
 * no deben destruirse.
 */
bool XmlUrlManager::Check(wxXmlNode *pXmlNode, std::list<wxXmlNode*> &BadUrls) {
   // Si no es URL lo ignoro
   if ((!pXmlNode) || (pXmlNode->GetName().Cmp(wxT(URL_NODE)) != 0)) {
      return false;
   }

   if (!IsFileSystemPath(pXmlNode->GetNodeContent().c_str())) return true;

   // Si el nodo esta vacio lo considero sin path
   if (!(pXmlNode->GetChildren()) || (pXmlNode->GetChildren()->IsWhitespaceOnly())) {
      return true;
   }

   wxFileName fnurl(pXmlNode->GetNodeContent().c_str());
   if (!fnurl.IsDir() && !fnurl.FileExists()) {
      // Puedo tener el caso de un directorio sin el ultimo /
      wxFileName fndir(pXmlNode->GetNodeContent().c_str(), "");
      if (!fndir.DirExists()) {
         BadUrls.push_back(pXmlNode);
      }
   } else {
      if (fnurl.IsDir() && !fnurl.DirExists()) {
         BadUrls.push_back(pXmlNode);
      }
   }

   return true;
}

/**
 * Hace absolutos a los URLs. Si el URL esta vacio no lo considero el dir Path.
 * @param[in] pXmlNode nodo cuyo URL se intentara hacer absoluto
 * @param[in] Path ruta que se usa para hacer url absoluto.
 * @return indica si se debe continuar con los hijos de este nodo.
 */
bool XmlUrlManager::MakeAbsolute(wxXmlNode *pXmlNode, const std::string &Path) {
   // Si no es URL lo ignoro
   if ((!pXmlNode) || (pXmlNode->GetName().Cmp(wxT(URL_NODE)) != 0)) {
      return false;
   }

   if (!IsFileSystemPath(pXmlNode->GetNodeContent().c_str())) return true;

   // Si el nodo esta vacio lo considero sin path y no como relativo a Path
   if (!(pXmlNode->GetChildren()) || (pXmlNode->GetChildren()->IsWhitespaceOnly())) {
      return true;
   }

   wxFileName fnpat(pXmlNode->GetNodeContent().c_str());
   if ((fnpat.IsRelative()) && (fnpat.MakeAbsolute(Path))) {
      pXmlNode->GetChildren()->SetContent(fnpat.GetFullPath());
   }

   return true;
}

/**
 * Hace relativos a los URLs
 * @param[in] pXmlNode nodo cuyo URL se intentara hacer relativo
 * @param[in] Directory ruta bajo las cual deben estar el URL para hacerce
 * relativo.
 * Se respetan los subdirectorios de Path en ruta relativa
 * @param[in] OnlySubfolders indica si solo deben hacerse relativos al
 * directorio los archivos que estan por debajo del mismo.
 * @return indica si se debe continuar con los hijos de este nodo.
 */
bool XmlUrlManager::MakeRelative(wxXmlNode *pXmlNode, const std::string &Directory,
                                 bool OnlySubfolders) {
   // Si no es URL lo ignoro
   if ((!pXmlNode) || (pXmlNode->GetName().Cmp(wxT(URL_NODE)) != 0)) {
      return false;
   }

   if (!IsFileSystemPath(pXmlNode->GetNodeContent().c_str())) return true;

   // Si el nodo esta vacio lo considero sin path
   if (!(pXmlNode->GetChildren()) || (pXmlNode->GetChildren()->IsWhitespaceOnly())) {
      return true;
   }

   // Veo si es absoluto
   wxFileName fnnode(pXmlNode->GetNodeContent().c_str());

   // Veo si es subcarpeta de Directory
   if (fnnode.IsAbsolute()) {
      wxArrayString subdirs;
      // Verifico si url esta en from
      if ((!OnlySubfolders)
            || (IsSubfolder(fnnode.GetPathWithSep().c_str(), Directory, subdirs))) {
         fnnode.MakeRelativeTo(Directory);
         pXmlNode->GetChildren()->SetContent(fnnode.GetFullPath());
      }
   }

   return true;
}

/**
 * Cambia el separador del path al formato pedido
 * @param[in] pXmlNode nodo en el que se intentara cambiar el formato del url
 * @param[in] PathFormat nuevo formato del path
 * @return indica si se debe continuar con los hijos de este nodo.
 */
bool XmlUrlManager::ChangePathFormat(wxXmlNode *pXmlNode, PathFormatType PathFormat) {
   // Si no es URL lo ignoro
   if ((!pXmlNode) || (pXmlNode->GetName().Cmp(wxT(URL_NODE)) != 0)) {
      return false;
   }

   if (!IsFileSystemPath(pXmlNode->GetNodeContent().c_str())) return true;

   // Si el nodo esta vacio lo considero sin path
   if (!(pXmlNode->GetChildren()) || (pXmlNode->GetChildren()->IsWhitespaceOnly())) {
      return true;
   }

   // Veo si es absoluto
   wxFileName fnpat(pXmlNode->GetNodeContent().c_str());

   wxString fullpath = fnpat.GetVolume();
   if (!fullpath.IsEmpty()) {
      fullpath += fnpat.GetVolumeSeparator();
   }
   fullpath += fnpat.GetPath(wxPATH_GET_SEPARATOR, (wxPathFormat) PathFormat)
         + fnpat.GetFullName();

   pXmlNode->GetChildren()->SetContent(fullpath.c_str());
   return true;
}

XmlUrlManager* XmlUrlManager::InitializeClass() {
   return this;
}

/**
 * Analiza si el Uri corresponde a ruta de sistema de archivos
 * @param[in] Uri string con uri a analizar
 * @return bool que indica si el url corresponde a ruta de sistema de archivos
 */
bool XmlUrlManager::IsFileSystemPath(std::string NodeUri) {
   //Para analizar si es path de SO analiza el tamanio del scheme,
   //longitud 0(unix) o longitud 1(windows)

   Uri uri(NodeUri);
   std::string schema = uri.GetScheme();
   return schema.size() == 0 || schema.size() == 1;
}

}  // end namspace suri

