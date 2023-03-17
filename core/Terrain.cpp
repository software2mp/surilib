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

// Includes standard
#include <cmath>
#include <algorithm>   // por min_element, max_element
#include <vector>
#include <map>

// Includes Suri
#include "Terrain.h"
#include "MemoryCanvas.h"
#include "suri/World.h"
#include "suri/Vector.h"
#include "suri/VectorElement.h"
#include "suri/Dimension.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Subset.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"

// Includes Gdal
#include "ogrsf_frmts.h"

// Includes Wx

// Includes App

// Defines
/** Elevacion de la esfera (respecto de la altura del terreno). */
#define SPHERE_ELEVATION 0
/** cantidad de puntos de la esfera. */
#define SPHERE_POINTS 50
/** Tamano de la textura */
#define TEXTURESIZE 256
/** cantidad de puntos de la esfera en latitud */
#define LATITUDE_POINTS SPHERE_POINTS
/** cantidad de puntos de la esfera en longitud */
#define LONGITUDE_POINTS LATITUDE_POINTS*2
/** constante pi. */
#define PI 3.1415926
/** constantes para normalizar tamano del terreno. */
/** normaliza el tamano del terreno */
#define TERRAIN_SURFACE_NORMALIZED_SIZE 100
/** normaliza la altura del terreno */
#define TERRAIN_SURFACE_NORMALIZED_HEIGHT 0.05

/** namespace suri */
namespace suri {
// ------------------------------------------------------------------------------
/**
 * Constructor.
 * @param[in]	pTerrainWorld: Mundo para el terreno
 * @param[in]	pTerrainCanvas: Canvas de memoria para el terreno
 * @param[in]	pTextureWorld: Mundo para la textura
 * @param[in]	pTextureCanvas: Canvas de memoria para la textura
 * @return instancia de la clase Terrain
 */
Terrain::Terrain(World *pTerrainWorld, Canvas *pTerrainCanvas, World *pTextureWorld,
                 Canvas *pTextureCanvas):searchedPoint_(-1, -1) {
   /*TODO(29/05/2009 - Alejandro): Ver porque no se estan inicializando el
    * resto de las propiedades de la clase y porque no se hace como lista de
    * parametros del ctor.*/
   terrainDisplayList_ = 0;
   normalizationConstantX_ = 1;
   normalizationConstantY_ = 1;
   texSphereName_ = 0;
   texName_ = 0;
   pTerrainWorld_ = pTerrainWorld;
   pTextureWorld_ = pTextureWorld;
   pTerrainCanvas_ = pTerrainCanvas;
   pTextureCanvas_ = pTextureCanvas;
   // vectorDisplayListCount_=0;
   vectorDisplayListsMap_.clear();
   currentVectorSphereSize_ = 0;
   init_ = false;
}

// ------------------------------------------------------------------------------
/**
 *	Destructor.
 */
Terrain::~Terrain() {
   glDeleteTextures(1, &texSphereName_);
   glDeleteTextures(1, &texName_);
}

// -------------------------------TERRENO----------------------------------------
/**
 * Este metodo es el encargado de cargar del Terreno.
 * Carga el vector de terreno con el contenido del Canvas.
 */
void Terrain::LoadTerrain() {
   // El terreno tiene una sola banda
   std::vector<int> band;
   band.push_back(0);

   // Reservo espacio para el terreno
   // el tamanio para el buffer es el del canvas y no el del mundo
   // ver que se comete este error en varios lugares mas
#ifdef __UNUSED_CODE__
   Subset world;
   pTerrainWorld_->GetWorld(world);  // Obtiene el subset del mundo
   Dimension dims(world);  // Obtiene dimension del mundo
#endif
   size_t size = pTerrainCanvas_->GetSizeX() * pTerrainCanvas_->GetSizeY()
         * pTerrainCanvas_->GetDataSize();
   terrain_.push_back(new unsigned char[size]);   // Reserva tamanio en vector

   // Leo los datos
   pTerrainCanvas_->Read(band, terrain_);  // Lee en vector de archivo
}

// ------------------------------------------------------------------------------
/**
 * Carga en el canvas el bloque de la
 * fila y columna indicadas
 * @param[in]	Column: Cantidad de bloques de textura en X
 * @param[in]	Row: Cantidad de bloques de textura en Y
 */
void Terrain::LoadTextureBlock(size_t Column, size_t Row) {
   // Bandas de la textura
   std::vector<int> band;
   band.push_back(2);
   band.push_back(1);
   band.push_back(0);

   size_t size = textureSize_ * textureSize_;

   // Crea un vector de 3 posiciones con la matriz de cada banda en ellas
   for (size_t j = 0; j < 3; j++) {
      texture_.push_back(new unsigned char[size]);
   }

   // Encuentro ventana de textura
   Subset texturewindow;
   pTextureWorld_->GetWindow(texturewindow);
   Dimension dimtexturewindow(texturewindow);
   // Encuentro mundo de terreno/textura
   Subset world;
   pTextureWorld_->GetWorld(world);
   Dimension dimworld(world);

   // Seteo el Window
   double ulx, uly, lrx, lry;
   // Coordenadas del subset en x
   if (Column == blocksX_ - 1 && backX_) {
      lrx = world.lr_.x_;
      ulx = world.lr_.x_ - dimtexturewindow.XSign() * dimtexturewindow.GetWidth();
   } else {
      ulx = world.ul_.x_
            + Column * (dimtexturewindow.XSign() * dimtexturewindow.GetWidth());
      lrx = ulx + (dimtexturewindow.XSign() * dimtexturewindow.GetWidth());
   }

   // Coordenadas del subset en y
   if (Row == blocksY_ - 1 && backY_) {
      lry = world.lr_.y_;
      uly = world.lr_.y_ - dimtexturewindow.YSign() * dimtexturewindow.GetHeight();
   } else {
      uly = world.ul_.y_
            + Row * (dimtexturewindow.YSign() * dimtexturewindow.GetHeight());
      lry = uly + (dimtexturewindow.YSign() * dimtexturewindow.GetHeight());
   }
   Subset subset(ulx, uly, lrx, lry);
   pTextureWorld_->SetWindow(subset);
   // Leo los datos
   pTextureCanvas_->Read(band, texture_);
}

// ------------------------------------------------------------------------------
/**
 * Inicializacion del bloque de textura
 */
void Terrain::InitTexture() {
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glDeleteTextures(1, &texName_);
   glGenTextures(1, &texName_);
   glBindTexture(GL_TEXTURE_2D, texName_);

   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   size_t buffersize = textureSize_ * textureSize_ * 3;
   unsigned char *pbuffer = new unsigned char[buffersize];
   DrawCrosshair();
   unsigned char *pbandbuffer;
   for (size_t j = 0; j < 3; j++) {
      pbandbuffer = (unsigned char*) texture_.back();
      for (size_t i = 0; i < textureSize_ * textureSize_; i++)
         pbuffer[3 * i + j] = pbandbuffer[i];

      delete[] (unsigned char*) texture_.back();
      texture_.pop_back();
   }
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureSize_, textureSize_, 0, GL_RGB,
                GL_UNSIGNED_BYTE, pbuffer);
   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

