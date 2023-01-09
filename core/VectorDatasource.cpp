/*! \file VectorDatasource.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <map>
#include <string>

// Includes Suri
#include "VectorDatasource.h"
#include "wxmacros.h"
#include "suri/DatasourceInterface.h"
#include "suri/VectorElement.h"
#include "suri/Element.h"
#include "suri/Image.h"
#include "suri/Vector.h"
#include "suri/LayerInterface.h"
#include "suri/DataTypes.h"

// Includes Wx
#include "wx/filename.h"
#include <ogrsf_frmts.h>

// Defines

// forwards

namespace suri {

/**
 * Macro para registrar el tipo de elemento
 */
AUTO_REGISTER_CLASS(DatasourceInterface, VectorDatasource, 0)

/**
 * Constructor
 */
VectorDatasource::VectorDatasource() :
      DatasourceInterface(TYPE_PROPERTY_VALUE_VECTOR),
      pNextVectorDatasource_(NULL), pElement_(NULL), activeLayer_(0) {
   vectorTypeMap[Vector::Line] = "LineString";
   vectorTypeMap[Vector::Point] = "Point";
   vectorTypeMap[Vector::Polygon] = "Polygon";
}

/**
 * Destructor
 */
VectorDatasource::~VectorDatasource() {
}

/** Funcion estatica para crear un VectorDatasource
 * Metodo que crea un VectorDatasource. Requerido para implementar Abstract Factory
 * @param[in]  FileName
 * @return VectorDatasource *
 */
VectorDatasource *VectorDatasource::Create(const std::string &FileName,
                                           const Option& Metadata) {
   VectorDatasource *pvector = new VectorDatasource;
   if (pvector->Initialize(FileName, Metadata) != pvector) {
      delete pvector;
      return NULL;
   }
   return pvector;
}

/**
 * Setea la capa activa en el elemento interno
 * \deprecated
 */
void VectorDatasource::SetActiveLayer(int ActiveLayer) {
   activeLayer_ = ActiveLayer;
   pElement_->SetActiveLayer(ActiveLayer);
}

/**
 * Obtiene la capa activa
 */
const int VectorDatasource::GetActiveLayer() const {
   return activeLayer_;
}

/**
 * Inicializa el Datasource a partir de un path
 * @param[in]  FileName
 * @return VectorDatasource * referencia a la creacion del datasource, NULL en caso contrario
 */
VectorDatasource* VectorDatasource::Initialize(const std::string &FileName,
                                               const Option& Metadata) {
   VectorElement* pvector = CreateVectorElement(FileName, Metadata);
   
   if (pvector != NULL) {
      pElement_ = pvector;
      SetName(FileName);
      SetActiveLayer(0);
      VectorDatasource* pprevious = this;
      SetName(FileName);
      InitializeSubdatasets(FileName, this);
      pElement_->SetAttributeFilter("");
      size_t layerscount = pvector->GetLayerCount();
      for (size_t i = 1; i < layerscount; ++i) {
         pvector = CreateVectorElement(FileName, Metadata);
         if (pvector != NULL) {
            VectorDatasource *pdatasource = new VectorDatasource;
            InitializeSubdatasets(FileName, pdatasource);
            DatasourceInterface* pNextDS = pprevious->GetNextDatasource();
            pdatasource->SetNextDatasource(pNextDS);
            pdatasource->pElement_ = pvector;
            pdatasource->SetName(FileName);
            pdatasource->SetActiveLayer(i);
            pprevious->SetNextDatasource(pdatasource);
            pprevious = pdatasource;
         }
      }
      return this;
   }
   return NULL;
}

VectorElement* VectorDatasource::CreateVectorElement(const std::string &FileName,
                                           const Option& Metadata,
                                           Vector::VectorType Type) {
   const char *pelementtypes[] = { "AnotationElement", "HotLinkVectorElement",
                                   "VectorElement" };
   Element *pelement = NULL;
   for (size_t i = 0; i < ARRAY_SIZE(pelementtypes) && pelement == NULL; ++i)
      pelement = suri::Element::Create(pelementtypes[i], FileName.c_str(), Metadata);
   // Downcasting: Todos los tipos son son VectorElements
   VectorElement* pVector = dynamic_cast<VectorElement*>(pelement);
   if (Type != Vector::Undefined) {
      pVector->SetType(Type);
   }
   return pVector;
}

