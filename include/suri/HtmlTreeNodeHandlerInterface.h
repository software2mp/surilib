/** \file HtmlTreeNodeInterface.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef HTMLTREENODEINTERFACE_H_
#define HTMLTREENODEINTERFACE_H_

// Includes standard
#include <string>

// Includes suri
#include "suri/NodePath.h"
#include "suri/Widget.h"

namespace suri {

/**
 * Interfaz que permite definir un tipo de nodo particular para el arbol.
 */
class HtmlTreeNodeHandlerInterface {
public:
   /** Dtor */
   virtual ~HtmlTreeNodeHandlerInterface() {}

   /** Retorna el html de un nodo */
   virtual std::string GetHtml(const NodePath &Node, int Width) = 0;

   /** Genera menu que se debera mostrar sobre el arbol. */
   virtual void CreateContextMenu(const NodePath &Node,
                              Widget* pWidget, int X, int Y) = 0;
   /** Se llama cuando el usuario hace doble click sobre un nodo. */
   virtual void ExecuteDoubleClick(const NodePath &Node) = 0;
   /** Se llama cuando el usuario hace doble click sobre un nodo. */
   virtual void ExecuteHtmlLink(const NodePath &Node,
                              const std::string &LinkId) = 0;
};

} /* namespace suri */

#endif /* HTMLTREENODEINTERFACE_H_ */
