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

#ifndef THRESHOLDCLASSIFICATIONALGORITHM_H_
#define THRESHOLDCLASSIFICATIONALGORITHM_H_

// Includes Estandar
#include <vector>
#include <string>

// Includes Suri
#include "ClassificationAlgorithmInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {
/** Clase encargada de realizar el calculo de la clasificacion mediante umbrales
 *  teniendo en cuenta los umbrales definidos por cada clase para cada banda.
 *  Para clasificar un pixel se realiza un AND entre todos los umbrales
 *  definido en una clase (si no cumple con la condicion el pixel pasa a la clase SINCLASE).
 */
class ThresholdClassificationAlgorithm : public ClassificationAlgorithmInterface {
   /** ctor copia **/
   ThresholdClassificationAlgorithm(const ThresholdClassificationAlgorithm&);

public:
   /** Estructura que almacena la definicion de umbrales para una clase **/
   struct BandThresholdInfo {
      int band_;
      double max_;
      double min_;
   };
   struct ClassThresholdInfo {
      int classIndex_;
      std::vector<BandThresholdInfo> bands_;
   };
   /** Funcion clasificacion */
   typedef bool (*ClassifyFunctionType)(
         int*, std::vector<void*>&, size_t, int, double, bool,
         const std::vector<ThresholdClassificationAlgorithm::ClassThresholdInfo> &Classes);
   /** ctor **/
   ThresholdClassificationAlgorithm();
   /** dtor **/
   virtual ~ThresholdClassificationAlgorithm();
   /**
    * Los metodos Update y GetNode: permiten persistir los parametros que
    * configuran al cada algoritmo en particular.
    * @param[in] pAlgorithmNode nodo XML con parametros del algoritmo
    * @return bool
    */
   /** Actualiza los datos internos del algoritmo con los del nodo. */
   virtual bool Update(const wxXmlNode* pAlgorithmNode);
   /** Genera nodo con los datos internos del algoritmo. */
   virtual wxXmlNode* GetNode();

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
   virtual bool Classify(int* pDest, std::vector<void*> pSource, size_t Size,
                         const std::string &DataType);

   /** Verifica que el algoritmo este bien configurado */
   virtual bool Check() const;

   /** Metodo que retorna un puntero al algoritmo de clasificacion */
   virtual ThresholdClassificationAlgorithm* Initialize(
         const std::string &AlgorithmName) {
      return this;
   }
   /** Indica la definicion de clases a utilizar por el algoritmo **/
   void SetClasses(std::vector<ClassThresholdInfo>& Classes) {
      classes_ = Classes;
   }

private:
   /*! puntero a funcion para la clasificacion */
   ClassifyFunctionType function_;

   /**
    * Metodo auxiliar que actualiza la informacion de clases en funcion del xml
    * @param[in] pClassNode nodo xml con la informacion de clases
    * @param[out] ClassInfo estructura en donde se almacena la configuracion de
    * clase que se encontraba en el xml
    * @return true en caso de parsear con exito el nodo
    * @return false en caso contrario
    */
   bool LoadClassInfoFromXml(const wxXmlNode* pClassNode,
                             ClassThresholdInfo& ClassInfo);
   /**
    * Metodo auxiliar que crea un nodo xml que representa la informacion
    * que contiene la clase que se pasa por parametro
    * @param[in] ClassInfo estructura que contiene la informacion de la clase
    * @return nodo xml con la informacion
    */
   wxXmlNode* CreateClassNode(const ClassThresholdInfo& ClassInfo) const;
   /**
    * Metodo auxiliar que genera un nodo xml con la informacion de los umbrales
    * de la banda que se pasa por parametro
    * @param[in] BandInfo estructura que contiene la informacion de las bandas
    * @return nodo xml con el contenido de la banda pasada por parametro
    */
   wxXmlNode* CreateBandNode(const BandThresholdInfo& BandInfo) const;

   std::vector<ClassThresholdInfo> classes_;
   /** indice para no clase **/
   int noClassIndex_;
};

} /** namespace suri */

#endif /* THRESHOLDCLASSIFICATIONALGORITHM_H_ */
