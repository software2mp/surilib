/*! \file KMeansAlgorithm.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef KMEANALGORITHM_H_
#define KMEANALGORITHM_H_

// Includes standard
// Includes Suri
#include "ClassificationAlgorithmInterface.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
/** Clase que implementa el algoritmo de maxima verosimilitud para la clasificacion */
/**
 * Si se configura el ClassificationRenderer con una instancia de esta clase
 * se clasificara la imagen usando el metodo de maxima verosimilitud.
 */
class KMeansAlgorithm : public ClassificationAlgorithmInterface {
   /** Ctor. de Copia. */
   KMeansAlgorithm(const KMeansAlgorithm &MaxLikAlgorithm);

public:

   /** Estructura de datos para las clases del algoritmo de clasificacion */
   struct KMeansAlgorithmClassData {
      /** Ctor */
      KMeansAlgorithmClassData(int Id) :
            id_(Id) {
      }
      int id_; /*! id de la clase */
      std::vector<double> mean_; /*! vector de medias */
   };

   /** Funcion clasificacion */
   typedef bool (*KMeansFunctionType)(
         int*, std::vector<void*>&, size_t, int, double, bool,
         const std::vector<KMeansAlgorithm::KMeansAlgorithmClassData> &Classes);

   /** Ctor */
   KMeansAlgorithm();
   /** Dtor */
   virtual ~KMeansAlgorithm();
   /** Actualiza los datos del algoritmo con los del nodo. */
   virtual bool Update(const wxXmlNode* pAlgorithmNode);
   /** Genera nodo con los datos del algoritmo. */
   virtual wxXmlNode* GetNode();
   /** Determina el tipo de dato a la entrada del algoritmo */
   virtual bool Classify(int* pDest, std::vector<void*> pSource, size_t Size,
                                                  const std::string &DataType);
   /** verifica que las dimensiones de los parametros sean consistentes */
   virtual bool Check() const;

// Parametros de clasificacion
   std::vector<KMeansAlgorithmClassData> classes_; /*! clases para la */
   /* clasificacion */
protected:
   KMeansFunctionType function_; /*! funcion para la clasificacion */
protected:
   /** Metodo que devuelve un puntero a this */
   virtual KMeansAlgorithm* Initialize(const std::string &FileName) {
      return this;
   }
private:
};
}

#endif /* KMEANALGORITHM_H_ */
