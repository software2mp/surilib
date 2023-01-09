/*! \file ControlArrangement.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CONTROLARRANGEMENT_H_
#define CONTROLARRANGEMENT_H_

// Includes standard

// Includes Suri

// Includes Wx
#include "wx/wx.h"

// Defines

// Forwards

/** namespace suri */
namespace suri {
/** Clase de distribucion de controles para el metodo Widget::AddControl */
/**
 *  Guarda los valores de los parametros que se utilizaran al agregar un control
 * en la clase Widget mediante el metodo AddControl.
 *
 *  Encapsula los parametros de los metodos, Add/Insert/Prepend de wxSizer.
 */
class ControlArrangement {
public:
   /** ctor con parametros */
   ControlArrangement(int Proportion = 1, int Position = -1, int Flags = wxEXPAND,
                      int Border = 0, bool CreateMiniFrame = false);
   /** dtor */
   ~ControlArrangement();
// -------------------------------- Getters --------------------------------
   /** getter para proportion_ */
   int GetProportion() const;
   /** getter para postion_ */
   int GetPosition() const;
   /** getter para flags_ */
   int GetFlags() const;
   /** getter para border_ */
   int GetBorder() const;
   /** getter para createMiniFrame_ */
   bool GetMiniFrame() const;
protected:
private:
   int proportion_; /*! Proporcion del espacio libre ocupado por el control */
   int position_; /*! Posicion dentro del sizer */
   int flags_; /*! flags de wx para el sizer */
   int border_; /*! tamano en pixels del borde */
   bool createMiniFrame_; /*! Indica si se debe crear con miniframe */
};
} /* namespace suri */
#endif /* CONTROLARRANGEMENT_H_ */
