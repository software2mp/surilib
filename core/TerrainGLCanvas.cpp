/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

#include "TerrainGLCanvas.h"
#include "suri/Dimension.h"
// Includes standard
#include <math.h>
#include <iostream>
#if !wxUSE_GLCANVAS
#  error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif
#include <GL/gl.h>
#include <GL/glu.h>

// Includes suri
#include "suri/messages.h"
#include "logmacros.h"
// Includes wx
// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"
#include <wx/defs.h>
#ifndef WX_PRECOMP
#  include "wx/wx.h"
#endif

// Defines
/** Z-Near para determinacion del Frustum */
#define LARGEZNEAR 5
/** sin uso */
#define SMALLZNEAR 0.1
/** Z-Far para determinacion del Frustum */
#define ZNEARRATIO 10
/** tamano del buffer de seleccion. */
#define BUFSIZE 64
/** sin uso */
#define TERRAINSELECTION 1
/** identificador para renderizador de opengl */
/** \todo revisar comentarios. */
#define POINTSELECTION 2

#ifdef __GNUG__
#  pragma implementation
#  pragma interface
#endif

#ifdef __BORLANDC__
#  pragma hdrstop
#endif

/** namespace suri */
namespace suri {
BEGIN_EVENT_TABLE(TerrainGLCanvas, wxGLCanvas) EVT_SIZE(TerrainGLCanvas::OnSize)
EVT_PAINT(TerrainGLCanvas::OnPaint)
#ifdef __UNUSED_CODE__
EVT_ENTER_WINDOW(TerrainGLCanvas::OnEnterWindow)
#endif
EVT_LEAVE_WINDOW(TerrainGLCanvas::OnLeaveWindow)
EVT_MOUSE_EVENTS(TerrainGLCanvas::OnMouse)
EVT_KEY_DOWN(TerrainGLCanvas::OnKeyboard)
EVT_ERASE_BACKGROUND(TerrainGLCanvas::OnEraseBackground)
END_EVENT_TABLE()


/** 
 * TODO(Gabriel): Es necesario pararle como atributo WX_GL_DOUBLEBUFFER al wxGLcanvas
 * ya que sino genera un error sobre el administrador de ventanas. Ver thread
 * https://forums.wxwidgets.org/viewtopic.php?f=23&t=32200&p=136086
**/

#ifdef __LINUX__
int wxGlAttribList[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 24, 0 };
#else
int wxGlAttribList[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 32, 0 };
#endif  // __LINUX__

/**
 * Ctor.
 * @param[in]	pParentWindow: puntero a ventana padre
 * \todo ver porque no se estan inicializando todas las propiedades de la clase.
 * @return instancia de la clase TerrainGLCanvas
 */
TerrainGLCanvas::TerrainGLCanvas(wxWindow *pParentWindow) :
      wxGLCanvas(pParentWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                 wxFULL_REPAINT_ON_RESIZE, _(caption_OPENGL_CANVAS), wxGlAttribList,
                 wxNullPalette),
      pWorld_(NULL), ready_(false), pCamera_(NULL), pTerrain_(NULL), mousePosX_(0), mousePosY_(0),
      zNear_(0.0), zFar_(0.0), action_(TerrainGLCanvas::SphericCam) {
   // se inicializan en lista de parametros del ctor.
   radius_ = 300;
   update_ = true;
   windowChanged_= false;
   refreshWorld_ = false;
#ifdef __UNUSED_CODE__
   // Flag de inicializacion
   ready_ = false;
   pCamera_ = NULL;
   action_ = TerrainGLCanvas::SphericCam;
#endif
}

/** dtor. */
TerrainGLCanvas::~TerrainGLCanvas() {
   ResetCanvas();
   ResetCamera();
}

/**
 * Resetea las propiedades. Limpia el Canvas para poder renderizar otro Terreno.
 * \todo  ver porque no se estan reseteando el resto de las propiedades de la clase.
 */
void TerrainGLCanvas::ResetCanvas() {
   // Flag de inicializacion
   ready_ = false;

   // Se desprende del terreno
   pTerrain_ = NULL;
}

/**
 * Resetea las propiedades de la camara. Elimina la camara actual y la nulifica.
 */
void TerrainGLCanvas::ResetCamera() {
   // Destruye la camara
   delete pCamera_;
   pCamera_ = NULL;
}

/*
 * Establece el terreno. Invoca a ResetCanvas y luego inicializa OpenGL, la
 * Camara y crea la perspectiva.
 * @param[in] pTerrain puntero a terreno.
 */
void TerrainGLCanvas::SetTerrain(Terrain *pTerrain) {
   ResetCanvas();
   pTerrain_ = pTerrain;
   if (pTerrain_) {
      // Inicializa OpenGL
      InitOpenGL();
      // Inicializa la Camara
      InitCamera();
      // Crea la perspectiva
      SetPerspective();
   }
}

/**
 * Establece el color de Fondo de la ventana.
 * Los valores pasados por parametro tienen un rango [0, 255], pero en este
 * metodo se normalizan porque en OpenGL los valores van [0, 1].
 * @param[in]	Red: valor para color rojo
 * @param[in]	Green: valor para color verde
 * @param[in]	Blue: valor para color azul
 */
void TerrainGLCanvas::SetBackGroundColour(float Red, float Green, float Blue) {
   if (!backGroundColour_.empty()) {
      backGroundColour_.clear();
   }

   backGroundColour_.push_back(Red / 255.0);
   backGroundColour_.push_back(Green / 255.0);
   backGroundColour_.push_back(Blue / 255.0);

   glClearColor(backGroundColour_[0], backGroundColour_[1], backGroundColour_[2], 1.0);
}

/**
 * Obtiene del color de Fondo de la ventana
 * Los valores que devuelve el metodo, tienen un rango [0, 255] y como los
 * mismos valores en OpenGL van [0, 1] se los multiplica por 255.
 * @param[out]	Red: valor para color rojo
 * @param[out]	Green: valor para color verde
 * @param[out]	Blue: valor para color azul
 */
void TerrainGLCanvas::GetBackGroundColour(int &Red, int &Green, int &Blue) {
   if (!backGroundColour_.empty()) {
      Red = static_cast<int> (backGroundColour_[0] * 255);
      Green = static_cast<int>(backGroundColour_[1] * 255);
      Blue = static_cast<int> (backGroundColour_[2] * 255);
   } else {
      Red = 255;
      Green = 255;
      Blue = 255;
   }
}

/**
 * Establece el paso del angulo de elevacion, angulo acimutal, radio.
 * @param[in]	Step: paso.
 */
void TerrainGLCanvas::SetStep(float Step) {
   // si no hay Camara vuelvo.
   if (!pCamera_) {
      return;
   }

   int cameraperformance = pCamera_->GetPerformance();
   pCamera_->SetThetaStep(Step / cameraperformance);
   pCamera_->SetPhiStep(Step / cameraperformance);
   pCamera_->SetRadiusStep(Step / cameraperformance);
   pCamera_->TranslationStep(Step / cameraperformance);
}

/**
 * Metodo utilizado para obtener una imagen del terreno visualizado.
 * Carga el Buffer frontal. El ancho y el alto se toman del panel donde
 * esta el GLCanvas.
 * @param[in]	Width: ancho del buffer.
 * @param[in]	Height: alto del buffer.
 * @param[out]	pBuffer: buffer a devolver.
 */
void TerrainGLCanvas::GetBuffer(unsigned char* pBuffer, int Width, int Height) {
   // Seteo el buffer a leer
   glReadBuffer(GL_FRONT);
   // Bufer auxiliar
   unsigned char *pbufferaux = (unsigned char*) malloc(Width * 3);

   // leo el buffer
   for (int i = 0; i < Height; i++) {
      // Leo la fila (Desde abajo hacia arriba)
      glReadPixels(0, i, Width, 1, GL_RGB, GL_UNSIGNED_BYTE, pbufferaux);

      // Asigno la fila al buffer de imagen
      for (int j = 0; j < Width * 3; j++)
         pBuffer[(Height - 1 - i) * Width * 3 + j] = pbufferaux[j];
   }
   // delete pprogress;
   free(pbufferaux);
}

/**
 * Recibe un evento de cambio de tamanio de la ventana y genera una nueva
 * perspectiva del terreno.
 * @param[in]	Event: evento.
 */
void TerrainGLCanvas::OnSize(wxSizeEvent &Event) {
   // this is also necessary to update the context on some platforms
   wxGLCanvas::OnSize(Event);
   // crea la perspectiva
   SetPerspective();
}

/**
 * Metodo que responde a eventos de pintado de pantalla. Manda a renderizar el
 * terreno.
 * @param[in]	Event: evento.
 */
void TerrainGLCanvas::OnPaint(wxPaintEvent &Event) {
   wxPaintDC dc(this);
   Render();
   Event.Skip();
}

/**
 *Guarda los cambios de bufferedWindow en el world
 */
void TerrainGLCanvas::RefreshWorld() {
   if (pWorld_ && refreshWorld_) {
       update_ = false;
       windowChanged_ = false;
       pWorld_->SetWindow(bufferedWindow_);
       refreshWorld_ = false;
    }
}
/**
 * Cuando se sale de ventana se limpia posicion del mouse. Esto evita
 * que la vista de saltos al abrir vectores con doble clic.
 * @param[in] Event evento.
 */
void TerrainGLCanvas::OnLeaveWindow(wxMouseEvent& Event) {
   mousePosX_ = -1;
   mousePosY_ = -1;
   refreshWorld_ = true;
   RefreshWorld();
}

/**
 * Metodo que responde a los Evento de Mouse. Verifica flags de accion para
 * determinar el tipo de movimiento a realizar sobre el terreno.
 * @param[in]	Event: evento de mouse
 */
void TerrainGLCanvas::OnMouse(wxMouseEvent &Event) {
   // si no hay Camara no proceso los eventos.
   if (!pCamera_ || !pTerrain_) {
      return;
   }

   if (pWorld_ && !bufferedWindow_.IsValid()) {
      pWorld_->GetWindow(bufferedWindow_);
      if (!bufferedWindow_.IsValid())
            pTerrain_->GetTerrainWorld()->GetWindow(bufferedWindow_);
            pWorld_->SetWindow(bufferedWindow_);
            initialWindow_ = bufferedWindow_;
   }

   size_t x = Event.GetX();
   size_t y = Event.GetY();

   if (mousePosY_ < 0 || mousePosY_ < 0) {
      mousePosX_ = x;
      mousePosY_ = y;
   }

   if (action_ == TerrainGLCanvas::SphericCam) {
      if (Event.LeftIsDown()) {
         pCamera_->MoveElevation(mousePosY_ - y);
         pCamera_->MoveAzimuth(mousePosX_ - x);
      } else if (Event.RightIsDown()) {
         double pz = (mousePosY_ - static_cast<int>(y));
         TranslateZ(pz);
      } else if (Event.MiddleIsDown()) {
         double px = (mousePosX_ - static_cast<int>(x));
         double py = (mousePosY_ - static_cast<int>(y));
         TranslateXY(px, py);
      } else if (Event.RightUp()) {
         // para que no repercuta en la performance de la aplicacion
         if (windowChanged_) {
            refreshWorld_ = true;
         }
      } else if (Event.MiddleUp()) {
         if (windowChanged_) {
            refreshWorld_ = true;
         }
      } else {
         mousePosX_ = x;
         mousePosY_ = y;
         return;
      }
      mousePosX_ = x;
      mousePosY_ = y;
   } else if (action_ == TerrainGLCanvas::EyeCam) {
      if (Event.LeftIsDown()) {
         pCamera_->MoveIntrinsicAzimuth(mousePosX_ - x);
         pCamera_->MoveIntrinsicElevation(mousePosY_ - y);
      } else if (Event.RightIsDown()) {
         pCamera_->TranslateForward(mousePosY_ - y);
      } else if (Event.MiddleIsDown()) {
         pCamera_->MoveIntrinsicRoll(mousePosX_ - x);
      } else {
         mousePosX_ = x;
         mousePosY_ = y;
         return;
      }
      mousePosX_ = x;
      mousePosY_ = y;
   }
   RefreshWorld();
   Render();
   Event.Skip();
}

/**
 * Metodo que se encarga de encapsular la traslacion de la camara
 * modificando a su ves el window del world para que el modelo sea consitente con el cambio
 * realizado en la camara
 * */
void TerrainGLCanvas::TranslateXY(double X, double Y) {
   if (!windowChanged_||!bufferedWindow_.IsValid()) {
        pWorld_->GetWindow(bufferedWindow_);
        if (!bufferedWindow_.IsValid()) {
              if (pTerrain_->GetTerrainWorld())
                 pTerrain_->GetTerrainWorld()->GetWindow(bufferedWindow_);
              else {
                 return;
              }
        }
    }
   Dimension initialdim(initialWindow_);
   Dimension currentdim(bufferedWindow_);
   double factorH = initialdim.GetHeight() / currentdim.GetHeight();
   double factorW = initialdim.GetWidth() / currentdim.GetWidth();
   double normalizationConstantX = 0;
   double normalizationConstantY = 0;
   pTerrain_->GetNormalizationConstant(normalizationConstantX, normalizationConstantY);
   double px = X * pCamera_->GetTranslationStep() * normalizationConstantX;
   double py = Y * pCamera_->GetTranslationStep() * normalizationConstantY;
   Coordinates dist(factorW * px, factorH * py);
   pWorld_->InverseTransform(bufferedWindow_.ul_, bufferedWindow_.ul_);
   pWorld_->InverseTransform(bufferedWindow_.lr_, bufferedWindow_.lr_);
   bufferedWindow_ = bufferedWindow_ + dist;
   pWorld_->Transform(bufferedWindow_.ul_, bufferedWindow_.ul_);
   pWorld_->Transform(bufferedWindow_.lr_, bufferedWindow_.lr_);
   pCamera_->TranslateRight(-X);
   pCamera_->TranslateUp(Y);
   windowChanged_ = true;
}

void TerrainGLCanvas::ZoomIn(int X, int Y, double Factor) {
   Dimension dim(bufferedWindow_);
   // REPORT_DEBUG("D:ZoomIn");
   World* pWorld = pTerrain_->GetTerrainWorld();
   Subset original;
   Subset modified;
   pWorld->InverseTransform(bufferedWindow_.ul_, bufferedWindow_.ul_);
   pWorld->InverseTransform(bufferedWindow_.lr_, bufferedWindow_.lr_);
   original = bufferedWindow_;
   bufferedWindow_.lr_.x_ /= Factor;
   bufferedWindow_.lr_.y_ /= Factor;
   bufferedWindow_.ul_.x_ /= Factor;
   bufferedWindow_.ul_.y_ /= Factor;
   modified = bufferedWindow_;
   Coordinates distance(original.GetCenter().x_ - modified.GetCenter().x_,
                         original.GetCenter().y_ - modified.GetCenter().y_);
   bufferedWindow_ = bufferedWindow_ + distance;
   pWorld->Transform(bufferedWindow_.ul_, bufferedWindow_.ul_);
   pWorld->Transform(bufferedWindow_.lr_, bufferedWindow_.lr_);
   pTerrain_->GetTerrainWorld()->SetWindow(bufferedWindow_);
   windowChanged_ = true;
}

void TerrainGLCanvas::TranslateZ(double Z) {
   if ((!windowChanged_||!bufferedWindow_.IsValid() ) && pWorld_) {
        pWorld_->GetWindow(bufferedWindow_);
        if (!bufferedWindow_.IsValid())
              pTerrain_->GetTerrainWorld()->GetWindow(bufferedWindow_);
    }
   Coordinates center = bufferedWindow_.GetCenter();
   int x = 0;
   int y = 0;
   pTerrain_->GetTerrainWorld()->W2VTransform(center.x_, center.y_, x, y);
   pCamera_->TranslateForward(Z);
   double factorH = 0;
   if ((-Z + radius_) != 0) {
      if (Z > 0)
         factorH = (-Z + radius_) / radius_;
      if (Z < 0)
         factorH = (Z + radius_) / radius_;
   } else {
      factorH  = 1;
   }
   if (Z > 0) {
      if (factorH < 0) factorH *= -1;
      if (factorH < 1) {
         factorH = 1 / factorH;
      }
      ZoomIn(x, y, factorH);
   } else if (Z < 0){
      if (factorH < 0) factorH *= -1;
      if (factorH < 1) factorH = 1 / factorH;
      ZoomOut(x, y, factorH);
   }
}

void TerrainGLCanvas::ZoomOut(int X, int Y, double Factor) {
     World* pWorld = pTerrain_->GetTerrainWorld();
     Subset original;
     Subset modified;
     pWorld->InverseTransform(bufferedWindow_.ul_, bufferedWindow_.ul_);
     pWorld->InverseTransform(bufferedWindow_.lr_, bufferedWindow_.lr_);
     original = bufferedWindow_;
     bufferedWindow_.lr_.x_ *= Factor;
     bufferedWindow_.lr_.y_ *= Factor;
     bufferedWindow_.ul_.x_ *= Factor;
     bufferedWindow_.ul_.y_ *= Factor;
     modified = bufferedWindow_;
     Coordinates distance(original.GetCenter().x_ - modified.GetCenter().x_,
                           original.GetCenter().y_ - modified.GetCenter().y_);
     bufferedWindow_ = bufferedWindow_ + distance;
     pWorld->Transform(bufferedWindow_.ul_, bufferedWindow_.ul_);
     pWorld->Transform(bufferedWindow_.lr_, bufferedWindow_.lr_);
     pTerrain_->GetTerrainWorld()->SetWindow(bufferedWindow_);
     windowChanged_ = true;
}




/**
 * Metodo que responde a los eventos producidos por el usuario al presionar
 * alguna tecla. Verifica la tecla presionada para determinar el movimiento a
 * realizar sobre el terreno.
 * @param[in]	Event: evento
 */


void TerrainGLCanvas::OnKeyboard(wxKeyEvent &Event) {
   // si no hay Camara no proceso los eventos.
   if (!pCamera_) {
      return;
   }

   long keycode = Event.m_keyCode;

   if (Event.ShiftDown()) {
      switch (keycode) {
         case(WXK_UP):
            pCamera_->MoveIntrinsicElevation(1);
            break;
         case(WXK_DOWN):
            pCamera_->MoveIntrinsicElevation(-1);
            break;
         case(WXK_RIGHT):
            pCamera_->MoveIntrinsicAzimuth(1);
            break;
         case(WXK_LEFT):
            pCamera_->MoveIntrinsicAzimuth(-1);
            break;
         default:
            break;
      }
   } else {
      switch (keycode) {
         case(WXK_UP):
            pCamera_->MoveElevation(1);
            break;
         case(WXK_DOWN):
            pCamera_->MoveElevation(-1);
            break;
         case(WXK_RIGHT):
            pCamera_->MoveAzimuth(1);
            break;
         case(WXK_LEFT):
            pCamera_->MoveAzimuth(-1);
            break;
         default:
            break;
      }
   }

   Render();
   Event.Skip();
}

/**
 * Metodo que se ejecuta al limpiar el fondo del viewer. No tiene codigo.
 * \todo revisar doc.
 * @param[in] WXUNUSED evento
 */
void TerrainGLCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(Event)) {
   // Do nothing, to avoid flashing on MSW
}

