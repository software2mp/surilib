/*! \file MapRasterElement.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "MapRasterElement.h"

// Includes standard

// Includes Suri

// Includes Wx

// Includes App

// Defines

// Forwards

/** namespace suri */
namespace suri {
/** Macro para registrar el tipo de elemento */
AUTO_REGISTER_CLASS(Element, MapRasterElement, 0)

/**
 * Funcion estatica para crear un MapRasterElement
 * @param[in] FileName path del archivo para crear el elemento
 * @param[in] Metadata mapa de metadatos
 * \attention quien invoque a este metodo es responsable del puntero devuelto,
 * por lo tanto debe hacerse cargo de su eliminacion.
 */
MapRasterElement* MapRasterElement::Create(const std::string &FileName,
                                           const Option& Metadata) {
   RasterElement *pelement = RasterElement::Create(FileName, Metadata);
   if (!pelement) {
      return NULL;
   }

   MapRasterElement *pmapelem = new MapRasterElement;
   wxXmlNode *pnode = NULL;
   pmapelem->AddNode(pnode, pelement->GetProperties());
   delete pelement;
   return pmapelem;
}
}  // namspace suri
