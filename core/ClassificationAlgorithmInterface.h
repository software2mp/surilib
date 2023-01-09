/*! \file ClassificationAlgorithmInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
