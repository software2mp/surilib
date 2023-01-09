/*! \file Uri.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
