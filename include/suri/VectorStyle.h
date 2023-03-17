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

#ifndef VECTORSTYLE_H_
#define VECTORSTYLE_H_

#define DEFAULT_STYLE "VECTORSTYLE[3,\"Poligono rojo\",BRUSH[\"Suri-brush\",1,COLOR[255,0,0,255]],PEN[\"Suri-pen\",1,COLOR[1,1,1,255],WIDTH[1]],SYMBOL[\"Suri-symbol\",0,COLOR[0,0,0,255],SIZE[1]],LABEL[\"Suri-label\",0,EXPRESSION[],COLOR[0,0,0,255],BACKCOLOR[0,0,0,0],SIZE[8],ITALIC[0],BOLD[0],UNDERLINED[0],ANGLE[0],OFFSET[0,0],ANCHOR[2]]]"

#define DEFAULT_GEO_DISTANCE_STYLE  "VECTORSTYLE[3,\"Poligono azul\",BRUSH[\"Suri-brush\",0,COLOR[255,0,0,255]],PEN[\"Suri-pen\",1,COLOR[253,21,2,255],WIDTH[1]],SYMBOL[\"Suri-symbol\",2,COLOR[250,219,5,255],SIZE[2]],LABEL[\"Suri-label\",0,EXPRESSION[],COLOR[0,0,0,255],BACKCOLOR[0,0,0,0],SIZE[8],ITALIC[0],BOLD[0],UNDERLINED[0],ANGLE[0],OFFSET[0,0],ANCHOR[4]]]"
#define DEFAULT_GEO_AREA_STYLE      "VECTORSTYLE[3,\"Poligono azul\",BRUSH[\"Suri-brush\",2,COLOR[253,21,2,255]],PEN[\"Suri-pen\",2,COLOR[250,219,5,255],WIDTH[1]],SYMBOL[\"Suri-symbol\",2,COLOR[250,219,5,255],SIZE[2]],LABEL[\"Suri-label\",0,EXPRESSION[],COLOR[0,0,0,255],BACKCOLOR[0,0,0,0],SIZE[8],ITALIC[0],BOLD[0],UNDERLINED[0],ANGLE[0],OFFSET[0,0],ANCHOR[4]]]"
#define DEFAULT_GEO_SELECTION_STYLE "VECTORSTYLE[3,\"Poligono rojo\",BRUSH[\"Suri-brush\",6,COLOR[58,175,175,255]],PEN[\"Suri-pen\",1,COLOR[58,175,175,255],WIDTH[1]],SYMBOL[\"Suri-symbol\",2,COLOR[58,175,175,255],SIZE[2]],LABEL[\"Suri-label\",0,EXPRESSION[],COLOR[0,0,0,255],BACKCOLOR[0,0,0,0],SIZE[8],ITALIC[0],BOLD[0],UNDERLINED[0],ANGLE[0],OFFSET[0,0],ANCHOR[2]]]"
#define DEFAULT_GEO_EDITION_STYLE   "VECTORSTYLE[3,\"Poligono rojo\",BRUSH[\"Suri-brush\",4,COLOR[250,219,5,255]],PEN[\"Suri-pen\",1,COLOR[250,219,5,255],WIDTH[1]],SYMBOL[\"Suri-symbol\",7,COLOR[250,219,5,255],SIZE[2]],LABEL[\"Suri-label\",0,EXPRESSION[],COLOR[0,0,0,255],BACKCOLOR[0,0,0,0],SIZE[8],ITALIC[0],BOLD[0],UNDERLINED[0],ANGLE[0],OFFSET[0,0],ANCHOR[2]]]"
#define DEFAULT_GEO_DELETING_STYLE  "VECTORSTYLE[3,\"Poligono rojo\",BRUSH[\"Suri-brush\",2,COLOR[253,21,2,255]],PEN[\"Suri-pen\",1,COLOR[253,21,2,255],WIDTH[1]],SYMBOL[\"Suri-symbol\",1,COLOR[253,21,2,255],SIZE[2]],LABEL[\"Suri-label\",0,EXPRESSION[],COLOR[0,0,0,255],BACKCOLOR[0,0,0,0],SIZE[8],ITALIC[0],BOLD[0],UNDERLINED[0],ANGLE[0],OFFSET[0,0],ANCHOR[2]]]"
#define DEFAULT_GEO_CREATION_STYLE  "VECTORSTYLE[3,\"Poligono azul\",BRUSH[\"Suri-brush\",4,COLOR[250,219,5,255]],PEN[\"Suri-pen\",1,COLOR[250,219,5,255],WIDTH[1]],SYMBOL[\"Suri-symbol\",7,COLOR[250,219,5,255],SIZE[2]],LABEL[\"Suri-label\",0,EXPRESSION[],COLOR[0,0,0,255],BACKCOLOR[0,0,0,0],SIZE[8],ITALIC[0],BOLD[0],UNDERLINED[0],ANGLE[0],OFFSET[0,0],ANCHOR[4]]]"

