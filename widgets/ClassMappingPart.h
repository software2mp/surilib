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

#ifndef CLASSMAPPINGPART_H_
#define CLASSMAPPINGPART_H_

// Includes standard
#include <vector>
#include <map>
#include <utility>
#include <string>

// Includes Suri
#include "suri/Part.h"
#include "suri/Element.h"
#include "TablePart.h"

// Includes Wx

// Includes App

// Defines

namespace suri {

/** Part que permite asignar a las clases de una imagen clasificada un string */
/**
 * Se configura con un vector de strings y permite que el usuario asigne a
 * cada clase de la imagen uno de los elementos de este string.
 */
class ClassMappingPart : public Part {
public:
   /** Defincion de par que se utiliza */
   typedef std::pair< std::string, std::string > MappingTablePair;
   /** Definicion de mapa que utiliza la tabla */
   typedef std::map<std::string, std::string >MappingTable;
   typedef std::pair<std::string, std::string> ChoiceOptionValue;
   typedef std::vector<std::string> ChoiceValues;
   /** Campo origen en el mapeo */
   static const std::string OriginField;
   /** Campo Destino en el mapeo */
   static const std::string DestinationField;
   /** Constructor */
   ClassMappingPart(Element *pElement, const std::vector<MappingTable>& MapTable,
                    const std::vector<ChoiceValues>& ChoiceOptionValues,
                    const std::string& OriginColumnName,
                    const std::vector<std::string>& DestinationColumnName,
                    bool Enable = true,
                    bool Modified = false);
   /** Destructor */
   virtual ~ClassMappingPart();
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
   /** Agrega una opcion al choice que posee el part */
   bool AddChoiceOption(const std::string& OptionValue, int MappingIndex = 0);
   /** Si se hizo commit, devuelve la informacion actual de la tabla en un map */
   std::vector<MappingTable> GetResultMappingTable();

   /**
    * Agrega una clase nueva a la tabla.
    */
   void AppendClass(const std::string& Value);
   bool IsEditing() { return pTablePart_->IsEditing();}
private:
   /** Constructor */
   ClassMappingPart(const ClassMappingPart&);
   /** Configura la tabla del part con la informacion que contiene el elemento */
   void ConfigureGuiTable();
   /* Carga la tabla a un map**/
   bool LoadMapFromTable();
   /** Tabla que se usa para asignar las bandas */
   Table* pVariablesTable_;
   /** Part que contiene la tabla **/
   TablePart* pTablePart_;
   /** Mapa que representa la tabla a mostrar */
   std::vector<MappingTable> mappingTable_;
   std::vector<MappingTable> rollbackMappingTable_;
   /** Elemento que posee el contenido a mostrar del part */
   Element* pElement_;
   /** Valores posibles del choice map<id,valor>*/
   std::vector<ChoiceValues> choiceValues_;
   /** Nombre de columna de origen */
   std::string originColumnName_;
   /** Nombre de columna de destino */
   std::vector<std::string> destinationColumnName_;
};
}

#endif /* CLASSMAPPINGPART_H_ */
