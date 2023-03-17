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

// Includes Standard
#include <vector>

// Includes suri
#include "IndexSelectionPart.h"
#include "suri/messages.h"
#include "suri/RasterElement.h"
#include "suri/IndexProcess.h"
#include "suri/EquationVariableMappingPart.h"
#include "suri/LibraryClient.h"
#include "suri/LibraryManagerFactory.h"
#include "UniversalGraphicalComponentPartInterface.h"
#include "suri/BasicLibraryItemSelector.h"
#include "suri/ComplexItemAttribute.h"
#include "suri/AuxiliaryFunctions.h"


// Includes wx
#include "wx/xrc/xmlres.h"
#include "wx/wx.h"
#include "wx/choicebk.h"
#include "wx/choice.h"

// Includes App
#include "resources.h"

// Includes
#include <sstream>

// Defines
/** Cantidad de paginas en el choicebook de seleccion de indices */
#define INDEX_PAGES_COUNT 3
/** Numero negativo que indica que no se asigno una banda */
#define NO_BAND_ASSIGNED -1

#define EQ_LIBRARY_PAGE 3

/** inicializa los recursos de la ventana */
void InitIndexSelectionXmlResource();

/** namespace suri */
namespace suri {

class BandSelectorWidget : public Part {

public:
   /** ctor **/
   BandSelectorWidget(Widget* pParent, std::vector< std::pair<std::string,std::string> >& VariableDescription,
                     wxArrayString& BandNames) : pParent_(pParent),
                     varDescription_(VariableDescription),
                     bandNames_(BandNames) {
   }
   virtual bool CreateToolWindow() {
      pToolWindow_ = new wxPanel(pParent_->GetWindow(), wxID_ANY);
      pToolWindow_->SetSizer(new wxFlexGridSizer(2));
      SetInitialValues();
      return true;
   }

   virtual void SetInitialValues() {
      for (size_t i = 0; i < varDescription_.size(); ++i) {
            pToolWindow_->GetSizer()->Add(new wxStaticText(pToolWindow_, wxID_ANY,
                                          varDescription_[i].second));
            wxChoice* pchoice = new wxChoice(pToolWindow_, wxID_ANY, wxDefaultPosition,
                                 wxDefaultSize, wxArrayString(bandNames_));
            bandsChoice_.push_back(std::make_pair<wxChoice*, int>(pchoice, 0));
            pToolWindow_->GetSizer()->Add(pchoice);
      } 

   }

   std::vector<int> GetVariableBands() {
      std::vector<int> variables;
      for (size_t choice = 0; choice < bandsChoice_.size(); ++choice) {
         wxChoice* pchoice = bandsChoice_[choice].first;
         variables.push_back(pchoice->GetSelection());
      }
      return variables;
   }

   virtual void Update() {
   }   

   virtual bool HasChanged() {
      bool change = false;
      for (size_t choice = 0; !change && choice < bandsChoice_.size(); ++choice) {
         wxChoice* pchoice = bandsChoice_[choice].first;
         change = pchoice->GetSelection() != bandsChoice_[choice].second;

      }
      return change;
   }
   virtual bool CommitChanges() {
      for (size_t choice = 0; choice < bandsChoice_.size(); ++choice) {
          wxChoice* pchoice = bandsChoice_[choice].first;
          bandsChoice_[choice].second = pchoice->GetSelection();
      }
      return true;
   }

