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

// Includes standart
#include <limits>
#include <vector>
#include <string>
#include <cmath>

// Includes Suri
#include "suri/KMeansClassificationProcess.h"
#include "ColorTable.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/VectorElement.h"
#include "suri/VectorStyle.h"
#include "suri/DataTypes.h"
#include "suri/DatasourceInterface.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/ViewcontextManagerInterface.h"
#include "ClassifiedRasterDatasourceManipulator.h"
#include "suri/DataViewManager.h"
#include "ClassInformation.h"
#include "ClassificationGetter.h"
#include "suri/XmlFunctions.h"
#include "ClassificationRenderer.h"
#include "KMeansAlgorithm.h"
#include "KMeansCanvas.h"
#include "suri/StatisticsCalculator.h"
#include "suri/ClassificationProcess.h"

// Includes gdal
#include "gdal_priv.h"

// Includes wx
#include "wx/wx.h"
#include "wx/progdlg.h"
// Defines
/** Offset (en bytes) para calcular el tamanio libre necesario para salvar */
#define EXTRA_SAVE_SPACE_REQUIRED 1000000
#define PIXEL_NOVALID_DN 0
/** namespace suri */
namespace suri {

/** Nombre del proceso **/
const std::string KMeansClassificationProcess::kProcessName = "KMeansClassificationProcess";
const std::string KMeansClassificationProcess::ThresholdAttrKey = "Threshold";
const std::string KMeansClassificationProcess::IterationsAttrKey = "MaxIterations";
const std::string KMeansClassificationProcess::ClassCountAttrKey = "Classcount";
const std::string KMeansClassificationProcess::MeansMethodAttrKey = "InitialMeans";

/**
 * Constructor
 * @param[in] pInputElement elemento que se quiere clasificar
 * @param[in] WindowSubset subset que se quiere clasificar del elemento
 * @param[out] pOutputElement elemento resultado de clasificar
 */
KMeansClassificationProcess::KMeansClassificationProcess(
      Element *pInputElement, const Subset &WindowSubset,
      DataViewManager* pDataViewManager) :
      FileExporterProcess(pInputElement, WindowSubset, pDataViewManager),
      running_(false), threshold_(0), iterations_(0), classcount_(0) {
   SetProcessName(kProcessName);
}

/** Dtor. */
KMeansClassificationProcess::~KMeansClassificationProcess() {
}

/**
 * Carga en el elemento la tabla con LUT generada a partir de los
 * vectores seleccionados. Los colores y los intervalos los obtiene
 * del ClassificationPart
 * @return informa si pudo configurar elemento
 */
bool KMeansClassificationProcess::ConfigureOutput() {
   if (!FileExporterProcess::ConfigureOutput())
      return false;
   // Genero tabla de colores con clases seleccionadas
   ColorTable colortable;
   colortable.Activate(true);
   /**
    * Se agrega como primer color de la tabla de colores el "valor no
    * valido de la imagen clasificada (el 0)
    */
   colortable.AddColor(ClassInformation::InvalidClassIndex, 1, 1, 1);

   // Agrego colores
   std::vector<ClassInformation> properties;
   suri::ClassificationGetter* pclasspropfunc = NULL;
   if (!pAdaptLayer_
         || !pAdaptLayer_->GetAttribute<ClassificationGetter*>(
               ClassificationProcess::ClassGetterAttrKey, pclasspropfunc)) {
      return false;
   }
   pclasspropfunc->GetClassProperties(properties);
   size_t classcount = properties.size();
   Element* pNewElement = NULL;
   pAdaptLayer_->GetAttribute<Element*>(
               ProcessAdaptLayer::OutputElementKeyAttr, pNewElement);
   DatasourceInterface* pdatasource = DatasourceInterface::Create("RasterDatasource",
                                                                  pNewElement);
   ClassifiedRasterDatasourceManipulator manipulator;
   manipulator.AddNature(pdatasource);
   for (size_t i = 0; i < classcount; i++) {
      colortable.AddColor(properties[i].GetIndex(),
                          properties[i].GetColor().red_,
                          properties[i].GetColor().green_,
                          properties[i].GetColor().blue_);
      if (i == classcount - 1) {
         colortable.AddColor(std::numeric_limits<double>::max(),
                             properties[i].GetColor().red_,
                             properties[i].GetColor().green_,
                             properties[i].GetColor().blue_);
      }
      manipulator.AddClassInformation(&properties[i], pdatasource, true);
   }

   // Genero nodo con datos parameters
   wxXmlNode* pctnode = NULL;
   if (!ColorTable::ToXml(colortable, pctnode)) {
      REPORT_DEBUG("Fallo al generar nodo de tabla de colores");
      return false;
   }

   // Modifico la LUT del raster
   wxXmlNode* prenderernode = pNewElement->GetNode(wxT(RENDERIZATION_NODE));
   wxString oldlutnodepath = wxString(wxT(RENDERIZATION_NODE)) + wxT("|")
         + wxString(wxT(LUT_NODE));
   wxXmlNode* poldlutnode = pNewElement->GetNode(oldlutnodepath.c_str());
   if (poldlutnode) {
      prenderernode->InsertChildAfter(pctnode, poldlutnode);
      prenderernode->RemoveChild(poldlutnode);
   } else {
      prenderernode->AddChild(pctnode);
   }
   /** se configura como valor no valido el valor de la clase no valida **/
   pdatasource->GetElement()->SetNoDataValue(ClassInformation::NDVIndex);

   ExportMetadataToFile(pdatasource, pdatasource->GetElement()->GetUrl().c_str());

   return pDataViewManager_->GetDatasourceManager()->AddDatasource(pdatasource);
}

/** Obtiene las estadisticas de la imagen **/
bool KMeansClassificationProcess::GetStatistics(std::vector<int> BandIndex, RasterElement* pRaster,
               std::vector<double>& Min, std::vector<double>& Max,
               std::vector< std::vector<int> > &Histogram) {
   suri::raster::data::StatisticsBase* pstatistics = NULL;
   suri::raster::data::HistogramBase* phistogram = NULL;
   suri::raster::data::StatisticsCalculator stats(pRaster);
   stats.CalculateStatistics(pstatistics, phistogram, true);
   if (pstatistics) {
      for (size_t b = 0; b < BandIndex.size(); b++) {
         Min[b] = pstatistics->pMin_[BandIndex[b]];
         Max[b] = pstatistics->pMax_[BandIndex[b]];
      }
   }
   if (phistogram)  {
      for (size_t b = 0; b < BandIndex.size(); b++)
         for (int bin = 0; bin < phistogram->GetNumBins()[BandIndex[b]]; ++bin)
            Histogram[b][bin] = phistogram->GetBins()[BandIndex[b]][bin];
   }
   return pstatistics != NULL;
}

/** Actuaiza los centroides de las clases */
void KMeansClassificationProcess::UpdateMeans(int Classes, int BandCount,
                           const std::vector< std::vector<double > >& ClassCentreData,
                           const std::vector<int>& PixelCount,
                           std::vector<std::vector<double> >& Means) {
   for (int a = 0; a < Classes; a++) {
      if (PixelCount[a] > 0) {
         double* pnewclasscentre = new double[BandCount];
         for (int i = 0; i < BandCount; i++) {
            pnewclasscentre[i] = ClassCentreData[a][i] / static_cast<double>(PixelCount[a]);
         }
         for (int i = 0; i < BandCount; i++) {
            Means[a][i] = pnewclasscentre[i];
         }
         delete[] pnewclasscentre;
      }
   }
}


/** Metodo auxiliar que configura los metadatos de la imagen que genera el proceso de clasificacion
 * @param[in] pDataset dataset que representa la imagen a clasificar y de la cual se tomara la metadata
 * @param[in] XOffset pixel inicial 
 * @param[in] XBlockSize tamanio de bloque en x
 * @param[in] InitialLine linea inicial (proviene de la seleccion espacial)
 * @param[in] YBlockSize tamanio de bloque en y
 * @param[in] SpatialReference Sistema de referencia 
 * @param[out] pDestDataset dataset sobre el cual se configurara la metadata
 **/
void KMeansClassificationProcess::ConfigureMetadata(GDALDataset* pDataset, int XOffset, int XBlockSize,
                                 int InitialLine, int YBlockSize,
                                 const std::string& SpatialReference, GDALDataset* pDestDataset) {
   /** Configuracion de seleccion espacial **/
   double* pgeotransform = new double[6];
   pDataset->GetGeoTransform(pgeotransform);
   int srcwin[4];
   // xoff
   srcwin[0] = XOffset;
   // yoff
   srcwin[1] = InitialLine;
   // xsize
   srcwin[2] = XBlockSize;
   // ysize
   srcwin[3] = YBlockSize;
   pgeotransform[0] += srcwin[0] * pgeotransform[1] + srcwin[1] * pgeotransform[2];
   pgeotransform[3] += srcwin[0] * pgeotransform[4] + srcwin[1] * pgeotransform[5];
   pgeotransform[1] *= srcwin[2] / static_cast<double>(XBlockSize);
   pgeotransform[2] *= srcwin[3] / static_cast<double>(YBlockSize);
   pgeotransform[4] *= srcwin[2] / static_cast<double>(XBlockSize);
   pgeotransform[5] *= srcwin[3] / static_cast<double>(YBlockSize);
   pDestDataset->SetGeoTransform(pgeotransform);
   pDestDataset->SetProjection(pDataset->GetProjectionRef());
   delete[] pgeotransform;

   // configuro el sistema de referencia de la imagen de saldia **/
   pDestDataset->SetMetadata(pDataset->GetMetadata(), "");
   pDestDataset->SetProjection(SpatialReference.c_str());
   if (pDataset->GetMetadata("INTERLEAVE"))
      pDestDataset->SetMetadata(pDataset->GetMetadata("INTERLEAVE"), "INTERLEAVE");
   if (pDataset->GetMetadata("IMAGE_STRUCTURE"))
      pDestDataset->SetMetadata(pDataset->GetMetadata("IMAGE_STRUCTURE"), "IMAGE_STRUCTURE");
   if (pDataset->GetMetadata("RPC"))
      pDestDataset->SetMetadata(pDataset->GetMetadata("RPC"), "RPC");
   if (pDataset->GetMetadata("GEOLOCATION"))
      pDestDataset->SetMetadata(pDataset->GetMetadata("GEOLOCATION"), "GEOLOCATION");
}

/** A traves del algoritmo de kmeans clasifica los pixeles del bloque se pasa por parametro
 *  @param[in] BlockData bloque que tiene los pixeles a clasificar
 *  @param[in] BlockSize tamanio del bloque
 *  @param[in] BandCount cantidad e bandas
 *  @param[in] NoDataValue valor no valido
 *  @param[in] NoDataValueEnable bool que indica si se encuentra configurado el valor no valido
 *  @param[in] InitialMeans medias iniciales al momento de procesar el bloque
 *  @param[out] StepMeans resultantes del procesamiento del bloque
 *  @param[out] PixelCount conteo de pixeles clasificados por clase del bloque
 *  @param[out] pOutputData bloque en donde se colocaran los pixeles clasificados
**/
void KMeansClassificationProcess::ClassifyBlock(std::vector<float*>& BlockData, int BlockSize,
                           int BandCount, double NoDataValue, bool NoDataValueEnable, 
                           const std::vector<std::vector<double> >& InitialMeans,
                           std::vector<std::vector<double> >& StepMeans,
                           std::vector<int>& PixelCount, float* pOutputData) {
   double pixeldistance = 0.0;
   for (int p = 0; p < BlockSize; ++p) {
      bool pixelinvalid = true;
      int pixelclass = 0;
      float pixelmindistance = std::numeric_limits<float>::max();
      double precision = 0.00000001;
      for (int c = 0; c < classcount_; ++c) {
         pixeldistance = 0;
         for (int b = 0; b < BandCount; ++b) {
            double diff = BlockData[b][p] - InitialMeans[c][b];
            // el pixel es no valido si en todas sus bandas el valor del mismo es el no valido
            pixelinvalid = NoDataValueEnable ?
                           pixelinvalid && fabs(BlockData[b][p] - NoDataValue) < precision :
                           false;
            pixeldistance += diff * diff;
         }
         if (pixeldistance < pixelmindistance) {
            pixelclass = c;
            pixelmindistance = pixeldistance;
         }
      }
      // el valor de la clase comienza a partir del no valido (0)
      pOutputData[p] = pixelinvalid ? 255 : pixelclass + 1;
      if (!pixelinvalid)
         PixelCount[pixelclass]++;
      for (int b = 0; b < BandCount; ++b)
         StepMeans[pixelclass][b] += BlockData[b][p];
   }
}

/** Corre el proceso y genera la salida usando los metodos de configuracion */
bool KMeansClassificationProcess::RunProcess() {
   if (!pAdaptLayer_->GetAttribute<double>(ThresholdAttrKey, threshold_) ||
                     !pAdaptLayer_->GetAttribute<int>(IterationsAttrKey, iterations_) ||
      !pAdaptLayer_->GetAttribute<int>(ClassCountAttrKey, classcount_)) {
      return false;
   }
   int method = 0;
   GDALDataset* pdataset =
      static_cast<GDALDataset*>(GDALOpen(inputElements_[0]->GetUrl().c_str(), GA_ReadOnly));
   /** Se crea la ventana de avance **/
   wxProgressDialog* pprogress = new wxProgressDialog(_("Clasificando imagen..."), "", 100);
   pprogress->SetSize(wxSize(400, -1));
   pprogress->Centre(wxCENTRE_ON_SCREEN);

   pAdaptLayer_->GetAttribute<int>(MeansMethodAttrKey, method);
   std::vector<int> bands = GetSelectedRasterBands();
   int bcount = bands.size();
   std::vector<double> mins(bcount), maxs(bcount);
   std::vector< std::vector<int> > histograms(bcount, std::vector<int>(256));
   pprogress->Update(0, _("Obteniendo medias iniciales..."));
    std::string filename;
   Option option;
   GetSelectedOutput(filename, option);

   bool finish = false;
   /** Creacion de imagen clasificada **/
   // Obtengo metadata de la imagen origen
   char** poptions = NULL;
   RasterSpatialModel* prmodel = GetOutputRasterSpatialModel();
   World* pworld = GetOutputWorld();
   Subset spatialselection;
   pworld->GetWorld(spatialselection);
   Subset worldin = spatialselection;
   prmodel->InverseTransform(spatialselection.ul_);
   prmodel->InverseTransform(spatialselection.lr_);
   REPORT_DEBUG("Seleccion espacial en PL ulx=%f uly=%f lrx=%f lry=%f", spatialselection.ul_.x_,
                  spatialselection.ul_.y_, spatialselection.lr_.x_, spatialselection.lr_.y_);

   int xblocksize = spatialselection.lr_.x_ - spatialselection.ul_.x_;
   int xoffset = spatialselection.ul_.x_;
   int yblocksize = spatialselection.lr_.y_ - spatialselection.ul_.y_;
   int initialine = spatialselection.ul_.y_;
   int finalline = spatialselection.lr_.y_;
   // Para calculo de medias random no hace falta calcular estadisticas
   if (method != RandomMeans)
      GetStatistics(bands, dynamic_cast<RasterElement*>(inputElements_[0]), mins, maxs, histograms);

   std::vector<std::vector<double> > initialmeans =
                  GetInitialMeans(method, classcount_, bcount, mins, maxs, histograms,
                                 spatialselection, pdataset);
   /* avanza un 10% **/
   pprogress->Update(10);
#ifdef __DEBUG__
   for (int classposs= 0; classposs < classcount_; classposs++) {
      for (size_t i = 0; i < initialmeans[classposs].size(); ++i)
         REPORT_DEBUG("Class #%d Band #%d Mean %f", classposs, i, initialmeans[classposs][i]);
   }
#endif
 GDALDriverH hdriver = GDALGetDriverByName(option.GetOption("Format").c_str());
   GDALDataset* pdestdataset = (GDALDataset*)GDALCreate(hdriver, filename.c_str(), xblocksize, yblocksize, 1,
                                          GDT_Int32, poptions);
   /** Configuracion de seleccion espacial y metadata**/
   ConfigureMetadata(pdataset, xoffset, xblocksize, initialine, yblocksize,
                     pworld->GetSpatialReference(), pdestdataset);
   bool ndvavailable = true;
   double nodatavalue;
   inputElements_[0]->GetNoDataValue(ndvavailable, nodatavalue);
   for (int iteration = 0; !finish && iteration < iterations_; ++iteration) {
      std::string msg = "iteracion " + NumberToString<int>(iteration);
      double progress = 10.0 + static_cast<double>(iteration)*80.0/static_cast<double>(iterations_);
      pprogress->Update(ceil(progress), msg);
      // load buffer data
      std::vector<float*> bufferdata(bcount);
      std::vector<int> pixelcount(classcount_);
      std::vector< std::vector<double > > classcentredata(classcount_, std::vector<double>(bcount));
      float* outputdata = NULL;
      // spatialselection.lr_.y_ representa el numero de linea final de la seleccion espacial
      // spatialselection.ul_.y_ representa el numero de linea inicial de la seleccion espacial
      for (int y = initialine, y0 = 0; y < finalline; ++y, y0++) {
        outputdata = new float[xblocksize];
        for (int b = 0; b < bcount; ++b) {
            // se utiliza la seleccion espectral para saber de que banda leer
            GDALRasterBand* pband = pdataset->GetRasterBand(bands[b]+1);
            bufferdata[b] = new float[xblocksize];
            // force to load pixel as float value using rasterio method
            // spatialselection.ul_.x_: representa el offset en pixeles a leer
            pband->RasterIO(GF_Read, xoffset, y, xblocksize, 1, bufferdata[b],
                           xblocksize, 1, GDT_Float32, 0, 0);
         }
         // Se clasifica el bloque
         ClassifyBlock(bufferdata, xblocksize, bcount, nodatavalue, ndvavailable,
                      initialmeans, classcentredata, pixelcount, outputdata);
         // Se escribe el resultado en la imagen de salida
         GDALRasterIO(pdestdataset->GetRasterBand(1), GF_Write, 0, y0, xblocksize, 1,
                      outputdata, xblocksize, 1, GDT_Float32, 0, 0);
         // free block mem
         for (size_t i = 0; i < bufferdata.size(); ++i)
            delete[] bufferdata[i];
         delete[] outputdata;
      }
      // Update the class centroids
      std::vector< std::vector<double> > currentmeans = initialmeans;
      UpdateMeans(classcount_, bcount, classcentredata, pixelcount, currentmeans);
      // end condition: percent of pixels changed < threshold
      double percentchange = CalculateChange(initialmeans, currentmeans);
      finish = percentchange < threshold_ /100.0;
      initialmeans = currentmeans;
#ifdef __DEBUG__
      REPORT_DEBUG("Iteration# %d resume", iteration);
      for (int c = 0; c < classcount_; ++c)
         REPORT_DEBUG("Class #%d pixelcount=%d", c, pixelcount[c]);
#endif
   }
   pprogress->Update(90, _("Configurando imagen de salida..."));

   GDALFlushCache(pdestdataset);
   GDALClose(pdestdataset);
   GDALClose(pdataset);
   bool success = ConfigureOutput();
   pprogress->Update(100);
   return success;
}

/**
 *  En funcion del minimo y maximo de la imagen (por banda) se generan clases iniciales de forma
 *  aleatoria.
 * @param[in] ClassCount cantidad de clases
 * @param[in] BandCount cantidad de bandas
 * param[in] pStatistics estadisticas de la imagen sobre la cual se calcularan las clases iniciales
 * @param[out] InitialMeans array con las medias iniciales
**/
std::vector<std::vector<double> > KMeansClassificationProcess::GetPartialRandomMeans(
      int Classes, int BandCount, const std::vector<double>& Min,
      const std::vector<double>& Max) {
   std::vector<std::vector<double> > means;
   for (int i = 0; i < Classes; i++) {
      means.push_back(std::vector<double>());      
      for (int j = 0; j < BandCount; j++) {
         double range = Max[j] - Min[j];
         // se genera de forma aleatoria un vlaor dentro del rango de valores de la banda
         double random = (static_cast<double>(rand()) / RAND_MAX);
         double value = Min[j] + random * range;
         means[i].push_back(value);
      }
   }
   return means;
}

/**
 * En funcion del minimo y maximo de la imagen (por banda) se generan clases iniciales 
 * utilizando maxima dispercion por la diagonal
 * @param[in] ClassCount cantidad de clases
 * @param[in] BandCount cantidad de bandas
 * param[in] pStatistics estadisticas de la imagen sobre la cual se calcularan las clases iniciales
 * @param[out] InitialMeans array con las medias iniciales
**/
std::vector<std::vector<double> >KMeansClassificationProcess::GetMaxDispersionMeans(int Classes,
                                          int BandCount,
                                          const std::vector<double>& Min,
                                          const std::vector<double>& Max) {
   std::vector< std::vector<double> > means;
   for (int i = 0; i < Classes; i++) {
      means.push_back(std::vector<double>());
      for (int j = 0; j < BandCount; j++) {
         double range = Max[j] - Min[j];
         double spacing = range / static_cast<double>(Classes);
         /** se genera de forma aleatoria un vlaor dentro del rango de valores de la banda**/
         double value = Min[j] + spacing * static_cast<double>(i);
         means[i].push_back(value);
      }
   }
   return means;
}

/**
 * Se toma como media inicial el valor dn de un pixel en la posicion (pixel,linea) de
 * aleatoria
 * @param[in] ClassCount cantidad de clases
 * @param[in] BandCount cantidad de bandas
 * param[in] pStatistics estadisticas de la imagen sobre la cual se calcularan las clases iniciales
 * @param[out] InitialMeans array con las medias iniciales
**/
std::vector< std::vector<double> > KMeansClassificationProcess::GetRandomMeans(int Classes,
                                    int BandCount,
                                    const Subset& SpatialSelection,
                                    GDALDataset* pSource) {
   std::vector< std::vector<double> > means;
   for (int i = 0; i < Classes; i++) {
      means.push_back(std::vector<double>());
      for (int j = 0; j < BandCount; j++) {
         GDALRasterBand* pband = pSource->GetRasterBand(j+1);
         int rangex = fabs(SpatialSelection.lr_.x_ - SpatialSelection.ul_.x_);
         int rangey = fabs(SpatialSelection.lr_.y_ - SpatialSelection.ul_.y_);
         int x = SpatialSelection.ul_.x_ + (static_cast<double>(rand()) / RAND_MAX)*rangex;
         int y = SpatialSelection.ul_.y_ + (static_cast<double>(rand()) / RAND_MAX)*rangey;
         float* pvalue = new float;
         // leo el pixel en la posicion random
         pband->RasterIO(GF_Read, x, y, 1, 1, pvalue, 1, 1, GDT_Float32, 0, 0);
         means[i].push_back(*pvalue);
         delete pvalue;
      }
   }
   return means;
}

/** Calcula las medias iniciales en funcion del metodo deseado **/
std::vector<std::vector<double> > KMeansClassificationProcess::GetInitialMeans(
      int Method, int Classes, int BandCount, const std::vector<double>& Min,
      const std::vector<double>& Max, const std::vector<std::vector<int> >& Histogram,
      const Subset& SpatialSelection, GDALDataset* pSource) {
   std::vector<std::vector<double> > means;
   switch (Method) {
      case PartialRandomMeans:
         means = GetPartialRandomMeans(Classes, BandCount, Min, Max);
         break;
      case MaximumDispersionMeans:
         means = GetMaxDispersionMeans(Classes, BandCount, Min, Max);
         break;
      case QuantilMeans:
         means = GetQuantilMeans(Classes, BandCount, Min, Max, Histogram);
         break;
      case ModeMeans:
         means = GetModeMeans(Classes, BandCount, Min, Max, Histogram);
         break;
      default:
         means = GetRandomMeans(Classes, BandCount, SpatialSelection, pSource);
         break;
   }
   return means;
}

/**
 * En funcion del histograma de la imagen asigna medias utilizando los extremos inferiores 
 * de los bins que tengan la mayor cantidad de pixeles (mayor frecuencia en la imagen)
**/
std::vector< std::vector<double> > KMeansClassificationProcess::GetModeMeans(int Classes,
                                                int BandCount,
                                                const std::vector<double>& Min,
                                                const std::vector<double>& Max,
                                                const std::vector< std::vector<int> >& Histograms) {
   std::vector<std::vector<int> > maxbinsperband(BandCount, std::vector<int>(Classes, 0));
   std::vector<std::vector<int> > binpixelcountperband(BandCount,
                                 std::vector<int>(Classes, std::numeric_limits<int>::min()));
   // Get bin with best frequency
   for (int band = 0; band < BandCount; ++band) {
      for (int bin = 0; bin < 256; ++bin) {
         for (int i = 0; i < Classes; ++i)
            if (binpixelcountperband[band][i] < Histograms[band][bin]) {
               maxbinsperband[band][i] = bin;
               binpixelcountperband[band][i] = Histograms[band][bin];
               break;
            }
      }
   }
   std::vector< std::vector<double> > means;
   for (int i = 0; i < Classes; i++) {
      means.push_back(std::vector<double>());
      for (int j = 0; j < BandCount; j++) {
         double range = (Max[j] - Min[j]) / 256.0;
         double mean = Min[j] + maxbinsperband[j][i]*range;
         means[i].push_back(mean);
      }
   }
   return means;
}

/** En funcion del histograma asociado a la imagen asigna medias en intervalos uniformes
 *  en cantidad de pixeles 
 */
std::vector<std::vector<double> > KMeansClassificationProcess::GetQuantilMeans(
      int Classes, int BandCount, const std::vector<double>& Min,
      const std::vector<double>& Max, const std::vector<std::vector<int> >& Histograms) {
   std::vector<int> pixelcount(BandCount);
   for (size_t i = 0; i < Histograms.size(); ++i)
      for (size_t j = 0; j < Histograms[i].size(); ++j)
         pixelcount[i] += Histograms[i][j];
   std::vector< std::vector<double> >binranges(BandCount, std::vector<double>(Classes));
   for (int j = 0; j < BandCount; ++j) {
         int bin = 0;
         int pixelperbin = pixelcount[j] / Classes;
         for (int c = 0; c < Classes; ++c) {
            int pixelbincount = 0;
            for (; pixelbincount < pixelperbin && bin < 256; ++bin)
               pixelbincount += Histograms[j][bin];
            binranges[j][c] = bin;
         }
   }
   std::vector< std::vector<double> > means;
   for (int i = 0; i < Classes; i++) {
      means.push_back(std::vector<double>());
      for (int j = 0; j < BandCount; j++) {
         double range = (Max[j] - Min[j]) / 256.0;
         double value = binranges[j][i]*range;
         means[i].push_back(value);
      }
   }
   return means;
}

/**
 * Calcula que porcentaje se modificaron las medias. Para cada media calcula la
 * diferencia entre la nueva posicion y la anterior y la divide por el modulo
 * de la nueva media. Se queda con la distancia mas grande.
 * @param[in] InitialMeans las medias que se usaron como entrada a iteracion
 * @param[in] NextMean las medias que se obtubieron de la iteracion
 * @return maximo porcentaje de modificacion entre todas las medias.
 * return = |NextMean-InitialMean|/|NextMean|
 */
double KMeansClassificationProcess::CalculateChange(
      const std::vector<std::vector<double> > &InitialMeans,
      const std::vector<std::vector<double> > &NextMean) {
   double maxdistance = 0;
   double currentdistance = 0;
   double mean_norm = 0;
   double diference = 0;
   // Calculo la diferencia porcentual mas grande entre todas las medias.
   for (size_t i = 0; i < InitialMeans.size(); i++) {
      diference = 0;
      mean_norm = 0;
      currentdistance = 0;
      for (size_t j = 0; j < InitialMeans[i].size(); j++) {
         diference += (NextMean[i][j] - InitialMeans[i][j]);
         mean_norm += NextMean[i][j] * NextMean[i][j];
         currentdistance += diference * diference;
      }
      currentdistance = currentdistance / mean_norm;
      if (currentdistance > maxdistance)
         maxdistance = currentdistance;
   }
   return maxdistance;
}

/** 
 * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
 * agreguen sus atributos especificos
 * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
 * @return vector vacio en caso de que no tenga parametros adicionales
**/
std::vector<wxXmlNode*> KMeansClassificationProcess::DoGetXmlParameters() const {
   std::vector<wxXmlNode*> params;
   params.push_back(CreateKeyValueNode("iteraciones", NumberToString<int>(iterations_)));
   params.push_back(CreateKeyValueNode("umbral", NumberToString<double>(threshold_)));
   int method = 0;
   pAdaptLayer_->GetAttribute<int>(MeansMethodAttrKey, method);
   params.push_back(CreateKeyValueNode("metodo", NumberToString<int>(method)));
   params.push_back(CreateKeyValueNode("clases", NumberToString<int>(classcount_)));
   return params;
}

}  /** namespace suri **/
