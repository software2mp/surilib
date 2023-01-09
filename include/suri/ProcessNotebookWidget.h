/** \file ProcessExporterWidget.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


#ifndef PROCESSEXPORTERWIDGET_H_
#define PROCESSEXPORTERWIDGET_H_

// Includes standard
#include <string>

// Includes suri
#include "suri/NotebookWidget.h"
#include "suri/ProcessPartProviderInterface.h"
#include "suri/messages.h"

/** namespace suri */
namespace suri {

// fordward
class ProcessInterface;
class LibraryManager;

/** Macro para declarar el objeto de los eventos */
DECLARE_EVENT_OBJECT(ProcessNotebookWidgetEvent);

/**
 * Clase que muestra parts de proceso y lo ejecuta al aceptar..
 */
class ProcessNotebookWidget : public NotebookWidget {
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(ProcessNotebookWidgetEvent);
   /** Ctor. de Copia. */
   ProcessNotebookWidget(const ProcessNotebookWidget &ProcessNotebookWidget);

public:
   /** Ctor. */
   ProcessNotebookWidget(ProcessInterface* pProcess,
                                  const std::string& ProcessName,
                                  LibraryManager* pLibraryManager);
   /** Dtor. */
   virtual ~ProcessNotebookWidget();

   /** Verifica los cambios y ejecuta Save */
   bool ApplyChanges();
   /** Agrega la parte de seleccion de lista de elementos */
   virtual bool CreateToolWindow();
   /** Evento temporal */
   void OnUIUpdate(wxUpdateUIEvent &Event);
   /** Evento que se dispara al cambiar de pagina en el book */
   void OnPageChanging(wxBookCtrlBaseEvent &Event);
   /** Evento que se dispara al cerrar un proceso */
   void OnClose(wxCloseEvent& Event);
   /** Evento que se dispara al presionar 'Cancelar' cerrar un proceso */
   void OnButtonCancel(wxCommandEvent& Event);
   /** Metodo que sirve para definir se se habilita o no el boton ok. */
   virtual bool EnableButtonOk();

private:
   /** Obtiene el part provider correspondiente al nombre del proceso**/
   ProcessPartProviderInterface* GetPartProvider(const std::string& ProcessName) const;

   ProcessInterface* pProcess_;  /*! Clase que ejecuta el proceso. */
   std::string processName_;  /*! Nombre del proceso que se esta configurando */
   LibraryManager* pLibraryManager_; /** Administrador de bibliotecas */
   /**
    * Parche: Evita que se actualicen los parts antes que se termine
    * de construir las ventanas.
    * Ocurre porque se conecta OnUIUpdate en clase padre antes de finalizar 
    * la construccion del part.
    */
   bool widgetCreated_;  /*! Flag que indica que se termino de construir el part */
   /*! Clase que conecta los eventos */
   EVENT_OBJECT_PROPERTY(ProcessNotebookWidgetEvent);
   /** Definicion de puntero a funcion para creadores**/
   typedef ProcessPartProviderInterface*(CreatorFunc)(const std::string&, ProcessAdaptLayer*);
   /** Obtiene los creadores de los partproviders */
   void GetPartProviderCreators(std::vector<CreatorFunc*>& Creator) const;
   /** Pregunta al usuario si desea cancelar el proceso actual */
   void VerifyExit(wxEvent& Event);
};

}  // namespace

#endif /* PROCESSEXPORTERWIDGET_H_ */
