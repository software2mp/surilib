/*! \file BrightnessTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef BRIGHTNESSTOOL_H_
#define BRIGHTNESSTOOL_H_

// Includes standard

// Includes Suri
#include "suri/Widget.h"
#include "suri/LayerList.h"
#include "suri/ListView.h"
#include "suri/Tool.h"

// Includes Wx

// Includes App

// Defines

// forwards
class wxScrollEvent;

/** namspace suri */
namespace suri {
/** Macro para declarar el objeto de los eventos */
DECLARE_EVENT_OBJECT(BrightnessEvent);

/** Herramienta de cambio de brillo contraste */
/**
 * Herramienta de contexto que permite modificar el brillo y contraste de un
 * elemento de tipo raster.
 *  Hace uso de las propiedades del RasterRenderer para realizar el cambio en
 * el nodo del elemento seleccionado.
 *
 *  Utiliza una interfaz grafica con sliders para modificar los valores.
 *
 *  El algoritmo utilizado es : DN' = C*DN+B
 * \todo Usar algoritmo de Brillo/Contraste mas riguroso.
 * \note	Hereda de suri::Widget porque posee una interfaz grafica.
 */
class BrightnessTool : public Tool, public Widget {
   /** Ctor. de Copia. */
   BrightnessTool(const BrightnessTool &BrightnessTool);

public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(BrightnessEvent);
   /** Ctor. default. */
   BrightnessTool(wxWindow *pParent = NULL);
   /** Dtor. */
   virtual ~BrightnessTool();
   /** Creacion de la ventana */
   virtual bool CreateToolWindow();
   /** Titulo de la ventana (en caso de pParent!=NULL */
   virtual const wxString GetWindowTitle() const;
   /** Retorna la ventana asociada a la herramienta */
   /**
    * @return Puntero a la ventana asociada a la herramienta o NULL en caso de
    *       de no tener ventana asociada.
    */
   virtual wxWindow *GetToolWindow() const {
      return pToolWindow_;
   }
   /** Metodo virtual que se llama al ejecutar SetList */
   virtual bool DoSetList(LayerList *pList);
   /** Actualizacion de la lista => verificar elemento seleccionado */
   void Update(Model *pModel);
   /** Indica si esta habilitado o no */
   virtual bool IsEnabled();
protected:
   /** Evento de movimiento de sliders */
   void OnSliderScroll(wxScrollEvent &Event);
private:
   EVENT_OBJECT_PROPERTY(BrightnessEvent); /*! Objeto para conectar eventos */
   ListView<BrightnessTool> *pListViewer_; /*! View de la lista */
   CallbackView<BrightnessTool> *pElementViewer_; /*! View para elementos */
};
}  // namspace suri

#endif /*BRIGHTNESSTOOL_H_*/
