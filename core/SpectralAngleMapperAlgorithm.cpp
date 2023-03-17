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
#include <math.h>

// Includes Suri
#include "SpectralAngleMapperAlgorithm.h"
#include "suri/xmlnames.h"
#include "suri/DataTypes.h"
#include "suri/AuxiliaryFunctions.h"
#include "ClassInformation.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/** Macro para registrar el tipo de algoritmo de clasificacion de forma automatica */
AUTO_REGISTER_CLASS(ClassificationAlgorithmInterface, SpectralAngleMapperAlgorithm, 0)

/**
 * El algoritmo de clasificacion spectral angle mapper debe verificar las firmas espectrales
 * definidas en cada clase para cada banda. Cada registro de la firma espectral
 * En caso de que el pixel en todas las bandas este dentro del intervalo definido,
 * se considera que pasa a formar parte de la clase definida.
 * En caso contrario, pasa a ser parte de la clase NOCLASE (AND entre condiciones de banda).
 * @param[out] pDest imagen con ints donde se guardan los ids
 * @param[in] pSource imagen que se quiere clasificar
 * @param[in] Size cantidad de pixeles en la imagen a clasificar
 * @param[in] Classes vector con la informacion de las clases a clasificar
 */
template<typename T>
bool samClassification(
      int* pDest, std::vector<void*> &pSource, size_t Size,
      int NDVPixelValue, double Ndv, bool NdvAvailable,
      const std::vector<SpectralAngleMapperAlgorithm::ClassSpectralInfo> &Classes) {
   int bandcount = pSource.size();
   std::vector<T*> psrc;

   if (Classes.empty())
      return false;

   // realizo una conversion de los tipos de datos
   for (int i = 0; i < bandcount; i++)
      psrc.push_back(static_cast<T*>(pSource[i]));

   // Para cada pixel en la imagen
   for (size_t pixelposition = 0; pixelposition < Size; pixelposition++) {
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
         std::vector<SpectralAngleMapperAlgorithm::ClassSpectralInfo>::const_iterator cit =
               Classes.begin();

         for (int c = 0;
               classindex == ClassInformation::NoClassIndex && cit != Classes.end();
               ++cit, ++c) {
            double spectralangle = cit->maxangle_ + 1;
            double upper = 0;
            double lowerpixel = 0, lowerreflectance = 0;
            std::vector<SpectralAngleMapperAlgorithm::BandSpectralInfo>::const_iterator bit =
                  cit->bands_.begin();
            for (; bit != (cit->bands_.end()); ++bit) {
               // obtengo el valor del pixel en la banda
               double pixeldata =
                     static_cast<double>(psrc.at(bit->band_)[pixelposition]);
               upper += (pixeldata * bit->reflectance_);
               lowerpixel += (pixeldata * pixeldata);
               lowerreflectance += (bit->reflectance_ * bit->reflectance_);
            }
            if (lowerpixel == 0 || lowerreflectance == 0) continue;
            double lower = sqrt(lowerpixel * lowerreflectance);
            spectralangle = acos(upper / lower);
            classindex =
                  spectralangle <= cit->maxangle_ ? cit->classIndex_ :
                                                    ClassInformation::NoClassIndex;
         }
      } else {
         // Sino le pongo un valor para la parte de la imagen que no
         // es valida.
         classindex = NDVPixelValue;
      }
      pDest[pixelposition] = classindex;
   }

   return true;
}

/**
 * El algoritmo de clasificacion spectral angle mapper debe verificar las firmas espectrales
 * definidas en cada clase para cada banda. Cada registro de la firma espectral
 * En caso de que el pixel en todas las bandas este dentro del intervalo definido,
 * se considera que pasa a formar parte de la clase definida.
 * En caso contrario, pasa a ser parte de la clase NOCLASE (AND entre condiciones de banda).
 * @param[out] pDest imagen con ints donde se guardan los ids
 * @param[in] pSource imagen que se quiere clasificar
 * @param[in] Size cantidad de pixeles en la imagen a clasificar
 * @param[in] Classes vector con la informacion de las clases a clasificar
 */
