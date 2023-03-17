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
