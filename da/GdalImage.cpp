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

#ifdef __GDAL__

// Includes standard
#include <string>
#include <map>
#include <utility>
#include <cmath>

// Includes suri
#include "GdalImage.h"
#include "GdalDriver.h"
#include "GdalRasterWriter.h"
#include "suri.h"
#include "suri/DataTypes.h"
#include "suri/RasterSpatialModel.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Configuration.h"
#include "gdal_priv.h"
#include "locale.h"

namespace {

/**
 * Funcion que establece una variable de entorno para el proceso.
 *
 * \todo(danieltesta-#5085): Mover, si tiene sentido, a un lugar mas
 * global.
 */
void PutEnvEx(const std::string& Key, const std::string& Val) {
   // sumo size + 2: 1 para el = y otro para el \0
   /** Aclaracion: la memoria se libera cuando se termina el proceso.
    * Si es eliminada luego del putenv se pierde la referencia a la configuracion
    * de la variable de entorno. Esto pasa en linux, en windows el CRT realiza
    * una copia de la misma **/
   size_t cmdsize = Key.size() + Val.size() + 2;
   char* pstr = new char[cmdsize];
   sprintf(pstr, "%s=%s", Key.c_str(), Val.c_str());
   putenv(pstr);
}

}  // namespace anonimo

// Defines
/** Lista de drivers de los formatos de imagenes soportados por GDAL */
#define GDAL_SUPPORTED_FORMATS \
   "AAIGrid ADRG AIG AIRSAR BLX BMP BSB BT CEOS COASP COSAR CPG DIMAP DIPEx DODS DOQ1 DOQ2 DTED ECW EHdr EIR ELAS ENVI EPSILON ERS ESAT FAST FIT FITS FujiBAS GENBIN GEORASTER GFF GIF GRIB GMT GRASS GSAG GSBG GS7BG GSC GTiff GXF HDF4 HDF5 HFA IDA ILWIS INGR ISIS2 ISIS3 PALSARJaxa JDEM JPEG JPEG2000 JP2KAK JP2ECW JP2MrSID L1B LAN LCP Leveller MEM MFF MFF2 MrSID MSG MSGN NDF NITF netCDF OGDI PAux PCIDSK PCRaster PDS PNG PNM R Rasterlite RIK RMF RPFTOC RS2 RST SAR_CEOS SDE SDTS SGI SRP SRTMHGT TERRAGEN Terralib TIL TSX USGSDEM VRT WCS WKTRaster WMS XPM"

