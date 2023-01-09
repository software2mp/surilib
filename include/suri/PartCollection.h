/*! \file PartCollection.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef PARTCOLLECTION_H_
#define PARTCOLLECTION_H_

// Includes standard
#include <list>

// Includes Suri
#include "suri/Part.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Contiene una agregacion de Parts(a nivel estructura de datos ). */
/**
 *  Las clases que hereden deberan definir la organizacion grafica y propagar
 * los eventos entre ellos.
 * Implementa metodos de Part propagando la llamada a los subparts.
 */
class PartCollection : public Part {
public:
   /** Ctor */
   PartCollection();
   /** Ctor */
   PartCollection(Widget* pWidget);
   /** Ctor */
   PartCollection(const std::string &XrcId, const std::string &PartName = "");
   /** Dtor */
   virtual ~PartCollection();
   /** Agrega un part a la coleccion */
   void AddPart(Part* pNewPart, const std::string &ParentWidget);
   /** Remueve un part de la coleccion */
   void RemovePart(Part* pPart);
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();

   /** Agrega a pWidget_ los widgets de todos los parts en partColection_. */
   virtual bool ConfigureWidget();

protected:
   std::list<std::pair<Part*, std::string> > partCollection_; /*! mapa con los parts */
   /* contenidos y el nombre del panel donde se agregaron. */
};
}  // namespace suri

#endif /* PARTCOLLECTION_H_ */
