/** \file GeoreferenceRefSysPart.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GEOREFERENCEREFSYSPART_H_
#define GEOREFERENCEREFSYSPART_H_

#include "suri/Widget.h"
#include "LibraryItemEditorPart.h"

namespace suri {

class TablePart;

/**
 *
 */
class GeoreferenceRefSysPart : public suri::LibraryItemEditorPart {
public:
   /**
    * Ctor.
    */
   GeoreferenceRefSysPart(const SuriObject::UuidType& LibraryId,
                          LibraryManager* pManager, TablePart* pTablePart);

   /**
    * Dtor.
    */
   ~GeoreferenceRefSysPart();

   /**
    * (METODO NO SOPORTADO)
    */
   std::vector<LibraryItemAttribute*>* CreateAttributesFromFields();

   /**
    * Establece el SRS del part.
    */
   void SetSrs(const std::string& Srs);

protected:
   /**
    * Metodo de configuracion particular para el part.
    */
   virtual bool ConfigureFeatures();

   /**
    * Actualiza la informacion en el part.
    */
   virtual void UpdateFields(const LibraryItem* pItem, bool Modifiable = true);

   /** Tabla a la que hay que notificar cuando cambio el srs */
   TablePart *pTablePart_;
};

} // namespace suri

#endif  // GEOREFERENCEREFSYSPART_H_
