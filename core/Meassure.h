/*! \file Meassure.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
