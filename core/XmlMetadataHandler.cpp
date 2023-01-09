/*! \file XmlMetadataHandler.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "XmlMetadataHandler.h"
#include "suri/xmlnames.h"

// Includes Wx
#include "wx/file.h"
#include "wx/filename.h"
#include "wx/xml/xml.h"

// Defines
// forwards

namespace suri {

/**
 * Constructor
 * @param[in] Xml del elemento
 * @param[in] OutputFilename path donde debe guardarse el xml
 */
XmlMetadataHandler::XmlMetadataHandler() {
}

/** Desstructor */
XmlMetadataHandler::~XmlMetadataHandler() {
}

/**
 * Realiza la lectura de un archivo xml de metadatos
 * @param[in] pDatasource fuente de datos que posee el elemento donde
 * @return true en caso de que exista el archivo con metadatos y pueda abrirlo, caso contrario false
 */
bool XmlMetadataHandler::ImportMetadata(DatasourceInterface* pDatasource) {
   wxFileName xmlinput(wxT(pDatasource->GetUrl()));
   xmlinput.SetExt(wxT(extension_XML));
   wxXmlDocument doc;
   if (!xmlinput.FileExists() || !doc.Load(xmlinput.GetFullPath()))
       return false;
   wxXmlNode* pchild = doc.GetRoot()->GetChildren();
   wxXmlNode* pparent = pDatasource->GetElement()->GetNode();
   while (pchild != NULL) {
      if (pchild->GetName() != URL_NODE || pchild->GetName() != DEPENDENCY_NODE
            || pchild->GetName() != NAME_NODE)
         pDatasource->GetElement()->AddNode(pparent, new wxXmlNode(*pchild), true);
      pchild = pchild->GetNext();
   }
   return true;
}

/**
 * Realiza la escritura de los metadatos en formato xml
 * @param[in] pDatasource fuente de datos que posee el elemento con los metadatos a exportar
 * @return true en caso de que pueda abrir y escribir el archivo, caso contrario false
 */
bool XmlMetadataHandler::ExportMetadata(DatasourceInterface* pDatasource, std::string Filename) {
   wxFile wxfile;
   wxFileName xmloutput(wxT(Filename));
   xmloutput.SetExt(wxT(extension_XML));
   wxString path = xmloutput.GetFullPath();
   if (!wxfile.Open(path, wxFile::write))
      return false;
   return wxfile.Write(pDatasource->GetElement()->GetXmlAsString());
}
} /** namespace suri */
