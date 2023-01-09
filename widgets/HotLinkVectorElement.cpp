/** \file HotLinkVectorElement.cpp
 *
 * @file HotLinkVectorElement.h
 *  Software de Procesamiento de Imagenes Satelitales de CONAE.
 *  Copyright 2013-2007 CONAE.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


#include "HotLinkVectorElement.h"

//Includes standard
#include <sstream>

//Includes Suri
#include "suri/Part.h"
#include "suri/Configuration.h"
#include "suri/Vector.h"
#include "suri/xmlnames.h"
#include "suri/FileManagementFunctions.h"
#include "suri/VectorEditor.h"
#include "suri/DataTypes.h"
#include "HotLinkPropertiesPart.h"

//Includes Wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"
#include "wx/ffile.h"
#include "wx/dir.h"

//Includes App
#include "resources.h"
#include "suri/messages.h"


//Inclutdes ORG
#include "ogr_geometry.h"
#include "ogr_feature.h"
#include "ogrsf_frmts.h"

//Defines

//Macros.
//! Obtiene path del dir. temp. donde van los hotlinks.
#define GET_HOTLINKS_TEMP_DIR(Path) do{                                       \
   Path = suri::Configuration::GetParameter("app_temp_dir", wxT("")) +        \
         suri::Configuration::GetParameter("app_hotlinks_dir",wxT("")) +      \
         suri::Configuration::GetParameter("pat_common_dir",wxT("")) +        \
         suri::Configuration::GetParameter("app_hotlinks_dir",wxT(""));}while(0)

// Obtiene path relativo al url pasado.
#define GET_PATH_RELATIVE(Url, NewPath) do{                                   \
   wxFileName fn(Url);                                                        \
   NewPath = fn.GetFullPath(wxPATH_UNIX).c_str();                             \
   if(fn.IsAbsolute()) {                                                      \
      std::string aux = suri::Configuration::GetParameter(                    \
                                    "app_hotlink_base_dir_volatile", wxT(""));\
      fn.MakeRelativeTo(aux);                                                 \
      NewPath = fn.GetFullPath(wxPATH_UNIX); }; } while(0)

AUTO_REGISTER_CLASS(suri::Element, HotLinkVectorElement, 0)

/**
 * Ctor. default.
 */
HotLinkVectorElement::HotLinkVectorElement() {
   // TODO Auto-generated constructor stub
}

/**
 * Destructor.
 */
HotLinkVectorElement::~HotLinkVectorElement() {
   // TODO Auto-generated destructor stub
}

//! retorna un string para el icono
wxString HotLinkVectorElement::DoGetIcon() const {
   if (GetHotLinkFieldValue().IsEmpty()) return VectorElement::DoGetIcon();

   return wxT(icon_HOTLINK_ELEMENT);
}

/**
 * Funcion estatica para crear un HotLinkVectorElement.
 * Realiza una construccion al estilo polimorfismo ya que llama a
 * VectorElement::Create para validar la creacion como Vector y luego
 * realiza las verificaciones para asegurar un Vector con hotlinks.
 *
 * Reemplaza el XML del HotLinkVectorElement con el del VectorElement y luego
 * completa la parte del XML asociada a Hotlink.
 * \pre Si el vector tiene en su base de datos el campo SURHotLink.
 * \pre Debe ser vector de puntos
 * \pre Alternativamente al campo SURHotLink, se acepta que tenga un (1) solo
 *      punto (feature)
 * \post modifica el xml del elemento agregando el nodo Hotlink y el nodo Caracteristicas.
 * @param[in]  UrlElement: url del elemento a crear.
 * @return puntero a un elemento HotLink o NULL en caso de error.
 * \attention quien invoque a este metodo es responsable del puntero devuelto,
 * por lo tanto debe hacerse cargo de su eliminacion.
 */
HotLinkVectorElement* HotLinkVectorElement::Create(const std::string &FileName) {
   HotLinkVectorElement *pelement = new HotLinkVectorElement;
   if (pelement->InitializeClass(FileName) != pelement) {
      delete pelement;
      pelement = NULL;
   }
   return pelement;
}

