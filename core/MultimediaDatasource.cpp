/*! \file MultimediaDatasource.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "MultimediaDatasource.h"
#include "MultimediaElement.h"
#include "suri/Element.h"
#include "suri/Image.h"
#include "suri/Vector.h"
#include "suri/LayerInterface.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/DataTypes.h"
#include "GdalImage.h"
#include "wxmacros.h"

// Includes Wx
#include "wx/filename.h"

// Defines

// forwards

namespace suri {

/**
 * Macro para registrar el tipo de elemento
 */
AUTO_REGISTER_CLASS(DatasourceInterface, MultimediaDatasource, 0)

/**
 * Constructor
 */
MultimediaDatasource::MultimediaDatasource() :
      DatasourceInterface(wxT(NAME_PROPERTY_VALUE_PHOTOGRAPHS)),
      pElement_(NULL) {
}

/**
 * Destructor
 */
MultimediaDatasource::~MultimediaDatasource() {
}

/**
 * Create a partir del nombre de archivo
 */
MultimediaDatasource *MultimediaDatasource::Create(const std::string &FileName) {
   MultimediaDatasource *pphoto = new MultimediaDatasource;
   if (pphoto->Initialize(FileName) != pphoto) {
      delete pphoto;
      return NULL;
   }
   return pphoto;
}

/**
 * Initialize a partir del nombre de archivo
 */
MultimediaDatasource* MultimediaDatasource::Initialize(const std::string &FileName) {
   Element *pelement = suri::Element::Create("MultimediaElement", FileName.c_str());
   if (pelement != NULL) {
	   pElement_ = pelement;
	   return this;
   }
   return NULL;
}

/**
 * Create a partir del nombre de archivo
 */
MultimediaDatasource *MultimediaDatasource::Create(const wxXmlNode* pRootNode) {
   MultimediaDatasource *pphoto = new MultimediaDatasource;
   if (pphoto->Initialize(pRootNode) != pphoto) {
      delete pphoto;
      return NULL;
   }
   return pphoto;
}

/**
 * Initialize a partir del nodo xml
 */