   delete[] pbuffer;
}




bool Terrain::DrawCrosshair() {
   int viewX = 0;
   int viewY = 0;
   pTerrainWorld_->GetViewport(viewX, viewY);
   if ((searchedPoint_.x_ == -1) || (searchedPoint_.y_ == -1)) return false;
   // Transformo coordenadas view port a  coordenadas imagen.
   searchedPoint_.x_ = (textureSize_*searchedPoint_.x_) / viewX;
   searchedPoint_.y_ = (textureSize_*searchedPoint_.y_) / viewY;
   for (unsigned int i = 0; i < textureSize_; i++) {
      int position = searchedPoint_.y_ * textureSize_ + i;
      for (int band = 0; band < 3; band++) {
          void* tBand = texture_[band];
          char* cBand = static_cast<char*>(tBand);
          cBand[position] = 0;
       }
   }
   for (unsigned int j = 0; j < textureSize_; j++) {
         int position = j * textureSize_ + searchedPoint_.x_;
         for (int band = 0; band < 3; band++) {
             void* tBand = texture_[band];
             char* cBand = static_cast<char*>(tBand);
             cBand[position] = 0;
          }
    }
   return true;
}




// ------------------------------------------------------------------------------
/**
 * Destruccion del displaylist de terreno.
 * Borra estructuras utilizadas para renderizado del terreno.
 */
void Terrain::ClearTerrainDisplayList() {
   if (glIsList(terrainDisplayList_) == GL_TRUE) {
      glDeleteLists(terrainDisplayList_, 1);
      // Borra el terreno
      delete[] (unsigned char*) terrain_.back();
      terrain_.pop_back();
   }
}

// ------------------------------------------------------------------------------
/**
 * Destruccion del displaylist del vector.
 * Borra estructuras utilizadas para renderizado del vector.
 * @param[in] pElement elemento vector.
 */
void Terrain::ClearVectorDisplayList(suri::Element *pElement) {
   // Limpia la Display List del vector
   if (!vectorDisplayListsMap_.empty()
         && !(vectorDisplayListsMap_.find(pElement) == vectorDisplayListsMap_.end())) {
      if (glIsList(vectorDisplayListsMap_[pElement]) == GL_TRUE) {
         glDeleteLists(vectorDisplayListsMap_[pElement], 1);
      }
      vectorDisplayListsMap_.erase(pElement);
   }
}

// ------------------------------------------------------------------------------
/**
 * Destruccion de TODOS los displaylist de vectores
 * Elimina lista de displaylist de vectores.
 */
void Terrain::ClearVectorDisplayLists() {
   if (!vectorDisplayListsMap_.empty()) {
      // Limpia TODAS las Display Lists del vector
      std::map<suri::Element*, GLuint>::iterator it = vectorDisplayListsMap_.begin();
      for (; it != vectorDisplayListsMap_.end(); ++it) {
         if (glIsList(it->second) == GL_TRUE) {
            GLuint i = (it->second);
            glDeleteLists(i, 1);
         }
      }
      vectorDisplayListsMap_.clear();
   }
}

// ------------------------------------------------------------------------------
/**
 * Creacion del DisplayList de terreno.
 * Obtiene datos del terreno y de la textura y genera un displaylist para el
 * terreno a renderizar.
 * Si hay DisplayLists de vectores los destruye
 * \todo (29/05/2009 - Alejandro): Verificar comentario del objetivo del metodo.
 */