/**
 * Devuelve las Paginas de propiedades de VectorElement agregando la particular
 * para edicion del campo HotLink.
 * @return vector con Part de Propiedades especificas del HotLinkVectorElement.
 */
std::vector<suri::Part*> HotLinkVectorElement::DoGetParts() {
   std::vector<suri::Part*> vectorparts = suri::VectorElement::DoGetParts();
   wxString fieldname = wxT("");
   if (!GetHotLinkFieldName(0, fieldname).IsEmpty())
      vectorparts.push_back(new HotLinkPropertiesPart(this));
   return vectorparts;
}

/**
 * @return nodo xml con el nodo caracteristicas.
 */
wxXmlNode *HotLinkVectorElement::GetFeaturesNode() {
   return GetNode(wxT(FEATURES_NODE));
}

/**
 * Objetivo: Obtiene los valores del nodo Caracteristicas
 * @param[out] Title: contenido del nodo titulo.
 * @param[out] Description: contenido del nodo descripcion.
 * @param[out] ImgUrl: contenido del nodo url.
 * @param[out] HtmlUrl: contenido del nodo html|url
 * @return obtiene datos? true : false.
 */
bool HotLinkVectorElement::GetFeaturesValues(wxString &Title, wxString &Description,
                                             wxString &ImgUrl, wxString &HtmlUrl) {
   Title = wxT(""), Description = wxT(""), ImgUrl = wxT(""), HtmlUrl = wxT("");

   //obtengo nodo Caracteristicas
   wxXmlNode *pnodefeatures = GetFeaturesNode();
   if (!pnodefeatures)
   REPORT_AND_FAIL_VALUE("D:No se encuentra nodo caracteristicas", false);

   std::string aux = GetHotLinkFieldValue().c_str();
   if (!aux.empty()) {
      wxFileName fn(aux);
      aux = fn.GetVolume().c_str()
            + (fn.HasVolume() ? fn.GetVolumeSeparator() : wxT(""));
      aux += fn.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR, wxPATH_UNIX).c_str();
   }

   if (aux.empty())
      aux = suri::Configuration::GetParameter("app_hotlink_base_dir_volatile", wxT(""));

   pnodefeatures = pnodefeatures->GetChildren();
   while (pnodefeatures) {
      if (pnodefeatures->GetName().Cmp(wxT(TITLE_NODE)) == 0)
         Title = pnodefeatures->GetNodeContent().c_str();

      if (pnodefeatures->GetName().Cmp(wxT(DESCRIPTION_NODE)) == 0) Description =
            pnodefeatures->GetNodeContent().c_str();

      if (pnodefeatures->GetName().Cmp(wxT(URL_NODE)) == 0)
         if (!pnodefeatures->GetNodeContent().IsEmpty())
            ImgUrl = aux
                  + wxFileName(pnodefeatures->GetNodeContent()).GetFullName().c_str();

      if (pnodefeatures->GetName().Cmp(wxT(HTML_NODE)) == 0) {
         //obtengo el nodo url del nodo html
         wxXmlNode *pchild = pnodefeatures->GetChildren();
         if (pchild && !pchild->GetNodeContent().IsEmpty())
            HtmlUrl = aux + wxFileName(pchild->GetNodeContent()).GetFullName().c_str();
      }

      pnodefeatures = pnodefeatures->GetNext();
   }
   return true;
}

/**
 * Agrega el nodo caracteristicas al xml del elemento.
 * @param[in]  pFeatureNode: puntero al nodo Caracteristicas.
 */
void HotLinkVectorElement::SetFeaturesNode(wxXmlNode *pFeatureNode) {
   if (!pFeatureNode)
   REPORT_AND_FAIL("D:Nodo caracteristicas vacio");

   //hago relativo el url de la imagen
   std::string newpath;
   wxXmlNode *pnodeurl = GetNode(wxT(URL_NODE), pFeatureNode);
   if (pnodeurl && !pnodeurl->GetNodeContent().IsEmpty()) {
      GET_PATH_RELATIVE(pnodeurl->GetNodeContent(), newpath);
      pnodeurl->GetChildren()->SetContent(newpath);
   }

   //hago relativo el url del html
   std::string pathnode = wxT(HTML_NODE);
   pathnode += wxT("|");
   pathnode += wxT(URL_NODE);
   pnodeurl = GetNode(pathnode, pFeatureNode);
   if (pnodeurl && !pnodeurl->GetNodeContent().IsEmpty()) {
      GET_PATH_RELATIVE(pnodeurl->GetNodeContent(), newpath);
      pnodeurl->GetChildren()->SetContent(newpath);
   }

   AddNode(GetNode(wxT("")), pFeatureNode);  //dejar con Replace=True
}

