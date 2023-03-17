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

#ifndef CONFUSIONMATRIXPART_H_
#define CONFUSIONMATRIXPART_H_



// Includes Estandar
#include <map>
#include <string>

// Includes Suri
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "ClassMappingPart.h"

// Includes Wx

// Defines

namespace suri {

// forwards
class Element;
class DatasourceManagerInterface;

/**
 *
 */
class ConfusionMatrixPart : public suri::Part, public suri::ProcessAtributeProvider {
public:
   ConfusionMatrixPart(Element* pElement, DatasourceManagerInterface* pDatasourceManager);
   virtual ~ConfusionMatrixPart();
   /** Indica si el Part tiene cambios para salvar */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part */
   virtual bool CommitChanges();
   /** Restaura los valor's originales del Part */
   virtual bool RollbackChanges();
   /** Crea el Tool Window */
   bool CreateToolWindow();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Devuelve si la parte tiene datos validos */
   virtual bool HasValidData();
   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
   typedef std::map<std::string, DatasourceInterface*> ResultMap;
   /** devuelve el mapa resultado en un mapa <nombreclase, recinto> **/
   std::vector<ResultMap> GetResultMap();

private:
   // Parche para que se pueda acceder a los datos desde el proceso. Refactorizar.
   ClassMappingPart* pClassMappingPart_;
   /** Asosiacion nombre-id para los recintos **/
   typedef std::map<std::string, DatasourceInterface*> EnclosureMap;
   EnclosureMap enclosureMap_;
   /** Metodo auxiliar para la carga del mapa y choice para el part de mapeo **/
   void LoadMap(ClassMappingPart::MappingTable& Map,
               ClassMappingPart::ChoiceValues& Choice);
   Element* pElement_;
   DatasourceManagerInterface* pDatasourceManager_;
   /** Indica si el contenido que se carga al mapeo clase-recinto es valido**/
   /** es false cuando se detecta algun recinto inicial no existente **/
   bool hasvaliddata_;
};

} /** namespace suri */

#endif /* CONFUSIONMATRIXPART_H_ */
