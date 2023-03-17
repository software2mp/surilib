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

#ifndef MEASSURE_H_
#define MEASSURE_H_

// Includes standard

// Includes Suri

// Includes Wx
#include "wx/wx.h"

// Includes App

// Defines

// Forwards

/** namespace suri */
namespace suri {
/** Clase Medicion. */
/**
 * Clase soporte para las clases que realizan mediciones
 * (MeassureDistanceElementEditor, MeassureAreaElementEditor).
 * Permite:
 *    - obtener un texto con la medicion formateada
 *    - cambiar de unidad de medida.
 * Mantiene la medicion realizada por las clases mencionadas.
 */
class Meassure {
   /** Ctor. de Copia. */
   Meassure(const Meassure &Meassure);
public:
   /** Tipo de unidad */
   typedef enum {
      Metre = 0, Kilometre, Hectare
   } UnitType;
   static const char *pUnitStrings_[]; /*! Nombres de las unidades de distancia */
   static const char *pAreaSuffix_[]; /*! Nombres de las unidades de area */
   static const double unitMultiplier_[]; /*! Multiplicador para transformar */
   /* metros en las unidades */
   /** Ctor. default. */
   Meassure(bool Area = false) :
         unit_(Kilometre), area_(0), perimeter_(0), distance_(0), isArea_(Area),
         active_(false) {
   }
   /** Dtor. */
   ~Meassure() {
   }
   /** Cambia la unidad de trabajo. */
   void ChangeUnit(UnitType Unit) {
      unit_ = Unit;
   }
   /** Obtiene la unidad de trabajo. */
   UnitType GetUnit() const {
      return unit_;
   }
   /** Obtiene la medicion. */
   wxString GetMeassurement() const;

   /**
    *
    */
   virtual void Start() { };

   /**
    *
    */
   virtual void Stop() { };

   /** */
   virtual bool IsActive() const {
      return active_;
   }
protected:
   UnitType unit_;  // Unidad de medicion
   double area_;  // Area
   double perimeter_;  // Perimetro
   double distance_;  // Distancia
   bool isArea_;  // indica si se trata de una medicion de area
   bool active_;
};
}

#endif /*MEASSURE_H_*/
