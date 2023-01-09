/*! \file FullFledgeLibraryEditorWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <string>

// Includes Suri
// Includes Wx
// Defines
#ifndef FullFledgeLibraryEditorWidget_H_
#define FullFledgeLibraryEditorWidget_H_

#include "suri/PartContainerWidget.h"
#include "suri/Part.h"
#include "suri/DialogWidget.h"
#include "LibraryItemListViewWidget.h"
#include "LibraryItemEditorPart.h"

namespace suri {

/** Declara la clase Notebook WidgetEvent */
DECLARE_EVENT_OBJECT(FullFledgeLibraryEditorWidgetEvent);

/**
 * Widget del editor de bibliotecas.
 * Posee un selector de items, para seleccionar el item
 * a editar.
 * Tiene un panel en el cual se ubica el editor especifico
 * para el tipo de item que se este deseando editar.
 *
 */
class FullFledgeLibraryEditorWidget : public suri::PartContainerWidget {
   /** Ctor. de Copia. */
   FullFledgeLibraryEditorWidget(const FullFledgeLibraryEditorWidget &);

public:
   /** Constructor **/
   FullFledgeLibraryEditorWidget(LibraryManager* pManager,
                                 const std::string& LibraryCode, 
											const std::string& WindowTitle = caption_LIBRARY);
   /** Dtor. */
   virtual ~FullFledgeLibraryEditorWidget();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
   /** Retorna la ventana padre de las partes. */
   virtual wxWindow *GetPartParent();
   /** Metodo encargado de actualizar el editor asociado al item seleccionado */
   void UpdateEditor(const LibraryItem* pItem);

private:
   /** Declara friend la clase de evento NotebookWidgetEvent */
   FRIEND_EVENT_OBJECT(FullFledgeLibraryEditorWidgetEvent);
   EVENT_OBJECT_PROPERTY(FullFledgeLibraryEditorWidgetEvent); /*! Objeto para conectar eventos */
   /** Metodo que se llama cuando se presiona el boton de agregar*/
   void OnAddClick(wxCommandEvent& Event);
   /** Metodo que se llama cuando se presiona el boton de eliminar*/
   void OnRemoveClick(wxCommandEvent& Event);
   /** Agrega Part al Notebook. */
   virtual bool DoInsert(suri::Part *pPart);
   /** Elimina Part del Notebook. */
   virtual bool DoRemove(suri::Part *pPart);
   /** Metodo encargado de verficar los cambios sobre el item que se pasa por parametro**/
   void CommitChangesFromActiveItem();
   /** Instancia del responsable de las notificaciones entre editor full-fledge
    *  y selector de items */
   LibraryItemListNotificator notificator_;
   /** Codigo de biblioteca que manejara el editor full fledge **/
   std::string libraryCode_;
   /** Widget activo para seleccion de items de biblioteca*/
   LibraryItemListViewWidget* pItemSelector_;
   /** Referencia al manager de biblioteca activo **/
   LibraryManager* pManager_;
   /** Editor de items en uso **/
   LibraryItemEditorPart* pCurrentEditor_;
   /** bool que indica que se ha elimiando un item recientemente */
   bool itemRemoved_;
   /** referencia a la biblioteca que se encuentra desplegando el editor **/
   const Library* pLibrary_;
};
}

#endif /* FullFledgeLibraryEditorWidget_H_ */
