/*! \file FileManagementFunctions.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#ifndef FILEMANAGEMENTFUNCTIONS_H_
#define FILEMANAGEMENTFUNCTIONS_H_

// Includes estandar
#include <vector>
// Includes Suri

// Includes Wx
#include "wx/xml/xml.h"
#include "wx/filename.h"

// Includes App

// Defines
/** namespace suri */
namespace suri {
// ------------------------------Funciones Soporte-------------------------------

/** Copia directorio From a To */
bool CopyDir(const wxFileName &FnDirFrom, const wxFileName &FnDirTo);

/** Elimina del FS el directorio FnDir. */
bool RemoveDir(const wxFileName &FnDir);

/** Devuelve un path unico para un elemento. */
wxString GetNextPath(const wxString &BasePath, const wxString &DirPrefix = wxT("Elem_"),
                     bool CreateDir = true);

/** Devuelve el espacio total y disponible de un disco */
bool GetDiskSpace(const wxString& Path, wxLongLong *pTotalDiskSpace,
                  wxLongLong *pFreeDiskSpace);

/** Devuelve el size del archivo en bytes **/
unsigned long GetFileSize(const std::string& Path);
// ------------------------------------------------------------------------------
}

#endif /* FILEMANAGEMENTFUNCTIONS_H_ */
