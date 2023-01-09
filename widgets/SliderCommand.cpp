/*! \file SliderCommand.cpp */
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

// Includes Suri
#include "suri/SliderCommand.h"
#include "suri/ParameterCollection.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/** clave para la configuracion del valor minimo del slider **/
const std::string SliderCommand::MaxValueInputParameterKey = "SliderMaxValue";
/** clave para la configuracion del valro maximo del slider **/
const std::string SliderCommand::MinValueInputParameterKey = "SliderMinValue";
/** clave para la configuracion del valro maximo del slider **/
const std::string SliderCommand::ValueInputParameterKey = "InitialValue";
/** clave que indica la clave asociada al parametro label **/
const std::string SliderCommand::LabelParameterKey = "Label";
/** clave que indica la seleccion actual del choice **/
const std::string SliderCommand::SelectionParameterKey = "Selection";

/**
 * Constructor
 * @param pCommand command asociado al control grafico slider
 */
SliderCommand::SliderCommand(Command* pCommand) :
      Slider(NULL, pCommand->GetDescription(), 0),
      pCommand_(pCommand) {
   std::string label;
   ParameterCollection* pparams = pCommand_->GetInputParameters();
   if (pparams && pparams->GetValue<std::string>(LabelParameterKey, label)) {
      label_ = label;
   }
}

/** Destructor **/
SliderCommand::~SliderCommand() {
}

/** Metodo que se llama cuando se ejecuta CreateToolWindow **/
void SliderCommand::OnCreateToolWindow() {
   ParameterCollection* pparams = pCommand_->GetInputParameters();
   int min = -1, max = -1;
   if (pparams && pparams->GetValue<int>(MinValueInputParameterKey, min)
         && pparams->GetValue<int>(MaxValueInputParameterKey, max)) {
      pSlider_->SetRange(min, max);
   }
   int value = -1;
   if (pparams->GetValue<int>(ValueInputParameterKey, value)) {
      pSlider_->SetValue(value);
   }
}

/** Metodo que se llama cuando cambia la seleccion en el control grafico.  */
void SliderCommand::OnValueChanged(wxScrollEvent &Event) {
   int selection = pSlider_->GetValue();
   ParameterCollection* pparams = new ParameterCollection;
   pparams->AddValue<int>(SelectionParameterKey, selection);
   pCommand_->SetOutputParameters(pparams);
   pCommand_->Execute();
}
} /** namespace suri */
