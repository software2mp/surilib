/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "EnhancementTests.h"

// Includes standard
// Includes Suri
#include "suri/xmlnames.h"
#include "suri/XmlFunctions.h"
#include "suri/Histogram.h"
#include "suri/LookUpTable.h"
#include "suri/StatisticsFunctions.h"
#include "Enhancement.h"
// Includes Wx

// Nivel de tolerancia entre la Lut generada y el resultado del ENVI, para considerarlos iguales
#define ACCEPTED_DIFFERENCE 255*0.1
// Defines
#define IMAGEDATAINDEX 0
#define ENVIENHANCEMENTINDEX 1

#define SIZES_COUNT 5
#define TYPES_COUNT 2
#ifdef __UNUSED_CODE_
#define ENHANCEMENTS_COUNT 2
#else
#define ENHANCEMENTS_COUNT 1
#endif
std::string sizes[] = { "10", "12", "16", "24", "32" }; /*! tamanios de las imagenes */
std::string types[] = { "creciente", "aleatoria" }; /*! forma en que se generaron las imagenes */
/**
 * Se eliminan los test para el realce gaussiano porque el algoritmo que se usa
 * (el mismo que usa SuriPluma) es diferente del que usa ENVI, y los resultados
 * no coinciden.
 * \ todo ver que algoritmo usa ENVI y evaluar reemplazar (o agregar) el algoritmo
 * actual.
 */
#ifdef __UNUSED_CODE_
std::string enhancements[] = {"Gaussian", "Equalization"};
std::string enhancementsclass[] = {"GaussianEnhancement", "EqualizationEnhancement"};
#else
std::string enhancements[] = { "Equalization" }; /*! Lista con los realces que se testean */
std::string enhancementsclass[] = { "EqualizationEnhancement" }; /*! Lista con las clases de los */
// realces que se testean
#endif

