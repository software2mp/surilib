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

#ifndef INDEXSELECTIONPART_H_
#define INDEXSELECTIONPART_H_

// Includes Standard
#include <vector>

// Includes suri
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/RasterElement.h"
// todo provisorio. Falta mover BandMathRenderer->suri?
#include "BandMathRenderer.h"
#include "suri/LibraryItemSelectionInterface.h"

// forwards
class wxChoice;
class wxPanel;

/** namespace suri */
namespace suri {

// forwards
class BasicLibraryItemSelector;
class LibraryManager;
class BandSelectorWidget;

/** Clase para crear la parte de seleccion de indice y bandas */
/**
 * Esta clase es un part que sirve para seleccionar un indice (para aplicar
 * sobre un elemento) y las bandas requeridas.
 */

class IndexSelectionPart : public Part, public ProcessAtributeProvider,
                           public LibraryItemSelectionInterface {
public:
   /** Constructor */
   explicit IndexSelectionPart(const RasterElement * pElement, LibraryManager* pManager);
   /** Destructor */
   virtual ~IndexSelectionPart();
   /** Crea la ventana de la parte */
   bool CreateToolWindow();
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Devuelve el estado del Part. */
   bool IsEnabled();
   /** Indica si la parte tiene datos validos */
   bool HasValidData();
   /** Indica si el Part tiene cambios para salvar. */
   bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   bool CommitChanges();
   /** Restaura los valores originales del Part. */
   bool RollbackChanges();
   /** Por defecto la lista de bandas esta vacia */
   void SetInitialValues();
   /** Retorna una ecuacion configurada con los parametros seleccionados por el usario */
   bool GetSelectedEquation(BandMathRenderer::Parameters::Equation* &pEquation) const;
   /** Actualiza el estado de la parte */
   virtual void Update() {
   }
   /**
    * Configura el editor con el item que se pasa por parametro. Se pasa por parametro
    * un boolean que indica si se trata de la primera seleccion (para que el part no piense
    * que posee cambios cuando en realidad es la seleccion inicial del CGU)
    */
   virtual void SetActiveItem(const LibraryItem* pItem);

private:
   std::vector< std::pair<std::string, std::string> > GetVariableDescription(const LibraryItem* pItem) const;
   wxArrayString bandNames_; /*! Nombres de las bandas */
   int selectedIndex_; /*! Pagina seleccionada(= a indice seleccionado) */
   std::vector<int> selectedBands_; /*! Bandas seleccionadas */
   wxString errorMessage_; /*! mensaje de error */
   /*! Contiene informacion de las ecuaciones que usa cada indice */
   std::vector<BandMathRenderer::Parameters::Equation*> indexDescriptions_;
   BasicLibraryItemSelector* pItemSelector_;
   BandSelectorWidget* pBandSelector_;
};
}  // namespace suri
#endif /* INDEXSELECTIONPART_H_ */
