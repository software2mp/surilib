/*! \file CellTableHandler.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
