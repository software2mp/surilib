/*! \file Viewer3DProperties.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VIEWER3DPROPERTIES_H_
#define VIEWER3DPROPERTIES_H_

// Includes standard

// Includes Suri
#include "suri/PartCollection.h"
#include "suri/ViewerWidget.h"

// Includes Wx

// Defines

namespace suri {

// forwards
class DataViewManager;
class RasterElement;

DECLARE_EVENT_OBJECT(Viewer3DPropertiesEvent);

/** Part con las propiedades de un visualizador 3D. */
/**
 * Muestra nombre y tipo de visualizador. Permite modificar el color de fondo
 * del visualizador.
 */
class Viewer3DProperties : public PartCollection {
public:
   /** Ctor */
   Viewer3DProperties(ViewerWidget *pViewer, DataViewManager* pDataViewManager);
   /** Dtor */
   virtual ~Viewer3DProperties();

   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valor's originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   virtual bool ConfigureWidget();

private:
   /** Evento de seleccion de DEM */
   void OnDemChange(wxCommandEvent &Event);
   /** Evento de movimiento de sliders */
   void OnSliderScroll(wxScrollEvent &Event);
   void SetParameters();
   FRIEND_EVENT_OBJECT(Viewer3DPropertiesEvent);
   EVENT_OBJECT_PROPERTY(Viewer3DPropertiesEvent);
   ViewerWidget *pViewer_;
   DataViewManager* pDataViewManager_;
   std::vector<Element*> dems_;
};
}

#endif /* VIEWER3DPROPERTIES_H_ */
