/*! \file WxsLayerConfigurationPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXSLAYERCONFIGURATIONPART_H_
#define WXSLAYERCONFIGURATIONPART_H_

// Includes standard
#include <string>

// Includes Suri
#include "suri/PartCollection.h"
#include "suri/LibraryItemSelectionInterface.h"
#include "BasicLibraryItemSelector.h"
#include "suri/DataViewManager.h"
// Includes Wx

// Includes App

// Defines

namespace suri {

class WxsDataSourcePart;
class WxsProperties;
class WxsVisualizationPart;

/** Permite agregar una capa wxs al proyecto */
/**
 * Para agregar fuentes wxs se pueden usar las que vienen precargadas o ingresar
 * una nueva a mano.
 */
class WxsLayerConfigurationPart : public PartCollection, public LibraryItemSelectionInterface {
   /** Ctor */
public:
   WxsLayerConfigurationPart(std::string* pWxsElementFactoryString,
                             DataViewManager* pDataViewManager);
   /** Dtor */
   virtual ~WxsLayerConfigurationPart();

   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Actualiza el TextCtrl del part con el valor de la biblioteca */
   void SetActiveItem(const LibraryItem* pItem);

protected:
   /** Actualiza el string que configura el elemento wms con los datos en parts */
   void UpdateFactoryString();

   std::string* pWxsElementFactoryString_;  ///< Clase donde se configuran los
                                            ///< datos ingresados por el usuario
   WxsDataSourcePart* pWxsDataSourcePart_;        ///< Part donde se carga la fuente wms
   WxsProperties* pWxsProperties_;  ///< Part donde se cargan las
                                    ///< propiedades wms
   WxsVisualizationPart* pWxsVisualizationPart_;  ///< Part donde se cargan las
   ///< propiedades de visualizacion
   BasicLibraryItemSelector* pItemSelector_; ///Puntero al selector de items de la biblioteca
};
}

#endif /* WXSLAYERCONFIGURATIONPART_H_ */
