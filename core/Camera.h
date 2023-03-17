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

#ifndef CAMERA_H_
#define CAMERA_H_

// Includes standrad
#include <math.h>
#include <GL/glu.h>

// Defines
/** Factor de performance de la camara */
#define CAMERA_PERFORMANCE_FACTOR 5

/** namespace suri */
namespace suri {
/** Clase para visualizar el terreno */
/**
 * Esta clase se utiliza para ubicar el observador y su linea de vision sobre el
 * terreno y la forma de desplazamiento sobre el mismo.
 * Proporciona una forma de desplazamiento, llamada camara esferica y una forma
 * de cambiar la linea de vision, llamada camara  de ojo.
 */
class Camera {
public:
   /** Ctor. */
   Camera(float Rho = 1, float Phi = 0, float Theta = 0);
   /** Dtor. */
   ~Camera();

   /** Inicializacion */
   void InitVars(float Rho = 1, float Phi = 0, float Theta = 0);

// Se comentan estos 3 metodos ya que no se los utilizaba dde ningun lado.
#ifdef __UNUSED_CODE__
   /** Posicionamiento esferico */
   void MoveTo(float Theta, float Phi, float Rho);
   /** void LookTo(float Elevation, float Azimuth); */
   /** void TranslateTo(float X, float Y, float Z); */
#endif

   /** Magnitud de paso */
   void SetThetaStep(float Step) {
      elevationStep_ = Step;
   }
   /** Magnitud de paso */
   void SetPhiStep(float Step) {
      azimuthStep_ = Step;
   }
   /** Magnitud de paso */
   void SetRadiusStep(float Step) {
      radiusStep_ = Step;
   }
   /** Magnitud de paso */
   void TranslationStep(float Step) {
      translationStep_ = Step;
   }
   /** Magnitud de paso */
   float GetRadiusStep() {
      return radiusStep_;
   }
   float GetTranslationStep(){
      return translationStep_;
   }
   /** Movimiento esferico. */
   void MoveAzimuth(int Steps = 1);
   /** Movimiento esferico. */
   void MoveElevation(int Steps = 1);
   /** Movimiento esferico. */
   void MoveRadius(int Steps = 1);

   /** Movimiento esferico intrinseco (apuntamiento) */
   void MoveIntrinsicElevation(int Steps = 1);
   /** Movimiento esferico intrinseco (apuntamiento) */
   void MoveIntrinsicAzimuth(int Steps = 1);
   /** Movimiento esferico intrinseco (apuntamiento) */
   void MoveIntrinsicRoll(int Steps = 1);

   /** Movimientos de traslacion */
   void TranslateRight(int Steps = 1);
   /** Movimientos de traslacion */
   void TranslateUp(int Steps = 1);
   /** Movimientos de traslacion */
   void TranslateForward(int Steps = 1);

   /** Otras */
   void EnableReset() {
      resetView_ = true;
   }
   ;

   /** Devuelve angulo de elevacion */
   float GetElevation() {
      return elevation_;
   }
   /** Devuelve angulo acimutal */
   float GetAzimuth() {
      return azimuth_;
   }
   /** Devuelve radio */
   float GetRadio() {
      return radius_;
   }

   /** Determinacion de valores de Borde */
   double GetMaxRadius() {
      return maxRadius_;
   }
   /** Establece el radio maximo */
   void SetMaxRadius(double MaxRadius) {
      maxRadius_ = MaxRadius;
   }
   /** Devuelve el valor minimo de z al que llega la camara */
   double GetMinZ() {
      return zMinimum_;
   }
   /** Establece el valor minimo de z al que llega la camara */
   void SetMinZ(double ZMinimum) {
      zMinimum_ = ZMinimum;
   }

   /** Devuelve factor de performance de la camara. */
   int GetPerformance() {
      return CAMERA_PERFORMANCE_FACTOR;
   }
   /** Realiza gluLookUP usando coordenadas esfericas del objeto */
   void DoLookUp();
   /** Agrega matriz de desplazamiento al la matriz GL_MODELVIEW */
   void Translate(float translationX, float translationY, float translationZ);
   void SetModelViewMatrix();

protected:
   /** Actualiza la vista con los nuevos angulos */
   void UpdateEye(float Azimuth, float Elevation);
   /** Calcula la posicion y rotacion de la camara luego de desplazamiento */
   void RecalculatePosition();
   /** Calcula la rotacion de la camara luego de rotacion */
   void RecalculateRotationMatrix();
   /** Agrega matriz de rotacion al la matriz GL_MODELVIEW */
   void Rotate(float elevation, float iAzimuth, float radius);


   bool resetView_; /*! resetea la vista de opengl al actualizar */
   float elevationStep_; /*! paso de cambio del angulo de elevacion */
   float azimuthStep_; /*! paso de cambio del angulo acimutal */
   float radiusStep_; /*! paso de cambio del radio */
   float translationStep_; /*! paso de cambio de traslado */
   float elevation_; /*! angulo de elevacion (theta) */
   float azimuth_; /*! angulo acimutal (phi) */
   float radius_; /*! radio (rho) */
   float phi_; /*! angulo que tendra azimuth_ luedgo de reset de */
   /* camara */
   float rho_; /*! angulo que tendra radius_ luedgo de reset de */
   /* camara */
   float theta_; /*! angulo que tendra elevation_ luedgo de reset */
   /* de camara */
   GLfloat pRotationMatrix_[16]; /*! matriz con rotacion desde vector lookat */
   /* con centro en (0,0,0) a vista final */
   double maxRadius_; /*! Radio Maximo de posicion de la camara */
   double zMinimum_; /*! Valor Minimo de Z al que llega la camara */
   GLfloat modelViewMatrix_[16];
};
}  // namespace suri

#endif /*CAMERA_H_*/
