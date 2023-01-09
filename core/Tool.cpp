/*! \file Tool.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/Tool.h"

// Includes standard

// Includes Suri
#include "suri/messages.h"

// Includes Wx
#include "wx/wx.h"

// Includes App

// Defines

// Forwards

/** namespace suri */
namespace suri {
/**
 * Ctor. default
 * @return instancia de la clase Tool
 */
Tool::Tool() :
      toolName_(caption_UNKNOWN_TOOL), active_(false), enabled_(false), pList_(NULL),
      pViewer_(NULL) {
}

/**
 *  Dtor.
 */
Tool::~Tool() {
}

/**
 * Metodo para Des/Activar la herramienta
 * Este metodo se llama para indicarle a la herramienta que ha sido
 * activada.
 * La Herramienta realiza las operaciones necesarias en el metodo virtual
 * DoSetActive()
 * @param[in] Active estado de activacion
 * @return resultado de invocar a DoSetActive().
 */
bool Tool::SetActive(bool Active) {
   bool retval;
   if ((retval = DoSetActive(Active))) {
      active_ = Active;
   }
   return retval;
}

/**
 * Informa si la herramienta esta activa.
 * @return informa si el tool esta activo.
 */
bool Tool::IsActive() {
   return active_;
}

/**
 * Asigna la lista de elementos asociada a la herramienta
 * Para actualizar el atributo DoSetList debe retornar true
 * @param[in] pList puntero a la nueva lista
 */
void Tool::SetList(LayerList *pList) {
   if (pList && DoSetList(pList)) {
      pList_ = pList;
   }
}

/**
 * Asigna el visualizador del contexto asociado a la herramienta
 * Para actualizar el atributo DoSetViewer debe retornar true
 * @param[in] pViewer puntero al nuevo viewer
 */
void Tool::SetViewer(ViewerWidget *pViewer) {
   if ((pViewer) && DoSetViewer(pViewer)) {
      pViewer_ = pViewer;
   }
}

/* Indica si esta habilitado o no
 * @return true si esta habilitada.
 * @return false si no esta habilitada
 */
bool Tool::IsEnabled() {
   return enabled_;
}

/*
 * Establece el estado de la herramienta (habilitado o no)
 * @param[in] Enabled estado a setear en la herramienta.
 */
void Tool::SetEnabled(const bool &Enabled) {
   enabled_ = Enabled;
}

/**
 * agrega ventanas a la lista de depnedentWindows
 * @param[in] Window ventana a agregar.
 */
void Tool::AddDependentWindow(wxWindow* Window) {
   dependentWindows_.insert(Window);
}

/**
 * Des/Habilita los controles que afectan el funcionamiento
 * \post Las ventanas en dependentWindows_ toman estado SetEnabled(Status)
 *       despues de pasar por DoSetActive (que le da tiempo para modificar
 *       el estado).
 *  @param[in]	Enabled: estado de los controles.
 */
void Tool::EnableControls(bool Enabled) {
   std::set<wxWindow*>::iterator it = dependentWindows_.begin();
   while (it != dependentWindows_.end()) {
      (*it)->Enable(Enabled);
      it++;
   }
}
}  // namespace suri
