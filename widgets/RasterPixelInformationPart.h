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

#ifndef RASTERPIXELINFORMATIONPART_H_
#define RASTERPIXELINFORMATIONPART_H_

#include "suri/Part.h"
#include "suri/Coordinates.h"

namespace suri {

class ParameterCollection;
class RasterDatasource;
class DataViewManager;
class World;

DECLARE_EVENT_OBJECT(RasterPixelInformationEvent);

/**
 * Muestra informacion del elemento raster, particularmente sobre el
 * pixel seleccionado.
 */
class RasterPixelInformationPart : public suri::Part {
public:
   FRIEND_EVENT_OBJECT(RasterPixelInformationEvent);

   /** Ctor */
   explicit RasterPixelInformationPart(ParameterCollection& Params);

   /** Dtor */
   virtual ~RasterPixelInformationPart();

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

   /** Implementacion del metodo de creacion de ventana de Widget */
   virtual bool CreateToolWindow();

   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;

private:
   static const std::string kRasterPixelInformationXrcName;
   static const std::string kRasterPixelInformationTitle;

   EVENT_OBJECT_PROPERTY(RasterPixelInformationEvent); /*! Objeto para conectar eventos */

   Coordinates click_;
   Coordinates coords_;
   RasterDatasource* pRasterDs_;
   World* pViewerWorld_;

   /**
    * Obtiene un dataviewmanager para el table part.
    */
   DataViewManager* GetDataView();

   /** Lanzado cuando cambia el tamanio de la ventana */
   void OnResize(wxSizeEvent &Event);
};

}  // namespace suri

#endif  // RASTERPIXELINFORMATIONPART_H_
