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

#include "Camera.h"

// Includes standard
#include <iostream>
#include <cmath>

/** namespace suri */
namespace suri {
/**
 * Ctor. Inicializa variables y actualiza la vista.
 * @param[in]	Theta: angulo de elevacion.
 * @param[in]	Phi: angulo acimutal.
 * @param[in]	Rho: radio.
 */
Camera::Camera(float Rho, float Phi, float Theta) {
   InitVars(Rho, Phi, Theta);
   UpdateEye(0, 0);
   glMatrixMode(GL_MODELVIEW);
   glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix_);
}

/** Dtor. */
Camera::~Camera() {
}

/**
 * Inicializa propiedades.
 * @param[in]	Rho: radio.
 * @param[in]	Phi: angulo acimutal.
 * @param[in]	Theta: angulo de elevacion.
 */
void Camera::InitVars(float Rho, float Phi, float Theta) {
   zMinimum_ = 0;
   elevationStep_ = 1;
   azimuthStep_ = 1;
   radiusStep_ = 1;
   translationStep_ = 1;
   maxRadius_ = 0;           // Si el radio maximo es cero, no lo toma en cuenta
   theta_ = Theta;
   phi_ = Phi;
   rho_ = Rho;
   elevation_ = Theta;
   azimuth_ = Phi;
   radius_ = Rho;
   glLoadIdentity();
   resetView_ = true;
}

// Se comentan estos 3 metodos ya que no se los utilizaba dde ningun lado.
#ifdef __UNUSED_CODE__
/**
 * \todo(27/05/2009 - Alejandro): validar comentario del objetivo del metodo.
 * Establece los nuevos valores para los angulos (de vision??) y actualiza la
 * vista.
 * @param[in]	Theta: angulo de elevacion.
 * @param[in]	Phi: angulo acimutal.
 * @param[in]	Rho: radio.
 */
void Camera::MoveTo(float Theta, float Phi, float Rho) {
   elevation_ = Theta;
   azimuth_ = Phi;
   radius_ = Rho;
   UpdateEye();
}

/**
 * \todo(27/05/2009 - Alejandro): especificar objetivo del metodo.
 * @param[in]	Theta: angulo de elevacion intrinseco
 * @param[in]	Phi: angulo acimutal intrinseco
 */
void Camera::LookTo(float Theta, float Phi)
{
   iElevation_ = Theta;
   iAzimuth_ = Phi;
}

/**
 * \todo(27/05/2009 - Alejandro): especificar objetivo del metodo.
 * @param[in]
 * @param[in]
 * @param[in]
 */
void Camera::TranslateTo(float X, float Y, float Z) {
   translationX_ = X;
   translationY_ = Y;
   translationZ_ = Z;
}
#endif

/**
 * actualiza la posicion del observador cambiando azimuth.
 * @param[in]	Steps: cantidad de pasos que se hicieron.
 */
void Camera::MoveAzimuth(int Steps) {
   UpdateEye(Steps * azimuthStep_, 0);
}

/**
 * actualiza la posicion del observador cambiando elevation.
 * @param[in]	Steps: cantidad de pasos que se hicieron.
 */
void Camera::MoveElevation(int Steps) {
   UpdateEye(0, Steps * elevationStep_);
}

/**
 * Asigna nuevo valor del radio y actualiza la posicion del observador.
 * @param[in]	Steps: cantidad de pasos que se hicieron.
 */
void Camera::MoveRadius(int Steps) {
   // Aumenta el radio si es menor que el  maximo
   if (radius_ + Steps * radiusStep_ > GetMaxRadius() && GetMaxRadius() > 0) {
      radius_ = GetMaxRadius();
   } else {
      radius_ += Steps * radiusStep_;
   }

   // Actualiza la Vista
   UpdateEye(0, 0);
}

/**
 * Rota el observador cambiando la elevacion de la camara
 * @param[in]	Steps: cantidad de pasos que se hicieron.
 */
void Camera::MoveIntrinsicElevation(int Steps) {
   Rotate(Steps * elevationStep_, 0, 0);
}

/**
 * Rota el observador cambiando el azimuth de la camara
 * @param[in]	Steps: cantidad de pasos que se hicieron.
 */
void Camera::MoveIntrinsicAzimuth(int Steps) {
   Rotate(0, Steps * azimuthStep_, 0);
}

/**
 * Rota el observador sobre el eje de vision(roll)
 * @param[in]	Steps: cantidad de pasos que se hicieron.
 */
void Camera::MoveIntrinsicRoll(int Steps) {
   Rotate(0, 0, Steps * azimuthStep_);
}

/**
 * Translada la camara en direccion x(derecha)
 * @param[in]	Steps: cantidad de pasos que se hicieron.
 */
void Camera::TranslateRight(int Steps) {
   Translate(Steps * translationStep_, 0, 0);
}

