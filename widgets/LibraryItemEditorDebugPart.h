/*! \file LibraryItemEditorDebugPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LIBRARYITEMEDITORDEBUGPART_H_
#define LIBRARYITEMEDITORDEBUGPART_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "LibraryItemEditorPart.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
/** Declara la clase TextChangedEvent */
DECLARE_EVENT_OBJECT(TextChangedEvent);
/** Declara la clase ValidDataCheckEvent */
DECLARE_EVENT_OBJECT(ValidDataCheckEvent);
class LibraryItemEditorNotificator;

/*!
 * Clase que representa un editor de un item el cual es de uso para debug.
 * Ademas se tomara como ejemplo de implementacion para desarrollar los editores
 * futuros.
 * Posee una comunicacion bidireccional con el CGU.
 */
class LibraryItemEditorDebugPart : public LibraryItemEditorPart {
public:
   LibraryItemEditorDebugPart(
         const SuriObject::UuidType& LibraryId,
         LibraryManager* pManager,
         const UniversalGraphicalComponentPartInterface::FeatureCombinationEnum& Feature =
               UniversalGraphicalComponentPartInterface::EditionWithFavorites,
         const std::string& WxWindowdId = "ID_LIBRARY_DEBUG_PANEL",
         const std::string& WxCguPanelId = "ID_CGU_PANEL");

   virtual ~LibraryItemEditorDebugPart();
   /** Metodo auxiliar que obtiene el texto del wxtextctrl y genera un item asociado **/
   virtual std::vector<LibraryItemAttribute*>* CreateAttributesFromFields();

private:
   FRIEND_EVENT_OBJECT(TextChangedEvent);FRIEND_EVENT_OBJECT(ValidDataCheckEvent);EVENT_OBJECT_PROPERTY(TextChangedEvent);
   ValidDataCheckEvent* pCheckEventHandler_;
   /**
    *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
    *  Este metodo es el que realmente tienen que implementar las clases hijas para
    * lograr la especializacion a la hora de iniciar el part (metodo que se llama
    * en CreateToolWindow, a modo de Template Method).
    **/
   virtual bool ConfigureFeatures();
   /**
    * Metodo auxiliar privado que es necesario implementar en las clases hijas.
    * Este metodo es el encargado de actualizar realmente la informacion en el part
    * Es llamado en el metodo SetActiveItem a modo de Template Method
    **/
   virtual void UpdateFields(const LibraryItem* pItem, bool Modifiable = true);
   /** Evento UIUpdate */
   void OnUIUpdate(wxUpdateUIEvent &Event);
   /** Evento para el check de valid data **/
   void OnValidDataCheckEvent(wxCommandEvent &Event);
   /** Metodo auxiliar que dado un item muestra sus atributos
    *  en un string
    * @param pItem item que se desea convertir a string
    * @return string con el contenido del item
    */
   std::string ConvertItemToString(const LibraryItem* pItem) const;
   /** item seleccionado **/
   const LibraryItem* pItemSelected_;
};

} /** namespace suri */

#endif /* LIBRARYITEMEDITORDEBUGPART_H_ */
