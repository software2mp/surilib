/*! \file PartContainerWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef PARTCONTAINERWIDGET_H_
#define PARTCONTAINERWIDGET_H_

// Includes standard
#include <list>

// Includes suri
#include "suri/messages.h"
#include "DialogWidget.h"
#include "Part.h"

// Includes wx
#include "wx/wx.h"
#include "wx/notebook.h"
#include "wx/listbook.h"
#include "wx/toolbook.h"
#include "wx/choicebk.h"
#include "wx/treebook.h"

// Defines
/** Flag para indicar que no se agregue ningun boton */
#define SUR_BTN_NONE            0x000000
/** Flag para indicar que se agrega el boton OK */
#define SUR_BTN_OK              0x000001
/** Flag para indicar que se agrega el boton Cancel */
#define SUR_BTN_CANCEL          0x000010
/** Flag para indicar que se agrega el boton Apply */
#define SUR_BTN_APPLY           0x000100
/** Flag para indicar que se agrega el boton Forward */
#define SUR_BTN_FORWARD         0x001000
/** Flag para indicar que se agrega el boton Backward */
#define SUR_BTN_BACKWARD        0x010000
/** Flag para indicar que se agregan todos los botones */
#define SUR_BTN_ALL             0x011111
/** Flag para indicar que se agregan todos los botones, salvo Apply*/
#define SUR_BTN_ALL_NO_APPLY    0x011011

/** namespace suri */
namespace suri {
/** Declara la clase PartContainerWidgetEvent */
DECLARE_EVENT_OBJECT(PartContainerWidgetEvent);

/** DialogWidget que contiene un conjunto de Parts y botones para manerjarlos */
/**
 * Contiene una lista de parts y permite hacer comits, rollbacks y consultas
 * en cada uno o en todos al mismo tiempo.
 * Ademas tiene botones ok, cancel, apply, backward, fordward que pueden
 * mostrarse o no, segun se indique mediante parametro.
 *  El primer constructor no crea la ventana en forma automatica y depende de
 * la clase derivada para agregar las ventanas de las partes conforme se van
 * agregando.
 *  El segundo constructor se utiliza para crear una ventana con una unica parte.
 * Instala la ventana de la parte en el lugar de control principal.
 * \note hereda de DialogWidget para mostrar los widget de configuracion en dialog
 */
class PartContainerWidget : public DialogWidget {
   /** Ctor. de Copia. */
   PartContainerWidget(const PartContainerWidget &PartContainerWidget);

public:
   /** Declara friend la clase de evento PartContainerWidgetEvent */
   FRIEND_EVENT_OBJECT(PartContainerWidgetEvent);

   /** Ctor. default. */
   PartContainerWidget(const wxString &Title = _(caption_PART_CONTAINER),
                       const long VisibleButtons = SUR_BTN_ALL,
                       const long EnabledButtons = SUR_BTN_ALL);
   /** Ctor. con un solo Part. */
   PartContainerWidget(Part *pUniquePart,
                       const wxString &Title = _(caption_PART_CONTAINER),
                       const long VisibleButtons = SUR_BTN_OK | SUR_BTN_CANCEL,
                       const long EnabledButtons = SUR_BTN_CANCEL);
   /** Dtor. */
   virtual ~PartContainerWidget();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
   /** Crea el panel principal que se asigna a pToolWindow */
   virtual wxWindow *GetToolWindow();
   /** Agrega el control que contendra a los Part's. */
   bool AddMainControl(wxWindow *pControlWindow);
   /** Agrega el control que contendra a los Part's. */
   bool AddMainControl(Widget *pTool);
   /** Inserta un Part al final. */
   bool InsertPart(suri::Part *pPart);
   /** Elimina el Part. */
   bool RemovePart(suri::Part *pPart);
   /** Devuelve cuantos Part tiene el Container. */
   int CountParts();
   /** Retorna la ventana padre de las partes. */
   virtual wxWindow *GetPartParent();
   /** Retorna la el icono de la ventana */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
   /** Devuelve el Part con el indice indicado. */
   Part *GetPart(const int &IndexPart);
   /** Devuelve el indice del Part dentro de la lista. */
   int GetPartIndex(const suri::Part *pPart);
   /** Aplica los cambios en los Part. */
   virtual bool ApplyChanges();
   /** Aplica los cambios de un Part en particular. */
   virtual bool ApplyChanges(const int &IndexPart);
   /** Descarta los cambios en los Part. */
   virtual bool DiscardChanges();
   /** Descarta los cambios de un Part en particular. */
   virtual bool DiscardChanges(const int &IndexPart);
   /** Evento click sobre Boton Ok */
   virtual void OnButtonOk(wxCommandEvent &Event);
   /** Evento click sobre Boton Aplicar */
   virtual void OnButtonApply(wxCommandEvent &Event);
   /** Evento click sobre Boton Cancel */
   virtual void OnButtonCancel(wxCommandEvent &Event);
   /** Evento click sobre Boton BackWard */
   virtual void OnButtonBackward(wxCommandEvent &Event);
   /** Evento click sobre Boton ForWard */
   virtual void OnButtonForward(wxCommandEvent &Event);
   /** Evento UIUpdate */
   virtual void OnUIUpdate(wxUpdateUIEvent &Event);
   /** Evento Close */
   virtual void OnClose(wxCloseEvent &Event);
   /** Evento click sobre Boton de ayuda */
   virtual void OnHelpButton(wxCommandEvent &Event);
   /** Decide que botones mostrar segun visibleButtons_ */
   void ShowButtons();

protected:
   /** Verifica si algun Part fue modificado. */
   virtual bool PartsChanged();
   /** Verifica si un Part fue modificado. */
   bool PartsChanged(const int &IndexPart);
   /** Implementar en Clases derivadas para agregar Part al Container. */
   virtual bool DoInsert(suri::Part *pPart);
   /** Implementar en Clases derivadas para eliminar Part del Container. */
   virtual bool DoRemove(suri::Part *pPart);
   /** Verifica que todas las partes tengan datos validos */
   bool PartsValidData();
   /** Metodo que habilita las partes */
   virtual void EnableWindows();

   /*TODO(28/10/2008 - Alejandro): Ver si conviene cambiar por:
    *          std::map <int, Part *> pPartList_*/
   std::list<Part *> partList_; /*! Lista con los Part del Container. */
#ifdef __UNUSED_CODE__
   bool enableOk_; /*! para mantener habilitado el boton de ok siempre. */
#endif
   long visibleButtons_; /*! flag que determina que botones estan visibles */
   long enabledButtons_; /*! flag que determina que botones estan activos */
private:
   EVENT_OBJECT_PROPERTY(PartContainerWidgetEvent); /*! Objeto para conectar */

   /**
    * Verifica si el usuario quiere salir.
    * (boton de cerrar de la ventana y boton cancelar)
    */
   void VerifyExit(wxEvent& Event);
};
}  // namespace suri

#endif /*PARTCONTAINERWIDGET_H_*/

