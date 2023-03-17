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

#ifndef SPECTRALANGLEMAPPERALGORITHM_H_
#define SPECTRALANGLEMAPPERALGORITHM_H_

// Includes Estandar
// Includes Suri
#include "ClassificationAlgorithmInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/**
 *  Clase que implementa el algoritmo spectral angle mapper para la clasificacion
 *  Si se configura el ClassificationRenderer con una instancia de esta clase
 *  se clasificara la imagen usando el spectral angle mapper.
 */
class SpectralAngleMapperAlgorithm : public suri::ClassificationAlgorithmInterface {
   /** Ctor. de Copia. */
   SpectralAngleMapperAlgorithm(const SpectralAngleMapperAlgorithm &Algorithm);
public:
   /** Estructura que almacena la definicion de firmas espectrales para una clase **/
   struct BandSpectralInfo {
      int band_;
      double reflectance_;
   };
   struct ClassSpectralInfo {
      ClassSpectralInfo() : classIndex_(0) {}
      int classIndex_;
      double maxangle_;
      std::vector<BandSpectralInfo> bands_;
   };
   /** Funcion clasificacion */
   typedef bool (*ClassifyFunctionType)(
         int*, std::vector<void*>&, size_t, int, double, bool,
         const std::vector<SpectralAngleMapperAlgorithm::ClassSpectralInfo> &Classes);   

   /**
    * Definicion de puntero a funcion para poder configurar la salida como float para el calculo de
    * del angulo espectral en el pixel de salida
    */
	typedef bool (*FloatClassifyFunctionType)(
         float*, std::vector<void*>&, size_t, int, double, bool,
         const std::vector<SpectralAngleMapperAlgorithm::ClassSpectralInfo> &Classes);

   /** Constructor */
   SpectralAngleMapperAlgorithm();
   /** Destructor */
   virtual ~SpectralAngleMapperAlgorithm();
   /** Actualiza los datos del algoritmo con los del nodo. */
   virtual bool Update(const wxXmlNode* pAlgorithmNode);
   /** Genera nodo con los datos del algoritmo. */
   virtual wxXmlNode* GetNode();
   /** Determina el tipo de dato a la entrada del algoritmo */
   virtual bool Classify(int* pDest, std::vector<void*> pSource, size_t Size,
                         const std::string &DataType);   
   /**
    * Funcion para poder configurar la salida como float para el calculo de
    * del angulo espectral en el pixel de salida
    */
   virtual bool Classify(float* pDest, std::vector<void*> pSource, size_t Size,
                         const std::string &DataType);

   /** verifica que las dimensiones de los parametros sean consistentes */
   virtual bool Check() const;
   /** Metodo que retorna un puntero al algoritmo de clasificacion */
   virtual SpectralAngleMapperAlgorithm* Initialize(
         const std::string &AlgorithmName) {
      return this;
   }
   /** Indica la definicion de clases a utilizar por el algoritmo **/
   void SetClasses(std::vector<ClassSpectralInfo>& Classes) {
      classes_ = Classes;
   }

private:
   bool LoadClassInfoFromXml(const wxXmlNode* pClassNode, ClassSpectralInfo& ClassInfo);
   /**
    * Metodo auxiliar que crea un nodo xml que representa la informacion
    * que contiene la clase que se pasa por parametro
    * @param[in] ClassInfo estructura que contiene la informacion de la clase
    * @return nodo xml con la informacion
    */
   wxXmlNode* CreateClassNode(const ClassSpectralInfo& ClassInfo) const;
   /**
    * Metodo auxiliar que genera un nodo xml con la informacion de los umbrales
    * de la banda que se pasa por parametro
    * @param[in] BandInfo estructura que contiene la informacion de las bandas
    * @return nodo xml con el contenido de la banda pasada por parametro
    */
   wxXmlNode* CreateBandNode(const BandSpectralInfo& BandInfo) const;
   /** clases para la clasificacion */
   std::vector<ClassSpectralInfo> classes_;
   /** indice para no clase **/
   int noClassIndex_;
   /*! puntero a funcion para la clasificacion */
   ClassifyFunctionType function_;
};

} /** namespace suri */

#endif /* SPECTRALANGLEMAPPERALGORITHM_H_ */
