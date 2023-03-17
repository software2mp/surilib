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

#ifndef CLASSIFICATIONALGORITHM_H_
#define CLASSIFICATIONALGORITHM_H_

#include <vector>
#include <string>

#include "NoDataValue.h"
#include "suri/FactoryHelpers.h"

#include "wx/xml/xml.h"

#define CLASSIFICATION_NDV    0

namespace suri {
/** Interfaz para todas las clases que implementen un algoritmo de clasificacion */
/**
 * Interfaz que utiliza ClassificationRenderer para asignar clases a cada pixel.
 * Debe existir una clase que implemente esta interfaz por cada algoritmo de
 * clasificacion.
 */
class ClassificationAlgorithmInterface : public NoDataValue {
   /** Utiliza la factoria para registrar la clase */
HAS_FACTORY(ClassificationAlgorithmInterface, std::string);

DECLARE_CREATE(ClassificationAlgorithmInterface, DeclareParameters_1
      (const std::string&) )=0; /*! todo: documentar */
public:
   /** Ctor */
   ClassificationAlgorithmInterface();

   /** Dtor */
   virtual ~ClassificationAlgorithmInterface();

   /**
    * Los metodos Update y GetNode: permiten persistir los parametros que
    * configuran al cada algoritmo en particular.
    * @param[in] pAlgorithmNode nodo XML con parametros del algoritmo
    * @return bool
    */

   /** Actualiza los datos internos del algoritmo con los del nodo. */
   virtual bool Update(const wxXmlNode* pAlgorithmNode)=0;

   /** Genera nodo con los datos internos del algoritmo. */
   virtual wxXmlNode* GetNode()=0;

   /**
    * Clasifica los datos de entrada(pSource) y guarda en pDest un entero con
    * el id de la clase ganadora.
    * @param[out] pDest destino de la clasificacion
    * @param[in] pSource fuente de datos para la clasificacion
    * @param[in] Size cantidad de datos
    * @param[in] DataType tipo de dato de la imagen a clasificar.
    * @return bool
    */
   virtual bool Classify(int* pDest, std::vector<void*> pSource, size_t Size,
                                              const std::string &DataType)=0;

   /** Verifica que el algoritmo este bien configurado */
   virtual bool Check() const=0;

   /**
    * Configura el valor que el algoritmo asigna a los pixels con valor no valido.
    */
   void SetNDVPixelValue(int NDVPixelValue);

   /**
    * Retorna el valor que el algoritmo asigna a los pixels valor no valido.
    */
   int GetNDVPixelValue() const;

private:
   int ndvPixelValue_;
};

}  // namespace suri

#endif /* CLASSIFICATIONALGORITHM_H_ */
