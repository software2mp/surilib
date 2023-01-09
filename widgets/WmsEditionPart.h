/*! \file WmsEditionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WMSEDITIONPART_H_
#define WMSEDITIONPART_H_

// Includes Estandar
// Includes Suri
#include "LibraryItemEditorPart.h"
// Includes Wx
// Defines
// forwards

namespace suri {

class WmsEditionPart : public suri::LibraryItemEditorPart {
   /** constructor copia **/
   WmsEditionPart(const WmsEditionPart&);
public:
   /** constructor **/
   WmsEditionPart(const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
                  UniversalGraphicalComponentPartInterface::FeatureCombinationEnum Feature);
   /** Destructor */
   virtual ~WmsEditionPart();
private:
   /**
    * Metodo auxiliar privado que es necesario implementar en las clases hijas.
    * Este metodo es el encargado de actualizar realmente la informacion en el part
    * Es llamado en el metodo SetActiveItem a modo de Template Method
    **/
   virtual void UpdateFields(const LibraryItem* pItem, bool Modifiable = true);
   /**  Metodo que obtiene a partir de los datos del editor, un vector con los atributos
    *   Este metodo es el que hace el comportamiento particular de cada editor a la hora
    *   de obtener la informacion que posee y transmitirla al sistema de bibliotecas.
    *   Es utilizado a modo de Template Method (en CommitChanges y en el notificador)
    *   @return vector con atributos
    *   @return NULL en caso de que la informnacion del part no sea valida
    *   **/
   virtual std::vector<LibraryItemAttribute*>* CreateAttributesFromFields();
   bool HasChanged();
   /** xrc de panel de editor de density slice **/
   static const std::string WMS_EDITION_XRC_NAME;
   /** id del panel donde se ubica el cgu **/
   static const std::string WMS_CGU_PANEL;
   std::string urlvalue_;
};

} /** namespace suri */

#endif /* WMSEDITIONPART_H_ */
