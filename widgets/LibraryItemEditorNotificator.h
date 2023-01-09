/*! \file LibraryItemEditorNotificator.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LIBRARYITEMEDITORNOTIFICATOR_H_
#define LIBRARYITEMEDITORNOTIFICATOR_H_

// Includes Estandar
#include <vector>
#include <string>

// Includes Suri
#include "LibraryItemEditorPart.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Clase que se encarga de realizar la comunicacion bidireccional
 * entre el Componente Grafico universal y
 * el Editor de biblioteca
 */
class LibraryItemEditorNotificator {
public:
   /** ctor **/
   LibraryItemEditorNotificator(LibraryItemEditorPart* pEditor = NULL,
                                UniversalGraphicalComponentPartInterface* pCgu = NULL);
   /** dtor **/
   virtual ~LibraryItemEditorNotificator();
   /** Notifica que se selecciono un nuevo item en el CGU.
    *  Si el item pasado por parametro es NULL significa que
    *  no se encuentra seleccionado ningun item **/
   void NotifyItemSelectionFromCgu(const LibraryItem* pItem = NULL,
                                   bool FirstSelection = false, bool Modifiable = true);
   /** Notifica que se selecciono un item desde el editor, indicandole el
    *  atributo principal. En caso de que no exista el item en el CGU
    *  Es resposabilidad del CGU tratarlo**/
   void NotifyItemSelectionFromEditor(const std::string& PrincipalAttribute);
   /** Notifica al cgu que el editor ha cambiado informacion del item, notificando
    * ademas la validez de la informacion*/
   void NotifyItemInformationChangedFromEditor(bool IsValidData);
   /** Indica la referencia al editor in situ*/
   void SetLibraryItemEditorDebugPart(LibraryItemEditorPart* pEditor);
   /** Indica la referencia al cgu*/
   void SetUniversalGraphicalComponentPart(UniversalGraphicalComponentPartInterface* pCgu);
   /** Obtiene los atributos modificados desde el editor
    *  @return vector con atributos
    *  @return NULL en caso de que no se haya modificado ningun atributo */
   std::vector<LibraryItemAttribute*>* GetAttributesModifiedFromEditor();

   /** Obtiene los atributos eliminados. */
   std::vector<LibraryItemAttribute*>* GetAttributesRemovedFromEditor();

   /** Metodo que consulta la validez de informacion del part del editor. **/
   bool AskEditorForValidData() const;
   /** Metodo que consulta el estado del check del cgu **/
   bool AskCguForCheckState() const;

private:
   LibraryItemEditorPart* pEditor_;
   UniversalGraphicalComponentPartInterface* pCgu_;
};

} /** namespace suri */

#endif /* LIBRARYITEMEDITORNOTIFICATOR_H_ */
