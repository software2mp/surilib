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

#ifndef CATEGORIZEDVECTORSTYLEPART_H_
#define CATEGORIZEDVECTORSTYLEPART_H_


// Includes Estandar
// Includes Suri
#include "suri/Part.h"
#include "TablePart.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class Table;
class DataViewManager;
class VectorLayer;

namespace ui {

// forwards
class CategorizedVectorStylePartHandler;
class VectorStyleClassGenerationWidget;

class CategorizedVectorStylePart : public Part {
public:
   CategorizedVectorStylePart(DataViewManager* pDataViewManager,
                              VectorLayer *pVectorLayer);
   virtual ~CategorizedVectorStylePart();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Implementacion del metodo de creacion de ventana de Widget */
   virtual bool CreateToolWindow();
   /** Devuelve si la parte tiene datos validos */
   virtual bool HasValidData() {
      return pTablePart_ ? pTablePart_->HasValidData() : true;
   }

protected:
   friend class CategorizedVectorStylePartHandler;
   /**
    *  Metodo que se llama cuando se genera el evento de click sobre el boton
    *  de remover clase
    */
   void OnRemoveClassClick(wxCommandEvent& Event);
   /**
    *  Metodo que se llama cuando se genera el evento de click sobre el boton
    *  de agregar clase
    */
   void OnAddClassClick(wxCommandEvent& Event);
   /**
    * Metodo que se llama cuando se genera el evento de click sobre el boton
    * de generar clases
    */
   void OnClassGenerationClick(wxCommandEvent& Event);
   /* Metodo auxiliar para la configuracion de la tabla **/
   void ConfigureClassTable();
   /** Metodo auxiliar que genera el nodo xml para la renderizacion vectorial
    *  clasificada a partir de la configuracion realizada por el usuario
    */
   wxXmlNode* GetCategorizedRenderNodeFromTable();
   TablePart* pTablePart_;
   Table* pTable_;
   DataViewManager* pDataViewManager_;
   VectorLayer *pVectorLayer_;
   CategorizedVectorStylePartHandler* pEventHandler_;
   VectorStyleClassGenerationWidget* pClassGenerationWidget_;
};

} /** namespace ui **/
} /** namespace suri */

#endif /* CATEGORIZEDVECTORSTYLEPART_H_ */