/**
 * Translada la camara en direccion y(arriba)
 * @param[in]	Steps: cantidad de pasos que se hicieron.
 */
void Camera::TranslateUp(int Steps) {
   Translate(0, Steps * translationStep_, 0);
}

/**
 * Translada la camara en direccion z(adelanta)
 * @param[in]	Steps: cantidad de pasos que se hicieron.
 */
void Camera::TranslateForward(int Steps) {
   Translate(0, 0, Steps * translationStep_);
}

/**
 * Utiliza los angulos guardados y los parametros para calcular la nueva
 * posicion, luego usa LookAt para ir a la misma y finalmente le aplica
 * la rotacion guardada.
 * @param[in] Azimuth valor que se sumara al angulo con eje x
 * @param[in] Elevation valor que se sumara al angulo con eje z
 */
void Camera::UpdateEye(float Azimuth, float Elevation) {
   glMatrixMode(GL_MODELVIEW);
   GLfloat auxmatrix[16];

   if (resetView_) {
      resetView_ = false;
      glLoadIdentity();
      glGetFloatv(GL_MODELVIEW_MATRIX, pRotationMatrix_);
      elevation_ = theta_;
      azimuth_ = phi_;
      radius_ = rho_;
   }

   // calculo nuevo desplazamiento
   glLoadIdentity();
   elevation_ += Elevation;
   azimuth_ += Azimuth;
   DoLookUp();
   glGetFloatv(GL_MODELVIEW_MATRIX, auxmatrix);

   // Amplico matriz de rotacion a desplazamiento
   glLoadIdentity();
   glMultMatrixf(pRotationMatrix_);
   glMultMatrixf(auxmatrix);
   glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix_);
}

/**
 * Calcula la posicion(en coord esfericas) de la camara a partir de
 * la posicion actual y luego llama a RecalculateRotationMatrix().
 * Al calcular los angulos debo elegir que direccion de giro tendra la camara
 * en la nueva posicion.
 * La direccion de giro estara determinada por los valores de los angulos: al
 * mismo punto se puede llegar con dos giros diferentes y segun cual se elija
 * se girara en direccion contraria.
 * De las dos posibilidades elijo la que al sumar los modulos de los angulos den
 * mas cercanos a los anteriores
 */
void Camera::RecalculatePosition() {
   GLfloat auxmatrix[16];
   glGetFloatv(GL_MODELVIEW_MATRIX, auxmatrix);

   // Encuentro los parametros de elevacion, azimuth y radio en las nuevas coord

   // Encuentro posicion en coordenadas originales
   glGetFloatv(GL_MODELVIEW_MATRIX, auxmatrix);
   float newx = -(auxmatrix[0] * auxmatrix[12] + auxmatrix[1] * auxmatrix[13]
         + auxmatrix[2] * auxmatrix[14]);
   float newy = -(auxmatrix[4] * auxmatrix[12] + auxmatrix[5] * auxmatrix[13]
         + auxmatrix[6] * auxmatrix[14]);
   float newz = -(auxmatrix[8] * auxmatrix[12] + auxmatrix[9] * auxmatrix[13]
         + auxmatrix[10] * auxmatrix[14]);

   // Encuentro angulos del giro sobre el centro de coord que llevan a pos actual
   radius_ = sqrt(newx * newx + newy * newy + newz * newz);
   float elevation = 0;
   if (newz / radius_ > 1) {
      elevation = 0;
   } else if (newz / radius_ < -1) {
      elevation = 180;
   } else {
      elevation = acos(newz / radius_) / 3.14159265 * 180;
   }

   float azimuth = 0;
   if (elevation != 0) {
      if (newx / radius_ / sin(elevation * 3.14159265 / 180) > 1) {
         azimuth = 0;
      } else if (newx / radius_ / sin(elevation * 3.14159265 / 180) < -1) {
         azimuth = 180;
      } else {
         azimuth = acos(newx / radius_ / sin(elevation * 3.14159265 / 180)) / 3.14159265
               * 180;
      }
   }
   if (newy < 0) {
      azimuth = 360 - azimuth;
   }

   // Analizo que angulos le asigno para no cambiar direccion de giro
   elevation_ = fmod(elevation_, 360);
   azimuth_ = fmod(azimuth_, 360);
   float dif1 = std::abs(fmod(azimuth + 180, 360) - azimuth_)
         + std::abs(fmod(360 - elevation, 360) - elevation_);
   float dif2 = std::abs(azimuth - azimuth_) + std::abs(elevation - elevation_);
   elevation_ = elevation;
   azimuth_ = azimuth;
   if (dif1 < dif2) {
      azimuth_ += 180;
      elevation_ = 360 - elevation;
   }
   RecalculateRotationMatrix();
}

