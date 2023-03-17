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

#include "Image.h"
#include <cppunit/extensions/HelperMacros.h>

/**
 * Capara dos imagenes
 * @param[in] plhs primer imagen
 * @param[in] prhs segunda imagen
 * @return resultado de comparar las imagenes
 */
bool compare_images(suri::Image *plhs, suri::Image *prhs) {
   int xrhs, yrhs;
   prhs->GetSize(xrhs, yrhs);
   int xlhs, ylhs;
   plhs->GetSize(xlhs, ylhs);
   if (xrhs != xlhs || yrhs != ylhs) {
      return false;
   }
   if (prhs->GetBandCount() != plhs->GetBandCount()) {
      return false;
   }
   if (prhs->GetBandCount() == 0) {
      return false;
   }
   if (prhs->GetDataSize() != plhs->GetDataSize()) {
      return false;
   }
   char *prhsbuff = new char[xrhs * yrhs * prhs->GetDataSize()];
   char *plhsbuff = new char[xlhs * ylhs * plhs->GetDataSize()];
   for (int i = 0; i < prhs->GetBandCount(); i++) {
      prhs->GetBand(i)->Read(prhsbuff, 0, 0, xrhs, yrhs);
      plhs->GetBand(i)->Read(plhsbuff, 0, 0, xlhs, ylhs);
      if (memcmp(prhsbuff, plhsbuff, xrhs * yrhs * prhs->GetDataSize()) != 0) {
         delete[] prhsbuff;
         delete[] plhsbuff;
         return false;
      }
   }
   delete[] prhsbuff;
   delete[] plhsbuff;
   return true;
}

/** Tests para la clase Image */
class ImageTest : public CPPUNIT_NS::TestFixture {
   /** Inicializa test para la clase ImageTest. Invoca a setUp. */
   CPPUNIT_TEST_SUITE(ImageTest);
   /** Evalua resultado de open */
   CPPUNIT_TEST(open);
   /** Evalua resultado de dimensions */
   CPPUNIT_TEST(dimensions);
   /** Evalua resultado de read */
   CPPUNIT_TEST(read);
   /** Evalua resultado de compare */
   CPPUNIT_TEST(compare);
   /** Evalua resultado de save */
   CPPUNIT_TEST(save);
   /** Evalua resultado de random_write */
   CPPUNIT_TEST(random_write);
   /** Evalua resultado de band_write */
   CPPUNIT_TEST(band_write);
   /** Finaliza test. Invoca a tearDown. */
   CPPUNIT_TEST_SUITE_END();
public:
   /** Ctor. */
   ImageTest() :
         blockSize_(0) {
   }

   /** Carga el archivo testigo e inicializa variables */
   void setUp();
protected:
   std::string witnessFilename_; /*! archivo testigo */
   static char *testFiles_[]; /*! archivos que se van a usar en los tests */
   std::string tempFile_; /*! archivo temporal */
   static char *outFormats_[]; /*! formatos de salida para los archivos */
   int blockSize_; /*! tamanio del bloque de datos */
   /** Prueba la apertura de imagenes */
   void open();
   /** Prueba que las dimensiones obtenidas de la imagen sean las correctas */
   void dimensions();
   /** Prueba la lectura de las imagenes */
   void read();
   /** Compara dos imagenes */
   void compare();
   /** Prueba salvar las imagenes en los distintos formatos de salida */
   void save();
   /** Prueba la escritura de una imagen */
   void random_write();
   /** Prueba la escritura de una banda */
   void band_write();
private:
};

char *ImageTest::testFiles_[] = { "testdata/buenos_aires.tiff",
                                  "testdata/buenos_aires_tiled.tiff", NULL }; /*! lista de archivos de imagenes */
/*! para los test */
char *ImageTest::outFormats_[] = { "envi", NULL }; /*! lista de formatos de salida */
#include <cppunit/config/SourcePrefix.h>

/** Macro para registrar los test */
CPPUNIT_TEST_SUITE_REGISTRATION(ImageTest);

/**
 * Carga el path del archivo testigo, y el nombre del archivo temporal.
 */
void ImageTest::setUp() {
   witnessFilename_ = "testdata/witness/buenos_aires.dat";
   tempFile_ = "tempfile.dat";
   blockSize_ = 100;
}

/**
 * Abre el archivo testigo. El test falla si no lo puede abrir.
 */
void ImageTest::open() {
   suri::Image *pimage = suri::Image::Open(witnessFilename_);
   CPPUNIT_ASSERT_MESSAGE(std::string("Falla al abrir: " + witnessFilename_), pimage);
   suri::Image::Close(pimage);
}

/**
 * Obtiene las dimensiones del archivo testigo. Las dimensiones tienen que ser
 * 2500 x 2500.
 */
void ImageTest::dimensions() {
   suri::Image *pimage = suri::Image::Open(witnessFilename_);
   int x, y;
   pimage->GetSize(x, y);
   CPPUNIT_ASSERT_MESSAGE("Error de dimensiones", x == 2500 && y == 2500);
   suri::Image::Close(pimage);
}

/**
 * Abre cada una de las imagenes, y las compara contra si mismas. Si la
 * comparacion da distinto es porque fallo la lectura.
 */
void ImageTest::read() {
   suri::Image *pimage = suri::Image::Open(witnessFilename_);
   CPPUNIT_ASSERT_MESSAGE("Error al leer la imagen testigo (no es igual a si misma)",
                          compare_images(pimage, pimage));
   suri::Image::Close(pimage);

   for (char **pfile = testFiles_; *pfile != NULL; pfile++) {
      suri::Image *ptestimage = suri::Image::Open(*pfile);
      CPPUNIT_ASSERT_MESSAGE(
            std::string("Error al leer ") + *pfile
                  + ". La imagen no es igual a si misma.",
            compare_images(ptestimage, ptestimage));
      suri::Image::Close(ptestimage);
   }
}

