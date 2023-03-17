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

// Includes Suri
#include "ThresholdClassificationAlgorithm.h"
#include "suri/xmlnames.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/DataTypes.h"
#include "ClassInformation.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Macro para registrar el tipo de algoritmo de clasificacion de forma automatica */
AUTO_REGISTER_CLASS(ClassificationAlgorithmInterface, ThresholdClassificationAlgorithm, 0)

/**
 * El algoritmo de clasificacion por umbrales debe verificar los umbrales definidos
 * en cada clase para cada banda.
 * En caso de que el pixel en todas las bandas este dentro del intervalo definido,
 * se considera que pasa a formar parte de la clase definida.
 * En caso contrario, pasa a ser parte de la clase NOCLASE (AND entre condiciones de banda).
 * Precondicion: el indice de las bandas en pSource debe coincidir con el
 * indice de las clases definida dado que la unica forma de identificar
 * a una banda es el indice dentro del vector pSource.
 * @param[out] pDest imagen con ints donde se guardan los ids
 * @param[in] pSource imagen que se quiere clasificar
 * @param[in] Size cantidad de pixeles en la imagen a clasificar
 * @param[in] Classes vector con la informacion de las clases a clasificar
 */
template<typename T>
bool thresholdClassification(
      int* pDest, std::vector<void*> &pSource, size_t Size,
      int NDVPixelValue, double Ndv, bool NdvAvailable,
      const std::vector<ThresholdClassificationAlgorithm::ClassThresholdInfo> &Classes) {

   int bandcount = pSource.size();
   std::vector<T*> psrc;

   if (Classes.empty()) return false;

   // realizo una conversion de los tipos de datos
   for (int i = 0; i < bandcount; i++)
      psrc.push_back(static_cast<T*>(pSource[i]));

   // Para cada pixel en la imagen
   for (size_t pixelposition = 0; pixelposition < Size; pixelposition++) {
      std::vector<ThresholdClassificationAlgorithm::ClassThresholdInfo>::const_iterator cit =
            Classes.begin();

      // Incluir valor no clase para que sea configurable
      int classindex = ClassInformation::NoClassIndex;

      bool invalidpixel = true;

      // Salida para pixel no valido.
      // (valor no valido en todas las bandas involucradas)
      // Ver TCK #7325
      if (NdvAvailable) {
         for (int i = 0; i < bandcount && invalidpixel; ++i) {
            if (static_cast<double>(psrc[i][pixelposition]) != CLASSIFICATION_NDV)
               invalidpixel = false;
         }
      } else {
         invalidpixel = false;
      }

      // Si el pixel es valido hago el proc normal.
      if (invalidpixel == false) {
         for (; classindex == ClassInformation::NoClassIndex && cit != Classes.end(); ++cit) {
            // comparo en cada clase los umbrales de las bandas. Si todos matchean entonces
            // clasifico
            std::vector<ThresholdClassificationAlgorithm::BandThresholdInfo>::const_iterator bit =
                  cit->bands_.begin();
            bool validthreshold = true;
            /**
             * TCK #7503 (Gabriel): 
             * Debido a que existe una seleccion espectral y esa misma influye en la cantidad de 
             * bandas que viene en pSource deja de ser valido utilizar el indice configurado
             * de la banda en la clase para obtener el valro del pixel. 
             * Ejemplo: raster con 5 bandas pero entran al proceso 3 bandas 1,3,5. En este
             * caso la bit->band_ tendria un valor mayor a la cantidad de bandas que viene en pSource
             * (para los casos 3 y 5). Si se toma como precondicion que siempre llegan en orden
             * numerico las bandas en cada definicion de clases se puede cambiar el indice de banda
             * que viene en bit por un indice secuencial
            **/
            for (int b = 0; validthreshold && bit != (cit->bands_.end()); ++bit, ++b) {
               if ((int) psrc.size() > b) {
                  // obtengo el valor del pixel en la banda
                  double pixeldata =
                        static_cast<double>(psrc.at(b)[pixelposition]);
                  // verifico si el pixel entra en el intervalo definido
                  validthreshold = validthreshold
                        && ((pixeldata >= bit->min_ && pixeldata < bit->max_)
                        // Cuando el intervalo es un punto
                              || (bit->min_ == bit->max_ && pixeldata == bit->min_));

               }
            }
            if (validthreshold) {
               // cuando el pixel encuentra una clase se considera clasificado
               // y sale del for
               classindex = cit->classIndex_;
            }
         }
      } else {
         classindex = NDVPixelValue;
      }
      pDest[pixelposition] = classindex;
   }

   return true;
}

/** Inicializa mapa de tipos de datos. */
INITIALIZE_DATATYPE_MAP(ThresholdClassificationAlgorithm::ClassifyFunctionType,
                        thresholdClassification);

