/*! \file wxGridColorEditor.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXGRIDCOLOREDITOR_H_
#define WXGRIDCOLOREDITOR_H_

// Includes Estandar

// Includes Suri
#include "wxGenericTableBase.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/grid.h"

// Defines

// forwards

namespace suri {

/**
 * Extension de wxGridCellEditor para permitir elegir colores con wxColorDialog
 * \attention En caso de inconvenientes de foco, evaluar el uso de un Combo Box con colores
 */
class wxGridColorCellEditor : public wxGridCellEditor {
public:
   /** Constructor */
   explicit wxGridColorCellEditor(wxGenericTableBase* pTable);
   /** Destructor */
   virtual ~wxGridColorCellEditor();
   /** Lo que se efectua cuando comienza la edicion de la celda */
   virtual void BeginEdit(int Row, int Column, wxGrid* pGrid);
   /** Creacion del objeto actual (es requerido por wxwidgets para inicializar el objeto) */
   virtual void Create(wxWindow* pParent, wxWindowID Id, wxEvtHandler* pEventHandler);
   /** Lo que se efectua cuando finaliza la edicion de la celda */
   virtual bool EndEdit(int Row, int Column, wxGrid* pGrid);
   /** Reseteo del valor original de la celda */
   virtual void Reset();
   /** Clonacion del objeto actual */
   virtual wxGridCellEditor* Clone() const;
   /** Obtencion de valor de la celda */
   virtual wxString GetValue() const;

private:
   wxGenericTableBase* pTable_;  /*! Puntero a la tabla */
   wxString oldValue_;           /*! Valor viejo */
   wxString newValue_;           /*! Valor nuevo */
};

} /** namespace suri */

#endif /* WXGRIDCOLOREDITOR_H_ */