/**
 * Compara cada imagen con la imagen testigo. El test falla si la comparacion
 * devuelve que son distintas.
 */
void ImageTest::compare() {
   suri::Image *pimage = suri::Image::Open(witnessFilename_);

   for (char **pfile = testFiles_; *pfile != NULL; pfile++) {
      suri::Image *ptestimage = suri::Image::Open(*pfile);
      CPPUNIT_ASSERT_MESSAGE(std::string("Error al leer ") + *pfile,
                             compare_images(pimage, ptestimage));
      suri::Image::Close(ptestimage);
   }
   suri::Image::Close(pimage);
}

/**
 * Abre cada archivo de testFiles_ y para cada formato los guarda en el archivo
 * temporal. Abre el archivo temporal y lo compara con el archivo original.
 * El test falla di la comparacion da distinto.
 */
void ImageTest::save() {
   for (char **pfile = testFiles_; *pfile != NULL; pfile++) {
      suri::Image *pimage = suri::Image::Open(*pfile);
      for (char **pformat = outFormats_; *pformat != NULL; pformat++) {
         CPPUNIT_ASSERT_MESSAGE(
               std::string("Error al salvar ") + *pfile + " en formato " + *pformat,
               pimage->Save(tempFile_, *pformat));
         suri::Image *ptempimage = suri::Image::Open(tempFile_);

         CPPUNIT_ASSERT_MESSAGE(
               std::string("La imagen salvada en formato ") + *pformat
                     + " no corresponde a la original " + *pfile,
               compare_images(pimage, ptempimage));
         suri::Image::Close(ptempimage);
      }
      suri::Image::Close(pimage);
   }
}

/**
 * Abre una imagen y la lee por bloque, cada bloque lo graba en el archivo
 * temporal. Repite para cada uno de los archivos en testFiles_ y para cada
 * formato de salida.
 * El test tiene exito si cuando termina de leer / grabar todos los bloques,
 * la imagen de entrada y la de salida son iguales.
 */
void ImageTest::random_write() {
   for (char **pfile = testFiles_; *pfile != NULL; pfile++) {
      suri::Image *pimage = suri::Image::Open(*pfile);
      CPPUNIT_ASSERT_MESSAGE(
            "La imagen testigo no es multiplo del bloque",
            pimage->GetSizeX() % blockSize_ == 0
                  && pimage->GetSizeY() % blockSize_ == 0);
      for (char **pformat = outFormats_; *pformat != NULL; pformat++) {
         suri::Image *poutimage = suri::Image::Open(tempFile_, suri::Image::WriteOnly,
                                                    *pformat, pimage->GetBandCount(),
                                                    pimage->GetSizeX(),
                                                    pimage->GetSizeY(),
                                                    pimage->GetDataType());
         CPPUNIT_ASSERT(poutimage != NULL);

         // Lectura por bloques (acceso random en el archivo)
         std::vector<int> bandvect;
         std::vector<void*> datavect;
         for (int i = 0; i < pimage->GetBandCount(); i++) {
            bandvect.push_back(i);
            datavect.push_back(
                  new char*[pimage->GetSizeX() * pimage->GetSizeY()
                        * pimage->GetDataSize()]);
         }
         for (int j = 0; j < pimage->GetSizeY(); j += blockSize_)
            for (int i = 0; i < pimage->GetSizeX(); i += blockSize_) {
               // realiza la lectura optimizada
               pimage->Read(bandvect, datavect, i, j, i + blockSize_, j + blockSize_);
               // escribe los datos que pidio el Writer
               poutimage->Write(bandvect, datavect, i, j, i + blockSize_,
                                j + blockSize_);
            }
         for (int i = 0; i < pimage->GetBandCount(); i++)
            delete[] static_cast<char*>(datavect[i]);
         // cierro la imagen de salida (baja todos los datos)
         suri::Image::Close(poutimage);

         suri::Image *ptempimage = suri::Image::Open(tempFile_);

         CPPUNIT_ASSERT_MESSAGE(
               std::string("La imagen salvada en formato ") + *pformat
                     + " no corresponde a la original " + *pfile,
               compare_images(pimage, ptempimage));
         suri::Image::Close(ptempimage);
      }
      suri::Image::Close(pimage);
   }
}

/**
 * Abre una imagen vacia con una sola banda. Crea un buffer de 2500x2500 con
 * valores entre 0 y 255. Obtiene la banda de la imagen creada y usa el buffer
 * para escribir un bloque de banda.
 * El test es exitoso si puede grabar la imagen creada a un archivo temporal.
 */
void ImageTest::band_write() {
   suri::Image *pimage = suri::Image::Open("./dummy.dat", suri::Image::ReadWrite,
                                           "envi", 1, 2500, 2500, "uchar");
   CPPUNIT_ASSERT_MESSAGE("No se puede crear la imagen", pimage != NULL);
   unsigned char *pbuffer = new unsigned char[2500 * 2500];
   for (int j = 0; j < 2500; j++)
      for (int i = 0; i < 2500; i++)
         pbuffer[i + j * 2500] = (i + j * 2500) % 256;
   suri::RasterBand *pband = pimage->GetBand(0);
   pband->Write(pbuffer, 0, 0, 2500, 2500);
   CPPUNIT_ASSERT_MESSAGE(std::string("Error al salvar memoria"),
                          pimage->Save(tempFile_, "envi"));

   suri::Image::Close(pimage);
}
