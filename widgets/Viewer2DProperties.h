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
