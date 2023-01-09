/*! \file Image.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <map>
#include <vector>
#include <string>

#include "Option.h"
#include "RasterBand.h"
#include "RasterDriver.h"
#include "RasterWriter.h"

#include "logmacros.h"

namespace suri {
class RasterWriter;
} /** namespace suri */

// Defines
/** Version de libreria suri_image */
#define SURI_IMAGE_LIBRARY 0.1

/** namespace suri */
namespace suri {
/** Clase imagen */
/**
 *  Esta clase es la mas importante.
 *  Tiene las bandas y detras de las mismas los drivers de lectura.
 *  Se obtiene una nueva imagen utilizando el metodo estatico Open(), este
 * metodo busca la clase derivada (usando la factoria) que puede abrir el
 * archivo requerido y retorna un puntero a la clase Image que lo abrio.
 *  Se cierra con el metodo estatico Close() que se encarga de limpiar todas las cosas.
 *  Permite realizar el registro de nuevas clases. (encapsula la factoria)
 * \note hereda de Option para guardar la informacion extra que trae la imagen como la georreferencia
 */
class Image : public Option {
   /** Ctor. de Copia. */
   Image(const Image &Image);

public:
   /** Enum que identifica el tipo de acceso a los datos que se desea */
   typedef enum {
      ReadOnly, WriteOnly, ReadWrite
   } ImageAccessType;
   /** Enum que identifica las propiedades mas comunes */
   typedef enum {
      Georreference, RasterMatrix, BandCombination, Compression, Format,
      FormatDescription, Mux, Description, PixelWEResolution, PixelNSResolution,
      PixelTotalResolution
   } ImageOptionIdType;
   /** Enum que identifica valores de propiedades comunes */
   typedef enum {
      RasterSpatialReference, RasterModel
   } ImageOptionValuesType;

