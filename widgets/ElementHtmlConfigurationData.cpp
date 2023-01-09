/*! \file ElementHtmlConfigurationData.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "ElementHtmlConfigurationData.h"

// Includes standard

// Includes Suri
#include "wxSimpleHtmlTreeCtrl.h"

// Includes Wx

// Includes App

// Defines

// Forwards

namespace suri {

/** Ctor */
ElementHtmlConfigurationData::ElementHtmlConfigurationData(
      Element* pElement, wxSimpleHtmlTreeCtrl* pWxTreeCtrl) :
      HtmlConfigurationData(""), pElement_(pElement), pWxTreeCtrl_(pWxTreeCtrl) {
   if (pElement_ == NULL)
      Leaf(false);
   else {
      pElement_->RegisterViewer(this);
      Update();
   }
}

/** Dtor */
ElementHtmlConfigurationData::~ElementHtmlConfigurationData() {
   if (pElement_ != NULL) pElement_->UnregisterViewer(this);
   pElement_ = NULL;
   pWxTreeCtrl_ = NULL;
}

/**
 * Retorna elemento asociado al nodo
 * @return Element con el que se configura el item. NULL si es un nodo interno
 * del arbol.
 */
Element* ElementHtmlConfigurationData::GetElement() const {
   return pElement_;
}

/** Actualiza el control con el contenido del elemento */
void ElementHtmlConfigurationData::Update() {
   if (!pElement_) return;

   pWxTreeCtrl_->BeginBatch();

   SetName(pElement_->GetName().c_str());
   if (pElement_->IsActive())
      SetState(HtmlConfigurationData::ON);
   else
      SetState(HtmlConfigurationData::OFF);
   SetIcon(pElement_->GetIcon().c_str());

   pWxTreeCtrl_->EndBatch();
}

/**
 * Evento de update de View
 * @param[in]  pModel: modelo que llama al Update.
 */
void ElementHtmlConfigurationData::Update(Model *pModel) {
   Update();
}

/**
 * Recibe el aviso de delete
 * @param[in]  pModel: modelo que se elimina.
 */
void ElementHtmlConfigurationData::OnModelDelete(Model *pModel) {
   pElement_ = NULL;
}

}

