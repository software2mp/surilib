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

#ifndef MAPRENDERER_H_
#define MAPRENDERER_H_

// Includes standard
#include <string>
#include <vector>

// Includes Suri
#include "suri/Renderer.h"
#include "suri/Canvas.h"
#include "suri/LayerList.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Configuration.h"

// Includes Wx

// Includes App

// Defines

/** Esquina en que aparece la leyenda */
#define LEGEND_CORNER 3
/** Ancho de la leyenda (en pixel) */
#define LEGEND_WIDTH 274
/** Distancia al borde (en pixel) */
#define SPACE_TO_BORDER 5
/** Separacion del subrectangulo negro de la leyenda */
#define SUB_RECT_SEPARATION 5
/** Ancho del preview de la leyenda */
#define LEGEND_PREVIEW_WIDTH 16
/** Alto del preview de la leyenda */
#define LEGEND_PREVIEW_HEIGHT LEGEND_PREVIEW_WIDTH
/** Espacio entre los items */
#define LEGEND_SPACER_ITEMS 4
/** Tamanio de letra de la leyenda */
#define LEGEND_FONT_SIZE 9
/** Tipo de letra de la leyenda */
#define LEGEND_FONT_FAMILY wxFONTFAMILY_DEFAULT
/** Estilo de letra de la leyenda */
#define LEGEND_FONT_STYLE wxFONTSTYLE_NORMAL
/** Ancho de la fuente de la leyenda */
#define LEGEND_FONT_WEIGHT wxFONTWEIGHT_NORMAL
/** Tamanio de letra del titulo de la leyenda */
#define LEGEND_TITLE_FONT_SIZE LEGEND_FONT_SIZE
/** Tipo de letra del titulo de la leyenda */
#define LEGEND_TITLE_FONT_FAMILY LEGEND_FONT_FAMILY
/** Estilo de letra del titulo de la leyenda */
#define LEGEND_TITLE_FONT_STYLE LEGEND_FONT_STYLE
/** Ancho de la fuente del titulo de la leyenda */
#define LEGEND_TITLE_FONT_WEIGHT wxFONTWEIGHT_BOLD
/** Canal rojo del color de letra para el titulo de la leyenda */
#define LEGEND_TITLE_FONT_COLOR_RED 1
/** Canal verde del color de letra para el titulo de la leyenda */
#define LEGEND_TITLE_FONT_COLOR_GREEN 1
/** Canal azul del color de letra para el titulo de la leyenda */
#define LEGEND_TITLE_FONT_COLOR_BLUE 1
/** Canal rojo del color de letra para el texto de la leyenda */
#define LEGEND_FONT_COLOR_RED 1
/** Canal verde del color de letra para el texto de la leyenda */
#define LEGEND_FONT_COLOR_GREEN 1
/** Canal azul del color de letra para el texto de la leyenda */
#define LEGEND_FONT_COLOR_BLUE 1
/** Color de fondo de la leyenda (canal red) */
#define LEGEND_BACKGROUND_RED 255
/** Color de fondo de la leyenda (canal green) */
#define LEGEND_BACKGROUND_GREEN 255
/** Color de fondo de la leyenda (canal blue) */
#define LEGEND_BACKGROUND_BLUE 255
/** Interlineado del texto */
#define TEXT_LINE_SEPARATION 0
/** Distancia entre el preview de la leyenda y el texto */
#define TEXT_TO_ICON_SEPARATION 5
/** Distancia entre el titulo de la ventana de leyenda y el primer item */
#define LEGEND_TITLE_SEPARATION LEGEND_SPACER_ITEMS
/** Ancho de caracter mas un factor */
#define CHAR_WIDTH_PLUS_FACTOR 1
/** Esquina en la que se muestra la brujula */
#define NORTH_ARROW_CORNER 2
/** Usada para que los puntos esten separados al menos esta distancia */
#define MIN_NORTH_ARROW_POINT_DIFF 10

/** Esquina en la que aparece la escala */
#define SCALE_CORNER 0
/** Tamanio inicial de la escala */
#define SCALE_INITIAL_SIZE 80

