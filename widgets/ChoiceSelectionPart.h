/*! \file ChoiceSelectionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CHOICESELECTIONPART_H_
#define CHOICESELECTIONPART_H_

// Includes standar
#include <map>

// Includes suri
#include "suri/Part.h"
#include "suri/RasterElement.h"

/** namespace suri */
namespace suri {

// Forwards
class RasterLayer;

/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(ChoiceSelectionPartEvent);

/** Permite seleccionar un valor en un Choice, activar/desactivar y un boton refresh oculto. */
/**
 * Part que recibe un elemento raster y permite seleccionar de una lista, al .
 * realizar commit se utiliza el valor seleccionado para modificar el nodo que
 * corresponda del elemento.
 */
class ChoiceSelectionPart : public Part {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(ChoiceSelectionPartEvent);
   /** Ctor. legacy */
   explicit ChoiceSelectionPart(RasterElement * pElement);
   /** Ctor. */
   explicit ChoiceSelectionPart(RasterLayer * pRasterLayer);
   /** Dtor **/
   virtual ~ChoiceSelectionPart();
   /** Crea la ventana de la parte */
   virtual bool CreateToolWindow();
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Devuelve si hubo cambios */
   virtual bool HasChanged();
   /** Aplica los cambios al elemento */
   virtual bool CommitChanges();
   /** Carga los datos del elemento al control wx */
   virtual bool RollbackChanges();
   /** Carga los valores iniciales */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Des/habilita la ventana de la herramienta */
   virtual void SetEnabled(bool State);
   /** Retorna la posicion del item seleccionado en el choice */
   int GetSelectedItem();
   /** Actualiza el contenido del choice usando los atributos de la clase. */
   void ConfigureChoiceList();
   /** Modifica estado de activacion del checkbox */
   void ActivateChoice(bool State);
   /** Des/habilita la lista de realces y el boton refrescar(si esta desplegado) */
   void ActivateControls(bool State);
   /** Informa si el checkbox dentro del part esta activo. */
   bool IsChoiceActive();
   /** Modifica el estado de refresh_ a true y llama a updata */
   void Refresh();
protected:
   /**
    * Configura el contenido de choiceContent_ y selecciona el item que
    * corresponda al estado actual del elemento
    */
   virtual bool ResetList()=0;
   /**
    * Modifica/Crea/Elimina el nodo del elemento cuyo contenido representa el
    * choice, con la opcion que selecciono el usuario.
    */
   virtual bool ConfigureElement()=0;

   /** Modifica el estado de activacion de los controles del part(ActivateControls) */
   void OnCheck(wxCommandEvent &Event);
   /** Modifica el estado de refresh de la herramienta */
   void OnListSelection();
   /** Funcion que se llama en update y permite modificar comportamiento */
   virtual void OnUpdate() {
   }

   RasterElement* pElement_; /*! Puntero al elemento */
   RasterLayer* pRasterLayer_; /*! Puntero a la capa */
   std::vector<wxString> choiceContent_; /*! Contenido del choice(traducido) */
   bool isActive_; /*! Estado del item en el elemento */
   int selectedItem_; /*! Posicion del item configurado en el elemento. */
   bool refresh_; /*! Le indica al choice que se desea refrescar. */
private:
   EVENT_OBJECT_PROPERTY(ChoiceSelectionPartEvent); /*! Objeto que conecta */
   /* los eventos */
};
}

#endif /* CHOICESELECTIONPART_H_ */