   /** Ctor */
   Image();
   /** Dtor. */
   virtual ~Image();

// ------------------------ METODOS STATICOS ------------------------
   /** Funcion que permite registrar una nueva clase imagen */
   static void RegisterImage(std::string ClassId, Image* (*RegisterFunction)());
   /** Abre un archivo */
   static Image* Open(const std::string &Filename, ImageAccessType ImageAccess =
         ReadOnly,
                      const std::string &Writer = "null", int BandCount = 0, int SizeX =
                            0,
                      int SizeY = 0, const std::string &DataName = "void",
                      const Option &Options = Option());
   /** Cierra la image */
   static void Close(Image* &pImage);
   /** Inicializa las clases */
   static bool Init();
// ------------------- ESCRITURA A IMAGEN EXTERNA -------------------
   /** Salva la imagen en un formato dado con las opciones dadas */
   virtual bool Save(const std::string &Filename, const std::string &Format,
                     const OptionsMapType &WriteOptions = OptionsMapType());

// ----------------------------- RASTER -----------------------------
   /** Acceso al tamanio */
   void GetSize(int &SizeX, int &SizeY) const;
   /** Acceso a la dimension X */
   int GetSizeX() const;
   /** Acceso a la dimension Y */
   int GetSizeY() const;
   /** Retorna un vector con buffers internos de cada banda del subset pedido */
   virtual std::vector<void*> GetBlock(std::vector<int> &BandIndex, int BlockX,
                                       int BlockY);
   /** Llena los punteros del vector con el subset de las bandas pedidas */
   virtual bool Read(const std::vector<int> &BandIndex, std::vector<void*> &Data, int Ulx,
                     int Uly, int Lrx, int Lry) const;
   /** Escribe mas de una banda */
   virtual void Write(const std::vector<int> &BandIndex, std::vector<void*> &Data, int Ulx,
                      int Uly, int Lrx, int Lry);
   /** Escribe mas de una banda */
   virtual void Write(const std::vector<int> &BandIndex, std::vector<void*> &Data, int BlockX,
                      int BlockY);
// ----------------------------- BLOQUE -----------------------------
   /** Tamanio del bloque X e Y */
   void GetBlockSize(int &SizeX, int &SizeY) const;
   /** Tamanio X del bloque */
   int GetBlockSizeX() const;
   /** Tamanio Y del bloque */
   int GetBlockSizeY() const;
   /** Bloque de bandas/subset que debe escribir */
   virtual bool GetNextBlock(std::vector<int> &BandIndex, int &Ulx, int &Uly, int &Lrx,
                             int &Lry);
   /** Bloque de bandas/subset que debe escribir */
   virtual bool GetNextBlock(std::vector<int> &BandIndex, int BlockX, int BlockY);
// -------------------------- MULTI-RASTER --------------------------
   /** Retorna la cantidad de bandas */
   int GetBandCount() const;
   /** Devuelve la banda */
   RasterBand* GetBand(size_t Index) const;
   /** Inserta una banda */
   void SetBand(RasterBand *pBand, size_t Index);
// --------------------------- DATA-TYPE ----------------------------
   /** Nombre del tipo de dato */
   std::string GetDataType() const;
   /** Tamanio del dato en bytes */
   int GetDataSize() const;
// ----------------------------- SOURCES ----------------------------
   /** Carga una nueva fuente */
   void PushSource(SourceSupervisor *pSource);
   /** Elimina la fuente */
   bool PopSource();
// ---------------------------- OPCIONES ----------------------------
// Para Poder overloadear los metodos Get/SetOption
   using Option::SetOption;
   using Option::GetOption;
   /** Agrega o cambia opciones */
   void SetOption(const ImageOptionIdType Id, const std::string &Value);
   /** Retorna el valor de una opcion */
   std::string GetOption(const ImageOptionIdType Id) const;
   /** Acceso a los nombres posibles de las opciones */
   static const std::string GetOptionsIds(const ImageOptionIdType &Id);
   /** Acceso a los valores posbiles de las opciones */
   static const std::string GetOptionValue(const ImageOptionValuesType &Value);
   /** Obtiene los metadatos de la imagen */
   virtual std::map<std::string, std::map<std::string, std::string> > GetMetadata(
         const std::vector<std::string>& Domains) const;

   /** Establece el sistema de referencia */
   virtual void SetSrs(const std::string& Wkt);

   /** Establece la transformacion para georreferenciacion */
   virtual void SetGeoTransform(double* pGeoTransform);

protected:
   /** Inspecciona el archivo, para saber si es del formato que maneja */
   virtual bool Inspect(const std::string &Filename, ImageAccessType ImageAccess =
         ReadOnly,
                        int BandCount = 0, int SizeX = 0, int SizeY = 0,
                        const std::string &DataType = "void")=0;
   /** Carga la imagen */
   virtual Image* Load(const std::string &Filename, ImageAccessType ImageAccess =
         ReadOnly,
                       int BandCount = 0, int SizeX = 0, int SizeY = 0,
                       const std::string &DataType = "void")=0;
   /** Inicializacion en las clases derivadas, se llama al registrarlas */
   virtual void InitializeClass()=0;
   /** tipo para las propiedades comunes */
   typedef std::map<ImageOptionIdType, std::string> OptionsIdMapType;
   /** tipo para los valores mas comunes */
   typedef std::map<ImageOptionValuesType, std::string> OptionValuesMapType;
   std::vector<RasterBand*> bandVector_; /*! Vector con las bandas */
   std::vector<RasterDriver*> driverVector_; /*! Vector con los drivers */
   RasterWriter *pWriter_; /*! Driver de escritura */
   static OptionsIdMapType optionsIds_; /*! Identificador de Ids <-> Opciones */
   static OptionValuesMapType optionValues_; /*! Identificador Valores */
   ImageAccessType access_; /*! Acceso a la imagen */
   int bandCount_; /*! Cantidad de bandas */
private:
};

}  // namespace suri

#endif  // IMAGE_H_