/** namespace suri */
namespace suri {

/**
 * Estructura que contiene la informacion de una clase (clasificacion)
 */
typedef struct _ClassificationInfo_t {
   int index;  // Indice
   std::string name;  // Nombre
   unsigned int color;  // Color
   std::string enclosure;  // Recinto
} CLASSIFICATIONINFO;

/**
 * Estructura que contiene la informacion de una clase de un estilo
 * vectorial clasificado
 */
typedef struct _VectorClassificationInfo_t {
   std::string styleWkt;
   std::string label;
} VECTORCLASSINFO;

/**
 * Estructura que contiene la informacion de la tabla de colores.
 */
typedef struct _ColorTableInfo_t {
   int index;  // Indice
   std::string name;  // Nombre
   unsigned int color;  // Color
   std::string boundary;

   bool operator==(const struct _ColorTableInfo_t& p1) const {
      double dboundary = StringToNumber<double>(boundary);
      double dp1boundary = StringToNumber<double>(p1.boundary);
      return DoubleCompare(dboundary, dp1boundary);
   }
} COLORTABLEINFO;



/** Clase MapRenderer */
/**
 * Esta clase deriva de Renderer. Es la encargada de dibujar sobre una imagen:
 *    - Escala
 *    - Recuadro de Leyenda
 *    - Norte (Rosa de los vientos)
 * Posee una propiedad que es del tipo Parameters el cual contiene toda la info
 * para poder graficar los elementos mencionados.
 */
class MapRenderer : public Renderer {
   /** Ctor. de Copia. */
   MapRenderer(const MapRenderer &MapRenderer);

public:
   /** Ctor. default */
   MapRenderer() {
   }
   /** Dtor. */
   virtual ~MapRenderer() {
   }
// ------------------------- ESTRUCTURA DE DATOS ----------------------------
   /** Clase Parameters */
   /**
    * Esta clase brinda soporte a la clase MapRenderer conteniendo toda la info.
    * para que la clase MapRenderer pueda dibujar los elementos.
    */
   class Parameters {
   public:
      /** Ctor. */
      Parameters() :
            legend_(false),
            grid_(false),
            north_(false),
            scale_(false),
            legendCorner_(LEGEND_CORNER),
            legendWidth_(LEGEND_WIDTH),
            previewWidth_(LEGEND_PREVIEW_WIDTH),
            previewHeight_(LEGEND_PREVIEW_HEIGHT),
            spacerHeight_(LEGEND_SPACER_ITEMS),
            font_(LEGEND_FONT_SIZE, LEGEND_FONT_FAMILY, LEGEND_FONT_STYLE,
                  LEGEND_FONT_WEIGHT),
            fontColour_(LEGEND_FONT_COLOR_RED, LEGEND_FONT_COLOR_GREEN,
                        LEGEND_FONT_COLOR_BLUE),
            backgroundColour_(LEGEND_BACKGROUND_RED, LEGEND_BACKGROUND_GREEN,
                              LEGEND_BACKGROUND_BLUE),
            northArrowCorner_(NORTH_ARROW_CORNER), scaleCorner_(SCALE_CORNER),
            pList_(NULL), dimension_(0), sizex_(0), sizey_(0) {
         std::string titletext = Configuration::GetParameter
                                                         (MapRenderer::MapLegendTitleConfigTag, "");
         font_ = wxFont();
         std::string backgroundcolor = Configuration::GetParameter
                                                   (MapRenderer::MapLegendBackgroundConfigTag, "");
         if (!backgroundcolor.empty())
            backgroundColour_ = wxColour(backgroundcolor);
      }