/** namespace suri */
namespace suri {

/** Mapeo entre los tipos GDAL a los tipos C/C++ */
typedef std::map<GDALDataType, std::pair<std::string, int> > Gdal2SuriDataType;
/** GdalDataTypeMap Variable del tipo Gdal2SuriDataType */
static Gdal2SuriDataType GdalDataTypeMap;
/** Mapeo entre los tipos C/C++ a los tipos GDAL */
typedef std::map<std::string, GDALDataType> Suri2GdalDataType;
/** SuriDataTypeMap Variable del tipo Suri2GdalDataType */
static Suri2GdalDataType SuriDataTypeMap;
/** Mapeo entre la constante de acceso GDAL y suri */
typedef std::map<GDALAccess, Image::ImageAccessType> Gdal2SuriAccessType;
/** GdalAccessTypeMap Variable del tipo Gdal2SuriAccessType */
static Gdal2SuriAccessType GdalAccessTypeMap;
/** Mapeo inverso entre constantes Suri y GDAL */
typedef std::map<Image::ImageAccessType, GDALAccess> Suri2GdalAccessType;
/** SuriAccessTypeMap Variable del tipo Suri2GdalAccessType */
static Suri2GdalAccessType SuriAccessTypeMap;

/**  */
const std::string GdalImage::SUBDATASET_OPTION = "SDS";
/** */
const std::string GdalImage::SUBDATASET_SEPARATOR = ";";

/**
 * Constructor
 *  Inicializa pDataset_ en nulo.
 */
GdalImage::GdalImage() :
      pDataset_(NULL) {
}

/** Destructor */
GdalImage::~GdalImage() {
   // solo si el dataset existe
   if (pDataset_) {
      GDALClose(pDataset_);
   }
}

/** Inspecciona el archivo, para saber si es del formato que maneja */
/**
 *  Esta funcion analiza el archivo y el tipo de acceso para determinar si es
 * el formato que esta clase maneja.
 *
 *  En caso de ImageAccess==WriteOnly, recibe parametros extra y determina si
 * puede crear un archivo para escritura con los parametros deseados.
 *
 *  Se llama desde el metodo Image::Open().
 *
 *  Este metodo debe fallar de la forma mas rapida posible.
 *
 *  En el caso de datasets, devuelve falso para tratarlos en la clase GdalSubdatasetImage
 *
 * \todo Soportar escritura
 * \todo modificar cuando haya soporte para subdatasets
 *
 * @param[in] Filename Nombre del archivo
 * @param[in] ImageAccess tipo de acceso a la imagen
 * @param[in] BandCount cantidad de bandas (solo para ImageAccess==WriteOnly)
 * @param[in] SizeX cantidad de columnas (solo para ImageAccess==WriteOnly)
 * @param[in] SizeY cantidad de filas (solo para ImageAccess==WriteOnly)
 * @param[in] DataType tipo de dato raster (solo para ImageAccess==WriteOnly)
 * @return bool True: se reconocio el formato; False: error o no se reconocio
 *  el formato
 */
bool GdalImage::Inspect(const std::string &Filename, ImageAccessType ImageAccess,
                        int BandCount, int SizeX, int SizeY,
                        const std::string &DataType) {

   // \todo soportar lecto-escritura
   if (ImageAccess == ReadWrite) {
      return false;
   }

   GDALDriver* pdriver = NULL;

   // Seteo de la configuracion numerica local a config EEUU
   std::string localconfiguration(setlocale(LC_NUMERIC, NULL));
   setlocale(LC_NUMERIC, "C");

   // En caso de imagen para escritura, creo el driver y a partir de el el
   // dataset
   if (ImageAccess == WriteOnly) {
      std::string format = GetOption(Format);
      // los drivers no se eliminan?
      pdriver = GetGDALDriverManager()->GetDriverByName(format.c_str());
      if (pdriver) {
         // verifico que el driver pueda usar Create()
         if (CSLFetchBoolean(GDALGetMetadata(pdriver, NULL), GDAL_DCAP_CREATE, FALSE)) {
            char **poptions = NULL;
            pDataset_ = pdriver->Create(Filename.c_str(), SizeX, SizeY, BandCount,
                                        SuriDataTypeMap[DataType], poptions);
         }
      }

      // Contemplo el hecho de querer actualizar un archivo existente.
      // \todo(danieltesta-#5214): WriteOnly no deberia implicar solo creacion.
      // Ver si se puede mejorar esta parte mas adelante.
      if (pDataset_ == NULL) {
         GDALDatasetH hraster = GDALOpen(Filename.c_str(), GA_Update);
         if (hraster) {
            GDALDriverH hdriver = GDALGetDatasetDriver(hraster);
            if (hdriver) {
               pdriver = static_cast<GDALDriver*>(hdriver);
               pDataset_ = static_cast<GDALDataset*>(hraster);
            }
         }
      }
   } else {
      pDataset_ = static_cast<GDALDataset*>(GDALOpen(Filename.c_str(),
                                                     SuriAccessTypeMap[ImageAccess]));
      if (pDataset_ == NULL) {
         return false;
      }
      // Si se determina que es una imagen con datasets y no tiene bandas,
      // agtegadevuelve error
      // En el caso de tener datasets, la clase que se encarga es GdalSubdatasetImage
      if (pDataset_->GetRasterCount() == 0 && pDataset_->GetMetadata("SUBDATASETS") != NULL) {
          SetOption(SUBDATASET_OPTION, GetDatasets());
          setlocale(LC_NUMERIC, localconfiguration.c_str());
          return true;
      }

      pdriver = pDataset_ ? pDataset_->GetDriver() : NULL;
   }
   setlocale(LC_NUMERIC, localconfiguration.c_str());


   // Si alguna condicion se cumple => Falla
   // 1) No hay driver
   // 2) No tiene capacidad de creacion y se pidio escritura
   // 3) No tiene bandas
   if (pDataset_ == NULL || pdriver == NULL) {
      GDALClose(pDataset_);
      return false;
   }
   return true;
}

/**
 * Devuelve una lista de strings con los url de los subdatasets separados por
 * SUBDATASET_SEPARATOR
 * */
std::string GdalImage::GetDatasets() {
   std::string name = "";
   std::string selectedsubdataset;
   char **pmetadata = CSLDuplicate(pDataset_->GetMetadata("SUBDATASETS"));
   int count = CSLCount(pmetadata);
   for (int i = 0; i < count; i++) {
      std::string field = CSLGetField(pmetadata, i);
      char** pkey = NULL;
      std::string subdatasetname = CPLParseNameValue(field.c_str(), pkey);
      GDALDataset *pdataset =
            static_cast<GDALDataset*>(GDALOpen(subdatasetname.c_str(), GA_ReadOnly));
      if (pdataset != NULL) {
         name.append(subdatasetname);
         name.append(GdalImage::SUBDATASET_SEPARATOR);
         GDALClose(pdataset);
      }
   }
   CSLDestroy(pmetadata);
   // Elimino el ultimo separador se inserta de mas
   int size = name.size();
   if (size > 0) {
      name.erase(size - 1);
   }
   return name;
}

/**
 * Obtiene los metadatos de la imagen.
 *
 * @return Devuelve un mapa con las categorias y por cada categoria
 * un mapa con los pares clave-valor correspondientes.
 */
std::map<std::string, std::map<std::string, std::string> > GdalImage::GetMetadata(
      const std::vector<std::string>& Domains) const {
   std::map<std::string, std::map<std::string, std::string> > retmap;

   for (int ix = 0, lenix = Domains.size(); ix < lenix; ++ix) {
      char** ppmetadata = pDataset_->GetMetadata(Domains[ix].c_str());
      if (CSLCount(ppmetadata) > 0) {
         std::map<std::string, std::string> metadatakv;

         for(int jx = 0; ppmetadata[jx] != NULL; ++jx) {
            std::vector<std::string> kv = SplitEx(std::string(ppmetadata[jx]), std::string("="));
            metadatakv[kv[0]] = kv[1];
         }

         retmap[Domains[ix]] = metadatakv;
      }
   }

   return retmap;
}

/**
 * Establece el sistema de referencia.
 */
void GdalImage::SetSrs(const std::string& Wkt) {
   if (Wkt.empty())
      return;

   pDataset_->SetProjection(Wkt.c_str());
   pDataset_->FlushCache();
}

/**
 * Establece la transformacion para georreferenciacion.
 */
void GdalImage::SetGeoTransform(double* pGeoTransform) {
   if (pGeoTransform == NULL)
      return;

   pDataset_->SetGeoTransform(pGeoTransform);
   pDataset_->FlushCache();
}


/** Carga la imagen */
/**
 *  Metodo que realiza la lectura completa del archivo en el modo de lectura
 * deseado. Carga las bandas e inicializa los drivers.
 *
 * @param[in] Filename Nombre del archivo.
 * @param[in] ImageAccess tipo de acceso a la imagen
 * @param[in] BandCount cantidad de bandas (solo para ImageAccess==WriteOnly)
 * @param[in] SizeX cantidad de columnas (solo para ImageAccess==WriteOnly)
 * @param[in] SizeY cantidad de filas (solo para ImageAccess==WriteOnly)
 * @param[in] DataType tipo de dato raster (solo para ImageAccess==WriteOnly)
 * @return this.
 */
Image* GdalImage::Load(const std::string &Filename, ImageAccessType ImageAccess,
                       int BandCount, int SizeX, int SizeY,
                       const std::string &DataType) {
   if (!pDataset_) {
      return this;
   }

   // inicializacion de la clase para soporte de lectura (y potencialmente actualizacion)
   if (ImageAccess != WriteOnly) {
      InitializeRead(Filename);
   }
   // Inicializacion de la clase para soporte de escritura (y potencialmente de conslta)
   if (ImageAccess != ReadOnly) {
      InitializeWrite(Filename);
   }

   return this;
}

/**
 * Inicializa para lectura
 * @param[in] Filename nombre de archivo a leer.
 */
void GdalImage::InitializeRead(const std::string &Filename) {
   int bandcount = pDataset_->GetRasterCount(), xblocksize = 0, yblocksize = 0;
   for (int i = 0; i < bandcount; i++) {
      std::pair<std::string, int> datatype =
      // +1 porque empieza desde 1
            GdalDataTypeMap[pDataset_->GetRasterBand(i + 1)->GetRasterDataType()];
      RasterDriver* pdriver = new GdalDriver(pDataset_, Filename, datatype.first, i,
                                             datatype.second);
      if (!pdriver) {
         continue;
      }

      // Seteo formato y descripcion.
      SetOption(Format, GDALGetDriverShortName(GDALGetDatasetDriver(pDataset_)));
      SetOption(FormatDescription, GDALGetDriverLongName(GDALGetDatasetDriver(pDataset_)));

      RasterBand* pband = RasterBand::Create(datatype.first, pdriver, this);
      if (pband) {
         pband->GetBlockSize(xblocksize, yblocksize);
         bandVector_.push_back(pband);
         driverVector_.push_back(pdriver);
      } else {
         delete pdriver;
      }
   }
   std::string projection = pDataset_->GetProjectionRef();
   if (!projection.empty()) {
      SetOption(Georreference, projection);
   }
   double pmodel[6];
   if (pDataset_->GetGeoTransform(pmodel) == CE_None) {
      RasterSpatialModel::Parameters parameters;
      int xsize = GDALGetRasterXSize(pDataset_);
      int ysize = GDALGetRasterYSize(pDataset_);
      for (size_t i = 0; i < 6; ++i)
         parameters.matrixModel_[i] = pmodel[i];
      /**
       * Si las dimensiones del dataset son menores a 1024 no modifico los parametros
       * Esta validacion es un fix temporal porque el dataset de algunas capas wms (ej bluemarble)
       * tienen la dimension incorrecta. Las capas TMS utilizan esta validacion.
		 */
      std::string drivername = pDataset_->GetDriver()->GetDescription();
      if (xsize > 1024 && ysize > 1024 && drivername.compare("WMS") == 0) {
         parameters.matrixModel_[0] = pmodel[0];
         parameters.matrixModel_[1] = pmodel[1] * xsize / xblocksize;
         parameters.matrixModel_[2] = pmodel[2] * ysize / yblocksize;
         parameters.matrixModel_[3] = pmodel[3];
         parameters.matrixModel_[4] = pmodel[4] * xsize / xblocksize;
         parameters.matrixModel_[5] = pmodel[5] * ysize / yblocksize;
      }
      std::string wkt = RasterSpatialModel::ParametersToWkt(parameters);
      if (!wkt.empty())
         SetOption(RasterMatrix, wkt);
   }
   // Obtiene la lista de archivos asociados al DataSet y los guarda como Option
   char ** pauxfilelist = pDataset_->GetFileList();
   std::string dependencies;
   int filecount = CSLCount(pauxfilelist);

   for (int i = 0; i < filecount; i++)
      if (Filename != CSLGetField(pauxfilelist, i)) {
         dependencies += CSLGetField(pauxfilelist, i);
         dependencies += ',';
      }
   CSLDestroy(pauxfilelist);

   if (!dependencies.empty()) {
      // elimina la ultima ","
      dependencies = dependencies.substr(0, dependencies.length() - 1);
      SetOption("dependencies", dependencies);
   }
}

/**
 * Inicializa para escritura
 * @param[in] Filename nombre de archivo a escribir.
 */
void GdalImage::InitializeWrite(const std::string &Filename) {
   if (!pDataset_) {
      return;
   }
   // Asigno georreferencia
   RasterSpatialModel *prm = RasterSpatialModel::Create(GetOption(Image::RasterMatrix));
   if (prm) {
      RasterSpatialModel::Parameters rastermodel = prm->GetModelParameters();
      if (rastermodel.MatrixIsValid()) {
         double gdalmodel[6];
         for (int i = 0; i < 6; i++)
            gdalmodel[i] = rastermodel.matrixModel_[i];
         pDataset_->SetGeoTransform(gdalmodel);
         pDataset_->SetProjection(GetOption(Image::Georreference).c_str());
      }
   }
   RasterSpatialModel::Destroy(prm);
   pWriter_ = new GdalRasterWriter(pDataset_, Filename);

   pWriter_->SetSize(pDataset_->GetRasterXSize(), pDataset_->GetRasterYSize());
   pWriter_->SetBandCount(pDataset_->GetRasterCount());
   // tiene al menos 1 banda
   pWriter_->SetDataType(
         GdalDataTypeMap[pDataset_->GetRasterBand(1)->GetRasterDataType()].first);
}

/** Inicializacion de la clase */
/**
 *  Inicializa los datos necesarios para esta clase.
 *
 *  Inicializa los drivers GDAL y los mapas de tipos de dato y tipo de
 * acceso a los datos.
 */
void GdalImage::InitializeClass() {
   LibSuriConfig.variables_["GDAL_SKIP"] = RemoveFromList(
         GDAL_SUPPORTED_FORMATS,
         suri::Configuration::GetParameter(
               "lib_supported_image_formats",
               LibSuriConfig.variables_["SURI_GDAL_INCLUDE"]),
         " ");
   suri::ParametersType::EnvironmentMapType::iterator it =
         LibSuriConfig.variables_.begin();
   while (it != LibSuriConfig.variables_.end()) {
      if (!it->first.empty()) {
         CPLSetConfigOption(it->first.c_str(), it->second.c_str());
      }
      it++;
   }

   CPLSetConfigOption("PROJ_LIB", (LibSuriConfig.dataDir_ + "proj/").c_str());
   CPLSetConfigOption("GEOTIFF_CSV", (LibSuriConfig.dataDir_ + "epsg_csv/").c_str());
   CPLSetConfigOption("GDAL_DATA", (LibSuriConfig.dataDir_ + "epsg_csv/").c_str());
   CPLSetConfigOption("GDAL_SKIP", "");

   // Workaround para GDAL cuando se compila con BuildEnv (compilacion custom)  ***
   PutEnvEx("PROJ_LIB", (LibSuriConfig.dataDir_ + "proj/").c_str());
   PutEnvEx("GEOTIFF_CSV", (LibSuriConfig.dataDir_ + "epsg_csv/").c_str());
   PutEnvEx("GDAL_DATA", (LibSuriConfig.dataDir_ + "epsg_csv/").c_str());
   PutEnvEx("GDAL_SKIP", "");
   // *****************************************************************************

   GDALAllRegister();
   // Los nombres son los de las bandas raster con el tipo de dato
   GdalDataTypeMap.insert(
         std::make_pair(
               GDT_Byte,
               std::make_pair(DataInfo<unsigned char>::Name,
                              DataInfo<unsigned char>::Size)));
   GdalDataTypeMap.insert(
         std::make_pair(
               GDT_UInt16,
               std::make_pair(DataInfo<unsigned short>::Name,
                              DataInfo<unsigned short>::Size)));
   GdalDataTypeMap.insert(
         std::make_pair(GDT_Int16,
                        std::make_pair(DataInfo<short>::Name, DataInfo<short>::Size)));
   GdalDataTypeMap.insert(
         std::make_pair(
               GDT_UInt32,
               std::make_pair(DataInfo<unsigned int>::Name,
                              DataInfo<unsigned int>::Size)));
   GdalDataTypeMap.insert(
         std::make_pair(GDT_Int32,
                        std::make_pair(DataInfo<int>::Name, DataInfo<int>::Size)));
   GdalDataTypeMap.insert(
         std::make_pair(GDT_Float32,
                        std::make_pair(DataInfo<float>::Name, DataInfo<float>::Size)));
   GdalDataTypeMap.insert(
         std::make_pair(
               GDT_Float64,
               std::make_pair(DataInfo<double>::Name, DataInfo<double>::Size)));
   // cargo el mapa inverso
   for (Gdal2SuriDataType::iterator it = GdalDataTypeMap.begin();
         it != GdalDataTypeMap.end(); it++)
      SuriDataTypeMap.insert(std::make_pair(it->second.first, it->first));

   // mapeo los tipos de acceso
   SuriAccessTypeMap.insert(std::make_pair(ReadOnly, GA_ReadOnly));
   SuriAccessTypeMap.insert(std::make_pair(WriteOnly, GA_Update));
   SuriAccessTypeMap.insert(std::make_pair(ReadWrite, GA_Update));

   // cargo el mapa inverso (cuidado con los repetidos)
   for (Suri2GdalAccessType::iterator it = SuriAccessTypeMap.begin();
         it != SuriAccessTypeMap.end(); it++)
      GdalAccessTypeMap.insert(std::make_pair(it->second, it->first));
}

}

#endif   // __GDAL__
