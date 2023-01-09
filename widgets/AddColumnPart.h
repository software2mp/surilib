/*! \file AddColumnPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ADDCOLUMNPART_H_
#define ADDCOLUMNPART_H_

// Includes Estandar
// Includes Suri
#include "suri/DialogWidget.h"
#include "suri/Table.h"
// Includes Wx
#include "wx/event.h"
// Defines

namespace suri {

// forwards
class TablePart;
class TableTool;

/** Macro para declarar el objeto de los eventos */
DECLARE_EVENT_OBJECT(AddColumnPartEvent);

class AddColumnPart : public DialogWidget {
public:
   /** Constructor */
   AddColumnPart(TableTool* pTableTool, TablePart* pTablePart);
   /** Destructor */
   virtual ~AddColumnPart();
   /** Creacion de Ventana  */
   virtual bool CreateToolWindow();
private:
   EVENT_OBJECT_PROPERTY(AddColumnPartEvent);
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(AddColumnPartEvent);
   /** Accion de verificar la ecuacion ingresada en el TextCtrl */
   void OnButtonVerifyEquation(wxCommandEvent &Event);
   /** Accion de cambiar la opcion del choice */
   void OnChoiceSelection(wxCommandEvent &Event);
   /** Inicializa los tipos de dato de wxChoice */
   void SetChoiceDataTypes();
   /** Inserta la nueva columna */
   void OnButtonOk(wxCommandEvent &Event);
   /** Evento click sobre Boton Cancel */
   void OnButtonCancel(wxCommandEvent &Event);
   /** Agrega una columna a la tabla */
   void AddColumn();
   /** Agrega una columna calculada a la tabla */
   void AddCalculatedColumn();
   /** Muestra un mensaje informando el estado del parseo de la ecuacion */
   void ShowStatusMessage(bool success);
   /** Obtiene el icono del part */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
   /** Puntero a TableTool */
   TableTool* pTableTool_;
   /** Puntero a TablePart */
   TablePart* pTablePart_;
   /** Nombre de la columna */
   wxString columnname_;
   /** Tipo de columna */
   Table::ColumnType type_;
   /** Retorna la validez del nombre */
   bool validname_;
   /** Retorna los permisos de append */
   bool append_;
   /** Retorna los permisos de insert */
   bool insert_;
};

} /** namespace suri */

#endif /* ADDCOLUMNPART_H_ */
