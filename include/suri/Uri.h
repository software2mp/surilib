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

#ifndef URI_H_
#define URI_H_

// Includes standard
#include <string>

// Includes Suri

// Includes Wx

// Includes App

// Defines

namespace suri {
/**
 * Clase que representa una uri generica. Ver rfc2396 y rfc3986.
 * Codificacion: rfc3986 determina que todos los caracteres que no
 * se puedan representar en ANSII, se deben representar en UTF8
 * y escapar cada caracter con %.
 */
class Uri {
public:
   /** Ctor */
   explicit Uri(std::string Uri);
   /** Dtor */
   ~Uri();

   /** Retorna Uri configurada en el objeto */
   std::string ToString() const;
   /** Retorna esquema de uri configurado en el objeto */
   std::string GetScheme() const;
   /** Retorna autoridad de uri configurado en el objeto */
   std::string GetAuthority() const;
   /** Retorna path de uri configurado en el objeto */
   std::string GetPath() const;
   /** Retorna query de uri configurado en el objeto */
   std::string GetQuery() const;
   /** Retorna host del uri configurado en el objeto */
   std::string GetHost() const;
   /** Retorna puerto del uri configurado en el objeto */
   std::string GetPort() const;
   /** Indica si el uri es absoluto o no. */
   bool IsAbsolute() const;
   /** Indica si el uri tiene estructura reconocida por rfc2396 */
   bool IsOpaque() const;

   /** Genera un URI relativo que apunta al mismo recurso que la instancia */
   bool Relativize(const Uri &BaseUri, Uri &RelativeUri) const;
   /** Genera un URI absoluto usando el Uri de la instancia como base */
   bool Resolve(const std::string &RelativeUri, Uri &AbsoluteUri) const;

   //------------------------ METODOS PROPIOS DE SURI --------------------------
   /** Obtiene la direccion de la fuente de datos sin el query */
   bool GetDatasource(std::string &DataSource);

   static const std::string QUERY_SEPARATOR;
   static const std::string QUERY_STARTER;

private:
   std::string uri_; /*! String con el uri que recibio al construirse */
   std::string scheme_; /*! Esquema del uri */
   std::string authority_; /*! Autoridad del uri */
   std::string path_; /*! Path del uri */
   std::string query_; /*! Query del uri */
   std::string host_; /*! Host de uri */
   std::string port_; /*! Port del uri */
   bool isOpaque_; /*! Indica si el uri tiene estructura reconocida por rfc2396 */
   bool isAbsolute_; /*! Indica si el uri es absoluto o relativo */
};

}  // namespace

#endif /* URI_H_ */
