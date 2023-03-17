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

// Includes Estandar
#include <string>
#include <algorithm>

// Includes suri
#include "suri/messages.h"
#include "logmacros.h"
#include "suri/DataTypes.h"
#include "suri/RasterSpatialModel.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Dimension.h"
#include "suri/World.h"
#include "suri/Configuration.h"
// Defines
/** Define valor para referencia espacial nula. */
#define SPATIAL_REFERENCE_NULL_VALUE ""

/** namespace suri */
namespace suri {
/**
 * Constructor.
 * @return instancia de la clase World
 */
World::World() :
      initialized_(false), world_(Subset()), window_(Subset()), viewportW_(0),
      viewportH_(0), wkt_(SPATIAL_REFERENCE_NULL_VALUE), pRasterModel_(NULL) {
   ResetWorld();
}

/**
 * Constructor de Copia.
 * \todo Esta clase sirve como clase base a  AspectPreservingWorld y el ctor.
 * de copia deberia ser privado.
 * Tendria que crearse un metodo World::Clone() y hacer el ctor. de copia privado.
 * @param[in] WorldWindow Mundo con el que se inicializa el mundo que se construye
 */
World::World(const World &WorldWindow) {
   initialized_ = WorldWindow.initialized_;
   world_ = WorldWindow.world_;
   window_ = WorldWindow.window_;
   viewportW_ = WorldWindow.viewportW_;
   viewportH_ = WorldWindow.viewportH_;
   wkt_ = WorldWindow.wkt_;
   pRasterModel_ = NULL;
   UpdateSpatialModel();
}

/** Dtor */
World::~World() {
}

// --------------------- SET -----------------------

/** Vuelve el mundo a el estado original */
/**
 * pone todos los atributos de world en falso, 0 o con objetos vacios
 */
void World::ResetWorld() {
   if (!initialized_) {
      return;
   }REPORT_DEBUG("D:Reseteando mundo");
   initialized_ = false;
   world_ = Subset();
   window_ = Subset();
   viewportW_ = 0;
   viewportH_ = 0;
   wkt_ = Configuration::GetParameterEx("GISMode_World_Wkt", SPATIAL_REFERENCE_NULL_VALUE);
   RasterSpatialModel::Destroy(pRasterModel_);
   // viewport no se resetea
}

/**
 * Modifica las coordenadas del mundo
 * @param[in] WorldUlx coordenada del mundo upper left x
 * @param[in] WorldUly coordenada del mundo upper left y
 * @param[in] WorldLrx coordenada del mundo lower right x
 * @param[in] WorldLry coordenada del mundo lower right y
 * \todo validar las coordenadas
 */
void World::SetWorld(double WorldUlx, double WorldUly, double WorldLrx,
                     double WorldLry) {
   // si da un mundo sin dimension o son iguales a las existentes
   if (FLOAT_COMPARE(WorldUlx, WorldLrx) || FLOAT_COMPARE(WorldUly, WorldLry)
         || (FLOAT_COMPARE(world_.ul_.x_, WorldUlx)
               && FLOAT_COMPARE(world_.ul_.y_, WorldUly)
               && FLOAT_COMPARE(world_.lr_.x_, WorldLrx)
               && FLOAT_COMPARE(world_.lr_.y_, WorldLry))) {
      PreventNextViewerUpdate(false);
      return;
   }REPORT_DEBUG(
         "D:Modificando Mundo: (%.2f,%.2f;%.2f,%.2f) -> (%.2f,%.2f;%.2f,%.2f)",
         world_.ul_.x_, world_.ul_.y_, world_.lr_.x_, world_.lr_.y_, WorldUlx
         , WorldUly, WorldLrx, WorldLry);
   world_.ul_.x_ = WorldUlx;
   world_.ul_.y_ = WorldUly;
   world_.lr_.x_ = WorldLrx;
   world_.lr_.y_ = WorldLry;
   initialized_ = true;
   // Seniala la actualizacion
   SetChanged();
   SendViewerUpdate();
}

/**
 * Cambia las coordenadas del mundo. Invoca a SetWorld.
 * @param[in] NewWorld coordenadas del nuevo mundo.
 */
void World::SetWorld(const Subset &NewWorld) {
   SetWorld(NewWorld.ul_.x_, NewWorld.ul_.y_, NewWorld.lr_.x_, NewWorld.lr_.y_);
}

/** Agrega a las coordenadas del mundo */
/**
 *  Agranda el mundo usando las coordenadas existentes y las nuevas para que
 * este contenga a todas.
 * @param[in] WorldUlx coordenada ul_.x_ del subset que se quiere anexar
 * @param[in] WorldUly coordenada ul_.y_ del subset que se quiere anexar
 * @param[in] WorldLrx coordenada lr_.x_ del subset que se quiere anexar
 * @param[in] WorldLry coordenada lr_.y_ del subset que se quiere anexar
 * \todo considerar el caso de las coordenadas del mundo sin inicializar
 */
void World::AppendWorld(double WorldUlx, double WorldUly, double WorldLrx,
                        double WorldLry) {
   if (!initialized_) {
      PreventNextViewerUpdate();
      SetWorld(WorldUlx, WorldUly, WorldLrx, WorldLry);
      // Si no estaba inicializado, fuerzo la actualizacion
      PreventNextViewerUpdate(false);
      SetWindow(WorldUlx, WorldUly, WorldLrx, WorldLry);
      return;
   }
   Dimension dim(world_);
   int sx, sy;
   dim.GetSign(sx, sy);

   double ulx, uly;
   double lrx, lry;
   if (sx > 0) {
      ulx = std::min(world_.ul_.x_, WorldUlx);
      lrx = std::max(world_.lr_.x_, WorldLrx);
   } else {
      ulx = std::max(world_.ul_.x_, WorldUlx);
      lrx = std::min(world_.lr_.x_, WorldLrx);
   }
   if (sy > 0) {
      uly = std::min(world_.ul_.y_, WorldUly);
      lry = std::max(world_.lr_.y_, WorldLry);
   } else {
      uly = std::max(world_.ul_.y_, WorldUly);
      lry = std::min(world_.lr_.y_, WorldLry);
   }

   SetWorld(ulx, uly, lrx, lry);
}

/**
 *  Agrega a las coordenadas del mundo
 *  @param[in] MoreWorld Coordenadas a agregar.
 */
void World::AppendWorld(const Subset &MoreWorld) {
   AppendWorld(MoreWorld.ul_.x_, MoreWorld.ul_.y_, MoreWorld.lr_.x_, MoreWorld.lr_.y_);
}

/**
 *  Cambia las coordenadas de la ventana
 * @param[in] WindowUlx coordenada de ventana (en coordenadas de mundo)
 * @param[in] WindowUly coordenada de ventana (en coordenadas de mundo)
 * @param[in] WindowLrx coordenada de ventana (en coordenadas de mundo)
 * @param[in] WindowLry coordenada de ventana (en coordenadas de mundo)
 * \attention la comparacion de float puede probocar fallas en otros lugares aun
 *             cuando el limite numerico no halla sido alcanzado
 * \sa ZoomButton::DoOnMouseMove
 * \todo validar las coordenadas
 */
void World::SetWindow(double WindowUlx, double WindowUly, double WindowLrx,
                      double WindowLry) {
   if (initialized_) {
      // Si la ventana tiene dimension 0 no hace el sendupdate por lo que no renderiza
      if (FLOAT_COMPARE(WindowUlx, WindowLrx) || FLOAT_COMPARE(WindowUly, WindowLry)) {
#ifdef __UNUSED_CODE__
         // Se comenta para que aunque no cambie el window renderice y corrija el
         // bug de drag en las esquinas
         ||(FLOAT_COMPARE(window_.ul_.x_, WindowUlx)&&
               FLOAT_COMPARE(window_.ul_.y_, WindowUly)&&
               FLOAT_COMPARE(window_.lr_.x_, WindowLrx)&&
               FLOAT_COMPARE(window_.lr_.y_, WindowLry) )
#endif

         // Las llamadas a este metodo deben ser consistentes en cuanto al
         // PreventUpdate (lo seteo false por si se hizo una llamada desde fuera
         // de este metodo y se espera el cambio)
         PreventNextViewerUpdate(false);
         return;
      }REPORT_DEBUG(
            "D:Modificando Window: (%.2f,%.2f;%.2f,%.2f) -> (%.2f,%.2f;%.2f,"
            "%.2f)",
            window_.ul_.x_, window_.ul_.y_, window_.lr_.x_, window_.lr_.y_,
            WindowUlx, WindowUly, WindowLrx, WindowLry);
      window_.ul_.x_ = WindowUlx;
      window_.ul_.y_ = WindowUly;
      window_.lr_.x_ = WindowLrx;
      window_.lr_.y_ = WindowLry;
      UpdateSpatialModel();
      // Seniala la actualizacion
      SetChanged();
      SendViewerUpdate();
   }
}

/** Cambia las coordenadas de la ventana */
/**
 * @param[in] Window Subset con la nueva ventana del mundo
 */
void World::SetWindow(const Subset &Window) {
   SetWindow(Window.ul_.x_, Window.ul_.y_, Window.lr_.x_, Window.lr_.y_);
}


void World::GetPixelSize(double &x, double &y) const{
   double geoTransf[6];
   pRasterModel_->GetGeoTransform(geoTransf);
   x = geoTransf[1];
   y = geoTransf[5];
}



/**
 * Cambia el tamanio del Viewport y llama a ViewerUpdate
 * @param[in] ViewportWidth dimension X del viewport
 * @param[in] ViewportHeight dimension Y del viewport
 * \todo validar las coordenadas
 */
void World::SetViewport(int ViewportWidth, int ViewportHeight) {
   if (viewportW_ == ViewportWidth && viewportH_ == ViewportHeight) {
      PreventNextViewerUpdate(false);
      return;
   }REPORT_DEBUG("D:Modificando Viewport (%d,%d) -> (%d,%d)",
                 viewportW_, viewportH_, ViewportWidth, ViewportHeight);
   viewportW_ = ViewportWidth;
   viewportH_ = ViewportHeight;
   UpdateSpatialModel();
   // Seniala la actualizacion
   SetChanged();
   SendViewerUpdate();
}

/** Carga el sistema de referencia del mundo */
/**
 * Si Wkt es diferente al que tenia reseteo el mundo, guardo el Wkt y
 * llamo a Update.
 * @param[in] Wkt Well known text con la nueva referencia espacial del mundo
 */
void World::SetSpatialReference(const std::string &Wkt) {
   // Ignoro el cambio si es igual a la anterior
   if (wkt_ == Wkt) {
      // Las llamadas a este metodo deben ser consistentes en cuanto al
      // PreventUpdate (lo seteo false por si se hizo una llamada desde fuera
      // de este metodo y se espera el cambio)
      PreventNextViewerUpdate(false);
      return;
   }REPORT_DEBUG("D:Seteando WorldWindow::SpatialReference a %s", Wkt.c_str());
   if (IsInitialized()) {
      ResetWorld();
   }
   wkt_ = Wkt;
   // Seniala la actualizacion
   SetChanged();
   SendViewerUpdate();
}

/**
 * Carga el sistema de referencia del mundo
 * @return string con la referencia espacial del mundo. Hace un return de la
 * propiedad ::wkt_
 */

std::string World::GetSpatialReference() const {
   return wkt_;
}

// --------------------- GET -----------------------

/**
 * Retorna las coordenadas de mundo (limites para window)
 * @param[out] WorldUlx coordenada del mundo upper left x
 * @param[out] WorldUly coordenada del mundo upper left y
 * @param[out] WorldLrx coordenada del mundo lower right x
 * @param[out] WorldLry coordenada del mundo lower right y
 */
void World::GetWorld(double &WorldUlx, double &WorldUly, double &WorldLrx,
                     double &WorldLry) const {
   WorldUlx = world_.ul_.x_;
   WorldUly = world_.ul_.y_;
   WorldLrx = world_.lr_.x_;
   WorldLry = world_.lr_.y_;
}

/**
 * Retorna las coordenadas de mundo (limites para window)
 * @param[out] World subset con coordeandas de mundo
 */
void World::GetWorld(Subset &World) const {
   World = world_;
}

/**
 * Retorna las coordenadas de la ventana en sistema window
 * @param[out] WindowUlx coordenada de ventana upper left x
 * @param[out] WindowUly coordenada de ventana upper left y
 * @param[out] WindowLrx coordenada de ventana lower right x
 * @param[out] WindowLry coordenada de ventana lower right y
 */
void World::GetWindow(double &WindowUlx, double &WindowUly, double &WindowLrx,
                      double &WindowLry) const {
   WindowUlx = window_.ul_.x_;
   WindowUly = window_.ul_.y_;
   WindowLrx = window_.lr_.x_;
   WindowLry = window_.lr_.y_;
}

/**
 * Retorna las coordenadas de la ventana en sistema window
 * @param[out] Window subset con coordeandas de ventana
 */
void World::GetWindow(Subset &Window) const {
   Window.ul_.x_ = window_.ul_.x_;
   Window.ul_.y_ = window_.ul_.y_;
   Window.lr_.x_ = window_.lr_.x_;
   Window.lr_.y_ = window_.lr_.y_;
}

/**
 * Retorna el tamanio del Viewport
 * @param[out] ViewportWidth dimension X del viewport
 * @param[out] ViewportHeight dimension Y del viewport
 */
void World::GetViewport(int &ViewportWidth, int &ViewportHeight) const {
   ViewportWidth = viewportW_;
   ViewportHeight = viewportH_;
}

// ------------------- TRANSFORM -------------------

/** Calcula el RasterSpatialModel correspondiente al mundo */
void World::UpdateSpatialModel() {
   if (!IsInitialized()) {
      return;
   }REPORT_DEBUG("D:Actualizando Modelo espacial");
   RasterSpatialModel::Destroy(pRasterModel_);
   pRasterModel_ = RasterSpatialModel::Create(Coordinates(0, 0),
                                              Coordinates(viewportW_, viewportH_),
                                              window_.ul_, window_.lr_);
#ifdef __DEBUG__
   if (!pRasterModel_) {
      REPORT_DEBUG(
            "D:Fail RasterModel((0,0),(%d,%d),(%.2f,%.2f),(%.2f,%.2f)",
            viewportW_, viewportH_, window_.ul_.x_, window_.ul_.y_, window_.lr_.x_, window_.lr_.y_);
   }
#endif
}

/** Transforma las coordenadas de Mundo en coordenadas de Viewport */
/**
 * Uso InverseTransform de RasterSpatialModel para la transformacion
 * @param[in] WorldX coordenada de eje x en sistema de referencia
 * @param[in] WorldY coordenada de eje y en sistema de referencia
 * @param[out] ViewportX columna en imagen del pixel que corresponde al punto
 * ingresado
 * @param[out] ViewportY fila en imagen del pixel que corresponde al punto
 * ingresado
 * \todo Optimizar como loco
 * \todo Verificar que no se exceda en limite del tipo (int) en el cast
 * \deprecated Funcion deprecada, usar InverseTransform y convertir a entero
 *  segun sea la operacion a realizar.
 */
void World::W2VTransform(double WorldX, double WorldY, int &ViewportX,
                         int &ViewportY) const {
   // temporal para la salida
   Coordinates temp;
   InverseTransform(Coordinates(WorldX, WorldY), temp);
   // Redondeo
   ViewportX = SURI_ROUND(int, temp.x_);
   ViewportY = SURI_ROUND(int, temp.y_);
}

/** Transforma las coordenadas de Viewport en coordenadas de Mundo */
/**
 * Uso Transform de RasterSpatialModel para la transformacion
 * @param[in] ViewportX columna en imagen del pixel que se quiere buscar
 * @param[in] ViewportY fila en imagen del pixel que corresponde al punto
 * @param[out] WorldX coordenada de eje x en sistema de referencia
 * @param[out] WorldY coordenada de eje y en sistema de referencia
 * \deprecated Funcion deprecada, usar Transform
 */
void World::V2WTransform(int ViewportX, int ViewportY, double &WorldX,
                         double &WorldY) const {
   // temporal para la salida
   Coordinates temp;
   Transform(Coordinates(ViewportX, ViewportY), temp);
   WorldX = temp.x_;
   WorldY = temp.y_;
}

/** Transforma las coordenadas de Mundo en coordenadas de Viewport */
/**
 *  Funcion que transforma coordenadas de mundo en coordenadas de Viewport.
 * @param[in] World Coordenadas de Mundo
 * @param[out] Viewport Coordenadas de Viewport en formato REAL, es necesario
 *             convertirlas a entero usando la funcion adecuada (FLOOR, TRUNC,
 *             ROUND, CIEL) que varia segun la operacion que se realiza. (ej. al
 *             calcular un subset, el UL debe realizar un FLOOR y el LR un CIEL)
 * \attention No tiene sentido hablar de coordenadas de Viewport Reales ya que
 *            es la minima unidad disponible para dicha matriz. Es necesario
 *            transformar las coordenadas de salida a un valor entero.
 */
void World::InverseTransform(const Coordinates &World, Coordinates &Viewport) const {
   if (!pRasterModel_) {
      // valores de error
      Viewport.x_ = 0;
      Viewport.y_ = 0;
      double ww = (window_.lr_.x_ - window_.ul_.x_);
      double wh = (window_.lr_.y_ - window_.ul_.y_);
      if (!initialized_ || ww == 0 || wh == 0) {
         return;
      }
      Viewport.x_ = static_cast<int>( ((viewportW_ * (World.x_ - window_.ul_.x_)) / ww));
      Viewport.y_ = static_cast<int>( ((viewportH_ * (World.y_ - window_.ul_.y_)) / wh));
      REPORT_AND_FAIL("D:Modelo raster del mundo no-inicializado.");
   }
   Viewport = World;
   pRasterModel_->InverseTransform(Viewport);
}

/** Transforma las coordenadas de Viewport en coordenadas de Mundo */
/**
 *  Calcula la transformacion de coordenadas de Viewport (Raster en
 * pantalla/archivo/memoria/etc) a coordenadas de Mundo.
 * \pre Las coordenadas de Viewport se truncan ya que no tiene sentido tomar
 *      coordenadas del Viewport Reales.
 * @param[in] Viewport Coordenada de Viewport
 * @param[out] World Coordenada de mundo
 */
void World::Transform(const Coordinates &Viewport, Coordinates &World) const {
   if (!pRasterModel_) {
      // valores de error
      World.x_ = world_.ul_.x_;
      World.y_ = world_.ul_.y_;
      double ww = (window_.lr_.x_ - window_.ul_.x_);
      double wh = (window_.lr_.y_ - window_.ul_.y_);
      if (!initialized_ || ww == 0 || wh == 0) {
         return;
      }
      World.x_ = window_.ul_.x_ + static_cast<double>(SURI_TRUNC(int, Viewport.x_)) / viewportW_ * ww;
      World.y_ = window_.ul_.y_ + static_cast<double>(SURI_TRUNC(int, Viewport.y_)) / viewportH_ * wh;
      REPORT_AND_FAIL("D:Modelo de Raster del Mundo no inicializado");
   }
   // No trunco porque un vector en terreno 3D puede tener coordenada de viewport no
   // entera: TCK #537
   World = Coordinates(Viewport.x_, Viewport.y_);
   pRasterModel_->Transform(World);
}

/** retorna true si el mundo esta inicializado, viewport!=0 y tiene wkt_ no nulo */
/**
 * Indica si el mundo esta inicializado o no.
 * @return true si el mundo esta inicializado, viewport!=0 y tiene wkt_ no nulo
 * @return false si el mundo no esta inicializado, viewport==0 y tiene wkt_ nulo
 */
bool World::IsInitialized() const {
   return initialized_ && viewportW_ > 0 && viewportH_ > 0 && wkt_ != SPATIAL_REFERENCE_NULL_VALUE;
}

/** operador de igualdad */
bool operator==(const World &Lhs, const World &Rhs) {
   return !(Lhs != Rhs);
}

/** operadore de desigualdad */
bool operator!=(const World &Lhs, const World &Rhs) {
   int lhsvpx, lhsvpy, rhsvpx, rhsvpy;
   Subset lhsworld, rhsworld, lshwindow, rhswindow;
   Lhs.GetViewport(lhsvpx, lhsvpy);
   Rhs.GetViewport(rhsvpx, rhsvpy);
   Lhs.GetWorld(lhsworld);
   Rhs.GetWorld(rhsworld);
   Lhs.GetWindow(lshwindow);
   Rhs.GetWindow(rhswindow);
   return (Lhs.IsInitialized() != Rhs.IsInitialized() || lhsvpx != rhsvpx
         || lhsvpy != rhsvpy || lhsworld != rhsworld || lshwindow != rhswindow
         || Lhs.GetSpatialReference() != Rhs.GetSpatialReference());
}
}
