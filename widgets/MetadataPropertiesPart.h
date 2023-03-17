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

#ifndef METADATAPROPERTIESPART_H_
#define METADATAPROPERTIESPART_H_

#include "suri/Part.h"
#include "suri/SuriObject.h"
#include "suri/RasterElement.h"

#include "wx/bitmap.h"

namespace suri {

DECLARE_EVENT_OBJECT(MetadataPropertiesPartEvent);

class RasterDatasource;

class MetadataPropertiesPart : public suri::Part {
public:
   FRIEND_EVENT_OBJECT(MetadataPropertiesPartEvent);

   /**
    * Ctor.
    */
   MetadataPropertiesPart(RasterDatasource* pDatasource);

   /**
    * Dtor.
    */
   virtual ~MetadataPropertiesPart();

   /**
    * Indica si el Part tiene cambios para salvar.
    */
   virtual bool HasChanged();

   /**
    * Salva los cambios realizados en el Part.
    */
   virtual bool CommitChanges();

   /**
    * Restaura los valores originales del Part.
    */
   virtual bool RollbackChanges();

   /**
    * Inicializa el part.
    */
   virtual void SetInitialValues();

   /**
    * Actualiza el estado de la parte.
    */
   virtual void Update();

   /**
    * Crea la ventana de la parte.
    */
   virtual bool CreateToolWindow();

   /**
    * Retorna el icono de la herramienta.
    */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;

   /**
    * Handler para el evento de click del boton guardar.
    */
   void OnSaveButtonClicked(wxCommandEvent &Event);

private:
   EVENT_OBJECT_PROPERTY(MetadataPropertiesPartEvent);

   RasterDatasource* pDatasource_;

   /**
    * Inicializa la lista.
    */
   void InitializeList();
};

}  // namespace suri

#endif  // METADATAPROPERTIESPART_H_
