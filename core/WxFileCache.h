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

#ifndef WXFILECACHE_H_
#define WXFILECACHE_H_

// Includes Estandar
#include <map>
#include <iostream>
#include <cstring>
#include <string>

// Includes Suri

// Includes Wx
#include "wx/wx.h"
#include "wx/sstream.h"

// Defines

// forwards

namespace suri {

/** Clase Cache de Archivos */
/**
 *  Para no cargar multiples veces un archivo en memoria (particularmente de imagenes)
 *  se usa esta clase cache cuya responsabilidad es administrar los archivos en memoria.
 *  Esta clase se disenio como un Singleton para poder acceder a una instancia unica
 *  desde cualquier parte del codigo.
 *  \Note: No es multithreading safe
 */
class WxFileCache {
private:
   /** Constructor */
   WxFileCache();
   /** Constructor de copia */
   WxFileCache(const WxFileCache&);
public:
   /** Destructor */
   virtual ~WxFileCache();
   /** Instancia del Singleton */
   static WxFileCache* Instance();
   /** Agrega un archivo al cache y a la memoria */
   virtual wxString AddFile(const std::string& FileUrl);
   /** Reescala un archivo de imagen y lo agrega al cache y a la memoria */
   virtual wxString AddImageFile(const std::string& FileUrl, const size_t Width,
                                 const size_t Height);
   /** Obtiene un archivo del cache (que ya esta en memoria) */
   virtual wxString GetFile(const std::string& FileUrl);
   /** Quita un archivo del cache y de la memoria */
   virtual bool RemoveFile(const std::string& FileUrl);
   /** Cuenta la cantidad de archivos almacenados en el Cache */
   virtual size_t CountFiles() const;
   /** Determina si el Cache esta vacio */
   virtual bool IsEmpty() const;
private:
   /** Puntero a la Instancia del Singleton */
   static WxFileCache* pInstance_;
   /** Typedef para el mapa */
   typedef std::map<std::string, wxString> FileCacheHashmapType;
   /** Typedef para el par (del mapa) */
   typedef std::pair<std::string, wxString> FileCachePairType;
   /** Typedef para el par resultado de std::map.insert() */
   typedef std::pair<FileCacheHashmapType::iterator, bool> MapAddResultPairType;
   /** Mapa para esta implementacion de WxFileCache */
   FileCacheHashmapType hashmap_;
   /** Crea un archivo en memoria con la ayuda de la libreria wxwidgets */
   virtual wxString CreateMemoryFile(const std::string& FileUrl) const;
   /** Crea un archivo de imagen en memoria con la ayuda de la libreria wxwidgets */
   virtual wxString CreateMemoryImageFile(const std::string& FileUrl,
                                          const size_t Width,
                                          const size_t Height) const;
   /** Elimina un archivo en memoria con la ayuda de la libreria wxwidgets */
   virtual void DeleteMemoryFile(const wxString& FileUrl) const;
   /** Devuelve el mimetype de acuerdo a una lista limitada de extensiones */
   virtual wxString GetMimeTypeFromExtension(const wxString& Extension) const;
   /** Determina si un mimetype es de imagen */
   virtual bool IsImage(const wxString& Mimetype) const;
};

} /* namespace suri */

#endif /* WXFILECACHE_H_ */

