/*! \file ChoiceCommand.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "suri/ChoiceCommand.h"
#include "suri/ParameterCollection.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** clave para la configuracion de opciones del choice **/
const std::string ChoiceCommand::OptionInputParameterKey = "ChoiceOptions";
/** clave que indica el valor seleccionado al iniciar el control grafico **/
const std::string ChoiceCommand::InitialSelectionParameterKey = "InitialSelection";
/** clave que indica la clave asociada al parametro label **/
const std::string ChoiceCommand::LabelParameterKey = "Label";
/** clave que indica la seleccion actual del choice **/
const std::string ChoiceCommand::SelectionParameterKey = "ChoiceSelection";

/**
 * Constructor
 * @param pCommand command asociado un choice
 */
ChoiceCommand::ChoiceCommand(Command* pCommand) :
      Choice(NULL, pCommand->GetDescription(), 0), pCommand_(pCommand) {
   std::string label;
   ParameterCollection* pparams = pCommand_->GetInputParameters();
   if (pparams && pparams->GetValue<std::string>(LabelParameterKey, label)) {
      label_ = label;
   }
}

/** Destructor **/
ChoiceCommand::~ChoiceCommand() {
   ParameterCollection* pparams = pCommand_->GetInputParameters();
   std::vector < std::string > *poptions = NULL;
   pparams->GetValue<std::vector<std::string>*>(OptionInputParameterKey,
                                                            poptions);
   delete poptions;
}

/** Metodo que se llama cuando cambia la seleccion en el control grafico.  */
void ChoiceCommand::OnSelectionChanged(wxCommandEvent &Event) {
   std::string selection = pChoice_->GetStringSelection().c_str();
   ParameterCollection* pparams = new ParameterCollection;
   pparams->AddValue<std::string>(SelectionParameterKey, selection);
   pCommand_->SetOutputParameters(pparams);
   pCommand_->Execute();
}

/** Metodo que se llama cuando se ejecuta CreateToolWindow **/
void ChoiceCommand::OnCreateToolWindow() {
   ParameterCollection* pparams = pCommand_->GetInputParameters();
   std::vector < std::string > *poptions = NULL;
   if (pparams
         && pparams->GetValue<std::vector<std::string>*>(OptionInputParameterKey,
                                                         poptions) && poptions) {
      std::vector<std::string>::iterator it = poptions->begin();
      for (; it != poptions->end(); ++it) {
         pChoice_->Append((*it).c_str());
      }
      std::string selectvalue;
      if (pparams->GetValue<std::string>(InitialSelectionParameterKey, selectvalue)) {
         pChoice_->SetStringSelection(selectvalue.c_str());
         pparams->AddValue<std::string>(SelectionParameterKey, selectvalue);
      }
   }
}
} /** namespace suri */
