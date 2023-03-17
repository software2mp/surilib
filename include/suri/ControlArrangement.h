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
