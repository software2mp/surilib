/*! \file XmlUrlManager.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef XMLURLMANAGER_H_
#define XMLURLMANAGER_H_

#include "XmlElementManager.h"

/** namspace suri */
namespace suri {
/** Realiza operaciones de XmlElementManager sobre todos los nodos url */
/**
 * Las operaciones analizan si es un nodo url, en caso afirmativo realizan
 * las operaciones indicadas.
 * Las funciones permiten modificar el formato del url y hacerlo relativo
 * o absoluto a un directorio especifico.
 * Tambien se puede mover el archivo a otro directorio, actualizando el
 * url correspondiente
 */
class XmlUrlManager : public XmlElementManager {
public:
   /** Ctor */
   XmlUrlManager();
   /** Dtor */
   virtual ~XmlUrlManager();

   /**
    * Si Path debajo de From loa cambia a To en Nodo y mueve archivos
    * si MoveFiles=true
    */
   virtual bool Move(wxXmlNode *pXmlNode, const std::string &From,
                     const std::string &To, bool MoveFiles = false);

   /** Cambia los URL para que esten debajo de To */
   virtual bool Move(wxXmlNode *pXmlNode, const std::string &To,
                     wxArrayString *NewPaths, wxArrayString *OldPaths);

   /** Verifica que los URL en el documento sean validos */
   virtual bool Check(wxXmlNode *pXmlNode, std::list<wxXmlNode*> &BadUrls);

   /** Hace absolutos a los URLs */
   virtual bool MakeAbsolute(wxXmlNode *pXmlNode, const std::string &Path);

   /** Hace relativos a los URLs */
   virtual bool MakeRelative(wxXmlNode *pXmlNode, const std::string &Directory,
                             bool OnlySubfolders = true);

   /** Cambia el separador del path al formato pedio */
   virtual bool ChangePathFormat(wxXmlNode *pXmlNode, PathFormatType PathFormat);

private:
   /** Inicializa el manager */
   virtual XmlUrlManager* InitializeClass();

   /** Analiza si el Uri corresponde a ruta de sistema de archivos */
   bool IsFileSystemPath(std::string Uri);

};
}  // namespace suri

#endif /* XMLURLMANAGER_H_ */
