/*! \file ThresholdClassificationAlgorithm.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
