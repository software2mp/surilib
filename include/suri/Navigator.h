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

#ifndef NAVIGATION_H_
#define NAVIGATION_H_

// Includes standard
#include <string>

// Includes suri
#include "suri/Coordinates.h"
#include "suri/Subset.h"
#include "suri/NavigationWidget.h"

/** namespace suri */
namespace suri {

// forwards
class World;
class CoordinatesTransformation;
class DataViewManager;
class ViewportManagerInterface;
class EventInterface;

/** Permite navegar por el mundo indicando las coordenadas */
/**
 * Para las funciones usa un World, esto evita que halla errores de redondeo al
 * usar PL.
 * El navegador por default espera/retorna los puntos en sistema de coordenadas
 * del mundo. Se puede modificar esto para que trabaje en otro sistema de
 * coordenadas y realice la conversion al sistema del mundo internamente.
 * \todo El NavigationWidget deberia permitir ingresar coordenadas en el sistema
 * de referencia asignado y no solo en latitud longitud.
 */
class Navigator {
public:
   /** ctor */
   Navigator(DataViewManager* pDataViewManager);
   /** dtor */
   ~Navigator();
   /** Establece el sistema de coordenadas en el que espera los puntos para navegar */
   bool SetSpatialReference(const std::string &SpatialReference);
   /** Retorna el sistema de coordenadas en el que espera los puntos para navegar */
   std::string GetSpatialReference();
   /** Si el punto esta en el window y no cambia zoom no hace nada, caso contrario llama a CenterPoint */
   bool ShowPoint(const Coordinates &Point, double Zoom = 0);
   /** Cambia la ventana para centrarla en Point con Zoom indicado */
   bool CenterPoint(const Coordinates &Point, double Zoom = 0);
   /** Informa las coordenadas del ultimo Show/Center que tubo exito */
   bool GetLastPoint(Coordinates &Point);
   /** Informa las coordenadas del ultimo Show/Center que tubo exito en coordenadas de mundo */
   bool GetLastPointInWorldCoordinates(Coordinates &Point);
   /** Informa las coordenadas del centro del window actual */
   bool GetCenterPoint(Coordinates &Point);
   /** Informa si el window contiene el punto */
   bool IsShow(const Coordinates &Point);
   /** Informa si el punto esta en el centro del window con 1% de error */
   bool IsCentred(const Coordinates &Point);
   /** Retorna NavigatorWidget que se usa para navegar. */
   NavigationWidget* GetWidget();
   /** Guarda el NavigatorWidget que se usa para navegar. */
   void SetWidget(NavigationWidget* pNavigationWidget);
   /** Borra la coordenada del ultimo show/center */
   void ResetCoordinate();
   /** Devuelve la transformacion que esta siendo utilizada. */
   CoordinatesTransformation* GetTransformation();
   /** Agrega al visualizador activo un evento paint */
   void StartPaintEvent();
   /** Elimina del visualizador activo un evento paint */
   void StopPaintEvent();

protected:
   /** Actualiza el coordinates transform si se cambio el mundo */
   bool UpdateCoordinatesTransform();

private:
   /** Actualiza el visualizador activo */
   void UpdateViewer();
   /** Referencia espacial en la que espera los puntos al hacer Show/Center. */
   std::string inputSpatialReference_;
   /** Clase que se usa para transformar coordenadas entre sistema elegido y el del mundo */
   CoordinatesTransformation* pCoordTransform_;
   /** Coordenada del ultimo show/center valido */
   Coordinates point_;
   /**
    * Informa si el punto es valido. No es valido si no fue ingresado ningun punto o si cambia
    * el sistema de  referencia del mundo
    */
   bool validPoint_;
   /** World que se usa para modificar la ventana */
   ViewerWidget* pViewer_;
   /**
    * Nivel de zoom que se hizo en ultimo show/center.
    * Guarda relacion entre tamano de window y world
    */
   double zoom_;
   /** Widget que permite ingresar */
   NavigationWidget* pNavigationWidget_;
   /* coordenadas en lat/long */
   /** Puntero a evento CrossHair */
   EventInterface* pCrossHair_;
   /** Puntero a evento DataViewManager */
   DataViewManager* pDataView_;
};
}

#endif /*NAVIGATION_H_*/
