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

#ifndef NAVIGATIONTOOLCOMMANDEXECUTIONHANDLER_H_
#define NAVIGATIONTOOLCOMMANDEXECUTIONHANDLER_H_

// Includes Estandar
// Includes Suri
#include "CommandExecutionHandlerInterface.h"

// Includes Wx
// Defines

namespace suri {

// forwards
class DataViewManager;
class ViewportLinkManager;
class CoordinatesNavigationInputInterface;
class Navigator;
class GoButton;

namespace core {

/**
 * Clase encargada de administrar la ejecucion de los comandos asociados al ViewportLink
 */
class ViewportLinkCommandExecutionHandler : public CommandExecutionHandlerInterface {
   /** Constructor Copia */
   ViewportLinkCommandExecutionHandler(const ViewportLinkCommandExecutionHandler&);
public:
   /** Constructor */
   explicit ViewportLinkCommandExecutionHandler(DataViewManager* pDataView);
   /** Destructor */
   virtual ~ViewportLinkCommandExecutionHandler();
   /**
    * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
    * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    */
   virtual bool Execute(const Command* pCommand, GenericTool* pTool);

private:
   DataViewManager* pDataView_;
   /** Puntero a ViewportLinkManager */
   ViewportLinkManager* pViewportLink_;
};

/**
 * Clase encargada de administrar la ejecucion de los comandos asociados al Navigation
 */
class NavigationCommandExecutionHandler : public CommandExecutionHandlerInterface {
   /** Constructor Copia */
   NavigationCommandExecutionHandler(const NavigationCommandExecutionHandler&);
public:
   /** Constructor */
   NavigationCommandExecutionHandler(
         DataViewManager* pDataView, GenericTool* pTool, Navigator *pNavigator);
   /** Destructor */
   virtual ~NavigationCommandExecutionHandler();
   /**
    * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
    * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    */
   virtual bool Execute(const Command* pCommand, GenericTool* pTool);

private:
   /** Puntero a DataViewManager */
   DataViewManager* pDataView_;
   /** Puntero a GenericTool */
   GenericTool* pTool_;
   /** Puntero al navegador */
   Navigator *pNavigator_;
   /** Widget que posee los sistemas de referencia **/
   CoordinatesNavigationInputInterface* pCoordinateInputWidget_;
   /** Puntero a GoButton */
   GoButton* pGoButton_;
};

/**
 * Clase encargada de administrar la ejecucion de los comandos asociados al FullExtent
 */
class FullExtentCommandExecutionHandler : public CommandExecutionHandlerInterface {
   /** Constructor Copia */
   FullExtentCommandExecutionHandler(const FullExtentCommandExecutionHandler&);
public:
   /** Constructor */
   explicit FullExtentCommandExecutionHandler(DataViewManager* pDataView);
   /** Destructor */
   virtual ~FullExtentCommandExecutionHandler();
   /**
    * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
    * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    */
   virtual bool Execute(const Command* pCommand, GenericTool* pTool);

private:
   /** Puntero a DataViewManager */
   DataViewManager* pDataView_;
};

/**
 * Clase encargada de administrar la ejecucion de los comandos asociados al LayerExtent
 */
class LayerExtentCommandExecutionHandler : public CommandExecutionHandlerInterface {
   /** Constructor Copia */
   LayerExtentCommandExecutionHandler(const LayerExtentCommandExecutionHandler&);
public:
   /** Constructor */
   explicit LayerExtentCommandExecutionHandler(DataViewManager* pDataView);
   /** Destructor */
   virtual ~LayerExtentCommandExecutionHandler();
   /**
    * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
    * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    */
   virtual bool Execute(const Command* pCommand, GenericTool* pTool);

private:
   /** Puntero a DataViewManager */
   DataViewManager* pDataView_;
};

/**
 * Clase encargada de administrar la ejecucion de los comandos asociados al Zoom1on1
 */
class Zoom1on1CommandExecutionHandler : public CommandExecutionHandlerInterface {
   /** Constructor Copia */
   Zoom1on1CommandExecutionHandler(const Zoom1on1CommandExecutionHandler&);
public:
   /** Constructor */
   explicit Zoom1on1CommandExecutionHandler(DataViewManager* pDataView);
   /** Destructor */
   virtual ~Zoom1on1CommandExecutionHandler();
   /**
    * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
    * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    */
   virtual bool Execute(const Command* pCommand, GenericTool* pTool);

private:
   /** Puntero a DataViewManager */
   DataViewManager* pDataView_;
};

/**
 * Clase encargada de administrar la ejecucion de los comandos asociados al SaveView
 */
class SaveViewCommandExecutionHandler : public CommandExecutionHandlerInterface {
   /** Constructor Copia */
   SaveViewCommandExecutionHandler(const SaveViewCommandExecutionHandler&);
public:
   /** Constructor */
   explicit SaveViewCommandExecutionHandler(DataViewManager* pDataView);
   /** Destructor */
   virtual ~SaveViewCommandExecutionHandler();
   /**
    * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
    * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    */
   virtual bool Execute(const Command* pCommand, GenericTool* pTool);

private:
   /** Puntero a DataViewManager */
   DataViewManager* pDataView_;
};

/**
 * Clase encargada de administrar la ejecucion de los comandos asociados al ZoomIn
 */
class ZoomInCommandExecutionHandler : public CommandExecutionHandlerInterface {
   /** Constructor Copia */
   ZoomInCommandExecutionHandler(const ZoomInCommandExecutionHandler&);
public:
   /** Constructor */
   explicit ZoomInCommandExecutionHandler(DataViewManager* pDataView);
   /** Destructor */
   virtual ~ZoomInCommandExecutionHandler();
   /**
    * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
    * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    */
   virtual bool Execute(const Command* pCommand, GenericTool* pTool);

private:
   /** Puntero a DataViewManager */
   DataViewManager* pDataView_;
};

/**
 * Clase encargada de administrar la ejecucion de los comandos asociados al ZoomOut
 */
class ZoomOutCommandExecutionHandler : public CommandExecutionHandlerInterface {
   /** Constructor Copia */
   ZoomOutCommandExecutionHandler(const ZoomOutCommandExecutionHandler&);
public:
   /** Constructor */
   explicit ZoomOutCommandExecutionHandler(DataViewManager* pDataView);
   /** Destructor */
   virtual ~ZoomOutCommandExecutionHandler();
   /**
    * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
    * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    */
   virtual bool Execute(const Command* pCommand, GenericTool* pTool);

private:
   /** Puntero a DataViewManager */
   DataViewManager* pDataView_;
};

/**
 * Clase encargada de administrar la ejecucion de los comandos asociados a la camara
 */
class CameraCommandExecutionHandler : public CommandExecutionHandlerInterface {
   /** Constructor Copia */
   CameraCommandExecutionHandler(const CameraCommandExecutionHandler&);
public:
   /** Constructor */
   explicit CameraCommandExecutionHandler(DataViewManager* pDataView);
   /** Destructor */
   virtual ~CameraCommandExecutionHandler();
   /**
    * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
    * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    */
   virtual bool Execute(const Command* pCommand, GenericTool* pTool);

private:
   /** Puntero a DataViewManager */
   DataViewManager* pDataView_;
};

} /** namespace core */
} /** namespace suri */

#endif /* NAVIGATIONTOOLCOMMANDEXECUTIONHANDLER_H_ */