void Terrain::CreateTerrainDisplayList() {
   // Si hay DisplayLists de vectores los destruye
   ClearTerrainDisplayList();

   // Genera una lista para terreno
   terrainDisplayList_ = glGenLists(1);
   glNewList(terrainDisplayList_, GL_COMPILE);

   // Cantidad de elementos del Terreno
   Subset subterrain;
   pTerrainWorld_->GetWindow(subterrain);
   Dimension dimterrain(subterrain);

   // Obtengo subsets de mundo y un recuadro de textura
   Subset subtexture;
   Subset subtexturewindow;
   pTerrainWorld_->GetWindow(subtexture);
   pTextureWorld_->GetWorld(subtexture);
   pTextureWorld_->GetWindow(subtexturewindow);
   Dimension dimtexture(subtexture);
   Dimension dimtexturewindow(subtexturewindow);

   // Obtengo cantidad de pixeles en terreno
   int width, height;
   pTerrainWorld_->GetViewport(width, height);
   double terrainelemx = width;
   double terrainelemy = height;

   // Obtengo cantidad de pixeles en recuadro de textura
   pTextureWorld_->GetViewport(width, height);
   double elemtexturex = dimtexture.GetWidth() * static_cast<double>(width)
         / dimtexturewindow.GetWidth();
   double elemtexturey = dimtexture.GetHeight() * static_cast<double>(height)
         / dimtexturewindow.GetHeight();

   // Calculo el tamanio de textura a utilizar
   textureSize_ = GetTextureSize(elemtexturex, elemtexturey);

   // Calculo la cantidad de bloques en x e y a leer
   blocksX_ = (size_t) SURI_TRUNC(int, elemtexturex/textureSize_);
   blocksY_ = (size_t) SURI_TRUNC(int, elemtexturey/textureSize_);

   // El numero de pixeles(double) de textura entre puntos de terreno. Le resto
   // 1 para que el ultimo punto de terreno este en el ultimo punto de textura
   double steptexturex = elemtexturex / (terrainelemx - 1) / textureSize_;
   double steptexturey = elemtexturey / (terrainelemy - 1) / textureSize_;

   // Veo si hay bloque de borde
   backX_ = false;
   backY_ = false;
   // Espacio del ultimo bloque
   double lastblocktextureoffsetx = 0, lastblocktextureoffsety = 0;
   if (((size_t) elemtexturex % textureSize_) != 0) {
      backX_ = true;                                               // Flag de Borde en x
      lastblocktextureoffsetx = 1.0
            - (elemtexturex - textureSize_ * blocksX_) / static_cast<double>(textureSize_);
      blocksX_++;                                             // Incremento el numero de
                                                              // bloques en x
   }
   if (((size_t) elemtexturey % textureSize_) != 0) {
      backY_ = true;                                               // Flag de Borde en y
      lastblocktextureoffsety = 1.0
            - (elemtexturey - textureSize_ * blocksY_) / static_cast<double>(textureSize_);
      blocksY_++;                                                  // Flag de Borde en y
   }

   // En pantalla el tamano estara entre 0
   normalizationConstantX_ = terrainelemx / TERRAIN_SURFACE_NORMALIZED_SIZE;
   normalizationConstantY_ = terrainelemx / TERRAIN_SURFACE_NORMALIZED_SIZE;

   // Variables que recuerdan posicion en terreno
   int terreinline = 1;
   int terreinpixel = 1;
   int blockfirstline = 0;
   int blockfirstpixel = 0;

   // vector con alturas
   double heights[4];

   // Obtengo el Terreno
   float *pterrainbuffer = reinterpret_cast<float *>(terrain_.back());
   for (size_t j = 0; j < blocksY_; j++) {
      blockfirstline = terreinline - 1;
      terreinpixel = 1;
      for (size_t i = 0; i < blocksX_; i++) {
         blockfirstpixel = terreinpixel - 1;

         LoadTextureBlock(i, j);
         InitTexture();

         // Agrego el offset si es el ultimo bloque
         float texoffsety = 0;  // terreinline*steptexturey-textureSize_*j;
         if ((j == blocksY_ - 1) && (backY_)) {
            texoffsety = lastblocktextureoffsety;
         }

         // Calculo linea inicial de punto de terreno en bloque de textura
         float texposy = blockfirstline * steptexturey - j;
         terreinline = blockfirstline;

         Subset texturesubset;
         Subset mapsubset;
         while ((texposy < 1) && (terreinline < terrainelemy - 1)) {
            // Agrego el offset si es el ultimo bloque
            float texoffsetx = 0;
            if ((i == blocksX_ - 1) && (backX_)) {
               texoffsetx = lastblocktextureoffsetx;
            }

            // Calculo pixel de punto de terreno en bloque de textura
            float texposx = blockfirstpixel * steptexturex - i;
            terreinpixel = blockfirstpixel;

            // Cargo rectaguno en OpenGl
            glBegin(GL_TRIANGLE_STRIP);
            while ((texposx < 1) && (terreinpixel < terrainelemx - 1)) {
               // Cargo subset con posiciones puntos textura
               texturesubset = Subset(texposx, texposy, texposx + steptexturex,
                                      texposy + steptexturey);

               // Cargo subset con posiciones puntos terreno
               mapsubset = Subset(terreinpixel - terrainelemx / 2,
                                  terreinline - terrainelemy / 2,
                                  terreinpixel + 1 - terrainelemx / 2,
                                  terreinline + 1 - terrainelemy / 2);

               // Calculo alturas
               size_t pos = (size_t) (terrainelemx * terreinline + terreinpixel);
               heights[0] = pterrainbuffer[pos];
               pos = (size_t) (terrainelemx * terreinline + terreinpixel + 1);
               heights[1] = pterrainbuffer[pos];
               pos = (size_t) (terrainelemx * (terreinline + 1) + terreinpixel);
               heights[2] = pterrainbuffer[pos];
               pos = (size_t) (terrainelemx * (terreinline + 1) + terreinpixel + 1);
               heights[3] = pterrainbuffer[pos];

               // Calculo los nuevos puntos
               CalculateHeights(texturesubset, mapsubset, heights);

               // Invierto el mapa para que quede igual que en viewer 2D
               mapsubset.ul_.y_ = -mapsubset.ul_.y_;
               mapsubset.lr_.y_ = -mapsubset.lr_.y_;
               // Genero los vertex en mapa
               // vertex 0
               glTexCoord2f(texturesubset.ul_.x_ + texoffsetx,
                            texturesubset.ul_.y_ + texoffsety);
               glVertex3f(
                     mapsubset.ul_.x_ / normalizationConstantX_,
                     mapsubset.ul_.y_ / normalizationConstantY_,
                     (exageration_ * heights[0] * TERRAIN_SURFACE_NORMALIZED_HEIGHT
                           / pixelSize_));

               // vertex 1
               glTexCoord2f(texturesubset.lr_.x_ + texoffsetx,
                            texturesubset.ul_.y_ + texoffsety);
               glVertex3f(
                     mapsubset.lr_.x_ / normalizationConstantX_,
                     mapsubset.ul_.y_ / normalizationConstantY_,
                     (exageration_ * heights[1] * TERRAIN_SURFACE_NORMALIZED_HEIGHT
                           / pixelSize_));

               // vertex 2
               glTexCoord2f(texturesubset.ul_.x_ + texoffsetx,
                            texturesubset.lr_.y_ + texoffsety);
               glVertex3f(
                     mapsubset.ul_.x_ / normalizationConstantX_,
                     mapsubset.lr_.y_ / normalizationConstantY_,
                     (exageration_ * heights[2] * TERRAIN_SURFACE_NORMALIZED_HEIGHT
                           / pixelSize_));
               // vertex 3
               glTexCoord2f(texturesubset.lr_.x_ + texoffsetx,
                            texturesubset.lr_.y_ + texoffsety);
               glVertex3f(
                     mapsubset.lr_.x_ / normalizationConstantX_,
                     mapsubset.lr_.y_ / normalizationConstantY_,
                     (exageration_ * heights[3] * TERRAIN_SURFACE_NORMALIZED_HEIGHT
                           / pixelSize_));

               texposx += steptexturex;
               terreinpixel++;
            }
            glEnd();
            texposy += steptexturey;
            terreinline++;
         }
      }
   }

   // Cierra el DisplayList
   glEndList();
}