// Includes standard
#include <vector>
#include <string>

// Includes suri
#include "suri/Vector.h"

/** namespace suri */
namespace suri {

/**
 *  Esta clase implementa los estilos para representacion de datos vectoriales
 *  \TODO(Sebastian - TCK #835): Ampliar definicion Pen, Brush, Symbol y Label
 *  \TODO(Sebastian - TCK #835): Color deberia estar definida en un lugar comun fuera de aca
 */
class VectorStyle {
public:
   /** guarda un color de 24 bits + 8 bits de transparencia */
   class Color {
   public:
      /** Constructor a partir de RGBA */
      Color(const unsigned char Red = 0, const unsigned char Green = 0,
            const unsigned char Blue = 0, const unsigned char Alpha = 255);
      /** Constructor a partir de un string con los datos de RGBA en hexadecimal (string) */
      explicit Color(const std::string& HexaString);
      /** rojo, unsigned char */
      unsigned char red_;
      /** verde, unsigned char */
      unsigned char green_;
      /** azul, unsigned char */
      unsigned char blue_;
      /** alpha, unsigned char */
      unsigned char alpha_;
      /** Devuelve un string asociado a la combinacion de colores en hexa con o sin alpha */
      std::string GetColorAsHexaString(bool Alpha = true) const;
      /** Configura los componentes del color en base a un hexa */
      bool SetColorFromHexaString(const std::string& HexaString);
      /** Determina si dos colores son iguales */
      bool Equals(const Color& color);
   };

   /**
    * Tipo de linea, color, ancho, nombre del sistema que genero el elemento.
    */
   class Pen {
   public:
      /** tipos de linea */
      typedef enum {
         Transparent = 0, Solid = 1, Dot = 2, LongDash = 3, ShortDash = 4, DotDash = 5,
         BackDiagonal = 6, CrossDiagonal = 7, ForDiagonal = 8, Cross = 9, Horizontal = 10,
         Vertical = 11,
#ifdef __UNUSED_CODE__
         Stipple = 2,
         UserDash = 7,
         Count = 14,
#endif
         Count
      } PenType;
      /** Constructor */
      explicit Pen(const std::string &System = "Suri-pen");
      /** Devuelve el string(en messages) asociado a un PenType */
      std::string GetTypeAsString(const size_t &Id);
      /** Nombre del sistema que genero el elemento */
      std::string system_;
      /** tipo de linea(solida, shortDash, ect) */
      PenType id_;
      /** color de linea */
      Color color_;
      /** ancho de linea */
      int width_;
#ifdef __UNUSED_CODE__
      std::vector<unsigned int> pattern_;
      wxBitmap stipple_;
      int cap_;
      int join;
      int perpendicularOffset_;
#endif
   };

   /**
    * Forma de pintar superficie, color, sistema que genero el elemento
    */
   class Brush {
   public:
      /** tipos de relleno */
      typedef enum {
         Transparent = 0, Solid = 1, BackDiagonal = 2, CrossDiagonal = 3, ForDiagonal = 4,
         Cross = 5, Horizontal = 6, Vertical = 7,
#ifdef __UNUSED_CODE__
         Stipple = 2,
         Count = 9,
#endif
         Count
      } BrushType;
      /** Constructor */
      explicit Brush(const std::string &System = "Suri-brush");
      /** Devuelve el string(en messages) asociado a un BrushType */
      std::string GetTypeAsString(const size_t &Id);
      /** Nombre del sistema que genero el elemento */
      std::string system_;
      /** tipo de relleno(solida, transparente, ect) */
      BrushType id_;
      /** color del relleno */
      Color color_;
#ifdef __UNUSED_CODE__
      Color backColor_;
      wxBitmap stipple_;
#endif
   };

   /**
    * Tipo de punto, color, tamano, sitema que genero el elemento
    */
   class Symbol {
   public:
      /** tipos de punto */
      typedef enum {
         NoSymbol = 0, Circle = 1, Square = 2, Triangle = 3, Cross = 4, Pentagon = 5,
         EmptyCircle = 6, EmptySquare = 7, EmptyTriangle = 8, EmptyCross = 9, EmptyPentagon = 10,
         Count
      } SymbolType;
      /** Constructor */
      explicit Symbol(const std::string &System = "Suri-symbol");
      /** Devuelve el string(en messages) asociado a un SymbolType */
      std::string GetTypeAsString(const size_t &Id);
      /** Nombre del sistema que genero el elemento */
      std::string system_;
      /** tipo de punto(cuadrado, circulo, ect) */
      SymbolType id_;
      /** color del punto */
      Color color_;
      /** tamano del punto */
      int size_;
   };

