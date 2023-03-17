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

#ifndef MEASURECHANGEDNOTIFICATION_H_
#define MEASURECHANGEDNOTIFICATION_H_

#include <string>

#include "suri/NotificationInterface.h"

namespace suri {

/**
 *  Contiene la informacion sobre los cambios en las medidas (distancia, area y perimetro)
 */
class MeasureChangedNotification : public NotificationInterface {
public:
   static const int kArea = 1;
   static const int kPerimeter = 2;
   static const int kDistance = 4;

   /**
    * Ctor.
    */
   MeasureChangedNotification() : area_(0), perimeter_(0), distance_(0), type_(0) { }

   /**
    * Dtor.
    */
   ~MeasureChangedNotification() { }

   /**
    * Metodo que ejecuta una accion sobre el observador a ser notificado.
    */
   virtual bool ApplyNotification(ObserverInterface* pObserver) {
      if (pObserver != NULL)
         pObserver->Update(this);
      return true;
   }

   /**
    * Devuelve el area.
    */
   double GetArea() { return area_; }

   /**
    * Establece el area.
    */
   void SetArea(double Area) { area_ = Area; }

   /**
    * Devuelve el perimetro.
    */
   double GetPerimeter() { return perimeter_; }

   /**
    * Establece el perimetro.
    */
   void SetPerimeter(double Perimeter) { perimeter_ = Perimeter; }

   /**
    * Devuelve la distancia.
    */
   double GetDistance() { return distance_; }

   /**
    * Establece la distancia.
    */
   void SetDistance(double Distance) { distance_ = Distance; }

   /**
    * Devuelve el tipo de medida que se esta notificando.
    */
   int GetType() { return type_; }

   /**
    * Establece el tipo de medida que se esta notificando.
    */
   void SetType(int Type) { type_ = Type; }

   void SetMeassureGeometry(const std::string& Wkt) { geometry_ = Wkt; }

   std::string GetMeassureGeometry() { return geometry_; }

private:
   double area_;
   double perimeter_;
   double distance_;
   int type_;
   std::string geometry_;
};

}  // namespace suri

#endif  // MEASURECHANGEDNOTIFICATION_H_
