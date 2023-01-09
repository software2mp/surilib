/*! \file AnimationTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ANIMATIONTOOL_H_
#define ANIMATIONTOOL_H_

// Includes standard
#include <map>
#include <string>
#include <vector>
#include <utility>
#include <list>
// Includes suri
#include "suri/Widget.h"
#include "ViewportInterface.h"
// Includes Wx

// Includes app
#include "Tool.h"

// Defines

// Forwards wx
class SwapToggleButton;
class wxCommandEvent;
class wxUpdateUIEvent;
class wxTimer;
class wxWindowDisabler;

/** namespace suri */
namespace suri {
// Forwards suri
class LayerList;
class Element;
class DataViewManager;
class NodePath;
class HtmlTreeWidget;

/** Macro para declarar el objeto de los eventos */
DECLARE_EVENT_OBJECT(AnimationEvent);

/** Clase Herramienta para animacion de elementos. */
/**
 * Esta clase es una herramienta que permite animar una serie de elementos
 * seleccionados de la lista.
 *
 * Los elementos deben ser del mismo grupo y cumplir una serie
 * de restricciones para poder ser desplegados sin modificar la referencia
 * espacial del mundo.
 */
class AnimationTool : public Tool, public Widget {
   /** Ctor. Copia */
   AnimationTool(const AnimationTool &AnimationTool);

public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(AnimationEvent);
   /** Ctor. */
   AnimationTool(wxWindow *pListWindow, wxWindow *pContextWindow, int ListGroup = 0,
                 wxWindow *pParent = NULL, DataViewManager* pDataViewManager = NULL,
                 HtmlTreeWidget* pTreeWidget = NULL);
   /** Dtor. */
   virtual ~AnimationTool();

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
      return (const_cast<AnimationTool*>(this))->GetWindow();
   }
   /** Metodo virtual para indicar que la herramienta se ha des/activado */
   virtual bool DoSetActive(bool Active);
   /** Asigna la lista de elementos asociada a la herramienta */
   virtual bool DoSetViewer(ViewerWidget *pViewer);
   /** Indica si esta habilitado o no */
   virtual bool IsEnabled();
   /** Metodo que realiza una avanza hacia atras un frame */
   void BackwardAnimation();
   /** Metodo que realiza una avanza hacia adelante un frame */
   void ForwardAnimation();
   /** Metodo que detiene la animacion en curso */
   void StopAnimation();
   /** Metodo que inicia una animacion */
   void StartAnimation();
   /** Indica la velocidad de la animacion    */
   void SetFrameRate(int Rate);

protected:
   /** Metodo para mostrar el elemento actualmente seleccionado */
   void ShowCurrentElement();
   /** Evento UIUpdate */
   void OnUIUpdate(wxUpdateUIEvent &Event);
   /** Evento OnTimer que cambia la el elemento activo */
   void OnTimer();
// ------------------------ EVENTOS DE LOS BOTONES -------------------------
   /** Metodo que recibe evento de boton Backward */
   void OnBackwardsButtonClick(wxCommandEvent &Event);
   /** Metodo que recibe evento de boton Stop */
   void OnStopButtonClick(wxCommandEvent &Event);
   /** Metodo que recibe evento de boton Play/Pause */
   void OnStartPauseButtonClick(wxCommandEvent &Event);
   /** Metodo que recibe evento de boton Forward */
   void OnForwardButtonClick(wxCommandEvent &Event);
   /** Metodo que recibe evento de boton Swapper */
   void OnSwapperButtonClick(wxCommandEvent &Event);
   virtual void EnableToggle(bool Enabled = true);
   /** Metodo para des/habilitar los controles peligrosos */
   virtual void EnableControls(bool Enabled = true);
   /** Metodos que manejan el intercambio entre las imagenes de los visualizadores**/
   virtual void OnRightMouseButtonClick(wxCommandEvent& event);
   virtual void OnLeftMouseButtonClick(wxCommandEvent& event);
// --------------------------- HELPERS DE LA LISTA --------------------------
   /** Obtiene el proximo elemento seleccionado a partir de currentElement_ */
   Element *GetNextSelected();
   /** Obtiene el anterior elemento seleccionado a partir de currentElement_ */
   Element *GetPrevSelected();
   /** Obtiene el elemento activo seleccionado o NULL si no hay activo */
   Element* GetActiveSelected();
   /** determina si hay un elemento activo que no este seleccionado */
   bool HasActiveNotSelected();
   /** determina si hay al menos un elemento seleccionado con SR != al primero */
   bool IsIncompatibleSelected();
   /** Actualiza la lista de imagenes seleccionadas **/
   void UpdateLayerList();
   void Swap(int id, bool clockwise);
   std::vector< std::vector<std::string> > GetActiveLayersIdFromViewPorts();
   std::vector<std::string> GetActiveRasterIdFromViewPorts();
   /**Obtiene los ids de los viewPorts activos**/
   std::vector<ViewportInterface*> GetActiveViewPorts();
   std::vector<std::string> GetActiveViewPortsIds();
   void OriginalStateUpdate();
private:

   EVENT_OBJECT_PROPERTY(AnimationEvent); /*! Objeto para conectar eventos */
   wxTimer *pAnimationTimer_; /*! Timer utilizado para el evento OnSize */
   int currentElement_; /*! indice del elemento actualmente seleccionado */
   int animatedGroup_; /*! grupo de elementos que se debe animar 0 = todos */
   Element *pCurrentElement_; /*! ultimo elemento seleccionado */
   bool isStillRendering_; /*! Flag para que el timer no venza 2 vece mientras */
   /* renderiza */
   int framesPerSecond_; /*! Velocidad de animacion */
   bool shouldStop_; /*! flag que indica si el timer debe detenerse (en OnTimer) */
   DataViewManager* pDataViewManager_;
   std::list<suri::NodePath> selectednodes_;
   /** listado de capas **/
   HtmlTreeWidget* pTreeWidget_;
   std::vector<ViewportInterface*> Views_;
   bool activeSwap_;
   /// id del widget, par (posicion viewport en Views_, view actual)
   std::map<int, std::pair<int, int> > swapState_;
   std::vector< std::string > originalState_;
   std::vector< std::string > lastState_;
   std::vector<wxWindow*> controls_;
   wxBitmapButton* ptoggle_;
};
}  // namespace suri

#endif /* ANIMATIONTOOL_H_ */
