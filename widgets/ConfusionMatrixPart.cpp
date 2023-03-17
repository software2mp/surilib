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

// Includes Estandar
#include<vector>
#include <string>
#include <set>
#include <map>

// Includes Suri
#include "ConfusionMatrixPart.h"
#include "resources.h"
#include "suri/DatasourceInterface.h"
#include "suri/DatasourceManagerInterface.h"
#include "ClassInformation.h"
#include "ClassifiedRasterDatasourceManipulator.h"
#include "EnclosureValidator.h"
#include "suri/messages.h"
#include "suri/ConfussionMatrixProcess.h"
#include "EnclosureManipulator.h"

// Includes Wx
// Defines
#define UNKNOWN_ENCLOSURE " "
// forwards

namespace suri {

/** Constructor **/
ConfusionMatrixPart::ConfusionMatrixPart(Element* pElement,
                                         DatasourceManagerInterface* pDatasourceManager) :
      pElement_(pElement), pDatasourceManager_(pDatasourceManager) {
   windowTitle_ = _(caption_CONFUSION_MATRIX_PROCESS_NAME);
}

/** Destructor **/
ConfusionMatrixPart::~ConfusionMatrixPart() {
}

/** Indica si el Part tiene cambios para salvar */
bool ConfusionMatrixPart::HasChanged() {
   return pClassMappingPart_->HasChanged();
}

/** Salva los cambios realizados en el Part */
bool ConfusionMatrixPart::CommitChanges() {
   if (!HasValidData()) {
      SHOW_ERROR(_(message_CONFUSION_MATRIX));
      return false;
   }
   bool returnvalue = pClassMappingPart_->CommitChanges();
   if (returnvalue && pPal_) {
      std::vector< std::map<std::string, DatasourceInterface*> > resultmap = GetResultMap();
      if (resultmap.size() > 0) {
         pPal_->AddAttribute<std::map<std::string, DatasourceInterface*> >(
               ConfussionMatrixProcess::TestResultMapKeyAttr, resultmap[0]);
      } else {
         returnvalue = false;
      }
   }
   return returnvalue;
}

/** Restaura los valor's originales del Part */
bool ConfusionMatrixPart::RollbackChanges() {
   bool returnvalue = pClassMappingPart_->RollbackChanges();
   if (returnvalue && pPal_) {
      std::vector< std::map<std::string, DatasourceInterface*> > resultmap = GetResultMap();
      if (resultmap.size() > 0) {
         pPal_->AddAttribute<std::map<std::string, DatasourceInterface*> >(
               ConfussionMatrixProcess::TestResultMapKeyAttr, resultmap[0]);
      } else {
         returnvalue = false;
      }
   }
   return returnvalue;
}

/** Crea el Tool Window */
bool ConfusionMatrixPart::CreateToolWindow() {
   pToolWindow_ = new wxPanel(pParentWindow_, wxID_ANY);
   pToolWindow_->SetSizer(new wxBoxSizer(wxVERTICAL));
   std::vector<ClassMappingPart::MappingTable> map(1);
   std::vector<ClassMappingPart::ChoiceValues> choice(1);
   LoadMap(map[0], choice[0]);
   std::vector<std::string> destinationcolumns;
   destinationcolumns.push_back(content_TEST_ENCLOSURE_COLUMN_NAME);
   pClassMappingPart_ = new ClassMappingPart(pElement_, map, choice,
                                             _(content_CLASS_COLUMN_NAME),
                                             destinationcolumns);
   AddControl(pClassMappingPart_->GetWidget(), pToolWindow_);
   return true;
}

/** Inicializa el part */
void ConfusionMatrixPart::SetInitialValues() {
   pClassMappingPart_->SetInitialValues();
   if (pPal_) {
      std::vector< std::map<std::string, DatasourceInterface*> > resultmap = GetResultMap();
      if (resultmap.size() > 0) {
         pPal_->AddAttribute<std::map<std::string, DatasourceInterface*> >(
               ConfussionMatrixProcess::TestResultMapKeyAttr, resultmap[0]);
      }
   }
}

/** Actualiza el estado de la parte */
void ConfusionMatrixPart::Update() {
}

/** Retorna el icono de la herramienta */
void ConfusionMatrixPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
      GET_BITMAP_RESOURCE(icon_CONFUSION_MATRIX, ToolBitmap);
}

/**
 *Metodo auxiliar para la carga del mapa y choice para el part de mapeo
 * @param[out] pMap referencia al mapa a llenar
 * @param[out] pChoice referencia al choice a llenar
 *
 */
