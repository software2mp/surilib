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

#ifndef NAVIGATIONTOOLCOMMANDCREATOR_H_
#define NAVIGATIONTOOLCOMMANDCREATOR_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/CommandCreatorInterface.h"

namespace suri {

class DataViewManager;
class GenericTool;
class EventInterface;

namespace core {

/**
 * Clase que encargada de configurar el Command para la herramienta ViewportLink
 */
class ViewportLinkCommandCreator : public CommandCreatorInterface {
   /** Constructor Copia */
   ViewportLinkCommandCreator(const ViewportLinkCommandCreator&);
public:
   /** Constructor */
   ViewportLinkCommandCreator();
   /** Destructor */
   virtual ~ViewportLinkCommandCreator();

private:
   /**
    * Creacion de un comando a partir de su nombre
    * (siempre y cuando el nombre del comando sea del dominio de la clase).
    */
   virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/**
 * Clase que encargada de configurar el Command para la herramienta Navigation
 */
class NavigationCommandCreator : public CommandCreatorInterface {
   /** Constructor Copia */
   NavigationCommandCreator(const NavigationCommandCreator&);
public:
   /** Constructor */
   NavigationCommandCreator();
   /** Destructor */
   virtual ~NavigationCommandCreator();

private:
   /**
    * Creacion de un comando a partir de su nombre
    * (siempre y cuando el nombre del comando sea del dominio de la clase).
    */
   virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/**
 * Clase que encargada de configurar el Command para la herramienta FullExtent
 */
class FullExtentCommandCreator : public CommandCreatorInterface {
   /** Constructor Copia */
   FullExtentCommandCreator(const FullExtentCommandCreator&);
public:
   /** Constructor */
   FullExtentCommandCreator();
   /** Destructor */
   virtual ~FullExtentCommandCreator();

private:
   /**
    * Creacion de un comando a partir de su nombre
    * (siempre y cuando el nombre del comando sea del dominio de la clase).
    */
   virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/**
 * Clase que encargada de configurar el Command para la herramienta LayerExtent
 */
class LayerExtentCommandCreator : public CommandCreatorInterface {
   /** Constructor Copia */
   LayerExtentCommandCreator(const LayerExtentCommandCreator&);
public:
   /** Constructor */
   LayerExtentCommandCreator();
   /** Destructor */
   virtual ~LayerExtentCommandCreator();

private:
   /**
    * Creacion de un comando a partir de su nombre
    * (siempre y cuando el nombre del comando sea del dominio de la clase).
    */
   virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/**
 * Clase que encargada de configurar el Command para la herramienta Zoom1on1
 */
class Zoom1on1CommandCreator : public CommandCreatorInterface {
   /** Constructor Copia */
   Zoom1on1CommandCreator(const Zoom1on1CommandCreator&);
public:
   /** Constructor */
   Zoom1on1CommandCreator();
   /** Destructor */
   virtual ~Zoom1on1CommandCreator();

private:
   /**
    * Creacion de un comando a partir de su nombre
    * (siempre y cuando el nombre del comando sea del dominio de la clase).
    */
   virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/**
 * Clase que encargada de configurar el Command para la herramienta SaveView
 */
class SaveViewCommandCreator : public CommandCreatorInterface {
   /** Constructor Copia */
   SaveViewCommandCreator(const SaveViewCommandCreator&);
public:
   /** Constructor */
   SaveViewCommandCreator();
   /** Destructor */
   virtual ~SaveViewCommandCreator();

private:
   /**
    * Creacion de un comando a partir de su nombre
    * (siempre y cuando el nombre del comando sea del dominio de la clase).
    */
   virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/**
 * Clase que encargada de configurar el Command para la herramienta Drag
 */
class DragCommandCreator : public CommandCreatorInterface {
   /** Constructor Copia */
   DragCommandCreator(const DragCommandCreator&);
public:
   /** Constructor */
   DragCommandCreator(DataViewManager* pDataViewManager, GenericTool* pTool);
   /** Destructor */
   virtual ~DragCommandCreator();

private:
   /**
    * Creacion de un comando a partir de su nombre
    * (siempre y cuando el nombre del comando sea del dominio de la clase).
    */
   virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
   /** Puntero a DataViewManager */
   DataViewManager* pDataView_;
   /** Puntero a GenericTool */
   GenericTool* pTool_;
   /** Puntero a evento Drag */
   EventInterface* pDrag_;
};

/**
 * Clase que encargada de configurar el Command para la herramienta MultiZoom
 */
class MultiZoomCommandCreator : public CommandCreatorInterface {
   /** Constructor Copia */
   MultiZoomCommandCreator(const MultiZoomCommandCreator&);
public:
   /** Constructor */
   MultiZoomCommandCreator(DataViewManager* pDataViewManager, GenericTool* pTool);
   /** Destructor */
   virtual ~MultiZoomCommandCreator();

private:
   /**
    * Creacion de un comando a partir de su nombre
    * (siempre y cuando el nombre del comando sea del dominio de la clase).
    */
   virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
   /** Puntero a DataViewManager */
   DataViewManager* pDataView_;
   /** Puntero a GenericTool */
   GenericTool* pTool_;
   /** Puntero a evento MultiZoom */
   EventInterface* pZoom_;
};

/**
 * Clase que encargada de configurar el Command para la herramienta ZoomIn
 */
class ZoomInCommandCreator : public CommandCreatorInterface {
   /** Constructor Copia */
   ZoomInCommandCreator(const ZoomInCommandCreator&);
public:
   /** Constructor */
   ZoomInCommandCreator();
   /** Destructor */
   virtual ~ZoomInCommandCreator();

private:
   /**
    * Creacion de un comando a partir de su nombre
    * (siempre y cuando el nombre del comando sea del dominio de la clase).
    */
   virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/**
 * Clase que encargada de configurar el Command para la herramienta ZoomOut
 */
class ZoomOutCommandCreator : public CommandCreatorInterface {
   /** Constructor Copia */
   ZoomOutCommandCreator(const ZoomOutCommandCreator&);
public:
   /** Constructor */
   ZoomOutCommandCreator();
   /** Destructor */
   virtual ~ZoomOutCommandCreator();

private:
   /**
    * Creacion de un comando a partir de su nombre
    * (siempre y cuando el nombre del comando sea del dominio de la clase).
    */
   virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/**
 * Clase que encargada de configurar el Command para la herramienta SphericalCamera
 */
class SphericalCameraCommandCreator : public CommandCreatorInterface {
   /** Constructor Copia */
   SphericalCameraCommandCreator(const SphericalCameraCommandCreator&);
public:
   /** Constructor */
   SphericalCameraCommandCreator();
   /** Destructor */
   virtual ~SphericalCameraCommandCreator();

private:
   /**
    * Creacion de un comando a partir de su nombre
    * (siempre y cuando el nombre del comando sea del dominio de la clase).
    */
   virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/**
 * Clase que encargada de configurar el Command para la herramienta EyeCamera
 */
class EyeCameraCommandCreator : public CommandCreatorInterface {
   /** Constructor Copia */
   EyeCameraCommandCreator(const EyeCameraCommandCreator&);
public:
   /** Constructor */
   EyeCameraCommandCreator();
   /** Destructor */
   virtual ~EyeCameraCommandCreator();

private:
   /**
    * Creacion de un comando a partir de su nombre
    * (siempre y cuando el nombre del comando sea del dominio de la clase).
    */
   virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};



} /** namespace core */
} /** namespace suri */

#endif /* NAVIGATIONTOOLCOMMANDCREATOR_H_ */
