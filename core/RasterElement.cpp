/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

// Includes Estandar
#include <vector>
#include <string>
#include <map>

// Includes suri
#include "suri/RasterElement.h"
#include "suri/Uri.h"
#include "suri/Image.h"
#include "suri/messages.h"
#include "suri/xmlnames.h"
#include "suri/DataTypes.h"
#include "suri/Configuration.h"
#include "suri/LayerInterface.h"
#include "RasterLayer.h"
#include "suri/RasterSpatialModel.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Dimension.h"
#include "suri/DatasourceInterface.h"
#include "suri/DatasourceManagerInterface.h"
#include "BandCombinationAndEnhancementPart.h"
#include "MetadataPropertiesPart.h"
#include "RasterRenderer.h"
#include "RasterProperties.h"
#include "ClassEditionPart.h"
#include "ClassifiedRasterDatasourceManipulator.h"
#include "suri/DataViewManager.h"
#include "suri/HistogramVisualizationPart.h"
#include "suri/ManualEnhancementPart.h"

// Includes App
#include "resources.h"

// <Includes Wx
#include "wx/sstream.h"

// Defines
#define ELEMENT_DEFAULT_TYPE              "Exact"
#define ELEMENT_DEFAULT_ALGORITHM         ""
#define ELEMENT_DEFAULT_ALGORITHM_ORDER   "1"
#define ELEMENT_DEFAULT_ALGORITHM_DELTA   "10"

