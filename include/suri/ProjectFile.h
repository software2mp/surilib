/*! \file ProjectFile.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#ifndef PROJECTFILE_H_
#define PROJECTFILE_H_

// Includes standard
#include <string>
#include <list>
#include <map>

// Includes Suri
#include "XmlElementManager.h"


// Includes Wx
#include "wx/string.h"
#include "wx/xml/xml.h"
#include "wx/filename.h"

// Includes App

// Defines

/** namespace suri */
namespace suri {

/** Administra el xml del projecto y sus archivos */
/**
 * Permite operar con los archivos del proyecto, obtener el Xml en formato
 * del SO y tranforma rutas a absolutas.
 */
class ProjectFile {
public:

   /** estructura que guarda informacion de formato del proyecto. */
   typedef struct Info {
      /** Ctor. */
      Info();

      std::string type_; /*! indica si los url son absolutos o relativos. */
      std::string application_; /*! version de la aplicacion. */
      std::string version_; /*! version del xml de proyecto. */
      std::string os_; /*! info sobre sistema operativo */
   } ProjectFormatData;

   /** Ctor */
   ProjectFile(const std::string &FullPath);
   /** Ctor */
   ProjectFile(const wxXmlDocument* Document = NULL);
   /** Dtor */
   virtual ~ProjectFile();
   /** Retorna wxXmlDocument de xml en Path, rutas a absolutas, codif a local */
   const wxXmlDocument *GetXmlDocument();
   /** Mueve las rutas debajo de From a To. Copia archivos si MoveFiles=true */
   void Move(const std::string &From, const std::string &To, bool MoveFiles = false);
   /** Mueve todas las rutas a To. Confia en XmlElementManager:ResolveColision */
   void Move(const std::string &To, wxArrayString *NewPaths, wxArrayString *OldPaths);
   /** Verifica que todas las rutas en xml sean validas */
   bool Check(std::list<wxXmlNode*> &BadUrls);
   /** Guarda Xml en destino, hace paths relativos, codificacion a UTF8 */
   bool Write(const std::string &XmlPath, const std::string &Directory = "");
   /** Compara dos xml para verificar si son iguales */
   bool Compare(wxXmlDocument *pXmlDocument);
   /** Tranforma wxXmlDocument a string, agrega DTD cuando falta */
   bool ToString(std::string &XmlAsString, wxXmlDocument *pXmlDocument = NULL);
   /** Informa si el proyecto es valido */
   bool IsOk();
   /** Tranforma xml en string a wxXmlDocument */
   static bool FromString(const std::string &XmlAsString, wxXmlDocument *pXmlDocument);
   /** Elimina un proyecto en Path indicado */
   static bool Delete(const std::string &Path);
   /** Devuelve informacion de formato del proyecto. */
   bool GetProjectFormatData(ProjectFormatData &Info);
   /** Agrega informacion de formato de proyecto */
   bool SetProjectFormatData(const ProjectFormatData &Info);
protected:
   /** Guarda el root como elemento actual en recorrido */
   void StartTraverse();
   /** Busca el siguiente nodo en arbol de xml */
   bool TraverseTree(bool MoveDown);
   /** Transforma la codificacion del xml al formato solicitado. */
   void Encode(const std::string &Encoding = "");
   /** Crea/Obtiene una lista de Managers a usar en nodos. */
   virtual std::list<XmlElementManager*> *GetManagers();
   /** Tranforma los paths al formato indicado */
   void ChagePathFormat(PathFormatType Format);
   /** tranforma rutas en absolutas */
   void MakePathsAbsolute(const std::string &Directory);
   /** tranforma rutas en relativas */
   void MakePathsRelative(const std::string &Directory);
   /** Abre xml en ruta */
   void OpenPath(const std::string &Path);
   /** Agrega linea de DocType a un XML */
   bool AddDocType(std::string &XmlString, const std::string &DocType);

   wxXmlDocument *pXmlDocument_; /*! xmldocument que se esta editando */
   std::list<XmlElementManager*> *pManagers_; /*! Managers que se van a */
   /* utilizar en nodos */
private:
   wxXmlNode *pCurrentChild_; /*! Nodo actual en recorrido con Traverse */
};
}  // namespace suri

#endif /* PROJECTFILE_H_ */