/**
 * Agrega el nodo caracteristicas al xml del elemento.
 * @param[in]  Title: contenido del nodo titulo.
 * @param[in]  Description: contenido del nodo descripcion
 * @param[in]  ImgUrl: url de la imagen. Contenido del nodo url.(siempre debe ser relativo al dir. del hotlink)
 * @param[in]  HtmlUrl: url del Html generado. (siempre debe ser relativo al dir. del hotlink)
 */
void HotLinkVectorElement::SetFeaturesNode(const wxString &Title,
                                           const wxString &Description,
                                           const wxString &ImgUrl,
                                           const wxString &HtmlUrl) {
   wxXmlNode *pnodefeatures = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, FEATURES_NODE);
   AddNode(pnodefeatures, TITLE_NODE, Title.c_str());
   AddNode(pnodefeatures, DESCRIPTION_NODE, Description.c_str());
   AddNode(pnodefeatures, URL_NODE, ImgUrl.c_str());
   wxXmlNode *pnodehtml = AddNode(pnodefeatures, HTML_NODE);
   AddNode(pnodehtml, URL_NODE, HtmlUrl.c_str());
   SetFeaturesNode(pnodefeatures);
}

/**
 * Devuelve el nombre del campo que contiene el url del hotlink
 * @param[in]  Layer: indice de la capa del vector.
 * @param[out] FieldName: nombre del campo en la base de datos del vector.
 * @return string con nombre del campo en la base de datos del vector.
 */
wxString HotLinkVectorElement::GetHotLinkFieldName(const int Layer,
                                                   wxString &FieldName) {
   FieldName = wxT("");
   //obtengo el nodo capas
   wxXmlNode *pnode = GetNode(wxT(LAYERS_NODE));
   //obtengo el nodo de la capa buscada
   pnode = GetLayerNode(Layer, pnode);
   if (!pnode)
   REPORT_AND_FAIL_VALUE("D:No se encuentra la capa: %d", wxT(""), Layer);

   //busco nodo hotlink
   pnode = GetNode(wxT(HOTLINK_NODE), pnode);

   if (!pnode)
   REPORT_AND_FAIL_VALUE("D:No se encuentra el nodo hotlink", wxT(""));

   return FieldName = pnode->GetNodeContent().c_str();
}

/**
 * Setea el nombre del campo que contiene los url de los hotlinks
 * @param[in]  Layer: indice de la capa del vector.
 * @param[in]  FieldName: nombre del campo a setear en la base de datos del vector.
 */
void HotLinkVectorElement::SetHotLinkFieldName(const int Layer,
                                               const wxString &FieldName) {
   //obtengo el nodo capas
   wxXmlNode *playersnode = GetNode(wxT(LAYERS_NODE));
   //obtengo el nodo de la capa buscada
   wxXmlNode *playernode = GetLayerNode(Layer, playersnode);
   //Agrego el nodo HotLink.
   AddNode(playernode, wxT(HOTLINK_NODE), FieldName);
}

/**
 * Setea el nombre del campo que contiene los url de los hotlinks
 * @param[in]  FieldName: nombre del campo a setear en la base de datos del vector.
 */
void HotLinkVectorElement::SetHotLinkFieldName(const wxString &FieldName) {
   SetHotLinkFieldName(0, FieldName);
}

/**
 * Agrega una dependencia al hotlink.
 * Agrega un nodo url al nodo dependencias del elemento.
 * @param[in]  Url: url de la dependencia(absoluto).
 */
void HotLinkVectorElement::SetDependencies(const std::string &Url) {
   AddDependency(Url);
}

