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

#include <map>
#include <vector>
#include <string>

#include "suri/Image.h"
#include "suri/RasterBand.h"
#include "suri/RasterDriver.h"
#include "ImageFactory.h"
#include "RasterFactory.h"
#include "suri/DataTypes.h"
#include "suri/Progress.h"
#include "suri/SourceSupervisor.h"
#include "suri/CoordinatesTransform.h"

// Defines
/** Raster Model identidad */
#define RASTER_IDENDTITY_MATRIX \
   "PARAM_MT[\"Affine\",PARAMETER[\"num_row\",2],PARAMETER[\"num_col\",3],PARAMETER[\"elt_0_0\",0],PARAMETER[\"elt_0_1\",1],PARAMETER[\"elt_0_2\",0],PARAMETER[\"elt_1_0\",0],PARAMETER[\"elt_1_1\",0],PARAMETER[\"elt_1_2\",1]]"

/** namespace suri */
namespace suri {
/** propiedades comunes */
Image::OptionsIdMapType Image::optionsIds_;
/** valores mas comunes */
Image::OptionValuesMapType Image::optionValues_;

/** Inicializa las clases derivadas */
/**
 *  Inicializa las factorias de bandas e imagenes
 */
void InitializeImages() {
   // Garantizo una sola inicializacion
   static bool isinitialized = false;
   if (isinitialized) {
      return;
   }
   isinitialized = true;
   // Registra todas las clases RasterBand
   RegisterAllRasterBands();
   // Registra todas las clases imagen
   RegisterAllImages();
}

/** ctor */
Image::Image() {
   pWriter_ = NULL;
}

/** dtor */
/**
 *  Elimina las bandas y los drivers.
 * \todo Verificar la destruccion de los sources (no se esta haciendo)
 * \todo reemplazar por el delete de la factoria
 * \todo borrar bien
 */
Image::~Image() {
   // Elimina las fuentes
   do {
   } while (PopSource());

   {
      // libero las bandas
      std::vector<RasterBand*>::iterator it = bandVector_.begin();
      while (!bandVector_.empty()) {
         // \todo reemplazar por el delete de la factoria
         delete (*it);
         bandVector_.erase(it);
         it = bandVector_.begin();
      }
   }
   {
      // libero los drivers
      std::vector<RasterDriver*>::iterator it = driverVector_.begin();
      while (!driverVector_.empty()) {
         RasterDriver *pdriver = *it;
         // \todo reemplazar por el delete de la factoria (o de la clase derivada de Image)
         delete pdriver;
         driverVector_.erase(it);
         it = driverVector_.begin();
      }
   }
   // \todo borrar bien
   delete pWriter_;
}

// ------------------------ METODOS STATICOS ------------------------

/**
 *  Registra la clase en la factoria.
 *  Inicializa la clase creando una instancia y llamando a InitializeClass()
 *  @param[in] ClassId id del tipo de imagen
 *  @param[in] CreateFunction funcion que se usa para crear la imagen
 */
void Image::RegisterImage(std::string ClassId, Image* (*CreateFunction)()) {
   ImageFactory &factory = TheImageFactory::Instance();
   factory.RegisterClass(ClassId, CreateFunction);
   Image *pimage = CreateFunction();
   pimage->InitializeClass();
   delete pimage;
}

/** Abre un archivo */
/**
 *  Abre un archivo en el modo de acceso deseado.
 *  Utiliza la factoria de imagenes para iterar por las mismas llamando al
 * metodo Inspect() y en caso positivo, carga el archivo en la clase y
 * retorna la instancia.
 * @param[in] Filename Nombre del archivo (mas adelante se puede referir a
 *                                      memoria, red, etc., ie. acceso virtual)
 * @param[in] ImageAccess Tipo de acceso al archivo deseado, WriteOnly siempre
 *  retorna NULL
 * @param[in] Writer nombre del "driver de escritura"
 * @param[in] BandCount cantidad de bandas (solo para ImageAccess==WriteOnly)
 * @param[in] SizeX cantidad de columnas (solo para ImageAccess==WriteOnly)
 * @param[in] SizeY cantidad de filas (solo para ImageAccess==WriteOnly)
 * @param[in] DataName tipo de dato raster (solo para ImageAccess==WriteOnly)
 * @param[in] Options para la apertura del archivo
 * @return Puntero a la instancia (derivada) de la clase que maneja el formato.
 */
Image* Image::Open(const std::string &Filename, ImageAccessType ImageAccess,
                   const std::string &Writer, int BandCount, int SizeX, int SizeY,
                   const std::string &DataName, const Option &Options) {
   // Si piden solo escritura, falla, se debe usar el otro overload
   ImageFactory &factory = TheImageFactory::Instance();
   ImageFactory::IdentifierVector::iterator it = factory.GetClassIds().begin();
   while (it != factory.GetClassIds().end()) {
      // no deberia tirar ninguna excepcion ya que usa los strings propios
      Image *pimage = TheImageFactory::Instance().CreateClass(*it);
      // En caso de tratarse de solo lectura, cargo como opcion el formato solicitado
      if (ImageAccess == WriteOnly || ImageAccess == ReadWrite) {
         pimage->SetOption(Format, Writer);
      }

      // Opciones a priori, dan oportunidad de sobreescribir luego
      pimage->SetOption("filename", Filename);
      // carga sistema de referencia dummy que da error en
      // las conversiones
      pimage->SetOption(optionsIds_[Image::Georreference],
                        optionValues_[Image::RasterSpatialReference]);
      pimage->SetOption(optionsIds_[Image::RasterMatrix],
                        optionValues_[Image::RasterModel]);
      // Agrego las opciones
      pimage->SetOption(Options.GetOption());
      if (pimage->Inspect(Filename, ImageAccess, BandCount, SizeX, SizeY, DataName)) {
         pimage = pimage->Load(Filename, ImageAccess, BandCount, SizeX, SizeY,
                               DataName);
         // Cargo la propiedad del tipo de acceso
         pimage->access_ = ImageAccess;
         // Cargo la propiedad de la cantidad de bandas
         switch (pimage->access_) {
            case WriteOnly: {
               if (pimage->pWriter_) {
                  pimage->bandCount_ = pimage->pWriter_->GetBandCount();
               }
               break;
            }
            case ReadWrite:
            case ReadOnly: {
               if (!pimage->bandVector_.empty()) {
                  pimage->bandCount_ = pimage->bandVector_.size();
               }
               break;
            }
         }
         return pimage;
      }
      // \todo delegar a la factoria
      delete pimage;
      it++;
   }
   REPORT_AND_FAIL_VALUE("D:No se pudo identificar el formato de la imagen (%s).", NULL,
                         Filename.c_str());
}

/**
 *  Debe eliminar la imagen de una manera amigable con la factoria
 *  @param[in] pImage imagen que quiero eliminar
 *  @param[out] pImage se modifica puntero a NULL
 * \todo Usar metodo de la factoria
 */
void Image::Close(Image* &pImage) {
   // \todo delegar a la factoria
   delete pImage;
   pImage = NULL;
}

/**
 * Inicializa las clases
 * @return informa si pudo inicializar la imagen
 */
bool Image::Init() {
   InitializeImages();
   // carga los identificadores de opciones importantes
   // \todo que se haga una sola vez
   optionsIds_.insert(std::make_pair(Image::Georreference, "Geo"));
   optionsIds_.insert(std::make_pair(Image::RasterMatrix, "Matrix"));
   optionsIds_.insert(std::make_pair(Image::BandCombination, "BandCombination"));
   optionsIds_.insert(std::make_pair(Image::Format, "Format"));
   optionsIds_.insert(std::make_pair(Image::FormatDescription, "FormatDescription"));
   optionsIds_.insert(std::make_pair(Image::Mux, "Mux"));
   optionsIds_.insert(std::make_pair(Image::Description, "Desc"));
   optionsIds_.insert(std::make_pair(Image::PixelWEResolution, "PixelWE"));
   optionsIds_.insert(std::make_pair(Image::PixelNSResolution, "PixelNS"));
   optionsIds_.insert(std::make_pair(Image::PixelTotalResolution, "PixelTotal"));
   // Carga los valores mas comunes
   optionValues_.insert(
         std::make_pair(Image::RasterSpatialReference, RASTER_SPATIAL_REFERENCE));
   optionValues_.insert(std::make_pair(Image::RasterModel, RASTER_IDENDTITY_MATRIX));
   return true;
}

// ----------------------- FIN METODOS ESTATICOS ------------------------

/**
 * Salva la imagen en un formato dado con las opciones
 * @param[in] Filename nombre del archivo de salida
 * @param[in] Format formato en que se va a guardar la imagen
 * @param[in] WriteOptions opciones
 * @return true si pudo salvar la imagen
 * @return false si cantidad de bandas < 1 o no pudo abrir
 * imagen de salida
 */
bool Image::Save(const std::string &Filename, const std::string &Format,
                 const OptionsMapType &WriteOptions) {
   if (GetBandCount() < 1) {
      return false;
   }
   int x, y;
   GetSize(x, y);
   int c = GetBandCount();
   // abro una imagen en modo crear (write, evaluar readwrite)
   Image *poutimage = Image::Open(Filename, WriteOnly, Format, c, x, y, GetDataType());
   if (poutimage) {
      // Seteo opciones del driver de escritura
      poutimage->SetOption(WriteOptions);
      std::vector<int> bandvect;
      std::vector<void*> datavect(c);
      poutimage->GetBlockSize(x, y);
      for (int i = 0; i < c; i++)
         datavect[i] = new unsigned char[x * y * GetDataSize()];
      int ulx, uly, lrx, lry;
      // copia en la imagen de salida los datos de la de entrada
      while (poutimage->GetNextBlock(bandvect, ulx, uly, lrx, lry)) {
         // realiza la lectura optimizada
         Read(bandvect, datavect, ulx, uly, lrx, lry);
         // escribe los datos que pidio el Writer
         poutimage->Write(bandvect, datavect, ulx, uly, lrx, lry);
      }
      // borro los buffers
      for (int i = 0; i < c; i++)
         delete[] static_cast<unsigned char*>(datavect[i]);
      // cierro la imagen de salida (baja todos los datos)
      Image::Close(poutimage);
      return true;
   }
   return false;
}

/** Acceso al tamanio */
/**
 *  Retorna las dimensiones X e Y.
 * @param[out] SizeX ancho de la imagen
 * @param[in] SizeY ancho de la imange
 * \pre Para modo ReadOnly y ReadWrite debe haber al menos una banda.
 * \pre Para modo ReadWrite se fija que hay al menos 1 banda.
 */
void Image::GetSize(int &SizeX, int &SizeY) const {
   SizeX = 0;
   SizeY = 0;
   switch (access_) {
      case WriteOnly: {
         if (pWriter_) {
            pWriter_->GetSize(SizeX, SizeY);
         }
         return;
      }
      case ReadWrite:
      case ReadOnly: {
         if (!bandVector_.empty()) {
            bandVector_.at(0)->GetSize(SizeX, SizeY);
         }
         return;
      }
   }
}

/**
 * @return Retorna la dimension X.
 * \pre Para modo ReadOnly y ReadWrite debe haber al menos una banda.
 * \pre Para modo ReadWrite se fija que hay al menos 1 banda..
 */
int Image::GetSizeX() const {
   switch (access_) {
      case WriteOnly: {
         return pWriter_ ? pWriter_->GetSizeX() : 0;
         break;
      }
      case ReadWrite:
      case ReadOnly: {
         return bandVector_.size() ? bandVector_.at(0)->GetSizeX() : 0;
         break;
      }
      default:
         return 0;
         break;
   }
}

/**
 * @return Retorna la dimension Y.
 * \pre Para modo ReadOnly y ReadWrite debe haber al menos una banda.
 * \pre Para modo ReadWrite se fija que hay al menos 1 banda.
 */
int Image::GetSizeY() const {
   switch (access_) {
      case WriteOnly: {
         return pWriter_ ? pWriter_->GetSizeY() : 0;
         break;
      }
      case ReadWrite:
      case ReadOnly: {
         return bandVector_.size() ? bandVector_.at(0)->GetSizeY() : 0;
         break;
      }
      default:
         return 0;
         break;
   }
}

/**
 * @return el tipo de dato
 */
std::string Image::GetDataType() const {
   switch (access_) {
      case WriteOnly:
         return pWriter_ ? pWriter_->GetDataType() : DataInfo<void>::Name;
      case ReadWrite:
      case ReadOnly:
         return
               bandVector_.size() ? bandVector_.at(0)->GetDataType() :
                                    DataInfo<void>::Name;
   }
   return DataInfo<void>::Name;
}

/**
 * @return el tamanio del dato
 */
int Image::GetDataSize() const {
   switch (access_) {
      case WriteOnly:
         return pWriter_ ? pWriter_->GetDataSize() : DataInfo<void>::Size;
      case ReadWrite:
      case ReadOnly:
         return
               bandVector_.size() ? bandVector_.at(0)->GetDataSize() :
                                    DataInfo<void>::Size;
   }
   return 0;
}

/**
 * Retorna un vector con punteros a un buffer interno de cada banda con el
 * subset pedido
 * @param[in] BandIndex vector con bandas que se quieren leer
 * @param[in] BlockX columna del bloque
 * @param[in] BlockY fila del bloque
 * @return vector con punteros a buffer interno
 */
std::vector<void*> Image::GetBlock(std::vector<int> &BandIndex, int BlockX,
                                   int BlockY) {
   if (access_ == WriteOnly) {
      REPORT_AND_FAIL_VALUE("D:Imagen de solo de lectura.",
                            std::vector<void*>(BandIndex.size(), NULL));
   }
   std::vector<void*> data;
   std::vector<int>::iterator it = BandIndex.begin();
   std::vector<int>::iterator end = BandIndex.end();
   while (it != end) {
      data.push_back(bandVector_.at(*it)->GetBlock(BlockX, BlockY));
      it++;
   }
   return data;
}

/**
 * Llena los punteros del vector con el subset de las bandas pedidas
 * @param[in] BandIndex vector con bandas que se quieren leer
 * @param[out] Data vector donde se van a cargar los datos
 * @param[in] Ulx upper left x del subset
 * @param[in] Uly upper left y del subset
 * @param[in] Lrx lower right x del subset
 * @param[in] Lry lower right y del subset
 * @return bool que indica si pudo leer la imagen
 */
bool Image::Read(const std::vector<int> &BandIndex, std::vector<void*> &Data, int Ulx,
                 int Uly, int Lrx, int Lry) const{
   if (access_ == WriteOnly) {
      REPORT_AND_FAIL_VALUE("D:Read, imagen de solo escritura.", false);
   }
   size_t size = BandIndex.size();
   if (Data.size() != size) {
      REPORT_AND_FAIL_VALUE("D:Read, se solicitan mas bandas de los buffers.", false);
   }

   bool success = true;
   Progress progress(size, _(message_READING_BANDS));
   for (size_t i = 0; i < size && success; i++) {
      progress.Update();
      success = success
            && bandVector_.at(BandIndex.at(i))->Read(Data.at(i), Ulx, Uly, Lrx, Lry);
   }

   return success;
}

/**
 * Escribe mas de una banda
 * @param[in] BandIndex vector con bandas que se quieren escribir
 * @param[in] Data vector donde estan los datos a escribir
 * @param[in] Ulx upper left x del subset
 * @param[in] Uly upper left y del subset
 * @param[in] Lrx lower right x del subset
 * @param[in] Lry lower right y del subset
 */
void Image::Write(const std::vector<int> &BandIndex, std::vector<void*> &Data, int Ulx,
                  int Uly, int Lrx, int Lry) {
   if (access_ == ReadOnly) {
      REPORT_AND_FAIL("D:Read, imagen de solo lectura.");
   }
   if (pWriter_) {
      pWriter_->Write(BandIndex, Data, Ulx, Uly, Lrx, Lry);
   }
}

/**
 * Escribe los bloques indicados
 * @param[in] BandIndex vector con bandas que se quieren escribir
 * @param[in] Data vector donde estan los datos a escribir
 * @param[in] BlockX columna del bloque
 * @param[in] BlockY fila del bloque
 */
void Image::Write(const std::vector<int> &BandIndex, std::vector<void*> &Data, int BlockX,
                  int BlockY) {
   int x, y;
   GetBlockSize(x, y);
   Write(BandIndex, Data, BlockX * x, BlockY * y, (BlockX + 1) * x, (BlockY + 1) * y);
}

/**
 * Tamanio del bloque X e Y
 * @param[out] SizeX ancho del bloque
 * @param[out] SizeY altura del bloque
 */
void Image::GetBlockSize(int &SizeX, int &SizeY) const {
   // Si hay Writer, usa su bloque (caso WriteOnly o ReadWrite)
   if (pWriter_) {
      pWriter_->GetBlockSize(SizeX, SizeY);
   }
   // Sino usa el bloque de banda (caso ReadOnly)
   if (!bandVector_.empty()) {
      bandVector_.at(0)->GetBlockSize(SizeX, SizeY);
   }
}

/**
 * Tamanio X del bloque
 * @return ancho del bloque
 */
int Image::GetBlockSizeX() const {
   int temp, dummy;
   GetBlockSize(temp, dummy);
   return temp;
}

/**
 * Tamanio Y del bloque
 * @return altura del bloque
 */
int Image::GetBlockSizeY() const {
   int temp, dummy;
   GetBlockSize(dummy, temp);
   return temp;
}

/**
 * Obtiene el proximo bloque de escritura deseado
 * @param[out] BandIndex vector con bandas de la imagen
 * @param[out] Ulx upper left x del subset
 * @param[out] Uly upper left y del subset
 * @param[out] Lrx lower right x del subset
 * @param[out] Lry lower right y del subset
 * @return informa si hay proximo bloque
 */
bool Image::GetNextBlock(std::vector<int> &BandIndex, int &Ulx, int &Uly, int &Lrx,
                         int &Lry) {
   // \todo tirar error
   if (access_ == ReadOnly) {
      REPORT_AND_RETURN_VALUE("D:GetNextBlock, Imagen solo de lectura.", false);
   }
   if (pWriter_) {
      return pWriter_->GetNextBlock(BandIndex, Ulx, Uly, Lrx, Lry);
   }
   REPORT_AND_RETURN_VALUE("D:GetNextBlock, No existe el writer asociado a la imagen.",
                           false);
}

/**
 * Obtiene el proximo bloque de escritura
 * @param[out] BandIndex vector con bandas de la imagen
 * @param[out] BlockX columna del bloque
 * @param[out] BlockY fila del bloque
 * @return true si quedan mas bloques
 */
bool Image::GetNextBlock(std::vector<int> &BandIndex, int BlockX, int BlockY) {
   int ulx, uly, lrx, lry;
   if (!GetNextBlock(BandIndex, ulx, uly, lrx, lry)) {
      return false;
   }
   int x, y;
   GetBlockSize(x, y);
   BlockX = ulx / x;
   BlockY = uly / y;
   return true;
}

/**
 * @return cantidad de bandas en imagen
 */
int Image::GetBandCount() const {
   // si es ReadOnly o ReadWrite, retorno la cantidad de bandas
   if (access_ != WriteOnly) {
      return !bandVector_.empty() ? bandVector_.size() : 0;
   }
   // si es solo de escritura, retorno lo que dice el writer
   return pWriter_ ? pWriter_->GetBandCount() : 0;
}

/**
 * Devuelve la banda
 * @param[in] Index indice de la banda
 * @return RasterBand con banda deseada
 */
RasterBand* Image::GetBand(size_t Index) const {
   return bandVector_.at(Index);
}

/**
 * Inserta una banda
 * @param[in] pBand banda que se quiere agregar
 * @param[in] Index indice de la banda
 */
void Image::SetBand(RasterBand *pBand, size_t Index) {
   if (pBand->GetDataType() != GetDataType() && GetDataType() != DataInfo<void>::Name) {
      REPORT_AND_FAIL(
            "D:Image::SetBand: Tipo de dato de la banda incompatible con la imagen.");
   }
   int newbandx, newbandy;
   pBand->GetSize(newbandx, newbandy);
   int x, y;
   GetSize(x, y);
   if (x != newbandx || y != newbandy) {
      REPORT_AND_FAIL(
            "D:Image::SetBand: Dimensiones de la banda incompatibles on la imagen.");
   }
   std::vector<RasterBand*>::iterator it = bandVector_.begin();
   std::vector<RasterBand*>::iterator end = bandVector_.end();
   while (it != end || Index != 0) {
      Index--;
      it++;
   }
   bandVector_.insert(it, pBand);
}

/** Carga una nueva fuente */
/**
 *  Duplica la fuente y se la agrega a las bandas
 *  @param[in] pSource SourceSupervisor que se desea agregar
 */
void Image::PushSource(SourceSupervisor *pSource) {
   if (!pSource) {
      return;
   }
   for (size_t i = 0; i < bandVector_.size(); i++)
      bandVector_.at(i)->PushSource(pSource->CreateSource());
}

/** Elimina la fuente */
/**
 * Saca y elimina una fuente del stack de fuentes de cada banda.
 * @return bool que indica si se elimino al menos una fuente.
 */
bool Image::PopSource() {
   bool result = false;
   for (size_t i = 0; i < bandVector_.size(); i++) {
      RasterSource *psource = bandVector_.at(i)->PopSource();
      if (psource) {
         result = true;
      }
      delete psource;
   }
   return result;
}

/**
 * Agrega o cambia opciones
 * @param[in] Id identificador de la opcion
 * @param[in] Value nuevo valor de la opcion
 */
void Image::SetOption(const ImageOptionIdType Id, const std::string &Value) {
   SetOption(optionsIds_[Id], Value);
}

/**
 * Retorna el valor de una opcion
 * @param[in] Id identificador de la opcion
 * @return valor de la opcion
 */
std::string Image::GetOption(const ImageOptionIdType Id) const {
   return GetOption(optionsIds_[Id]);
}

/**
 * Acceso a los nombres posibles de las opciones
 * @param[in] Id identificador de la opcion
 * @return nombre de la opcion
 */
const std::string Image::GetOptionsIds(const ImageOptionIdType &Id) {
   return optionsIds_[Id];
}

/**
 * Acceso a los valores posbiles de las opciones
 * @param[in] Value tipo de valor en la opcion
 * @return valor de la opcion
 */
const std::string Image::GetOptionValue(const ImageOptionValuesType &Value) {
   return optionValues_[Value];
}

/**
 * Obtiene los metadatos de la imagen.
 *
 * @return Devuelve un mapa con las categorias y por cada categoria
 * un mapa con los pares clave-valor correspondientes.
 */
std::map<std::string, std::map<std::string, std::string> > Image::GetMetadata(
      const std::vector<std::string>& Domains) const {
   return std::map<std::string, std::map<std::string, std::string> >();
}

/**
 * Establece el sistema de referencia.
 */
void Image::SetSrs(const std::string& Wkt) {
   // implementar en las clases que heredan.
}

/**
 * Establece la transformacion para georreferenciacion.
 */
void Image::SetGeoTransform(double* pGeoTransform) {
   // implementar en las clases que heredan.
}

}
