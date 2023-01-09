/*! \file ClassMappingPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