void TerrainGLCanvas::DrawCross(wxPaintEvent &Event) {
   wxPaintDC dc(this);
   glBegin(GL_LINES);
     glVertex3f(0.0f, 0.0f, 0.0f);
     glVertex3f(50.0f, 50.0f, 50.0f);
   glEnd();
}



/**
 * Metodo que se encarga de renderizar el terreno en la ventana.
 * Si no hay terreno disponible pone el fondo de la pantalla en blanco.
 */
void TerrainGLCanvas::Render() {
   wxClientDC dc(this);
   if (!ready_) {
      dc.Clear();
      return;
   }
#ifndef __WXMOTIF__
   if (!GetContext()) {
      return;
   }
#endif
   SetCurrent();
   pCamera_->SetModelViewMatrix();
   // Inicializa el terreno, la camara y las opciones de openGL
   if (ready_) {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      // Renderiza el terreno
      pTerrain_->Draw();

   } else {
      // Limpio los buffers
      // glClearColor(1,1,1,1);
      SetBackGroundColour(255, 255, 255);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   }

   SwapBuffers();
}

/**
 * Generacion de la perspectiva.
 * Setea la proyeccion y el modelo de vista.
 */
void TerrainGLCanvas::SetPerspective() {
   // Tamano de la ventana
   int w, h;
   GetClientSize(&w, &h);

#ifndef __WXMOTIF__
   if (GetContext())
#endif
   {
      SetCurrent();
      glViewport(0, 0, (GLsizei) w, (GLsizei) h);  // Setea el tamanio del viewport
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(45.0, (GLdouble) w / (GLdouble) h, zNear_, zFar_);
      glMatrixMode(GL_MODELVIEW);
   }
}

