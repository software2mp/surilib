/*! \file ParametersSelectionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef PARAMETERSSELECTIONPART_H_
#define PARAMETERSSELECTIONPART_H_

// Includes standard
#include <string>
// Includes Suri
#include "suri/StatisticsFunctions.h"
#include "ClassificationAlgorithmInterface.h"
// Includes Wx
// Includes App
// Defines

/** Definido si ParametricClassificationPart tambien se usa para clasificacion no supervisada */
#define NOT_SUPERVISED_CODE

// forwards
#include "ClassificationPart.h"

/** namespace suri */
namespace suri {
/** Interfaz que deben implementar los parts que representen clasificaciones parametricas */
/**
 * Si se quiere agregar una clasificacion parametrica que requiera las
 * estadisticas de clases que se forman con poligonos en la imagen se debe
 * implementar esta interfaz y la de ClassificationAlgorithm.
 * ParametricClassificationPart primero llama a AddClass con las estadisticas
 * correspondientes a cada clase seleccionada por el usuario.
 * Luego obtiene el ClassificationAlgorithm configurado con datos de
 * de clasificacion.
 * Finalmente configura la tabla de colores del raster generado con
 * lo que devuelva GetClassProperties
 */
class ParametersSelectionPart : public Part {
   /** Macro que declara metodos de la clase */
HAS_FACTORY(ParametersSelectionPart, std::string);
public:
   /** Ctor */
   ParametersSelectionPart();
   /** Dtor */
   virtual ~ParametersSelectionPart();
   
   /** Retorna el nombre del algoritmo que se usara para clasificar */
   virtual std::string GetAlgorithmName()=0;
   /** Retorna un puntero al algoritmo de clasificacion creado por part */
   virtual ClassificationAlgorithmInterface* GetClassificationManager()=0;
};
}  // namespace suri

#endif /* PARAMETERSSELECTIONPART_H_ */
