/** \file FileCache.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "WxFileCache.h"

// Includes Estandar
#include <string>
#include <map>

// Includes Suri
#include "wxmacros.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/sstream.h"
#include <wx/fs_mem.h>
#include <wx/ffile.h>

// Defines

// forwards

namespace suri {

/**
 *  Inicializacion de la instancia con la definicion
 */
WxFileCache* WxFileCache::pInstance_;

/**
 * Constructor por defecto
 */
WxFileCache::WxFileCache() :
      hashmap_() {
}

/**
 * Constructor de copia
 */
WxFileCache::WxFileCache(const WxFileCache&) {
}

/**
 * Destructor
 */
WxFileCache::~WxFileCache() {
}

/**
 *  Devuelve la instancia (la primera vez la crea, las siguientes la accede)
 *  @return Instancia del Singleton WxFileCache
 */
WxFileCache* WxFileCache::Instance() {
   if (!pInstance_) {
      pInstance_ = new WxFileCache();
   }
   return pInstance_;
}

/**
 *  Crea un archivo en memoria que se asocia al archivo determinado por la url
 *  @param[in] FileUrl: String con la Url del archivo.
 *  @return Archivo en memoria (wxString). Si falla el wxString es vacio.
 */
wxString WxFileCache::AddFile(const std::string& FileUrl) {
   wxString memfile = CreateMemoryFile(FileUrl);
   MapAddResultPairType result = hashmap_.insert(FileCachePairType(FileUrl, memfile));
   if (result.second == false) {
      return wxT("");
   }
   return memfile;
}

/** Reescala un archivo de imagen y lo agrega al cache y a la memoria */
wxString WxFileCache::AddImageFile(const std::string& FileUrl, const size_t Width,
                                   const size_t Height) {
   wxString memfile = CreateMemoryImageFile(FileUrl, Width, Height);
   MapAddResultPairType result = hashmap_.insert(FileCachePairType(FileUrl, memfile));
   if (result.second == false) return wxT("");
   return memfile;
}

/**
 *  Devuelve el archivo de memoria asociado al url dado
 *  @param[in] FileUrl: String con la Url del archivo.
 *  @return Devuelve el archivo de memoria asociado al url parametro
 */
wxString WxFileCache::GetFile(const std::string& FileUrl) {
   FileCacheHashmapType::const_iterator it = hashmap_.find(FileUrl);
   if (it == hashmap_.end()) return wxT("");
   return it->second;
}

/**
 *  Quita el archivo de la memoria y del cache
 *  @param[in] FileUrl: String con la Url del archivo.
 *  @return Devuelve verdadero si tuvo exito, sino devuelve falso
 */
bool WxFileCache::RemoveFile(const std::string& FileUrl) {
   wxString memfile = this->GetFile(FileUrl);
   DeleteMemoryFile(memfile);
   int erased = hashmap_.erase(FileUrl);
   return (erased > 0);
}

/**
 * Devuelve la cantidad de elementos contenidos en el Cache
 * @return cantidad de elementos contenidos en el Cache
 */
size_t WxFileCache::CountFiles() const {
   return hashmap_.size();
}

/**
 * Determina si el Cache esta vacio
 * @return verdadero si esta vacio
 */
bool WxFileCache::IsEmpty() const {
   return hashmap_.empty();
}

/**
 * Crea un archivo en memoria con la ayuda de la libreria wxwidgets
 * @param[in] FileUrl: String con la Url del archivo.
 * @return el archivo creado en memoria (un wxString al url en memoria)
 */
wxString WxFileCache::CreateMemoryFile(const std::string& FileUrl) const {
   wxFFile file(FileUrl.c_str(), "rb");
   size_t size = file.Length();
   char* binarydata = new char[size];
   file.Read(binarydata, size);

   wxFileName filename(FileUrl);
   wxString extension = filename.GetExt();
   wxString mimetype = GetMimeTypeFromExtension(extension);

   wxMemoryFSHandler::AddFileWithMimeType(FileUrl.c_str(), binarydata, size, mimetype);

   return wxString(wxT("memory:") + FileUrl);
}

/**
 * Crea un archivo de imagen en memoria con la ayuda de la libreria wxwidgets
 * @param[in] FileUrl: String con la Url del archivo.
 * @return el archivo de imagen creado en memoria (un wxString al url en memoria)
 */
wxString WxFileCache::CreateMemoryImageFile(const std::string& FileUrl,
                                            const size_t Width,
                                            const size_t Height) const {
   wxBitmap bitmap;
   GET_BITMAP_RESOURCE(FileUrl.c_str(), bitmap);
   ESCALE_BITMAP(bitmap, (static_cast<int>(Width)), (static_cast<int>(Height)));
   wxMemoryFSHandler::AddFile(FileUrl.c_str(), bitmap, wxBITMAP_TYPE_PNG);
   return wxString(wxT("memory:") + FileUrl);
}

/**
 * Elimina un archivo en memoria con la ayuda de la libreria wxwidgets
 * @param[in] FileUrl: String con la Url del archivo.
 * @return verdadero si elimino el archivo de memoria con exito
 */
void WxFileCache::DeleteMemoryFile(const wxString& FileUrl) const {
   wxMemoryFSHandler::RemoveFile(FileUrl);
}

/**
 * Devuelve el mimetype de acuerdo a una lista limitada de extensiones
 * @param[in] FileUrl: String con la extension
 * @return Devuelve un wxString vacio si falla, sino el mimetype
 */
wxString WxFileCache::GetMimeTypeFromExtension(const wxString& Extension) const {
   if (Extension.CmpNoCase("gif") == 0) {
      return "image/gif";
   } else if (Extension.CmpNoCase("jpeg") == 0 || Extension.CmpNoCase("jpg") == 0
         || Extension.CmpNoCase("jpe") == 0) {
      return "image/jpeg";
   } else if (Extension.CmpNoCase("bmp") == 0) {
      return "image/bmp";
   } else if (Extension.CmpNoCase("gif") == 0) {
      return "image/gif";
   } else if (Extension.CmpNoCase("png") == 0) {
      return "image/png";
   } else if (Extension.CmpNoCase("tiff") == 0 || Extension.CmpNoCase("tif") == 0) {
      return "image/tiff";
   } else if (Extension.CmpNoCase("xpm") == 0) {
      return "image/x-xpixmap";
   } else if (Extension.CmpNoCase("ico") == 0) {
      return "image/x-icon";
   } else if (Extension.CmpNoCase("pcx") == 0) {
      return "image/x-pcx";
   } else if (Extension.CmpNoCase("pnm") == 0) {
      return "image/x-portable-anymap";
   } else if (Extension.CmpNoCase("iff") == 0) {
      return "image/iff";
   } else if (Extension.CmpNoCase("ani") == 0) {
      return "application/x-navi-animation";
   } else if (Extension.CmpNoCase("kml") == 0) {
      return "application/vnd.google-earth.kml+xml";
   } else if (Extension.CmpNoCase("kmz") == 0) {
      return "application/vnd.google-earth.kmz";
   } else if (Extension.CmpNoCase("xml") == 0) {
      return "application/xml";
   }
   return "";
}

/**
 * Determina si un mimetype es de imagen
 * @param[in] Mimetype: String con el mimetype
 * @return Devuelve verdadero si el mimetype es de imagen
 */
bool WxFileCache::IsImage(const wxString& Mimetype) const {
   return Mimetype.StartsWith("image/");
}

} /* namespace suri */

