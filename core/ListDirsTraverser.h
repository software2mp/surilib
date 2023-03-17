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

#ifndef LISTDIRSTRAVERSER_H_
#define LISTDIRSTRAVERSER_H_

// Includes standard

// Includes Suri

// Includes Wx
#include <wx/dir.h>

// Includes App

// Defines

// Forwards

/** namespace suri */
namespace suri {
/** clase traverser que lista el 1er nivel de directorios */
/**
 * Clase soporte para recorrer directorios.
 */
class ListDirsTraverser : public wxDirTraverser {
public:
   /** Ctor. default. */
   ListDirsTraverser(wxArrayString& Dirs) :
         m_dirs(Dirs) {
   }
   /** Este metodo es invocado por cada archivo encontrado */
   /**
    * @param[in] WXUNUSED
    * @return wxDIR_CONTINUE para que siga iterando.
    */
   virtual wxDirTraverseResult OnFile(const wxString& WXUNUSED(filename)) {
      return wxDIR_CONTINUE;
   }
   /** Este metodo es invocado por cada directorio encontrado. */
   /**
    * Agrega el directorio (nombre del directorio) al vector de directorios.
    * @param[in] DirName: nombre del directorio.
    * @return wxDIR_IGNORE para que no haga recursion.
    */
   virtual wxDirTraverseResult OnDir(const wxString &DirName) {
      m_dirs.Add(DirName);
      return wxDIR_IGNORE;
   }
private:
   wxArrayString& m_dirs; /*! vector de directorios. */
};
}

#endif /*LISTDIRSTRAVERSER_H_*/
