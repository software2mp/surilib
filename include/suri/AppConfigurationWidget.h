/*! \file AppConfigurationWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef APPCONFIGURATIONWIDGET_H_
#define APPCONFIGURATIONWIDGET_H_

#include "suri/NotebookWidget.h"
#include "AppConfigPartsProviderInterface.h"

/** namespace suri */
namespace suri {

// forwards
class DataViewManager;

DECLARE_EVENT_OBJECT(AppConfigurationWidgetEvent);

/**
 * Clase encargada de administrar los parts de configuracion general de la aplicacion.
 * Hereda de NotebookWidget para aprovechar el comportamiento de Wizard que tiene el mismo. 
**/
class AppConfigurationWidget : public NotebookWidget {   
   /* Constructor copia. */
   AppConfigurationWidget(const AppConfigurationWidget &Appconfigurationwidget);

public:
   /* Constructor. */
   AppConfigurationWidget(AppConfigPartsProviderInterface* pappconfigparts,
         wxString Titulo, DataViewManager* pDataviewmanager);
   /* Destructor */
   virtual ~AppConfigurationWidget();
   /** Commit de los cambios efectuados. */
   virtual bool ApplyChanges();
   /** Inserta los parts necesarios para la configuracion de la aplicacion */
   virtual bool InsertParts();
   /** Evento click sobre Boton Ok */
   virtual void OnButtonOk(wxCommandEvent &Event);

private:
   /** Proveedor de parts de configuracion **/
   AppConfigPartsProviderInterface* pAppconfigparts_;
   /** Administrador general de la aplicacion **/
   DataViewManager* pDataviewmanager_;
   EVENT_OBJECT_PROPERTY(AppConfigurationWidgetEvent); /*! Objeto para conectar */
};

} // End namespace.

#endif /* APPCONFIGURATIONWIDGET_H_ */