void Terrain::GetNormalizationConstant(double& X, double &Y) {
   X = normalizationConstantX_;
   Y = normalizationConstantY_;
}
// ------------------------------------------------------------------------------
/**
 * Este metodo es el encargado de invocar a los metodos que renderizan Terreno y
 * vectores.
 */
void Terrain::Draw() {
   // Terreno
   DrawTerrain();

   // Vectores de punto
   DrawPointVectors();
}

/**
 * Renderiza el terreno.
 */
void Terrain::DrawTerrain() {
   if (glIsList(terrainDisplayList_) == GL_FALSE) {
      REPORT_AND_RETURN("D:terrainDisplayList_ es invalida");
   }
   glCallList(terrainDisplayList_);
}

/**
 * Recorre todas las capas de un vector e invoca al metodo que corresponda para
 * que dibuje dicha capa.
 * \pre pElement debe ser no nulo, casteable a VectorElement y estar Activo.
 * @param[in] pElement: puntero al VectorElement a dibujar.
 * @param[in] DisplayListIndex indice de la capa
 * @return true en caso de poder dibujar el vector.
 * @return false en caso de error al dibujar el vector.
 */
bool Terrain::DrawVector(Element *pElement, const int DisplayListIndex) {
   if (!pElement || !pElement->IsActive() ||
         dynamic_cast<suri::VectorElement*>(pElement) == NULL)
      return false;

   Vector *pvector = Vector::Open(pElement->GetUrl().c_str() );
   if ( !pvector ) {
      return false;
   }

   // recorro las capas del vector y dibujo dependiendo el tipo de capa.
   for ( int layeridx = 0; layeridx < pvector->GetLayerCount(); layeridx++ )
   switch ( pvector->GetLayerType(layeridx) ) {
      case Vector::Point:
      DrawLayerPoint(pElement, pvector->GetLayer(layeridx),
            pvector->GetLayerSR(layeridx).c_str(), DisplayListIndex);
      break;
      case Vector::Line:
      DrawLayerLine(pElement, pvector->GetLayer(layeridx),
            pvector->GetLayerSR(layeridx).c_str(), DisplayListIndex);
      break;
      case Vector::Polygon:
      DrawLayerPolygon(pElement, pvector->GetLayer(layeridx),
            pvector->GetLayerSR(layeridx).c_str(), DisplayListIndex);
      break;
      default:
      REPORT_DEBUG("D: Capa %d del tipo: %s ignorada.", layeridx,
            pvector->GetVectorTypeAsString(
                  pvector->GetLayerType(layeridx) ).c_str() );
      break;
   }
   Vector::Close(pvector);
   return true;
}

