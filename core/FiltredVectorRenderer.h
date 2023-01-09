/** \file FiltredVectorRenderer.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FILTREDVECTORRENDERER_H_
#define FILTREDVECTORRENDERER_H_

// Includes estandard
#include <string>

// Includes suri
#include "VectorRenderer.h"

namespace suri {

class Filter;

/**
 * Hereda de VectorRenderer.
 * Aplica un filtro a los features antes de renderizar.
 */
class FiltredVectorRenderer : public VectorRenderer {
public:
   /** Ctor */
   FiltredVectorRenderer();
   /** Dtor */
   virtual ~FiltredVectorRenderer();

   /** Retorna los filtros que se aplican sobre las capas del vector */
   static bool SetFilter(const Filter* pFilter, wxXmlNode *pNode);
   /** Retorna los filtros que se aplican sobre las capas del vector */
   static bool GetFilter(const wxXmlNode *pNode, int LayerIndex, Filter* &pFilter);

   /** creador + inicializador */
   virtual VectorRenderer *Create(Element *pElement, Renderer *pLastRenderer) const;
   /** Nombre del renderizador == al nombre del nodo */
   virtual std::string CreatedNode() const;

   /** Retorna vector a renderizar */
   virtual Vector* OpenVector();

   /** Filtros que se deben aplicar sobre el vector */
   Filter* pFilter_;
};

} /* namespace suri */
#endif /* FILTREDVECTORRENDERER_H_ */
