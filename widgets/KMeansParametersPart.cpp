/*! \file KMeansParametersPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