/**
 * Dibuja una capa de tipo Punto. Recorre los feature de la capa enviando a
 * dibujar todos los puntos que la componen.
 * \pre	pLayer debe ser de tipo Punto.
 * \pre	debe existir una transformacion de coordenadas entre el sistema de
 * referencia de la Capa y el del Terreno.
 * @param[in]	pElement puntero al VectorElement a dibujar.
 * @param[in]	pLayer puntero a la capa del vector a dibujar
 * @param[in]	SpatialRef referencia espacial de la capa del vector.
 * @param[in]	DisplayListIndex indice del DisplayList creado para dibujar el vector.
 * (\see Terrain::CreateVectorDisplayList)
 * @return	true en caso de poder dibujar la capa.
 * @return	false en caso de que no exista una transformacion de coordenadas.
 */
bool Terrain::DrawLayerPoint(Element *pElement, OGRLayer *pLayer,
                             const std::string &SpatialRef,
                             const int DisplayListIndex) {
   // Verifica que existe transformacion
   ParameterCollection params;
     params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                  SpatialRef);
     params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                  pTerrainWorld_->GetSpatialReference().c_str());
     TransformationFactory* pfactory =
           TransformationFactoryBuilder::Build();
     CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact,
                                              params);
     TransformationFactoryBuilder::Release(pfactory);
   if (!pct) {
      REPORT_DEBUG("D: Referencias espaciales incompatibles %s,%s",
                   SpatialRef.c_str(), pTerrainWorld_->GetSpatialReference().c_str());
      return false;
   }

   std::vector<Coordinates> coordvec;
   int decimatedwidth, decimatedheight;
   pTerrainWorld_->GetViewport(decimatedwidth, decimatedheight);

   // Resetea el reading de la capa para que apunte al primer feature
   pLayer->ResetReading();
   OGRFeature *pfeature = pLayer->GetNextFeature();  // Apunta al primer feature
   while (pfeature != NULL) {
      OGRPoint * ppoint = dynamic_cast<OGRPoint *>(pfeature->GetGeometryRef());
      if (ppoint) {
         // Georreferencia
         Coordinates point(ppoint->getX(), ppoint->getY());
         pct->Transform(point);
         // Dibuja
         double dx = point.x_;
         double dy = point.y_;
         // Si la esfera se va del tamanio del terreno no se renderiza
         if (dx < decimatedwidth || dy < decimatedheight || dx > 0 || dy > 0) {
            Coordinates rasterposition(dx, dy);
            coordvec.push_back(rasterposition);
         }
      } else {
         REPORT_DEBUG("D: Error al obtener la geometria del feature");
      }

      OGRFeature::DestroyFeature(pfeature);
      pfeature = pLayer->GetNextFeature();
   }

   delete pct;

   // Crea los display list
   glNewList(DisplayListIndex, GL_COMPILE);

   // Obtiene el estilo del elemento
   VectorElement* pvelem = dynamic_cast<suri::VectorElement*>(pElement);
   VectorStyle* pvstyle = VectorStyle::Create(pvelem->GetStyle(pvelem->GetActiveLayer()));
   if (!pvstyle)
      return false;
   // Obtiene el factor de las esferas. DEBE IR ANTES DE LoadVectorHeight
   currentVectorSphereSize_ = pvstyle->GetSymbol()->size_;

   // Carga la altura en los vectores
   LoadVectorHeight(coordvec);

   size_t vectsize = coordvec.size();
   for (size_t i = 0; i < vectsize; i++) {
      Coordinates vpoint;
      vpoint = coordvec.back();
      coordvec.pop_back();
      /*Se deja de usar terrainFactor_ para ajustar el tamano de la esfera,
       * que ahora queda constante.
       */
      float radius = currentVectorSphereSize_ * SPHERE_RADIUS
            / static_cast<float>(pixelSize_);
      DrawSphere(pvstyle, radius, vpoint.x_ / normalizationConstantX_,
                 vpoint.y_ / normalizationConstantY_, vpoint.z_);
   }

   // Borra el estilo
   VectorStyle::Destroy(pvstyle);
   glEndList();
   return true;
}

/**
 * Dibuja Vectores de Tipo Linea
 * @param[in] pElement puntero al VectorElement a dibujar.
 * @param[in] pLayer puntero a la capa
 * @param[in] SpatialRef referencia espacial de la capa
 * @param[in] DisplayListIndex indice de la capa
 * @return true
 * \note este metodo solo hace un REPORT_DEBUG, no utiliza los parametros.
 */
bool Terrain::DrawLayerLine(Element *pElement, OGRLayer *pLayer,
                            const std::string &SpatialRef, const int DisplayListIndex) {
   REPORT_DEBUG("D: Capa de Tipo Linea ignorada.");
   return true;
}