      bool legend_; /*! indica si hay que dibujar el recuadro de leyenda */
      bool grid_; /*! indica si hay que dibujar la grilla */
      bool north_; /*! indica si hay que dibujar el vector del norte */
      bool scale_; /*! indica si hay que dibujar la escala */
// --LEGEND
      size_t legendCorner_; /*! esquina del viewer en que se situa el recuadro */
      /* de leyenda. */
      size_t legendWidth_; /*! ancho del recuadro de leyenda. */
      size_t previewWidth_; /*! ancho de la imagen previa de los items de la */
      /* leyenda. */
      size_t previewHeight_; /*! alto de la imagen previa de los items de la */
      /* leyenda. */
      size_t spacerHeight_; /*! separacion vertical entre los items de la */
      /* leyenda. */
      wxFont font_; /*! fuente establecida para la leyenda. */
      wxColour fontColour_; /*! color establecido para la fuente de la leyenda */
      wxColour backgroundColour_; /*! color de fondo para el recuadro de la */
      /* leyenda. */
// --NORTH ARROW
      size_t northArrowCorner_; /*! esquina del viewer en que se situa el */
      /* vector del norte. */
// --SCALE
      size_t scaleCorner_; /*! esquina del viewer en que se situa el recuadro */
      /* de escala. */
      static LayerList *pStaticList_; /*! lista de elementos. */
      LayerList *pList_; /*! Lista de elementos. */
// static int listGroup_;  /*! indica sobre que grupo se dibujan las referencias. */
      int dimension_, sizex_, sizey_;
   };
// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
   /** genera un nodo XML a partir de los parametros */
   static wxXmlNode *GetXmlNode(const Parameters &RenderParameters = Parameters());
   /** Obtiene el nodo de interes a partir de un elemento */
   static wxXmlNode *GetXmlNode(const Element *pElement);
   /** Reemplaza el nodo renderizado por el nuevo */
   static bool SetXmlNode(Element *pElement, wxXmlNode *pNode);
   /** genera los parametros a partir de un nodo */
   static Parameters GetParameters(const wxXmlNode *pNode);
// -------------------------- METODO DE CREACION ----------------------------
   /** Crea un renderizador a partir de un pElement */
   virtual Renderer *Create(Element *pElement, Renderer *pPreviousRenderer) const;
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
   /** Dibuja recuadro de Leyenda. */
   virtual void DrawLegend(wxDC *pDC, const int &ViewerWidth, const int &ViewerHeight);
// Concatena el bitmap rhs al bitmap lhs
   void AppendBitmap(wxBitmap &lhs, wxBitmap rhs, int Separation = 0, bool Vertical =
         true);
// Devuelve un array con lineas de texto cuyo ancho (en px) no supera al parametro
   void SingleToMultilineText(wxString SingleLineText, int MaxWidth, wxFont TextFont,
                              wxArrayString &MultiLineText);
// Crea el texto de la leyenda para el elemento
   void CreateElementLegend(Element *pElement, wxString &Legend);
// Recibe un texto y lo dibuja en un Bitmap
   void DrawMultilineText(wxString Text, int TextWidth, wxFont TextFont,
                          wxColor FontColour, wxBitmap &TextBitmap);
// Crea un bitmap con la leyenda del elemento (icono + texto)
   void CreateElementBitmapLegend(Element *pElement, wxBitmap &Legend);
   /** Dibuja el vector de Norte. */
   virtual void DrawNorth(wxDC *pDC, const World *pWorldWindow, const int ViewerWidth,
                          const int ViewerHeight);
   /** Dibuja la escala. */
   virtual void DrawScale(wxDC *pDC, const World *pWorldWindow, const int ViewerWidth,
                          const int ViewerHeight);
   /** Dibuja rectangulo en la posicion indicada */
   void DrawRoundedRectangle(wxDC *pDC, const int coordx, const int coordy,
                             const int ItemWidth, const int ItemHeight);
   /** Dibuja texto sombreado en la posicion indicada */
   void DrawShadedText(wxDC *pDC, const int coordx, const int coordy, std::string text);
   /** Dibuja imagen de la escala */
   void DrawScaleImage(wxDC *pDC, const int coordx1, const int coordx2,
                       const int coordy);
   /** Deprecada */
   virtual void DrawGrid();

// ----------------------- METODOS DE ACTUALIZACION -------------------------
   /** Actualizacion de parametros para representar un elemento */
   virtual void Update(Element *pElement);

   static std::vector<int> staticElementGroupVector_; /*! indica sobre que grupo se dibujan las */
   // referencias.
   std::vector<int> elementGroupVector_; /*! indica sobre que grupo se dibujan las referencias. */
   static const std::string MapLegendNativeFontInfo;
   static const std::string MapLegendTitleConfigTag;
   /*static const std::string MapLegendFontConfigTag;
   static const std::string MapFontStyleConfigTag;
   static const std::string MapLegendFontSizeConfigTag;*/
   static const std::string MapLegendBorderConfigTag;
   static const std::string MapLegendBackgroundConfigTag;
   static const std::string MapNorthIconConfigTag;

private:
   /** devuelve las coordenadas de la esquina donde se debe dibujar el item. */
   void GetCornerCoordinates(const int corner, const int ViewerWidth,
                             const int ViewerHeight, const int ItemWidth,
                             const int ItemHeight, int &X, int &Y);
   /** Corta la cadena Name a una longitud fija (Length) */
   wxString CutElementName(const wxString &Name, size_t Length);

   Parameters parameters_; /*! parametros de la clase */

   /** Extrae la informacion de clasificacion del elemento. */
   static std::vector<CLASSIFICATIONINFO> ExtractClassificationInfo(Element* pElement);

   /** Extrae la informacion de la tabla de colores. */
   static std::vector<COLORTABLEINFO> ExtractColorTableInfo(Element* pElement);
   /** Extrae informacion de estilos clasificados **/
   static std::vector<VECTORCLASSINFO> ExtractVectorClassesInfo(Element* pElement);

   /** Agrega informacion nueva al vector */
   static void LoadColorTableInfo(std::vector<COLORTABLEINFO>& CTInfo,
                                  wxXmlNode* pTable, int Component);

   /** Carga las etiquetas para cada intervalo */
   static void LoadColorTableInfoLabels(std::vector<COLORTABLEINFO>& CTInfo,
                                        wxXmlNode* pTable);

   /** Agrega la informacion de clasificacion si esta esta disponible. */
   void AddClassificationInfo(std::vector<CLASSIFICATIONINFO>& CInfo, wxBitmap& DestBM);

   /** Agrega la informacion de la tabla de colores si esta disponible. */
   void AddColorTableInfo(std::vector<COLORTABLEINFO>& CTInfo, wxBitmap& DestBM);

   /** Agrega la informacion de la tabla de colores si esta disponible. */
   void AddVectorClassInfo(std::vector<VECTORCLASSINFO>& Classes, wxBitmap& DestBM);

   int lengenddimension_, scaledimension_, northdimension_;
};
}

#endif /*MAPRENDERER_H_*/
