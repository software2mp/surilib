/*! \file LibraryItemSelectionInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LIBRARYITEMSELECTIONINTERFACE_H_
#define LIBRARYITEMSELECTIONINTERFACE_H_

// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {
class LibraryItem;

/* Interfaz necesaria para generalizar la creacion de LibraryItemEditorPart de la cual
 * deberan heredar las clases en las que se quiera incluir una biblioteca */

class LibraryItemSelectionInterface {
public:
   /** Constructor */
   LibraryItemSelectionInterface() {};
   /** Destructor */
   virtual ~LibraryItemSelectionInterface() {};
   /** Configura el editor con el item que se pasa por parametro. Se pasa por parametro
    * un boolean que indica si se trata de la primera seleccion (para que el part no piense
    * que posee cambios cuando en realidad es la seleccion inicial del CGU)*/
   virtual void SetActiveItem(const LibraryItem* pItem)=0;
};

} /** namespace suri */

#endif /* LibraryItemSelectionInterface_H_ */