   // TODO(Javier - Sin TCK): Verificar familias de fonts (en Windows hay varias que son iguales)
   /**
    * Tipo de letra, posicion de letras, posicionamiento, color, color de fondo, negrita, etc.
    */
   class Label {
   public:
      /** tipos de letra */
      typedef enum {
         NoLabel = 0, Default = 1, Decorative = 2, Roman = 3, Script = 4, Swiss = 5, Modern = 6,
         Teletype = 7, CountLabel
      } LabelType;
      /** tipo de alineaciones */
      typedef enum {
         TopLeft = 0, CenterLeft = 1, BottomLeft = 2, TopCenter = 3, Center = 4, BottomCenter = 5,
         TopRight = 6, CenterRight = 7, BottomRight = 8, CountAnchor
      } AnchorType;
      /** tipos de ubicacion */
      typedef enum {
         FirstPoint = 0, LastPoint = 1, EveryPoint = 2, StretchedAlongLine = 3, EverySegment = 4,
         Middle = 5, CountPlacement
      } PlacementModeType;
      /** Constructor */
      explicit Label(const std::string &System = "Suri-label");
      /** Devuelve el string(en messages) asociado a un LabelType */
      std::string GetTypeAsString(const size_t &Id);
      /** nombre del sistema que genero el elemento */
      std::string system_;
      /** tipo de letra(Roman, Modern, ect) */
      LabelType id_;
      /** Ejemplo: "{field_name}" o "hola, mundo" */
      std::string expression_;
      /** color de las letras */
      Color color_;
      /** color de fondo */
      Color backColor_;
      /** tamano de la letra */
      int size_;
      /** si el texto esta en italic */
      bool italic_;
      /** si el texto esta en negrita */
      bool bold_;
      /** si el texto esta subrallado */
      bool underlined_;
      /** en grados (contra reloj) */
      int angle_;
      /** en pixels */
      int dx_;
      /** en pixels */
      int dy_;
      /** punto de anclaje */
      AnchorType anchor_;
#ifdef __UNUSED_CODE__
      PlacementModeType placementMode_;
#endif
   };

   /** Crea el VectorStyle */
   static VectorStyle* Create(const std::string &VStyleWkt = DEFAULT_STYLE);
   /** Crea un VectorStyle con nombre y tipo indicados */
   static VectorStyle* Create(const Vector::VectorType &VType, const std::string &Name = "");
   /** Destruye el VectorStyle */
   static void Destroy(VectorStyle * &pVectorStyle);
   /** Indica el tipo de vector del estilo */
   Vector::VectorType GetVectorType() const;
   /** Devuelve el nombre del estilo */
   std::string GetName() const;
   /** Devuelve el brush */
   Brush* GetBrush() const;
   /** Devuelve el pen */
   Pen* GetPen() const;
   /** Devuelve el symbol */
   Symbol* GetSymbol() const;
   /** Devuelve el label */
   Label * GetLabel() const;
   /** Devuelve el Wkt del estilo */
   std::string GetWkt() const;
   /** Cambia el nombre del estilo */
   void ChangeName(const std::string &Name);
   /** Clona el estilo */
   VectorStyle * Clone(const std::string& NewName = "") const;
   /** Copia el label a otro VectorStyle */
   bool CopyLabel(const VectorStyle* PDestinationVector) const;
   /** Copia el pen a otro VectorStyle */
   bool CopyPen(const VectorStyle* PDestinationVector) const;
   /** Copia el brush a otro VectorStyle */
   bool CopyBrush(const VectorStyle* PDestinationVector) const;
   /** Copia el symbol a otro VectorStyle */
   bool CopySymbol(const VectorStyle* PDestinationVector) const;
   /** Verifica si el estilo es invertido **/
   bool IsInverted() const;

   void SetBrush(Brush* pBrush) { pBrush_ = new Brush(*pBrush); }

   void SetPen(Pen* pPen) { pPen_ = new Pen(*pPen); }

protected:
   /** Constructor */
   VectorStyle(const Vector::VectorType &VType, const std::string &Name = "");
   /** Destructor */
   ~VectorStyle();
   /** Tipo de vector */
   Vector::VectorType vType_;
   /** Nombre del estilo */
   std::string name_;
   /** Brush (usado por Polygon) */
   Brush* pBrush_;
   /** Pen (usado por Line y Polygon) */
   Pen* pPen_;
   /** Symbol (usado por Point, Line y Polygon) */
   Symbol* pSymbol_;
   /** Label (usado por Point, Line y Polygon) */
   Label* pLabel_;
   /** bool que indica si el estilo debe ser invertido **/
   bool invert_;
};

} /** namespace suri */

#endif /*VECTORSTYLE_H_*/
