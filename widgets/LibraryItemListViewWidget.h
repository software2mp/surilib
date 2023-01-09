/*! \file LibraryItemListViewWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LIBRARYITEMLISTVIEWWIDGET_H_
#define LIBRARYITEMLISTVIEWWIDGET_H_

// Includes Estandar
#include <string>
#include <vector>
#include <map>
#include <utility>

// Includes Suri
#include "suri/Widget.h"
#include "suri/SuriObject.h"
#include "LibraryItemListNotificator.h"
#include "suri/LibraryClient.h"

// Includes Wx
#include "wx/treectrl.h"

// Defines
// forwards

namespace suri {

DECLARE_EVENT_OBJECT(TreeSelectionChangeEvent);

/*!
 * Clase que encapsula el comportamiento de la representacion visual de una biblioteca.
 */
class LibraryItemListViewWidget : public suri::Widget {
   /** ctor copia **/
   LibraryItemListViewWidget(const LibraryItemListViewWidget&);

public:
   /** Enum que brinda las opciones de agrupamiento de la lista **/
   typedef enum {
      ORIGIN, ALPHABETICAL
   } GroupByType;
   /** Constructor **/
   LibraryItemListViewWidget(LibraryManager* pManager,
                             LibraryItemListNotificator* pNotificator,
                             const std::string& LibraryCode,
                             const GroupByType& GroupBy = ORIGIN);
   /** dtor **/
   virtual ~LibraryItemListViewWidget();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
   /** Configura los valores del wxTree a partir de el contenido de la biblioteca */
   void SetInitialValues();
   /** Agrega un item a la lista*/
   void AddItem(const SuriObject::UuidType& OriginId, const LibraryItem* pItem);
   /** Remueve un item de la lista */
   void RemoveActiveItem();
   /** Obtiene el id del origen seleccionado en el arbol.
    * @return id del origen seleccionado
    * @return SuriObject::NullUuid en caso de que la seleccion no sea un origen
    */
   SuriObject::UuidType GetOriginIdSelected() const;

private:
   FRIEND_EVENT_OBJECT(TreeSelectionChangeEvent);
   EVENT_OBJECT_PROPERTY(TreeSelectionChangeEvent);
   /** Metodo auxiliar que captura el evento de cambio de seleccion **/
   void OnItemSelectionChange(wxTreeEvent& Event);
   /** Metodo encargado de actualizar el editor asociado al item seleccionado */
   void UpdateEditorPart(const std::string& ItemSelected);
   /** Devuelve el nombre asociado a la categoria de origen **/
   std::string GetCategoryName(LibraryItemOrigin::OriginCategoryType Category) const;
   /**
    * Metodo que verifica que el id pasado por parametro pertenezca a un item de biblioteca
    * valido. Se verifica que el padre del item corresponda a un origen valido
    * Precondicion: pTreeCtrl_ es != NULL
    * @param[in] ItemId id del item
    * @return true en caso de que corresponda a un item, false en caso contrario
    */
   bool IsLibraryItem(wxTreeItemId& ItemId);
   /** Client de biblioteca que se encarga de manejar la comunicacion con la biblioteca*/
   LibraryClient client_;
   /** Manager de biblioteca **/
   LibraryManager* pManager_;
   /** Id de la biblioteca que debe mostrar **/
   SuriObject::UuidType libraryCode_;
   LibraryItemListNotificator* pNotificator_;
   wxTreeCtrl* pTreeCtrl_;
   /** bool que indica que se ha removido un item*/
   bool itemRemove_;
   /** vector con los ids del arbol que corresponden a los origenes que se cargan*/
   typedef std::vector<wxTreeItemId> ItemIdVector;
   ItemIdVector originItemIds_;
   typedef std::map<SuriObject::UuidType, wxTreeItemId> OriginItemAssociationMap;
   OriginItemAssociationMap originMap_;
   /** Referencia a la bilbioteca que se encuentra desplegada **/
   const Library* pLibrary_;
};

} /** namespace suri */

#endif /* LIBRARYITEMLISTVIEWWIDGET_H_ */
