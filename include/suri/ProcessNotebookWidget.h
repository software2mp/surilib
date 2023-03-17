/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
