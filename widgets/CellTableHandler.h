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

#ifndef CELLTABLEHANDLER_H_
#define CELLTABLEHANDLER_H_

// Includes Estandar
// Includes Suri
#include "suri/Table.h"
// Includes Wx
#include "wx/grid.h"
// Defines
// forwards

namespace suri {

/** Clase que representa un handler para eventos sobre una tabla. Cada handler
 *  de eventos puede tener un sucesor. De esta forma se realiza una cadena
 *  de responsabilidad para el manejo del evento. **/
class CellTableHandler {
public:
   /** Enum para teclas que se pueden presionar e influyen en la captura del evento**/
   enum KeyDown {NONE = -1, CTRL = 0, UP, DOWN, LEFT, RIGHT, ALT, SHIFT};

   enum MouseEvent {LCLICK = 0 , RCLICK, DLCLICK };
   /** Constructor **/
   CellTableHandler() : pSucesor_(NULL) {}
   /** Destructor **/
   virtual ~CellTableHandler() {
	   delete pSucesor_;
   }
   /** Indica cual es el sucesor en la cadena de captura de eventos **/
   void SetSucesor(CellTableHandler* Sucesor) {
	   pSucesor_ = Sucesor;
   }
   /** Obtiene el handler sucesor **/
   CellTableHandler* GetSucesor() {
	   return pSucesor_;
   }
   /** Metodo que se encarga del manejo un evento de click sobre la celda de una tabla
    * Este metodo fuerza la cadena de responsabilidad para los handlers
    * @return true en caso de poder capturar el evento.
    * @return false en caso contrario
    */
   bool Handle(int RowNumber, int ColNumber, wxGrid* pGrid, Table* pTable,
               KeyDown KeyDown = NONE, MouseEvent MEvent = LCLICK) {
      bool success = DoHandle(RowNumber, ColNumber, pGrid, pTable, KeyDown, MEvent);
      if (!success && pSucesor_)
         return pSucesor_->DoHandle(RowNumber, ColNumber, pGrid, pTable, KeyDown,
                                    MEvent);
      return success;
   }

protected:
   /**
    * Metodo que se encargada de realizar la captura del evento por parte
    * del handler
    * @return true en caso de poder capturar el evento
    * @return false en caso contrario
    */
   virtual bool DoHandle(int rowNumber, int colNumber, wxGrid* pGrid, Table* table,
                         KeyDown KeyDown = NONE, MouseEvent MEvent = LCLICK)=0;
   CellTableHandler* pSucesor_;
};

} /** namespace suri */

#endif /* CELLTABLEHANDLER_H_ */
