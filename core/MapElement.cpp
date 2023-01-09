/*! \file MapElement.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard

// Includes Suri
#include "MapElement.h"
#include "suri/xmlnames.h"

// Includes Wx

// Includes App
#include "MapRenderer.h"

// Defines

// Forwards

/** namespace suri */
namespace suri {

/**
 * Constructor
 */
MapElement::MapElement() :
      Element(wxT(TYPE_PROPERTY_VALUE_MAP)) {
}

/**
 * Destructor
 */
MapElement::~MapElement() {
}

/**
 * Funcion estatica para crear un MapElement
 * @return puntero a un elemento o null.
 * \attention quien invoque a este metodo es responsable del puntero devuelto,
 * por lo tanto debe hacerse cargo de su eliminacion.
 */
MapElement* MapElement::Create() {
   MapElement *pelement = new MapElement;
   if (pelement->Initialize("") != pelement) {
      return NULL;
   }
   return pelement;
}

MapElement* MapElement::Initialize(const std::string &FileName) {
   // Nodos de renderizacion - No se traduce, elemento de XML
   wxXmlNode *prnode = AddNode(GetNode(wxT("")), wxT(RENDERIZATION_NODE));
   AddNode(prnode, MapRenderer::GetXmlNode());
   // Nodo Archivo - No se traduce, elemento de XML
   SetFileNode(wxT(TYPE_PROPERTY_VALUE_MAP));
   return this;
}

} /** namespace suri */
