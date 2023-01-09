/*! \file ControlArrangement.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/ControlArrangement.h"

// Includes standard

// Includes Suri

// Includes Wx

// Defines

// Forwards

/** namespace suri */
namespace suri {
/**
 *  Guarda los valores de los parametros que se utilizaran al agregar un control
 * en la clase Widget mediante el metodo AddControl.
 *
 * @param[in] Proportion Proporcion del espacio disponible en la ventana
 * @param[in] Position Posicion dentro del sizer.
 * @param[in] Flags Flags de wx para las opciones del sizer.
 * @param[in] Border Borde en pixels.
 * @param[in] CreateMiniFrame Flag para indicar que se desea que la ventana a
 *  agregar posea un Mini-Frame (solo para el caso en que no haya sido creada
 *  todavia con una llamaga a GetWindow() o CreateTool()).
 */
ControlArrangement::ControlArrangement(int Proportion, int Position, int Flags,
                                       int Border, bool CreateMiniFrame) :
      proportion_(Proportion), position_(Position), flags_(Flags), border_(Border),
      createMiniFrame_(CreateMiniFrame) {
}

ControlArrangement::~ControlArrangement() {
}

// -------------------------------- Getters --------------------------------
/** getter para proportion_ */
int ControlArrangement::GetProportion() const {
   return proportion_;
}

/** getter para postion_ */
int ControlArrangement::GetPosition() const {
   return position_;
}

/** getter para flags_ */
int ControlArrangement::GetFlags() const {
   return flags_;
}

/** getter para border */
int ControlArrangement::GetBorder() const {
   return border_;
}

/** getter para createMiniFrame_ */
bool ControlArrangement::GetMiniFrame() const {
   return createMiniFrame_;
}
} /* namespace suri */