void VectorDatasource::InitializeSubdatasets(const std::string &FileName,
                                             VectorDatasource* pDatasource) {
   if (pDatasource->pElement_->GetType() != Vector::GeometryCollection) {
      return;
   }
   std::map<Vector::VectorType, std::string>::iterator it;
   int i = pDatasource->GetActiveLayer();
   VectorDatasource* pprevious = pDatasource;
   for (it = vectorTypeMap.begin(); it != vectorTypeMap.end(); ++it) {
      // Armo el nombre de la capa a mostrar y el filtro segun su geometria
      VectorDatasource *pSubdatasource = new VectorDatasource;
      std::string layerName = pDatasource->GetName().append(" (").append(it->second).append(")");
      std::string filter;
      filter.append("OGR_GEOMETRY='").append(it->second).append("'");
      // Creo un vector para configurar el element
      Vector* pVector = Vector::Open(FileName, Vector::ReadOnly, 1, it->first);
      VectorElement* pNewElement = CreateVectorElement(FileName, Option(), it->first);
      if (pVector != NULL) {
         pNewElement->SetAttributeFilter(filter);
         pNewElement->InitVector(pVector, FileName, it->first);
         std::string geom = pNewElement->GetTypeAsString().c_str();
         REPORT_DEBUG("Su geometria es %s", geom.c_str());
         pSubdatasource->pElement_ = pNewElement;
         pSubdatasource->SetName(layerName);
         pSubdatasource->SetActiveLayer(i);
         pprevious->SetNextDatasource(pSubdatasource);
         pprevious = pSubdatasource;
      }
      Vector::Close(pVector);
   }
   pprevious->SetNextDatasource(0);
}


/**
 * Create a partir del nodo de xml
 */
VectorDatasource *VectorDatasource::Create(const wxXmlNode* pRootNode) {
   VectorDatasource *pvector = new VectorDatasource;
   if (pvector->Initialize(pRootNode) != pvector) {
      delete pvector;
      return NULL;
   }
   return pvector;
}

/**
 * Initialize a partir del nodo de xml
 */
VectorDatasource* VectorDatasource::Initialize(const wxXmlNode* pRootNode) {
   const char *pelementtypes[] = { "AnotationElement", "HotLinkVectorElement",
                                   "VectorElement" };
   Element *pelement = NULL;
   for (size_t i = 0; i < ARRAY_SIZE(pelementtypes) && pelement == NULL; ++i)
      pelement = suri::Element::Create(pelementtypes[i], pRootNode, Option());
   // Downcasting: Todos los tipos son son VectorElements
   VectorElement* pVector = dynamic_cast<VectorElement*>(pelement);
   if (pVector == NULL)
      return NULL;

   pElement_ = pVector;
   return this;
}

/**
 * Initialize a partir del elemento
 * \deprecated
 */
VectorDatasource* VectorDatasource::Initialize(Element *pElement) {
   // Downcasting: Todos los tipos son son VectorElements
   VectorElement* pvector = dynamic_cast<VectorElement*>(pElement);
   if (pvector == NULL)
      return NULL;
   pElement_ = pvector;
   return this;
}

/** Inicializa los atributos de Datasource a partir de un DatasourceInterface */
VectorDatasource* VectorDatasource::Initialize(DatasourceInterface *pDatasource) {
   if (!pDatasource)
      return NULL;
   return Initialize(pDatasource->GetElement()->GetNode());
}

/** Funcion estatica para crear un DatasourceInterface */
VectorDatasource* VectorDatasource::Create(DatasourceInterface *pDatasource) {
   if (!pDatasource)
      return NULL;
   VectorDatasource *pvector = new VectorDatasource;
   if (pvector->Initialize(pDatasource) != pvector) {
      delete pvector;
      return NULL;
   }
   return pvector;
}

