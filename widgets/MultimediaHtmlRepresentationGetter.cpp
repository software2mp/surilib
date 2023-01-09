/*! \file MultimediaHtmlRepresentationGetter.cpp */
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
*/ 

// Includes Suri
#include "suri/MultimediaHtmlRepresentationGetter.h"
#include "suri/Element.h"
#include "MultimediaElement.h"
#include "suri/Configuration.h"

namespace suri {

/** ctor default **/
MultimediaHtmlRepresentationGetter::MultimediaHtmlRepresentationGetter() {
}
/** dtor */
MultimediaHtmlRepresentationGetter::~MultimediaHtmlRepresentationGetter() {
}

/**
 * Objetivo: Verifica que exista el directorio donde se encuentran los templates
 * html para visualizar la informacion del PAT.
 * En caso de que el directorio no exista lo crea y extrae los templates.
 * @param[in]	Path: path del directorio a verificar.
 * @return si existe o si puede crearlo retorna true; otro caso false.
 */
bool MultimediaHtmlRepresentationGetter::CheckDataDirectory(const wxString &Path) const {
   wxFileName fn(Path);

   if (!fn.Normalize())
   REPORT_AND_FAIL_VALUE("D:Falla normalizacion", false);
   if (!fn.IsDir())
      REPORT_AND_FAIL_VALUE("D:%s no es un directorio valido", false,
                            fn.GetFullPath().c_str());
   if (!fn.DirExists())
   //Trata de crear el directorio. Si ya existe no hace nada.
      if (!fn.Mkdir(fn.GetFullPath().c_str(), 0755, wxPATH_MKDIR_FULL))
      REPORT_AND_FAIL_VALUE("D:Error al intentar crear directorio de templates", false);
   //Verifico que exista el template html.
   if (!wxFileName::FileExists(Path + "imagen.html")) {
      SHOW_ERROR(message_HTML_FILES_NOT_FOUND);
      return false;
   }
   return true;
}
/** Metodo polimorfico encargado de obtener la representacion html de la capa que se
 *  encuentra por parametro.
 * @param[in] pElement elemento sobre el que se obtendra su representacion html
 * @param[out] Representation representacion en formato html del elemento
 * @return true en caso de poder obtener correctamente la interpretacion html del elemento
 * @return false en caso contrario 
**/
bool MultimediaHtmlRepresentationGetter::DoGetRepresentation(const Element* pElement,
                                       std::string& Representation) const {
   if (!dynamic_cast<const MultimediaElement*>(pElement))
      return false;
   wxString html = wxT("");
   std::string htmlphotopath = suri::Configuration::GetParameter(wxT("app_data_dir"),
                                                                 wxT("./")) + "fotografia/";
   const MultimediaElement* pphoto = dynamic_cast<const MultimediaElement*>(pElement);
   if (!CheckDataDirectory(htmlphotopath) || !pphoto->GetHtmlInfo(html))  
      return false;
   Representation = html.c_str();
   return true;
}

}  /** namespace suri */
