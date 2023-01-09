/*! \file FilterInterpreterInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FILTERINTERPRETERINTERFACE_H_
#define FILTERINTERPRETERINTERFACE_H_

// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class Filter;
class Image;
class Vector;
class Table;

/*!
 *
 */
class FilterInterpreterInterface {
public:
   FilterInterpreterInterface();
   virtual ~FilterInterpreterInterface();
   /**  Interpreta el filtro pasado por parametro.  */
   virtual bool Interpret(Filter* pFilter)=0;
   /** Devuelve el resultado de la interpretacion del filtro por un Vector*/
   virtual Vector* GetInterpretationAsVector() = 0;
   /** Devuelve el resultado de la interpretacion del filtro por un Raster*/
   virtual Image* GetInterpretationAsRaster() = 0;
   /** Devuelve el resultado de la interpretacion del filtro representado por una Tabla*/
   virtual Table* GetInterpretationAsTable() = 0;
};

} /** namespace suri */

#endif /* FILTERINTERPRETERINTERFACE_H_ */