/**
 * Create a partir del elemento
 */
VectorDatasource *VectorDatasource::Create(Element *pElement) {
   VectorDatasource *pvector = new VectorDatasource;
   if (pvector->Initialize(pElement) != pvector) {
      delete pvector;
      return NULL;
   }
   return pvector;
}

/**
 * Obtiene el elemento
 * \deprecated
 */
Element* VectorDatasource::GetElement() {
   return pElement_;
}

/**
 * Setea el siguiente Datasource si es que al crearse se detecta multiplicidad
 * @return Devuelve true si el Datasource a ser asignado es del tipo VectorDatasource, sino false
 */
bool VectorDatasource::SetNextDatasource(DatasourceInterface* pDatasource) {
   VectorDatasource* pvectordatasource = NULL;
   if (pDatasource != NULL) {
      pvectordatasource = dynamic_cast<VectorDatasource*>(pDatasource);
      if (pvectordatasource == NULL)
         return false;
   }
   pNextVectorDatasource_ = pvectordatasource;
   return true;
}

/**
 * Devuelve el siguiente Datasource si es que al crearse se detecta multiplicidad
 */
DatasourceInterface* VectorDatasource::GetNextDatasource() {
   return pNextVectorDatasource_;
}

/**
 *verifica que la capa activa del elemento sea la misma
 * que el elem asociado a la fuente de datos
 * @param[in] pElement elemento a comparar
 * @return true en caso de que sea la misma, false en caso contrario
 */
bool VectorDatasource::HasSameActiveLayer(Element *pElement) {
   bool same = true;
   VectorElement *pvector = dynamic_cast<VectorElement*>(pElement);
   if (pvector != NULL)
      same = (pvector->GetActiveLayer() == GetActiveLayer());
   return same;
}

/**
 * Compara dos tipos diferentes de Datasource
 */
bool VectorDatasource::Equals(DatasourceInterface &Datasource) {
   bool sameid = (Datasource.GetElement()->GetUid() == GetElement()->GetUid());
   bool sameactivelayer = HasSameActiveLayer(Datasource.GetElement());
   return (sameid && sameactivelayer);
}

/**
 * Compara un Datasource contra un Layer
 */
bool VectorDatasource::Equals(LayerInterface &Layer) {
   bool sameid = (Layer.GetElement()->GetUid() == GetElement()->GetUid());
   bool sameactivelayer = HasSameActiveLayer(Layer.GetElement());
   return (sameid && sameactivelayer);
}

/**
 * Compara un Datasource contra un Element
 */
bool VectorDatasource::Equals(Element &Element) {
   bool sameid = Element.GetUid() == GetElement()->GetUid();
   bool sameactivelayer = HasSameActiveLayer(&Element);
   return (sameid && sameactivelayer);
}

/**
 * Devuelve la interpretacion de la fuente de datos como una imagen
 */
Image* VectorDatasource::GetDatasourceAsImage() {
   // TODO(Sebastian - TCK #852): Implementar
   return NULL;
}

/**
 * Devuelve la interpretacion de la fuente de datos como un vector
 */
Vector* VectorDatasource::GetDatasourceAsVector() {
   return Vector::Open(GetUrl().c_str());
}

/** Setea el nombre de la fuente de datos
 * @param[in] Name string con el nombre a asignar
 * @return Devuelve true si tuvo exito, false en caso contrario
 */
bool VectorDatasource::SetName(const std::string &Name) {
   if (pElement_ == NULL)
      return false;
   if (wxFileName::FileExists(Name)) {
      wxFileName filename(Name);
      pElement_->SetName(filename.GetName());
   } else {
      pElement_->SetName(Name);
   }
   return true;
}

/** Obtiene el nombre de la fuente de datos
 * @return String con el nombre del Elemento asociado, string vacio en caso contrario
 */
std::string VectorDatasource::GetName() const {
   std::string name;
   if (pElement_ == NULL)
      return name;
   wxString filename = pElement_->GetName();
   name = WS2S(filename);
   return name;
}

