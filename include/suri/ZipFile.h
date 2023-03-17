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

#ifndef ZIPFILE_H_
#define ZIPFILE_H_

// Includes standard
#include <deque>
#include <string>
#include <memory>

// Includes Suri

// Includes Wx
#include <wx/zipstrm.h>
#include <wx/wfstream.h>

// Includes App

// Defines

// Forwards

/** namespace suri */
namespace suri {
/** Clase de acceso a archivos ZIP */
/**
 * Esta Clase permite el manejo de archivos comprimidos en formato 'zip',
 * proporcionando una abstraccion de las operaciones basicas sobre dichos
 * archivos:
 *    - Listado de su Contenido
 *    - Obtencion del tamanio de los archivos comprimidos(1 o varios), asi como
 * tambien de la totalidad del archivo zip.
 *    - Extraer archivos(1, varios o todos) en cualquier directorio del FS.
 *    - Agregar archivos(1, varios o todos) al zip.
 */
class ZipFile {
public:
   /** tipo de lista de archivos */
   typedef std::deque<std::string> ZipListType;
   /** ctor */
   ZipFile(const std::string &ZipName);
   /** dtor */
   virtual ~ZipFile() {
   }
   ;
// ------------------------- CONSULTA DE ARCHIVO ----------------------------
   /** Retorna una lista con los archivos del zip */
   ZipListType ListFiles() const;
   /** Retorna el tamanio sin compresion de todos los archivos del zip */
   size_t GetSize() const;
   /** Retorna el tamanio sin compresion del archivo */
   size_t GetSize(const std::string &FileName) const;
   /** Retorna el tamanio sin compresion de los archivos de la lista */
   size_t GetSize(const ZipListType &FileList) const;
// ------------------------ EXTRACCION DE ARCHIVOS --------------------------
   /** Extrae todos los archivos a un path */
   bool Extract(const std::string &Path, bool KeepTree = true,
                bool Overwrite = false) const;
   /** Extrae el archivo del zip */
   bool Extract(const std::string &Filename, const std::string &Path, bool KeepTree =
         true,
                bool Overwrite = false) const;
   /** Extrae los archivos de la lista */
   bool Extract(ZipListType &FileList, const std::string &Path, bool KeepTree = true,
                bool OverWrite = false) const;
// ------------------------- AGREGADO DE ARCHIVOS ---------------------------
   /** Agrega un archivo al zip */
   bool Add(const std::string &FileName, const std::string &Path = "",
            const bool &OverWrite = true);
   /** Agrega una lista de archivos al zip */
   bool Add(ZipListType &FileList, const std::string &Path = "", const bool &OverWrite =
         true);
   /** Agrega una lista de archivos al zip con el path de la lista */
   bool Add(ZipListType &FileList, const ZipListType &PathList, const bool &OverWrite =
         true);

   /** Devuelve el path completo del zip para el cual se instancio la clase. */
   std::string GetZipName() {
      return zipName_;
   }
   ;
   /** Deveulve el mensaje de error seteado en la operacion. */
   std::string GetErrorMsg() {
      return errorMsg_;
   }
   ;
   /** Deveulve lista con urls que no se pudieron copiar */
   ZipListType GetErrorFileList() {
      return errorFiles_;
   }
   ;
   /** Deveulve lista con urls de archivos sobrescritos */
   ZipListType GetOverwritenFileList() {
      return overwritenFiles_;
   }
   ;
protected:
private:
   /** Setea un mensaje de error de alguna operacion. */
   void SetErrorMsg(const std::string &Msg) {
      errorMsg_ = Msg;
   }
   ;
   /** agrega una Lista de archivos a un zip. */
   bool AddFileToZip(wxZipOutputStream *ZipOut, ZipListType &FileList,
                     const ZipListType &PathList);
   /** Determina la lista de archivos a agregar a un zip. */
   ZipListType GetListFilesToAdd(const ZipListType &FilesToAdd,
                                 const ZipListType &PathList, ZipListType &PathToAdd);
   /** Verifica que el archivo en el zip sea uno de los que se solicito la extraccion.																								ZipListType */
// &PathToAdd);
   bool FindFileToExtract(ZipListType &FileList,
                          const std::auto_ptr<wxZipEntry> &Entry) const;
   /** Transforma lista de strings de encoding local al del zip */
   void LocalToZipEncoding(const ZipListType &FileList, ZipListType &NewFileList) const;
   /** Transforma lista de strings de encoding del zip al local */
   void ZipToLocalEncoding(const ZipListType &FileList, ZipListType &NewFileList) const;
   /** Transforma string de encondig local al del zip */
   void LocalToZipEncoding(wxString &Data) const;
   /** Transforma string de encoding del zip al local. */
   void ZipToLocalEncoding(wxString &Data) const;
   /** Transforma ZipListType al encoding indicado */
   void ChangeEncoding(const ZipListType &FileList, ZipListType &NewFileList,
                       const std::string &OriginalEncoding,
                       const std::string &NewEncondig) const;
   /** Transforma string al encoding interno del zip */
   void ChangeEncoding(wxString &Data, const std::string &OriginalEncoding,
                       const std::string &NewEncondig) const;

// ---- Propiedades ----
   std::string zipName_; /*! path completo del archivo zip. */
   std::string errorMsg_; /*! mensaje de error devuelto por las operaciones del */
   /* objeto. */
   ZipListType overwritenFiles_; /*! archivos sobrescritos en ultimo */
   /* extract o add */
   ZipListType errorFiles_; /*! archivos con error en ultimo extract o add */
};
}  // namespace suri

#endif /*ZIPFILE_H_*/
