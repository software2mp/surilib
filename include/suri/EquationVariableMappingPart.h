/*! \file EquationVariableMappingPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef __EQUATION_VARIABLE_MAPPING_WIDGET_H__
#define __EQUATION_VARIABLE_MAPPING_WIDGET_H__

// Includes Estandard
#include <vector>
#include <string>
#include <map>

// Includes suri
#include "suri/Part.h"
#include "EquationParser.h"

namespace suri {

// forward
class wxGenericTableBase;
class EquationVariableMappingPartEventHandler;
class Table;

/** 
 * Control grafico que permita asignar variables a bandas y asociar una breve descripcion de 
 * cada variable. Es posible configurar mediantes flags las columnas que se utilzaran en la tabla 
 * de mapeo (ej. solo ver variables y descripcion).
 * Para mas inforamcion ver ticket DIS #7728
**/
class EquationVariableMappingPart : public Part {
   /** const copia **/
   EquationVariableMappingPart(const EquationVariableMappingPart&);

public:
   static const int VariableDescriptionField = 0x02;
   static const int ReadOnlyDescription = 0x04;
   explicit EquationVariableMappingPart(const std::vector< std::pair<std::string, std::string> > &VarDescription,
                                       int LayoutFlags = VariableDescriptionField);
   explicit EquationVariableMappingPart(const std::vector<std::string>& BandNames,
                                       const std::vector< std::pair<std::string, std::string> >& VarDescription,
                                       int LayoutFlags = 0);
   /** dtor **/
   virtual ~EquationVariableMappingPart(); 
   /** Se obtiene la asociacion de variables con bandas actual */
   std::map<std::string, int> GetVariableBands() const;
   /** se obtiene la descripcion de las bandas actual **/
   std::vector< std::pair<std::string, std::string> > GetVariableDescription() const;
   /** obtiene la ecuacion **/
   std::string GetEquation() const;
   /** configura la ecuacion a utilizar **/
   void SetEquation(const std::string& Equation);
   void SetEquation(const std::string& Equation,
                     const std::vector< std::pair<std::string, std::string> >& VarDescription);
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
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
   /** Devuelve si la parte tiene datos validos */
   virtual bool HasValidData();

private:
   /** metodo privado encargado de configurar la cantidad de valores que puede tomar la columna de bandas**/
   void SetBandFieldRestrictions();
   /** Retorna las variables generadas al parsear Equation */
   EquationParser::EquationError ReadEquationVariables(const std::string& Equation,
                                    std::map<std::string, int> &Variables);
   /** Evento que se ejecuta al hacer click sobre boton validar ecuacion */
   void OnUpdateEquationButton(wxCommandEvent &Event);
   /**
    * Actualiza el control grafico con datos en los atributos widgetEquation_,
    * widgetVariableBands_ y widgetSelectedDataType_
    * @return bool que indica si ocurrio un error al actualizar los datos.
    */
   bool UpdateParametersToWidget();
   /** Actualiza los atributos widget con datos en control grafico */
   bool ReadParametersFromWidget();
   /** Retorna la bandas seleccionada por el usuario para una variable */
   int GetVariableSelectedBand(const std::string &VariableName);
   friend class EquationVariableMappingPartEventHandler;
   int layoutFlags_;
   std::string equation_;
   wxGenericTableBase* pVariablesTable_;
   suri::Table* pTable_;
   std::vector<std::string> imageBands_;
   EquationVariableMappingPartEventHandler* pEventHandler_;
   std::map<std::string, int> variableBands_;
   std::string widgetEquation_; /*! Ecuacion ingresada en widget */
   std::map<std::string, int> widgetVariableBands_; /*! Bandas asignadas en widget */
   std::vector< std::pair<std::string, std::string> >variableDescription_;
};

}  // namespace suri
#endif   // __EQUATION_VARIABLE_MAPPING_WIDGET_H__
