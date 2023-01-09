/*! \file ListDirsTraverser.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
