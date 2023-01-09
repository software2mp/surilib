/** \file MeasureChangedNotification.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
