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

#include "suri/Dimension.h"

// Includes standard
#include <cmath>

// Includes suri
#include "suri/AuxiliaryFunctions.h"

/** namespace suri */
namespace suri {
/** ctor */
/**
 * Construye un objeto dimension a partir de un Subset. Calcula el Ancho y Alto
 * del subset y los almacena.
 * @param[in] TargetSubset subset que se quiere medir
 */
Dimension::Dimension(const Subset &TargetSubset) {
   *this = Dimension(TargetSubset.ul_, TargetSubset.lr_);
}

/** ctor */
/**
 * Construye un objeto dimension a partir de dos Coordinates que representan
 * el UL y LR. Calcula el Ancho y Alto del subset y los almacena.
 * @param[in] Ul coordenada upper left del subset a medir
 * @param[in] Lr coordenada lower right del subset a medir
 */
Dimension::Dimension(const Coordinates &Ul, const Coordinates &Lr) {
   width_ = std::abs(Lr.x_ - Ul.x_);
   height_ = std::abs(Lr.y_ - Ul.y_);
   signX_ = Lr.x_ < Ul.x_ ? -1 : 1;
   signY_ = Lr.y_ < Ul.y_ ? -1 : 1;
}

/**
 * Operador igualdad
 * Compara valores del tipo Coordinates. Utiliza el metodo Coordinates::IsEqual
 * para efectuar la comparacion.
 * @param[in] Lhs Operando 1 (izquierdo)
 * @param[in] Rhs Operando 2 (derecho)
 * @return true si Lhs.IsEqual(Rhs)
 * @return false si !Lhs.IsEqual(Rhs)
 */
bool operator==(const Coordinates &Lhs, const Coordinates &Rhs) {
   return Lhs.IsEqual(Rhs);
}

/**
 * Operador desigualdad
 * Compara valores del tipo Coordinates.
 * @param[in] Lhs Operando 1 (izquierdo)
 * @param[in] Rhs Operando 2 (derecho)
 * @return true si Lhs!=Rhs
 * @return false si Lhs==Rhs
 * \note no se utiliza el metodo Coordinates::IsEqual
 */
bool operator!=(const Coordinates &Lhs, const Coordinates &Rhs) {
   return !(Lhs == Rhs);
}

/**
 * Operador igualdad.
 * Compara valores del tipo Subset.
 * @param[in] Lhs Operando 1 (izquierdo)
 * @param[in] Rhs Operando 2 (derecho)
 * @return true si las componentes ul_ y lr_ de ambos operandos son iguales.
 * @return false si alguna de las componentes ul_ o lr_ de ambos operandos son distintas.
 */
bool operator==(const Subset &Lhs, const Subset &Rhs) {
   return Lhs.ul_ == Rhs.ul_ && Lhs.lr_ == Rhs.lr_;
}

/**
 * Operador desigualdad.
 * Compara valores del tipo Subset.
 * @param[in] Lhs Operando 1 (izquierdo)
 * @param[in] Rhs Operando 2 (derecho)
 * @return true si alguna de las componentes ul_ o lr_ de ambos operandos son distintas.
 * @return false si las componentes ul_ y lr_ de ambos operandos son iguales.
 */
bool operator!=(const Subset &Lhs, const Subset &Rhs) {
   return !(Lhs == Rhs);
}

/**
 * Operador igualdad.
 * Compara valores del tipo Dimension.
 * @param[in] Lhs Operando 1 (izquierdo)
 * @param[in] Rhs Operando 2 (derecho)
 * @return true si:
 *  * las dimensiones en x y las dimensiones en y de ambos operandos son iguales.
 *  * los signos en x e y de ambos operandos son iguales.
 * @return false si alguna de las condiciones mencionadas no se cumple.
 */
bool operator==(const Dimension &Lhs, const Dimension &Rhs) {
   return Lhs.GetWidth() == Rhs.GetWidth() && Lhs.GetHeight() == Rhs.GetHeight()
         && Lhs.XSign() == Rhs.XSign() && Lhs.YSign() == Rhs.YSign();
}

/**
 * Operador desigualdad
 * Compara valores del tipo Dimension.
 * @param[in] Lhs Operando 1 (izquierdo)
 * @param[in] Rhs Operando 2 (derecho)
 * @return true si !(Lhs==Rhs)
 * @return true si (Lhs==Rhs)
 */
bool operator!=(const Dimension &Lhs, const Dimension &Rhs) {
   return !(Lhs == Rhs);
}

/**
 * operator suma de una coordenada a un subset (corrimiento).
 * Realiza la suma algebraica, de las componentes de cada operando.
 * @param[in] Lhs es un objeto del tipo Subset
 * @param[in] Rhs es un objeto del tipo Coordinates
 * @return Subset resultado de sumar la Coordenada al Subset de entrada.
 * (Subset = Lhs + Rhs).
 */
Subset operator+(const Subset &Lhs, const Coordinates &Rhs) {
   Subset result;
   result.ul_.x_ = Lhs.ul_.x_ + Rhs.x_;
   result.ul_.y_ = Lhs.ul_.y_ + Rhs.y_;
   result.ul_.z_ = Lhs.ul_.z_ + Rhs.z_;
   result.lr_.x_ = Lhs.lr_.x_ + Rhs.x_;
   result.lr_.y_ = Lhs.lr_.y_ + Rhs.y_;
   result.lr_.z_ = Lhs.lr_.z_ + Rhs.z_;

   return result;
}

/**
 * operator suma de una Dimension a un subset (corrimiento)
 * Crea un objeto del tipo Coordinates con las propiedades del objeto Dimension
 * y luego utiliza el operator+ definido para Subset y Coordenada.
 * @param[in] Lhs es un objeto del tipo Subset
 * @param[in] Rhs es un objeto del tipo Dimension.
 * @return Subset resultado de sumar la Dimension al Subset de entrada.
 * (Subset = Lhs + Rhs).
 */
Subset operator+(const Subset &Lhs, const Dimension &Rhs) {
   return Lhs + Coordinates(Rhs.XSign() * Rhs.GetWidth(), Rhs.YSign() * Rhs.GetHeight());
}
} /*namespace suri*/

