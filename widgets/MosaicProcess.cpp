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

// Includes standard
#include <string>
#include <vector>
#include <limits>
#include <complex>
#include <math.h>
// Includes suri
#include "suri/MosaicProcess.h"
#include "suri/Image.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/DataTypes.h"
#include "suri/DatasourceInterface.h"
#include "suri/DatasourceManagerInterface.h"
#include "ClassifiedRasterDatasourceManipulator.h"
#include "ColorTable.h"
#include "suri/DataViewManager.h"
#include "suri/XmlFunctions.h"
#include "suri/Dimension.h"
#include "suri/DataTypes.h"
#include "DataCastRenderer.h"
#include "GdalResizer.h"
// Includes Wx
#include "wx/wx.h"
#include "wx/xml/xml.h"

/** namespace suri */
namespace suri {

/** Template cast */
/**
 * Este template se utiliza paracambiar los datos del canvas.
 * Guarda los datos de pSrc en pDest haciendo el casteo correspondiente
 * \pre pDest debe tener Size*tamanioTipoDatoDest tamanio reservado
 * \pre pSrc debe tener Size*tamanioTipoDatoSrc tamanio reservado
 * \post pDest tiene los datos de pSrc con tipo destino
 * @param[out] pDest puntero a los datos con nuevo tipo
 * @param[in] pSrc puntero a los datos originales
 * @param[in] Size cantidad de elementos a castear
 */
template<typename T1, typename T2>
void datacastMosaic(void* pDest, void* pSrc, size_t Size) {
   REPORT_DEBUG("D: Tipo de dato destino: %s", DataInfo<T1>::Name);
   REPORT_DEBUG("D: Tipo de dato origen: %s", DataInfo<T2>::Name);
   T1* pdest = static_cast<T1*>(pDest);
   T2* psrc = static_cast<T2*>(pSrc);
   // \todo Here be interpolation
   for (size_t i = 0; i < Size; i++) {
      T1 aux = static_cast<T1>(psrc[i]);
      pdest[i] = aux;
   }
   delete[] psrc;
}

template<typename T1>
void setPixelValue(void* DataOn, void * DataUnder, double PonderationOn,
                   double PonderationUnder, int Position) {
   T1* on = static_cast<T1*>(DataOn);
   T1* under = static_cast<T1*>(DataUnder);
   T1 newpixelValue = static_cast<T1> ( (on[Position] * PonderationOn
         + under[Position] * PonderationUnder) / (PonderationOn + PonderationUnder) );
   on[Position] =  newpixelValue;
}

template<typename T1>
void SetNewPixelFinalValue(void* DataFinal, void* DataOn, void * DataUnder, double PonderationOn,
                           double PonderationUnder, int Position){
           T1* final = static_cast<T1*>(DataFinal);
           T1* on = static_cast<T1*>(DataOn);
           T1* under = static_cast<T1*>(DataUnder);
           T1 newpixelValue = static_cast<T1> ( (on[Position] * PonderationOn
                 + under[Position] * PonderationUnder) / (PonderationOn + PonderationUnder));
           final[Position] = newpixelValue;
}

template<typename T1>
bool isValid(const std::vector<void*>& Bands, int Position, int NoDataValue) {
   std::vector<void*>::const_iterator it = Bands.begin();
   for (; it != Bands.end(); ++it) {
      T1* data = static_cast<T1*>(*it);
      T1  noData = static_cast<T1>(NoDataValue);
      if (data[Position] != noData)
           return true;
   }
   return false;
}

typedef void (*CastFunctionType)(void*, void*, size_t);
typedef void (*SimpleMapfunction)(void*, void *, double, double, int);
typedef void (*SimpleMapfunction2)(void*, void*, void *, double, double, int);
typedef bool(*comparatorFunction)(const std::vector<void*>&, int, int);

/** Genera un mapa doble (matriz) indexado por tipos a la funcion */
INITIALIZE_DUAL_DATATYPE_MAP(CastFunctionType, datacastMosaic);
INITIALIZE_DATATYPE_MAP(SimpleMapfunction, setPixelValue);
INITIALIZE_DATATYPE_MAP(SimpleMapfunction2, SetNewPixelFinalValue);
INITIALIZE_DATATYPE_MAP(comparatorFunction, isValid);
/** Nombre del proceso **/
const std::string MosaicProcess::kProcessName = "MosaicProcess";
/**
 * Constructor
 * Asigna el tipo de dato del elemento creado por la herramienta.
 * @param[in] pInputElement: puntero al elemento sobre el que se aplica la herramienta
 * @param[in] WindowSubset subset de la imagen desplegada cuando se selecciono la herramienta
 * @param[out] pOutputElement referencia al elemento creado por la herramienta
 */
MosaicProcess::MosaicProcess(std::vector<Element*> InputElements,
                                           const Subset &WindowSubset,
                                           DataViewManager* pDataViewManager,
                                           bool ShowSpectralSelectionPart) :
      FileExporterProcess(InputElements, WindowSubset, pDataViewManager),
      pDatasourceManager_(pDataViewManager ?
                           pDataViewManager->GetDatasourceManager() : NULL)  {
   // Oculta/Despliega part seleccion espectral
   showSpectralSelectionPart_ = ShowSpectralSelectionPart;
   SetProcessName(kProcessName);
   dataSize_ = 0;
   pixelDistance_ = 0;
   noDataColour_ = 0;
   noDataValueOn_ = 0;
   difx_ = 0;
   dify_ = 0;
   pixelSize_ = 0;
}

/**
 * Destructor
 */
MosaicProcess::~MosaicProcess() {
}

/**
 * Dado los pesos de cada pixel y los datos de las imagenes, calcula el nuevo valor para el pixel
 * de la imagen en la posicion dada.
 **/
void MosaicProcess::SetNewPixelValue(void* DataOn, void * DataUnder, double PonderationOn,
                                           double PonderationUnder, int Position) {
   setPixelValueTypeMap[dataType_] (DataOn, DataUnder, PonderationOn, PonderationUnder, Position);
}



/**Setea el tipo de dato del raster de salida, como el mayor del tipo de dato de entrada**/
void MosaicProcess::SetDataSize(){
   RasterElement* pRasterElement = NULL;
   std::vector<Element*>::const_iterator it = inputElements_.begin();
   for (; it != inputElements_.end(); ++it) {
      pRasterElement = dynamic_cast<RasterElement*>( (*it) );
      if (pRasterElement) {
         std::string datatype = pRasterElement->GetRasterProperties().DataType;
         unsigned int  size = SizeOf(datatype);
         if (size > dataSize_) {
            dataSize_ = size;
            dataType_ = datatype;
         }
      }
   }
}


/**
 * Pemite configurar el RasterElement que se va a renderizar
 * @param[in] pRasterElement elemento raster a renderizar
 * @return true siempre
 */
bool MosaicProcess::ConfigureRaster(RasterElement *pRasterElement) {
   FileExporterProcess::ConfigureRaster(pRasterElement);
   return true;
}

/**
 * Guarda en la imagen de salida la tabla de colores de la entrada.
 * @return informa si pudo configurar elemento
 */
bool MosaicProcess::ConfigureOutput() {
   if (!FileExporterProcess::ConfigureOutput())
      return false;
   return true;
}


/**
 * Crea un elemento con el filename del elemento recibido
 * Llama a ConfigureRaster para permitir a las clases que heredan configurar
 * el elemento.
 * Crea un lista y el agrega el elemento.
 * \attention La lista devuelta (y su destruccion) es responsabilidad del que la recibe.
 * @return playerlist : devuelve la lista con el elemento. Devuelve null si falla
 * la creacion del elemento raster o su configuracion.
 */
LayerList* MosaicProcess::GetElementList() {
   LayerList *playerlist = new LayerList();
   // Se copia el elemento porque el original esta en la LayerList y
   // tiene eventos conectados
   std::vector<Element*>::const_iterator it = inputElements_.begin();
   for (; it != inputElements_.end(); ++it){
      Element* inputElem = *it;
      RasterElement* praster = dynamic_cast<RasterElement*>(Element::Create(
            "RasterElement", inputElem->GetNode(), inputElem->GetMetadataMap()));
      bool available = false;
      double value = 0;
      inputElem->GetNoDataValue(available, value);
      // Si falla la creacion/configuracion del elemento retorna NULL
      praster->SetNoDataValue(value);
      if (!praster || !ConfigureRasterBands(praster) || !ConfigureRaster(praster)) {
         SHOW_ERROR(message_RASTER_CONFIGURATION_ERROR);
         delete playerlist;
         delete praster;
         return NULL;
      }
      /**
       * Se elimina el nodo del renderizador multiplexor para evitar problemas en la renderizacion
       * en el procesamiento
      **/
      wxXmlNode* pmultnode =
                     praster->GetNode(RENDERIZATION_NODE NODE_SEPARATION_TOKEN
                                      BAND_MULTIPLEXOR_NODE);
      if (pmultnode)
         praster->RemoveNode(praster->GetNode(RENDERIZATION_NODE), pmultnode);
      // como esta el raster. Tiene interpolacion? => agregarselo a lo ultimo.
      praster->Activate(true);
      playerlist->AddElement(praster);
   }
   return playerlist;
}



/** Metodo que se llama cuando se configura el ProcessAdaptLayer del proceso**/
void MosaicProcess::DoOnProcessAdaptLayerSet() {
   FileExporterProcess::DoOnProcessAdaptLayerSet();
}

int MosaicProcess::GetDataSize(const Subset& Extent,  Element* pElement){
   RasterElement* pRaster = dynamic_cast<RasterElement*>(pElement);
   std::string spatial = pRaster->GetRasterModel();
   RasterSpatialModel * pm = RasterSpatialModel::Create(spatial);
   Subset aux = Extent;
   pm->InverseTransform(aux.ul_);
   pm->InverseTransform(aux.lr_);
   FixSubset(aux);
   Dimension dim(aux);
   return  SURI_CEIL( dim.GetHeight()) * SURI_CEIL(dim.GetWidth());
}

/** Devuelve el valor de pixeles de cada banda del extent del elemento indicado  **/
std::vector<void*> MosaicProcess::GetPixelValue(const Subset& Extent, Element* pElement) {
   RasterElement* pRaster = dynamic_cast<RasterElement*>(pElement);
   std::vector<void*> data(pRaster->GetBandCount());
   int size = 0;
   if (pRaster) {
      std::string spatial = pRaster->GetRasterModel();
      RasterSpatialModel * pm = RasterSpatialModel::Create(spatial);
      Subset aux = Extent;
      pm->InverseTransform(aux.ul_);
      pm->InverseTransform(aux.lr_);
      FixSubset(aux);
      Dimension dim = GetIntersectionDimension();
      size = SURI_CEIL(dim.GetHeight()) * SURI_CEIL(dim.GetWidth());
      std::vector<int> bandindex(pRaster->GetBandCount());
      for (int i = 0; i < pRaster->GetBandCount(); i++) {
         bandindex[i] = i;
         data[i] = new unsigned char[size * SizeOf(pRaster->GetRasterProperties().DataType)];
      }
      Image* pimage = Image::Open(pRaster->GetUrl().mb_str());

      int ulx = SURI_ROUND(int, aux.ul_.x_);
      int uly = SURI_ROUND(int, aux.ul_.y_);
      int lrx = SURI_ROUND(int, aux.lr_.x_);
      int lry = SURI_ROUND(int, aux.lr_.y_);
      if (difx_ == 0 && dify_ == 0) {
         difx_ = ulx - lrx;
         dify_ = lry - uly;
      }
      if ( (ulx - lrx) > difx_ ){
         lrx += (ulx - lrx) - difx_;
      }
      if ( (ulx - lrx) < difx_ ){
         lrx -= (ulx - lrx) - difx_;
      }
      pimage->Read(bandindex, data, ulx, uly, lrx, lry);
      std::vector<void*>::const_iterator it = data.begin();
      std::vector<void*> castedData;
      if (pRaster->GetRasterProperties().DataType != dataType_) {
         for (; it != data.end(); ++it) {
            unsigned char* pixels = new unsigned char[size * dataSize_];
            datacastMosaicTypeMatrix[dataType_][pRaster->GetRasterProperties().DataType]
                                                                     (pixels, *it, size);

            castedData.push_back(pixels);
         }
         data = castedData;
      }
      Image::Close(pimage);
   }
   return data;
}

Subset MosaicProcess::GetIntersection(RasterElement* pOn, RasterElement* pUnder){
   Subset subseton;
   Subset subsetunder;
   pOn->GetElementExtent(subseton);
   pUnder->GetElementExtent(subsetunder);
   return Intersect(subseton, subsetunder);
}


/** Arregla el subset para que siempre se lea la misma cantidad de pixeles**/
void MosaicProcess::FixSubset(Subset& ActualSub){
   int biggerulx = SURI_ROUND(int, biggerIntersectionSubset_.ul_.x_);
   int biggeruly = SURI_ROUND(int, biggerIntersectionSubset_.ul_.y_);
   int biggerlrx = SURI_ROUND(int, biggerIntersectionSubset_.lr_.x_);
   int biggerlry = SURI_ROUND(int, biggerIntersectionSubset_.lr_.y_);
   int biggerdifx = biggerlrx - biggerulx;
   int biggerdify = biggerlry - biggeruly;
   int actualulx = SURI_ROUND(int, ActualSub.ul_.x_);
   int actualuly = SURI_ROUND(int, ActualSub.ul_.y_);
   int actuallrx = SURI_ROUND(int, ActualSub.lr_.x_);
   int actuallry = SURI_ROUND(int, ActualSub.lr_.y_);
   int actualdifx = actuallrx - actualulx;
   int actualdify = actuallry - actualuly;
   if (actualdifx < biggerdifx) {
      int dif = biggerdifx - actualdifx;
      ActualSub.lr_.x_ += dif;
   }
   if (actualdify < biggerdify) {
      int dif = biggerdify - actualdify;
      ActualSub.lr_.y_ += dif;
   }
}

void MosaicProcess::SetBiggerIntersectionSubset(){
   std::vector<Element*>::const_iterator it = inputElements_.begin();
   bool valid;
   (*it)->GetNoDataValue(valid, noDataValueOn_);
   for (; it != inputElements_.end(); ++it){
      RasterElement* pRaster = dynamic_cast<RasterElement*>(*it);
      Subset aux = intersection_;
      std::string spatial = pRaster->GetRasterModel();
      RasterSpatialModel * pm =RasterSpatialModel::Create(spatial);
      pm->InverseTransform(aux.ul_);
      pm->InverseTransform(aux.lr_);
      if (!biggerIntersectionSubset_.IsValid()){
         biggerIntersectionSubset_ = aux;
      } else {
         Dimension dimBigger(biggerIntersectionSubset_);
         Dimension dimActual(aux);
         if ( (dimActual.GetWidth() > dimBigger.GetWidth())
               || dimActual.GetHeight() > dimBigger.GetWidth()) {
            biggerIntersectionSubset_ = aux;
         }
      }
    }
   return ;
}

/**
  * Obtiene el tamaño de pixel mayor y escala las imagenes para que todas tengan el mismo tamaño
  * de pixel.
  */
void MosaicProcess::SetElementsPixelSize() {
   std::vector<Element*>::const_iterator it = inputElements_.begin();
   double maxPixelSize = 0;
   for (; it != inputElements_.end(); ++it) {
      RasterElement* pRaster = dynamic_cast<RasterElement*>(*it);
      double pixelres = pRaster->GetPixelRes();
      double pixelsize = sqrt(pixelres);
      if (pixelsize > maxPixelSize) {
         maxPixelSize = pixelsize;
      }
   }
   pixelSize_ = maxPixelSize;
   std::vector<Element*> aux = inputElements_;
   inputElements_.clear();
   std::vector<Element*>::iterator itElement = aux.begin();
   for (; itElement != aux.end(); ++itElement) {
      RasterElement* pRaster = dynamic_cast<RasterElement*>(*itElement);
      double pixelsize = sqrt(pRaster->GetPixelRes());
      Element* pEl = NULL;
      if (SURI_ROUND(int, pixelsize) != SURI_ROUND(int, maxPixelSize)) {
         pEl = GetNewElementWithPixelValue(pRaster, maxPixelSize);
      }else {
         pEl = pRaster;
      }
      inputElements_.push_back(pEl);
   }
   return ;
}

Element* MosaicProcess::GetNewElementWithPixelValue(RasterElement* pElement, double PixelValue) {
   std::string tempname = wxFileName::CreateTempFileName("").c_str();
   std::vector<int> bandcount(pElement->GetBandCount());
   for (int x = 0; x < pElement->GetBandCount(); x++) {
      bandcount[x] = x;
   }
   Subset outputSubset;
   pElement->GetElementExtent(outputSubset);
   Dimension dimensionout(outputSubset);
   int pixels = SURI_TRUNC(int, SURI_CEIL(dimensionout.GetWidth() / PixelValue));
   int lines = SURI_TRUNC(int, SURI_CEIL(dimensionout.GetHeight() / PixelValue));
   GdalResizer res(pElement->GetUrl().c_str(), tempname, bandcount);
   res.Resize(outputSubset, pixels, lines, PixelValue, -PixelValue);
   Option opt;
   RasterElement* raster = raster->Create(tempname.c_str(), opt);
   return raster;
}

bool MosaicProcess::RunProcess() {
   inputElements_.clear();
   pAdaptLayer_->GetAttribute< std::vector<Element*> > (ProcessAdaptLayer::MosaicRasterVectorAttr,
                                                      inputElements_);
   SetElementsPixelSize();
   std::string finalFileName;
   pAdaptLayer_->GetAttribute<std::string>(
                     ProcessAdaptLayer::OutputFileNameKeyAttr, finalFileName);
   std::list<Element*> auxList;
   std::vector<Element*> originalVector = inputElements_;
   std::vector<Element*>::const_iterator it = inputElements_.begin();
   for (; it != inputElements_.end(); ++it) {
      auxList.push_back(*it);
   }
   if (auxList.size() < 1) return false;
   std::string tempname = wxFileName::CreateTempFileName("").c_str();
   if (auxList.size() > 2) {
      pAdaptLayer_->AddAttribute<std::string>(ProcessAdaptLayer::OutputFileNameKeyAttr,
                                                                                     tempname);
   }
   inputElements_.clear();
   inputElements_.push_back(auxList.front());
   auxList.pop_front();
   if (!auxList.empty()) {
      inputElements_.push_back(auxList.front());
      auxList.pop_front();
   }

   bool result = RasterProcess::RunProcess();
   while ( result && !auxList.empty()) {
      inputElements_.clear();
      RasterElement* praster = new RasterElement();
      Option opt;
      praster = praster->Create(tempname.c_str(), opt);
      praster->SetNoDataValue(noDataColour_);
      inputElements_.push_back(praster);
      inputElements_.push_back(auxList.front());
      auxList.pop_front();
      tempname = wxFileName::CreateTempFileName("").c_str();
      if (auxList.size() > 1) {
          pAdaptLayer_->AddAttribute<std::string>(
                              ProcessAdaptLayer::OutputFileNameKeyAttr, tempname);
      } else {
         pAdaptLayer_->AddAttribute<std::string>(
                                       ProcessAdaptLayer::OutputFileNameKeyAttr, finalFileName);
      }
      delete pRenderizationObject_;
      pRenderizationObject_ = new FileRenderization();
      intersection_ = Subset(-1, -1, -1, -1);
      biggerIntersectionSubset_ = Subset(-1, -1, -1, -1);
      pRasterModelOut_ = NULL;
      difx_ = 0;
      dify_ = 0;
      result = RasterProcess::RunProcess();
   }
   inputElements_ = originalVector ;
   return result;
}

/* *
 * Devuelve la distancia de pixel en "x" y en "y" , si la distancia seteada en el part
 * es mayor que el ancho y el alto de la interseccion, se reemplazan por el ancho y el alto
 * respectivamente.
 * */
void MosaicProcess::GetPixelDistance(int& DistanceX, int& DistanceY) {
   DistanceX = pixelDistance_;
   DistanceY = pixelDistance_;
   Dimension dim = GetIntersectionDimension();
   /* Si la distancia de pixel es mayor al ancho de la interseccion ,
    la distancia de pixel se reemplaza por el ancho de la interseccion*/
   if (pixelDistance_ > SURI_ROUND(int, dim.GetWidth())) {
      DistanceX = SURI_ROUND(int, dim.GetWidth());
   }
   if (pixelDistance_ > SURI_ROUND(int, dim.GetHeight())) {
      DistanceY = SURI_ROUND(int, dim.GetHeight());
   }
   return;
}


/**
 *  Aplica feathering de oeste a este de la imagen, solo aplica si el pixel es valido ,
 *  si es invalido pasa al siguiente
 **/
void MosaicProcess::ApplyFeatheringFromWestToEast(std::vector<void*>& DataOn,
                                                        const std::vector<void*>& DataUnder) {

   int pixelDistanceX = 0;
   int pixelDistanceY = 0;
   std::vector<void*>::const_iterator bandIteratorOn;
   std::vector<void*>::const_iterator bandIteratorUnder;
   Dimension dim = GetIntersectionDimension();
   GetPixelDistance(pixelDistanceX, pixelDistanceY);
   int columnCount = SURI_ROUND(int, dim.GetWidth());
   int positionX = 0;
   int positionY = 0;
   while (positionY < dim.GetHeight()) {
      positionX = 0;
      // Cantidad de pixeles a los que se le aplico feathering hasta ahora en la linea
      int featherCount = 0;
      while (positionX < dim.GetWidth()) {
         unsigned int pos = positionY * columnCount + positionX;
         double ponderationUnder = 0;
         double ponderationOn = 0;

         bandIteratorOn = DataOn.begin();
         bandIteratorUnder = DataUnder.begin();
         if (IsValidData(DataOn, pos)) {
            if (featherCount < pixelDistanceX) {
            for (; bandIteratorOn != DataOn.end(); ++bandIteratorOn, ++bandIteratorUnder) {
               ponderationUnder = static_cast<double>(pixelDistanceX - featherCount)
                                                             / static_cast<double>(pixelDistanceX);
               ponderationOn =  static_cast<double>(featherCount) /
                     static_cast<double>(pixelDistanceX);
               SetNewPixelValue(*bandIteratorOn, *bandIteratorUnder, ponderationOn,
                                               ponderationUnder, pos);
            }
            featherCount++;
            }
         }
         positionX++;
      }
      positionY++;
   }
}

/**
 *  Aplica feathering de este a oeste de la imagen, solo aplica si el pixel es valido ,
 *  si es invalido pasa al siguiente
 **/
void MosaicProcess::ApplyFeatheringFromEastToWest(std::vector<void*>& DataOn,
                                                        const std::vector<void*>& DataUnder) {

   int pixelDistanceX = 0;
   int pixelDistanceY = 0;
   GetPixelDistance(pixelDistanceX, pixelDistanceY);
   std::vector<void*>::const_iterator bandIteratorOn;
   std::vector<void*>::const_iterator bandIteratorUnder;
   Dimension dim = GetIntersectionDimension();
   int columnCount = SURI_ROUND(int, dim.GetWidth());
   int positionX = dim.GetWidth() -1;
   int positionY = dim.GetHeight() -1;
   while (positionY >  0) {
      positionX = dim.GetWidth() -1;
      // cantidad de pixeles a los que se le aplico feathering hasta ahora en la linea
      int featherCount = pixelDistanceX ;
      while (positionX > 0) {
         unsigned int pos = positionY * columnCount + positionX;
         double ponderationUnder = 0;
         double ponderationOn = 0;
         bandIteratorOn = DataOn.begin();
         bandIteratorUnder = DataUnder.begin();
         if (IsValidData(DataOn, pos)) {
            if (featherCount > 0) {
            for (; bandIteratorOn != DataOn.end(); ++bandIteratorOn, ++bandIteratorUnder) {
               ponderationUnder = static_cast<double>(featherCount)
                                                             / static_cast<double>(pixelDistanceX);
               ponderationOn = static_cast<double>(pixelDistanceX  - featherCount)
                                                             / static_cast<double>(pixelDistanceX);
               SetNewPixelValue(*bandIteratorOn, *bandIteratorUnder, ponderationOn,
                                                             ponderationUnder, pos);
            }
            featherCount--;
           }
         }
         positionX--;
      }
      positionY--;
   }
}

/**
 *  Aplica feathering de norte a sur de la imagen, solo aplica si el pixel es valido ,
 *  si es invalido pasa al siguiente
 **/
void MosaicProcess::ApplyFeatheringFromNorthToSouth(std::vector<void*>& DataOn,
                                                        const std::vector<void*>& DataUnder) {
    int pixelDistanceX = 0;
    int pixelDistanceY = 0;

    std::vector<void*>::const_iterator bandIteratorOn;
    std::vector<void*>::const_iterator bandIteratorUnder;

    Dimension dim = GetIntersectionDimension();

    GetPixelDistance(pixelDistanceX, pixelDistanceY);

    int columnCount = SURI_ROUND(int, dim.GetWidth());
    int positionX = 0;
    int positionY = 0;
    while (positionX < dim.GetWidth()) {
       positionY = 0;
       // cantidad de pixeles a los que se le aplico feathering hasta ahora en la linea
       int featherCount = 0;
       while (positionY < dim.GetHeight()) {
          unsigned int pos = positionY * columnCount + positionX;
          double ponderationUnder = 0;
          double ponderationOn = 0;

          bandIteratorOn = DataOn.begin();
          bandIteratorUnder = DataUnder.begin();
          if (IsValidData(DataOn, pos)) {
             if (featherCount < pixelDistanceY) {
             for (; bandIteratorOn != DataOn.end(); ++bandIteratorOn, ++bandIteratorUnder) {

                ponderationUnder = static_cast<double>(pixelDistanceY - featherCount )
                                                              / static_cast<double>(pixelDistanceY);
                ponderationOn =  static_cast<double>(featherCount) /
                      static_cast<double>(pixelDistanceY);
                SetNewPixelValue(*bandIteratorOn, *bandIteratorUnder, ponderationOn,
                                                              ponderationUnder, pos);

             }
             featherCount++;
             }
          }
          positionY++;
       }
       positionX++;
    }
}

/**
 *  Aplica feathering de sur a norte de la imagen, solo aplica si el pixel es valido ,
 *  si es invalido pasa al siguiente
 **/
void MosaicProcess::ApplyFeatheringFromSouthToNorth(std::vector<void*>& DataOn,
                                                        const std::vector<void*>& DataUnder) {

   int pixelDistanceX = 0;
   int pixelDistanceY = 0;

   std::vector<void*>::const_iterator bandIteratorOn;
   std::vector<void*>::const_iterator bandIteratorUnder;

   Dimension dim = GetIntersectionDimension();
   GetPixelDistance(pixelDistanceX, pixelDistanceY);
   int columnCount = SURI_ROUND(int, dim.GetWidth());
   int positionX = dim.GetWidth() -1;
   int positionY = dim.GetHeight() -1;
   while (positionX > 0) {
      positionY = dim.GetHeight() -1;
      // cantidad de pixeles a los que se le aplico feathering hasta ahora en la linea
      int featherCount = pixelDistanceY ;
      while (positionY > 0) {
         unsigned int pos = positionY * columnCount + positionX;
         double ponderationUnder = 0;
         double ponderationOn = 0;
         bandIteratorOn = DataOn.begin();
         bandIteratorUnder = DataUnder.begin();
         if (IsValidData(DataOn, pos)) {
            if (featherCount > 0) {
            for (; bandIteratorOn != DataOn.end(); ++bandIteratorOn, ++bandIteratorUnder) {

               ponderationUnder = static_cast<double>(featherCount)
                                                             / static_cast<double>(pixelDistanceY);
               ponderationOn = static_cast<double>(pixelDistanceY  - featherCount)
                                                             / static_cast<double>(pixelDistanceY);
               SetNewPixelValue(*bandIteratorOn, *bandIteratorUnder, ponderationOn,
                                                             ponderationUnder, pos);

            }
            featherCount--;
            }
         }
         positionY--;
      }
      positionX--;
   }
}


/**
 *  Se encarga de aplicar feathering entre data under y data on el resultado se guarda en data final
 *
 */
void MosaicProcess::ApplyFeathering(std::vector<void*> DataFinal, const std::vector<void*>& DataUnder,
                                                   const std::vector<void*>& DataOn,
                                                   const Coordinates& Begin, const Coordinates& End,
                                                                        bool Axis, double Factor) {
   int beginY = SURI_ROUND(int, Begin.y_);
   int endY = SURI_ROUND(int, End.y_);
   int beginX = SURI_ROUND(int, Begin.x_);
   int endX = SURI_ROUND(int, End.x_);
   int pixelDistanceX = pixelDistance_;
   int pixelDistanceY = pixelDistance_;
   std::vector<void*>::const_iterator bandIteratorFinal;
   std::vector<void*>::const_iterator bandIteratorOn;
   std::vector<void*>::const_iterator bandIteratorUnder;
   Subset aux = intersection_;
   pRasterModelOut_->InverseTransform(aux.ul_);
   pRasterModelOut_->InverseTransform(aux.lr_);
   FixSubset(aux);
   Dimension dim(aux);
   if (pixelDistance_ > SURI_ROUND(int, dim.GetWidth())) {
      pixelDistanceX = SURI_ROUND(int, dim.GetWidth());
   }
   if (pixelDistance_ > SURI_ROUND(int, dim.GetHeight())) {
      pixelDistanceY = SURI_ROUND(int, dim.GetHeight());
   }
   int columnCount = SURI_ROUND(int, dim.GetWidth());
   for (int y = beginY; y < endY; y++) {
      for (int x = beginX; x < endX; x++) {
         double ponderationUnder = 0;
         double ponderationOn = 0;
         bandIteratorFinal = DataFinal.begin();
         bandIteratorOn = DataOn.begin();
         bandIteratorUnder = DataUnder.begin();
         for (; bandIteratorOn != DataOn.end(); ++bandIteratorOn, ++bandIteratorUnder,
                                                                             ++bandIteratorFinal) {
            unsigned int pos = y * columnCount + x;
            if (!Axis) { // si Axis = false  el eje y
               ponderationUnder = static_cast<double>(y - Factor)
                                                           / static_cast<double>(pixelDistanceY);
               ponderationOn = static_cast<double>(pixelDistanceY - (y - Factor))
                                                           / static_cast<double>(pixelDistanceY);
            } else { // Si Axis = true el eje x
               ponderationUnder = static_cast<double>(x - Factor)
                                                              / static_cast<double>(pixelDistanceX);
               ponderationOn = static_cast<double>(pixelDistanceX - (x - Factor))
                                                              / static_cast<double>(pixelDistanceX);
            }
            if (Factor == 0) {
               double aux = ponderationOn;
               ponderationOn = ponderationUnder;
               ponderationUnder = aux;
            }
            SetNewPixelFinalValueTypeMap[dataType_](*bandIteratorFinal,*bandIteratorOn,
                  *bandIteratorUnder, ponderationOn,
                                                         ponderationUnder, pos);
         }
      }
   }
}


/**
 * Devuelve la Dimension de la interseccion
 */
Dimension MosaicProcess::GetIntersectionDimension() {
   Subset aux = intersection_;
   pRasterModelOut_->InverseTransform(aux.ul_);
   pRasterModelOut_->InverseTransform(aux.lr_);
   std::vector<void*>::const_iterator bandIteratorOn;
   std::vector<void*>::const_iterator bandIteratorUnder;
   FixSubset(aux);
   Dimension dim(aux);
   return dim;
}

/** Si DataOn tiene pixeles no validos los reemplaza por los pixeles de DataUnder**/
void MosaicProcess::RepleaceNoDataArea(std::vector<void*>& DataOn,
                         const std::vector<void*>& DataUnder){
   std::vector<void*>::const_iterator bandIteratorOn;
   std::vector<void*>::const_iterator bandIteratorUnder;
   Dimension dim = GetIntersectionDimension();
   int columnCount = SURI_TRUNC(int, dim.GetWidth());
   for (int y = 0; y < dim.GetHeight() ; y++) {
      for (int x = 0; x < dim.GetWidth() ; x++) {
         unsigned int pos = y * columnCount + x;
         bandIteratorOn = DataOn.begin();
         bandIteratorUnder = DataUnder.begin();
         if (!IsValidData(DataOn, pos)) {
            for (; bandIteratorOn != DataOn.end(); ++bandIteratorOn,
                                 ++bandIteratorUnder) {
               SetNewPixelValue(*bandIteratorOn, *bandIteratorUnder, 0, 1, pos);
            }
         }
      }
   }
}

/* *
 * Devuelve false, si todas las bandas tienen en la posicion Position el valor no valido, true
 * en caso contrario
 * */
bool MosaicProcess::IsValidData(const std::vector<void*>& Bands, int Position) {
   return isValidTypeMap[dataType_](Bands, Position, noDataValueOn_);
}

/**
 * Verifica si los limites de la  interseccion entre las imagenes tienen valores validos
 * en la imagen superior
 **/
void MosaicProcess::IsValidEdge(std::vector<void*>& Data, bool& North, bool& South,
                                bool& East, bool& West) {
   Dimension dim = GetIntersectionDimension();
   int x = 0;
   int y = 0;
   int columncount = SURI_TRUNC(int, dim.GetWidth());
   int position = 0;
   North = true;
   South = true;
   East = true;
   West = true;
   for (x = 0, y = 0; x < dim.GetWidth(); x++) {
      position = y * columncount + x;
      if (!IsValidData(Data, position)) {
         North = false;
      }
   }
   for (x = 0, y = dim.GetHeight() - 1; x < dim.GetWidth(); x++) {
      position = y * columncount + x;
      if (!IsValidData(Data, position)) {
         South = false;
      }
   }
   for (x = 0, y = 0; y < dim.GetHeight(); y++) {
      position = y * columncount + x;
      if (!IsValidData(Data, position)) {
         West = false;
      }
   }
   for (x = 0, y = dim.GetHeight() - 1; y < dim.GetHeight(); y++) {
      position = y * columncount + x;
      if (!IsValidData(Data, position)) {
         East = false;
      }
   }
}


std::vector<void*> MosaicProcess::GetFeatheringData() {
   bool north = true;
   bool south = true;
   bool east = true;
   bool west = true;
   RasterElement* pRasterOn =  dynamic_cast<RasterElement*>(inputElements_[0]);
   RasterElement* pRasterUnder =  dynamic_cast<RasterElement*>(inputElements_[1]);
   Subset subOn;
   Subset subUnder;
   Subset intersection = GetIntersection(pRasterOn, pRasterUnder);
   Dimension dim = GetIntersectionDimension();
   pRasterOn->GetElementExtent(subOn);
   pRasterUnder->GetElementExtent(subUnder);
   std::vector<void*> dataOn = GetPixelValue(intersection, pRasterOn);
   std::vector<void*> dataUnder = GetPixelValue(intersection, pRasterUnder);
   IsValidEdge(dataOn, north, south, east, west);
   Subset subsetout;
   pAdaptLayer_->GetAttribute<Subset>(ProcessAdaptLayer::OutputSubsetWindowKeyAttr, subsetout);
   pRasterModelOut_->InverseTransform(subsetout.ul_);
   pRasterModelOut_->InverseTransform(subsetout.lr_);
   if ((subOn.ul_.x_ == intersection.ul_.x_)
           && ((subUnder.ul_.x_ != intersection.ul_.x_) || !west)) {
      ApplyFeatheringFromWestToEast(dataOn, dataUnder);
   }
   if ((subOn.lr_.x_ == intersection.lr_.x_)
         && ((subUnder.lr_.x_ != intersection.lr_.x_) || !east)) {
      ApplyFeatheringFromEastToWest(dataOn, dataUnder);
   }
   if ((subOn.ul_.y_ == intersection.ul_.y_)
         && ((subUnder.ul_.y_ != intersection.ul_.y_) || !north)) {
      ApplyFeatheringFromNorthToSouth(dataOn, dataUnder);
   }
   if ((subOn.lr_.y_ == intersection.lr_.y_)
         && ((subUnder.lr_.y_ != intersection.lr_.y_) || !south)) {
      ApplyFeatheringFromSouthToNorth(dataOn, dataUnder);
   }
   RepleaceNoDataArea(dataOn, dataUnder);
   ApplySecondaryFeathering(dataOn, subOn, subUnder);
   // libero memoria
   std::vector<void*>::iterator it = dataUnder.begin();
   for (; it != dataUnder.end(); ++it ) {
      unsigned char* under = static_cast<unsigned char*>(*it);
      delete [] under;
   }
   return  dataOn;
}



/**
 * Luaego de aplicar el feathering principal  se generan lineas en los extremos, este metodo
 * se encarga de aplicar feathering a esas lineas
 **/
void MosaicProcess::ApplySecondaryFeathering(std::vector<void*>&DataOn, const Subset& SubOn,
                                             const Subset& SubUnder) {
   RasterElement* pRasterOn =  dynamic_cast<RasterElement*>(inputElements_[0]);
   Subset aux = intersection_;
   pRasterModelOut_->InverseTransform(aux.ul_);
   pRasterModelOut_->InverseTransform(aux.lr_);
   Dimension dim(aux);
   int pixelDistanceY = 15;
   int pixelDistanceX = 15;
   int auxPixel = pixelDistance_;
   pixelDistance_ = 15;
   std::vector<void*> dataOnCopy = GetPixelValue(intersection_, pRasterOn);
   bool lrUnder = (SubUnder.lr_.x_ != intersection_.lr_.x_) || (SubUnder.lr_.x_ != SubOn.lr_.x_);
   bool ulUnder = (SubUnder.ul_.x_ != intersection_.ul_.x_) || (SubUnder.ul_.x_ != SubOn.ul_.x_) ;
   bool lrUpper = SubOn.lr_.y_ != intersection_.lr_.y_;
   bool ulUpper = SubOn.ul_.y_ != intersection_.ul_.y_;
   bool ulX = (SubOn.ul_.x_ == intersection_.ul_.x_) && (SubUnder.ul_.x_ != intersection_.ul_.x_);
   bool lrX = (SubOn.lr_.x_ == intersection_.lr_.x_) && (SubUnder.lr_.x_ != intersection_.lr_.x_);
   bool ulY = (SubOn.ul_.y_ == intersection_.ul_.y_) && (SubUnder.ul_.y_ != intersection_.ul_.y_);
   bool lrY = (SubOn.lr_.y_ == intersection_.lr_.y_) && (SubUnder.lr_.y_ != intersection_.lr_.y_);
   bool xLower = lrUpper && (ulX || lrX) && (!lrY);
   bool xUpper = ulUpper && (ulX || lrX) && (!ulY);
   bool yRight = lrUnder && (ulY || lrY) && (!lrX);
   bool yLeft  = ulUnder && (ulY || lrY) && (!ulX);
   if (xLower){
        Coordinates begin(0, SURI_ROUND(int, dim.GetHeight() - pixelDistanceY));
        Coordinates end(SURI_ROUND(int, dim.GetWidth()), SURI_ROUND(int, dim.GetHeight()));
        double factor = dim.GetHeight() - pixelDistanceY;
        ApplyFeathering(DataOn, dataOnCopy, DataOn, begin, end, false, factor);
     }
     if (xUpper) {
        Coordinates begin(0, 0);
        Coordinates end(SURI_ROUND(int, dim.GetWidth()), pixelDistanceY);
        double factor = 0;
        ApplyFeathering(DataOn, dataOnCopy, DataOn, begin, end, false, factor);
     }
     if (yRight) {
        Coordinates begin(SURI_ROUND(int, dim.GetWidth() - pixelDistanceX), 0);
        Coordinates end(SURI_ROUND(int, dim.GetWidth()), SURI_ROUND(int, dim.GetHeight()));
        double factor = dim.GetWidth() - pixelDistanceX;
        ApplyFeathering(DataOn, dataOnCopy, DataOn, begin, end, true, factor);
     }
     if (yLeft) {
        Coordinates begin(0, 0);
        Coordinates end(pixelDistanceX, SURI_ROUND(int, dim.GetHeight()));
        double factor = 0;
        ApplyFeathering(DataOn, dataOnCopy, DataOn, begin, end, true, factor);
     }
     pixelDistance_ = auxPixel;
     std::vector<void*>::const_iterator it = dataOnCopy.begin();
     for (; it != dataOnCopy.end(); ++it) {
        delete [] static_cast<unsigned char*>(*it);
     }
}


bool MosaicProcess::ConfigureProcess() {
   if (!pRasterModelOut_) return false;
   SetDataSize();
   pAdaptLayer_->GetAttribute<int>(ProcessAdaptLayer::MosaicPixelDistanceAttr, pixelDistance_);
   pAdaptLayer_->GetAttribute<unsigned char>(ProcessAdaptLayer::MosaicNoDataColourAttr,
                                                                                   noDataColour_);
   FileRenderization* red = dynamic_cast<FileRenderization*>(pRenderizationObject_);
   if (pixelDistance_ > 0) {
      std::vector<void*> pixelData(inputElements_.size());
      RasterElement* pRasterOn =  dynamic_cast<RasterElement*>(inputElements_[0]);
      RasterElement* pRasterUnder =  dynamic_cast<RasterElement*>(inputElements_[1]);
      Subset subOn;
      Subset subUnder;
      intersection_ = GetIntersection(pRasterOn, pRasterUnder);
      SetBiggerIntersectionSubset();
      pRasterOn->GetElementExtent(subOn);
      pRasterUnder->GetElementExtent(subUnder);
      pixelData = GetFeatheringData ();
      pRasterModelOut_->InverseTransform(intersection_.ul_);
      pRasterModelOut_->InverseTransform(intersection_.lr_);
      FixSubset(intersection_);
      red->SetFeathering(intersection_, pixelData, noDataColour_);
   }
   red->SetNoDataValue(noDataColour_);
   return FileExporterProcess::ConfigureProcess();
}





RasterSpatialModel* MosaicProcess::GetOutputRasterSpatialModel() {
   if (!pRasterModelOut_) {
      Subset subsetout;
      RasterElement* pelement = dynamic_cast<RasterElement*>(inputElements_[0]);
      pAdaptLayer_->GetAttribute<Subset>(ProcessAdaptLayer::OutputSubsetWindowKeyAttr, subsetout);
      pRasterModelOut_ = RasterSpatialModel::Create(pelement->GetRasterModel());
      RasterSpatialModel::Parameters params = pRasterModelOut_->GetModelParameters();
      params.matrixModel_[0] = subsetout.ul_.x_;
      params.matrixModel_[3] = subsetout.ul_.y_;
      RasterSpatialModel::Destroy(pRasterModelOut_);
      pRasterModelOut_ = RasterSpatialModel::Create(params);
   }
   return pRasterModelOut_;
}



/** Calcula el extent del mosaico **/
void MosaicProcess::SetSubsetOutput(){
   Subset subsetout(-1,-1,-1,-1);
   std::vector<Element*>::const_iterator it = inputElements_.begin();
   std::vector<Subset> extents;
   for (; it != inputElements_.end(); ++it) {
      RasterElement* pelement = dynamic_cast<RasterElement*>(*it);
      if (pelement) {
           Subset extent;
           pelement->GetElementExtent(extent);
           extents.push_back(extent);
           if (extent.IsValid()) {
              if ( (std::abs(subsetout.ul_.x_) > std::abs(extent.ul_.x_))
                                                        || (subsetout.ul_.x_ == -1) )
                                              subsetout.ul_.x_ = extent.ul_.x_;
              if ( (subsetout.ul_.y_ < extent.ul_.y_) || (subsetout.ul_.y_ == -1))
                                              subsetout.ul_.y_ = extent.ul_.y_;

              if  (subsetout.lr_.x_ < extent.lr_.x_)
                                               subsetout.lr_.x_ = extent.lr_.x_;
              if ( (subsetout.lr_.y_ > extent.lr_.y_) || (subsetout.lr_.y_ == -1) )
                                               subsetout.lr_.y_ = extent.lr_.y_;
           }

     }
  }
  pAdaptLayer_->AddSerializableAttribute<Subset>(ProcessAdaptLayer::OutputSubsetWindowKeyAttr,
                                                                                        subsetout);
  return ;
}

World * MosaicProcess::GetOutputWorld(){
   SetSubsetOutput();
   return RasterProcess::GetOutputWorld();
}


}  // namespace