/** Constructor. Inicializa el indice no de clase SINCLASE **/
ThresholdClassificationAlgorithm::ThresholdClassificationAlgorithm() :
      noClassIndex_(0) {
}

/** destructor **/
ThresholdClassificationAlgorithm::~ThresholdClassificationAlgorithm() {
}

/**
 * Los metodos Update y GetNode: permiten persistir los parametros que
 * configuran al cada algoritmo en particular.
 * @param[in] pAlgorithmNode nodo XML con parametros del algoritmo
 * @return bool
 */
/** Actualiza los datos internos del algoritmo con los del nodo. */
bool ThresholdClassificationAlgorithm::Update(const wxXmlNode* pAlgorithmNode) {
   wxXmlNode* pchild = pAlgorithmNode ? pAlgorithmNode->GetChildren() : NULL;
   if (pchild == NULL)
      return false;
   bool ok = true;
   classes_.clear();
   while (ok && pchild != NULL) {
      if (pchild->GetName().compare(CLASSIFICATION_NO_CLASS_NODE) == 0 ) {
         noClassIndex_ = StringToNumber<int>(pchild->GetNodeContent().c_str());
      } else if (pchild->GetName().compare(CLASSIFICATION_CLASS_NODE) == 0 ) {
         ClassThresholdInfo classinfo;
         ok = LoadClassInfoFromXml(pchild, classinfo);
         if (ok)
            classes_.push_back(classinfo);
      }
      pchild = pchild->GetNext();
   }
   return ok;
}

/**
 * Metodo auxiliar que actualiza la informacion de clases en funcion del xml
 * @param[in] pClassNode nodo xml con la informacion de clases
 * @param[out] ClassInfo estructura en donde se almacena la configuracion de
 * clase que se encontraba en el xml
 * @return true en caso de parsear con exito el nodo
 * @return false en caso contrario
 */
bool ThresholdClassificationAlgorithm::LoadClassInfoFromXml(
      const wxXmlNode* pClassNode, ClassThresholdInfo& ClassInfo) {
   wxXmlNode* pchild = pClassNode ? pClassNode->GetChildren() : NULL;
   if (!pchild)
      return false;
   bool ok = true;
   while (ok && pchild != NULL) {
      if (pchild->GetName().compare(CLUSTER_INDEX_NODE) == 0) {
         ClassInfo.classIndex_ = StringToNumber<int>(pchild->GetNodeContent().c_str());
      } else if (pchild->GetName().compare(CLASSIFICATION_BAND_NODE) == 0) {
         wxXmlNode* pband = pchild->GetChildren();
         BandThresholdInfo bandinfo;
         while (ok && pband != NULL) {
            if (pband->GetName().compare(CLUSTER_INDEX_NODE) == 0) {
               bandinfo.band_ = StringToNumber<int>(pband->GetNodeContent().c_str());
            } else if (pband->GetName().compare(CLASSIFICATION_BAND_MIN_NODE) == 0) {
               bandinfo.min_ = StringToNumber<double>(pband->GetNodeContent().c_str());
            } else if (pband->GetName().compare(CLASSIFICATION_BAND_MAX_NODE) == 0) {
               bandinfo.max_ = StringToNumber<double>(pband->GetNodeContent().c_str());
            }
            pband = pband->GetNext();
         }
         ClassInfo.bands_.push_back(bandinfo);
      }
      pchild = pchild->GetNext();
   }
   return ok;
}

/** Genera nodo con los datos internos del algoritmo. Se utilizara una estructura
 *  como la siguiente para la definicion
 *  * \verbatim
 * <classificacion>
 *        <nombre_aglritmo>ThresholdClassificationAlgorithm</nombre_aglritmo>
 *          <noclase>
 *                 <indice>0</indice> !-- nocluster no puede tener indice anterior.
 *          </noclase>
 *          <clase>
 *               <indice>1</indice> <!-- Indice que posee el cluster -->
 *               <banda>
 *                    <!-- Indice de la banda sobre que aplica el umbral -->
 *                    <indice> 0 </indice>
 *                    <!--Umbrales dedinidos por cada clase  -->
 *                    <min_umbral> </min_umbral>
 *                    <max_umbral> </m_umbral>
 *               </banda>
 *          </clase>
 *        </algoritmo>
 * </classificacion>
 * \endverbatim*/
