/*! \file FileManagementFunctions.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/FileManagementFunctions.h"
// Includes estandar
#include <list>

// Includes Suri
#include "logmacros.h"
#include "wxmacros.h"
#include "suri/messages.h"
#include "suri/Configuration.h"

// Includes Wx
#include "wx/arrstr.h"
#include "wx/dir.h"
#include "wx/msgdlg.h"
#include "wx/filefn.h"
// Includes App
#include "ListDirsTraverser.h"

// Defines
/** Permisos de creacion */
#define CREATE_PERMISSIONS 0755

namespace suri {
// --------------------------------------------

/**
 * \pre para que FnDirFrom y FnDirTo sean directorios validos, el path debe terminar en "/".
 * @param[in]	FnDirFrom: wxFileName que contiene el path del directorio origen
 * @param[in]	FnDirTo: wxFileName que contiene el path del directorio destino
 * @return true en caso de que se copie con exito el directorio. false C.O.C.
 */
bool CopyDir(const wxFileName &FnDirFrom, const wxFileName &FnDirTo) {
   // verifico que el directorio origen exista
   if (!FnDirFrom.IsDir() || !FnDirFrom.DirExists()) {
      REPORT_AND_FAIL_VALUE("D:Error al intentar copiar: %s", false,
                            FnDirFrom.GetFullPath().c_str());
   }
   // verifico que el directorio destino exista
   if (!FnDirTo.IsDir()) {
      REPORT_AND_FAIL_VALUE("D:Error al intentar copiar: %s", false,
                            FnDirTo.GetFullPath().c_str());
   }

   // verifico que no sean el mismo directorio.
   // si falla, como es que retorna true?
   if (FnDirFrom == FnDirTo) {
      REPORT_AND_FAIL_VALUE("D:El destino y origen son el mismo: %s", true,
                            FnDirFrom.GetFullPath().c_str());
   }

   // si el destino no existe lo creo.
   if (!FnDirTo.DirExists()) {
      if (!wxFileName::Mkdir(FnDirTo.GetFullPath(), CREATE_PERMISSIONS,
                             wxPATH_MKDIR_FULL)) {
         REPORT_AND_FAIL_VALUE("D:Error al crear directorio %s.", false,
                               FnDirTo.GetFullPath().c_str());
      }
   }

   // abro dir. origen para obtener elementos y copiarlos a dir. destino.
   wxDir dirfrom(FnDirFrom.GetFullPath());
   if (!dirfrom.IsOpened()) {
      REPORT_AND_FAIL_VALUE("D:Error al intentar abrir directorio: %s", false,
                            FnDirFrom.GetFullPath().c_str());
   }
   wxString elementfrom;
   if (dirfrom.GetFirst(&elementfrom)) {
      do {
#ifdef __DEBUG__
         wxString aux2Form = FnDirFrom.GetFullPath();
         aux2Form = aux2Form;

         wxString auxTo = FnDirTo.GetFullPath();
         auxTo = auxTo;
#endif  // __DEBUG__
         if (wxDirExists(FnDirFrom.GetFullPath() + elementfrom)) {
            // si dir. destino no existe lo creo.
            if (!wxDirExists(FnDirTo.GetFullPath() + elementfrom)) {
               if (!wxFileName::Mkdir(FnDirTo.GetFullPath() + elementfrom,
                                      CREATE_PERMISSIONS, wxPATH_MKDIR_FULL)) {
                  REPORT_AND_FAIL_VALUE(
                        "D:Error al crear directorio %s.", false,
                        wxString( (FnDirTo.GetFullPath() + elementfrom) ).c_str());
               }
            }

            /* agrego la barra al final de elementfrom para que me lo tome como
             * dir. en la ste. iteracion. Sino hago esto falla.*/
            /*****************/
//            wxString aux2Form = FnDirFrom.GetFullPath();
//            wxString auxTo = FnDirTo.GetFullPath();
            /*****************/

            elementfrom += wxFileName::GetPathSeparator();
            wxFileName newfnfrom(FnDirFrom.GetFullPath() + elementfrom);
            wxFileName newfnto(FnDirTo.GetFullPath() + elementfrom);
            CopyDir(newfnfrom, newfnto);
         } else if (!wxCopyFile(FnDirFrom.GetFullPath() + elementfrom,
                                FnDirTo.GetFullPath() + elementfrom)) {
            REPORT_DEBUG("D:Error al intentar copiar: %s",
                         wxString(FnDirFrom.GetFullPath() + elementfrom).c_str());
         }
      } while (dirfrom.GetNext(&elementfrom));
   }

   return true;
}

/**
 * Objetivo: Eliminia el Directorio pasado por parametro y su contenido.
 * @param[in]	FnDir: wxFileName con el path del directorio.
 * \post Se eliminan todos los directorios posibles.
 * \post En caso de encontrar un error, no elimina los archivos del directorio
 *       en curso.
 * @return true en caso de que se eliminen todos los directorios
 * @return false en caso de que al menos 1 directorio/archivo no haya podido
 *  ser eliminado.
 */
bool RemoveDir(const wxFileName &FnDir) {
   // convierto a un directorio (si FnDir se construyo sin terminador)
   //wxFileName directory(FnDir.GetFullPath(), wxEmptyString);
   wxFileName directory(FnDir.GetPath(), wxEmptyString);
   // si el directorio no existe salgo con exito (puedo decir que lo borre)
   if (!wxDir::Exists(directory.GetPath())) {
      return true;
   }
   wxArrayString dirs;
   wxArrayString files;
   ListDirsTraverser traverser(dirs);
   size_t count = 0;
   /* hago la creacion del wxDir dentro del bloque ya que al hacerlo abre el
    * directorio y necesito que libere el directorio para poder eliminarlo. La
    * liberacion del recurso sucede cuando se destruye el wxDir.
    */
   {
      wxDir dir(directory.GetPathWithSep());
      count = dir.Traverse(traverser);
   }
   if (count == static_cast<size_t>(-1)) {
      REPORT_DEBUG("D:No se enumero el directorio");
   }
   wxDir::GetAllFiles(directory.GetPath(), &files, wxT(""), wxDIR_FILES);
   dirs.Sort();
   files.Sort();
   bool result = true;
   for (size_t i = 0; i < dirs.Count(); i++)
      if (!RemoveDir(wxFileName(dirs.Item(i), wxT("")))) {
         REPORT_DEBUG(
               "D:Error al intentar eliminar directorio: %s. Verifique que exista y que no este siendo usado.",
               dirs.Item(i).c_str());
         result = false;
         // trato de borrar los directorios
         // si el ultimo directorio se repite 40 veces, corta la ejecucion
         // sirve para protejerme de borrar directorios importantes.
         // Agrego un directorio .0 con un symlink adentro .0 -> .
         // fuerza una recursion (que por alguna razon llega hasta ~40) y luego
         // empieza a salir.
         if (directory.GetPath().Replace(
               directory.GetDirs().Item(directory.GetDirs().Count() - 1), wxT(""))
               > 35) {
            if (SHOWQUERY(NULL, question_TOO_MANY_DIRECTORY_RECURSIONS,
                  wxYES_NO|wxNO_DEFAULT) == wxID_NO) {
               break;
            }
         }
      }
   // si hubo errores al eliminar los directorios dentro del actual, no
   // elimino los archivos (me protejo)
   if (result) {
      // elimino los archivos del directorio actual
      for (size_t i = 0; i < files.Count(); i++)
         if (!wxRemoveFile(files.Item(i))) {
            REPORT_DEBUG(
                  "D:Error al intentar eliminar archivo: %s. Verifique que exista y que no este siendo usado.",
                  files.Item(i).c_str());
            result = false;
         }
   }REPORT_DEBUG("D:Eliminando %s", directory.GetPathWithSep().c_str());
   // condicion de salida
   return result && wxRmdir(directory.GetPathWithSep());
}

/**
 * Genera un nombre de directorio siguiendo la regla \verbatim <Prefijo>+<numero> \endverbatim.
 * \pre BasePath debe existir
 * \post Se genera un nombre de directorio y, optativamente, se crea dicho
 *      directorio segun la regla \verbatim <prefijo>+<numero> \endverbatim que
 *      no exista en BasePath.
 * @param[in] BasePath es la ruta del directorio donde se desea crear el nuevo.
 * @param[in] DirPrefix prefijo para la creacion del nuevo nombre de directorio.
 * @param[in] CreateDir flag para indicar si se debe crear el directorio o solo
 *            generar el nombre.
 * @return wxString con el nombre de directorio.
 */
wxString GetNextPath(const wxString &BasePath, const wxString &DirPrefix,
                     bool CreateDir) {
   wxArrayString dirs;
   ListDirsTraverser traverser(dirs);
   wxDir dir(BasePath);
   dir.Traverse(traverser, DirPrefix + wxT("*"));

   // recorro dirs, para obtener el nombre del proximo directorio a utlizar.
   std::list<int> indexes;
   for (size_t i = 0; i < dirs.Count(); i++) {
      // \todo hacer que busque el ultimo DirPrefix ya que si hay mas de uno rompe
      if (dirs.Item(i).Find(DirPrefix) != wxNOT_FOUND) {
         long index = 0;
         if (dirs.Item(i).AfterLast(DirPrefix[DirPrefix.Length() - 1]).ToLong(&index)) {
            indexes.push_back(index);
         }
      }
   }
   indexes.sort();
   std::list<int>::iterator it = indexes.begin();
   int lastval = it != indexes.end() ? *it : 0;
   bool found = false;
   while (it != indexes.end() && !found) {
      if (*it - lastval > 1) {
         found = true;
      } else {  // cuando lo encuentra, no debo incrementar
         lastval = *it + 1;
      }
      it++;
   }

   /* tengo que agregarle el separador de directorios al path porque sino el
    * metodo wxFileName::DirExits devuelve true aunque no exista el directorio.
    */
   wxString newpath = wxString::Format(
         "%s%d%s", (BasePath + DirPrefix).c_str(), lastval,
         wxString(wxFileName::GetPathSeparator(wxPATH_UNIX)).c_str());
   wxFileName fn(newpath);
   // Verifico estar retornando un directorio que no exista
   if (fn.DirExists()) {
      REPORT_AND_FAIL_VALUE("D:El directorio generado ya existe", wxT(""));
   }
   if (CreateDir) {
      // Creo el directorio
      if (!fn.Mkdir(CREATE_PERMISSIONS, wxPATH_MKDIR_FULL)) {
         REPORT_AND_FAIL_VALUE("D:No se pudo crear el directorio: %s", wxT(""),
                               fn.GetFullPath().c_str());
      }
   }
   // valor de la ruta formateada
   return fn.GetVolume() + fn.GetVolumeSeparator() + fn.GetFullPath(wxPATH_UNIX);;
}

/**
 * Devuelve el espacio total y disponible de un disco
 * @param[in] Path : directorio
 * @param[out] pTotalDiskSpace: espacio total en disco
 * @param[out] pFreeDiskSpace: espacio disponible en disco
 * @return true/false : si existe o no el directorio
 */
bool GetDiskSpace(const wxString& Path, wxLongLong *pTotalDiskSpace,
                  wxLongLong *pFreeDiskSpace) {
   if (!wxGetDiskSpace(Path, pTotalDiskSpace, pFreeDiskSpace)) {
      return false;
   }
   return true;
}

/** Devuelve el size del archivo en bytes **/
/** Devuelve el size del archivo en bytes **/
unsigned long GetFileSize(const std::string& Path) {
   wxFileName file(Path);
   return file.GetSize().ToULong();
}
}  /** namespace suri **/