   virtual bool RollbackChanges() {
      for (size_t choice = 0; choice < bandsChoice_.size(); ++choice) {
         wxChoice* pchoice = bandsChoice_[choice].first;
         pchoice->SetSelection(bandsChoice_[choice].second);
      }
      return true;
   }

private:
   Widget* pParent_;
   std::vector< std::pair<std::string, std::string> > varDescription_;
   wxArrayString& bandNames_;
   std::vector<std::pair<wxChoice*, int> > bandsChoice_;
};

/**
 * Constructor
 * Obtiene las bandas del elemento y las guarda en la propiedad interna.
 * @param[in] pElement : elemento sobre el cual se aplica la herramienta.
 * \attention el elemento se usa para configurar el part y no se guarda
 * un puntero al mismo ni se modifica.
 */
IndexSelectionPart::IndexSelectionPart(const RasterElement * pElement, LibraryManager* pManager) :
      Part(true, false), selectedIndex_(0), pBandSelector_(NULL) {
   windowTitle_ = _(caption_INDEX_SELECTION);

   if (pElement != NULL) {
      // Obtiene la cantidad de bandas del elemento
      int bandcount = pElement->GetBandCount();
      std::string bandname;
      // Para cada banda obtiene el nombre y lo guarda en la propiedad interna
      for (int bandindex = 0; bandindex < bandcount; bandindex++) {
         pElement->GetBandName(bandname, bandindex);
         int valid = 1;
         pElement->GetBandValidity(valid, bandindex);
         if (valid != 0)
            bandNames_.Add(bandname);
      }
   }
   LibraryClient client(pManager, LibraryManager::READONLY);
   const Library* pLib = client.GetLibraryByCode(LibraryManagerFactory::IndexLibraryCode);
   pItemSelector_ = new BasicLibraryItemSelector(
         this, pLib->GetId(), pManager, "ID_LIBRARY_SELECTION_PANEL", "ID_LIBRARY_SELECTION_PANEL",
         true, UniversalGraphicalComponentPartInterface::ExtendedROWithFF);
}

/**
 * Destructor
 */
IndexSelectionPart::~IndexSelectionPart() {
}

/**
 * Crea la ventana de la parte.
 * Conecta el evento UIUpdate.
 * Carga la lista de bandas en los controles wx de seleccion de bandas.
 * @return true: si pudo crear la ventana
 * @return false: si no pudo crear la ventana.
 */
bool IndexSelectionPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_INDEX_SELECTION_PART"));
   if (!pToolWindow_) {
      return false;
   }

   AddControl(pItemSelector_->GetWidget(), "ID_INDEX_LIBRARY_PANEL");
   return true;
}

/**
 * Objetivo: Devuelve el icono de propiedades
 * Por ahora no tiene en cuenta el X e Y dado que devuelve el de 32x32 pixels
 * @param[in] ToolBitmap icono de la ventana
 * @param[in] X ancho del icono de la ventana
 * @param[in] Y alto del icono de la ventana
 */
void IndexSelectionPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_INDEX_SELECTION, ToolBitmap);
}

/**
 * Devuelve la propiedad que indica si la parte esta o no habilitada.
 * @return true si esta habilitada, false en otro caso
 */
bool IndexSelectionPart::IsEnabled() {
   return enabled_;
}

/**
 * Verifica que los datos seleccionados en el control wx sean validos.
 * @return true Si la cantidad de bandas seleccionadas concuerda con la cantidad
 * de bandas que requiere el indice
 * @return false En otro caso.
 */
bool IndexSelectionPart::HasValidData() {
   // TODO(Gabriel): Implementar.
   return true;
}

/**
 * Devuelve la propiedad que indica si hubo o no cambios entre los datos de las
 * propiedades y los datos de los controles wx.
 * @return true si hay cambios entre las propiedades y los valores seleccionados
 *    en el control wx
 */
bool IndexSelectionPart::HasChanged() {
   return modified_ || pBandSelector_->HasChanged();
}

/**
 * Verifica que los controles tengan datos validos, si no muestra un error en
 * pantalla. Si hay datos validos pero no hubo cambios desde el ultimo commit
 * sale retornando true.
 * Obtiene el indice de la pagina seleccionada y la lista de bandas seleccionadas
 * para esa pagina. Guarda ambos valores en las propiedades internas.
 * @return true: si los controles tienen datos validos.
 * @return false: si los controles no tienen datos validos.
 */
bool IndexSelectionPart::CommitChanges() {
   if (!HasValidData()) {
      SHOW_ERROR(errorMessage_);
      return false;
   }
   BandMathRenderer::Parameters::Equation* peq = NULL;
   if (GetSelectedEquation(peq)) {
      pPal_->AddAttribute<BandMathRenderer::Parameters::Equation *>(
                     IndexProcess::EquationKey, peq);
   } else {
      SHOW_ERROR(errorMessage_);
      return false;
   }
   pPal_->AddAttribute<int>(IndexProcess::IndexKeyAttr, selectedIndex_);
   pPal_->AddAttribute< std::vector<int> >(ProcessAdaptLayer::SelectedBandsKey,
                                                 selectedBands_);
   modified_ = false;
   return true;
}