wxXmlNode* ThresholdClassificationAlgorithm::GetNode() {
   wxXmlNode *palgorithmnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                             CLASSIFICATION_ALGORITHM_NODE);
   // se agrega nombre del algoritmo
   new wxXmlNode(new wxXmlNode(palgorithmnode, wxXML_ELEMENT_NODE,
   ALGORITHM_NAME_NODE),
                 wxXML_TEXT_NODE, wxEmptyString,
                 ALGORITHM_VALUE_THRESHOLD);

   // generacion de nodo no clase con indice 0
   wxXmlNode* pclassnode = new wxXmlNode(palgorithmnode, wxXML_ELEMENT_NODE,
                                         CLASSIFICATION_NO_CLASS_NODE);
   // se agrega como hijo el indice de no clase
   new wxXmlNode(new wxXmlNode(pclassnode, wxXML_ELEMENT_NODE,
   CLUSTER_INDEX_NODE), wxXML_TEXT_NODE, wxEmptyString, "0");

   std::vector<ClassThresholdInfo>::const_iterator it = classes_.begin();
   // se recorren las definiciones de las clases y se arma el xml de las mismas
   for (; it != classes_.end(); ++it) {
      pclassnode = CreateClassNode(*it);
      // completar el nodo
      palgorithmnode->AddChild(pclassnode);
   }
   return palgorithmnode;
}


/**
 * Metodo auxiliar que crea un nodo xml que representa la informacion
 * que contiene la clase que se pasa por parametro
 * @param[in] ClassInfo estructura que contiene la informacion de la clase
 * @return nodo xml con la informacion
 */
wxXmlNode* ThresholdClassificationAlgorithm::CreateClassNode(
      const ClassThresholdInfo& ClassInfo) const {
   wxXmlNode* pclassnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
   CLASSIFICATION_CLASS_NODE);
   // nodo con indice de la clase
   new wxXmlNode(new wxXmlNode(pclassnode, wxXML_ELEMENT_NODE,
   CLUSTER_INDEX_NODE),
                 wxXML_TEXT_NODE, wxEmptyString,
                 NumberToString<int>(ClassInfo.classIndex_).c_str());

   std::vector<BandThresholdInfo>::const_iterator bit = ClassInfo.bands_.begin();
   for (; bit != ClassInfo.bands_.end(); ++bit) {
      // se genera el nodo para cada banda
      wxXmlNode* pbandsnode = CreateBandNode(*bit);
      pclassnode->AddChild(pbandsnode);
   }
   return pclassnode;
}
/**
 * Metodo auxiliar que genera un nodo xml con la informacion de los umbrales
 * de la banda que se pasa por parametro
 * @param[in] BandInfo estructura que contiene la informacion de las bandas
 * @return nodo xml con el contenido de la banda pasada por parametro
 */
wxXmlNode* ThresholdClassificationAlgorithm::CreateBandNode(
      const BandThresholdInfo& BandInfo) const {
   // se genera el nodo para cada banda
   wxXmlNode* pbandsnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
   CLASSIFICATION_BAND_NODE);
   // indice
   new wxXmlNode(new wxXmlNode(pbandsnode, wxXML_ELEMENT_NODE,
   CLUSTER_INDEX_NODE),
                 wxXML_TEXT_NODE, wxEmptyString,
                 NumberToString<int>(BandInfo.band_).c_str());
   // min umbral
   new wxXmlNode(new wxXmlNode(pbandsnode, wxXML_ELEMENT_NODE,
   CLASSIFICATION_BAND_MIN_NODE),
                 wxXML_TEXT_NODE, wxEmptyString, DoubleToString(BandInfo.min_).c_str());
   // max umbral
   new wxXmlNode(new wxXmlNode(pbandsnode, wxXML_ELEMENT_NODE,
   CLASSIFICATION_BAND_MAX_NODE),
                 wxXML_TEXT_NODE, wxEmptyString, DoubleToString(BandInfo.max_).c_str());
   return pbandsnode;
}

/**
 * Clasifica los datos de entrada(pSource) y guarda en pDest un entero con
 * el id de la clase ganadora.
 * @param[out] pDest destino de la clasificacion
 * @param[in] pSource fuente de datos para la clasificacion
 * @param[in] Size cantidad de datos
 * @param[in] DataType tipo de dato de la imagen a clasificar.
 * @return bool
 */
/** Clasifica los datos de entrada */
bool ThresholdClassificationAlgorithm::Classify(int* pDest, std::vector<void*> pSource,
                                                size_t Size,
                                                const std::string &DataType) {
   function_ = thresholdClassificationTypeMap[DataType];
   return function_(pDest, pSource, Size, GetNDVPixelValue(), GetNoDataValue(),
                    IsNoDataValueAvailable(), classes_);
}

/** Verifica que el algoritmo este bien configurado mirando que los umbrales
 *  para las clases se encuentren bien definidos */
bool ThresholdClassificationAlgorithm::Check() const {
   bool validdef = true;

   std::vector<ClassThresholdInfo>::const_iterator it = classes_.begin();
   for (; validdef && it != classes_.end(); ++it) {
      std::string minstr, maxstr;
      std::vector<BandThresholdInfo>::const_iterator bit = it->bands_.begin();
      for (; validdef && bit != it->bands_.end(); ++bit) {
         if (bit->min_ > bit->max_)
            validdef = false;
      }
   }
   return validdef;
}
} /** namespace suri */