/**
 * Seteo del frustum en funcion del tamanio del terreno
 */
void TerrainGLCanvas::SetFrustum() {
   // Radio de la esfera del terreno
   double terrainradius = pTerrain_->GetTerrainRadius();
   // Determino el Z-Near
   zNear_ = LARGEZNEAR;
   // Determino el Z-Far
   zFar_ = ZNEARRATIO * terrainradius;
}

/**
 * Inicializacion del terreno
 */
void TerrainGLCanvas::InitTerrain() {
   // Inicializa OpenGL
   InitOpenGL();
   // Inicializa la Camara
   InitCamera();
   // Crea la perspectiva
   SetPerspective();
   // Cargo el terreno
   pTerrain_->LoadTerrain();
   // Crea los display list para los vectores
   // pTerrain_->CreateVectorDisplayLists();
   // Crea el display list para terreno
   pTerrain_->CreateTerrainDisplayList();
}

/**
 * Inicializacion de vectores de puntos. Invoca a Terrain::DrawVector.
 * @param[in] pElement elemento del vector que se quiere agregar.
 * @return true si pudo dibujar el vector.
 * @return false si no pudo dibujar el vector.
 * \see Viewer3D.cpp#L703
 */
bool TerrainGLCanvas::InitPointVector(suri::Element *pElement) {
   // Crea los display list para los vectores
   int displaylistindex = pTerrain_->CreateVectorDisplayList(pElement);
   return pTerrain_->DrawVector(pElement, displaylistindex);
}

