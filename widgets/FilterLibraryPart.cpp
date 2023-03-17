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

#include <vector>

#include "FilterLibraryPart.h"

#include "BandMathRenderer.h"
#include "EquationParser.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/BasicLibraryItemSelector.h"
#include "suri/ConvolutionFilterItemAttribute.h"
#include "suri/LibraryItemAttributeFactory.h"
#include "suri/LibraryManagerFactory.h"

// Includes Wx
#include "wx/choicebk.h"
// Defines
// forwards

#include "resources.h"

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(FilterLibraryPartEvent, FilterLibraryPart)
      IMPLEMENT_EVENT_CALLBACK(OnUiUpdate, OnUiUpdate(event), wxUpdateUIEvent)
END_IMPLEMENT_EVENT_OBJECT

/**
 * Ctor.
 */
FilterLibraryPart::FilterLibraryPart(LibraryManager* pManager, int):
   FilterPart(pManager),
   NEW_EVENT_OBJECT(FilterLibraryPartEvent) {
   windowTitle_ = _(caption_FILTER_SELECTION);
}

/**
 * Dtor.
 */
FilterLibraryPart::~FilterLibraryPart() {
   DELETE_EVENT_OBJECT;
}

/**
 * Restaura los valores originales del Part.
 */
bool FilterLibraryPart::RollbackChanges() {
   return true;
}

/**
 * Por defecto la lista de bandas esta vacia.
 */
void FilterLibraryPart::SetInitialValues() {
   this->selected_ = false;
}

/**
 * Crea la ventana de la parte.
 */
bool FilterLibraryPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_FILTER_LIBRARY_SELECTION_PART_EX"));

   LibraryClient client(pManager_, LibraryManager::READONLY);

   const Library* plib = client.GetLibraryByCode(
         LibraryManagerFactory::ConvolutionFiltersCode);

   pItemSelector_ = new BasicLibraryItemSelector(
         this, plib->GetId(), pManager_, "ID_LIBRARY_SELECTION_PANEL",
         "ID_LIBRARY_SELECTION_PANEL", false,
         UniversalGraphicalComponentPartInterface::RoWithFFAndNoLabel);
   AddControl(pItemSelector_, "ID_FLSPEX_SELECTOR_PANEL");

   //Se actualiza para setear como seleccionado el item 0
   pToolWindow_->Connect(wxEVT_UPDATE_UI,
                         wxUpdateUIEventHandler(FilterLibraryPartEvent::OnUiUpdate),
                         NULL, pEventHandler_);
   return true;
}

/**
 * Obtiene el icono de la parte.
 *
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho del icono
 * @param[in] Y alto del icono
 */
void FilterLibraryPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_FILTER_SELECTION, ToolBitmap);
}

/**
 * Analiza si se modifico el tipo de filtro o el tamanio del mismo desde el ultimo commit.
 *
 * @param[in] Event evento UIUpdate
 */
void FilterLibraryPart::OnUiUpdate(wxUpdateUIEvent &Event) {
   modified_ = pItemSelector_->HasChanged();
   /**
    * TODO: emprolijar
    * Esto es para cargar el primer filtro existente en la biblioteca.
    */
   if(!selected_) {
      CommitChanges();
      selected_ = true;
   }
   Event.Skip();
}

/**
 * Obtiene el tipo de filtro que se va a aplicar.
 */
FilterProcess::FilterType FilterLibraryPart::GetFilterType() {
   return FilterProcess::LibraryFilter;
}

/**
 * @return lista que contiene las matrices del filtro seleccinonado
 */
std::list<std::vector<std::vector<double> > > FilterLibraryPart::GetFilterMatrix(
      FilterProcess::FilterType, int) {
   std::list<std::vector<std::vector<double> > > filterlist;
   int matrixsize = -1;
   int matrixcount = -1;

   if (pSelectedFilter_) {
      matrixsize = StringToNumber<int>(
            pSelectedFilter_->GetAttribute("size")->GetValue());
      matrixcount = StringToNumber<int>(
            pSelectedFilter_->GetAttribute("dim")->GetValue());
   }

   if (matrixsize < 0 || matrixcount < 0) return filterlist;

   LibraryItem::MapAttribute attrs = pSelectedFilter_->GetAttributes();
   LibraryItem::MapAttribute::const_iterator it = attrs.begin();
   for (int m = 0; m < matrixcount && it != attrs.end(); ++it) {
      // obtengo la matriz asociada al filtro
      if (LibraryItemAttributeFactory::IsConvolutionFilterItemAttribute(it->second)) {
         std::vector<std::vector<double> > matrix(matrixsize,
                                                  std::vector<double>(matrixsize));
         // obtengo la matriz asociada al atributo
         ConvolutionFilterItemAttribute* pfilterattr =
               LibraryItemAttributeFactory::CreateConvolutionFilterItemAttribute(
                     it->second, matrixsize);
         for (int x = 0; x < matrixsize; ++x) {
            for (int y = 0; y < matrixsize; ++y) {
               int nominalval = pfilterattr->GetNominalValue();
               double matrixval = pfilterattr->GetConvolutionFilterMatrixValues(x, y);

               if (nominalval == 0) nominalval = 1.0;

               matrix[x][y] = matrixval / nominalval;
            }
         }
         filterlist.push_back(matrix);
         ++m;
      }
   }
   return filterlist;
}

/**
 * Obtiene el nodo que representa a la ecuacion seleccionada.
 */
wxXmlNode * FilterLibraryPart::GetBandsEquationNode(int BandCount) {
   BandMathRenderer::Parameters parameters;
   BandMathRenderer::Parameters::Equation equation;
   const LibraryItemAttribute* peqattr = pSelectedFilter_? pSelectedFilter_->GetAttribute("eq") : NULL;
   if (!peqattr)
      return NULL;
   equation.equation_ = peqattr->GetValue();
   // TODO(Gabriel - TCK #7142): Configurar las variables en funcion de bandas seleccionadas
   EquationParser eqparser;
   if (eqparser.SetEquation(equation.equation_))
      return NULL;
   std::vector<std::string> variables;
   eqparser.GetVariableNames(variables);
   for (size_t i = 0; i < variables.size(); ++i)
      equation.bandNames_.insert(std::pair<std::string, int>(variables[i], i));
   equation.bandNames_.insert(std::pair<std::string, int>(wxT("b2"), 1));
   parameters.equations_.push_back(equation);
   return BandMathRenderer::GetXmlNode(parameters);
}

/** Devuelve LibraryFilter */
FilterProcess::FilterType FilterLibraryPart::GetSelectedFilterType() {
   return FilterProcess::LibraryFilter;
}


/**
 * Implementa interfaz.
 */
void FilterLibraryPart::SetSelectedFilterType(FilterProcess::FilterType Type) {}

/**
 * NO SE IMPLEMENTA.
 */
void FilterLibraryPart::GetSelectedKernelSize(FilterProcess::FilterType Type, int &Rows,
                                              int &Columns) {
}

/**
 * NO SE IMPLEMENTA.
 */
void FilterLibraryPart::SetSelectedKernelSize(FilterProcess::FilterType Type, int Rows,
                                              int Columns) {
}

}  // namespace suri
