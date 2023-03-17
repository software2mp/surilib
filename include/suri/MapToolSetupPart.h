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

#ifndef __MAP_TOOL_SETUP_PART_H__
#define __MAP_TOOL_SETUP_PART_H__

#include "suri/Part.h"
#include "suri/DataViewManager.h"
namespace suri {

/** Clase encargada de administrar la configuracion general de la aplicacion **/
class MapToolSetupPart : public Part {
   /** constructor copia **/
   explicit MapToolSetupPart(const MapToolSetupPart&);

public:
   /** Ctor. */
   explicit MapToolSetupPart(DataViewManager* pDataViewManager = NULL);
   /** Destructor. */
   virtual ~MapToolSetupPart();
   /** Crea el part (el interior de la ventana)*/
   virtual bool CreateToolWindow();
   /** Asigna el icono para el part.*/
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Inicializa los controles*/
   virtual void SetInitialValues();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();

   virtual void Update() {
   }

private:
   DataViewManager* pDataViewManager_;
   wxString nativeFontInfo_;
   std::string titleText_;
   int titleTextStyle_;
   int titleTextSize_;
   int titleFamily_;
   std::string TitleColour_;
   std::string legendBorderColour_;
   std::string legendBackgroundColour_;
   std::string northPath_;
};

} /* namespace suri */

#endif   // __MAP_TOOL_SETUP_PART_H__
