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

#ifndef VECTORTABLESTRUCTUREEDITIONPART_H_
#define VECTORTABLESTRUCTUREEDITIONPART_H_

// Includes standard

// Includes Suri
#include "suri/Part.h"
#include "VectorDatasource.h"

// Includes Wx
#include "wx/xrc/xmlres.h"
#include "wx/grid.h"

// Includes App

// Defines

namespace suri {

// Forwards
class Table;
DECLARE_EVENT_OBJECT(ComboSelectionEvent);
class VectorLayer;

/**
 * Part para seleccion de interpretacion de campo como hipervinculo.
 */
class VectorTableStructureEditionPart : public suri::Part {
public:
   /** Ctor */
   VectorTableStructureEditionPart(VectorDatasource* pVectorDatasource,
                                   VectorLayer* pLayer);
   /** Dtor */
   virtual ~VectorTableStructureEditionPart();
   /** Implementacion del metodo de creacion de ventana de Widget */
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
   /**
    * Devuelve el icono de la parte
    * @param[out] ToolBitmap icono de la ventana
    * @param[in] X ancho de icono
    * @param[in] Y alto de icono
    */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;

private:
   FRIEND_EVENT_OBJECT(ComboSelectionEvent);
   EVENT_OBJECT_PROPERTY(ComboSelectionEvent);
   /** Metodo que captura el evento de cambio del choice **/
   void OnComboSelectionEventHandler(wxCommandEvent &Event);
   /*! Nombre de xrc de Part */
   static const std::string VECTOR_TABLE_STRUCTURE_EDITION_XRC_NAME;
   /** fuente de datos vectorial en edicion **/
   VectorDatasource* pVectorDatasource_;
   Table* pTable_;
   /** Capa activa que se encuentra en edicion **/
   VectorLayer* pLayer_;
};
}  /** namespace suri **/

#endif /* VECTORTABLESTRUCTUREEDITIONPART_H_ */
