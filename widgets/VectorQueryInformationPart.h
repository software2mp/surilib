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

#ifndef VECTORQUERYINFORMATIONPART_H_
#define VECTORQUERYINFORMATIONPART_H_

// Includes standard
#include <string>

// Includes Suri
#include "suri/Part.h"
#include "suri/DatasourceInterface.h"

// Includes Wx

// Includes App

// Defines


namespace suri {

class DataViewManager;

class VectorQueryInformationPart : public Part {
public:
   /** Ctor */
   explicit VectorQueryInformationPart(DatasourceInterface *pDatasource);

   /** Dtor */
   virtual ~VectorQueryInformationPart();

   /** crea la ventana del Part */
   virtual bool CreateToolWindow();
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
   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;

private:
   DatasourceInterface *pDatasource_;  /**! Fuente de datos a mostrar */
   Part *pLayerTablePart_; /**! parte que contiene la tabla de la consulta */

   /**
    * Obtiene un dataviewmanager para el table part.
    */
   DataViewManager* GetDataView();

   /**
    * Obtiene el nombre del vector en memoria.
    */
   std::string GetMemoryUrl(const std::string& FileNameP);
};
}  // namespace suri
#endif /* VECTORQUERYINFORMATIONPART_H_ */