/**
 * Incializa las variables de OpenGL
 */
void TerrainGLCanvas::InitOpenGL() {
#ifndef __WXMOTIF__
   if (!GetContext()) {
      return;
   }
#endif

   SetCurrent();
   // Pinta ambos lados del Terreno
   glPolygonMode(GL_FRONT, GL_FILL);
   glPolygonMode(GL_BACK, GL_FILL);

   // Suvizado del los vertices
   glShadeModel(GL_SMOOTH);

   // Habilito el Test de Profundidad
   glEnable(GL_DEPTH_TEST);
   glDepthRange(0, 1);
   glDepthFunc(GL_LESS);
}

/**
 * Inicializacion de la camara
 */
void TerrainGLCanvas::InitCamera(float Rho, float Phi, float Theta ) {
   // El radio de inicio es dos veces la esfera que contiene al terreno
   size_t rho = (size_t) pTerrain_->GetTerrainRadius()*2;
   float phi = 90;
   float theta = 0;
   size_t rhostep = 3;
   const float phistep = 1;
   const float thetastep = 1;
   if (Rho != -1) rho = Rho;
   if (Phi != -1) phi = Phi;
   if (Theta != -1) theta = Theta;
   if (!pCamera_) {
      ResetCamera();
   }
   pCamera_ = new Camera(rho, phi, theta);
   pCamera_->SetRadiusStep(rhostep);
   pCamera_->SetPhiStep(phistep);
   pCamera_->SetThetaStep(thetastep);
   // Determinacion del Z-Near y el Z-Far
   SetFrustum();
   // Seteo el Radio Maximo de la Camara
   pCamera_->SetMaxRadius((size_t) pTerrain_->GetTerrainRadius() * (ZNEARRATIO - 3));
}

