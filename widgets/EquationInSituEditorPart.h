/*! \file EquationInSituEditorPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef EQUATIONINSITUEDITORPART_H_
#define EQUATIONINSITUEDITORPART_H_

// Includes Estandar
// Includes Suri
#include "LibraryItemEditorPart.h"
// Includes Wx
// Defines

namespace suri {

// forwards
class EquationVariableMappingPart;

/**
 * Clase que representa un editor de items de biblioteca de ecuaciones.
 */
class EquationInSituEditorPart : public LibraryItemEditorPart {
   /** Constructor copia**/
   EquationInSituEditorPart(const EquationInSituEditorPart&);
public:
   /** Constructor */
   EquationInSituEditorPart(
         const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
         UniversalGraphicalComponentPartInterface::FeatureCombinationEnum Feature);
   /** Destructor */
   virtual ~EquationInSituEditorPart();
private:
   /**
    * Metodo auxiliar privado que es necesario implementar en las clases hijas.
    * Este metodo es el encargado de actualizar realmente la informacion en el part
    * Es llamado en el metodo SetActiveItem a modo de Template Method
    **/
   virtual void UpdateFields(const LibraryItem* pItem, bool Modifiable = true);
   /**  Metodo que obtiene a partir de los datos del editor, un vector con los atributos
    *   Este metodo es el que hace el comportamiento particular de cada editor a la hora
    *   de obtener la informacion que posee y transmitirla al sistema de bibliotecas.
    *   Es utilizado a modo de Template Method (en CommitChanges y en el notificador)
    *   @return vector con atributos
    *   @return NULL en caso de que la informnacion del part no sea valida
    *   **/
   virtual std::vector<LibraryItemAttribute*>* CreateAttributesFromFields();
   /**
    *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
    *  Este metodo es el que realmente tienen que implementar las clases hijas para
    * lograr la especializacion a la hora de iniciar el part (metodo que se llama
    * en CreateToolWindow, a modo de Template Method).
    **/
   virtual bool ConfigureFeatures();
   /** Indica si el Part tiene cambios para salvar */
   bool HasChanged();
   /** Verifica la validez de ecuacion */
   bool IsValidEquation(const std::string& Equation) const;
   std::vector< std::pair<std::string, std::string> > GetVariableDescription(const LibraryItem* pItem);

   /** xrc de panel de editor de density slice **/
   static const std::string EQUATION_EDITION_XRC_NAME;
   /** id del panel donde se ubica el cgu **/
   static const std::string EQUATION_CGU_XRC_NAME;
   /** Contiene el string con la ecuacion */
   std::string equationvalue_;
   EquationVariableMappingPart* pVariableMappingPart_;
};

} /** namespace suri */

#endif /* EQUATIONINSITUEDITORPART_H_ */
