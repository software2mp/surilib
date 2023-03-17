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

#ifndef RASTERIZATIONPART_H_
#define RASTERIZATIONPART_H_

// Includes standard
#include <string>
#include <vector>

// Includes Suri
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/Element.h"
// Includes Wx
// Includes wx
#include "wx/listctrl.h"
// Includes App

/** namespace suri */
namespace suri {
/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(RasterizationPartEvent);

class RasterizationPart : public Part, public ProcessAtributeProvider {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(RasterizationPartEvent);
   /** Ctor. */
   RasterizationPart(std::vector<Element*> Selected);
   /** Dtror. */
   virtual ~RasterizationPart();
   /** Crea la ventana de la parte */
   bool CreateToolWindow();
   /** Devuelve el estado del Part. */
   bool IsEnabled();
   /** Indica si el Part tiene cambios para salvar. */
   bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   bool CommitChanges();
   /** Restaura los valores originales del Part. */
   bool RollbackChanges();
   /** Carga los valores iniciales */
   void SetInitialValues();
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Evento UIUpdate */
   void OnUiUpdate(wxUpdateUIEvent &Event);
   /** Verifica que tenga datos validos */
   bool HasValidData();
   /** Actualiza el estado de la parte */
   virtual void Update() {
   }
protected:
private:
   void ConfigureDataTypeChoice();
   std::vector<std::string> dataTypeNames_;
   std::string attributeSelection_;
   std::string typeSelection_;
   wxChoice* pDNChoice_;
   wxChoice* pDataTypeChoice_;
   EVENT_OBJECT_PROPERTY(RasterizationPartEvent); /*! Objeto para conectar */
};
}  // namespace suri
#endif /* RASTERIZATIONPART_H_ */
