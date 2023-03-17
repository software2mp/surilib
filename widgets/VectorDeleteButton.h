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

#ifndef VECTORDELETEBUTTON_H_
#define VECTORDELETEBUTTON_H_

// Includes Estandar
// Includes Suri
#include "GeometryDeleteEvent.h"
#include "VectorDatasourceSelectionSource.h"
#include "VectorEditorButton.h"

// Includes Wx
// Defines
// forwards
class VectorElementEditor;

namespace suri {

class ViewerWidget;
class VectorEditionTaskEvent;

/**
 * Clase que se encarga de borrar una geometria
 */

class VectorDeleteButton : public VectorEditorButton {
public:
   /** Constructor */
   VectorDeleteButton(wxWindow *pToolbar, ViewerWidget *pViewer, const wxString &Bitmap,
                      GuiGeometryCreator::GeometryType GeoType, wxString ButtonTooltip,
                      DataViewManager* pDataViewManager_, int Flags);
   /** Destructor */
   virtual ~VectorDeleteButton();

   /** Inicio de eliminacion  de geometria */
   void Start();
   /** Finalizacion de eliminacion de geometria */
   void End();
   /** Indica si se encuentra activo el editor **/
   virtual bool IsActive() const {
      return pDeleteEventHandler_ != NULL || pPaintEventHandler_ != NULL;
   }
   /**  Metodo que se llama cuando finaliza la edicion vectorial.*/
   virtual void OnVectorEditionFinished() {
      if (IsActive())
         DoSetValue(false);
   }

private:
   /** Setea el estado del boton. */
   virtual bool DoSetValue(bool State);
   ViewportManagerInterface *pViewportManager_;
   ViewcontextManagerInterface *pViewcontextManager_;
   FeatureSelection* pFeatureSelection_;
   GeometryDeleteEvent* pDeleteEventHandler_;
   VectorEditionTaskEvent* pPaintEventHandler_;
   VectorDatasourceSelectionSource* pSelectionSource_;
   DataViewManager* pDataViewManager_;
   VectorEditionTask* pTask_;
   Table* pTable_;
};
} // suri namespace



#endif /* VECTORDELETEBUTTON_H_ */