// ----------------------------------Seleccion de Puntos-------------------------
/**
 * Verifica si al hacer click sobre el terreno se hizo click sobre un Vector.
 * @param[in]	X: posicion X en coordenadas de pantalla(click).
 * @param[in]	Y: posicion Y en coordenadas de pantalla(click)
 * @return devuelve verdadero si se clickeo sobre un vector.
 */
bool TerrainGLCanvas::Selection(size_t X, size_t Y) {
   GLuint pselectedbuffer[BUFSIZE];
   GLint pviewport[4];
   GLint hits;

   // Buffer de seleccion
   glSelectBuffer(BUFSIZE, pselectedbuffer);

   // Obtengo el tamano del vierport
   glGetIntegerv(GL_VIEWPORT, pviewport);

   // Cambio a  projection y guardo la matriz de transformacion
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();

   // Cambio a modo seleccion
   glRenderMode(GL_SELECT);
   glLoadIdentity();

   // Inicio los identificadores en el stack
   glInitNames();
   glPushName(0);

   // Marco el area del cursor
   gluPickMatrix((GLdouble) X, (GLdouble) (pviewport[3] - Y + pviewport[1]), 5.0, 5.0,
                 pviewport);

   // Apply perspective matrix
   GLfloat faspect = (GLdouble) pviewport[2] / (GLdouble) pviewport[3];
   gluPerspective(45.0, faspect, 1.0, 15000.0);

   // Dibujo los vetctores de punto
   glLoadName(POINTSELECTION);
   pTerrain_->DrawPointVectors();

   // Chequeo de error
   GLuint errornum = glGetError();

   if (errornum != GL_NO_ERROR) {
      REPORT_DEBUG("D:Error de OpenGL numero \"%s\".", gluErrorString(errornum));
   }

   // Obtengo los casos y vuelvo al modo de renderizado
   hits = glRenderMode(GL_RENDER);

   // Restore the projection matrix
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();

   // Retorno al modelview para renderizado normal
   glMatrixMode(GL_MODELVIEW);

   // Retorno true si se clickeo sobre el terreno
   if (hits == 1) {
      return true;
   }

   return false;
}

