/*! \file ZipFile.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/ZipFile.h"

// Includes standard
#include <algorithm> // find en GetSize
// Includes standard

// Includes Suri
#include "logmacros.h"
#include "suri/Progress.h"

// Includes Wx
#include "wx/wx.h"
#include <wx/wfstream.h>
#include <wx/filename.h>
#include "wx/sstream.h"
#include "wx/xml/xml.h"
// Includes App

// Defines
/** Codificacion de strings */
#define ZIP_ENCODING "ISO-8859-1"
/** Nodo url. */
#define TEMP_URL_NODE_NAME "url"
// Forwards

/** namespace suri */
namespace suri {
/**
 * Contructor.
 * @param[in]	ZipName: nombre del archivo Zip.
 * @return instancia de la clase ZipFile
 */
ZipFile::ZipFile(const std::string &ZipName) :
      zipName_(ZipName) {
   errorMsg_.clear();
}

// ------------------------- CONSULTA DE ARCHIVO ----------------------------
/**
 * Retorna una lista con los archivos del zip.
 * @return lista de cadenas con los nombres de archivos(no directorios)
 * contenidos en el zip
 */
ZipFile::ZipListType ZipFile::ListFiles() const {
   const_cast<ZipFile*>(this)->errorMsg_.clear();
   ZipListType filesname;
   ZipListType returnfilesname;
   wxZipEntry *pzipentry;

   // open the zip
   wxFFileInputStream in(_T(zipName_));
   wxZipInputStream zip(in);

   while ((pzipentry = zip.GetNextEntry()) != NULL)
      if (!pzipentry->IsDir()) {
         wxFileName fn(pzipentry->GetInternalName().c_str());
         filesname.push_back(fn.GetFullPath(wxPATH_UNIX).c_str());
      }

   zip.CloseEntry();

   ZipToLocalEncoding(filesname, returnfilesname);

   return returnfilesname;
}

/**
 * Retorna el tamanio total que ocupan los archivos contenidos en el zip, sin
 * comprimir.
 * @return tamanio total de los archivos sin comprimir del zip o  0(cero) en caso de
 * que este vacio.
 */
size_t ZipFile::GetSize() const {
   return GetSize(ListFiles());
}

/**
 * Retorna el tamanio que ocupa un archivo particular contenido en el zip,
 * sin comprimir.
 * @param[in]	FileName: nombre del archivo contenido en el zip.
 * @return tamanio del archivo sin comprimir o 0(cero) en caso de que no exista.
 */
size_t ZipFile::GetSize(const std::string &FileName) const {
   ZipListType filesname;
   filesname.push_back(FileName);
   return GetSize(filesname);
}

/**
 * Retorna el tamanio que ocupa una lista de archivos contenidos en el zip,
 * sin comprimir.
 * @param[in]	FileList: lista con los nombre de archivo contenidos en el zip.
 * @return tamanio total de los archivos sin comprimir o 0(cero) en caso de que
 * no existan.
 */
size_t ZipFile::GetSize(const ZipListType &FileList) const {
   wxZipEntry *pzipentry;
   size_t sumsize = 0;
   ZipListType::iterator fit;
   ZipListType filelistaux;
   const_cast<ZipFile*>(this)->errorMsg_.clear();

   if (FileList.empty()) {
      return 0;
   }

   for (size_t i = 0; i < FileList.size(); i++)
      filelistaux.push_back(wxZipEntry(FileList.at(i)).GetInternalName().c_str());

   // open the zip
   wxFFileInputStream in(_T(zipName_));
   wxZipInputStream zip(in);
   while (((pzipentry = zip.GetNextEntry()) != NULL) && !filelistaux.empty())
      /* Uso GetInternalName porque sino no devuelve de forma correcta los
       * nombres de los archivos que se encuentran en subdirectorios dentro
       * del zip.*/
      if (!pzipentry->IsDir()
            && ((fit = find(filelistaux.begin(), filelistaux.end(),
                            pzipentry->GetInternalName().c_str())) != filelistaux.end())) {
         sumsize += pzipentry->GetSize();
         filelistaux.erase(fit);
      }
   return sumsize;
}

// ------------------------ EXTRACCION DE ARCHIVOS --------------------------

/**
 *	Extrae todos los archivos de un zip.
 *	@param[in]	Path: Path donde extraer los archivos.
 *	@param[in]	KeepTree: indica si se debe mantener el arbol de directorios de los
 * archivos.
 *	@param[in]	OverWrite: indica si se deben sobreescribir los archivos.
 *	@return se extrajeron todos los archivos de la lista?true:false.
 */
bool ZipFile::Extract(const std::string &Path, bool KeepTree, bool OverWrite) const {
   // tengo que hacerlo asi xq sino el gcc da error.
   ZipListType filelist = ListFiles();
   return Extract(filelist, Path, KeepTree, OverWrite);
}

/**
 *	Extrae un archivo de un zip.
 *	@param[in]	Filename: nombre del archivo a extraer. (En formato local)
 *	@param[in]	Path: Path donde extraer los archivos.
 *	@param[in]	KeepTree: indica si se debe mantener el arbol de directorios de los
 * archivos.
 *	@param[in]	OverWrite: indica si se deben sobreescribir los archivos.
 *	@return se extrajeron todos los archivos de la lista?true:false.
 */
bool ZipFile::Extract(const std::string &Filename, const std::string &Path,
                      bool KeepTree, bool OverWrite) const {
   ZipListType filelist;
   filelist.push_back(Filename);
   return Extract(filelist, Path, KeepTree, OverWrite);
}

/**
 *	Extrae una lista de archivos de un zip.
 *	@param[in]	FileList: lista de nombres de archivos a extraer.(En formato local)
 *	@param[in]	Path: Path donde extraer los archivos.
 *	@param[in]	KeepTree: indica si se debe mantener el arbol de directorios de los
 * archivos.
 *	@param[in]	OverWrite: indica si se deben sobreescribir los archivos.
 *	@return se extrajeron todos los archivos de la lista?true:false.
 */
bool ZipFile::Extract(ZipListType &FileList, const std::string &Path, bool KeepTree,
                      bool OverWrite) const {
   const_cast<ZipFile*>(this)->errorFiles_.clear();
   const_cast<ZipFile*>(this)->overwritenFiles_.clear();
   const_cast<ZipFile*>(this)->errorMsg_.clear();
   if (FileList.empty()) {
// \todo: Mover a messages.h
      const_cast<ZipFile*>(this)->errorMsg_ =
            "Lista de archivos vacia. No existen archivos para extraer.";
      REPORT_AND_RETURN_VALUE("D:No hay archivos para extraer.", wxSTREAM_NO_ERROR);
   }

   // abro zip para lectura
   std::auto_ptr<wxFFileInputStream> in(new wxFFileInputStream(_T(zipName_)));
   if (!in->IsOk()) {
      REPORT_AND_FAIL_VALUE("D:Error al crear stream de entrada", false);
   }
   wxZipInputStream zipin(*in);
   std::auto_ptr<wxZipEntry> entry;

   // Copio lista de archivos a extraer.
   // TODO: ver si es necesario copiar la lista de archivos; me parece que no.
   ZipListType errorlist;
   ZipListType filestoextract = FileList;
   LocalToZipEncoding(FileList, filestoextract);
   size_t extractedfiles = 0;  // es para verificar errores cuando el ZIP viene vacio.

   suri::Progress progress(FileList.size(), _(progress_EXTRACTING_FILES));
   // recorro zip, creando los archivos
   while (entry.reset(zipin.GetNextEntry()), entry.get() != NULL) {
      // Si el archivo no esta en la lista de archivos a extraer vuelvo a ciclar.
      if (!FindFileToExtract(filestoextract, entry)) {
         continue;
      }

      // creo estructura de directorios para extraer los archivos del zip
      wxFileName zipentry(_T(entry->GetName().c_str() ));
      wxFileName fnpath(Path);

      wxString pathaux = fnpath.GetVolume().c_str()
            + (fnpath.HasVolume() ? fnpath.GetVolumeSeparator() : wxT(""));
      pathaux += fnpath.GetFullPath(wxPATH_UNIX).c_str();
      // Lo transformo para poder concatenarlo con rutas internas
      LocalToZipEncoding(pathaux);
      wxString pathfile = wxT("");
      wxString filename = wxT("");
      // Es un archivo temporal.
      if (!pathaux.IsEmpty() && fnpath.FileExists()) {
         filename = pathaux.c_str();
      } else  // no es un archivo temporal.
      {
         if (!pathaux.IsEmpty()) {
            pathfile = pathaux.c_str();
         }
         // pathfile += zipentry.GetPathSeparator(wxPATH_UNIX);

         if (KeepTree && zipentry.GetDirCount() > 0) {
            pathfile += zipentry.GetPath(wxPATH_GET_VOLUME, wxPATH_UNIX)
                  + zipentry.GetPathSeparator(wxPATH_UNIX);
         }
         if (!pathfile.empty()) {
            filename = pathfile.c_str();
         }

         filename += zipentry.GetFullName();
         if (!pathfile.empty()) {
            // El directorio debe tener encoding local
            ZipToLocalEncoding(pathfile);
            if (!zipentry.Mkdir(pathfile, 0777, wxPATH_MKDIR_FULL)) {
               REPORT_AND_FAIL_VALUE("D:Error al crear directorio de salida.", false);
            }
         }
      }

      // Transformo la codificacion del archivo al sistema local.
      // Esto se hace para que coincida con los datos en xml que estan en formato local.
      ZipToLocalEncoding(filename);

      // Verifico que no exista para no pisarlo
      if (OverWrite || (!OverWrite && !wxFileName(filename).FileExists())) {
         if (wxFileName(filename).FileExists()) {
            const_cast<ZipFile*>(this)->overwritenFiles_.push_back(filename.c_str());
         }

         // creo stream de salida.
         wxFFileOutputStream outarc(_T(filename.c_str() ));
         if (!outarc.IsOk()) {
            const_cast<ZipFile*>(this)->errorMsg_ =
                  "No se extrajeron archivos al Zip. Error en apertura del Zip: "
                        + zipName_;
            REPORT_AND_FAIL_VALUE("D:Error en apertura del Zip: %s", false,
                                  zipName_.c_str());
         }

         outarc.Write(zipin);
         int error;
         if ((error = outarc.GetLastError()) != wxSTREAM_NO_ERROR) {
            const_cast<ZipFile*>(this)->errorFiles_.push_back(filename.c_str());
            outarc.Close();
            continue;
         }
         outarc.Close();
      }
      extractedfiles++;
      progress.Update();
   }

   zipin.CloseEntry();  // para que no queden recursos lockeados.

   if (errorlist.size() > 0 || (!FileList.empty() && extractedfiles == 0)) {
      const_cast<ZipFile*>(this)->errorMsg_ = "Error en la extraccion de los archivos";
      FileList.clear();
      FileList = errorlist;
      REPORT_AND_FAIL_VALUE("D:Error en la extraccion de los archivos", false);
   }
   return true;
}

// ------------------------- AGREGADO DE ARCHIVOS ---------------------------
/**
 * Agrega un archivo al zip.
 * @param[in]	FileName: nombre del archivo a agregar al zip. Debe ser el path
 * completo.(En formato local)
 *	@param[in]	Path: path a crear dentro del zip para el archivo.(En formato local)
 *	@param[in]	OverWrite: indica si debe sobreescribirse el archivo.
 * @return se agrego al zip toda la lista?true:false;
 */
bool ZipFile::Add(const std::string &FileName, const std::string &Path,
                  const bool &OverWrite) {
   if (FileName.empty() && Path.empty()) {
      REPORT_AND_RETURN_VALUE("D:Archivo y Path vacio.", false);
   }

   ZipListType filelist, pathlist;
   filelist.push_back(FileName);
   pathlist.push_back(Path);
   return Add(filelist, pathlist, OverWrite);
}

/**
 * Agrega una lista de archivos al zip.
 * @param[in]	FileList: lista de archivos a agregar al zip. Cada elemento debe
 * ser el path completo.(En formato local)
 *	@param[in]	Path: path a crear dentro del zip para la lista de archivos.(En formato local)
 *	@param[in]	OverWrite: indica si deben sobreescribirse los archivos.
 * @param[out]	FileList: devuelve los nombres de los archivos que no se pudieron
 * insertar al zip.
 * @return se agrego al zip toda la lista?true:false;
 */
bool ZipFile::Add(ZipListType &FileList, const std::string &Path,
                  const bool &OverWrite) {
   if (FileList.empty() && Path.empty()) {
      REPORT_AND_RETURN_VALUE("D:Archivos y Path vacio.", false);
   }

   ZipListType pathlist;
   /* creo una lista con igual cant. de elementos que la cant. de archivos con
    * el mismo path.*/
   for (size_t i = 0; i < FileList.size(); i++)
      pathlist.push_back(Path);

   return Add(FileList, pathlist, OverWrite);
}

/**
 * Agrega una lista de archivos al zip, con una lista de Path.
 * @param[in]	FileList: lista de archivos a agregar al zip. Cada elemento debe
 * ser el path completo.(En formato local)
 *	@param[in]	PathList: lista de Path para la lista de archivos. Debe tener igual
 * cantidad de elementos que FileList.(En formato local)
 *	@param[in]	OverWrite: indica si deben sobreescribirse los archivos.
 * @param[out]	FileList: devuelve los nombres de los archivos que no se pudieron
 * insertar al zip.
 * @return se agrego al zip toda la lista?true:false;
 */
bool ZipFile::Add(ZipListType &FileList, const ZipListType &PathList,
                  const bool &OverWrite) {
   errorFiles_.clear();
   overwritenFiles_.clear();
   errorMsg_.clear();
   if (FileList.empty() || PathList.empty()) {
      errorMsg_ = "No se agregaron archivos al Zip. No hay archivos para agregar.";
      REPORT_AND_RETURN_VALUE("D:No hay archivos para agregar.", false);
   }

   if (FileList.size() != PathList.size()) {
      errorMsg_ =
            "No se pudieron agregar archivos. Cantidad de path distinta a la cantidad de archivos.";
      REPORT_AND_RETURN_VALUE("D:Cantidad de path menor a la cantidad de archivos.",
                              false);
   }

   // Transformo paths a encodign interno de zip
   ZipListType pathlist;
   ZipListType filelist = FileList;
   LocalToZipEncoding(PathList, pathlist);

   if (!wxFileName::FileExists(zipName_.c_str()) || OverWrite) {
      if (wxFileName::FileExists(zipName_.c_str())) {
         overwritenFiles_.push_back(zipName_.c_str());
      }

      // abro el zip.
      wxFFileOutputStream out(_T(zipName_));
      if (!out.IsOk()) {
         errorMsg_ = "No se agregaron archivos al Zip. Error en apertura del Zip: "
               + zipName_;
         REPORT_AND_RETURN_VALUE("D:Error en apertura del Zip: %s", false,
                                 zipName_.c_str());
      }

      wxZipOutputStream zip(out);
      if (!AddFileToZip(&zip, filelist, pathlist)) {
         REPORT_AND_RETURN_VALUE("D:Error en AddFileToZip: %s", false,
                                 zipName_.c_str());
      }

      if (!zip.Close()) {
         REPORT_AND_RETURN_VALUE("D:Error al cerrar Zip: %s", false, zipName_.c_str());
      }
   } else  // si existe Y no quiero sobreescribirlo
   {
      ZipListType pathtoadd;
      ZipListType filestoadd = GetListFilesToAdd(filelist, pathlist, pathtoadd);
      if (filestoadd.empty()) {
         errorMsg_ =
               "No se agregaron archivos al Zip. Archivos ya existentes en el Zip.";
         REPORT_AND_RETURN_VALUE(
               "D:No se agregaron archivos al Zip. Causa: ya existian.", false);
      }

      // abro el zip
      std::auto_ptr<wxFFileInputStream> in(new wxFFileInputStream(_T(zipName_)));
      if (!in->IsOk()) {
         REPORT_AND_RETURN_VALUE("D:Error al crear stream de entrada", false);
      }

      wxTempFileOutputStream out(_T(zipName_));
      if (!out.IsOk()) {
         REPORT_AND_RETURN_VALUE("D:Error al crear stream temporal", false);
      }

      wxZipInputStream inzip(*in);
      wxZipOutputStream outzip(out);
      std::auto_ptr<wxZipEntry> entry;

      // copia info. de cada archivo del zip.
      outzip.CopyArchiveMetaData(inzip);

      // copia cada archivo en el zip a un tmp.
      while (entry.reset(inzip.GetNextEntry()), entry.get() != NULL)
         if (!outzip.CopyEntry(entry.release(), inzip)) {
            break;
         }

      // libera el ptr. al stream de entrada, para que pueda sobreescribirse luego.
      in.reset();

      // agrego archivos restantes.
      if (!AddFileToZip(&outzip, filestoadd, pathtoadd)) {
         REPORT_AND_RETURN_VALUE("D:Error en AddFileToZip: %s", false,
                                 zipName_.c_str());
      }

      if (!outzip.Close()) {
         REPORT_AND_RETURN_VALUE("D:Error al cerrar zip de salida", false);
      }

      if (!out.Commit()) {
         out.Discard();
         REPORT_AND_RETURN_VALUE("D:Error al comitear zip", false);
      }
   }

   return true;
}

// ------------------------- PRIVATE ----------------------------
/**
 *	agrega una Lista de archivos a un zip.
 * @param[in]	ZipOut: stream de un archivo zip para escritura.
 *	@param[in]	FileList: lista de archivos a agregar al zip.
 *	@param[in]	PathList: lista de Path para los archivos a agregar.
 *	@param[out]	ZipOut: stream modificado con los datos agregados
 *	@param[out]	FileList: lista con los archivos que no se pudieron agregar al zip.
 * @return se agrego al zip toda la lista?true:false;
 */
bool ZipFile::AddFileToZip(wxZipOutputStream *ZipOut, ZipListType &FileList,
                           const ZipListType &PathList) {
   ZipListType errorfiles;
   SetErrorMsg("");

   if (FileList.size() == 0) {
      REPORT_AND_RETURN_VALUE("D:Lista de archivos vacia.", false);
   }

   if (FileList.size() != PathList.size()) {
      REPORT_AND_RETURN_VALUE(
            "D:Lista de archivos y path con distinta cantidad de elementos.", false);
   }
   suri::Progress progress(FileList.size(), _(progress_ADDING_FILES));
   for (unsigned int i = 0; i < FileList.size(); i++) {
      if (FileList.at(i).empty()) {  // || dirs.empty()
         REPORT_AND_RETURN_VALUE("D:Nombre de archivo invalido.", false);
      }
      progress.Update();

      wxFileName filename(FileList.at(i));
      if (filename.IsOk() && filename.DirExists() && filename.FileExists()) {
         std::string namefiletozip;
         // Si es un nombre de archivo
         if (!PathList.at(i).empty() && wxFileName(PathList.at(i)).HasExt()) {
            namefiletozip = PathList.at(i).c_str();
         } else {
            // armo el nombre del archivo a guardar en el zip
            namefiletozip = filename.GetFullName().c_str();
            if (!PathList.at(i).empty()) {
               namefiletozip = PathList.at(i) + filename.GetPathSeparator(wxPATH_UNIX)
                     + filename.GetFullName();
            }
         }

         // creo una entrada en el zip
         ZipOut->PutNextEntry(_T(namefiletozip));
         // asocio el archivo a guardar en el zip a un stream de entrada
         wxFFileInputStream in(_T(FileList.at(i) ));
         ZipOut->Write(in);
         in.Reset();
         if (ZipOut->GetLastError() != wxSTREAM_NO_ERROR) {
            errorfiles.push_back(FileList.at(i));
            errorFiles_.push_back(FileList.at(i));
            REPORT_DEBUG("D:%s.\nError LastError: %s",
                         false, errorMsg_.c_str(), ZipOut->GetLastError());
         }
      }
   }

   if (errorfiles.size() > 0) {
      SetErrorMsg("Error de escritura en archivo: ");
      FileList.clear();
      FileList = errorfiles;
      return false;
   }
   return true;
}

/**
 *	Determinar la lista de archivos a agregar a un zip. Para no
 * sobreescribir archivos.
 *	@param[in]	FilesToAdd: lista de posibles archivos a agregar al zip.
 *	@param[in]	PathList: Lista de posibles path para los archivos a agregar al zip.
 *	@param[out]	PathToAdd: Lista de Path a agregar efectivamente al zip.
 *	@return Lista de archivos a agregar efectivamente al zip.
 */
ZipFile::ZipListType ZipFile::GetListFilesToAdd(const ZipListType &FilesToAdd,
                                                const ZipListType &PathList,
                                                ZipListType &PathToAdd) {
   ZipListType listaddfiles = FilesToAdd, listzipfiles = ListFiles();
   ZipListType::iterator fit, pit, delf, delp;
   unsigned int i = 0;
   PathToAdd = PathList;

   if (listaddfiles.empty() || PathToAdd.empty()
         || (listaddfiles.size() != PathToAdd.size())) {
      PathToAdd.clear();
      listaddfiles.clear();
      return listaddfiles;
   }

   // obtengo los archivos que no estan en el zip
   fit = listaddfiles.begin();
   pit = PathToAdd.begin();
   while (!listaddfiles.empty() && fit != listaddfiles.end()) {
      // busco si el archivo a agregar esta dentro del zip(comparo nombres)
      while ((i < listzipfiles.size())
            && (wxZipEntry(*fit).GetInternalName().c_str() != listzipfiles.at(i)))
         i++;
      // si hubo coincidencia, borro el elemento de la lista
      /* NOTA: tengo que incrementar fit de esta forma xq sino da errores al
       * hacer erase.
       */
      if (i < listzipfiles.size()) {
         delf = fit++;
         listaddfiles.erase(delf);
         delp = pit++;
         PathToAdd.erase(delp);
      } else {
         fit++;
         pit++;
      }
      i = 0;
   }
   return listaddfiles;
}

/**
 *	Verifica que el archivo del zip se encuentre en la lista de archivos
 * a extraer.
 * @param[in]	FileList: lista de archivos a extraer.
 *	@param[in]	Entry: puntero a un archivo en el zip.
 * @return archivo se encuentra en la lista?true:false.
 */
bool ZipFile::FindFileToExtract(ZipListType &FileList,
                                const std::auto_ptr<wxZipEntry> &Entry) const {
   ZipListType::iterator fit = FileList.begin();
   bool found = false;
   // busco el archivo del zip en la lista de archivos a extraer.
   while (fit != FileList.end() && !found) {
      wxString zippath, zipname, zipext, entrypath, entryname, entryext;
      wxFileName entryaux(Entry->GetName().c_str());
      entryaux.SplitPath(entryaux.GetFullPath(), &entrypath, &entryname, &entryext);
      wxFileName zipaux(fit->c_str());
      zipaux.SplitPath(zipaux.GetFullPath(), &zippath, &zipname, &zipext);

      if (zipaux.GetDirs().size() > 0) {
         if (entrypath.Cmp(zippath) == 0 && entryname.Cmp(zipname) == 0
               && entryext.Cmp(zipext) == 0) {
            found = true;
         }
      } else {
         if (entryname.Cmp(zipname) == 0 && entryext.Cmp(zipext) == 0) {
            found = true;
         }
      }
      fit++;
   }
   /* Si el archivo no esta en la lista de archivos a extraer devuelvo false
    * para volver a ciclar.*/
   if (!found) {
      return false;
   }
   return true;
}

/**
 * Transforma lista de strings de encoding local al del zip
 * @param[in] FileList lista de urls en formato local
 * @param[out] NewFileList nueva lista de urls en formato de zip
 */
void ZipFile::LocalToZipEncoding(const ZipListType &FileList,
                                 ZipListType &NewFileList) const {
   ChangeEncoding(FileList, NewFileList, wxLocale::GetSystemEncodingName().c_str(),
                  wxT(ZIP_ENCODING));
}

/**
 * Transforma lista de strings de encoding del zip al local
 * @param[in] FileList lista de urls en formato de zip
 * @param[out] NewFileList nueva lista de urls en formato local
 */
void ZipFile::ZipToLocalEncoding(const ZipListType &FileList,
                                 ZipListType &NewFileList) const {
   ChangeEncoding(FileList, NewFileList, wxT(ZIP_ENCODING),
                  wxLocale::GetSystemEncodingName().c_str());
}

/**
 * Transforma string de encondig local al del zip
 * @param[in] Data string en formato local
 * @param[out] Data string en formato de zip
 */
void ZipFile::LocalToZipEncoding(wxString &Data) const {
   ChangeEncoding(Data, wxLocale::GetSystemEncodingName().c_str(), wxT(ZIP_ENCODING));
}

/**
 * Transforma string de encoding del zip al local.
 * @param[in] Data string en formato de zip
 * @param[out] Data string en formato lcoal
 */
void ZipFile::ZipToLocalEncoding(wxString &Data) const {
   ChangeEncoding(Data, wxT(ZIP_ENCODING), wxLocale::GetSystemEncodingName().c_str());
}

/**
 * Transforma lista de strings al encoding pedido
 * @param[in] FileList lista de strings a convertir
 * @param[out] NewFileList lista de strings en codificacion de salida
 * @param[in] OriginalEncoding encoding del string de entrada
 * @param[in] NewEncondig encoding del string de salida
 */
void ZipFile::ChangeEncoding(const ZipListType &FileList, ZipListType &NewFileList,
                             const std::string &OriginalEncoding,
                             const std::string &NewEncondig) const {
   ZipListType::const_iterator it = FileList.begin();
   while (it != FileList.end()) {
      wxString filename((*it).c_str());
      ChangeEncoding(filename, OriginalEncoding, NewEncondig);
      NewFileList.push_back(filename.c_str());
      it++;
   }
}

/**
 * Transforma string al encoding pedido.
 * @param[in] Data string a convertir
 * @param[out] Data string en codificacion de salida
 * @param[in] OriginalEncoding encoding del string de entrada
 * @param[in] NewEncondig encoding del string de salida
 */
void ZipFile::ChangeEncoding(wxString &Data, const std::string &OriginalEncoding,
                             const std::string &NewEncondig) const {
   // Creo documento xml y le agrego nodo root con Data.
   wxXmlDocument *pxmldocument = new wxXmlDocument();
   wxXmlNode *prootnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                        wxT(TEMP_URL_NODE_NAME));
   prootnode->AddChild(new wxXmlNode(prootnode, wxXML_TEXT_NODE, wxT(""), Data));
   pxmldocument->SetRoot(prootnode);
   pxmldocument->SetEncoding(OriginalEncoding);
   // Transformo documento a la nueva codificacion.
   wxStringOutputStream localxmloutput;
   pxmldocument->Save(localxmloutput);
   wxString localxmlstring = localxmloutput.GetString();
   wxStringInputStream localxmlinput(localxmlstring);
   pxmldocument->Load(localxmlinput, NewEncondig);
   // Obtengo url y lo remplazo en Data.
   Data = pxmldocument->GetRoot()->GetNodeContent();

#ifdef __UNUSED_CODE__
   // Este deberia ser el metodo utilizado pero la conversion no funciona para
   // todos los strings. wxWidgets sugiere utilizar:
   // const wchar_t* pmultichar = tempfile.wc_str(wxConvISO8859_1);
   // newfile=wxString(multichar,localencoding,size);
   // El problema que se me presento es que la primer linea no siempre
   // convierte correctamente los URLs. No pude descubrir la razon y por lo
   // tanto termine recurriendo a los nodos.

   // Transformo encoding de string a formato local
   // wxCSConv zipencoding(wxFONTENCODING_ISO8859_1);
   wxCSConv localencoding(wxLocale::GetSystemEncodingName() );
   wxString newfile="";
   wxString tempfile=wxString(FileName);
   if ( Type==ZIP_TO_LOCAL )
   {
      const wchar_t* pmultichar = tempfile.wc_str(wxConvISO8859_1);
      size_t size = wxConvISO8859_1.FromWChar(NULL, 0, pmultichar);
      REPORT_DEBUG("D: SIZE: %d", size);
      newfile=wxString(multichar, localencoding, size);
   }
   if ( Type==LOCAL_TO_ZIP )
   {
      const wchar_t* multichar = tempfile.wc_str(localencoding);
      size_t size = wxConvLocal.FromWChar(NULL, 0, multichar);
      REPORT_DEBUG("D: SIZE: %d", size);
      newfile=wxString(multichar, wxConvISO8859_1);
   }
   if ( !newfile.empty() ) {
      FileName=newfile;
   }
#endif
}
}  // namespace suri
