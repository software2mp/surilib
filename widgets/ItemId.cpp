/*! \file ItemId.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "ItemId.h"

// Includes standard
#include <string>
// Includes Suri

// Includes Wx

// Includes App

// Defines

namespace suri {
/**
 * Recibe por parametro el identificador unico
 * @param[in] Id valor del id
 */
ItemId::ItemId(std::string Id) :
      id_(Id) {
}

/** Ctor de copia */
ItemId::ItemId(const SuriObject &Id) : id_(Id.GetId()) {
}

/** Ctor de copia */
ItemId::ItemId(const ItemId &Id) {
   id_ = Id.id_;
}

/** Dtor */
ItemId::~ItemId() {
}

/**
 * Operador de copia
 * @param[in] Id instancia con datos a copiar
 */
ItemId& ItemId::operator=(const ItemId& Id) {
   id_ = Id.id_;
   return *this;
}

/**
 * Compara el id contra el del ItemId que recibe por parametro
 * @param[in] OtherItemId Identificador contra el que se compara el valor.
 * @return 0 si son iguales, <>0 si son diferentes
 */
int ItemId::Compare(const ItemId &OtherItemId) const {
   return id_.compare(OtherItemId.id_);
}

/**
 * Retorna id en formato string. Este metodo no se deberia usar fuera de wx.
 * @return std::string con id
 */
std::string ItemId::GetId() const {
   return id_;
}
}  // namespace
