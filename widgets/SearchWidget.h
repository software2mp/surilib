/*! \file SearchWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SEARCHWIDGET_H_
#define SEARCHWIDGET_H_

// Includes Estandar
#include <string>
// Includes Suri
#include "suri/Widget.h"
#include "suri/SearchFilterApplierInterface.h"
// Includes Wx
#include "wx/event.h"
// Defines
// forwards

namespace suri {

/** Macro para declarar el objeto de los eventos */
DECLARE_EVENT_OBJECT(SearchWidgetEvent);

class SearchWidget : public Widget {
public:
   /** Constructor */
   SearchWidget(SearchFilterApplierInterface* pFilterApplier);
   /** Destructor */
   virtual ~SearchWidget();
   /** Creacion de Ventana  */
   virtual bool CreateToolWindow();
   void SetValue(const std::string Condition);
private:
   EVENT_OBJECT_PROPERTY(SearchWidgetEvent);
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(SearchWidgetEvent);
   /** Metodo que es llamado cuando se produce el evento de click sobre el boton de busqueda. */
   void OnButtonSearch(wxCommandEvent &Event);
   /** Metodo que es llamado cuando se produce el evento de click sobre el boton query builder. */
   void OnButtonQueryBuilder(wxCommandEvent &Event);
   /** Metodo que es llamado cuando se produce el evento de
    * click sobre el boton eliminar filtros */
   void OnButtonDeleteFilters(wxCommandEvent &Event);
   /** Puntero a SearchFilterApplierInterface */
   SearchFilterApplierInterface* pFilterApplier_;
};

} /** namespace suri */

#endif /* SEARCHWIDGET_H_ */