/**
 * Dibuja Vectores de Tipo Poligono
 * @param[in] pElement puntero al VectorElement a dibujar.
 * @param[in] pLayer puntero a la capa
 * @param[in] SpatialRef referencia espacial de la capa
 * @param[in] DisplayListIndex indice de la capa
 * @return true
 * \note este metodo solo hace un REPORT_DEBUG, no utiliza los parametros.
 * @return true
 */
bool Terrain::DrawLayerPolygon(Element *pElement, OGRLayer *pLayer,
                               const std::string &SpatialRef,
                               const int DisplayListIndex) {
   REPORT_DEBUG("D: Capa de Tipo Poligono ignorada.");
   return true;
}

/**
 * Renderiza Vectores de Punto
 */
void Terrain::DrawPointVectors() {
   // Dibuja la Display Lists del vector
   std::map<suri::Element*, GLuint>::iterator it = vectorDisplayListsMap_.begin();
   for (; it != vectorDisplayListsMap_.end(); ++it) {
      if (glIsList(it->second) == GL_TRUE) {
         glCallList(it->second);
      } else {
         REPORT_DEBUG("D:VectorDisplayList %d es invalida", it->second);
      }
   }
}

// ------------------------------------------------------------------------------

/**
 * Este metodo calcula el radio de la esfera que contiene al terreno.
 * @return devuelve un double que es el radio de la esfera que contiene al
 * terreno.
 */
double Terrain::GetTerrainRadius() {
   return TERRAIN_SURFACE_NORMALIZED_SIZE;
}

// ------------------------------------------------------------------------------
/**
 * Calcula tamanio de la textura
 * @param[in]	TextureElemX: cantidad de elementos de Textura en X
 * @param[in]	TextureElemY: cantidad de elementos de Textura en Y
 * @return tamanio de la textura.
 */
int Terrain::GetTextureSize(double TextureElemX, double TextureElemY) {
   // Tamano maximo de textura que soporta la implementacion de OpenGL
   GLint texSize = 0;
   glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);

   // Asigno como tamanio de textura aquel valor que sea menor a la cantidad
   // de elementos de textura en x e y
   while ((long) TextureElemX < texSize || (long) TextureElemY < texSize)
      texSize = texSize / 2;

   return texSize;
}

// -----------------------------Vectores----------------------------------------
/**
 * Dibuja una esfera con centro en CenterCoordX, CenterCoordY, CenterCoordZ y
 * radio Radius, utilizando el estilo del vector para la textura.
 * @param[in]	pVectorStyle: estilo del vector.
 * @param[in]	Radius: radio de la esfera.
 * @param[in]	CenterCoordX: coordenada del centro de la esfera en X
 * @param[in]	CenterCoordY: coordenada del centro de la esfera en Y
 * @param[in]	CenterCoordZ: coordenada del centro de la esfera en Z
 */
void Terrain::DrawSphere(VectorStyle* pVectorStyle, float Radius, float CenterCoordX,
                         float CenterCoordY, float CenterCoordZ) {
   InitPointTexture(pVectorStyle);

   float n = LATITUDE_POINTS;
   float step = PI / n;
   float theta1, theta2, theta3;

   for (size_t fil = 0; fil < LATITUDE_POINTS; fil++) {
      theta1 = fil * step - PI / 2.0;
      theta2 = (fil + 1) * step - PI / 2.0;

      glBegin(GL_QUAD_STRIP);

      float x, y, z;
      for (size_t col = 0; col <= LONGITUDE_POINTS; col++) {
         theta3 = col * step;

         x = CenterCoordX + Radius * (cos(theta2) * cos(theta3));
         y = CenterCoordY + Radius * (cos(theta2) * sin(theta3));
         z = CenterCoordZ - Radius * (sin(theta2));

         glTexCoord2f(col % LONGITUDE_POINTS / static_cast<double>(LONGITUDE_POINTS),
                      (fil + 1) / static_cast<double>(LATITUDE_POINTS));
         glVertex3f(x, y, z);

         x = CenterCoordX + Radius * (cos(theta1) * cos(theta3));
         y = CenterCoordY + Radius * (cos(theta1) * sin(theta3));
         z = CenterCoordZ - Radius * (sin(theta1));

         glTexCoord2f(col % LONGITUDE_POINTS / static_cast<double>(LONGITUDE_POINTS),
                      fil / static_cast<double>(LATITUDE_POINTS));
         glVertex3f(x, y, z);
      }
      glEnd();
   }
}

// ------------------------------------------------------------------------------
/**
 * Genera la textura para los puntos de los vectores 3D, utilizando los colores
 * establecidos en el estilo del vector.
 * @param[in]	pVectorStyle: estilo del vector.
 */
void Terrain::InitPointTexture(VectorStyle* pVectorStyle) {
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glDeleteTextures(1, &texSphereName_);
   glGenTextures(1, &texSphereName_);
   glBindTexture(GL_TEXTURE_2D, texSphereName_);

   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   unsigned char *pbuffer = new unsigned char[TEXTURESIZE * TEXTURESIZE * 3];
   unsigned char red = pVectorStyle->GetSymbol()->color_.red_, green =
         pVectorStyle->GetSymbol()->color_.green_, blue =
         pVectorStyle->GetSymbol()->color_.blue_;
   for (size_t j = 0; j < TEXTURESIZE; j++)
      for (size_t i = 0; i < TEXTURESIZE; i++) {
         pbuffer[3 * j * TEXTURESIZE + 3 * i] = red - (red * j) / 255;  // red
         pbuffer[3 * j * TEXTURESIZE + 3 * i + 1] = green - (green * j) / 255;  // green
         pbuffer[3 * j * TEXTURESIZE + 3 * i + 2] = blue - (blue * j) / 255;  // blue
      }

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEXTURESIZE, TEXTURESIZE, 0, GL_RGB,
                GL_UNSIGNED_BYTE, pbuffer);
   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

   delete[] pbuffer;
}