void ConfusionMatrixPart::LoadMap(ClassMappingPart::MappingTable& Map,
                                      ClassMappingPart::ChoiceValues& Choice) {
   ClassifiedRasterDatasourceManipulator manipulator;
   EnclosureValidator validator;
   EnclosureManipulator emanipulator;
   DatasourceInterface* pdatasource = DatasourceInterface::Create("RasterDatasource",
                                                                  pElement_);
   std::vector<ClassInformation> classes = manipulator.GetClassInformationList(
         pdatasource);
   DatasourceInterface::Destroy(pdatasource);
   std::vector<ClassInformation>::iterator it = classes.begin();
   for (; it != classes.end(); ++it) {
      int classIndex = (*it).GetIndex();
      if ( classIndex != ClassInformation::NoClassIndex &&
            classIndex != ClassInformation::NDVIndex) {
         std::string enclosureid = (*it).GetDatasourceId();
         DatasourceInterface* penclosuredatasource =
               pDatasourceManager_->GetDatasource(enclosureid);
         std::string name = (*it).GetName();
         ClassMappingPart::MappingTablePair pair;        
         bool isValidEnclosure = validator.IsValid(penclosuredatasource);
         if (isValidEnclosure) {
            EnclosureInformation einfo = emanipulator.GetEnclosureInformation(penclosuredatasource);
            enclosureMap_.insert(std::make_pair(penclosuredatasource->GetName(), penclosuredatasource));
            pair = ClassMappingPart::MappingTablePair(name, penclosuredatasource->GetName());
         } else {
            pair = ClassMappingPart::MappingTablePair(name, UNKNOWN_ENCLOSURE);
         }
         Map.insert(pair);
      }
   }

   std::vector<SuriObject::UuidType> idvector = pDatasourceManager_->GetOrderedIds();
   std::vector<SuriObject::UuidType>::const_iterator dsit = idvector.begin();
   for (; dsit != idvector.end() ; ++dsit) {
      DatasourceInterface* pdatasourcelisted =
            pDatasourceManager_->GetDatasource(*dsit);
      EnclosureInformation einfo = emanipulator.GetEnclosureInformation(pdatasourcelisted);
      if (validator.IsValid(pdatasourcelisted)) {
         if (einfo.GetEnclosureType() == EnclosureInformation::Test){
            Choice.push_back(pdatasourcelisted->GetName());
         }
      }
      enclosureMap_.insert(
            std::make_pair(pdatasourcelisted->GetName(), pdatasourcelisted));
   }
}

/** Devuelve si la parte tiene datos validos
 *  Los datos se consideran validos si:
 *  * Hay un recinto asociado a mas de una clase
 *  * Todas las clases tienen asociados recintos existentes en la lista
 *  de fuente de datos
 *
 */
bool ConfusionMatrixPart::HasValidData() {
   if (!pClassMappingPart_->HasChanged())
      return hasvaliddata_;
   bool hasinvalidenclsoure = false;
   bool hasduplicatedenclosure = false;
   EnclosureValidator validator;
   std::set<std::string> enclosurenames;
   std::vector<ClassMappingPart::MappingTable> resultmap = pClassMappingPart_->GetResultMappingTable();
   EnclosureMap::iterator encit = enclosureMap_.begin();
   for (;encit != enclosureMap_.end(); ++encit){
      std::string strFirst(encit->first);
      strFirst.size();
   }
   for (size_t i = 0; !hasinvalidenclsoure && !hasinvalidenclsoure && i < resultmap.size(); ++i) {
      ClassMappingPart::MappingTable::iterator it = resultmap[i].begin(); 
      for (; it != resultmap[i].end() && !hasinvalidenclsoure && !hasduplicatedenclosure;
            ++it) {
         // obtengo el id del recinto y me fijo si es un recinto
         std::string currentMapping(it->second);
         EnclosureMap::iterator findit = enclosureMap_.find(it->second);
         if (findit == enclosureMap_.end()) {
            hasinvalidenclsoure = true;
         } else {
            // si no se puede insertar en el set es porque ya existe el nombre
            if (enclosurenames.insert(it->second).second) {
               DatasourceInterface* penclosuredatasource = findit->second;
               hasinvalidenclsoure = (!penclosuredatasource || !validator.IsValid(penclosuredatasource));
            } else {
               hasduplicatedenclosure = true;
            }
         }
      }
   }
   
   hasvaliddata_ = !hasinvalidenclsoure && !hasduplicatedenclosure;
   return hasvaliddata_;
}

/** devuelve el mapa resultado **/
std::vector<ConfusionMatrixPart::ResultMap> ConfusionMatrixPart::GetResultMap() {
   std::vector<ClassMappingPart::MappingTable> map = pClassMappingPart_->GetResultMappingTable();
   std::vector<ResultMap> resultmap(map.size());
   for (size_t i = 0; i < map.size(); ++i) {
      ClassMappingPart::MappingTable::iterator it = map[i].begin();
      for ( ; it != map[i].end() ; ++it) {
         std::string classname = it->first;
         EnclosureMap::iterator enclosureit = enclosureMap_.find(it->second);
         if (enclosureit !=enclosureMap_.end()) {
            DatasourceInterface* pdatasource = enclosureit->second;
            resultmap[i].insert(std::make_pair(classname, pdatasource));
         }
      }
   }
   return resultmap;
}
} /** namespace suri */
