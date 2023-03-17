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

#include "suri/XmlElementManager.h"

// Includes standard
// Includes Suri
#include "suri/AbstractFactory.h"
#ifndef __IS_DLL__
#  include "suri/XmlUrlManager.h"
#endif
// Includes Wx
#include "wx/arrstr.h"
#include "wx/dir.h"
#include "wx/filename.h"
#include "wx/xml/xml.h"
// Includes App
// Defines
/** Sin uso */
#define COLISION_DIRECTORY "dir_"

/** namespace suri */
namespace suri {
/** Macro de la factoria que crea metodos de implementacion */
HAS_FACTORY_IMPLEMENTATION_COMMON(XmlElementManager, 0)

/**
 * Crea directorio con formato "dir_num" y agrega al final el nombre
 * del archivo.
 * @param[in] Path con conflicto, si es un directorio debe terminar con /.
 * @param[in] pExistingPaths lista de paths existentes
 */
std::string XmlElementManager::ResolveColision(const std::string &Path,
                                               wxArrayString *pExistingPaths) {
   // Agrego archivos en carpeta destino
   wxArrayString allPaths;
   if (pExistingPaths) {
      allPaths = *pExistingPaths;
   }

   wxFileName filename(Path);

   // Genero directorio que no exista en lista
   bool resolved = false;
   int value = 0;
   wxFileName dirnametest;
   while (!resolved) {
      dirnametest.Assign(filename.GetPathWithSep());
      value += 1;
      std::string aux1 = dirnametest.GetFullPath().c_str();

      std::string directory = dirnametest.GetPath().c_str();
      directory = directory + "_" + wxString::Format("%d", value)
            + wxFileName::GetPathSeparator();
      dirnametest.SetPath(directory);

      std::string aux2 = dirnametest.GetFullPath().c_str();

      // dirnametest.AppendDir(COLISION_DIRECTORY+);
      if ((allPaths.Index(dirnametest.GetPathWithSep()) == wxNOT_FOUND)
            && (!dirnametest.DirExists())) {
         resolved = true;
      }
   }
   // Retorno el direcorio encontrado seguido de nombre del archivo
   dirnametest.SetFullName(filename.GetFullName());
   return dirnametest.GetFullPath().c_str();
}

/**
 * Verifica si un directorio/archivo es/esta subcarpeta de otro y trae
 * diferencia.
 * @param[in] Url directorio que quiero analizar
 * @param[in] Directory directorio del que debe ser subcarpeta
 * @param[out] SubDirs ruta relativa de Url desde Directory
 */
bool XmlElementManager::IsSubfolder(const std::string &Url,
                                    const std::string &Directory,
                                    wxArrayString &SubDirs) {
   bool returnvalue = false;
   // Verifico si el url del nodo esta debajo de from
   wxFileName fnfrom(Directory, "");
   wxFileName fnurl(Url, "");
   wxArrayString dirsurl = fnurl.GetDirs();
   wxArrayString dirsdirectory = fnfrom.GetDirs();

   unsigned int dirarrayposition = 0;
   if (dirsurl.Count() >= dirsdirectory.Count()) {
      wxFileName fntempurl(fnurl.GetVolume(), "", "", "");
      wxFileName fntempdir(fnfrom.GetVolume(), "", "", "");
      std::string aux3;
      std::string aux4;
      while ((fntempurl == fntempdir) && (dirarrayposition < dirsdirectory.Count())) {
         aux3 = fntempurl.GetFullPath().c_str();
         aux4 = fntempdir.GetFullPath().c_str();

         fntempurl.AppendDir(dirsurl[dirarrayposition]);
         fntempdir.AppendDir(dirsdirectory[dirarrayposition]);
         dirarrayposition = dirarrayposition + 1;
      }
      if (fntempurl == fntempdir) {
         // Si el url es subcarpeta de directory guardo dif en ruta
         returnvalue = true;
         while (dirarrayposition < dirsurl.Count()) {
            SubDirs.Add(dirsurl[dirarrayposition]);
            dirarrayposition = dirarrayposition + 1;
         }
      }
   }
   return returnvalue;
}

/**
 * Busca nodo con nombre indicado en partenNode y lo retorna
 * @param[in] pParentNode lugar donde buscara el nodo
 * @param[in] NodeName nombre del nodo que buscara.
 * @param[in] pNode nodo con nombre pedido,no se modifica si no encuentra nada.
 * @return informa si encontro el nodo
 */
bool XmlElementManager::GetNode(wxXmlNode *pParentNode, const std::string &NodeName,
                                wxXmlNode* &pNode) {
   if (!pParentNode) {
      return false;
   }
   wxXmlNode* pnode = pParentNode->GetChildren();
   bool found = false;
   while ((pnode != NULL) && (!found)) {
      if (pnode->GetName().IsSameAs(NodeName.c_str())) {
         found = true;
      } else {
         pnode = pnode->GetNext();
      }
   }
   if (found) {
      pNode = pnode;
   }
   return found;
}
}  // namespace suri
