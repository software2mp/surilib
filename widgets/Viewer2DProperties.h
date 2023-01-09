/*! \file Viewer2DProperties.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VIEWER2DPROPERTIES_H_
#define VIEWER2DPROPERTIES_H_

// Includes standard

// Includes Suri
#include "suri/PartCollection.h"
#include "suri/ViewerWidget.h"

// Includes Wx

// Includes App

// Defines

namespace suri {

// fordwards
class LibraryManager;
class UniversalGraphicalComponentPart;
class LibraryItem;

DECLARE_EVENT_OBJECT(Viewer2DPropertiesEvent);

/** Part con las propiedades de un visualizador 2D. */
/**
 * Muestra nombre y tipo de visualizador. Permite modificar el modo de visualizacion (Raster o Gis)
 * Permite modificar el sistema de referencia del visualizador
 */
class Viewer2DProperties : public PartCollection {
public:
   /** Constructor */
   Viewer2DProperties(ViewerWidget *pViewer, LibraryManager* pManager);
   /** Destructor */
   virtual ~Viewer2DProperties();
   /** Indica si el Part tiene cambios para salvar */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part */
   virtual bool RollbackChanges();
   /** Modifica el nombre del viewer */
   virtual bool ConfigureWidget();

private:
   /** Evento que se ejecuta al seleccionar un item en el choice cambiando el tooltip asociado */
   void OnViewerModeChange(wxCommandEvent &Event);
   /** Metodo auxiliar para cambiar a modo raster */
   void ChangeToRasterMode();
   /** Metodo auxiliar para cambiar a modo gis */
   void ChangeToGisMode();
   FRIEND_EVENT_OBJECT(Viewer2DPropertiesEvent);
   EVENT_OBJECT_PROPERTY(Viewer2DPropertiesEvent);
   ViewerWidget* pViewer_;
   LibraryManager* pManager_;
   UniversalGraphicalComponentPart* pSrsSelectionPart_;
   int mode_;
   const LibraryItem* pSelectedItem_;
};
}

#endif /* VIEWER2DPROPERTIES_H_ */