// ------------------------------------------------------------------------------
/**
 * Crea los DisplayList para los vectores.
 * \pre	si hay DisplayLists de Vectores los destruye.
 * \post	agrega DisplayList generado a mapa de DisplayList.
 * @param[in]	pElement: elemento vector.
 * @return indice del DisplayList generado.
 */
int Terrain::CreateVectorDisplayList(suri::Element *pElement) {
   // Limpia la lista si existe
   ClearVectorDisplayList(pElement);
   // Genera un conjunto de DisplayList (con solo 1 DisplayList)
   GLint listindex = glGenLists(1);
   // Genera el DisplayList del vector
   vectorDisplayListsMap_.insert(std::make_pair(pElement, listindex));

   return listindex;
}

// ------------------------------------------------------------------------------
/**
 * Carga la altura de un vector de puntos a partir de las coordenadas en X e Y.
 * Si la coordenada calculada no se encuentra dentro del terreno, se elimina
 * dicha coordenada.
 * @param[in]	Vector: vector con coordenadas(x, y) del vector.
 * @param[out]	Vector: vector con coordenadas(x, y, z) del vector.
 * \todo (29/05/2009 - Alejandro): Verificar comentario.
 */
void Terrain::LoadVectorHeight(std::vector<Coordinates> &Vector) {
   // Obtengo el tamanio del Terreno

#ifdef __UNUSED_CODE__
   size_t s = terrain_.size();
   s = s;
#endif

   float *pterrainbuffer = reinterpret_cast<float*>(terrain_.back());

   int decimatedwidth, decimatedheight;
   pTerrainWorld_->GetViewport(decimatedwidth, decimatedheight);
   Coordinates point;
   std::vector<Coordinates> validcoordinates;
   Coordinates temppoint;
   for (size_t j = 0; j < Vector.size(); j++) {
      point = Vector[j];
      pTerrainWorld_->InverseTransform(point, temppoint);
      point.x_ = temppoint.x_;
      point.y_ = temppoint.y_;
      // Calcula la posicion dentro del vector para la coordenada z
      size_t pos = decimatedwidth * (size_t) point.y_ + (size_t) point.x_;
      /*Valido que las coordenadas esten dentro del terreno. */
      if (pos < (size_t) (pTerrainCanvas_->GetSizeX() * pTerrainCanvas_->GetSizeY())
            && (point.x_ < pTerrainCanvas_->GetSizeX())
            && (point.y_ < pTerrainCanvas_->GetSizeY())) {
         // Calcula la coordenada en sistema opengl
         point.x_ = point.x_ - decimatedwidth / 2;
         point.y_ = decimatedheight / 2 - point.y_;

         float radius = currentVectorSphereSize_ * SPHERE_RADIUS
               / static_cast<float>(pixelSize_);

         // Calcula la altura de la esfera, en base a la altura del terreno.
         point.z_ = (exageration_ * pterrainbuffer[pos]
               * TERRAIN_SURFACE_NORMALIZED_HEIGHT / static_cast<float>(pixelSize_))
               + radius + SPHERE_ELEVATION;

         validcoordinates.push_back(point);
      }
   }
   Vector = validcoordinates;
}

/**
 * Transforma las coordenadas del sistema 3D a coordenadas raster.
 * Transforma las coordenadas 3D a sistema de viewport y despues transforma
 * a coordendas de mundo del terreno.
 * \pre las coordenadas TerrainPosition son de acuerdo al terreno decimado.
 * @param[in]	TerrainPosition: coordenadas de posicion del Terreno.
 * @return coordenadas de posicion del terreno en coordenadas de imagen.
 * \todo (29/05/2009 - Alejandro): Verificar comentario.
 */
Coordinates Terrain::GetRasterPosition(Coordinates TerrainPosition) {
   int width, height;
   pTerrainWorld_->GetViewport(width, height);
   Coordinates tempcoord((TerrainPosition.x_ * normalizationConstantX_ + width / 2),
                         (-TerrainPosition.y_ * normalizationConstantY_ + height / 2));
   Coordinates rasterposition;
   if (TerrainPosition.x_  == 0)
      REPORT_DEBUG("GetRasterPosition TerrainPosition.x_ == 0");
   if (TerrainPosition.y_  == 0)
      REPORT_DEBUG("GetRasterPosition TerrainPosition.y_ == 0");
   if (width  == 0)
      REPORT_DEBUG("GetRasterPosition width == 0");
   if (height  == 0)
      REPORT_DEBUG("GetRasterPosition height == 0");
   if (normalizationConstantX_  == 0)
      REPORT_DEBUG("GetRasterPosition normalizationConstantX_ == 0");
   if (normalizationConstantY_  == 0)
      REPORT_DEBUG("GetRasterPosition normalizationConstantY_ == 0");
   pTerrainWorld_->Transform(tempcoord, rasterposition);
   rasterposition.z_ = 0;

   return rasterposition;
}

