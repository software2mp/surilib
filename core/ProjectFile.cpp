/*! \file ProjectFile.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/ProjectFile.h"

// Includes standard
#include <list>

// Includes Suri
#include "suri/xmlnames.h"
#include "suri/Configuration.h"
#include "suri/messages.h"
#include "suri/XmlElementManager.h"
#include "logmacros.h"

// Includes Wx
#include "wx/sstream.h"
#include "wx/ffile.h"
#include "wx/utils.h"

// Includes App

// Defines
/** Codificacion del XML en el archivo */
#define XML_DISK_FORMAT "UTF-8"
/** Document Type Template del XML */
#define DOCTYPE_LINE_PROJECT "<!DOCTYPE suriedu SYSTEM \"suri.dtd\">"

/** namespace suri */
namespace suri {
/**
 * Setea valores default a las propiedades que guardan informacion de formato
 * de proyecto.
 * @return instancia de la estructura ProjectFormatData.
 */
ProjectFile::ProjectFormatData::Info() {
   std::string appversion = suri::Configuration::GetParameter("app_version",
                                                              wxT(message_NA));
   type_ = wxT(TYPE_PROPERTY_VALUE_ABSOLUTE);
   application_ = appversion;
   version_ = wxT(message_NA);
   os_ = wxGetOsDescription().c_str();
}

ProjectFile::ProjectFile(const std::string &FullPath) :
      pXmlDocument_(NULL), pManagers_(NULL), pCurrentChild_(NULL) {
   OpenPath(FullPath);
}

/**
 * Luego de cargar el proyecto se modifica la codificacion con la del
 * sistema operativo. Esto configura el encabezado.
 * @param[in] pDocument xml del projecto con que tengo que inicializar Project.
 * Si es NULL se crea uno nuevo.
 */
ProjectFile::ProjectFile(const wxXmlDocument* pDocument) :
      pXmlDocument_(NULL), pManagers_(NULL), pCurrentChild_(NULL) {
   if (pDocument) {
      pXmlDocument_ = new wxXmlDocument(*pDocument);
      Encode();
   }
}

/** Dtor */
ProjectFile::~ProjectFile() {
   // Elimino los managers en la lista.
   std::list<XmlElementManager*>::iterator it;
   if (!pManagers_) {
      return;
   }
   for (it = pManagers_->begin(); it != pManagers_->end(); it++)
      XmlElementManager::Destroy(*it);
   delete pManagers_;
   delete pXmlDocument_;
}

/**
 * Retorno el documento.
 * @return la estructura interna,
 * \attention la estructura es responsabilidad de esta clase, no eliminar.
 */
const wxXmlDocument *ProjectFile::GetXmlDocument() {
   return pXmlDocument_;
}

/**
 * Mueve los url en el documento xml que se encuentran debajo de From a
 * archivos debajo de To respetando los directorios intermedios.
 * Todas las rutas dentro del xml deben ser absolutas
 * \post si MoveFiles=true tambien se movieron los archivos
 * @param[in] From directorio bajo el cual estan los urls a modificar
 * @param[in] To directorio donde se moveran los archivos
 * @param[in] MoveFiles flag que indica si se deben copiar los archivos
 */
void ProjectFile::Move(const std::string &From, const std::string &To, bool MoveFiles) {
   // Si el xml es invalido informo del error y salgo
   if (!IsOk()) {
      REPORT_DEBUG("D: El documento xml es invalido");
      return;
   }

   // Las rutas se mueven de From a To
   std::list<XmlElementManager*>::iterator it;
   StartTraverse();
   bool movedown = true;
   while (TraverseTree(movedown)) {
      movedown = true;
      it = GetManagers()->begin();
      while ((it != GetManagers()->end()) && movedown) {
         movedown = !((*it)->Move(pCurrentChild_, From, To, MoveFiles));
         it++;
      }
   }
}

/**
 * Mueve todos los url del documento al directorio solicitado. Todas
 * las rutas dentro del xml deben ser absolutas.
 * @param[in] To directorio donde se moveran los archivos
 * @param[out] NewPaths rutas generadas en move
 * @param[out] OldPaths rutas originales
 */
void ProjectFile::Move(const std::string &To, wxArrayString *NewPaths,
                       wxArrayString *OldPaths) {
   // Si el xml es invalido informo del error y salgo
   if (!IsOk()) {
      REPORT_DEBUG("D: El documento xml es invalido");
      return;
   }

   // Las rutas se mueven a To
   std::list<XmlElementManager*>::iterator it;
   StartTraverse();
   bool movedown = true;
   while (TraverseTree(movedown)) {
      movedown = true;
      it = GetManagers()->begin();
      while ((it != GetManagers()->end()) && movedown) {
         movedown = !((*it)->Move(pCurrentChild_, To, NewPaths, OldPaths));
         it++;
      }
   }
}

/**
 * Verifica que los URLs en el documentos sean validos. Todas
 * las rutas dentro del xml deben ser absolutas.
 * @param[in] BadUrls lista donde se guardan los url sin archivos
 * @return informa si hubo al menos un error en los urls
 */
bool ProjectFile::Check(std::list<wxXmlNode*> &BadUrls) {
   // Si el xml es invalido informo del error y salgo
   if (!IsOk()) {
      REPORT_DEBUG("D: El documento xml es invalido");
      return false;
   }

   // Las rutas se controlan
   std::list<XmlElementManager*>::iterator it;
   StartTraverse();
   bool movedown = true;
   while (TraverseTree(movedown)) {
      movedown = true;
      it = GetManagers()->begin();
      while ((it != GetManagers()->end()) && movedown) {
         movedown = !((*it)->Check(pCurrentChild_, BadUrls));
         it++;
      }
   }
   return (BadUrls.size() == 0);
}

/**
 * Guarda XML en disco en codificacion correcta y convirtiendo los URLs a
 * relativos.
 * @param[in] Directory directorio a partir del cual hacer relativos los
 * archivos. Si no se pasa directorio se hacen relativos los archivos
 * debajo del directorio en XmlPath
 * @param[in] XmlPath archivo de instalacion del xml
 * @return informa si pudo guardar el xml.
 */
bool ProjectFile::Write(const std::string &XmlPath, const std::string &Directory) {
   std::string directory = Directory;
   // Si el xml es invalido informo del error y salgo
   if (!IsOk()) {
      REPORT_DEBUG("D: El documento xml es invalido");
      return false;
   }

   // Si no se paso directorio uso el de XmlPath
   if (directory == "") {
      wxFileName fnpath(XmlPath);
      directory = fnpath.GetPathWithSep();
   }

   // Transformo paths
   ProjectFile tempprojectfile(pXmlDocument_);
   tempprojectfile.MakePathsRelative(directory);
   tempprojectfile.ChagePathFormat(PATH_UNIX);

   // Tranforma codificacion de XML
   tempprojectfile.Encode(wxT(XML_DISK_FORMAT));
   // Transforma a String
   std::string xmlasstring;
   tempprojectfile.ToString(xmlasstring);

   // Guarda a disco el XML
   wxFFile ffile(XmlPath.c_str(), "w");
   if (!ffile.IsOpened()) {
      REPORT_AND_FAIL_VALUE("D:Error al intentar grabar archivo: %s.", false,
                            XmlPath.c_str());
   }
   if (!ffile.Write(xmlasstring)) {
      REPORT_AND_FAIL_VALUE("D:Error al escribir archivo: %s.", false, XmlPath.c_str());
   }
   ffile.Flush();
   if (!ffile.Close()) {
      REPORT_AND_FAIL_VALUE("D:Error al cerrar archivo : %s.", false, XmlPath.c_str());
   }
   return true;
}

/**
 * Compara dos documentos xml, hasta ahora lu unico que hace es
 * comparar caracter a caracter los xml
 * @param[in] pXmlDocument documento xml a comparar
 * @return resultado de la comparacion
 */
bool ProjectFile::Compare(wxXmlDocument *pXmlDocument) {
   // Si el xml es invalido informo del error y salgo
   if (!IsOk()) {
      REPORT_DEBUG("D: El documento xml es invalido");
      return false;
   }

   // compara XMLs
   std::string xmlasstring1, xmlasstring2;
   ToString(xmlasstring1);
   ToString(xmlasstring2, pXmlDocument);
   return xmlasstring1 == xmlasstring2;
}

/**
 * Transforma wxXmlDocument a string y agrega DTD
 * @param[in] pXmlDocument documento xml que se debe convertir a string
 * @param[out] XmlAsString string con documento
 * @return bool que informa si tuvo exito
 */
bool ProjectFile::ToString(std::string &XmlAsString, wxXmlDocument *pXmlDocument) {
   // Si el xml es invalido informo del error y salgo
   if (!IsOk()) {
      REPORT_DEBUG("D: El documento xml es invalido");
      return false;
   }

   if (!pXmlDocument) {
      pXmlDocument = pXmlDocument_;
   }

   bool returnvalue;
   // Tranforma pXmlDocument a string
   wxStringOutputStream localxmloutput;
   returnvalue = pXmlDocument->Save(localxmloutput);
   XmlAsString = localxmloutput.GetString();

   // Agrega DTD
   std::string temp = XmlAsString;
   if (!AddDocType(temp, DOCTYPE_LINE_PROJECT)) {
      REPORT_AND_FAIL_VALUE("D:No se pudo agregar linea de DocType.", false);
   }
   XmlAsString = temp;
   return returnvalue;
}

/** Informa si el proyecto es valido */
bool ProjectFile::IsOk() {
   if ((!pXmlDocument_) || (!pXmlDocument_->IsOk())) {
      return false;
   }
   return true;
}

/**
 * Transforma string a wxXmlDocument en codificacion local
 * @param[in] XmlAsString string con documento
 * @param[out] pXmlDocument documento xml
 * @return string con documento
 */
bool ProjectFile::FromString(const std::string &XmlAsString,
                             wxXmlDocument *pXmlDocument) {
   wxString encode = wxLocale::GetSystemEncodingName();
   return pXmlDocument->Load(XmlAsString, encode);
}

/**
 * Elimina archivo/directorio de disco
 * @param[in] Path: directorio del archivo a eliminar
 * @return bool que informa si se tuvo exito
 */
bool ProjectFile::Delete(const std::string &Path) {
   wxFileName path(Path);
   return wxRmdir(path.GetPath());
}

/**
 * Obtiene una instancia de la estructura ProjectFormatData y la devuelve con
 * valores default o con los valores que se hayan seteado en las propiedades
 * del nodo proyecto
 * @param[in] Info estructura que guarda informacion de formato del proyecto.
 * @param[out] Info estructura que guarda informacion de formato del proyecto.
 * @return true en caso de poder obtener la estructura y llenarla con datos
 * @return false en caso de que no exista un documento en edicion o que no se
 * haya podido obtener el nodo proyecto
 */
bool ProjectFile::GetProjectFormatData(ProjectFormatData &Info) {
   Info = ProjectFormatData();

   if (!pXmlDocument_) {
      REPORT_DEBUG("D: No existe documento de proyecto en edicion.");
      return false;
   }

   wxXmlNode *prootnode = pXmlDocument_->GetRoot();
   if (!prootnode) {
      REPORT_DEBUG("D: no se pudo obtener nodo raiz del proyecto.");
      return false;
   }

   Info.type_ = prootnode->GetPropVal(TYPE_PROPERTY, Info.type_);
   Info.application_ = prootnode->GetPropVal(APPLICATION_PROPERTY, Info.application_);
   Info.version_ = prootnode->GetPropVal(VERSION_PROPERTY, Info.version_);
   Info.os_ = prootnode->GetPropVal(OS_PROPERTY, Info.os_);
   return true;
}

/**
 * Agrega al nodo proyecto las siguientes propiedades:
 * - tipo: (indica si los url de los archivos del proyecto son absolutos o relativos)
 * - aplicacion: version de la aplicacion.
 * - version: version del archivo de proyecto
 * - os: datos del sistema operativo sobre el cual se genero el proyecto.
 * @param[in] Info estructura con informacion de formato del proyecto.
 * @return true en caso de poder agregar la informacion detallada.
 * @return false en caso de no poder agregar la informacion detallada.
 */
bool ProjectFile::SetProjectFormatData(const ProjectFormatData &Info) {
   if (!pXmlDocument_) {
      REPORT_DEBUG("D: No existe documento de proyecto en edicion.");
      return false;
   }

   wxXmlNode *prootnode = pXmlDocument_->GetRoot();
   if (!prootnode) {
      REPORT_DEBUG("D: no se pudo obtener nodo raiz del proyecto.");
      return false;
   }

   prootnode->AddProperty(TYPE_PROPERTY, Info.type_);
   prootnode->AddProperty(APPLICATION_PROPERTY, Info.application_);
   prootnode->AddProperty(VERSION_PROPERTY, Info.version_);
   prootnode->AddProperty(OS_PROPERTY, Info.os_);
   return true;
}

/**
 * Devuelve el proximo nodo recorriendo en profundidad.
 * El primer nodo devuelto es el primer hijo del root, no el root.
 * @param[in] MoveDown si es falso no se intentara seguir por los hijos
 * del nodo actual y se pasara al hermano
 * @return retorna false si ya no quedan nodos por recorrer o ocurrio un
 * error.
 */
bool ProjectFile::TraverseTree(bool MoveDown) {
   // Si el xml es invalido informo del error y salgo
   if (!IsOk()) {
      REPORT_DEBUG("D: El documento xml es invalido");
      return false;
   }
   // Si no inicializaron el nodo actual fallo
   if (!pCurrentChild_) {
      REPORT_DEBUG("D: No se inicializo el nodo actual");
      return false;
   }

   wxXmlNode *pnode = NULL;
   if (MoveDown) {
      pnode = pCurrentChild_->GetChildren();
   }
   // Si no tiene hijos busco al siguiente hermano
   if (!pnode) {
      pnode = pCurrentChild_->GetNext();
   }
   // Si no tiene hermano busco al hermano del padre
   wxXmlNode *pparent = pCurrentChild_->GetParent();
   while ((pparent) && (!pnode)) {
      pnode = pparent->GetNext();
      if (!pnode) {
         pparent = pparent->GetParent();
      }
   }
   // Si es el ultimo nodo retorna false
   if (!pnode) {
      return false;
   }

   pCurrentChild_ = pnode;
   return true;
}

/**
 * Guarda el root como nodo actual
 */
void ProjectFile::StartTraverse() {
   // Si el xml es invalido informo del error y salgo
   if (!IsOk()) {
      REPORT_DEBUG("D: El documento xml es invalido");
      return;
   }

   pCurrentChild_ = pXmlDocument_->GetRoot();
}

/**
 * Transforma la codificacion del XML a la codificacion pedida.
 * Si no se pasa codificacion se utiliza la local.
 * \todo Ver si se pierde el DTD
 * @param[in] Encoding nueva codificacion del xml
 */
void ProjectFile::Encode(const std::string &Encoding) {
   wxString encoding(Encoding);
   // Si el xml es invalido informo del error y salgo
   if (!IsOk()) {
      REPORT_DEBUG("D: El documento xml es invalido");
      return;
   }

   wxStringOutputStream localxmloutput;
   pXmlDocument_->Save(localxmloutput);
   wxString localxmlstring = localxmloutput.GetString();
   wxStringInputStream localxmlinput(localxmlstring);

   if (encoding == wxEmptyString) {
      encoding = wxLocale::GetSystemEncodingName();
   }

   pXmlDocument_->Load(localxmlinput, encoding);
   pXmlDocument_->SetEncoding(encoding);
   pXmlDocument_->SetFileEncoding(encoding);
}

/**
 * Obtine las instancias de la clase XmlElementManager.
 * pManagers_ se limpia en destructor
 * @return lista de managers
 */
std::list<XmlElementManager*> *ProjectFile::GetManagers() {
   if (!pManagers_) {
      pManagers_ = new std::list<XmlElementManager*>;
      XmlElementManager* pmanager = XmlElementManager::Create("XmlUrlManager");
      if (pmanager) {
         pManagers_->push_back(pmanager);
      }
   }
   return pManagers_;
}

/**
 * Tranforma los paths al formato indicado.
 * @param[in] Format tipo de separador a usar
 */
void ProjectFile::ChagePathFormat(PathFormatType Format) {
   // Si el xml es invalido informo del error y salgo
   if (!IsOk()) {
      REPORT_DEBUG("D: El documento xml es invalido");
      return;
   }

   // Cambia separador en rutas al nativo
   std::list<XmlElementManager*>::iterator it;
   StartTraverse();
   bool movedown = true;
   while (TraverseTree(movedown)) {
      movedown = true;
      it = GetManagers()->begin();
      while ((it != GetManagers()->end()) && movedown) {
         movedown = !((*it)->ChangePathFormat(pCurrentChild_, Format));
         it++;
      }
   }
}

/**
 * tranforma rutas en absolutas.
 * @param[in] Directory transforma todos los paths relativos que encuentre
 * a absolutos a partir de este directorio.
 */
void ProjectFile::MakePathsAbsolute(const std::string &Directory) {
   // Si el xml es invalido informo del error y salgo
   if (!IsOk()) {
      REPORT_DEBUG("D: El documento xml es invalido");
      return;
   }

   // Las rutas se hacen absolutas
   std::list<XmlElementManager*>::iterator it;
   StartTraverse();
   bool movedown = true;
   while (TraverseTree(movedown)) {
      movedown = true;
      it = GetManagers()->begin();
      while ((it != GetManagers()->end()) && movedown) {
         movedown = !((*it)->MakeAbsolute(pCurrentChild_, Directory));
         it++;
      }
   }
}

/**
 * tranforma rutas en relativas
 * @param[in] Directory directorio bajo el cual las rutas se hacen
 * relativas. Se respetan las subcarpetas dentro de este directorio
 */
void ProjectFile::MakePathsRelative(const std::string &Directory) {
   // Si el xml es invalido informo del error y salgo
   if (!IsOk()) {
      REPORT_DEBUG("D: El documento xml es invalido");
      return;
   }

   // Se usa TraverseTree para cambiar URL a relativos debajo de Directory
   std::list<XmlElementManager*>::iterator it;
   StartTraverse();
   bool movedown = true;
   while (TraverseTree(movedown)) {
      movedown = true;
      it = GetManagers()->begin();
      while ((it != GetManagers()->end()) && movedown) {
         movedown = !((*it)->MakeRelative(pCurrentChild_, Directory));
         it++;
      }
   }
}

/**
 * Abre xml en ruta indicada cambiando el encoding al local. Tranforma las
 * rutas al formato nativo y los hace absolutos.
 * @param[in] Path ruta del xml a abrir
 */
void ProjectFile::OpenPath(const std::string &Path) {
   // Limpio lista managers
   if (pManagers_) {
      std::list<XmlElementManager*>::iterator it;
      for (it = pManagers_->begin(); it != pManagers_->end(); it++)
         XmlElementManager::Destroy(*it);
      delete pManagers_;
      pManagers_ = NULL;
   }

   // Abro el xml en formato local
   pXmlDocument_ = new wxXmlDocument(Path);
   // Si el xml es invalido informo del error y salgo
   if (!IsOk()) {
      REPORT_DEBUG("D: El documento xml es invalido");
      return;
   }

   Encode();

   // Calculo el Path de instalacion.
   wxFileName filename(Path);
   std::string path = filename.GetPathWithSep().c_str();

   // Primero hago URLs absolutos y luego los transformo a formato UNIX
   // Caso contrario MakePathsAbsolute los deja en formato local.
   MakePathsAbsolute(path);
   ChagePathFormat(PATH_UNIX);
}

/**
 * Objetivo: Agrega linea de DocType a un XML
 * \todo Si existe el doctype hace el reemplazo cuando se podria dejarlo, esto
 *      queda ya que puede servir para reemplzar el doctype por otro en lugar de
 *      si mismo como es ahora.
 * @param[in]	XmlString el string con el xml
 * @param[in]  DocType linea DockType
 * @return	Agrega Ok? true: false;
 */
bool ProjectFile::AddDocType(std::string &XmlString, const std::string &DocType) {
   wxString xmlstring(XmlString);
   // si ya trae el doctype, lo reemplazo (asumo que viene bien puesto)
   int start = xmlstring.Find(DocType.c_str());
   // si no encontro, no reemplaza, inserta
   int len = start == wxNOT_FOUND ? 0 : wxString(DocType.c_str()).length() + 1;
   // si no lo tiene, el punto de insercion es la segunda linea
   start = start == wxNOT_FOUND ? xmlstring.Find('\n') : start - 1;
   wxString predoctype = xmlstring.Mid(0, start + 1);
   wxString postdoctype = xmlstring.Mid(start + len);
   xmlstring = predoctype + DocType.c_str() + postdoctype;
   XmlString = xmlstring.c_str();
   return true;
}
}  // namespace suri
