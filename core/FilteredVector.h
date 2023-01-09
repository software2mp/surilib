/*! \file FilteredVector.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FILTEREDVECTOR_H_
#define FILTEREDVECTOR_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/Vector.h"
#include "FilterInterpreterInterface.h"
#include "Filter.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Clase que representa un vector sobre el cual se aplica un filtro
 * (funciona a modo de adapter). Contiene una capa sobre la cual se aplica
 * un filtro sql
 */
class FilteredVector : public suri::Vector, public suri::FilterInterpreterInterface {
   /** ctor copia **/
   FilteredVector(const FilteredVector&);

public:
   /** ctor */
   FilteredVector(Vector* pVector, Filter* pFilter);
   /** dtor */
   virtual ~FilteredVector();
   /** Acceso a la capa por indice
   * @param LayerIndex
   * @return capa filtrada
   * @return NULL en caso de que el filtro no coincida con ningun registro*/
   virtual OGRLayer *GetLayer(int LayerIndex) const;
   /** Devuelve el indice a la capa por nombre (-1 si falla) */
   virtual int GetLayerIndex(const std::string &LayerName) const;
   /** Configura el filtro a aplicar */
   void SetFilter(Filter* pFilter);
   /**  Interpreta el filtro pasado por parametro.  */
   virtual bool Interpret(Filter* pFilter);
   /** Devuelve el resultado de la interpretacion del filtro por un Vector*/
   virtual Vector* GetInterpretationAsVector();
   /** Devuelve el resultado de la interpretacion del filtro por un Raster*/
   virtual Image* GetInterpretationAsRaster();
   /** Devuelve el resultado de la interpretacion del filtro representado por una Tabla*/
   virtual Table* GetInterpretationAsTable();

private:
   /** Metodo auxiliar que actualiza la capa filtrada en funcion del filtro*/
   void UpdateFilteredLayer();
   /** Vector sobre  el cual aplicara el filtro **/
   Vector* pVector_;
   /** Filtro a aplicar */
   Filter* pFilter_;
   /** Capa resultante del aplicado del filtro al vector **/
   OGRLayer* pFilteredLayer_;
};

} /** namespace suri */

#endif /* FILTEREDVECTOR_H_ */
