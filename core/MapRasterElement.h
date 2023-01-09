/*! \file MapRasterElement.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MAPRASTERELEMENT_H_
#define MAPRASTERELEMENT_H_

// Includes standard

// Includes Suri
#include "suri/RasterElement.h"
#include "resources.h"

// Includes Wx

// Includes App

// Defines

// Forwards

/** namspace suri */
namespace suri {
// ----------------------- CLASE DE ELEMENTO TIPO MAPA ------------------------
/** clase derivada de RasterElement con el icono para MAPAS */
/**
 * Clase para el contexto "Mapas", que cambia el icono de Imagen por el de Mapa.
 */
class MapRasterElement : public RasterElement {
   /** Ctor. de Copia. */
   MapRasterElement(const MapRasterElement &MapRasterElement);

public:
   /** Ctor. */
   MapRasterElement() {
   }
   /** Destructor. */
   virtual ~MapRasterElement() {
   }
   /** Funcion estatica para crear un RasterElement */
   static MapRasterElement* Create(const std::string &FileName,
                                   const Option& Metadata = Option());
   /** retorna un string para el icono */
   virtual wxString DoGetIcon() const {
      return wxT(icon_MAP_ELEMENT);
   }
private:
   /** Metodo que devuelve un puntero a this */
   virtual MapRasterElement* InitializeClass(const std::string &FileName) {
      return this;
   }
};
}  // namespace suri

#endif /*MAPRASTERELEMENT_H_*/