template<typename T>
bool samCalculator(
      float* pDest, std::vector<void*> &pSource, size_t Size,
      int NDVPixelValue, double Ndv, bool NdvAvailable,
      const std::vector<SpectralAngleMapperAlgorithm::ClassSpectralInfo> &Classes) {
   int bandcount = pSource.size();
   std::vector<T*> psrc;

   if (Classes.empty()) return false;

   // realizo una conversion de los tipos de datos
   for (int i = 0; i < bandcount; i++)
      psrc.push_back(static_cast<T*>(pSource[i]));

   // Para cada pixel en la imagen
   for (size_t pixelposition = 0; pixelposition < Size; pixelposition++) {

      int classindex = ClassInformation::NoClassIndex;

      bool invalidpixel = true;

      // Salida para pixel no valido.
      // (valor no valido en todas las bandas involucradas)
      if (NdvAvailable) {
         for (int i = 0; i < bandcount && invalidpixel; ++i) {
            if (static_cast<double>(psrc[i][pixelposition]) != Ndv) invalidpixel =
                  false;
         }
      } else {
         invalidpixel = false;
      }

      // Si el pixel es valido hago el proc normal.
      if (invalidpixel == false) {
         std::vector<SpectralAngleMapperAlgorithm::ClassSpectralInfo>::const_iterator cit =
               Classes.begin();

         double pixelspectralangle = Classes.begin()->maxangle_ + 1;
         for (int c = 0;
               classindex == ClassInformation::NoClassIndex && cit != Classes.end();
               ++cit, ++c) {
            double spectralangle = cit->maxangle_ + 1;
            double upper = 0;
            double lowerpixel = 0, lowerreflectance = 0;
            std::vector<SpectralAngleMapperAlgorithm::BandSpectralInfo>::const_iterator bit =
                  cit->bands_.begin();
            for (; bit != (cit->bands_.end()); ++bit) {
               // obtengo el valor del pixel en la banda
               double pixeldata =
                     static_cast<double>(psrc.at(bit->band_)[pixelposition]);
               upper += (pixeldata * bit->reflectance_);
               lowerpixel += (pixeldata * pixeldata);
               lowerreflectance += (bit->reflectance_ * bit->reflectance_);
            }
            if (lowerpixel == 0 || lowerreflectance == 0) continue;
            double lower = sqrt(lowerpixel * lowerreflectance);
            spectralangle = acos(upper / lower);
            pixelspectralangle =
                  spectralangle < pixelspectralangle ? spectralangle :
                                                       pixelspectralangle;
            spectralangle <= cit->maxangle_ ? cit->classIndex_ :
                                              ClassInformation::NoClassIndex;
            classindex = pixelspectralangle;
         }
      } else {
         // Sino le pongo un valor para la parte de la imagen que no
         // es valida.
         classindex = NDVPixelValue;
      }
      pDest[pixelposition] = classindex;
   }
   return true;
}

/** Constructor */
SpectralAngleMapperAlgorithm::SpectralAngleMapperAlgorithm() :
      noClassIndex_(0) {
}

/** Inicializa mapa de tipos de datos. */
INITIALIZE_DATATYPE_MAP(SpectralAngleMapperAlgorithm::ClassifyFunctionType,
                        samClassification);

INITIALIZE_DATATYPE_MAP(SpectralAngleMapperAlgorithm::FloatClassifyFunctionType, samCalculator);

/** Destructor */
SpectralAngleMapperAlgorithm::~SpectralAngleMapperAlgorithm() {
}

/**
 * Actualiza los datos internos del algoritmo con los del nodo.
 * @param[in] pAlgorithmNode nodo del algoritmo
 * @return informa si el nodo era del tipo correto.
 */