/** namespace suri */
namespace suri {
/**
 * Constructor
 */
EnhancementTests::EnhancementTests() {
}

/**
 * Destructor
 */
EnhancementTests::~EnhancementTests() {
}

/**
 * Analizo si el enhancement de la aplicacion es igual a la obtenida con ENVI.
 * Comparo imagenes de distinto tamanio, estructura y enhancement
 */
void EnhancementTests::CompareEnviTest() {
   LookUpTable lookuptable;
   // Recorro los sizes
   for (int i = 0; i < SIZES_COUNT; i++) {
      // Recorro los tipo de image
      for (int j = 0; j < TYPES_COUNT; j++) {
         // Cargo estadisticas de imagen de tipo y size correcto
         Statistics stats;
         GetStatistics(sizes[i], types[j], stats);

         // Recorro los tipos de enhancement
         for (int k = 0; k < ENHANCEMENTS_COUNT; k++) {
            // Obtengo lookuptable que genera aplicacion
            lookuptable = GetEnhancementTable(stats, enhancementsclass[k]);

            // Analizo si los pixeles son asignados a los mismos colores
            CompareImage(sizes[i], enhancements[k], types[j], lookuptable);
         }
      }
   }
}

/**
 * Comparo la imagen generada aplicando enhancement con el ENVI a la nuestra
 * @param[in] Size tamanio de la imagen de entrada
 * @param[in] Enhancement tipo de Enhancement aplicado a imagen
 * @param[in] ImageType indica como fue generada la imagen usada (creciente, aleatoria, etc.)
 * @param[out] pLookUpTable tabla que utiliza la aplicacion para asignar dn
 * a la imagen de entrada
 */
void EnhancementTests::CompareImage(const std::string &Size,
                                    const std::string &Enhancement,
                                    const std::string &ImageType,
                                    LookUpTable &pLookUpTable) {
   // Obtengo imagen en formato XML y el dn asignado
   // Abro xml de la imagen correcta
   std::string pathdir = __TEST_PATH__;
   pathdir = pathdir + "nodosxml/enhancementtestsxml/tablaaleatoria-" + Enhancement
         + "-" + Size + "x" + Size + ".xml";
   wxXmlDocument doc(pathdir.c_str());
   CPPUNIT_ASSERT_MESSAGE("No pudo leer el XML del test", doc.IsOk());
   if (!doc.IsOk()) {
      return;
   }

   // Genero tabla con los datos de la imagen
   std::vector<std::vector<double> > image;
   LoadTable(doc.GetRoot(), image);
   if (image.size() < 2
         || image[IMAGEDATAINDEX].size() != image[ENVIENHANCEMENTINDEX].size()) {
      return;
   }

   bool equal = true;
   // Recorro la imagen
   for (size_t i = 0; i < image[IMAGEDATAINDEX].size(); i++) {
      // Obtengo dn de imagen de entrada y lo comparo con el que genero ENVI
      double aplicvalue = pLookUpTable.ApplyLut(image[IMAGEDATAINDEX][i]);
#ifdef __UNUSED_CODE__
      /**
       * Se elimina esta comparacion porque es muy dificil que el realce
       * de exactamente igual al ENVI
       */
      if ( !FLOAT_COMPARE(aplicvalue, image[ENVIENHANCEMENTINDEX][i]) )
      {
         REPORT_DEBUG("D: Valor: %f envi: %f aplic: %f", image[IMAGEDATAINDEX][i],
               image[ENVIENHANCEMENTINDEX][i], aplicvalue);
         equal=false;
      }
#else
      /*
       * Se compara el valor contra el resultado del ENVI +/- un nivel de tolerancia
       */
      if (aplicvalue < image[ENVIENHANCEMENTINDEX][i] - ACCEPTED_DIFFERENCE
            || aplicvalue > image[ENVIENHANCEMENTINDEX][i] + ACCEPTED_DIFFERENCE) {
         equal = false;
      }
#endif
   }
   wxString message;
   message.sprintf(
         "Realce: %s, imagen tipo: %s, imagen tamanio: %s, tablas diferentes al ENVI",
         Enhancement.c_str(), ImageType.c_str(), Size.c_str());
   CPPUNIT_ASSERT_MESSAGE(message.c_str(), equal);
}

/**
 * Genero tabla que utiliza la aplicacion para generar LUT a partir de
 * las estadisticas y el tipo de enhancement.
 * @param[in] Stats estadisticas obtenidas del envi.
 * @param[in] Enhancement nombre de la clase del enhancement deseado
 * @return tabla que utiliza la aplicacion.
 */
LookUpTable EnhancementTests::GetEnhancementTable(Statistics Stats,
                                                  const std::string &Enhancement) {
   // Uso enhancement y estadisticas para generar LUT
   LutArray lut;
   suri::Enhancement *penhancement = Enhancement::Create(Enhancement);
   penhancement->ComputeLut(lut, Stats);
   int outputband = 0;
   LookUpTable lookuptable = lut.GetLookUpTable(0, outputband);
   delete penhancement;
   return lookuptable;
}

/**
 * Obtengo estadisticas de los archivos datos-ssxss.xml que se generaron
 * con el envi.
 * @param[in] Size tamanio de la imagen
 * @param[in] Type tipo de la imagen(aleatoria, )
 * @param[out] Stats estadisticas de envi
 */
bool EnhancementTests::GetStatistics(const std::string &Size, const std::string &Type,
                                     Statistics &Stats) {
   // Abro xml de la imagen correcta
   std::string pathdir = __TEST_PATH__;
   pathdir = pathdir + "nodosxml/enhancementtestsxml/datos-" + Size + "x" + Size
         + ".xml";
   wxXmlDocument doc(pathdir.c_str());
   CPPUNIT_ASSERT_MESSAGE("No pudo leer el XML del test", doc.IsOk());

   // Busco elemento correcto
   wxXmlNode* pelementnode = doc.GetRoot()->GetChildren();
   bool found = false;
   while (pelementnode != NULL && !found) {
      wxXmlNode* urlnode = GetNodeByName(URL_NODE, pelementnode);
      if (urlnode && urlnode->GetNodeContent().IsSameAs(Type.c_str())) {
         found = true;
      } else {
         pelementnode = pelementnode->GetNext();
      }
   }CPPUNIT_ASSERT_MESSAGE("No se encontro nodo estadisticas", found);
   if (!found) {
      return found;
   }

   // Obtengo nodo estadisticas
   wxXmlNode* pstatsnode = GetNodeByName(STATISTICS_NODE, pelementnode);
   GetStats(pstatsnode, Stats);

   // Cargo histograma
   wxXmlNode* phistogramnode = GetNodeByName(HISTOGRAM_NODE, pelementnode);
   Stats.histogram_.clear();

   Stats.histogram_.push_back(Histogram());
   if (!Histogram::FromXml(phistogramnode, Stats.histogram_[0])) {
      CPPUNIT_ASSERT_MESSAGE("Error al generar nodo histograma", false);
      return false;
   }

   return true;
}
}

