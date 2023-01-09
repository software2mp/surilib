/*! \file HtmlTreeNode.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "HtmlConfigurationData.h"

// Includes standard

// Includes Suri
#include "resources.h"

// Includes Wx

// Includes App

// Defines

namespace suri {
/** Ctor */
HtmlConfigurationData::HtmlConfigurationData(std::string Name, bool Leaf) :
      name_(Name), icon_(icon_UNDEFINED_ELEMENT), state_(OFF), isLeaf_(Leaf),
      pExecuteOnState_(NULL) {
}

/** Dtor */
HtmlConfigurationData::~HtmlConfigurationData() {
}

/** Retorna nombre de nodo */
std::string HtmlConfigurationData::GetName() const {
   return name_;
}

/** Modifica nombre del nodo//! Modifica el nombre del icono */
void HtmlConfigurationData::SetName(std::string Name) {
   this->name_ = Name;
}

/** Retorna el nombre del icono */
std::string HtmlConfigurationData::GetIcon() const {
   return icon_;
}

/** Modifica el nombre del icono */
void HtmlConfigurationData::SetIcon(std::string Icon) {
   this->icon_ = Icon;
}

/** Indica si es nodo hoja */
bool HtmlConfigurationData::IsLeaf() const {
   return isLeaf_;
}

/** Modifica indicador de si es hoja */
void HtmlConfigurationData::Leaf(bool IsLeaf) {
   this->isLeaf_ = IsLeaf;
}

/** Retorna estado del item. */
HtmlConfigurationData::State HtmlConfigurationData::GetState() const {
   return state_;
}

/** Configura estado del item. */
void HtmlConfigurationData::SetState(HtmlConfigurationData::State State) {
   this->state_ = State;
}

/** Retorna command. */
Command* HtmlConfigurationData::GetExecuteOnState() const {
   return pExecuteOnState_;
}

void HtmlConfigurationData::SetExecuteOnState(Command* pExecuteOnState) {
   this->pExecuteOnState_ = pExecuteOnState;
}
}