/**
 * Obtiene el contenido del campo SurHotLink del dbf, es decir, obtiene el url
 * del html que conforma el hotlink.
 * @return string con el contenido del campo SurHotLink del dbf
 */
wxString HotLinkVectorElement::GetHotLinkFieldValue() const {
   suri::Vector *pvector = suri::Vector::Open(GetUrl().c_str());

   /* Si no hay vector, o no hay layers o no hay ningun feature, no no sirve y cerramos. */
   if (!pvector || !pvector->GetLayer(0) || pvector->GetLayer(0)->GetFeatureCount() < 1)
      return wxT("");
   // inicializo la busqueda
   pvector->GetLayer(0)->ResetReading();
   // obtengo el ultimo feature
   OGRFeature *pnextfeat = pvector->GetLayer(0)->GetNextFeature();
   OGRFeature *pfeat = NULL;
   while (pnextfeat != NULL) {
      OGRFeature::DestroyFeature(pfeat);
      pfeat = pnextfeat;
      pnextfeat = pvector->GetLayer(0)->GetNextFeature();
   }

   if (!pfeat) return wxT("");

   wxString value = pfeat->GetFieldAsString(SUR_HOTLINK_FIELD);
   OGRFeature::DestroyFeature(pfeat);
   suri::Vector::Close(pvector);
   return value;
}

/**
 * Establece el contenido del campo SurHotLink del dbf. Guarda el url del html
 * que conforma el hotlink.
 * @param[in]  Value: string a setear en el campo SurHotLink
 */
void HotLinkVectorElement::SetHotLinkFieldValue(const wxString &Value) {
   // si el url a setear es absoluto y no existe no lo seteo.
#ifdef __WINDOWS__
   std::string value =
         wxString(wxConvUTF8.cMB2WC(Value.c_str()),
                  wxCSConv(suri::XmlElement::xmlEncoding_.c_str())).c_str();
   if (wxFileName(value).IsAbsolute() && !wxFileName::FileExists(value))
   REPORT_AND_FAIL("D:No existe el archivo que se intenta setear al vector.");
#else
   if (wxFileName(Value).IsAbsolute() && !wxFileName::FileExists(Value))
   REPORT_AND_FAIL("D:No existe el archivo que se intenta setear al vector.");
#endif  // __WINDOWS__

   suri::Vector *pvector = suri::Vector::Open(GetUrl().c_str(),
                                              suri::Vector::ReadWrite);
   if (!pvector || !pvector->GetLayer(0) || pvector->GetLayer(0)->GetFeatureCount() < 1)
   REPORT_AND_FAIL("D:Error al intentar acceder a la capa vectorial");

   // inicializo la busqueda
   pvector->GetLayer(0)->ResetReading();
   // obtengo el ultimo feature
   OGRFeature *pnextfeat = pvector->GetLayer(0)->GetNextFeature();
   OGRFeature *pfeat = NULL;
   while (pnextfeat != NULL) {
      OGRFeature::DestroyFeature(pfeat);
      pfeat = pnextfeat;
      pnextfeat = pvector->GetLayer(0)->GetNextFeature();
   }
   // no tengo elemento
   if (pfeat) {
#ifdef __WINDOWS__
      pfeat->SetField(SUR_HOTLINK_FIELD, value.c_str());
#else
      pfeat->SetField(SUR_HOTLINK_FIELD, Value.c_str());
#endif  // __WINDOWS__
      pvector->GetLayer(0)->SetFeature(pfeat);
      pvector->GetLayer(0)->SyncToDisk();
   }
   suri::Vector::Close(pvector);
}

/**
 * Objetivo: Copia el vector(todo el directorio) a la carpeta temporal de la app.
 * Tambien modifica el valor del campo SURHotLink con el nuevo url del HotLink,
 * y modifica el nodo url del vector con la nueva ubicacion del elemento.
 * @param[in]  UrlHotLink: url del HotLink.
 * @return modifica_ok? true : false;
 */
