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

// Includes standard
// Includes Suri
#include "suri/Uri.h"

// Includes Wx

// Includes App

// Defines

namespace suri {

/** Variables estaticas */
const std::string Uri::QUERY_SEPARATOR = "&";
const std::string Uri::QUERY_STARTER = "?";

/**
 * Libkml tiene un parser de uris.
 * Por restriccion de tiempo se prefirio implementar un parser de funcionalidad
 * limitada.
 * Limitaciones:
 * - Asume que no hay paths opacos.
 * - No soporta direcciones IPv6
 * - Codificacion: No se modica la codificacion de los caracteres. Esto implica
 *   que los uris no son compatibles entre sistemas operativos.
 *
 * Opciones a futuro:
 * - Linkear libkml desde la aplicacion. Hice varios tests y funciona.
 * - libkml usa la biblioteca uriparser, la mejor solucion seria importar solo
 *   uriparser, pero esto implicaria agregar otra biblioteca al proyecto y
 *   decirle a libkml que use suriparser externo en vez de compilarlo.
 * - Otra opcion seria descargar codigo de uriparser y compilarlo con
 *   suri. (La licencia es la nueva BSD)
 *
 * Nota: El parser implementado retorna los mismos resultados que la biblioteca
 *       para que se pueda remplazar sin modificar el codigo que utiliza la
 *       interfaz.
 */
Uri::Uri(std::string Uri) {
   uri_ = Uri;

   std::string uri = Uri;
   size_t firstdots = uri.find(':');
   size_t firstbar = uri.find('/');
   size_t firstquestion = uri.find('?');

   isAbsolute_ = false;
   isOpaque_ = false;

   //Analizo si tiene esquema
   if (firstdots < firstbar && firstdots < firstquestion
         && firstdots != std::string::npos) {
      scheme_ = uri.substr(0, firstdots);
      uri = uri.substr(firstdots + 1);
      isAbsolute_ = true;
   }

   //Analizo si hay autority
   if (uri.size() > 1 && uri.substr(0, 2).compare("//") == 0) {
      uri = uri.substr(2);
      size_t endauthority = std::min(uri.find('/'), uri.find('?'));
      authority_ = uri.substr(0, endauthority);
      uri = uri.substr(endauthority);
      isAbsolute_ = true;
      //TODO : Separar autority en host y port
   }

   //Analizo si hay path
   size_t endpath = uri.find('?');
   if (uri.size() > 0 && endpath != 0) {
      size_t start = (uri.substr(0, 1).compare("/") == 0) ? 1 : 0;
      path_ = uri.substr(start, endpath - start);
      if (endpath != std::string::npos)
         uri = uri.substr(endpath);
      else
         uri = "";
      if (uri.substr(0, 1).compare("/") == 0) isAbsolute_ = true;
   }

   //El resto es un query.
   if (uri.size() > 0 && uri.substr(0, 1).compare("?") == 0) query_ = uri.substr(1);
}

/** Dtor */
Uri::~Uri() {
}

/**
 * Concatena los elementos de la clase en string respetando formato de rfc2396
 * @return std::string con datos en Uri
 */
std::string Uri::ToString() const {
   return uri_;
}

/**
 * Retorna el scheme del uri.
 * @return std::string con esquema
 */
std::string Uri::GetScheme() const {
   return scheme_;
}

/**
 * Retorna la autoridad del uri
 * @return std::string con autoridad
 */
std::string Uri::GetAuthority() const {
   return authority_;
}

/**
 * Retorna el path del uri
 * @return std::string con path
 */
std::string Uri::GetPath() const {
   return path_;
}

/**
 * Retorna el query del uri
 * @return std::string con query
 */
std::string Uri::GetQuery() const {
   return query_;
}

/**
 * Retorna el host del uri
 * @return std::string con host
 */
std::string Uri::GetHost() const {
   return host_;
}

/**
 * Retorna el puerto del uri
 * @return std::string con puerto
 */
std::string Uri::GetPort() const {
   return port_;
}

/**
 * Indica si el uri es absoluto o no.
 * @return bool que indica si es absoluto
 */
bool Uri::IsAbsolute() const {
   return isAbsolute_;
}

/**
 * Indica si el uri sigue la convencion de rfc3986 o tiene
 * estructura propia no compatible. Por ejemplo usando caracteres reservados
 * de forma incorrecta.
 */
bool Uri::IsOpaque() const {
   return isOpaque_;
}

/**
 * Genera un URI relativo que apunta al mismo recurso que la instancia
 * invocada pero usando otro uri como base.
 * Este metodo solo se puede llamar cuando el uri de la instancia no
 * es relativo.
 * @param[in] BaseUri string de uri que quiero usar como base
 * @param[out] RelativeUri uri relativa a BaseUri
 * @return Uri creado que apunta al mismo recurso
 */
bool Uri::Relativize(const Uri &BaseUri, Uri &RelativeUri) const {
   if (isOpaque_) return false;

   std::string basepath = BaseUri.GetPath();

   if (BaseUri.GetScheme().compare(scheme_) != 0
         || BaseUri.GetAuthority().compare(authority_) != 0) return false;

   if (path_.size() > 0 && path_.substr(0, basepath.size()).compare(basepath) != 0)
      return false;

   std::string newpath = path_.substr(basepath.size());
   if (!query_.empty()) newpath = newpath + "?" + query_;

   if (newpath.size() > 0 && newpath.substr(0, 1).compare("/") == 0)
      newpath = newpath.substr(1);

   RelativeUri = Uri(newpath);

   return true;
}

/**
 * Genera un URI absoluto usando el Uri actual como base y RelativeUri
 * como parte relativa.
 * @param[in] RelativeUri string de uri relativo
 * @param[out] AbsoluteUri uri absoluta a RelativeUri
 * @return Uri creado
 */
bool Uri::Resolve(const std::string &RelativeUri, Uri &AbsoluteUri) const {
   if (isOpaque_) return false;

   // NOTA: Ver que dice rfc3986 sobre bases con query!=""
   if (!query_.empty()) return false;

   std::string absolutepath = "";
   if (!scheme_.empty()) absolutepath = absolutepath + scheme_ + ":";

   if (!authority_.empty()) absolutepath = absolutepath + "//" + authority_;

   if (!path_.empty()) absolutepath = absolutepath + "/" + path_;

   if (absolutepath.empty()
         || absolutepath.substr(absolutepath.size() - 1, 1).compare("/") != 0)
      absolutepath = absolutepath + "/";

   absolutepath = absolutepath + RelativeUri;

   AbsoluteUri = Uri(absolutepath);

   return true;
}

/**
 * Obtiene la direccion de la fuente de datos sin el query
 * @param[out] Datasource formado por schema://authority/path. Uno
 * o mas elementos del uri pueden faltar.
 * @return bool que indica si pudo generar Datasource
 */
bool Uri::GetDatasource(std::string &DataSource) {
   if (isOpaque_) return false;

   std::string datasource = "";

   if (!scheme_.empty()) datasource = scheme_ + ":";

   if (!authority_.empty()) datasource = datasource + "//" + authority_;

   if (!path_.empty()) datasource = datasource + "/" + path_;

   DataSource = datasource;
   return true;
}

}  //namespace

