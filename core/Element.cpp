/*! \file Element.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standar
#include <string>
#include <vector>
#include <typeinfo>
#include <memory>
#include <map>

// Includes suri
#include "suri/Element.h"
#include "suri/messages.h"
#include "suri/Widget.h"
#include "suri/xmlnames.h"
#include "suri/Subset.h"
#include "suri/AuxiliaryFunctions.h"
#include "VectorRenderer.h"
#include "suri/VectorStyle.h"
#include "resources.h"
#include "suri/Uri.h"
#include "GenericPropertiesPart.h"
#include "XmlPropertiesPart.h"
#include "suri/AbstractFactory.h"
#include "GeorreferenceProperties.h"
#include "suri/md5.h"
#include "WxFileCache.h"
#include "GdalWarperTool.h"
#include "wxmacros.h"
#include "suri/DataViewManager.h"
#include "suri/DatasourceInterface.h"
#include "suri/LayerInterface.h"
#include "suri/Configuration.h"
#include "suri/Wkt.h"
#include "suri/ProcessHistoryPart.h"

// Includes wx
#include "wx/sstream.h"
#include "wx/xrc/xmlres.h"

// Defines

#ifndef ICON_MAX_SIZE
/** Size maximo de archivo permitido */
#  define ICON_MAX_SIZE    10485760
#endif

#ifndef ICON_WIDTH
/** Ancho de los iconos (valor al cual se escalan) */
#  define ICON_WIDTH       32
#endif

#ifndef ICON_HEIGHT
/** Altura de los iconos (valor al cual se escalan) */
#  define ICON_HEIGHT      32
#endif


