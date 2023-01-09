/*! \file QueryBuilderPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef QUERYBUILDERPART_H_
#define QUERYBUILDERPART_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/PartContainerWidget.h"
// Includes Wx
#include "wx/event.h"
#include "wx/grid.h"
// Defines

namespace suri {
// forwards
class TablePart;
class SearchWidget;
class Table;

/** Macro para declarar el objeto de los eventos */
DECLARE_EVENT_OBJECT(QueryBuilderPartEvent);

class QueryBuilderPart : public suri::PartContainerWidget {
public:
   typedef enum {
      Greater = 1, Less = 2, Equal = 3, And = 4, Or = 5, NotEqual = 6
   } OperationType;
   /** Constructor */
   QueryBuilderPart(TablePart* pTablePart, SearchWidget* pSearchWidget,
                    std::string LastCondition);
   /** Constructor */
   QueryBuilderPart(Table* pTable,
                    std::string* pCondition);
   /** Destructor */
   virtual ~QueryBuilderPart();
   /** Creacion de Ventana  */
   virtual bool CreateToolWindow();
private:
   EVENT_OBJECT_PROPERTY(QueryBuilderPartEvent);
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(QueryBuilderPartEvent);
   /** Inicializa la ejecuci�n de la consulta */
   virtual void OnButtonOk(wxCommandEvent &Event);
   /** Captura el evento de click sobre el boton con el operador > */
   void OnButtonGreaterOperation(wxCommandEvent &Event);
   /** Captura el evento de click sobre el boton con el operador < */
   void OnButtonLessOperation(wxCommandEvent &Event);
   /** Captura el evento de click sobre el boton con el operador = */
   void OnButtonEqualOperation(wxCommandEvent &Event);
   /** Captura el evento de click sobre el boton con el operador != */
   void OnButtonNotEqualOperation(wxCommandEvent &Event);
   /** Captura el evento de click sobre el boton con el operador AND */
   void OnButtonAndOperation(wxCommandEvent &Event);
   /** Captura el evento de click sobre el boton con el operador OR */
   void OnButtonOrOperation(wxCommandEvent &Event);
   /** Captura el evento de doble click sobre la grilla */
   void OnCellDoubleClick(wxGridEvent &Event);
   /** Captura el evento de click sobre el boton Agregar columna */
   void OnButtonAppendColumnName(wxCommandEvent &Event);
   /** Agrega un string al final del TextCtrl */
   void AppendString(OperationType operationType);
   /** Agrega un string al final del TextCtrl */
   void AppendColumnName(std::string ColumnName);
   TablePart* pTablePart_; /* Puntero a Table */
   SearchWidget* pSearchWidget_; /* Puntero a SearchWidget */
   std::string lastCondition_; /** Guarda la ultima condicion ejecutada */
   std::string* pCondition_;
   Table* pTable_;
};

} /** namespace suri */

#endif /* QUERYBUILDERPART_H_ */
