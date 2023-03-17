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

#ifndef CLASSEDITIONPART_H_
#define CLASSEDITIONPART_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "suri/Part.h"
#include "RasterDatasource.h"
#include "wxGenericTableBase.h"
#include "ClassInformation.h"
#include "ColorTable.h"
#include "suri/LayerInterface.h"
#include "TablePart.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Part que permite editar las propiedades de las clases de un raster
 * clasificada
 */
class ClassEditionPart : public suri::Part {
public:
   /** Constructor */
   ClassEditionPart(RasterDatasource* pRasterDatasource, LayerInterface* pLayer);
   /** Destructor */
   virtual ~ClassEditionPart();
   /** Indica si el Part tiene cambios para salvar */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part */
   virtual bool RollbackChanges();
   /** Inicializa los valores iniciales de la grilla del part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Crea la Tool Window */
   virtual bool CreateToolWindow();
   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;

private:
   /** Metodo auxiliar para configurar la tabla de colores de la fuente de datos **/
   void ConfigureDatasourceColorTable(const ColorTable& ColorTable);
   /** Nombre de la primera columna */
   static const std::string FirstColumn;
   /** Nombre de la segunda columna */
   static const std::string SecondColumn;
   /** Puntero a la fuente de datos raster */
   RasterDatasource* pRasterDatasource_;
   /** Puntero a la capa activa a actualizar la tabla de colores **/
   LayerInterface* pLayer_;
   /** Tabla que se usa para asignar las bandas */
   Table* pVariablesTable_;
   /** Part que contiene la tabla **/
   TablePart* pTablePart_;
   /** Asociacion fila - indice */
   std::vector<ClassInformation::IndexType> indexes_;
};

} /** namespace suri */

#endif /* CLASSEDITIONPART_H_ */
