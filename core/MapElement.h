/*! \file MapElement.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MAPELEMENT_H_
#define MAPELEMENT_H_

// -------------------------------- MAP ELEMENT --------------------------------
// Includes standard

// Includes Suri
#include "suri/Element.h"
#include "suri/Part.h"

// Includes Wx

// Includes App

// Defines

// Forwards

/** namespace suri */
namespace suri {
/** Clase que representa un elemento Overlay Mapa */
/**
 * Clase que representa el Overlay de Mapa creado por la herramienta de Mapa.
 *
 * En el nodo "renderizacion|mapa" que guarda los parametros (leyenda, grilla,
 * norte, escala) que luego seran renderizados por el MapRenderer.
 * El nodo mencionado es actualizado por la clase MapTool.
 * \note los parametros guardados en el nodo son booleanos, solo indican si
 * dichos parametros fueron activados.
 * \note Tener en cuenta que el nodo "grilla" este no es utilizado ya que
 * la grilla se trata como un caso aparte.
 */
class MapElement : public Element {
   /** Ctor. de Copia. */
   MapElement(const MapElement &MapElement);

public:
   /** Constructor */
   MapElement();
   /** Destructor */
   virtual ~MapElement();
   /** retorna el nombre del elemento */
   virtual wxString GetName() const {
      return _(label_MAP_LAYERS);
   }
   /** retorna los detalles del elemento */
   virtual wxString GetDetails() const {
      return wxT("");
   }
   /** Muestra la herramienta de propiedades */
   virtual void ShowPropertyTool() {
   }
   ;
   /** Funcion estatica para crear un MapElement */
   static MapElement* Create();

protected:
private:
   /** Inicializa elemento con archivo en ruta indicada */
   MapElement* Initialize(const std::string &FileName);
};
}  // namespace suri

#endif /*MAPELEMENT_H_*/