/** namespace suri */
namespace suri {
const std::string Element::uidNull_ = wxT("");

/** Macro de la factoria que crea metodos de implementacion */
HAS_FACTORY_IMPLEMENTATION_COMMON(Element, 0)
/** Macro de la factoria que crea metodos de implementacion */
HAS_FACTORY_IMPLEMENTATION(Element, 2, const std::string &Value1,
                           const Option& Value2)
/** Macro de la factoria que crea metodos de implementacion */
HAS_FACTORY_IMPLEMENTATION(Element, 2, const wxXmlNode* Value1,
                           const Option& Value2)

/** inicializa la clase con un nodo xml */
/**
 * Prueba de crear elementos utilizando el URL que proviene del pRootNode hasta
 * que encuentra uno que se crea con dicho URL y que tiene el mismo tipo que
 * el actual (this).
 * \todo Mover a las clases derivadas y hacer una actualizacion del XML
 *      contemplando las idiosincracias de cada elemento.
 * @param[in] pRootNode XML correspondiente a un elemento
 * @return la instancia creada con una copia del XML pasado
 */
Element *Element::Initialize(const wxXmlNode* pRootNode,
                             const Option& Metadata) {
   if (!pRootNode)
      return NULL;
   // obtengo el url de un XmlElement
   wxXmlNode* pnode = pRootNode->GetChildren();
   wxString pnodecontent = pnode->GetName();
   while (pnode != NULL && (pnodecontent.CompareTo(URL_NODE) != 0)) {
      pnode = pnode->GetNext();
      pnodecontent = pnode->GetName();
   }
   if (pnode == NULL) {
      return NULL;
   }
   metadata_ = Metadata;
   std::string url = pnode->GetNodeContent().c_str();
   // recorro los Elementos disponibles
   // ATENCION: Debe haber un orden jerarquico en la enumeracion
   EnumerationType enumeration = Enumerate();
   for (EnumerationType::const_iterator it = enumeration.begin(); it != enumeration.end(); it++) {
      // creo elementos con el url
      Element *pelement = Element::Create(*it, url, Metadata);
      // si tiene exito y son del mismo tipo, le agrego el xml a this y salgo
      if (pelement && typeid(*this) == typeid(*pelement)) {
         AddNode(NULL, new wxXmlNode(*pRootNode), true);
         Element::Destroy(pelement);
         // condicion de exito para los metodos de factoria
         return this;
      }
      Element::Destroy(pelement);
   }
   // falla
   return NULL;
}

/**
 * Inicializa el elemento con el url. Permite instanciar Elements.
 * @param[in] Url url del elemento
 * @return retorna siempre this
 */
Element *Element::Initialize(const std::string& Url,
                             const Option& Metadata) {
   this->SetUrl(Url);
   return this;
}

// ---------------------------- ELEMENTO DE CONTEXO ----------------------------

/**
 * Constructor
 * @param[in] ElementType tipo de elemento
 */
Element::Element(const wxString& ElementType) :
      XmlElement(CONTEXT_ELEMENT_NODE, ElementType), shouldRender_(true), isActive_(false),
      edited_(false), pAssociatedElement_(NULL) {
}

/**
 * Destructor
 */
Element::~Element() {
   REPORT_DEBUG("D:Destruyendo elemento %s", GetName().c_str());
   isActive_ = false;
   Clean();
}

/**
 * Asigna el nombre del elemento
 * @param[in] ElementName nombre del elemento
 */
void Element::SetName(const wxString& ElementName) {
   AddNode(GetNode(wxT("")), wxT(NAME_NODE), ElementName);
}

/**
 * Asigna la descripcion del elemento
 * @param[in] ElementDescription descripcion del elemento
 */
void Element::SetDescription(const wxString& ElementDescription) {
   AddNode(GetNode(""), DESCRIPTION_NODE, ElementDescription);
}

/** asigna el url del elemento. */
/**
 * guarda el path en ElementUrl con formato de unix en el nodo pParentNode.
 * @param[in] ElementUrl url que se debe agregar al nodo pParentNode
 * @param[in] pParentNode nodo en el que debe agregarse el URL
 * @param[in] Replace si se remplaza en caso de existir el URL en el nodo
 */
void Element::SetUrl(const wxString &ElementUrl, wxXmlNode *pParentNode, bool Replace) {
   wxString url = NormalizeUrl(ElementUrl);
   AddNode(pParentNode ? pParentNode : GetNode(wxT("")), URL_NODE, url, wxT(""), wxT(""), Replace);
   SetChanged();
   SendViewerUpdate();
}

/**
 * Agrega el nodo copyright al elemento
 * @param[in] ElementCopyright copyright del elemento
 */
void Element::SetCopyRight(const wxString &ElementCopyright) {
   AddNode(GetNode(wxT("")), COPYRIGHT_NODE, ElementCopyright);
}

/**
 * Objetivo: Genera el nodo Archivo, completando los nodo-hijo: Tamanio y Tipo
 * de archivo.
 * @param[in]  FileType: tipo de archivo.
 * @param[in]  FormatDescription : formato de archivo
 */
void Element::SetFileNode(const wxString &FileType, const wxString &FormatDescription) {
   wxXmlNode *pnode = AddNode(GetNode(wxT("")), FILE_NODE);
   AddNode(pnode, SIZE_NODE, GetSizeAsString());
   wxXmlNode *pnodeformat = AddNode(pnode, FORMAT_NODE);
   AddNode(pnodeformat, TYPE_NODE, wxT(FileType));
   AddNode(pnodeformat, DESCRIPTION_NODE, wxT(FormatDescription));
   AddNode(pnodeformat, PROPERTIES_NODE);
}

/**
 * retorna la descripcion del elemento
 * @return string con la descripcion del elemento, vacio si no se encontro el nodo
 */
wxString Element::GetDescription() {
   wxXmlNode *pnode = GetNode(DESCRIPTION_NODE);
   if (pnode && pnode->GetChildren()) {
      return pnode->GetChildren()->GetContent();
   }
   return wxT("");
}

/**
 * devuelve el nombre asociado al elemento
 * @return string con el nombre del elemento, vacio si no se encontro el nodo
 */
wxString Element::GetName() const {
   wxXmlNode *pnode = GetNode(NAME_NODE);
   if (pnode && pnode->GetChildren()) {
      wxString name = pnode->GetChildren()->GetContent();
      wxString shpmem = wxT("shpmemory:");
      wxString auxName;
      bool memory = name.StartsWith(shpmem, &auxName);
      if (memory) return auxName;
      REPORT_DEBUG("Devolviendo el nombre %s", name.c_str());
      return name;
   }
   return wxT("");
}

/**
 * devuelve el url asociado al elemento.
 * @return string con el url del elemento, vacio si no se encontro el nodo
 */
wxString Element::GetUrl() const {
   wxXmlNode *pnode = GetNode(URL_NODE);
   if (pnode && pnode->GetChildren()) {
      return pnode->GetChildren()->GetContent();
   }
   return wxT("");
}

/**
 *  Setea el icono y lo actualiza en el XML y las dependencias.
 *  Si no tenia icono customizado, lo agrega y sino lo modifica,
 *  eliminando el anterior.
 *
 *  \todo Si agrego el icono por defecto, no deberia agregarse el nodo icon
 *  @param[in] FileUrl: String con la Url del archivo.
 *  @return icono (dependiendo de lo explicado anteriormente)
 */
wxString Element::SetIcon(const wxString& IconUrl) {
   wxXmlNode *pnode = GetNode(wxString(ICON_NODE));

   // Agrego o actualizo las dependencias
   if (pnode == NULL) {
      // Nodo XML
      AddNode(GetNode(wxT("")), ICON_NODE);
   } else {
      // Nodo XML
      pnode = GetNode(ICON_NODE NODE_SEPARATION_TOKEN URL_NODE);
      wxString oldiconurl = NormalizeUrl(pnode->GetNodeContent());
      // Dependencias
      DelDependency(WS2S(oldiconurl), GetNode(DEPENDENCY_NODE));
   }

   wxString formattedurl = NormalizeUrl(IconUrl);
   wxString memfile = WxFileCache::Instance()->GetFile(WS2S(formattedurl));
   SetUrl(formattedurl, GetNode(ICON_NODE), true);
   // Si no se encuentra en el cache, lo agrego
   if (memfile.IsEmpty()) {
      memfile = WxFileCache::Instance()->AddImageFile(WS2S(formattedurl), ICON_WIDTH, ICON_HEIGHT);
   } else {
      // Si se trata de un archivo en el FS de memoria(wx) no lo agrega como dependencia
      if (!formattedurl.StartsWith("memory:"))
         AddDependency(WS2S(formattedurl));
   }

   return memfile;
}

/**
 * Devuelve el identificador unico del elemento. El UID estara formado por el
 * url concatenado con las dependencias del mismo.
 * @return UID del elemento.
 * \see VectorElement::GetUid
 */
Element::UidType Element::GetUid() const {
   // obtengo url
   std::vector<wxString> aux;
   aux.push_back(GetUrl());

   // obtengo dependencias.
   std::vector<wxString> depvec = GetDependency();
   std::vector<wxString>::iterator it = depvec.begin();
   for (; it != depvec.end(); it++)
      aux.push_back(*it);

   return suri::MD5String(join(aux.begin(), aux.end(), wxT(":")).c_str());
}

/**
 * retorna el copyright asociado
 * @return string con el nodo copyright del elemento, vacio si no se encontro el nodo
 */
wxString Element::GetCopyRight() const {
   wxXmlNode *pnode = GetNode(COPYRIGHT_NODE);
   if (pnode && pnode->GetChildren()) {
      return pnode->GetChildren()->GetContent();
   }
   return wxT("");
}

/**
 * Devuelve el Icono:
 * -  En caso de poseer un nodo ICON_NODE en el XML, se cargara ese icono.
 *    Se actualizaran las dependencias en este caso.
 * -  Si la url a ese icono tiene algun problema (no existe, muy grande, etc),
 *    se mostrara devolvera el mismo valor (que se reflejara como icono roto).
 * -  Si no posee ICON_NODE, se adopta el comportamiento por defecto definido
 *    en DoGetIcon definido en las clases derivadas.
 * @return icono (dependiendo de lo explicado anteriormente)
 * \Note:   Este metodo no es "const" debido a que utiliza AddDependency
 *          (que no es const)
 */
wxString Element::GetIcon() {
   wxXmlNode *pnode = GetNode(ICON_NODE NODE_SEPARATION_TOKEN URL_NODE);
   if (pnode != NULL) {
      wxString iconurl = NormalizeUrl(pnode->GetNodeContent());
      if (!iconurl.IsEmpty()) {
         wxULongLong size = wxFileName::GetSize(iconurl);

         if (size != wxInvalidSize && 0 < size && size < ICON_MAX_SIZE) {
            wxString memoryfile = WxFileCache::Instance()->GetFile(WS2S(iconurl));
            // Si no se encuentra en el cache, lo agrego
            if (memoryfile.IsEmpty()) {
               memoryfile = WxFileCache::Instance()->AddImageFile(WS2S(iconurl), ICON_WIDTH,
                                                                  ICON_HEIGHT);
               // Si no lo puede agregar, hay un problema de memoria
               // y se termina por mostrar un icono roto
               if (memoryfile.IsEmpty()) {
                  return iconurl;
               }
            }
            // Para el caso en que se valide la totalidad de los requisitos
            // Se agrega la dependencia al xml y se lo agrega al cache
            AddDependency(WS2S(iconurl));
            return memoryfile;
         } else {
            // Este es el caso donde hay errores
            // (no existe archivo o es demasiado grande)
            // Se procede a devolver el contenido del nodo para mostrar
            // el comportamiento por defecto de un icono roto
            return iconurl;
         }
      }
   }
   // En el caso que no tenga nodo "icon" o lo tenga vacio se adopta
   // el comportamiento por defecto (devuelve el icono por defecto segun es
   // redefinido por cada clase hija de Element)
   return DoGetIcon();
}

/**
 * Devuelve un string para el icono.
 * Este metodo debe ser reescrito en las clases que hereden de element.
 * @return string vacio
 */
wxString Element::DoGetIcon() const {
   return icon_UNDEFINED_ELEMENT;
}

/**
 * crea una previsualizacion del elemento
 * @param[out] Preview icono previsualizacion elemento
 * @param[in] Width ancho de la previsualizacion
 * @param[in] Height alto de la previsualizacion
 */
void Element::GetPreview(wxBitmap &Preview, int Width, int Height) const {
   Preview = wxBitmap(Width, Height);
}

/**
 * retorna los detalles del elemento
 * @return vector de strings con el contenido de los nodos hijos del elemento
 */
std::vector<wxString> Element::GetDependency() const {
   wxXmlNode *pnode = GetNode(DEPENDENCY_NODE);
   std::vector<wxString> vecdep;
   if (pnode && pnode->GetChildren()) {
      pnode = pnode->GetChildren();
   }
   while (pnode) {
      vecdep.push_back(pnode->GetContent());
      pnode = pnode->GetNext();
   }
   return vecdep;
}

/**
 * retorna el tipo de elemento
 * @return string con el tipo de elemento, vacio si no se encontro el nodo
 */
wxString Element::GetPropertyType() const {
   wxXmlNode *pnode = GetNode(wxT(""));
   if (pnode && pnode->GetProperties()) {
      return pnode->GetPropVal(TYPE_PROPERTY, wxT(""));
   }
   return wxT("");
}

/**
 * Obtiene la referencia espacial del elemento
 * @return el wxString con SPATIAL_REFERENCE_NODE, caso contrario wxT("")
 */
wxString Element::GetSpatialReference() const {
   wxXmlNode *pnode = GetNode(SPATIAL_REFERENCE_NODE, GetNode(GEORREFERENCE_NODE));
   if (pnode && pnode->GetChildren()) {
      return pnode->GetChildren()->GetContent();
   }
   return wxT("");
}

/**
 * carga la referencia espacial
 * Agrega el nodo Wkt al nodo de GEORREFERENCE_NODE(si no existe el nodo lo crea)
 * @param[in] Wkt string con referencia espacial para agrear al elemento
 * @return true / false resultado de agregar el nodo al elemento
 */
bool Element::SetSpatialReference(const std::string &Wkt) {
   wxXmlNode *pnode = GetNode(GEORREFERENCE_NODE);
   if (!pnode) {
      pnode = AddNode(GetNode(wxT("")), GEORREFERENCE_NODE);
   }
   return AddNode(pnode, SPATIAL_REFERENCE_NODE, Wkt, wxT(""), wxT(""), true) != NULL;
}

/**
 * Muestra la herramienta asociada
 * @param[in] pParent puntero a la ventana padre
 * @return ventana de propiedades del elemento
 */
wxWindow* Element::GetPropertyWindow(wxWindow *pParent) {
   XmlPropertiesPart *ptool = new XmlPropertiesPart(this);
   return ptool->GetWindow();
}

/**
 * Accesor para el estado de activo
 * @param[in] SetActive estado de activacion del elemento
 */
void Element::Activate(bool SetActive) {
   if (IsEdited()) {
      SetActive = true;
   }

   if (isActive_ == SetActive) {
      PreventNextViewerUpdate(false);
      return;
   }

   isActive_ = SetActive;
   SetChanged();
   SendViewerUpdate();
}

/**
 * Accesor para el estado de activo en modo GIS.
 * Solo se usa para el modo GIS. NO usar en otro caso.
 *
 * @param[in] SetActive estado de activacion del elemento.
 */
void Element::ActivateGIS(bool SetActive) {
   if (SetActive) {
      std::string worldwkt = Configuration::GetParameterEx("GISMode_World_Wkt", "");
      std::string srcfilepath = GetUrl().c_str();
      std::string tempfilepath = wxFileName::CreateTempFileName(_("")).c_str();

      Wkt* pworld = Wkt::Create(worldwkt);
      Wkt* pme = Wkt::Create(GetSpatialReference().c_str());
      double noData = 0;
      bool valid = false;
      GetNoDataValue(valid, noData);
      if (pworld && pworld->GetAuthorityCode() != pme->GetAuthorityCode()) {
         GdalWarperTool warper(srcfilepath, tempfilepath);
         if (warper.Warp(worldwkt, false, 0)) {
            wxString temp =  tempfilepath;
            temp.Replace(".tmp", ".tif");
            wxRenameFile(tempfilepath, temp.c_str());
            Element* pe = Element::Create("RasterElement", temp.c_str());
            pe->AddNode(pe->GetNode(), new wxXmlNode(*(GetNode(RENDERIZATION_NODE))), true);
            if (valid)
                  pe->SetNoDataValue(noData);
            std::vector<View*> viewvec;
            std::list<View*>::iterator it;

            // Arma viewvec con los mismos datos que viewers_
            for (it = viewers_.begin(); it != viewers_.end(); ++it) {
               viewvec.push_back((*it));
            }

            // Para cada view en viewvec
            for (size_t i = 0; i < viewvec.size(); i++) {
               // si el viewer esta en uniqueviewers
               if (uniqueviewers_.find(viewvec[i]) != uniqueviewers_.end()) {
                  pe->RegisterViewer(viewvec[i]);
               }
            }
            pe->isActive_ = true;
            pe->SetChanged();
            SetAssociatedElement(pe);
         }
      }
   } else {
      SetAssociatedElement(NULL);
   }

   Activate(SetActive);
}

/**
 * Accesor para el estado de editar
 * @param[in] SetEdit estado de edicion del elemento
 */
void Element::SetEdited(bool SetEdit) {
   // si se estaba editando y se cierra la edicion
   // actualiza
   if (edited_ && !SetEdit) {
      UpdateData();
   }
   edited_ = SetEdit;
}

/**
 * Informa si el estado del elemento es valido. Por default verifica
 * si el url apunta a archivo existente
 * @return indica si existe el archivo que figura en url de elemento.
 */
bool Element::IsValid() {
   return wxFileName::FileExists(this->GetUrl());
}

/**
 * Devuelve el tamanio de un elemento incluyendo el de sus dependencias como
 * string.
 * @return Tamanio del elemento si el tamanio es > 0 (cero). Representa un
 *         numero en punto flotante con precision 2 seguido por la unidad que
 *         puede ser (B, kB, MB, GB, TB).
 * @return "No disponible." si el tamanio es <= 0 (cero).
 */
wxString Element::GetSizeAsString() {
   return wxFileName::GetHumanReadableSize(GetSize(), _(message_NOT_AVAILABLE), 2);
}

/**
 * Devuelve el tamanio de un elemento incluyendo el de sus dependencias en bytes.
 * En caso de que algun URL sea un directorio, el tamanio sera considerado 0.
 * \pre asegurarse de que el url del elemento y el url de sus dependencias sean
 * absolutos, de lo contrario calcula mal el tamanio del elemento.
 * @return tamanio en bytes del elemento.
 * @return wxInvalidSize si se produjo algun error al intentar obtener el tamanio
 * del elemento (o dependencia).
 */
wxULongLong Element::GetSize() {
   wxULongLong size = wxFileName::GetSize(GetUrl());
   // si dio invalido pero se trata de un directorio, reinicializo con 0
   if (size == wxInvalidSize && wxFileName::DirExists(GetUrl())) {
      size = 0;
   }

   std::vector<wxString> vecdep = GetDependency();
   wxULongLong sizedep = 0;
   // mientras haya dependencias y no tenga tamanio invalido
   for (size_t i = 0; i < vecdep.size() && size != wxInvalidSize; i++) {
      sizedep = wxFileName::GetSize(vecdep.at(i));
      // mismo concepto, si se trata de un directorio, asumo sizedep = 0
      if (sizedep == wxInvalidSize) {
         if (!wxFileName::DirExists(vecdep.at(i))) {
            size = wxInvalidSize;
         }
      } else {
         size += sizedep;
      }
   }
   return size;
}

/** Genera vector con parts de elemento y los de sus descendientes */
/**
 * Crea vector con part de elemento generico y agrega los que genere su clase
 * hijo en DoGetParts
 * @return Vector con Part de propiedades genericas (comun a todos los elementos)
 * mas las Parts que agregue un elemento en particular.
 * @return si esta definido el simbolo __DEBUG__, vector con Part de propiedades
 * genericas (comun a todos los elementos) mas las Parts que agregue un elemento
 * en particular mas Part que muestra xml del elemento.
 */
std::vector<Part *> Element::GetParts(DataViewManager* pDataViewManager,
                                      LayerInterface* pLayer,
                                      DatasourceInterface* pDatasource) {
   std::vector<Part *> vectorparts;
   if (!pDataViewManager || !pDatasource || !pLayer) {
      vectorparts.push_back(new GenericPropertiesPart(this));
      std::vector<Part *> concreteparts = DoGetParts();
      vectorparts.insert(vectorparts.end(), concreteparts.begin(), concreteparts.end());
      if (!GetSpatialReference().IsEmpty()) {
         vectorparts.push_back(
               new GeorreferenceProperties(this, pDataViewManager->GetLibraryManager()));
      }
#ifdef __DEBUG__
      vectorparts.push_back(new XmlPropertiesPart(this));
#endif
   } else {
      vectorparts.push_back(
            new GenericPropertiesPart(pDataViewManager->GetDatasourceManager(),
                                      pDatasource));
      std::vector<Part *> concreteparts = DoGetParts(pDataViewManager, pLayer,
                                                     pDatasource);
      vectorparts.insert(vectorparts.end(), concreteparts.begin(), concreteparts.end());
      if (!GetSpatialReference().IsEmpty()
            && (Configuration::GetParameter("app_short_name", "") == wxT("SoPI"))) {
         vectorparts.push_back(
               new GeorreferenceProperties(pDataViewManager->GetDatasourceManager(),
                                           pDatasource, pDataViewManager->GetLibraryManager()));
      }

#ifdef __DEBUG__
      vectorparts.push_back(new XmlPropertiesPart(pDatasource, pLayer));
#endif
   }
   // TCK #5964 : Fix temporal para fusionar aspecto y propiedades.
   if (Configuration::GetParameter("app_short_name", "") != wxT("SoPI")) {
      std::vector<Part *> parts = GetVisualizationParts(pDataViewManager,
                                                        pLayer, pDatasource);
      for (size_t i = 0; i < parts.size(); ++i) {
         vectorparts.push_back(parts[i]);
      }
   } else if (pDatasource->GetElement()->GetNode(PROCESS_HISTORY_NODE)) {
      // Historial de procesamiento, solo valido para sopi.
      vectorparts.push_back(new ProcessHistoryPart(pDatasource));
   }
   return vectorparts;
}
/** Genera un vector con Parts especificos de la visualizacion elemento. */
std::vector<suri::Part*> Element::GetVisualizationParts(
      DataViewManager* pDataViewManager, LayerInterface *pLayer,
      DatasourceInterface *pDatasource) {
   return DoGetVisualizationParts(pDataViewManager, pLayer, pDatasource);
}

/**
 * Obtiene el nodo con informacion del archivo asociado al elemento(tamano, formato)
 * @return nodo con informacion dle archivo asociado
 */
wxXmlNode * Element::GetFileNode() {
   return GetNode(FILE_NODE);
}

/**
 * Indica si se debe renderizar el elemento
 * @return estado de la propiedad shouldRender_
 */
bool Element::ShouldRender() {
   return shouldRender_;
}

/**
 * Retorna Extent de elemento default (0,0,0,0). Este metodo deberia ser
 * definido para cada clase que herede de Element y tenga un extent.
 * @param[out] ElementExtent Extent del elemento
 */
void Element::GetElementExtent(Subset &ElementExtent) {
   ElementExtent = Subset(0, 0, 0, 0);
}

/** Agrega un archivo a la lista de dependencias */
/**
 * lee en nodo de dependencias(lo crea si no existe), verifica que la dependencia
 * no exista en el mismo y llama a SetUrl.
 * \post se agrega nodo con la dependencia al nodo del elemento
 * @param[in] Filename dependecia que se debe agregar al nodo del elemento
 */
void Element::AddDependency(const std::string &Filename) {
   // Agrega nodo DEPENDENCIAS
   wxXmlNode *pdnode = GetNode(DEPENDENCY_NODE);
   if (!pdnode) {
      pdnode = AddNode(GetNode(wxT("")), DEPENDENCY_NODE);
   }

   // verifico que no se agregue 2 veces el mismo nodo url
   wxXmlNode *pnodeurl = pdnode->GetChildren();
   while (pnodeurl && pnodeurl->GetNodeContent().Cmp(Filename) != 0)
      pnodeurl = pnodeurl->GetNext();

   // si no encontro un nodo url con el mismo contenido que Filename => inserto.
   if (!pnodeurl)
#ifndef __UNUSED_CODE__
   {
      BlockViewerUpdate();
      SetUrl(Filename, pdnode, false);
      UnblockViewerUpdate();
   }
#else
   { // Saco esto para Agregar URLs usando SetUrl para que transforme las barras en forma correca
      AddNode(pdnode, URL_NODE, Filename, wxT(""), wxT(""), false);
   }
#endif
}

/** Elimina un archivo de la lista de dependencias */
/**
 * Si el nodo pasado es de dependencias busco el Filename dentro de este.
 * Si lo encuentro lo retiro del nodo y lo elimino.
 * @param[in] Filename dependencia que debe eliminarse
 * @param[in] pDepNode nodo de dependencias con Filename
 * @param[out] pDepNode nodo de dependencias sin Filename
 */
void Element::DelDependency(const std::string &Filename, wxXmlNode *pDepNode) {
   if (Filename.empty()) {
      return;
   }
   wxXmlNode *pdnode = NULL;
   // valido que el nodo pasado por parametro sea el nodo dependencias.
   // No se traduce, elemento XML
   if (pDepNode && pDepNode->GetName().Cmp(wxT(DEPENDENCY_NODE)) == 0) {
      pdnode = pDepNode;
   } else {
      pdnode = GetNode(DEPENDENCY_NODE);  // obtengo el nodo dependencias
   }
   if (!pdnode) {
      return;
   }

   // recorro los nodo url hasta encontrar el nodo cuyo contenido sea FileName
   wxXmlNode *psiblingnode = NULL;  // nodo anterior al buscado.
   wxXmlNode *purlnode = GetNode(URL_NODE, pdnode);
   wxString ctt = purlnode->GetNodeContent();
   while (purlnode && ctt.Cmp(Filename) != 0) {
      psiblingnode = purlnode;
      purlnode = purlnode->GetNext();
      ctt = purlnode->GetNodeContent();
   }

   // no encuentro el nodo buscado.
   if (!purlnode) {
      return;
   }

   // enlazo los nodos.
   if (psiblingnode) {
      psiblingnode->SetNext(purlnode->GetNext());
   }
   if (purlnode->GetNext()) {
      purlnode->GetNext()->SetParent(purlnode->GetParent());
   }

   // elimino el nodo.
   pdnode->RemoveChild(purlnode);
   delete purlnode;
}

/**
 * Elimina todos los archivos de la lista de dependencias.
 * Recorre el nodo de dependencias del elemento y para cada una llama a DelDependency
 */
void Element::DelDependency() {
   wxXmlNode *pdnode = GetNode(DEPENDENCY_NODE);
   while (pdnode && pdnode->GetChildren())
      DelDependency(pdnode->GetChildren()->GetNodeContent().c_str(), pdnode);
}

/**
 * Modifica el url de un archivo de la lista de dependencia.
 * @param[in] FileNameOld nombre de la dependencia a modificar
 * @param[in] FileNameNew nombre nuevo de la dependencia
 */
void Element::ModifyDependency(const std::string &FileNameOld, const std::string &FileNameNew) {
   /*TODO(25/03/2009 - Alejandro): Falta trabajo.
    * El problema que tiene este metodo es que si es invocado dde. un bucle del
    * estilo:
    *    while(dependencyurlnode != NULL)
    *       ModifyDependency(...)
    * donde se recorren todas las dependencias(en el orden en que estan) el
    * ciclo se hace infinito ya que se elimina la primer dependencia y se agrega
    * la nueva al final, por lo tanto siempre hay nodos url dentro del nodo
    * dependencia.
    */
   DelDependency(FileNameOld);
   AddDependency(FileNameNew);
}

/**
 * Obtiene, si esta disponible, el valor no valido para el elemento.
 */
void Element::GetNoDataValue(bool& Available, double& NoDataValue) {
   Available = false;
   NoDataValue = 0.0;
   wxXmlNode* pdatanotvalidvalue = this->GetNode(
   PATH_NOT_VALID_DATA_VALUE_NODE_PIPES);
   if (pdatanotvalidvalue) {
      pdatanotvalidvalue = pdatanotvalidvalue->GetChildren();
      if (pdatanotvalidvalue->GetType() == wxXML_TEXT_NODE) {
         wxString ctt = pdatanotvalidvalue->GetContent();
         Available = true;
         NoDataValue = suri::StringToNumber<double>(ctt.mb_str());
      }
   }
}

/** configura en el elemento el valor no valido **/      
void Element::SetNoDataValue(double NoDataValue) {
   /** verifica si ya tenia un valor no valido configurado **/
   wxXmlNode* pdatanotvalidvalue = this->GetNode(PATH_NOT_VALID_DATA_VALUE_NODE_PIPES);
   if (pdatanotvalidvalue){
      RemoveNode(pdatanotvalidvalue->GetParent(), pdatanotvalidvalue);
   }
   AddNode(GetNode(PATH_PROPERTIES_NODE_PIPES), NOT_VALID_DATA_VALUE,
                     NumberToString<double>(NoDataValue));
}

/**
 * Carga nodo en pnode que contiene extremos del subset. El
 * formato es:
 * \verbatim <ul><x>double x</x><y>double y</y></ul>
 * <lr><x>double x</x><y>double y</y></lr> \endverbatim
 * @param[in] Subset Subset que se guardara en nodo;
 * @param[in] pParentNode nodo donde se guardara el subset;
 */
void Element::SetSubset(const Subset &Subset, wxXmlNode *pParentNode) {
   // Agrega nodos dentro de CAPAS con atributo la cantidad de layers
   std::stringstream ss;
   wxXmlNode *pulnode = AddNode(pParentNode, UL_NODE, wxT(""));
   // Cargo UL
   AddNode(pulnode, X_NODE, wxString::Format("%f", Subset.ul_.x_));
   AddNode(pulnode, Y_NODE, wxString::Format("%f", Subset.ul_.y_));
   // Cargo LR
   wxXmlNode *plrnode = AddNode(pParentNode, LR_NODE, wxT(""));
   AddNode(plrnode, X_NODE, wxString::Format("%f", Subset.lr_.x_));
   AddNode(plrnode, Y_NODE, wxString::Format("%f", Subset.lr_.y_));
}

/**
 * Obtiene nodo de pnode con extremos del subset.
 * @param[out] NodeSubset Subset donde se guarda el nodo
 * @param[in] pParentNode node donde esta guardado el subset
 */
void Element::GetSubset(Subset &NodeSubset, wxXmlNode *pParentNode) const {
   bool valid = true;

   Subset subset;

   // ulx
   wxXmlNode *pauxnode = GetNode(wxString::Format("%s%s%s", UL_NODE, NODE_SEPARATION_TOKEN, X_NODE),
                                 pParentNode);
   valid = valid && GetContentAsDouble(subset.ul_.x_, pauxnode);
   // uly
   pauxnode = GetNode(wxString::Format("%s%s%s", UL_NODE, NODE_SEPARATION_TOKEN, Y_NODE),
                      pParentNode);
   valid = valid && GetContentAsDouble(subset.ul_.y_, pauxnode);
   // lrx
   pauxnode = GetNode(wxString::Format("%s%s%s", LR_NODE, NODE_SEPARATION_TOKEN, X_NODE),
                      pParentNode);
   valid = valid && GetContentAsDouble(subset.lr_.x_, pauxnode);
   // lry
   pauxnode = GetNode(wxString::Format("%s%s%s", LR_NODE, NODE_SEPARATION_TOKEN, Y_NODE),
                      pParentNode);
   valid = valid && GetContentAsDouble(subset.lr_.y_, pauxnode);
   // Si fallo alguna de las lecturas no actualiza el nodo
   if (!valid) {
      REPORT_AND_RETURN("No se pudo obtener el subset del nodo");
   }
   NodeSubset = subset;
}

/**
 * Lee un double de un node
 * @param[out] Value valor double leido
 * @param[in] pNode de que contiene un double
 * @return true si pudo leer
 * @return false si no existe el nodo o fallo al convertir contenido a double
 */
bool Element::GetContentAsDouble(double &Value, wxXmlNode *pNode) const {
   if (!pNode) {
      REPORT_DEBUG("D:Error al leer double de nodo");
      return false;
   }
   double aux;
   wxString ulx(pNode->GetNodeContent().c_str());
   if (!ulx.ToDouble(&aux)) {
      REPORT_DEBUG("D:Error al convertir double de nodo");
      return false;
   }
   Value = aux;
   return true;
}

/**
 * Si FileName no tiene esquema (Unix) o el esquema es de una sola
 * letra (Windows) trato de leerlo como un path de sistema operativo
 * @param[in] String con Url
 * @return Url con el formato correcto
 * \TODO(Sebastian - TCK #837): Analizar de pasar este metodo a la clase "Uri"
 */
wxString Element::NormalizeUrl(const wxString& Url) {
   wxString formattedurl = Url;
   Uri uri(Url.c_str());
   if (uri.GetScheme().size() <= 1) {
      wxFileName filename(Url);
      if (filename.HasVolume()) {
         formattedurl = filename.GetVolume();
         formattedurl += filename.GetVolumeSeparator();
         formattedurl += filename.GetFullPath(wxPATH_UNIX);
      } else {
         formattedurl = filename.GetFullPath(wxPATH_UNIX);
      }
   }
   // Hack para formatear correctamente porque wxFileName no maneja correctamente
   // un nombre que contiene : en la ruta.
   if (formattedurl.length() != Url.length()) {
      formattedurl = Url;
//      formattedurl.Replace(GetVolumeSeparator(wxPATH_DOS),GetVolumeSeparator(wxPATH_UNIX));
      formattedurl.Replace(wxT("\\"), wxT("/"));
}
   return formattedurl;
}

}  // namespace suri