/**
 * Carga en los controles wx los valores que tiene en las propiedades internas,
 * que corresponden al ultimo commit (o a los valores iniciales si no hubo commit)
 * Incializa las bandas del indice seleccionado con los valores default
 * @return false: cuando no puede obtener la pagina seleccionada
 * @return true: si puede obtener la pagina seleccionada.
 */
bool IndexSelectionPart::RollbackChanges() {
   // TODO(Gabriel): Implementar.
   if (pBandSelector_)
      pBandSelector_->RollbackChanges();
   modified_ = false;
   return true;
}

/**
 * Selecciona la pagina 0 por defecto, en la que esta el indice NDVI
 * Inicializa los choice con el valor que retorna GetDefaultChoiceValue
 * Invoca a RollbackChanges, que carga estos valores en los controles wx.
 */
void IndexSelectionPart::SetInitialValues() {
   BandMathRenderer::Parameters::Equation* peq = NULL;
   if (GetSelectedEquation(peq))
      pPal_->AddAttribute<BandMathRenderer::Parameters::Equation *>(
                     IndexProcess::EquationKey, peq);
   pPal_->AddAttribute<int>(IndexProcess::IndexKeyAttr, selectedIndex_);
   pPal_->AddAttribute< std::vector<int> >(ProcessAdaptLayer::SelectedBandsKey,
                                                 selectedBands_);
}

/**
 * Retorna una ecuacion configurada con los parametros seleccionados por el usario
 * @param[out] pEquation ecuacion configurada
 * return informa si la ecuacion esta bien configurada.
 */
bool IndexSelectionPart::GetSelectedEquation(
      BandMathRenderer::Parameters::Equation* &pEquation) const {
   BandMathRenderer::Parameters::Equation equation;
   const LibraryItem* pitem = pItemSelector_->GetActiveItem();
   equation.equation_ = pitem->GetAttribute("EQ")->GetValue();
   std::vector<int> selectedbands = pBandSelector_->GetVariableBands();
   EquationParser eqparser;
   if (!eqparser.SetEquation(equation.equation_))
      return false;
   // Valido que las variables en la ecuacion esten asignadas a bandas existentes
   std::vector<std::string> variablenames;
   eqparser.GetVariableNames(variablenames);
   if (variablenames.size() != selectedbands.size())
      return false;
   for (size_t i = 0; i < selectedbands.size(); ++i)
      equation.bandNames_[variablenames[i]] = selectedbands[i];
   
   pEquation = new BandMathRenderer::Parameters::Equation(equation);
   return true;
}

std::vector< std::pair<std::string, std::string> > IndexSelectionPart::GetVariableDescription(
                                                             const LibraryItem* pItem) const {
   std::vector< std::pair<std::string, std::string> > variables;
   LibraryItem::MapAttribute attrs = pItem->GetAttributes();
   for (LibraryItem::MapAttribute::const_iterator attr = attrs.begin(); attr != attrs.end(); ++attr) {
      if ((*attr).second->GetName().find("variable") != std::string::npos) {
         ComplexItemAttribute complexitem((*attr).second->GetName(), (*attr).second->GetValue());
         variables.push_back(std::pair<std::string, std::string>(complexitem.GetAttribute(0), complexitem.GetAttribute(1)));
      }
   }
   return variables;
}


/**
 * Configura el editor con el item que se pasa por parametro. Se pasa por parametro
 * un boolean que indica si se trata de la primera seleccion (para que el part no piense
 * que posee cambios cuando en realidad es la seleccion inicial del CGU)
 */
void IndexSelectionPart::SetActiveItem(const LibraryItem* pItem) {
   if (pItem) {
      const LibraryItemAttribute* attr = pItem->GetAttribute("EQ");
      if (attr) {
         std::string equationvalue = pItem->GetAttribute("EQ")->GetValue();
         std::vector< std::pair<std::string, std::string> > vardescription = GetVariableDescription(pItem);
         if (pBandSelector_)
            RemoveControl(pBandSelector_);
         pBandSelector_ = new BandSelectorWidget(this->GetWidget(), vardescription, bandNames_);
         modified_ = true;
         AddControl(pBandSelector_->GetWidget(), "ID_INDEX_VARIABLES_PANEL");
         }
      }
}

}  // namespace suri
