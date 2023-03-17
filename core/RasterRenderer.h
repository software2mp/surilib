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

#ifndef RASTERRENDERER_H_
#define RASTERRENDERER_H_

// Includes standard
#include <vector>
#include <deque>
#include <string>

// Includes suri
#include "suri/Renderer.h"
#include "suri/Subset.h"
#include "suri/Option.h"

// forwards
class wxXmlNode;

/** namespace suri */
namespace suri {
class Image;
class Element;

/** Renderer que lee un raster y lo vuelca en un canvas */
/**
 *  Esta especializacion de la clase Renderer tiene como objetivo leer una
 * combinacion de bandas de un archivo de imagen y volcarla a un Canvas.
 * Si la relacion Window/Viewport lo exige, realiza un decimado de los datos.
 * \note hereda de renderer para formar parte del pipeline de renderizacion
 */
class RasterRenderer : public Renderer {
   int prawmeta;

   /** Ctor. de Copia. */
   RasterRenderer(const RasterRenderer &RasterRenderer);

public:
   /** ctor */
   RasterRenderer();
   /** dtor */
   virtual ~RasterRenderer();
// ------------------------- ESTRUCTURA DE DATOS ----------------------------
   /** Parametros de RasterRenderer(Tipo de lectura, url, bandas, etc.) */
   class Parameters {
   public:
      // TODO(Gabriel 08/08/13): Se vuelve a poner en true ya que de otra forma no
      // funcionaba el emnascaramiento del modo GIS. El que este en false
      // hace que vuelva a surgir el defecto del TCK #4526
      Parameters():generateMask_(false), noDataValue_(0.0) {}
      /** enum con metodos de lectura que se usa para leer imagen */
      typedef enum {
         Decimate, Aggregate
      } ReadFunctionType;
      std::string imageUrl_; /*! info de URL en nodo */
      std::vector<int> bandCombination_; /*! que bandas se van a renderizar */
      ReadFunctionType readFunction_; /*! forma de leer pixeles al decimar */
      std::string rasterModel_; /*! info contexto de un SPATIAL_REFERENCE_NODE */
      std::string spatialModel_; /*! info contexto de un RASTER_SPATIAL_MODEL_NODE */
      bool generateMask_;  /*! Generar mascara a partir de datos raster */
      double noDataValue_; /*! Valor a tomar como dato no valido */
      /** mapa para metadatos de raster crudos **/
      Option rawMetadata_;
   };
// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
   /** genera un nodo XML a partir de los parametros */
   static wxXmlNode *GetXmlNode(const Parameters &RenderParameters);
   /** genera los parametros a partir de un nodo */
   static Parameters GetParameters(const wxXmlNode *pNode);
   /* Busca el valor del valor no valido */
   static void GetNoValidDnValue(Parameters &params, wxXmlNode *pElementNode);
   /* Busca el valor para la Referencia  */
   static void GetReferenceNode(Parameters &params, wxXmlNode *pElementNode);
   // -------------------------- METODO DE CREACION ----------------------------
   /** creador + inicializador */
   virtual RasterRenderer *Create(Element *pElement, Renderer *pPreviousRenderer) const;
   /** Nombre del renderizador == al nombre del nodo */
   virtual std::string CreatedNode() const;
// ----------------------- METODOS DE RENDERIZACION -------------------------
   /** Renderiza el elemento dado un World en un Canvas */
   virtual bool Render(const World *pWorldWindow, Canvas* pCanvas, Mask* pMask);
   /** Obtiene el "bounding box" del elemento renderizado */
   virtual void GetBoundingBox(const World *pWorld, double &Ulx, double &Uly,
                               double &Lrx, double &Lry);
   /** Obtiene los parametros de entrada del renderer */
   virtual void GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
                                   std::string &DataType) const;
   /** Obtiene los parametros de salida del renderer */
   virtual void GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                    std::string &DataType) const;
// ----------------------- METODOS DE ACTUALIZACION -------------------------
   /** Actualizacion de parametros para representar un elemento */
   virtual void Update(Element *pElement);
   /** Indica si el renderizador tiene cambios para aplicar */
   virtual bool IsDirty();

   /**
    * Genera xml que requiere GDAL para abrir una imagen.
    * NO USAR. Este metodo existe hasta que se envie funcionalidad
    * de parsear uri a Image.
    */
   static std::string GenerateImageId(const std::string &ImageId);

protected:
   /** Metodo auxiliar para extraer informacion de metadata en el xml **/
   static void GetRasterMetadata(wxXmlNode* pMetadataNode,
                                 Parameters& Param);
   friend class StackingRenderer;
   /** Lee los datos de la imagen */
   virtual bool ReadImageData(Image* pImage, Canvas* pCanvas, Mask* pMask,
                                          int Ulx, int Uly, int Lrx, int Lry,
                                          int SubsetWidth, int SubsetHeight,
                                          int ImgOffsetX, int ImgOffsetY);
   /** Genera la mascara en funcion de los datos */
   virtual void FillMaskFromRasterData(std::vector<void*> &RasterData,
                                             const std::string &RasterDataType,
                                             Mask *pMask) const;
   /** Abre la parte de la imagen que se quire desplegar */
   virtual bool UpdateImage(const World* pWorldWindow);
   /** Valida que los datos en clase Parameters sean congruentes con la imagen */
   bool ValidateParameters(const Parameters &Parameters);


   Image *pImage_; /*! Imagen asociada */
   Image *pMask_; /*! Mascara de la imagen */
   bool changed_; /*! flag para determinar en Update si cambio algun parametro */
   Parameters parameters_; /*! parametros de renderizacion (visualizacion) */
};
}

#endif /*RASTERRENDERER_H_*/
