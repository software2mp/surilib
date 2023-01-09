/*! \file HtmlDatasource.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <string>
#include <vector>

#include "GdalImage.h"
#include "HtmlDatasource.h"
#include "HtmlElement.h"
#include "suri/Element.h"
#include "suri/Image.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/DataTypes.h"
#include "suri/LayerInterface.h"
#include "suri/Vector.h"

#include "wxmacros.h"

#include "wx/file.h"
#include "wx/filename.h"

namespace suri {

/**
 * Macro para registrar el tipo de elemento
 */
AUTO_REGISTER_CLASS(DatasourceInterface, HtmlDatasource, 0)

/**
 * Constructor
 */
HtmlDatasource::HtmlDatasource() :
      DatasourceInterface(wxT(TYPE_PROPERTY_VALUE_HTML)),
      pElement_(NULL) {
}

/**
 * Destructor
 */
HtmlDatasource::~HtmlDatasource() {
}

/**
 * Create a partir del nombre de archivo
 */
HtmlDatasource *HtmlDatasource::Create(const std::string &FileName) {
   HtmlDatasource *phtml = new HtmlDatasource;
   if (phtml->Initialize(FileName) != phtml) {
      delete phtml;
      return NULL;
   }
   return phtml;
}

/**
 * Initialize a partir del nombre de archivo
 */
HtmlDatasource* HtmlDatasource::Initialize(const std::string &FileName) {
   Element *pelement = suri::Element::Create("HtmlElement", FileName.c_str());
   if (pelement != NULL) {
      pElement_ = pelement;
      return this;
   }
   return NULL;
}

/**
 * Create a partir del nombre de archivo
 */
HtmlDatasource *HtmlDatasource::Create(const wxXmlNode* pRootNode) {
   HtmlDatasource *phtml = new HtmlDatasource;
   if (phtml->Initialize(pRootNode) != phtml) {
      delete phtml;
      return NULL;
   }
   return phtml;
}

/**
 * Initialize a partir del nodo xml
 */
HtmlDatasource* HtmlDatasource::Initialize(const wxXmlNode* pRootNode) {
   Element *pelement = suri::Element::Create("HtmlElement", pRootNode);
   if (pelement != NULL) {
      pElement_ = pelement;
      return this;
   }
   return NULL;
}

/**
 * Create a partir del elemento
 * \deprecated
 */
HtmlDatasource *HtmlDatasource::Create(Element *pElement) {
   HtmlDatasource *phtml = new HtmlDatasource;
   if (phtml->Initialize(pElement) != phtml) {
      delete phtml;
      return NULL;
   }
   return phtml;
}

/** Funcion estatica para crear un DatasourceInterface */
HtmlDatasource* HtmlDatasource::Create(DatasourceInterface *pDatasource) {
   if (!pDatasource)
      return NULL;
   HtmlDatasource *phtml = new HtmlDatasource;
   if (phtml->Initialize(pDatasource) != phtml) {
      delete phtml;
      return NULL;
   }
   return phtml;
}

/**
 * Initialize a partir del elemento
 * \deprecated
 */
HtmlDatasource* HtmlDatasource::Initialize(Element *pElement) {
   if (dynamic_cast<HtmlElement*>(pElement) == NULL) {
      return NULL;
   }
   pElement_ = pElement;
   if (!pElement->GetName().IsEmpty())
      SetName(pElement->GetName().c_str());
   else
      SetName(pElement->GetUrl().c_str());
   return this;
}

/** Inicializa los atributos de Datasource a partir de un DatasourceInterface */
HtmlDatasource* HtmlDatasource::Initialize(DatasourceInterface *pDatasource) {
   if (!pDatasource)
      return NULL;
   return Initialize(pDatasource->GetElement()->GetNode());
}

/**
 * Obtiene el elemento
 * \deprecated
 */
Element* HtmlDatasource::GetElement() {
   return pElement_;
}

/**
 * Compara dos tipos diferentes de Datasource
 */
bool HtmlDatasource::Equals(DatasourceInterface &Datasource) {
   if (GetElement() && Datasource.GetElement())
      return (Datasource.GetElement()->GetUid() == GetElement()->GetUid());
   else
      return false;
}

/**
 * Compara un Datasource contra un Layer
 */
bool HtmlDatasource::Equals(LayerInterface &Layer) {
   return (Layer.GetElement()->GetUid() == GetElement()->GetUid());
}

/**
 * Compara un Datasource contra un Element
 * \deprecated
 */
bool HtmlDatasource::Equals(Element &Element) {
   return (Element.GetUid() == GetElement()->GetUid());
}

