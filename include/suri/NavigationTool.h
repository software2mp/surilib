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

#ifndef NAVIGATIONTOOL_H_
#define NAVIGATIONTOOL_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/GenericTool.h"

// Includes Wx

// Defines

// forwards

namespace suri {

// forwards
class GoButton;
class ViewportLinkManager;
class CoordinatesNavigationInputInterface;
class Viewer3D;

/*!
 * Ejecuta los comandos de navegacion
 */
class NavigationTool : public GenericTool {
public:
   /** Constructor */
   explicit NavigationTool(DataViewManager* pDataViewManager);
   /** Destructor */
   virtual ~NavigationTool();
   /** Ejecuta un comando */
   virtual void Execute(const Command* pToolCommand);

protected:
   /** Crea un comando */
   virtual Command* CreateCommand(const std::string &Command);
   /** Destruye un comando */
   virtual void DestroyCommand(Command* &pCommand) const;
   /** Crea un widget */
   virtual Widget* CreateWidget(const std::string &Widget);

private:
   /** Metodo que ejecuta el comando de navegacion **/
   void ExecuteNavigation(bool CommandActive);
   void ExecuteNavigation(ViewerWidget* pViewer, bool CommandActive);
   void ExecuteNavigation3D(Viewer3D* pViewer, bool CommandActive);
   /** Ejecuta el comando LayerExtent **/
   void ExecuteLayerExtent();
   /** Metodo que ejecuta el comando Zoom1on1 **/
   void ExecuteZoom1on1();
   /** Metodo que ejecuta comando SaveView **/
   void ExecuteSaveView();
   EventInterface* pZoom_;                /*! Puntero al zoom */
   EventInterface* pDrag_;                /*! Puntero al drag */
   EventInterface* pCrossHair_;                /*! Puntero al evento crosshair */
   GoButton* pGoButton_;                    /*! Puntero al boton */
   ViewportLinkManager* pViewportLink_;   /*! Puntero al vp link */
   /** Widget que posee los sistemas de referencia **/
   CoordinatesNavigationInputInterface* pCoordinateInputWidget_;
};

} /* namespace suri */

#endif /* NAVIGATIONTOOL_H_ */