/** namespace suri */
namespace suri {

/**
 * Macro para registrar el tipo de elemento
 */
AUTO_REGISTER_CLASS(Element, RasterElement, 0)

/**
 * Constructor
 */
RasterElement::RasterElement() :
      Element(wxT(TYPE_PROPERTY_VALUE_RASTER)) {
}

/**
 * Destructor
 */
RasterElement::~RasterElement() {
}

/**
 * Retorna un string para el icono
 */
wxString RasterElement::DoGetIcon() const {
   return icon_RASTER_ELEMENT;
}

/**
 * Crea una previsualizacion del elemento
 */
void RasterElement::GetPreview(wxBitmap &Preview, int Width, int Height) const {
   GET_BITMAP_RESOURCE(icon_PREVIEW_RASTER_ELEMENT, Preview);
   ESCALE_BITMAP(Preview, Width, Height)
}

/**
 * Retorna los detalles del elemento
 */
wxString RasterElement::GetDetails() const {
   return wxT("");
}

/** Funcion estatica para crear un RasterElement */
/**
 * Crea RasterElement y le carga la URL. Crea y abre la imagen de FileName
 * y setea la Referencia espacial y el raster model.
 * Obtiene nodo de renderizacion de la imagen y le agrega las bandas.
 * Llama a funciones Get para que se guarden en
 * nodo dimension el extent y el rastersize.
 * Setea las propiedades del raster
 * Agrega los nodos de brillo, contraste, interpolacion.
 * Agrega los nodos dependencias (archivos asociados a un dataset).T
 * @param[in] FileName ruta al raster con el que debe crearse el raster
 * @param[in] Metadata mapa con metadatos en formato clave-valor
 * \attention el RastereElement retornado(y su eliminacion) son responsabilidad del invocante
 */
RasterElement *RasterElement::Create(const std::string &FileName,
                                     const Option& Metadata) {
   RasterElement *pelement = dynamic_cast<RasterElement*>(Element::Create(
         "RasterElement", FileName, Metadata));
   if (pelement && !pelement->Initialize(FileName, Metadata)) {
      // si initialize falla, retorna NULL, y nadie informa error hacia arriba
      Element *pelem = pelement;
      suri::Element::Destroy(pelem);
      pelement = NULL;
   }
   return pelement;
}

RasterElement* RasterElement::Clone() const {
   std::string url = this->GetUrl().mb_str();
   return Create(url, this->metadata_);
}


/**
 * Seteo la matriz del modelo de raster
 * @param[in] Wkt texto que representa la referencia espacial del raster
 * @return informa si pudo configurar la georreferencia
 */
bool RasterElement::SetRasterModel(const std::string &Wkt) {
   wxXmlNode *pnode = GetNode(GEORREFERENCE_NODE);
   if (!pnode) {
      pnode = AddNode(GetNode(wxT("")), GEORREFERENCE_NODE);
   }
   return AddNode(pnode, RASTER_SPATIAL_MODEL_NODE, Wkt, wxT(""), wxT(""), true) != NULL;
}

/**
 * Busca la georreferencia en nodo del elemento
 * @return georreferencia del raster
 */
std::string RasterElement::GetRasterModel() const {
   wxXmlNode *pnode = GetNode(
         wxString::Format("%s%s%s", GEORREFERENCE_NODE, NODE_SEPARATION_TOKEN,
         RASTER_SPATIAL_MODEL_NODE));
   if (pnode) {
      std::string content = pnode->GetNodeContent().c_str();
      return content;
   }
   return "";
}

/**
 * Obtengo el tipo de dato de la imagen
 * @return estructura con las propiedades de la imagen
 */
RasterElement::RasterPropertiesType RasterElement::GetRasterProperties() {
   wxString path = FILE_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += FORMAT_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += PROPERTIES_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += DATA_TYPE_NODE;
   wxXmlNode *pfileprops = GetNode(path);
   if (!pfileprops) {
      REPORT_AND_FAIL_VALUE("D:Error al actualizar las propiedades raster",
                            RasterPropertiesType());
   }
   RasterPropertiesType rasterprops;
   if (pfileprops->GetChildren()) {
      rasterprops.DataType = pfileprops->GetChildren()->GetContent();
   }
   return rasterprops;
}

/**
 * Cargo las propiedades de archivo
 * @param[in] Properties nuevas propiedades del raster
 */
void RasterElement::SetRasterProperties(const RasterPropertiesType &Properties) {
   wxString path = FILE_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += FORMAT_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += PROPERTIES_NODE;
   wxXmlNode *pfileprops = GetNode(path);
   if (pfileprops) {
      AddNode(pfileprops, wxT(DATA_TYPE_NODE), Properties.DataType);
   } else {
      REPORT_AND_FAIL("D:Error al actualizar las propiedades raster");
   }
}

/**
 * Devuelve ventanas de propiedades de los elementos Raster.
 * @return vector con punteros a los Part generados.\n
 * NOTA: en caso de tener que agregar Part, RECORDAR que GetParts()devuelve 2
 * Part(1ro Comun a todos los elementos, 2do Para editar xml(solo en DEBUG))
 * ==> los Part a agregar deben agregarse entre medio de dichos Part's.
 * @return vector de parts con propiedades del Raster.
 */
std::vector<Part *> RasterElement::DoGetParts(DataViewManager* pDataViewManager,
                                             LayerInterface *pLayer,
                                             DatasourceInterface *pDatasource) {
   std::vector<Part*> partvector;
   if (!pDataViewManager || !pDatasource || !pLayer) {
      // partvector.push_back(new BandCombinationAndEnhancementPart(NULL, this));
      // partvector.push_back(new RasterProperties(this));
   } else {
      RasterDatasource* prasterdatasource = dynamic_cast<RasterDatasource*>(pDatasource);
      if (prasterdatasource
            && (Configuration::GetParameter("app_short_name", "") == wxT("SoPI"))) {
         partvector.push_back(new RasterProperties(prasterdatasource, pLayer, pDataViewManager));
         partvector.push_back(new MetadataPropertiesPart(prasterdatasource));
         ui::HistogramVisualizationPart* phistpart =
            new ui::HistogramVisualizationPart(dynamic_cast<RasterLayer*>(pLayer));
         partvector.push_back(phistpart);
      }
   }
   return partvector;
}

/** Devuelve vector con Parts especificos de la visualizacion elemento. */
std::vector<suri::Part*> RasterElement::DoGetVisualizationParts(
      DataViewManager* pDataViewManager, LayerInterface *pLayer,
      DatasourceInterface *pDatasource) {
   std::vector<Part*> partvector;
   if (Configuration::GetParameter("app_short_name", "") != wxT("SoPI")) {
      return partvector;
   }
   RasterLayer *prasterlayer = dynamic_cast<RasterLayer*>(pLayer);
   if (prasterlayer) {
      RasterDatasource* prasterdatasource = dynamic_cast<RasterDatasource*>(pDatasource);
      ClassifiedRasterDatasourceValidator validator =
            ClassifiedRasterDatasourceValidator();
      if (prasterdatasource != NULL && validator.IsValid(prasterdatasource)) {
         partvector.push_back(new ClassEditionPart(prasterdatasource, pLayer));
      } else {
         partvector.push_back(
               new BandCombinationAndEnhancementPart(
                     pDataViewManager->GetLibraryManager(), prasterlayer));
      }
      partvector.push_back(new ui::ManualEnhancementPart(prasterlayer));
   }
   return partvector;
}

/**
 * Devuelve el valor del nodo archivo|tipo. Busca en el nodo del elemento, el
 * tipo de dato que se usa para guardar el raster.
 * @return   cadena con el tipo de elemento en caso de exito.
 * @return   "N/A" en caso de que no se encuentre el nodo archivo.
 */
wxString RasterElement::GetTypeAsString() {
   wxXmlNode *pnode = GetFileNode();
   if (!pnode) {
      return _(message_NOT_APPLICABLE);
   }

   wxXmlNode *pchild = pnode->GetChildren();
   while (pchild && pchild->GetName().Cmp(wxT(FORMAT_NODE)) != 0)
      pchild = pchild->GetNext();

   pchild = pchild->GetChildren();
   while (pchild && pchild->GetName().Cmp(wxT(TYPE_NODE)) != 0)
      pchild = pchild->GetNext();

   if (!pchild) {
      return _(message_NOT_APPLICABLE);
   }

   return pchild->GetNodeContent().c_str();
}

/**
 * Devuelve el contenido del nodo <archivo|formato|descripcion>
 * @return el contenido del nodo <archivo|formato|descripcion> en caso de que
 * exista dicho nodo.
 * @return "N/A" en caso de que dicho nodo no exista.
 */
wxString RasterElement::GetFormatDescription() {
   // obtengo el nodo archivo|formato|descripcion.
   wxString formatnodepath = wxString::Format("%s|%s|%s", FILE_NODE, FORMAT_NODE,
                                              DESCRIPTION_NODE);
   wxXmlNode *pnode = GetNode(formatnodepath);
   if (!pnode) {
      return _(message_NOT_APPLICABLE);
   }

   return _(pnode->GetNodeContent().c_str());
}

/**
 * Busca el tamano del raster en el nodo. Si no lo encuentra lo busca
 * en el archivo y actualiza el nodo. Si esto tambien falla retorna error
 * @param[out] Width tamano imagen en x en pixeles
 * @param[out] Height tamano imagen en y en pixeles
 */
void RasterElement::GetRasterSize(int &Width, int &Height) {
   wxXmlNode *pnode = GetNode(
         wxString::Format("%s%s%s", DIMENSION_NODE, NODE_SEPARATION_TOKEN,
                          RASTER_NODE));
   if (!pnode) {
      Image::ImageAccessType access = Image::ReadOnly;
      std::string writer = "null";
      std::string dataname = "void";
      int bandcount = 0, sizex = 0, sizey = 0;
      std::string pixels = metadata_.GetOption("Pixels");
      if (!pixels.empty()) {
         sizex = StringToNumber<int>(pixels);
      }
      std::string lines = metadata_.GetOption("Lines");
      if (!lines.empty()) {
         sizey = StringToNumber<int>(lines);
      }
      dataname =
            !metadata_.GetOption("Datatype").empty() ? metadata_.GetOption("Datatype") :
                                                       dataname;

      std::string bstr = metadata_.GetOption("Bandcount");
      if (!bstr.empty()) {
         bandcount = StringToNumber<int>(bstr);
      }

      Image *pimage = Image::Open(RasterRenderer::GenerateImageId(GetUrl().c_str()),
                                  access, writer, bandcount, sizex, sizey, dataname,
                                  metadata_);
      if (!pimage) {
         REPORT_AND_FAIL("Error al encontrar el subset del raster");
         return;
      }
      // Cargo nodo con size del raster
      SetRasterSize(pimage->GetSizeX(), pimage->GetSizeY());
      Width = pimage->GetSizeX();
      Height = pimage->GetSizeY();
      Image::Close(pimage);
   } else {
      Subset worldextent;
      GetSubset(worldextent, pnode);
      Width = static_cast<int>(worldextent.lr_.x_);
      Height = static_cast<int>(worldextent.lr_.y_);
   }
}

/**
 * Devuelve el extent del raster guardado en el nodo. Si no existe crea el nodo
 * usando la imagen.
 * @param[out] ElementExtent subset que contiene extent de raster
 */
void RasterElement::GetElementExtent(Subset &ElementExtent) {
   wxXmlNode *pnode = GetNode(
         wxString::Format("%s%s%s", DIMENSION_NODE, NODE_SEPARATION_TOKEN,
                          EXTENT_NODE));
   if (!pnode) {
      int width, height;
      GetRasterSize(width, height);
      // Cargo nodo con extent del raster
      RasterSpatialModel *prastermodel = RasterSpatialModel::Create(GetRasterModel());
      if (!prastermodel) {
         REPORT_DEBUG("D:No se pudo crear RasterSpatialModel");
         return;
      }
      Coordinates ul(0, 0), lr(static_cast<double>(width), static_cast<double>(height));
      prastermodel->Transform(ul);
      prastermodel->Transform(lr);
      Subset rasterextent(ul, lr);
      SetElementExtent(Subset(ul, lr));
      ElementExtent.ul_ = ul;
      ElementExtent.lr_ = lr;
      RasterSpatialModel::Destroy(prastermodel);
   } else {
      GetSubset(ElementExtent, pnode);
   }
}

/**
 * Carga las propiedades de las bandas de la imagen
 * Obtiene la cantidad de bandas de la imagen y agrega el nodo bandas
 * \todo por ahora define el nombre como "Banda X", deberia obtener el nombre
 * de la banda.
 * @param[in] pImage : imagen.
 */
void RasterElement::InitializeBandProperties(Image *pImage) {
   if (!pImage) {
      return;
   }

   int bandcount = 0;

   // Agrega la cantidad de bandas de la imagen
   bandcount = pImage->GetBandCount();
   SetBandCount(bandcount);

   char bandname[30];
   // Asigna el nombre de la banda
   for (int bandnumber = 0; bandnumber < bandcount; bandnumber++) {
      snprintf(bandname, sizeof(bandname), BAND_NAME_PREFIX_d, bandnumber + 1);
      SetBandName(bandname, bandnumber);
      SetBandValidity(1, bandnumber);
   }
}

/**
 * Carga la cantidad de bandas
 * Agrega el nodo bandas con una propiedad con la cantidad de bandas
 * @param[in] BandCount : cantidad de bandas de la imagen
 */
void RasterElement::SetBandCount(int BandCount) {
   wxString path = FILE_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += FORMAT_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += PROPERTIES_NODE;
   wxXmlNode *pbandnode = GetNode(path);

   if (!pbandnode) {
      pbandnode = AddNode(GetNode(""), path);
   }

   AddNode(pbandnode, BANDS_NODE, wxT(""), QUANTITY_PROPERTY,
           wxString::Format("%d", BandCount));
}

/**
 * Establece la cantidad de bandas sin modificar el contenido.
 * Solo actualiza la propiedad del elemento "bandas"
 */
void RasterElement::SetBandCountEx(int BandCount) {
   wxString path = FILE_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += FORMAT_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += PROPERTIES_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += BANDS_NODE;

   wxXmlNode* pbandsnode = GetNode(path);

   if (pbandsnode) {
      wxXmlProperty* pprops = pbandsnode->GetProperties();
      while (pprops) {
         if (pprops->GetName().Cmp(QUANTITY_PROPERTY) == 0) {
            pprops->SetValue(wxString::Format("%d", BandCount));
            break;
         }
         pprops = pprops->GetNext();
      }
   }
}

/**
 * Carga el nombre de una banda
 * @param[in] BandName : nombre de la banda
 * @param[in] BandNumber : indice de la banda
 * @return informa si pudo configurar la banda
 * \todo hacer que si existe la banda reemplaze el nombre
 */
bool RasterElement::SetBandName(const std::string &BandName, int BandNumber) {
   if (BandNumber > GetBandCount()) {
      return false;
   }

   wxString path = FILE_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += FORMAT_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += PROPERTIES_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += BANDS_NODE;

   wxXmlNode *pbandnode = GetNode(path);
   wxXmlNode *pnamenode;

   if (pbandnode) {
      wxString bandindex;
      long int auxband;

      pnamenode = pbandnode->GetChildren();

      // Busco si existe el nodo con ese indice de banda
      while (pnamenode) {
         pnamenode->GetPropVal(BAND_INDEX_PROPERTY, &bandindex);
         bandindex.ToLong(&auxband);
         if (BandNumber != static_cast<int>(auxband)) {
            pnamenode = pnamenode->GetNext();
         } else {
            // Si encuentra el nodo le reemplaza el nombre
            AddNode(pnamenode, BAND_NAME, wxT(BandName), wxT(""), wxT(""), true);
            break;
         }
      }
      // Si sale porque el nodo no existe, agrega el nodo banda con indice, y el nomobre
      if (!pnamenode) {
         pnamenode = AddNode(pbandnode, BAND_NODE, wxT(""), BAND_INDEX_PROPERTY,
                             wxString::Format("%d", BandNumber), false);
         AddNode(pnamenode, BAND_NAME, wxT(BandName));
      }
   }
   return true;
}
/**
 * Devuelve la cantidad de bandas de la imagen
 * @return cantidad de bandas de la imagen
 * @return cero si no encuentra el nodo
 */
int RasterElement::GetBandCount() const {
   int bandcount = 0;

   wxString path = FILE_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += FORMAT_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += PROPERTIES_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += BANDS_NODE;

   wxXmlNode *pbandnode = GetNode(path);
   if (pbandnode) {
      wxString auxbandcount;
      long int auxband;
      if (pbandnode->GetPropVal(QUANTITY_PROPERTY, &auxbandcount)) {
         auxbandcount.ToLong(&auxband);
         bandcount = static_cast<int>(auxband);
      }
   }

   return bandcount;
}

/**
 * Devuelve el nombre de la banda, para el indice de banda recibido
 * @param[in] BandNumber : numero de banda.
 * @param[out] BandName: nombre de la banda, si no tiene nombre devuelve nombre default.
 * @return true si encuentra la banda
 * @return false si el indice de banda es mayor a la cantidad
 */
bool RasterElement::GetBandName(std::string &BandName, int BandNumber) const {
   // Si el indice es mayor que la cantidad de bandas devuelve false
   if (BandNumber >= GetBandCount()) {
      return false;
   }

   wxString path = FILE_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += FORMAT_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += PROPERTIES_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += BANDS_NODE;

   wxXmlNode *pnode = GetNode(path);
   wxXmlNode *pbandnode = NULL;
   wxString auxbandindex;
   long int auxbandint;

   // Si encontro el nodo bandas
   if (pnode) {
      pbandnode = pnode->GetChildren();
      // Recorre el nodo nombre comparando el indice de banda
      while (pbandnode) {
         pbandnode->GetPropVal(BAND_INDEX_PROPERTY, &auxbandindex);
         auxbandindex.ToLong(&auxbandint);
         // Si no es el indice, trae el siguiente nodo
         if (BandNumber != static_cast<int>(auxbandint)) {
            pbandnode = pbandnode->GetNext();
            // Si es el indice obtiene el nombre de la banda
         } else {
            BandName = pbandnode->GetChildren()->GetNodeContent().c_str();
            break;
         }
      }
   }

   // Si no encontro el nodo bandas, o no encontro la banda con el indice buscado
   // o encontro la banda pero el nombre esta vacio, devuelve el nombre default
   if (!pnode || !pbandnode || (pbandnode && BandName.empty())) {
      std::ostringstream auxname;
      auxname << "Banda " << BandNumber;
      BandName = auxname.str();
   }
   return true;
}

/**
 * Devuelve las resolucion del pixel.
 */
double RasterElement::GetPixelRes() const {
   double retval = 0.0;

   RasterSpatialModel::Parameters params = RasterSpatialModel::WktToParameters(
         GetRasterModel());

   if (params.pixelSizes_.size() == 2)
      retval = params.pixelSizes_[0] * params.pixelSizes_[1];

   return retval;
}

/**
 * Guarda el tamano del raster en nodo raster dentro de
 * dimension con formato subset
 * @param[in] Width tamano imagen en x en pixeles
 * @param[in] Height tamano imagen en y en pixeles
 */
void RasterElement::SetRasterSize(int Width, int Height) {
   Subset imagesize(0, 0, Width, Height);
   wxXmlNode *pnode = GetNode(wxString::Format("%s", DIMENSION_NODE));
   if (!pnode) {
      pnode = AddNode(GetNode(""), DIMENSION_NODE);
   }
   wxXmlNode *prnode = GetNode(wxString::Format("%s", RASTER_NODE));
   if (!prnode) {
      prnode = AddNode(pnode, RASTER_NODE);
   }

   SetSubset(imagesize, prnode);
}

/**
 * Devuelve el valor de validez de la banda.
 */
bool RasterElement::GetBandValidity(int& BandValidity, int BandNumber) const {
   // Si el indice es mayor que la cantidad de bandas devuelve false
   if (BandNumber >= GetBandCount()) {
      return false;
   }

   wxString path = FILE_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += FORMAT_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += PROPERTIES_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += BANDS_NODE;

   wxXmlNode *pnode = GetNode(path);
   wxXmlNode *pbandnode = NULL;
   wxString auxbuffer;
   long auxvalue = 0;

   if (pnode) {
      pbandnode = pnode->GetChildren();
      while (pbandnode) {
         pbandnode->GetPropVal(BAND_INDEX_PROPERTY, &auxbuffer);
         auxbuffer.ToLong(&auxvalue);
         if (BandNumber != static_cast<int>(auxvalue)) {
            pbandnode = pbandnode->GetNext();
         } else {
            pbandnode->GetPropVal(BAND_VALIDITY_PROPERTY, &auxbuffer);
            auxbuffer.ToLong(&auxvalue);
            BandValidity = static_cast<int>(auxvalue);
            break;
         }
      }
   }

   return true;
}

/**
 * Establece la validez de una banda.
 */
bool RasterElement::SetBandValidity(int BandValidity, int BandNumber) {
   if (BandNumber > GetBandCount()) {
      return false;
   }

   wxString path = FILE_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += FORMAT_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += PROPERTIES_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += BANDS_NODE;

   wxXmlNode *pbandnode = GetNode(path);
   wxXmlNode *pnamenode = NULL;
   wxXmlProperty* pprops = NULL;

   if (pbandnode) {
      wxString bandindex;
      long int auxband;

      pnamenode = pbandnode->GetChildren();

      // Busco si existe el nodo con ese indice de banda
      while (pnamenode) {
         pnamenode->GetPropVal(BAND_INDEX_PROPERTY, &bandindex);
         bandindex.ToLong(&auxband);
         if (BandNumber != static_cast<int>(auxband)) {
            pnamenode = pnamenode->GetNext();
         } else {
            // Si encuentra el nodo le reemplaza el valor
            pprops = pnamenode->GetProperties();
            while (pprops != NULL) {
               if (pprops->GetName().Cmp(wxT(BAND_VALIDITY_PROPERTY)) == 0) {
                  pprops->SetValue(wxString::Format("%d", BandValidity));
                  break;
               }
               pprops = pprops->GetNext();
            }
            break;
         }
      }
      // no encontro la propiedad asi que la agrega
      if (pnamenode && !pprops) {
         pnamenode->AddProperty(BAND_VALIDITY_PROPERTY, wxString::Format("%d", BandValidity));
      }
   }
   return true;
}

/**
 * Borra la banda especificada (del XML)
 */
bool RasterElement::DeleteBand(int BandNumber) {
   if (BandNumber > GetBandCount()) {
      return false;
   }

   wxString path = FILE_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += FORMAT_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += PROPERTIES_NODE;
   path += NODE_SEPARATION_TOKEN;
   path += BANDS_NODE;

   wxXmlNode* pbandnode = GetNode(path);
   wxXmlNode* pnamenode = NULL;

   if (pbandnode) {
      wxString bandindex;
      long int auxband;

      pnamenode = pbandnode->GetChildren();

      // Busco si existe el nodo con ese indice de banda
      while (pnamenode) {
         pnamenode->GetPropVal(BAND_INDEX_PROPERTY, &bandindex);
         bandindex.ToLong(&auxband);
         if (BandNumber != static_cast<int>(auxband)) {
            pnamenode = pnamenode->GetNext();
         } else {
            // Si encuentra el nodo le reemplaza el valor
            pbandnode->RemoveChild(pnamenode);
            break;
         }
      }
   }
   return true;
}

/**
 * Configura un mundo nuevo para el renderizador.
 */
World* RasterElement::CreateWorld() {
   World* pWorld = new World();
   // Le asigna al mundo recibido la referencia espacial del elemento
   pWorld->SetSpatialReference(GetSpatialReference().c_str());

   // Creo un subset para asignar el window y el world al mundo recibido, y uno
   // para obtener el tamanio del viewport
   Subset subset, viewport;
   // asigno al subset y al viewport el extent del elemento
   int width = 0, height = 0;
   GetRasterSize(width, height);
   GetElementExtent(subset);
   // El tamanio del viewport es igual al de la imagen
   viewport.lr_.x_ = width;
   viewport.lr_.y_ = height;
   // Uso el subset generado para asignar el window y el world al mundo recibido
   pWorld->SetWorld(subset);
   pWorld->SetWindow(subset);

   // Auxiliar para obtener alto y ancho del viewport
   Dimension auxmatriz(viewport);

   // Asigna el tamanio del raster en P-L al tamanio del viewport
   pWorld->SetViewport(SURI_TRUNC(int, auxmatriz.GetWidth() ),
                       SURI_TRUNC(int, auxmatriz.GetHeight() ));
   return pWorld;
}

/**
 * Guarda el extent del raster en nodo dimension.
 * @param[in] ElementExtent subset que contiene extent de raster
 */
void RasterElement::SetElementExtent(const Subset &ElementExtent) {
   wxXmlNode *pnode = GetNode(wxString::Format("%s", DIMENSION_NODE));
   if (!pnode) {
      pnode = AddNode(GetNode(""), DIMENSION_NODE);
   }
   wxXmlNode *penode = GetNode(wxString::Format("%s", EXTENT_NODE));
   if (!penode) {
      penode = AddNode(pnode, EXTENT_NODE);
   }
   SetSubset(ElementExtent, penode);
}

/**
 * Inicializa el raster con el archivo en FileName
 * @param[in] FileName ruta al raster
 *  * @param[in] Metadata mapa con metadatos en formato clave-valor
 * @return this si se pudo configurar correctamente. NULL en otro caso
 */
RasterElement* RasterElement::Initialize(const std::string &FileName,
                                         const Option& Metadata) {
   SetUrl(FileName);
   metadata_ = Metadata;
   Image::ImageAccessType access = Image::ReadOnly;
   std::string writer = "null";
   std::string dataname = "void";
   int bandcount = 0, sizex = 0, sizey = 0;
   std::string pixels = metadata_.GetOption("Pixels");
   if (!pixels.empty()) {
      sizex = StringToNumber<int>(pixels);
   }
   std::string lines = metadata_.GetOption("Lines");
   if (!lines.empty()) {
      sizey = StringToNumber<int>(lines);
   }
   dataname = !metadata_.GetOption("Datatype").empty()? metadata_.GetOption("Datatype") : dataname;

   std::string bstr = metadata_.GetOption("Bandcount");
   if (!bstr.empty()) {
      bandcount = StringToNumber<int>(bstr);
   }


   Image *pimage = Image::Open(RasterRenderer::GenerateImageId(GetUrl().c_str()),
                               access, writer, bandcount, sizex, sizey, dataname,
                               metadata_);
   if (!pimage || pimage->GetBandCount() == 0) {
      Image::Close(pimage);
      return NULL;
   }

   SetUrl(pimage->GetOption("filename"));

   // nodos de georreferencia
   SetSpatialReference(pimage->GetOption(Image::Georreference));
   SetRasterModel(pimage->GetOption(Image::RasterMatrix));

   // nodos de dependencias
   std::vector < std::string > dependencies = tokenizer(
         pimage->GetOption("dependencies"), ",");
   for (size_t i = 0; i < dependencies.size(); i++)
      AddDependency(dependencies[i]);

   // nodos de renderizacion
   wxXmlNode *prnode = AddNode(GetNode(wxT("")), RENDERIZATION_NODE);
   // bandas a renderizar, 123 si tiene 3 o mas, 1 sino
   if (pimage->GetBandCount() > 2) {
      AddNode(prnode, BAND_COMBINATION_NODE, wxT("0 1 2"));
   } else {
      AddNode(prnode, BAND_COMBINATION_NODE, wxT("0"));
   }
   // nodo para la reproyeccion
//   wxXmlNode *preprojnode = AddNode(prnode,  REPROJECTION_NODE);
//   // TODO(Gabriel - TCK #2416): Se debe cambiar el tipo de reproyeccion a triangulacion
//   // ya que es la reproyeccion a utilizar por defecto
//   AddNode(preprojnode, REPROJECTION_TYPE_NODE, wxT(ELEMENT_DEFAULT_TYPE));
//   AddNode(preprojnode, REPROJECTION_TYPE_ALGORITHM_NODE, wxT(ELEMENT_DEFAULT_ALGORITHM));
//   AddNode(preprojnode, REPROJECTION_TYPE_ORDER_NODE, wxT(ELEMENT_DEFAULT_ALGORITHM_ORDER));
//   AddNode(preprojnode, REPROJECTION_TYPE_DELTA_NODE, wxT(ELEMENT_DEFAULT_ALGORITHM_DELTA));

   AddNode(prnode, CANVAS_CACHE_NODE);

   // Cargo nodo data cast
   std::string datatype = pimage->GetDataType();
   if (datatype != DataInfo<unsigned char>::Name) {
      // casteo de datos a uchar
      AddNode(prnode, DATA_CAST_NODE, wxT(""), OUTPUT_PROPERTY,
              DataInfo<unsigned char>::Name);
   }

   Subset subaux(0, 0);
   int width, height;
   // Como no encuentra el nodo RasterSize lo crea
   GetRasterSize(width, height);
   // Como no encuentra el nodo ElementExtent lo crea
   GetElementExtent(subaux);

   // Nodo Archivo
   /*TODO(02/12/2008 - Alejandro): Definir el tipo de archivo a asignar
    * Javier dijo que se deberia obtener a partir de la clase Image*/
   SetFileNode(pimage->GetOption(Image::Format),
               pimage->GetOption(Image::FormatDescription));
   RasterPropertiesType props;
   props.DataType = datatype;
   SetRasterProperties(props);

   // Agrego el nodo de Brillo/Contraste
   wxXmlNode *plutnode = AddNode(prnode, wxT(BRIGHTNESS_NODE), wxT(""),
                                 wxT(NAME_PROPERTY),
                                 wxT(NAME_PROPERTY_VALUE_LUT_BRIGHTNESS));
   AddNode(plutnode, wxT(LUT_BRIGHTNESS_NODE), wxT("0"));
   AddNode(plutnode, wxT(LUT_CONTRAST_NODE), wxT("1"));

   wxXmlNode *pinterpnode = AddNode(
         prnode, INTERPOLATION_NODE, wxT(""), wxT(INTERPOLATION_PROPERTY_ZOOM_IN),
         wxT(INTERPOLATION_PROPERTY_VALUE_NEAREST_NEIGHBOUR));
   if (!pinterpnode) {
      REPORT_AND_FAIL_VALUE("D:Error al agregar la interpolacion al raster", NULL);
   }

   pinterpnode->AddProperty(wxT(INTERPOLATION_PROPERTY_ZOOM_OUT),
                            wxT(INTERPOLATION_PROPERTY_VALUE_NEAREST_NEIGHBOUR));

   // Carga la propiedades de las bandas
   InitializeBandProperties(pimage);

   Image::Close(pimage);

   return this;
}

} /** namespace suri */
