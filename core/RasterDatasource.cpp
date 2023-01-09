/*! \file RasterDatasource.cpp */
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
#include "RasterDatasource.h"
#include "RasterRenderer.h"
#include "TmsCreator.h"

#include "suri/AuxiliaryFunctions.h"
#include "suri/DataTypes.h"
#include "suri/Element.h"
#include "suri/Image.h"
#include "suri/LayerInterface.h"
#include "suri/RasterElement.h"
#include "suri/TerrainElement.h"
#include "suri/Vector.h"

#include "wxmacros.h"

// Includes Wx
#include "wx/filename.h"

// Defines

// forwards

namespace suri {

/**
 * Macro para registrar el tipo de elemento
 */
AUTO_REGISTER_CLASS(DatasourceInterface, RasterDatasource, 0)

/**
 * Constructor
 */
RasterDatasource::RasterDatasource() :
      DatasourceInterface(wxT(TYPE_PROPERTY_VALUE_RASTER)),
      pElement_(NULL),
      pNextRasterDatasource_(NULL) {
}

/**
 * Destructor
 */
RasterDatasource::~RasterDatasource() {
}

/**
 * Create a partir del nombre de archivo
 */
RasterDatasource *RasterDatasource::Create(const std::string &FileName,
                                           const Option& Metadata) {
   RasterDatasource *praster = new RasterDatasource;
   if (praster->Initialize(FileName, Metadata) != praster) {
      delete praster;
      return NULL;
   }
   return praster;
}

/**
 * Initialize a partir del nombre de archivo
 */
RasterDatasource* RasterDatasource::Initialize(const std::string &FileName,
                                               const Option& Metadata) {
   metadata_ = Metadata;
   Element* pelement = NULL;

   pelement = suri::TmsCreator::Create(FileName.c_str(), Metadata);
   if (pelement == NULL)
      pelement = suri::Element::Create("WxsElement", FileName.c_str(), Metadata);

   bool iswxs = pelement != NULL;
   // Si falla al crear un WxsElement, intenta crear un Raster
   if (pelement == NULL) {
      // La recursividad corta aca cuando crea los subdatasets con su "File:LALA01"
      pelement = suri::Element::Create("RasterElement", FileName.c_str(), Metadata);

      // Si falla en crear un Raster, trata de ver si tiene subdataset
      if (pelement == NULL) {
         Image *pimage = Image::Open(RasterRenderer::GenerateImageId(FileName.c_str()));
         if (pimage) {
            // Obtiene los nombres de los subdatasets
            std::vector<std::string> subdatasetsnames = tokenizer(
                  pimage->GetOption(GdalImage::SUBDATASET_OPTION),
                  GdalImage::SUBDATASET_SEPARATOR);
            Image::Close(pimage);

            //
            if (subdatasetsnames.size() == 0)
               return NULL;

            // Itero por los nombres de archivos para ir agregando los datasources uno a uno
            std::vector<std::string>::iterator it = subdatasetsnames.begin();
            pElement_ = suri::Element::Create("RasterElement", *it);
            if (pElement_ == NULL)
               return NULL;

            // TODO(Gabriel - TCK #852): Esta es la parte donde se puede mejorar el
            // nombre archs. HDF5
            SetName(*it);
            DatasourceInterface* pprevious = this;
            for (++it; it != subdatasetsnames.end(); ++it) {
               DatasourceInterface* pds = DatasourceInterface::Create(
                     "RasterDatasource", *it, Metadata);
               if (pds) {
                  pprevious->SetNextDatasource(pds);
                  pprevious = pds;
               }
            }
            return this;
         }
      } else {
         Option::OptionsMapType options = Metadata.GetOption();
         if (!options.empty()) {
            // si lo pudo inicializar configura el xml con la metada
            pelement->AddNode(pelement->GetNode(FILE_NODE), RAW_METADATA);
            Option::OptionsMapType::const_iterator it = options.begin();
            wxXmlNode* prawmeta = pelement->GetNode(
            FILE_NODE NODE_SEPARATION_TOKEN RAW_METADATA);
            for (; it != options.end(); ++it) {
               pelement->AddNode(prawmeta, it->first, it->second);
            }
         }
      }
   }
   if (pelement == NULL)
      pelement = Element::Create("TerrainElement", FileName.c_str());
   pElement_ = pelement;
   // TODO(Gabriel - TCK #3580): Parche para la configuracion de nombre de la capa remota
   SetName(!iswxs ? FileName : pElement_->GetName().c_str());
   !metadata_.GetOption("title").empty() ? SetName(metadata_.GetOption("title")) : false;
   // Si se utiliza este tipo de renderizador para capas remotas no funciona.
   if (pElement_ && !iswxs)
      AddMultiplexorRenderer();
   return pElement_ ? this : NULL;
}

/**
 * Create a partir del nombre de archivo
 */
RasterDatasource *RasterDatasource::Create(const wxXmlNode* pRootNode) {
   RasterDatasource *praster = new RasterDatasource;
   if (praster->Initialize(pRootNode) != praster) {
      delete praster;
      return NULL;
   }
   return praster;
}

/**
 * Initialize a partir del nodo xml
 */
RasterDatasource* RasterDatasource::Initialize(const wxXmlNode* pRootNode) {
   const char *pelementtypes[] = { "TerrainElement", "WxsElement", "RasterElement"};
   Element *pelement = NULL;
   /** Si la fuente de datos es clasificada es necesario levantarla
    *  como raster. Al ser de una sola banda se crea como TerrainElement.
    */
   wxString elementtype;
   pRootNode->GetPropVal(TYPE_NODE, &elementtype);
   size_t i = 0;
   if (elementtype.compare(TYPE_PROPERTY_VALUE_RASTER) == 0)
      ++i;
   for (; i < ARRAY_SIZE(pelementtypes) && pelement == NULL; ++i)
      pelement = suri::Element::Create(pelementtypes[i], pRootNode);
   if (pelement == NULL)
      return NULL;

   pElement_ = pelement;
   // Si se utiliza este tipo de renderizador para capas remotas no funciona.
   if (pElement_->GetClassId().compare("WxsElement") != 0)
      AddMultiplexorRenderer();
   return this;
}

/**
 * Create a partir del elemento
 * \deprecated
 */
RasterDatasource *RasterDatasource::Create(Element *pElement) {
   RasterDatasource *praster = new RasterDatasource;
   if (praster->Initialize(pElement) != praster) {
      delete praster;
      return NULL;
   }
   return praster;
}

/** Funcion estatica para crear un DatasourceInterface */
RasterDatasource* RasterDatasource::Create(DatasourceInterface *pDatasource) {
   if (!pDatasource)
      return NULL;
   RasterDatasource *praster = new RasterDatasource;
   if (praster->Initialize(pDatasource) != praster) {
      delete praster;
      return NULL;
   }
   return praster;
}

/**
 * Initialize a partir del elemento
 * \deprecated
 */
RasterDatasource* RasterDatasource::Initialize(Element *pElement) {
   const char *pelementtypes[] = { "TerrainElement", "WxsElement", "RasterElement" };
   bool valid = false;
   if (!pElement) return NULL;
   std::string classid = pElement->GetClassId().c_str();
   for (int i = 0; !valid && i < 3; ++i) {
      valid = classid.compare(pelementtypes[i]) == 0;
   }
   if (!valid) {
      return NULL;
   }
   pElement_ = pElement;
   // Si se utiliza este tipo de renderizador para capas remotas no funciona.
   if (pElement_->GetClassId().compare("WxsElement") != 0) AddMultiplexorRenderer();
   return this;
}

/** Inicializa los atributos de Datasource a partir de un DatasourceInterface */
RasterDatasource* RasterDatasource::Initialize(DatasourceInterface *pDatasource) {
   if (!pDatasource)
      return NULL;
   return Initialize(pDatasource->GetElement()->GetNode());
}

/**
 * Obtiene el elemento
 * \deprecated
 */
Element* RasterDatasource::GetElement() {
   return pElement_;
}

/**
 * Setea el siguiente Datasource si es que al crearse se detecta multiplicidad
 * @return Devuelve true si el Datasource a ser asignado es del tipo RasterDatasource, sino false
 */
bool RasterDatasource::SetNextDatasource(DatasourceInterface* pDatasource) {
   RasterDatasource* rasterdatasource = NULL;
   if (pDatasource != NULL) {
      // Se efectua dynamic_cast porque solo pueden secuenciarse fuentes de dato del mismo tipo
      rasterdatasource = dynamic_cast<RasterDatasource*>(pDatasource);
      if (rasterdatasource == NULL)
         return false;
   }
   pNextRasterDatasource_ = rasterdatasource;
   return true;
}

/**
 * Devuelve el siguiente Datasource si es que al crearse se detecta multiplicidad
 */
DatasourceInterface* RasterDatasource::GetNextDatasource() {
   return pNextRasterDatasource_;
}

/**
 * Compara dos tipos diferentes de Datasource
 */
bool RasterDatasource::Equals(DatasourceInterface &Datasource) {
   if (GetElement() && Datasource.GetElement())
      return (Datasource.GetElement()->GetUid() == GetElement()->GetUid());
   else
      return false;
}

/**
 * Compara un Datasource contra un Layer
 */
bool RasterDatasource::Equals(LayerInterface &Layer) {
   return (Layer.GetElement()->GetUid() == GetElement()->GetUid());
}

/**
 * Compara un Datasource contra un Element
 * \deprecated
 */
bool RasterDatasource::Equals(Element &Element) {
   return (Element.GetUid() == GetElement()->GetUid());
}

/**
 * Devuelve la interpretacion de la fuente de datos como una imagen
 */
Image* RasterDatasource::GetDatasourceAsImage() {
   // TODO(Sebastian - TCK #852): Implementar
   return NULL;
}

/**
 * Devuelve la interpretacion de la fuente de datos como un vector
 */
Vector* RasterDatasource::GetDatasourceAsVector() {
   // TODO(Sebastian - TCK #852): Implementar
   return NULL;
}

/**
 * Setea el nombre de la fuente de datos
 *  @param[in] Name string con el nombre a asignar
 *  @return Devuelve true si tuvo exito, false en caso contrario
 **/
bool RasterDatasource::SetName(const std::string &Name) {
   if (pElement_ == NULL)
      return false;

   wxString fname;
   wxFileName filename(Name);

   // Caso especial para Landsat-8 (MTL)
   int typeix = filename.GetExt().Find(_(":"));
   if (typeix != wxNOT_FOUND) {
      wxString typedesc = filename.GetExt().Mid(typeix + 1);
      fname.Printf(_("%s (%s)"), filename.GetName().c_str(), typedesc.c_str());
   } else {
      fname = filename.GetName();
   }

   pElement_->SetName(fname);
   return true;
}

/**
 * Obtiene el nombre de la fuente de datos
 *  @return String con el nombre del Elemento asociado, string vacio en caso contrario
 **/
std::string RasterDatasource::GetName() const {
   std::string name;
   if (pElement_ == NULL)
      return name;
   wxString filename = pElement_->GetName();
   name = WS2S(filename);
   return name;
}

/**
 * Cambia el nombre de la fuente de datos
 */
bool RasterDatasource::SetCopyRight(const std::string &CopyRight) {
   if (pElement_ == NULL)
      return false;
   wxString copyright(CopyRight);
   pElement_->SetCopyRight(copyright);
   return true;
}

/**
 * Cambia la descripcion de una fuente de datos
 */
bool RasterDatasource::SetDescription(const std::string &Description) {
   if (pElement_ == NULL)
      return false;
   wxString description(Description);
   pElement_->SetDescription(description);
   return true;
}

/**
 * Cambia el url de la fuente de datos
 */
bool RasterDatasource::SetUrl(const std::string &Url) {
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
bool RasterDatasource::SetIcon(const std::string &Icon) {
   if (pElement_ == NULL)
      return false;
   wxString icon(Icon);
   pElement_->SetIcon(icon);
   return true;
}

/**
 * Cambia el nombre de la fuente de datos
 */
const std::string RasterDatasource::GetCopyRight() const {
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
const std::string RasterDatasource::GetDescription() const {
   std::string description;
   if (pElement_ == NULL) return description;
   wxString wxdescription = pElement_->GetDescription();
   description = WS2S(wxdescription);
   return description;
}

/**
 * Cambia el url de la fuente de datos
 */
const std::string RasterDatasource::GetUrl() const {
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
const std::string RasterDatasource::GetIcon() const {
   std::string icon;
   if (pElement_ == NULL) return icon;
   wxString wxicon = pElement_->GetIcon();
   icon = WS2S(wxicon);
   return icon;
}

/**
 * Retorna la cantidad de bandas. Si no tiene bandas, devuelve -1
 */
int RasterDatasource::GetBandCount() const {
   // Downcasting: Todos los tipos son son RasterElement
   RasterElement* pre = dynamic_cast<RasterElement*>(pElement_);
   if (pre == NULL)
      return -1;
   return pre->GetBandCount();
}

/**
 * Nombre del tipo de dato. Si no tiene tipo, devuelve un string vacio
 */
std::string RasterDatasource::GetDataType() const {
   // Downcasting: Todos los tipos son son RasterElement
   RasterElement* pre = dynamic_cast<RasterElement*>(pElement_);
   if (pre == NULL)
      return "";
   return pre->GetRasterProperties().DataType;
}

/** Devuelve una instancia del validador asociado a la fuente de datos**/
DatasourceValidatorInterface* RasterDatasource::GetAssociatedValidator() const {
   return NULL;
}

/** Obtiene el sistema de referencia que posee la fuente de datos*/
std::string RasterDatasource::GetSpatialReference() const {
   std::string spatialreference;
   if (pElement_)
      spatialreference = pElement_->GetSpatialReference().c_str();
   return spatialreference;
}

/** Devuelve true si un elemento esta listo para ser utilizado **/
bool RasterDatasource::IsValid() const {
   return true;
}


/** Metodo auxiliar que agregar el nodo multiplexor a la fuente de datos**/
void RasterDatasource::AddMultiplexorRenderer() {
   /**
    * TODO(Gabriel - #6748): Se inserta el nodo del renderizador multiplexor a modo de parche
    * para el defecto que imagenes de una sola banda hacen que los vectores
    * rendericen en escalas de gris. Si posee tabla de colores no se debe
    * agregar el nodo multiplexor
    **/
   if (!pElement_->GetNode(RENDERIZATION_NODE NODE_SEPARATION_TOKEN COLOR_TABLE)) {

      wxXmlNode* pmulnode = pElement_->GetNode(
      RENDERIZATION_NODE NODE_SEPARATION_TOKEN BAND_MULTIPLEXOR_NODE);
      if (pmulnode)
         pElement_->RemoveNode(pElement_->GetNode(RENDERIZATION_NODE), pmulnode);
      pmulnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, BAND_MULTIPLEXOR_NODE);
      wxXmlNode* prendernode = pElement_->GetNode(RENDERIZATION_NODE);
      wxXmlNode* pchild = prendernode ? prendernode->GetChildren() : NULL;
      while (pchild && pchild->GetNext())
         pchild = pchild->GetNext();
      prendernode->InsertChildAfter(pmulnode, pchild);
   }
}
} /** namespace suri */
