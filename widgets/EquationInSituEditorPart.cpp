/*! \file EquationInSituEditorPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <vector>
#include <utility>

// Includes Suri
#include "EquationInSituEditorPart.h"
#include "UniversalGraphicalComponentPartInterface.h"
#include "suri/LibraryManager.h"
#include "suri/LibraryItemAttribute.h"
#include "EquationParser.h"
#include "suri/EquationVariableMappingPart.h"
#include "suri/ComplexItemAttribute.h"
#include "suri/AuxiliaryFunctions.h"

// Includes Wx
// Defines
#define ID_EQUATION_CONFIG_PANEL "ID_EQUATION_CONFIG_PANEL"
// forwards

namespace suri {

/** Nombre del XRC que representa la ventana. */
const std::string EquationInSituEditorPart::EQUATION_EDITION_XRC_NAME =
      "ID_EQUATION_IN_SITU_EDITOR";
/** Nombre del XRC que representa el panel del cgu */
const std::string EquationInSituEditorPart::EQUATION_CGU_XRC_NAME = "ID_EQUATION_CGU_PANEL";

/**
 * Constructor
 * @param LibraryId id de la biblioteca de tabla de intervalos
 * @param pManager manager que administra la biblioteca
 * @param Feature layout de features del cgu
 */
EquationInSituEditorPart::EquationInSituEditorPart(const SuriObject::UuidType& LibraryId,
      LibraryManager* pManager,
      UniversalGraphicalComponentPartInterface::FeatureCombinationEnum Feature) :
      LibraryItemEditorPart(LibraryId, pManager, EQUATION_EDITION_XRC_NAME,
                            EQUATION_CGU_XRC_NAME, true, Feature), pVariableMappingPart_(NULL) {
}

/** Destructor */
EquationInSituEditorPart::~EquationInSituEditorPart() {
}

std::vector< std::pair<std::string, std::string> > EquationInSituEditorPart::GetVariableDescription(
                                                         const LibraryItem* pItem) {
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
 * Metodo auxiliar privado que es necesario implementar en las clases hijas.
 * Este metodo es el encargado de actualizar realmente la informacion en el part
 * Es llamado en el metodo SetActiveItem a modo de Template Method
 **/
void EquationInSituEditorPart::UpdateFields(const LibraryItem* pItem, bool Modifiable) {
   if (pItem) {
      const LibraryItemAttribute* pattr = pItem->GetAttribute("EQ");
      equationvalue_ = pattr ? pattr->GetValue() : "";
      std::vector < std::pair<std::string, std::string> > vardescription =
            GetVariableDescription(pItem);
      if (pVariableMappingPart_) {
         pVariableMappingPart_->SetEquation(equationvalue_, vardescription);
         if (Modifiable)
            pVariableMappingPart_->Enable();
         else
            pVariableMappingPart_->Disable();
      }
   }
}

/**
 * Metodo que obtiene a partir de los datos del editor, un vector con los atributos
 * Este metodo es el que hace el comportamiento particular de cada editor a la hora
 * de obtener la informacion que posee y transmitirla al sistema de bibliotecas.
 * Es utilizado a modo de Template Method (en CommitChanges y en el notificador)
 * @return vector con atributos
 * @return NULL en caso de que la informnacion del part no sea valida
 */
std::vector<LibraryItemAttribute*>* EquationInSituEditorPart::CreateAttributesFromFields() {
   std::vector<LibraryItemAttribute*>* pattributes = new std::vector<
            LibraryItemAttribute*>;
   if (pVariableMappingPart_->CommitChanges()) {
      std::string equationvalue = pVariableMappingPart_->GetEquation();
      suri::LibraryItemAttribute* libattr = new suri::LibraryItemAttribute("EQ", equationvalue);
      // TODO(Gabriel TCK #7727): Aca se deberia obtener las descripciones de las variables
      // y generar los atributos correspondientes para el item
      std::vector< std::pair<std::string, std::string> > variables = pVariableMappingPart_->GetVariableDescription();
      for (size_t i = 0; i < variables.size(); ++i) {
         std::string variableattr = "{" + variables[i].first + "," + variables[i].second + "}";
         std::string varname = "variable" + NumberToString<int>(i);
         pattributes->push_back(new LibraryItemAttribute(varname, variableattr));
         
      }
      pattributes->push_back(libattr);
   }
   return pattributes;
}

/** Indica si el Part tiene cambios para salvar */
bool EquationInSituEditorPart::HasChanged() {
   return pVariableMappingPart_->HasChanged();
}

/**
 * Metodo auxiliar privado que configura el part en si para cada editor en particular.
 * Este metodo es el que realmente tienen que implementar las clases hijas para lograr
 * la especializacion a la hora de iniciar el part (metodo que se llama en
 * CreateToolWindow, a modo de Template Method).
 */
bool EquationInSituEditorPart::ConfigureFeatures() {
   pVariableMappingPart_ = new EquationVariableMappingPart(std::vector< std::pair<std::string, std::string> >());
   AddControl(pVariableMappingPart_->GetWidget(), ID_EQUATION_CONFIG_PANEL);
   if (!equationvalue_.empty())
      pVariableMappingPart_->SetEquation(equationvalue_);
   return true;
}
} /** namespace suri */