/** Cambia el nombre de la fuente de datos
 * @param[in] CopyRight
 * @return true si pudo cambiar el nombre (el elem asociado no es NULL), false en otro caso
 */
bool VectorDatasource::SetCopyRight(const std::string &CopyRight) {
   if (pElement_ == NULL)
      return false;
   wxString copyright(CopyRight);
   pElement_->SetCopyRight(copyright);
   return true;
}

/** Cambia la descripcion de una fuente de datos
 *  @param[in] Description
 *  @return true si pudo cambiar la descripcion (el elem asociado no es NULL), false en otro caso
 */
bool VectorDatasource::SetDescription(const std::string &Description) {
   if (pElement_ == NULL)
      return false;
   wxString desc(Description);
   pElement_->SetDescription(desc);
   return true;
}

/** Cambia el url de la fuente de datos
 *  @param[in] Url
 *  @return true si pudo cambiar el Url (el elem asociado no es NULL), false en otro caso
 */
bool VectorDatasource::SetUrl(const std::string &Url) {
   if (pElement_ == NULL)
      return false;
   wxString url(Url);
   pElement_->SetUrl(url);
   return true;
}

/** Cambia el icono asociado a la fuente de datos
 * \deprecated
 *  @param[in] Icon
 *  @return true si pudo cambiar el icono (el elem asociado no es NULL), false en otro caso
 */
bool VectorDatasource::SetIcon(const std::string &Icon) {
   if (pElement_ == NULL)
      return false;
   wxString ic(Icon);
   pElement_->SetIcon(ic);
   return true;
}

/**
 * Obtiene el copyright de la fuente de datos
 */
const std::string VectorDatasource::GetCopyRight() const {
   std::string copyright;
   if (pElement_ == NULL)
      return copyright;
   wxString wxcopyright = pElement_->GetCopyRight();
   copyright = WS2S(wxcopyright);
   return copyright;
}

/**
 * Obtiene la descripcion de una fuente de datos
 */
const std::string VectorDatasource::GetDescription() const {
   std::string description;
   if (pElement_ == NULL)
      return description;
   wxString wxdescription = pElement_->GetDescription();
   description = WS2S(wxdescription);
   return description;
}

/**
 * Obtiene el url de la fuente de datos
 */
const std::string VectorDatasource::GetUrl() const {
   std::string url;
   if (pElement_ == NULL)
      return url;
   wxString wxurl = pElement_->GetUrl();
   url = WS2S(wxurl);
   return url;
}

/**
 * Obtiene el icono asociado a la fuente de datos
 * \deprecated
 */
const std::string VectorDatasource::GetIcon() const {
   std::string icon;
   if (pElement_ == NULL)
      return icon;
   wxString wxicon = pElement_->GetIcon();
   icon = WS2S(wxicon);
   return icon;
}

/** Devuelve una instancia del validador asociado a la fuente de datos**/
DatasourceValidatorInterface* VectorDatasource::GetAssociatedValidator() const {
   return NULL;
}


bool VectorDatasource::HasHotlink() const {
   if (pElement_ == NULL)
      return false;
   VectorElement* pvector = dynamic_cast<VectorElement*>(pElement_);
   if (!pvector)
      return false;
   return !pvector->GetHotLinkFieldName(pvector->GetActiveLayer()).empty();
}

/** Obtiene el sistema de referencia que posee la fuente de datos*/
std::string VectorDatasource::GetSpatialReference() const {
   std::string spatialreference;
   if (pElement_)
      spatialreference = pElement_->GetSpatialReference().c_str();
   return spatialreference;
}
/** Obtiene el nombre de la capa activa
 *  @return string con el nombre de la capa
 *  @return string vacio en caso de no encontrar la capa activa **/
std::string VectorDatasource::GetActiveLayerName() const {
   return pElement_ ? pElement_->GetActiveLayerName() : "";
}

/** Devuelve true si un elemento esta listo para ser utilizado **/
bool VectorDatasource::IsValid() const {
   bool valid = false;
   if (pElement_){
      valid = pElement_->GetType() != Vector::GeometryCollection;
   }
   return valid;
}

} /* namespace suri */