/**
 * Devuelve la interpretacion de la fuente de datos como una imagen
 */
Image* HtmlDatasource::GetDatasourceAsImage() {
   // TODO(Sebastian - TCK #852): Implementar
   return NULL;
}

/**
 * Devuelve la interpretacion de la fuente de datos como un vector
 */
Vector* HtmlDatasource::GetDatasourceAsVector() {
   // TODO(Sebastian - TCK #852): Implementar
   return NULL;
}

/**
 * Setea el nombre de la fuente de datos
 *  @param[in] Name string con el nombre a asignar
 *  @return Devuelve true si tuvo exito, false en caso contrario
 **/
bool HtmlDatasource::SetName(const std::string &Name) {
   if (pElement_ == NULL)
      return false;

   if (wxFile::Exists(Name.c_str())) {
      wxFileName filename(Name);
      pElement_->SetName(filename.GetName());
   } else {
      pElement_->SetName(Name);
   }

   return true;
}

/**
 * Obtiene el nombre de la fuente de datos
 *  @return String con el nombre del Elemento asociado, string vacio en caso contrario
 **/
std::string HtmlDatasource::GetName() const {
   if (pElement_ == NULL)
      return "";
   return pElement_->GetName().c_str();
}

/**
 * Cambia el nombre de la fuente de datos
 */
bool HtmlDatasource::SetCopyRight(const std::string &CopyRight) {
   if (pElement_ == NULL)
      return false;
   wxString copyright(CopyRight);
   pElement_->SetCopyRight(copyright);
   return true;
}

/**
 * Cambia la descripcion de una fuente de datos
 */
bool HtmlDatasource::SetDescription(const std::string &Description) {
   if (pElement_ == NULL)
      return false;
   wxString description(Description);
   pElement_->SetDescription(description);
   return true;
}

/**
 * Cambia el url de la fuente de datos
 */
bool HtmlDatasource::SetUrl(const std::string &Url) {
   if (pElement_ == NULL)
      return false;
   wxString url(Url);
   pElement_->SetUrl(url);
   return true;
}

/**
 * Cambia el icono asociado a la fuente de datos
 * \deprecated
 */
bool HtmlDatasource::SetIcon(const std::string &Icon) {
   if (pElement_ == NULL)
      return false;
   wxString icon(Icon);
   pElement_->SetIcon(icon);
   return true;
}

/**
 * Cambia el nombre de la fuente de datos
 */
const std::string HtmlDatasource::GetCopyRight() const {
   std::string copyright;
   if (pElement_ == NULL)
      return copyright;
   wxString wxcopyright = pElement_->GetCopyRight();
   copyright = WS2S(wxcopyright);
   return copyright;
}

/**
 * Cambia la descripcion de una fuente de datos
 */
const std::string HtmlDatasource::GetDescription() const {
   std::string description;
   if (pElement_ == NULL) return description;
   wxString wxdescription = pElement_->GetDescription();
   description = WS2S(wxdescription);
   return description;
}

/**
 * Cambia el url de la fuente de datos
 */
const std::string HtmlDatasource::GetUrl() const {
   std::string url;
   if (pElement_ == NULL) return url;
   wxString wxurl = pElement_->GetUrl();
   url = WS2S(wxurl);
   return url;
}

/**
 * Cambia el icono asociado a la fuente de datos
 * \deprecated
 */
const std::string HtmlDatasource::GetIcon() const {
   std::string icon;
   if (pElement_ == NULL) return icon;
   wxString wxicon = pElement_->GetIcon();
   icon = WS2S(wxicon);
   return icon;
}

/** Devuelve una instancia del validador asociado a la fuente de datos**/
DatasourceValidatorInterface* HtmlDatasource::GetAssociatedValidator() const {
   return NULL;
}

/** Obtiene el sistema de referencia que posee la fuente de datos*/
std::string HtmlDatasource::GetSpatialReference() const {
   std::string spatialreference;
   if (pElement_)
      spatialreference = pElement_->GetSpatialReference().c_str();
   return spatialreference;
}

/** Devuelve el siguiente Datasource si es que al crearse se detecta multiplicidad */
DatasourceInterface* HtmlDatasource::GetNextDatasource() {
   return NULL;
}

/** Setea el siguiente Datasource si es que al crearse se detecta multiplicidad */
bool HtmlDatasource::SetNextDatasource(DatasourceInterface* pDatasource) {
   return false;
}

/** Devuelve true si un elemento esta listo para ser utilizado **/
bool HtmlDatasource::IsValid() const {
   return true;
}

} /** namespace suri */
