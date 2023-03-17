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

// Includes standard

// Includes Suri
#include "KMeansParametersPart.h"
#include "suri/FactoryHelpers.h"
#include "suri/messages.h"
#include "suri/AuxiliaryFunctions.h"

// Includes Wx
#include "wx/xrc/xmlres.h"

// Includes App

// Defines

namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(KMeansParametersPartEvent, KMeansParametersPart)IMPLEMENT_EVENT_CALLBACK(OnScroll, OnScroll(event), wxScrollEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/* /! Macro para registrar la clase */
AUTO_REGISTER_CLASS(NonSupervisedParameterSelectionPart, KMeansParametersPart, 0)

KMeansParametersPart::KMeansParametersPart() :
      NEW_EVENT_OBJECT(KMeansParametersPartEvent) {
}

KMeansParametersPart::~KMeansParametersPart() {
}

/**
 * Crea la ventana de la parte
 * @return true si pudo crear la ventana
 */
bool KMeansParametersPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_KMEANS_PARAMS_PANEL"));

   GET_CONTROL(*pToolWindow_, wxT("ID_THRESHOLD"), wxSlider)
      ->Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(
            KMeansParametersPartEvent::OnScroll),
                NULL, pEventHandler_);

   GET_CONTROL(*pToolWindow_, wxT("ID_THRESHOLD"), wxSlider)
      ->Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(
            KMeansParametersPartEvent::OnScroll),
                NULL, pEventHandler_);

   return true;
}

/**
 * Devuelve el estado del Part.
 * @return true si el part esta habilitado, false en otro caso
 */
bool KMeansParametersPart::IsEnabled() {
   return true;
}

/**
 * Indica si el Part tiene cambios para salvar.
 * @return true si los datos de las propiedades son distintos a los del control
 *          wx. false si son iguales.
 */
bool KMeansParametersPart::HasChanged() {
   bool haschanged = false;
   return haschanged;
}

/**
 * Salva los cambios realizados en el Part.
 * @return true siempre
 */
bool KMeansParametersPart::CommitChanges() {
   return true;
}

/**
 * Restaura los valores originales del Part.
 * @return true siempre
 */
bool KMeansParametersPart::RollbackChanges() {
   return true;
}

/**
 * Inicializa los datos mostrados
 */
void KMeansParametersPart::SetInitialValues() {
}

/**
 * Actualiza el estado de la parte
 */
void KMeansParametersPart::Update() {
}

// --- Metodos de ParametersSelectionPart ---

/**
 * Elimina todas las clases cargadas.
 * @return true si pudo crear una instancia nueva de la clase MaxLikelihoodAlgorithm
 * @return false en otro caso
 */
bool KMeansParametersPart::Reset() {
   return true;
}

/**
 * Retorna el nombre del algoritmo que se usara para clasificar
 * @return string con el nombre del algoritmo de clasificacion
 */
std::string KMeansParametersPart::GetAlgorithmName() {
   return _(caption_KMEANS);
}

/**
 * Agrega clases que se usan para clasificar
 * @param[in] ClassId identificador de la clase
 * @param[in] Stats estadisticas de la clase
 * @return true si pudo agregar las clases
 * @return false si el id de clase es cero, indica sin clase
 */
bool KMeansParametersPart::AddClass(int ClassId, Statistics Stats) {
   return true;
}

/**
 * Retorna un puntero al clasificador creado por part
 * @return puntero al algoritmo usado para clasificar
 * \attention El puntero que se retorna es propiedad de esta clase, no eliminar
 */
ClassificationAlgorithmInterface* KMeansParametersPart::GetClassificationManager() {
   return NULL;
}

/**
 * Retorna la cantidad de clases. Le suma 1 debido a que la clase 0 se
 * considera sin clase.
 * @return cantidad de clases, o -1 si no existe el algoritmo de clasificacion
 */
int KMeansParametersPart::GetClassCount() {
   return 0;
}

/**
 * Si la posicion pedida es 0 se guarda el color default(seleccionado por el
 * usuario), caso contrario no se hace nada
 * @param[in] Properties contiene el id de la clase que se desea
 * @param[out] Properties propiedades de la clase indiciada
 */
bool KMeansParametersPart::GetClassProperties(
      ClassInformation &Properties) {
   bool found = false;
   return found;
}

/**
 * modifica el control que informa la posicion
 * @param[in] Event evento scroll
 */
void KMeansParametersPart::OnScroll(wxScrollEvent& Event) {
   // Para asignarle el label a la barra se usa escala lineal y no logaritmica
   std::string percentasstring = wxString::Format("%i", Event.GetPosition()).c_str();
   GET_CONTROL(*pToolWindow_, wxT("ID_SELECTED_THRESHOLD_TEXT"), wxTextCtrl)
      ->ChangeValue(percentasstring.c_str());
}

}
