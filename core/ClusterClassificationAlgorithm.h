/** \file ClusterClassificationAlgorithm.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CLUSTERCLASSIFICATIONALGORITHM_H_
#define CLUSTERCLASSIFICATIONALGORITHM_H_

#include <string>
#include <vector>

#include "suri/StatisticsFunctions.h"
#include "ClassificationAlgorithmInterface.h"
#include "Clusters.h"

namespace suri {

/**
 * Clase con parametros comunes a los algoritmos de clasificacion que usan
 * las estadisticas de clusters para determinar el valor del pixel de salida.
 */
class ClusterClassificationAlgorithm : public ClassificationAlgorithmInterface {
   /** Ctor. de Copia. */
   ClusterClassificationAlgorithm(const ClusterClassificationAlgorithm
                                             &ClusterClassificationAlgorithm);

public:
   /** Ctor */
   explicit ClusterClassificationAlgorithm(const std::string &AlgorithmName);

   /** Dtor */
   virtual ~ClusterClassificationAlgorithm();

   /** Actualiza los datos del algoritmo con los del nodo. */
   virtual bool Update(const wxXmlNode* pAlgorithmNode);

   /** Genera nodo con los datos del algoritmo. */
   virtual wxXmlNode* GetNode();

   /** Configura el threshold del algoritmo */
   void SetThreshold(double Threshold);

   /** Retorna el threshold configurado */
   double GetThreshold() const;

   /** Configura el valor que el algoritmo asigna a los pixels no clasificados */
   void SetNoClassPixelValue(int NoClassPixelValue);

   /** Retorna el valor que el algoritmo asigna a los pixels no clasificador */
   int GetNoClassPixelValue() const;

   /** Configura los clusters que se usan para clasificar */
   void SetClusters(Clusters* pClusters);

   /** Retorna la clase que administra los clusters que se usan para clasificar */
   Clusters* GetClusters() const;

private:
   double threshold_;  /*! Umbral para asignar un pixel a una clase */
   int noClassPixelValue_;  /*! Valor que reciben los pixels sin clase */
   Clusters* pClusters_;  /*! Clusters generados con las areas de entrenamiento, */
                          /** que se usan para clasificar */
   std::string algorithmName_; /*! Nombre de la clase, se usa en factoria */
                               /** para crear el algoritmo correcto */
};

}  // namespace suri

#endif /* CLUSTERCLASSIFICATIONALGORITHM_H_ */
