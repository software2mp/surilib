/*! \file TerrainGLCanvas.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef _TERRAINGLCANVAS_H_
#define _TERRAINGLCANVAS_H_

// Includes standard
#include <vector>

// Includes suri
// \TODO(Javier - Sin TCK): Arreglar el orden de inclusion para Terrain.h que genera warnings
// si se incluye despues de los includes wx
#include "Terrain.h"
#include "suri/Coordinates.h"
#include "suri/World.h"
#include "Camera.h"

// Includes wx
#include "wx/glcanvas.h"
#include "wx/event.h"

/** namespace suri */
namespace suri {
/** Clase para renderizar los graficos de OpenGL */
/*
 * Esta clase es una ventana que hereda de wxGLCanvas, permite renderizar los graficos
 * en 3D utilizando OpenGL y responder a los eventos.
 * Se encarga de:
 *    _ renderizar el terreno.
 *    _ responder a los eventos que suceden sobre dicha ventana(mouse, teclado,
 * pintado, etc.)
 */
class TerrainGLCanvas : public wxGLCanvas {
public:
   /** Tipo de accion de los botones del Contexto 3D */
   typedef enum {
      SphericCam, EyeCam, Query
   } ToggleButtonActionType;

   /** ctor. */
   explicit TerrainGLCanvas(wxWindow *pParentWindow);
   /** dtor. */
   virtual ~TerrainGLCanvas();
   /** resetea propiedades */
   void ResetCanvas();
   /** resetea la camara */
   void ResetCamera();
   /** establece el terreno. */
   void SetTerrain(Terrain *pTerrain);
   /** devuelve el terreno. */
   Terrain* GetTerrain() {
      return pTerrain_;
   }
   /** establece el color de fondo. */
   void SetBackGroundColour(float R, float G, float B);
   /** devuelve el color de fondo. */
   void GetBackGroundColour(int& R, int& G, int& B);
   /** establece el paso de la camara. */
   void SetStep(float N);
   /** Modifica estado listo(ready_) */
   void SetReady(bool Ready) {
      ready_ = Ready;
   }
   /** Retorna el Buffer frontal */
   void GetBuffer(unsigned char* pBuffer, int Width, int Height);
   /** evento OnSize */
   void OnSize(wxSizeEvent& Event);
   /** evento OnPaint */
   void OnPaint(wxPaintEvent& Event);
#ifdef __UNUSED_CODE__
   /** Procesa eventos cuando entra el mouse en la ventana. */
   void OnEnterWindow(wxMouseEvent& Event);
#endif
   /** Procesa eventos cuando sale el mouse en la ventana. */
   void OnLeaveWindow(wxMouseEvent& Event);
   /** Procesa eventos de mouse en la ventana. */
   /** Procesa eventos de teclado sobre la ventana. */
   void OnMouse(wxMouseEvent& Event);
   void OnKeyboard(wxKeyEvent& Event);
   /** Evento que vuelve a pintar el fondo de la pantalla */
   void OnEraseBackground(wxEraseEvent& WXUNUSED(Event));
   /** Funcion de Renderizado */
   void Render();
   /** Genera perspectica */
   void SetPerspective();
   /** Setea frustum en funcian del tamanio del terreno */
   void SetFrustum();
   /** Inicializa terreno */
   void InitTerrain();
   /** Inicializacion de vectores de puntos */
   bool InitPointVector(suri::Element *pElement);
   /** Incializa variables de OpenGL */
   void InitOpenGL();
   /** Inicializa camara */
   void InitCamera(float Rho=-1, float Phi=-1, float Theta=-1);
   /** Devuelve accion seleccionada. */
   ToggleButtonActionType GetAction() {
      return action_;
   }
   /** Establece accion seleccionada. */
   void SetAction(ToggleButtonActionType Action) {
      action_ = Action;
   }
   /** Seleccion de puntos en la pantalla */
   bool Selection(size_t X, size_t Y);
   Coordinates GetPosition(size_t X, size_t Y);
   Camera * GetCamera() {return pCamera_;}
   void SetWorld(World* pWorld) {pWorld_ = pWorld;}
   bool GetUpdate() {return update_;}
   void SetUpdate(bool Value) {update_ = Value;}
   void DrawCross(wxPaintEvent &Event);
private:
   bool refreshWorld_;
   bool windowChanged_;
   Subset bufferedWindow_;
   Subset initialWindow_;
   void ZoomOut(int X, int Y, double Factor);
   void ZoomIn(int X, int Y, double Factor);
   void TranslateZ(double Z);
   void TranslateXY(double X, double Y);
   void RefreshWorld();
   double radius_;
   World * pWorld_;
   bool update_;
// Flag de inicializacion
   bool ready_; /*! flag que indica si hay un terreno disponible para */
   /* renderizar. */
   Camera *pCamera_; /*! camara para visualizar el terreno.(modifica al */
   /* recibir eventos sobre el canvas) */
   Terrain *pTerrain_; /*! terreno a renderizar. */
   int mousePosX_; /*! Guarda la pos X del mouse */
   int mousePosY_; /*! Guarda la pos Y del mouse */
   double zNear_; /*! Z-Near para determinacion del Frustum */
   double zFar_; /*! Z-Far para determinacion del Frustum */
   std::vector<float> backGroundColour_; /*! Color de Fondo */
   ToggleButtonActionType action_; /*! accion seleccionada desde 3DToolBar. */
// propiedades sin utilizar
DECLARE_EVENT_TABLE()
};
}  // namespace suri

#endif /*TERRAINGLCANVAS*/