bool HotLinkVectorElement::CopyVectorToTemp(const std::string &UrlHotLink) {
   if (UrlHotLink.empty()) return false;

   std::string pathtempaux = suri::Configuration::GetParameter(
         "app_vector_base_dir_volatile", wxT(""));
   pathtempaux = suri::GetNextPath(pathtempaux);
   if (pathtempaux.empty()) return false;

   wxFileName fnfrom(wxFileName(GetUrl()).GetPath() + wxFileName::GetPathSeparator());
   wxFileName fnto(pathtempaux + wxFileName::GetPathSeparator());
   //copio el dir. del vector al dir. temp. de la app.

   if (suri::CopyDir(fnfrom, fnto)) {
      wxString newpathvector = fnto.GetVolume().c_str()
            + (fnto.HasVolume() ? fnto.GetVolumeSeparator() : wxT(""));
      newpathvector += fnto.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR,
                                    wxPATH_UNIX);
      //Modifico los nodo url del vector con la nueva ubicacion.
      ModifyUrls(newpathvector.c_str());
   } else
      REPORT_AND_FAIL_VALUE("D:Error al copiar vector a directorio temporal.", false);
   return true;
}

/**
 * Modifico el url del elemento y de las dependencias, con el nuevo Path pasado
 * por parametro.
 * @param[in]  BasePath: path con la nueva ubicacion de los vectores.
 */
void HotLinkVectorElement::ModifyUrls(const std::string &BasePath) {
   if (BasePath.empty()) return;

   //modifico el url del elemento.
   std::string urlelement = BasePath + wxFileName(GetUrl()).GetFullName().c_str();
   SetUrl(urlelement);

   //modifico url de las dependencias.
   wxXmlNode *pdnode = GetNode(wxT(DEPENDENCY_NODE));  //No se traduce, es elemento de XML
   wxXmlNode *purlnode = NULL;
   wxArrayString deps;
   if (pdnode) purlnode = pdnode->GetChildren();
   while (purlnode) {
      wxFileName fn(purlnode->GetNodeContent());
      deps.Add(BasePath + fn.GetFullName());
      purlnode = purlnode->GetNext();
   }

   //elimino todas las dependencias y las vuelvo a cargar con los nuevos url.
   DelDependency();
   for (size_t i = 0; i < deps.Count(); i++)
      AddDependency(deps.Item(i).c_str());
}

HotLinkVectorElement* HotLinkVectorElement::InitializeClass(
      const std::string &FileName) {
   // uso un elemento vector para que me filtre el URL
   suri::VectorElement *ptempelem = suri::VectorElement::Create(FileName.c_str());
   // si no se puede crear un vector, entonces asumo que no puede ser un vector con HotLink
   if (!ptempelem) return NULL;
   wxString filename = FileName;
   filename = ptempelem->GetUrl();

   suri::Vector *pvector = suri::Vector::Open(filename.c_str(),
                                              suri::Vector::ReadWrite);

   if (!pvector || pvector->GetLayerCount() <= 0) {
      suri::Vector::Close(pvector);
      return NULL;
   }

   //creo el HotLinkVectorElement y reemplazo el nodo con el VectorElement.
   AddNode(NULL, ptempelem->GetProperties());  //dejar replace true
   delete ptempelem;

   //TODO: recorrer todas las capas de pvector
   OGRLayer *player = pvector->GetLayer(0);
   if (!player
         || (player->GetLayerDefn() && player->GetLayerDefn()->GetGeomType() != wkbPoint)) {
      suri::Vector::Close(pvector);
      return NULL;
   }

   //Verifico si la capa tiene un campo SUR_HOTLINK_FIELD
   bool found = false;
   if (player->GetLayerDefn()->GetFieldIndex(SUR_HOTLINK_FIELD) >= 0) found = true;

   /* si tiene campo SURHotlink, inserto el nodo "hotlink" en el xml.
    * y el nodo caracteristicas
    * Alternativamente, si tiene 1 solo punto, tambien lo acepto */
   if (found || player->GetFeatureCount() == 1) {
      SetHotLinkFieldName(SUR_HOTLINK_FIELD);
      SetFeaturesNode(_("Titulo"), _("Descripcion"),  ///no se traduce, elemento de XML
                      wxT(""), wxT(""));
   } else {
      suri::Vector::Close(pvector);
      return NULL;
   }

   //Cierra el vector
   suri::Vector::Close(pvector);
   return this;
}