/**
 * \pre invocar a Selection(x,y) para ver si se clickeo sobre un vector.
 * Este metodo devuelve la posicion del vector en coordenadas de mundo.
 * @param[in]	X: posicion X en coordenadas de pantalla(click).
 * @param[in]	Y: posicion Y en coordenadas de pantalla(click)
 * @return posicion (x,y,z) del punto seleccionado en la pantalla
 */
Coordinates TerrainGLCanvas::GetPosition(size_t X, size_t Y) {
   GLint pviewport[4] = {0};
   GLdouble pmodelview[16] = {0};
   GLdouble pprojection[16] = {0};
   GLfloat winx, winy, winz = 0;
   GLdouble posx = 0, posy = 0, posz = 0;

   // Tamanio del vierport
   glGetIntegerv(GL_VIEWPORT, pviewport);

   // Posicion (x,y) en coordenadas de pantalla
   winx = static_cast<float>(X);
   winy = static_cast<float>(pviewport[3]) - static_cast<float>(Y);

   // Parametos para la funcion de proyeccion inversa
   // Matriz del Modelo
   glGetDoublev(GL_MODELVIEW_MATRIX, pmodelview);

   // Matriz de Proyeccion
   glGetDoublev(GL_PROJECTION_MATRIX, pprojection);

   // Obtengo la posicion z de la ventana del buffer
   glReadPixels(static_cast<int>(winx), static_cast<int>(winy), 1, 1, GL_DEPTH_COMPONENT,
                GL_FLOAT, &winz);

   // Realiza la proyeccion inversa retornando los valores (x,y,z)
   gluUnProject(winx, winy, winz, pmodelview, pprojection, pviewport, &posx, &posy,
                &posz);

   // Asigno los valores al vector
   Coordinates position(posx, posy, posz);

   return position;
}
}  // namespace Suri
