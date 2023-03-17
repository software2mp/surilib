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

#ifndef SINGLEVIEWERSELECTIONPART_H_
#define SINGLEVIEWERSELECTIONPART_H_

#include <vector>
#include <string>
#include <map>

#include "suri/Part.h"
#include "suri/SuriObject.h"

namespace suri {

DECLARE_EVENT_OBJECT(SingleViewerSelectionPartEvent);

/**
 * Esta clase genera un part que permite la seleccion de un viewer.
 */
class SingleViewerSelectionPart : public Part {
public:
   FRIEND_EVENT_OBJECT(SingleViewerSelectionPartEvent);

   /**
    * Ctor
    */
   SingleViewerSelectionPart();

   /**
    * Dtor
    */
   virtual ~SingleViewerSelectionPart();

   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();

   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();

   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();

   /** Inicializa el part */
   virtual void SetInitialValues();

   /** Actualiza el estado del Part */
   virtual void Update();

   /** Modifica el nombre del viewer */
   virtual bool ConfigureWidget();

   /** Configura los visualizadores disponibles. */
   void SetCurrentViewers(const std::map<suri::SuriObject::UuidType, std::string>& CurrentViewers);

   /** Handler para el evento de cambio de seleccion de choice. */
   void OnChViewerChanged(wxCommandEvent &Event);

   /** Devuelve el visualizador seleccionado */
   SuriObject::UuidType GetSelectedViewer();

private:
   EVENT_OBJECT_PROPERTY(SingleViewerSelectionPartEvent);

   static const std::string SINGLE_VIEWER_SELECTION_XRC_NAME; /* < Id Panel */
   static const int kInitialPos = 0;
   static const std::string kViewer;
   SuriObject::UuidType selectedViewerId_;

   std::map<suri::SuriObject::UuidType, std::string> currentViewers_;

   /** Inicializa el part realizando todas las configuraciones necesarias. */
   void InitializePart();

   /** Inicializa el combo de seleccion de visualizadores */
   void InitializeChoice();

};
}  // namespace suri

#endif /* SINGLEVIEWERSELECTIONPART_H_ */