bool SpectralAngleMapperAlgorithm::Update(const wxXmlNode* pAlgorithmNode) {
   wxXmlNode* pchild = pAlgorithmNode ? pAlgorithmNode->GetChildren() : NULL;
   if (pchild == NULL) return false;
   bool ok = true;
   classes_.clear();
   while (ok && pchild != NULL) {
      if (pchild->GetName().compare(CLASSIFICATION_NO_CLASS_NODE) == 0) {
         noClassIndex_ = StringToNumber<int>(pchild->GetNodeContent().c_str());
      } else if (pchild->GetName().compare(CLASSIFICATION_CLASS_NODE) == 0) {
         ClassSpectralInfo classinfo;
         ok = LoadClassInfoFromXml(pchild, classinfo);
         if (ok) classes_.push_back(classinfo);
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
bool SpectralAngleMapperAlgorithm::LoadClassInfoFromXml(const wxXmlNode* pClassNode,
                                                        ClassSpectralInfo& ClassInfo) {
   wxXmlNode* pchild = pClassNode ? pClassNode->GetChildren() : NULL;
   if (!pchild) return false;
   bool ok = true;
   while (ok && pchild != NULL) {
      if (pchild->GetName().compare(CLUSTER_INDEX_NODE) == 0) {
         ClassInfo.classIndex_ = StringToNumber<int>(pchild->GetNodeContent().c_str());
      } else if (pchild->GetName().compare(SAM_NODE) == 0) {
         ClassInfo.maxangle_ = StringToNumber<double>(pchild->GetNodeContent().c_str());
      } else if (pchild->GetName().compare(CLASSIFICATION_BAND_NODE) == 0) {
         wxXmlNode* pband = pchild->GetChildren();
         BandSpectralInfo bandinfo;
         while (ok && pband != NULL) {
            if (pband->GetName().compare(CLUSTER_INDEX_NODE) == 0) {
               bandinfo.band_ = StringToNumber<int>(pband->GetNodeContent().c_str());
            } else if (pband->GetName().compare(CLASSIFICATION_REFLECTANCE_NODE) == 0) {
               bandinfo.reflectance_ = StringToNumber<double>(pband->GetNodeContent().c_str());
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
 *        <nombre_aglritmo>SpectralAngleMapperAlgorithm</nombre_aglritmo>
 *          <noclase>
 *                 <indice>0</indice> !-- nocluster no puede tener indice anterior.
 *          </noclase>
 *          <sam> double </sam>
 *          <clase>
 *               <indice>1</indice> <!-- Indice que posee el cluster -->
 *               <banda>
 *                    <!-- Indice de la banda -->
 *                    <indice> 0 </indice>
 *                    <!-- Reflectancia definida para cada banda  -->
 *                    <reflectance> double </reflectance>
 *               </banda>
 *          </clase>
 *        </algoritmo>
 * </classificacion>
 * \endverbatim*/
wxXmlNode* SpectralAngleMapperAlgorithm::GetNode() {
   wxXmlNode *palgorithmnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                             wxT(CLASSIFICATION_ALGORITHM_NODE));
   // se agrega nombre del algoritmo
   new wxXmlNode(new wxXmlNode(palgorithmnode, wxXML_ELEMENT_NODE,
   ALGORITHM_NAME_NODE),
                 wxXML_TEXT_NODE, wxEmptyString,
                 ALGORITHM_VALUE_SAM);

   // generacion de nodo no clase con indice 0
   wxXmlNode* pclassnode = new wxXmlNode(palgorithmnode, wxXML_ELEMENT_NODE,
   CLASSIFICATION_NO_CLASS_NODE);
   // se agrega como hijo el indice de no clase
   wxString noclassindex;
   noclassindex << ClassInformation::NoClassIndex;
   new wxXmlNode(new wxXmlNode(pclassnode, wxXML_ELEMENT_NODE,
   CLUSTER_INDEX_NODE),
                 wxXML_TEXT_NODE, wxEmptyString, noclassindex);

   std::vector<ClassSpectralInfo>::const_iterator it = classes_.begin();
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
wxXmlNode* SpectralAngleMapperAlgorithm::CreateClassNode(
      const ClassSpectralInfo& ClassInfo) const {
   wxXmlNode* pclassnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
   CLASSIFICATION_CLASS_NODE);
   // nodo con indice de la clase
   new wxXmlNode(new wxXmlNode(pclassnode, wxXML_ELEMENT_NODE,
   CLUSTER_INDEX_NODE),
                 wxXML_TEXT_NODE, wxEmptyString,
                 NumberToString<int>(ClassInfo.classIndex_).c_str());
   // nodo con angulo espectral de la clase
   new wxXmlNode(new wxXmlNode(pclassnode, wxXML_ELEMENT_NODE, SAM_NODE),
                 wxXML_TEXT_NODE, wxEmptyString, DoubleToString(ClassInfo.maxangle_));
   std::vector<BandSpectralInfo>::const_iterator bit = ClassInfo.bands_.begin();
   for (; bit != ClassInfo.bands_.end(); ++bit) {
      // se genera el nodo para cada banda
      wxXmlNode* pbandsnode = CreateBandNode(*bit);
      pclassnode->AddChild(pbandsnode);
   }
   return pclassnode;
}

/**
 * Metodo auxiliar que genera un nodo xml con la informacion de la firma espectral
 * de la banda que se pasa por parametro
 * @param[in] BandInfo estructura que contiene la informacion de las bandas
 * @return nodo xml con el contenido de la banda pasada por parametro
 */
wxXmlNode* SpectralAngleMapperAlgorithm::CreateBandNode(
      const BandSpectralInfo& BandInfo) const {
   // se genera el nodo para cada banda
   wxXmlNode* pbandsnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
   CLASSIFICATION_BAND_NODE);
   // indice
   new wxXmlNode(new wxXmlNode(pbandsnode, wxXML_ELEMENT_NODE,
   CLUSTER_INDEX_NODE),
                 wxXML_TEXT_NODE, wxEmptyString,
                 NumberToString<int>(BandInfo.band_).c_str());
   // reflectancia
   new wxXmlNode(new wxXmlNode(pbandsnode, wxXML_ELEMENT_NODE, CLASSIFICATION_REFLECTANCE_NODE),
                 wxXML_TEXT_NODE, wxEmptyString, DoubleToString(BandInfo.reflectance_).c_str());
   return pbandsnode;
}

/**
 * Utiliza la funcion de sam para asignar una clase a cada pixel.
 * @param[in] pDest destino de la clasificacion
 * @param[in] pSource fuente de datos para clasificar
 * @param[in] Size cantidad de pixeles
 * @return true si puede aplicar la funcion de clasificacion, false en otro caso
 */
bool SpectralAngleMapperAlgorithm::Classify(int* pDest, std::vector<void*> pSource,
                                            size_t Size, const std::string &DataType) {
   function_ = samClassificationTypeMap[DataType];
   return function_(pDest, pSource, Size, GetNDVPixelValue(), GetNoDataValue(),
                    IsNoDataValueAvailable(), classes_);
}

/**
 * Utiliza la funcion de sam para asignar una clase a cada pixel.
 * @param[in] pDest destino de la clasificacion
 * @param[in] pSource fuente de datos para clasificar
 * @param[in] Size cantidad de pixeles
 * @return true si puede aplicar la funcion de clasificacion, false en otro caso
 */
bool SpectralAngleMapperAlgorithm::Classify(float* pDest, std::vector<void*> pSource,
                                            size_t Size, const std::string &DataType) {
   return samCalculatorTypeMap[DataType](pDest, pSource, Size, GetNDVPixelValue(),
                                         GetNoDataValue(), IsNoDataValueAvailable(),
                                         classes_);
}

bool SpectralAngleMapperAlgorithm::Check() const {
   bool validdef = true;
   std::vector<ClassSpectralInfo>::const_iterator it = classes_.begin();
   for (; validdef && it != classes_.end(); ++it) {
      std::string minstr, maxstr;
      std::vector<BandSpectralInfo>::const_iterator bit = it->bands_.begin();
      for (; validdef && bit != it->bands_.end(); ++bit) {
//         if (bit->min_ > bit->max_)
//            validdef = false;
      }
   }
   return validdef;
}

} /** namespace suri */