/**
 * Transforma las coordenadas raster a coordenadas sistema 3D. No usa decimado
 * para ninguna de las 2 coordenadas.
 * Despues de transformar las coordenadas de mundo a viewport, las vuelve a
 * transformar para que esten en sistema 3D(desplaza y normaliza).
 * @param[in]	RasterPosition: posicion del Terreno en coordenadas de Imagen.
 * @return posicion del Terreno en coordenadas de Terreno..
 */
Coordinates Terrain::GetTerrainPosition(Coordinates RasterPosition) {
   // Busco tam viewport
   int width, height;
   pTerrainWorld_->GetViewport(width, height);

   // Calculo RasterPosition en coord de viewport
   Coordinates viewportposition;
   pTerrainWorld_->InverseTransform(RasterPosition, viewportposition);

   // Normalizo coordenadas y las centro
   Coordinates terrainposition;
   terrainposition.x_ = (viewportposition.x_ - width / 2) / normalizationConstantX_;
   terrainposition.y_ = -(viewportposition.y_ - height / 2) / normalizationConstantY_;
   terrainposition.z_ = 0;

   return terrainposition;
}

/**
 * \todo (29/05/2009 - Alejandro): Agregar comentario.
 * @return true si terrainDisplayList_ es el nombre de un displaylist
 * @return true si terrainDisplayList_ no es el nombre de un displaylist
 * \see http://www.opengl.org/sdk/docs/man/xhtml/glIsList.xml
 */
bool Terrain::IsInit() {
   if (glIsList(terrainDisplayList_) == GL_TRUE) {
      return true;
   } else {
      return false;
   }
}

/**
 * Encuentra las alturas en en los limites (0,1) y cambia las posiciones de
 * ser necesario.
 * Modifica las posiciones del mapa.Como el ancho del subset de mapa es 1
 * solo divide por el ancho del subset de textura para calcular recorte a mapa.
 * @param[in] TexturePositions posiciones de los puntos de terreno en la textura
 * @param[out] TexturePositions Si las posiciones estaban fuera de (1,0) las
 * modifica
 * @param[in] MapPositions puntos en mapa de los extremos del pixel a dibujar. Coinciden con
 * numero pixel y linea de terreno
 * @param[out] MapPositions Si se modifico la textura por estar fuera de rengo se modifica
 * los extremos del mapa que se van a dibujar
 * @param[in] Heights Alturas en los extremos del pixel a dibujar en orden
 * ul, ur, ll, lr.
 * @param[out] Heights Si se modifico el pixel a dibujar se relaculan las
 * alturas para ese punto. Aprox lineal.
 */
void Terrain::CalculateHeights(Subset &TexturePositions, Subset &MapPositions,
                               double* Heights) {
   Subset positions = Intersect(TexturePositions, Subset(0, 0, 1, 1));
   // Recalculo alturas si x fuera de rango
   if (TexturePositions.ul_.x_ < 0) {
      Heights[0] = (Heights[0] + Heights[1]) / 2;
      Heights[2] = (Heights[2] + Heights[3]) / 2;
      MapPositions.ul_.x_ = MapPositions.ul_.x_
            - TexturePositions.ul_.x_
                  / (TexturePositions.lr_.x_ - TexturePositions.ul_.x_);
   }
   if (TexturePositions.lr_.x_ > 1) {
      Heights[1] = (Heights[0] + Heights[1]) / 2;
      Heights[3] = (Heights[2] + Heights[3]) / 2;
      MapPositions.lr_.x_ = MapPositions.lr_.x_
            - (TexturePositions.lr_.x_ - 1)
                  / (TexturePositions.lr_.x_ - TexturePositions.ul_.x_);
   }
   // Recalculo alturas si y fuera de rango
   if (TexturePositions.ul_.y_ < 0) {
      Heights[0] = (Heights[0] + Heights[2]) / 2;
      Heights[1] = (Heights[1] + Heights[3]) / 2;
      MapPositions.ul_.y_ = MapPositions.ul_.y_
            - TexturePositions.ul_.y_
                  / (TexturePositions.lr_.y_ - TexturePositions.ul_.y_);
   }
   if (TexturePositions.lr_.y_ > 1) {
      Heights[2] = (Heights[0] + Heights[2]) / 2;
      Heights[3] = (Heights[1] + Heights[3]) / 2;
      MapPositions.lr_.y_ = MapPositions.lr_.y_
            - (TexturePositions.lr_.y_ - 1)
                  / (TexturePositions.lr_.y_ - TexturePositions.ul_.y_);
   }
   // Cambio posiciones
   TexturePositions = positions;
}

/** 
 *  Actualiza las texturas asociadas al terreno 
 *  @param[in] pTextureWorld configuracion espacial para las texturas
 *  @param[in] pTextureCanvas canvas donde se dibujan las texturas
 */
void Terrain::UpdateTexture(World *pTextureWorld, Canvas *pTextureCanvas) {
   pTextureCanvas_ = pTextureCanvas;
   pTextureWorld_ = pTextureWorld;
   if (glIsList(terrainDisplayList_) == GL_TRUE)
      glDeleteLists(terrainDisplayList_, 1);
   CreateTerrainDisplayList();
}

}  // namespace suri
