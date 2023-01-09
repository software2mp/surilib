/*! \file wxWidgetCellEditor.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXWIDGETCELLEDITOR_H_
#define WXWIDGETCELLEDITOR_H_



// Includes Estandar
// Includes Suri
#include "wxCellEditorWidgetInterface.h"
#include "wxGenericTableBase.h"

// Includes Wx
#include "wx/grid.h"
// Defines
// forwards

namespace suri {

class wxWidgetCellEditor : public wxGridCellEditor {

   /** ctor copia **/
   wxWidgetCellEditor(const wxWidgetCellEditor&);

public:
   wxWidgetCellEditor(wxGenericTableBase* pTable, wxCellEditorWidgetInterface* pEditor);
   virtual ~wxWidgetCellEditor();
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
   wxCellEditorWidgetInterface* pEditor_;
   wxString oldValue_;           /*! Valor viejo */
   wxString newValue_;           /*! Valor nuevo */
   bool endedit_;
};

} /** namespace suri */

#endif /* WXWIDGETCELLEDITOR_H_ */
