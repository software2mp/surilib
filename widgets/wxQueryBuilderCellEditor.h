/*! \file wxQueryBuilderCellEditor.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXQUERYBUILDERCELLEDITOR_H_
#define WXQUERYBUILDERCELLEDITOR_H_

// Includes Estandar
// Includes Suri
#include "wxCellEditorWidgetInterface.h"
// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class QueryBuilderPart;
class Table;
class DatasourceInterface;

class wxQueryBuilderCellEditor : public suri::wxCellEditorWidgetInterface {
   suri::DatasourceInterface* pdatasource;

public:
   wxQueryBuilderCellEditor(const std::string& VectorUrl);
   virtual ~wxQueryBuilderCellEditor();
   /** Metodo virtual que inicia la edicion del atributo de la celda **/
   virtual bool BeginEdition();
   /** Obtiene el valor resultante de la edicion de la grilla */
   virtual std::string GetValue();
   /** Configura el valor a editar **/
   virtual void SetValue(const std::string& Value);

protected:
   Table* pTable_;
   QueryBuilderPart* pQueryEditor_;
   std::string currentValue_;
   DatasourceInterface* pDatasource_;
};

} /** namespace suri */

#endif /* WXQUERYBUILDERCELLEDITOR_H_ */
