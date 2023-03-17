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

#ifndef TERRAIN_H_
#define TERRAIN_H_

// Includes standard
#include <GL/glu.h>
#include <string>
#include <vector>
#include <map>

// Includes Suri
#include "suri/Canvas.h"
#include "suri/Coordinates.h"
#include "suri/LayerList.h"
#include "suri/VectorStyle.h"

// Includes Wx

// Includes App

// Defines
/** radio de la esfera. Utilizado para Query3DButton y para dibujar vectores en 3D. */
#define SPHERE_RADIUS 54.0

/** namespace suri */
namespace suri {
class World;

/** Clase Terrain */
/**
 *
 *  Esta clase es responsable de:
 *  - Mantener en memoria los datos del terreno (1 banda)
 *  - Mantener en memoria los datos de la textura (3 bandas)
 *  - Crea la DisplayList OpenGL para el terreno
 *  - Crea la DisplayList OpenGL para los vectores de puntos
 *  - Calcular la altura (valor Z) de las esferas de puntos a partir de la
 *    altura del terreno
 *  - Ejecuta la DisplayList para dibujar el terreno y los vectores
 *  \todo Que la elevacion de la esfera respecto del terreno dependa de la
 *   altura del terreno que la rodea.
 */
class Terrain {
   /** Ctor. de copia */
   Terrain(const Terrain &NewTerrain);

public:
   /** Ctor. default. */
   Terrain(World *pTerrainWorld, Canvas *pTerrainCanvas, World *pTextureWorld,
           Canvas *pTextureCanvas);
   /** Dtor. */
   ~Terrain();

   /** Establece el factor de exageracion. */
   void SetExageration(size_t Exageration) {
      exageration_ = Exageration;
   }
   /** Establece el factor de decimacion de terreno. */
   void SetTerrainFactor(size_t TerrainFactor) {
      terrainFactor_ = TerrainFactor;
   }
   /** Establece el factor de decimacion de la textura. */
   void SetTextureFactor(size_t TextureFactor) {
      textureFactor_ = TextureFactor;
   }
   /** Establece el tamanio del pixel del terreno. */
   void SetPixelSize(float PixelSize) {
      pixelSize_ = PixelSize;
   }

   /** Obtiene el factor de decimado del terreno. */
   size_t GetTerrainFactor() {
      return terrainFactor_;
   }
   /** Obtiene el factor de decimado de la textura. */
   size_t GetTextureFactor() {
      return textureFactor_;
   }
   /** Obtiene el tamanio del pixel del terreno. */
   float GetPixelSize() {
      return pixelSize_;
   }
   /** Obtiene el factor de exageracion. */
   double GetExageration() {
      return exageration_;
   }

   /** Carga el Terreno. */
   void LoadTerrain();
   /** Carga del Bloque de Textura */
   void LoadTextureBlock(size_t, size_t);
   /** Inicializa */
   void InitTexture();
   /** Genera la textura para los puntos de los vectores 3D */
   void InitPointTexture(VectorStyle* pVectorStyle);
   /** Crea el DisplayList del Terreno. */
   void CreateTerrainDisplayList();
   /** Crea el DisplayList del Vector. */
   int CreateVectorDisplayList(suri::Element *pElement);
   /** Resetea el DisplayList del Terreno. */
   void ClearTerrainDisplayList();
   /** Resetea el DisplayList del Vector. */
   void ClearVectorDisplayList(suri::Element *pElement);
   /** Resetea el DisplayList del Vector. */
   void ClearVectorDisplayLists();
   /** Dibuja Terreno y vector. */
   void Draw();
   /** Dibuja el Terreno. */
   void DrawTerrain();
   /** Dibuja Vectores */
   bool DrawVector(Element *pElement, const int DisplayListIndex);
   /** Dibuja Vectores de Tipo Punto */
   bool DrawLayerPoint(Element *pElement, OGRLayer *pLayer,
                       const std::string &SpatialRef, const int DisplayListIndex);
   /** Dibuja Vectores de Tipo Linea */
   bool DrawLayerLine(Element *pElement, OGRLayer *pLayer,
                      const std::string &SpatialRef, const int DisplayListIndex);
   /** Dibuja Vectores de Tipo Poligono */
   bool DrawLayerPolygon(Element *pElement, OGRLayer *pLayer,
                         const std::string &SpatialRef, const int DisplayListIndex);
   /** Dibuja puntos del vector. */
   void DrawPointVectors();
   /** Dibuja una esfera. */
   void DrawSphere(VectorStyle* pVectorStyle, float Radius, float X, float Y, float Z);
   /** Calculo el radio de la Espera que contiene el modelo */
   double GetTerrainRadius();
   /** Calculo del tamanio de la textura */
   int GetTextureSize(double TextureElem_x, double TextureElem_y);
   /** Carga la altura de un vector de puntos */
   void LoadVectorHeight(std::vector<Coordinates> &Vector);
   /** Obtiene la posicion del Terreno en coordenadas de Imagen */
   Coordinates GetRasterPosition(Coordinates TerrainPosition);
   /** Obtiene la posicion del Terreno en coordenadas de Terreno(3D) */
   Coordinates GetTerrainPosition(Coordinates RasterPosition);
   /** Verifica que opengl tenga la lista desplegada */
   bool IsInit();
   void UpdateTexture(World *pTextureWorld, Canvas *pTextureCanvas);
   void GetNormalizationConstant(double& X, double &Y);
   World * GetTerrainWorld(){return pTerrainWorld_;}
   void SetSearchedPoint(Coordinates Point) {searchedPoint_ = Point; }

private:
   bool DrawCrosshair(); /* Dibuja Cruz en el terreno centrada en searchedPoint*/
   /** Encuentra las alturas en en los limites (0,1) */
   void CalculateHeights(Subset &TexturePositions, Subset &MapPositions,
                         double* Heights);

   double normalizationConstantX_; /*! constantes que se usan para normalizar */
   /* terreno en x */
   double normalizationConstantY_; /*! constantes que se usan para normalizar */
   /* terreno en y */
   World *pTerrainWorld_; /*! Mundo para el terreno */
   World *pTextureWorld_; /*! Mundo para la textura */
   Canvas *pTerrainCanvas_; /*! Canvas de memoria para el terreno */
   Canvas *pTextureCanvas_; /*! Canvas de memoria para la textura */
   std::vector<void*> terrain_; /*! Datos del terreno */
   std::vector<void*> texture_; /*! Datos del la textura */
   size_t exageration_; /*! Factor de exageracion */
   size_t terrainFactor_; /*! Factor de decimacion del terreno */
   size_t textureFactor_; /*! Factor de decimacion de la textura */
   size_t textureSize_; /*! Tamanio del mosaico de textura */
   float pixelSize_; /*! Tamnio de pixel del terreno */
   GLuint terrainDisplayList_; /*! DisplayList con el Terreno */
   /*! Cantidad de DisplayList de vectores de puntos */
   std::map<suri::Element*, GLuint> vectorDisplayListsMap_;
   int currentVectorSphereSize_; /*! tamanio de la esfera. */
   GLuint texName_; /*! Nombre de la textura */
   GLuint texSphereName_; /*! Nombre de la textura de punto */
   bool backX_; /*! Flag de textura de borde en x */
   bool backY_; /*! Flag de textura de borde en y */
   bool init_; /*! Flag de inicializacion */
   size_t blocksX_; /*! Cantidad de bloques de textura en x */
   size_t blocksY_; /*! Cantidad de bloques de textura en y */
   Coordinates searchedPoint_; /* Punto al que se intenta llegar con Navegar */
};
}
#endif /*TERRAIN_H_*/