void Camera::SetModelViewMatrix(){
   glMatrixMode(GL_MODELVIEW);
   glLoadMatrixf(modelViewMatrix_);
}


/**
 * Calcula la matriz de rotacion de la camara, esta debera aplicarce luego de
 * hacer un LookUp.
 * La rotacion se obtiene multiplicando a la matriz actual por la inversa de
 * LookAt: Mactual=Mrotacion*Mlookat.
 */
void Camera::RecalculateRotationMatrix() {
   GLfloat originalmatrix[16];
   glGetFloatv(GL_MODELVIEW_MATRIX, originalmatrix);
   GLfloat auxmatrix[16];
   glGetFloatv(GL_MODELVIEW_MATRIX, auxmatrix);

   // calculo rotacion con los parametros
   glLoadIdentity();
   DoLookUp();

   GLdouble aux;
   // guardo matriz obtenida de LookAt en variable y encuentro inversa
   glGetFloatv(GL_MODELVIEW_MATRIX, pRotationMatrix_);
   for (int i = 0; i < 3; i++)
      for (int j = i + 1; j < 3; j++) {
         aux = pRotationMatrix_[4 * i + j];
         pRotationMatrix_[4 * i + j] = pRotationMatrix_[4 * j + i];
         pRotationMatrix_[4 * j + i] = aux;
      }
   for (int i = 0; i < 3; i++)
      pRotationMatrix_[12 + i] = 0;

   // Aplico inversa encontrada a rotacion original para matriz final
   for (int i = 0; i < 3; i++)
      auxmatrix[12 + i] = 0;
   glLoadIdentity();
   glMultMatrixf(auxmatrix);
   glMultMatrixf(pRotationMatrix_);

   // Guardo la matriz en rotationMatrix_
   glGetFloatv(GL_MODELVIEW_MATRIX, pRotationMatrix_);

   // Vuelvo matriz a estado inicial
   glLoadIdentity();
   glMultMatrixf(originalmatrix);
}

/**
 * Realiza gluLookUP usando coordenadas esfericas del objeto
 */
void Camera::DoLookUp() {
   GLdouble eyex;
   GLdouble eyey;
   GLdouble eyez;
   GLdouble upx;
   GLdouble upy;
   GLdouble upz;
   eyex = radius_ * sin(elevation_ * 3.1416 / 180) * cos(azimuth_ * 3.1416 / 180);
   eyey = radius_ * sin(elevation_ * 3.1416 / 180) * sin(azimuth_ * 3.1416 / 180);
   eyez = radius_ * cos(elevation_ * 3.1416 / 180);
   upx = sin((elevation_ + 90) * 3.1416 / 180) * cos(azimuth_ * 3.1416 / 180);
   upy = sin((elevation_ + 90) * 3.1416 / 180) * sin(azimuth_ * 3.1416 / 180);
   upz = cos((elevation_ + 90) * 3.1416 / 180);
   gluLookAt(eyex, eyey, eyez, 0, 0, 0, upx, upy, upz);
}

/**
 * Rota la camara respecto a los ejes de la vista actual
 * Multiplica la matriz la matriz de GL_MODELVIEW con la rotacion a la izquierda
 * @param[in] IAzimuth cambio de azimuth (giro sobre eje y)
 * @param[in] IElevation ambio de elevation (giro sobre eje x)
 * @param[in] IRoll cantidad de roll (giro sobre eje z)
 * \post La matriz rotacion y los angulos del objeto se actualizan con la nueva
 * posicion
 */
void Camera::Rotate(float IElevation, float IAzimuth, float IRoll) {
   glMatrixMode(GL_MODELVIEW);
   // Aplico rotacion a matriz identidad
   glLoadIdentity();
   glRotatef(IElevation, 1, 0, 0);
   glRotatef(IAzimuth, 0, 1, 0);
   glRotatef(IRoll, 0, 0, 1);
   // Aplico las rotaciones despues de la transformacion actual
   glMultMatrixf(modelViewMatrix_);
   RecalculateRotationMatrix();
   glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix_);
}

/**
 * Aplica desplazamiento al la matriz GL_MODELVIEW
 * @param[in] TranslationX translacion en x
 * @param[in] TranslationY translacion en y
 * @param[in] TranslationZ translacion en z
 * \post La matriz rotacion y los angulos del objeto se actualizan con la nueva
 * posicion
 */
void Camera::Translate(float TranslationX, float TranslationY, float TranslationZ) {
   glMatrixMode(GL_MODELVIEW);
   glLoadMatrixf(modelViewMatrix_);
   glLoadIdentity();
   glTranslatef(TranslationX, TranslationY, TranslationZ);
   // Aplico la translacion despues de la transformacion actual
   glMultMatrixf(modelViewMatrix_);
   RecalculatePosition();
   glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix_);
}
}   // namespace Suri