MultimediaDatasource* MultimediaDatasource::Initialize(const wxXmlNode* pRootNode) {
	Element *pelement = suri::Element::Create("MultimediaElement", pRootNode);
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
MultimediaDatasource *MultimediaDatasource::Create(Element *pElement) {
   MultimediaDatasource *pphoto = new MultimediaDatasource;
   if (pphoto->Initialize(pElement) != pphoto) {
      delete pphoto;
      return NULL;
   }
   return pphoto;
}

/** Funcion estatica para crear un DatasourceInterface */
MultimediaDatasource* MultimediaDatasource::Create(DatasourceInterface *pDatasource) {
   if (!pDatasource)
      return NULL;
   MultimediaDatasource *pphoto = new MultimediaDatasource;
   if (pphoto->Initialize(pDatasource) != pphoto) {
      delete pphoto;
      return NULL;
   }
   return pphoto;
}

/**
 * Initialize a partir del elemento
 * \deprecated
 */
MultimediaDatasource* MultimediaDatasource::Initialize(Element *pElement) {
	if (dynamic_cast<MultimediaElement*>(pElement) == NULL) {
		return NULL;
	}
	pElement_ = pElement;
	return this;

}

/** Inicializa los atributos de Datasource a partir de un DatasourceInterface */
MultimediaDatasource* MultimediaDatasource::Initialize(DatasourceInterface *pDatasource) {
   if (!pDatasource)
      return NULL;
   return Initialize(pDatasource->GetElement()->GetNode());
}

/**
 * Obtiene el elemento
 * \deprecated
 */
Element* MultimediaDatasource::GetElement() {
   return pElement_;
}

/**
 * Compara dos tipos diferentes de Datasource
 */
bool MultimediaDatasource::Equals(DatasourceInterface &Datasource) {
   if (GetElement() && Datasource.GetElement())
      return (Datasource.GetElement()->GetUid() == GetElement()->GetUid());
   else
      return false;
}

/**
 * Compara un Datasource contra un Layer
 */
bool MultimediaDatasource::Equals(LayerInterface &Layer) {
   return (Layer.GetElement()->GetUid() == GetElement()->GetUid());
}

/**
 * Compara un Datasource contra un Element
 * \deprecated
 */
bool MultimediaDatasource::Equals(Element &Element) {
   return (Element.GetUid() == GetElement()->GetUid());
}

/**
 * Devuelve la interpretacion de la fuente de datos como una imagen
 */
Image* MultimediaDatasource::GetDatasourceAsImage() {
   // TODO(Sebastian - TCK #852): Implementar
   return NULL;
}

/**
 * Devuelve la interpretacion de la fuente de datos como un vector
 */
Vector* MultimediaDatasource::GetDatasourceAsVector() {
   // TODO(Sebastian - TCK #852): Implementar
   return NULL;
}

/**
 * Setea el nombre de la fuente de datos
 *  @param[in] Name string con el nombre a asignar
 *  @return Devuelve true si tuvo exito, false en caso contrario
 **/
bool MultimediaDatasource::SetName(const std::string &Name) {
   if (pElement_ == NULL)
      return false;
   wxFileName filename(Name);
   pElement_->SetName(filename.GetName());
   return true;
}

/**
 * Obtiene el nombre de la fuente de datos
 *  @return String con el nombre del Elemento asociado, string vacio en caso contrario
 **/
std::string MultimediaDatasource::GetName() const {
   if (pElement_ == NULL)
      return "";
   return pElement_->GetName().c_str();
}

/**
 * Cambia el nombre de la fuente de datos
 */
bool MultimediaDatasource::SetCopyRight(const std::string &CopyRight) {
   if (pElement_ == NULL)
      return false;
   wxString copyright(CopyRight);
   pElement_->SetCopyRight(copyright);
   return true;
}

/**
 * Cambia la descripcion de una fuente de datos
 */
bool MultimediaDatasource::SetDescription(const std::string &Description) {
   if (pElement_ == NULL)
      return false;
   wxString description(Description);
   pElement_->SetDescription(description);
   return true;
}

/**
 * Cambia el url de la fuente de datos
 */
bool MultimediaDatasource::SetUrl(const std::string &Url) {
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
bool MultimediaDatasource::SetIcon(const std::string &Icon) {
   if (pElement_ == NULL)
      return false;
   wxString lasticon = pElement_->GetIcon();
   wxString icon(Icon);
   if (icon.compare(lasticon) != 0)
      pElement_->SetIcon(icon);
   return true;
}

/**
 * Cambia el nombre de la fuente de datos
 */
const std::string MultimediaDatasource::GetCopyRight() const {
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
const std::string MultimediaDatasource::GetDescription() const {
   std::string description;
   if (pElement_ == NULL) return description;
   wxString wxdescription = pElement_->GetDescription();
   description = WS2S(wxdescription);
   return description;
}

/**
 * Cambia el url de la fuente de datos
 */
const std::string MultimediaDatasource::GetUrl() const {
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
const std::string MultimediaDatasource::GetIcon() const {
   std::string icon;
   if (pElement_ == NULL) return icon;
   wxString wxicon = pElement_->GetIcon();
   icon = WS2S(wxicon);
   return icon;
}

/** Devuelve una instancia del validador asociado a la fuente de datos**/
DatasourceValidatorInterface* MultimediaDatasource::GetAssociatedValidator() const {
   return NULL;
}

/** Obtiene el sistema de referencia que posee la fuente de datos*/
std::string MultimediaDatasource::GetSpatialReference() const {
   std::string spatialreference;
   if (pElement_)
      spatialreference = pElement_->GetSpatialReference().c_str();
   return spatialreference;
}

/** Devuelve el siguiente Datasource si es que al crearse se detecta multiplicidad */
DatasourceInterface* MultimediaDatasource::GetNextDatasource() {
	return NULL;
}

/** Setea el siguiente Datasource si es que al crearse se detecta multiplicidad */
bool MultimediaDatasource::SetNextDatasource(DatasourceInterface* pDatasource) {
	return false;
}

/** Devuelve true si un elemento esta listo para ser utilizado **/
bool MultimediaDatasource::IsValid() const {
   return true;
}

} /** namespace suri */
