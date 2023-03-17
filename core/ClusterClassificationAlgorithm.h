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
