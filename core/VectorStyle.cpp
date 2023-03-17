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
#include<sstream>
#include <string>
#include <vector>

// Includes suri
#include "suri/VectorStyle.h"
#include "suri/messages.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Wkt.h"
#include "logmacros.h"

// Defines
/** Vector con labels de tipo de linea. */
std::string PenIdLabels[] = { _(label_TRANSPARENT), _(label_SOLID), _(label_OF_POINTS), _(
      label_LONG_POINTED),
                              _(label_SHORT_POINTED), _(label_POINT_AND_LINE), _(label_RIGHT_DIAG),
                              _(label_CROSS_DIAG), _(label_LEFT_DIAG), _(label_CROSSED), _(
                                    label_HORIZONTAL),
                              _(label_VERTICAL)
// _("Manchado"),
// _("User Dash"),
      };

/** Vector con labels de tipo de relleno. */
std::string BrushIdLabels[] = { _(label_TRANSPARENT), _(label_SOLID), _(label_RIGHT_DIAG), _(
      label_CROSS_DIAG),
                                _(label_LEFT_DIAG), _(label_CROSSED), _(label_HORIZONTAL), _(
                                      label_VERTICAL)
// _("Manchado"),
      };

/** Vector con labels de simbolos. */
std::string SymbolIdLabels[] = { _(label_NO_SYMBOL), _(label_CIRCLE), _(label_QUAD), _(
      label_TRIANGLE),
                                 _(label_CROSS), _(label_PENTAG), _(label_CIRCLE_NO_FILL), _(
                                       label_QUAD_NO_FILL),
                                 _(label_TRIANGLE_NO_FILL), _(label_CROSS_NO_FILL), _(
                                       label_PENTAG_NO_FILL) };

/** Vector con labels de etiquetas. */
std::string LabelIdLabels[] =
      { _(label_NO_LABEL), _(label_NORMAL), _(label_DECORATIVE), _(label_ROMAN), _(label_CURSIVE),
        _(label_SWISS), _(label_MODERN), _(label_TELETYPE) };

namespace suri {
// --------------------------- Clase VectorStyle y derivadas ------------------------------------

/**
 * Constructor de color
 * @param[in] Red rojo
 * @param[in] Green verde
 * @param[in] Blue azul
 * @param[in] Alpha alfa
 * @return instancia de la clase VectorStyle::Color
 */
VectorStyle::Color::Color(const unsigned char Red, const unsigned char Green,
                          const unsigned char Blue, const unsigned char Alpha) :
      red_(Red), green_(Green), blue_(Blue), alpha_(Alpha) {
}

/**
 * Constructor a partir de un string con los datos de RGBA en hexadecimal (string)
 * @param[in] HexaString String con la informacion hexadecimal del color
 */
VectorStyle::Color::Color(const std::string& HexaString) {
   SetColorFromHexaString(HexaString);
}

/**
 * Devuelve un string asociado a los componentes de  color en formato hexa con o sin alpha
 * @param[in] alpha determina si agrega o no el alpha
 * @return std::string con los componentes del color separados por el separador
 */
std::string VectorStyle::Color::GetColorAsHexaString(bool Alpha) const {
   std::stringstream stringindex;
   stringindex << LongToHexaString(red_, 2);
   stringindex << LongToHexaString(green_, 2);
   stringindex << LongToHexaString(blue_, 2);
   if (Alpha) {
      stringindex << LongToHexaString(alpha_, 2);
   }
   return stringindex.str();
}

/**
 * Configura los colores a partir de un string en hexa.
 * Se considera un string valido a un string con longitud 8 o 6
 * cada componente corresponde a 2 caracteres
 * @return true si el string es valido y se pueden setear los colores
 * @return false en caso de no poder setear los valores
 */
bool VectorStyle::Color::SetColorFromHexaString(const std::string& HexaString) {
   size_t validalphasize = 8;
   size_t validrgbsize = 6;
   if (HexaString.size() != validrgbsize && HexaString.size() != validalphasize)
      return false;
   size_t colorlengh = 2;
   // Siempre se presupone que el orden de los componentes del color son RGBA
   size_t color[4] = { 0, 2, 4, 6 };
   red_ = HexaStringToLong(HexaString.substr(color[0], colorlengh));
   green_ = HexaStringToLong(HexaString.substr(color[1], colorlengh));
   blue_ = HexaStringToLong(HexaString.substr(color[2], colorlengh));
   if (HexaString.size() == validalphasize)
      alpha_ = HexaStringToLong(HexaString.substr(color[3], colorlengh));
   else
      alpha_ = 255;
   return true;
}

/**
 * Determina si dos colores son iguales
 * @param[in] Color Otro color contra el cual comprar
 * @return Devuelve verdadero si son iguales (son iguales a nivel RGBA)
 */
bool VectorStyle::Color::Equals(const VectorStyle::Color& Color) {
   bool eqr = Color.red_ == red_;
   bool eqg = Color.green_ == green_;
   bool eqb = Color.blue_ == blue_;
   bool eqa = Color.alpha_ == alpha_;
   return eqr && eqg && eqb && eqa;
}

/**
 * Constructor de pen
 * @param[in] System nombre del sistema que genero el elemento
 * @return instancia de la clase VectoStyle::Pen
 */
VectorStyle::Pen::Pen(const std::string &System) :
      system_(System) {
   id_ = Solid;
   width_ = 1;
}

/**
 * Devuelve el string asociado al tipo de Pen
 * @param[in] Id id del Pen cuyo nombre me interesa
 * @return nombre del Pen
 */
std::string VectorStyle::Pen::GetTypeAsString(const size_t &Id) {
   if (Id < 0 || Id >= Count) {
      return _(message_VECTOR_STYLE_ELEMENT_INVALID_TYPE_ERROR);
   }
   return PenIdLabels[Id];
}

/**
 * Constructor de brush
 * @param[in] System nombre del sistema que genero el elemento
 * @return instancia de la clase VectoStyle::Brush
 */
VectorStyle::Brush::Brush(const std::string &System) :
      system_(System) {
   id_ = Transparent;
}

/**
 * Devuelve el string asociado al tipo de Brush
 * @param[in] Id id del Brush cuyo nombre me interesa
 * @return nombre del Brush
 */
std::string VectorStyle::Brush::GetTypeAsString(const size_t &Id) {
   if (Id < 0 || Id >= Count) {
      return _(message_VECTOR_STYLE_ELEMENT_INVALID_TYPE_ERROR);
   }
   return BrushIdLabels[Id];
}

/**
 * Constructor de symbol
 * @param[in] System nombre del sistema que genero el elemento
 * @return instancia de la clase VectoStyle::Symbol
 */
VectorStyle::Symbol::Symbol(const std::string &System) :
      system_(System) {
   id_ = NoSymbol;
   size_ = 1;
}

/**
 * Devuelve el string asociado al tipo de Symbol
 * @param[in] Id id del Symbol cuyo nombre me interesa
 * @return nombre del Symbol
 */
std::string VectorStyle::Symbol::GetTypeAsString(const size_t &Id) {
   if (Id < 0 || Id >= Count) {
      return _(message_VECTOR_STYLE_ELEMENT_INVALID_TYPE_ERROR);
   }
   return SymbolIdLabels[Id];
}

/**
 * Constructor de label
 * @param[in] System nombre del sistema que genero el elemento
 * @return instancia de la clase VectoStyle::Label
 */
VectorStyle::Label::Label(const std::string &System) :
      system_(System) {
   id_ = NoLabel;
   size_ = 8;
   // por defecto el fondo es transparente
   backColor_.alpha_ = 0;
   italic_ = bold_ = underlined_ = false;
   angle_ = 0;
   dx_ = dy_ = 0;
   anchor_ = Center;
}

/**
 * Devuelve el string asociado al tipo de Label
 * @param[in] Id id del Label cuyo nombre me interesa
 * @return nombre del Label
 */
std::string VectorStyle::Label::GetTypeAsString(const size_t &Id) {
   if (Id < 0 || Id >= CountLabel) {
      return _(message_VECTOR_STYLE_ELEMENT_INVALID_TYPE_ERROR);
   }
   return LabelIdLabels[Id];
}

/**
 * Constructor de un Estilo de tipo VType
 * @param[in] VType tipo de vector
 * @param[in] Name nombre del estilo
 * @return instancia de la clase VectorStyle
 */
VectorStyle::VectorStyle(const Vector::VectorType &VType, const std::string &Name) :
      vType_(VType), name_(Name), pBrush_(NULL), pPen_(NULL), pSymbol_(NULL), pLabel_(NULL),
      invert_(false) {
   switch (vType_) {
      case Vector::Polygon: {
         pBrush_ = new Brush;
      }
      case Vector::Line: {
         pPen_ = new Pen;
      }
      case Vector::Point: {
         pSymbol_ = new Symbol;
         pLabel_ = new Label;
         break;
      }
      default:
         break;
   }
}

/** Destructor */
VectorStyle::~VectorStyle() {
   delete pBrush_;
   delete pPen_;
   delete pSymbol_;
   delete pLabel_;
}

/** Crea el VectorStyle */
/**
 * Crea un VectorStyle a partir de Wkt generado usando VStyleWkt
 * Tiene un switch y segun el tipo de vector que afecta el VectorStyle, inicializa
 * los valores del mismo, usando el Wkt
 * @param[in] VStyleWkt texto con configuracion del estilo que hay que crear
 * @return VectorStyle configurado deacuerdo a VStyleWkt. Si no pudo crearlo NULL
 * \attention el VectorStyle retornado(y su eliminacion) es responsabilidad del invocante
 */
VectorStyle * VectorStyle::Create(const std::string &VStyleWkt) {
   Wkt *pstylewkt = Wkt::Create(VStyleWkt);
   if (pstylewkt == NULL) {
      return NULL;
   }

   std::vector < std::string > stringvec;

   // Determina el tipo de estilo y nombre
   stringvec = pstylewkt->GetLeafChildren("VECTORSTYLE");
   if (stringvec.size() != 2) {
      return NULL;
   }
   VectorStyle * pvstyle = new VectorStyle(
         (Vector::VectorType) StringToNumber<int>(stringvec.at(0)), stringvec.at(1));

   switch (pvstyle->GetVectorType()) {
      case Vector::Polygon: {
         // Invert
         stringvec = pstylewkt->GetLeafChildren("VECTORSTYLE|INVERT");
         if (stringvec.size() == 2) {
            pvstyle->invert_ = StringToNumber<int>(stringvec.at(1));
         }

         // Brush
         stringvec = pstylewkt->GetLeafChildren("VECTORSTYLE|BRUSH");
         if (stringvec.size() == 2) {
            pvstyle->GetBrush()->system_ = stringvec.at(0);
            pvstyle->GetBrush()->id_ = (Brush::BrushType) StringToNumber<int>(stringvec.at(1));
         }
         stringvec = pstylewkt->GetLeafChildren("VECTORSTYLE|BRUSH|COLOR");
         if (stringvec.size() == 4) {
            pvstyle->GetBrush()->color_.red_ = StringToNumber<unsigned char>(stringvec.at(0));
            pvstyle->GetBrush()->color_.green_ = StringToNumber<unsigned char>(stringvec.at(1));
            pvstyle->GetBrush()->color_.blue_ = StringToNumber<unsigned char>(stringvec.at(2));
            pvstyle->GetBrush()->color_.alpha_ = StringToNumber<unsigned char>(stringvec.at(3));
         }
      }
      case Vector::Line: {
         // Pen
         stringvec = pstylewkt->GetLeafChildren("VECTORSTYLE|PEN");
         if (stringvec.size() == 2) {
            pvstyle->GetPen()->system_ = stringvec.at(0);
            pvstyle->GetPen()->id_ = (Pen::PenType) StringToNumber<int>(stringvec.at(1));
         }
         stringvec = pstylewkt->GetLeafChildren("VECTORSTYLE|PEN|COLOR");
         if (stringvec.size() == 4) {
            pvstyle->GetPen()->color_.red_ = StringToNumber<unsigned char>(stringvec.at(0));
            pvstyle->GetPen()->color_.green_ = StringToNumber<unsigned char>(stringvec.at(1));
            pvstyle->GetPen()->color_.blue_ = StringToNumber<unsigned char>(stringvec.at(2));
            pvstyle->GetPen()->color_.alpha_ = StringToNumber<unsigned char>(stringvec.at(3));
         }
         stringvec = pstylewkt->GetLeafChildren("VECTORSTYLE|PEN|WIDTH");
         if (stringvec.size() == 1) {
            pvstyle->GetPen()->width_ = StringToNumber<int>(stringvec.at(0));
         }
      }
      case Vector::Point: {
         // Symbol
         stringvec = pstylewkt->GetLeafChildren("VECTORSTYLE|SYMBOL");
         if (stringvec.size() == 2) {
            pvstyle->GetSymbol()->system_ = stringvec.at(0);
            pvstyle->GetSymbol()->id_ = (Symbol::SymbolType) StringToNumber<int>(stringvec.at(1));
         }
         stringvec = pstylewkt->GetLeafChildren("VECTORSTYLE|SYMBOL|COLOR");
         if (stringvec.size() == 4) {
            pvstyle->GetSymbol()->color_.red_ = StringToNumber<unsigned char>(stringvec.at(0));
            pvstyle->GetSymbol()->color_.green_ = StringToNumber<unsigned char>(stringvec.at(1));
            pvstyle->GetSymbol()->color_.blue_ = StringToNumber<unsigned char>(stringvec.at(2));
            pvstyle->GetSymbol()->color_.alpha_ = StringToNumber<unsigned char>(stringvec.at(3));
         }
         stringvec = pstylewkt->GetLeafChildren("VECTORSTYLE|SYMBOL|SIZE");
         if (stringvec.size() == 1) {
            pvstyle->GetSymbol()->size_ = StringToNumber<int>(stringvec.at(0));
         }

         // Label
         stringvec = pstylewkt->GetLeafChildren("VECTORSTYLE|LABEL");
         if (stringvec.size() == 2) {
            pvstyle->GetLabel()->system_ = stringvec.at(0);
            pvstyle->GetLabel()->id_ = (Label::LabelType) StringToNumber<int>(stringvec.at(1));
         }
         stringvec = pstylewkt->GetLeafChildren("VECTORSTYLE|LABEL|EXPRESSION");
         if (stringvec.size() == 1) {
            pvstyle->GetLabel()->expression_ = stringvec.at(0);
         }
         stringvec = pstylewkt->GetLeafChildren("VECTORSTYLE|LABEL|COLOR");
         if (stringvec.size() == 4) {
            pvstyle->GetLabel()->color_.red_ = StringToNumber<unsigned char>(stringvec.at(0));
            pvstyle->GetLabel()->color_.green_ = StringToNumber<unsigned char>(stringvec.at(1));
            pvstyle->GetLabel()->color_.blue_ = StringToNumber<unsigned char>(stringvec.at(2));
            pvstyle->GetLabel()->color_.alpha_ = StringToNumber<unsigned char>(stringvec.at(3));
         }
         stringvec = pstylewkt->GetLeafChildren("VECTORSTYLE|LABEL|BACKCOLOR");
         if (stringvec.size() == 4) {
            pvstyle->GetLabel()->backColor_.red_ = StringToNumber<unsigned char>(stringvec.at(0));
            pvstyle->GetLabel()->backColor_.green_ = StringToNumber<unsigned char>(stringvec.at(1));
            pvstyle->GetLabel()->backColor_.blue_ = StringToNumber<unsigned char>(stringvec.at(2));
            pvstyle->GetLabel()->backColor_.alpha_ = StringToNumber<unsigned char>(stringvec.at(3));
         }
         stringvec = pstylewkt->GetLeafChildren("VECTORSTYLE|LABEL|SIZE");
         if (stringvec.size() == 1) {
            pvstyle->GetLabel()->size_ = StringToNumber<int>(stringvec.at(0));
         }
         stringvec = pstylewkt->GetLeafChildren("VECTORSTYLE|LABEL|ITALIC");
         if (stringvec.size() == 1) {
            pvstyle->GetLabel()->italic_ = StringToNumber<bool>(stringvec.at(0));
         }
         stringvec = pstylewkt->GetLeafChildren("VECTORSTYLE|LABEL|BOLD");
         if (stringvec.size() == 1) {
            pvstyle->GetLabel()->bold_ = StringToNumber<bool>(stringvec.at(0));
         }
         stringvec = pstylewkt->GetLeafChildren("VECTORSTYLE|LABEL|UNDERLINED");
         if (stringvec.size() == 1) {
            pvstyle->GetLabel()->underlined_ = StringToNumber<bool>(stringvec.at(0));
         }
         stringvec = pstylewkt->GetLeafChildren("VECTORSTYLE|LABEL|ANGLE");
         if (stringvec.size() == 1) {
            pvstyle->GetLabel()->angle_ = StringToNumber<int>(stringvec.at(0));
         }
         stringvec = pstylewkt->GetLeafChildren("VECTORSTYLE|LABEL|OFFSET");
         if (stringvec.size() == 2) {
            pvstyle->GetLabel()->dx_ = StringToNumber<int>(stringvec.at(0));
            pvstyle->GetLabel()->dy_ = StringToNumber<int>(stringvec.at(1));
         }
         stringvec = pstylewkt->GetLeafChildren("VECTORSTYLE|LABEL|ANCHOR");
         if (stringvec.size() == 1) {
            pvstyle->GetLabel()->anchor_ = (Label::AnchorType) StringToNumber<int>(stringvec.at(0));
         }
         break;
      }
      default:
         break;
   }
   Wkt::Destroy(pstylewkt);
   return pvstyle;
}

/** Crea el VectorStyle */
/**
 * @param[in] VType tipo de  vector
 * @param[in] Name nombre del estilo
 * @return Estilo con tipo y nombre inidicados
 * \attention el VectorStyle retornado (y su eliminacion) es responsabilidad
 * del invocante
 */
VectorStyle * VectorStyle::Create(const Vector::VectorType &VType, const std::string &Name) {
   return new VectorStyle(VType, Name);
}

/**
 * Destruye el VectorStyle
 * @param[in] pVectorStyle estilo de vector que quiero destruir
 * @param[out] pVectorStyle luego de eliminar el estilo la variable se
 * setea a NULL.
 */
void VectorStyle::Destroy(VectorStyle * &pVectorStyle) {
   delete pVectorStyle;
   pVectorStyle = NULL;
}

/**
 * Indica el tipo de vector del estilo
 * @return tipo de vector
 */
Vector::VectorType VectorStyle::GetVectorType() const {
   return vType_;
}

/**
 * Devuelve el nombre del estilo
 * @return nobmre del estilo
 */
std::string VectorStyle::GetName() const {
   return name_;
}

/** Devuelve el brush */
/**
 * @return retorna putenro a Brush interno
 * \attention el Brush retornado es responsabilidad de esta clase (no eliminar externamente)
 */
VectorStyle::Brush * VectorStyle::GetBrush() const {
   return pBrush_;
}

/** Devuelve el pen */
/**
 * @return retorna puntero a pen interno
 * \attention el Pen retornado es responsabilidad de esta clase (no eliminar externamente)
 */
VectorStyle::Pen * VectorStyle::GetPen() const {
   return pPen_;
}

/** Devuelve el symbol */
/**
 * @return retorna puntero a symbol interno
 * \attention el Symbol retornado es responsabilidad de esta clase(no eliminar externamente)
 */
VectorStyle::Symbol * VectorStyle::GetSymbol() const {
   return pSymbol_;
}

/** Devuelve el label */
/**
 * @return retorna puntero a label interno
 * \attention el Label retornado es responsabilidad de esta clase (no eliminar externamente)
 */
VectorStyle::Label * VectorStyle::GetLabel() const {
   return pLabel_;
}

/** Devuelve el Wkt del estilo */
/**
 * Crea un Wkt y segun el tipo de vector que afecta el VectorStyle le carga
 * los datos necesarios
 * @return string con wkt que identifica al estilo
 */
std::string VectorStyle::GetWkt() const {
   // Nombre y tipo del estilo
   Wkt *pstylewkt = Wkt::Create(
         std::string("VECTORSTYLE[") + LongToString((long) GetVectorType()) + ",\"" + name_
               + "\"]");
   if (pstylewkt == NULL) {
      REPORT_ERROR("D:Error en Estilo de vector. No se puede crear el string de WKT.");
      return std::string();
   }

   std::vector < std::string > stringvec;
   switch (GetVectorType()) {
      case Vector::Polygon: {
         // Invert
         if (IsInverted()) {
            stringvec.clear();
            stringvec.push_back("Invert brush colors");
            stringvec.push_back("1");
            pstylewkt->InsertValue(stringvec, "VECTORSTYLE|INVERT");
         }
         // Brush
         stringvec.clear();
         stringvec.push_back(GetBrush()->system_);
         stringvec.push_back(LongToString(GetBrush()->id_));
         pstylewkt->InsertValue(stringvec, "VECTORSTYLE|BRUSH");
         stringvec.clear();
         stringvec.push_back(LongToString(GetBrush()->color_.red_));
         stringvec.push_back(LongToString(GetBrush()->color_.green_));
         stringvec.push_back(LongToString(GetBrush()->color_.blue_));
         stringvec.push_back(LongToString(GetBrush()->color_.alpha_));
         pstylewkt->InsertValue(stringvec, "VECTORSTYLE|BRUSH|COLOR");
      }
      case Vector::Line: {
         // Pen
         stringvec.clear();
         stringvec.push_back(GetPen()->system_);
         stringvec.push_back(LongToString(GetPen()->id_));
         pstylewkt->InsertValue(stringvec, "VECTORSTYLE|PEN");
         stringvec.clear();
         stringvec.push_back(LongToString(GetPen()->color_.red_));
         stringvec.push_back(LongToString(GetPen()->color_.green_));
         stringvec.push_back(LongToString(GetPen()->color_.blue_));
         stringvec.push_back(LongToString(GetPen()->color_.alpha_));
         pstylewkt->InsertValue(stringvec, "VECTORSTYLE|PEN|COLOR");
         pstylewkt->InsertValue(LongToString(GetPen()->width_), "VECTORSTYLE|PEN|WIDTH");
      }
      case Vector::Point: {
         // Symbol
         stringvec.clear();
         stringvec.push_back(GetSymbol()->system_);
         stringvec.push_back(LongToString(GetSymbol()->id_));
         pstylewkt->InsertValue(stringvec, "VECTORSTYLE|SYMBOL");
         stringvec.clear();
         stringvec.push_back(LongToString(GetSymbol()->color_.red_));
         stringvec.push_back(LongToString(GetSymbol()->color_.green_));
         stringvec.push_back(LongToString(GetSymbol()->color_.blue_));
         stringvec.push_back(LongToString(GetSymbol()->color_.alpha_));
         pstylewkt->InsertValue(stringvec, "VECTORSTYLE|SYMBOL|COLOR");
         pstylewkt->InsertValue(LongToString(GetSymbol()->size_), "VECTORSTYLE|SYMBOL|SIZE");
         // Label
         stringvec.clear();
         stringvec.push_back(GetLabel()->system_);
         stringvec.push_back(LongToString(GetLabel()->id_));
         pstylewkt->InsertValue(stringvec, "VECTORSTYLE|LABEL");
         pstylewkt->InsertValue(GetLabel()->expression_, "VECTORSTYLE|LABEL|EXPRESSION");
         stringvec.clear();
         stringvec.push_back(LongToString(GetLabel()->color_.red_));
         stringvec.push_back(LongToString(GetLabel()->color_.green_));
         stringvec.push_back(LongToString(GetLabel()->color_.blue_));
         stringvec.push_back(LongToString(GetLabel()->color_.alpha_));
         pstylewkt->InsertValue(stringvec, "VECTORSTYLE|LABEL|COLOR");
         stringvec.clear();
         stringvec.push_back(LongToString(GetLabel()->backColor_.red_));
         stringvec.push_back(LongToString(GetLabel()->backColor_.green_));
         stringvec.push_back(LongToString(GetLabel()->backColor_.blue_));
         stringvec.push_back(LongToString(GetLabel()->backColor_.alpha_));
         pstylewkt->InsertValue(stringvec, "VECTORSTYLE|LABEL|BACKCOLOR");
         pstylewkt->InsertValue(LongToString(GetLabel()->size_), "VECTORSTYLE|LABEL|SIZE");
         pstylewkt->InsertValue(LongToString(GetLabel()->italic_), "VECTORSTYLE|LABEL|ITALIC");
         pstylewkt->InsertValue(LongToString(GetLabel()->bold_), "VECTORSTYLE|LABEL|BOLD");
         pstylewkt->InsertValue(LongToString(GetLabel()->underlined_),
                                "VECTORSTYLE|LABEL|UNDERLINED");
         pstylewkt->InsertValue(LongToString(GetLabel()->angle_), "VECTORSTYLE|LABEL|ANGLE");
         stringvec.clear();
         stringvec.push_back(LongToString(GetLabel()->dx_));
         stringvec.push_back(LongToString(GetLabel()->dy_));
         pstylewkt->InsertValue(stringvec, "VECTORSTYLE|LABEL|OFFSET");
         pstylewkt->InsertValue(LongToString(GetLabel()->anchor_), "VECTORSTYLE|LABEL|ANCHOR");
         break;
      }
      default:
         break;
   }

   std::string stylestr = pstylewkt->GetWktString();
   Wkt::Destroy(pstylewkt);
   return stylestr;
}

/**
 * Cambia el nombre del estilo
 * @param[in] Name nuevo nombre del estilo
 */
void VectorStyle::ChangeName(const std::string &Name) {
   name_ = Name;
}

/** Clona el estilo */
/**
 * Crea nuevo VectorStile con nombre NewName y copia las
 * propiedades que correspondan
 * @param[in] NewName nombre del VectoStyle creado a partir de este
 * @return VectorStyle con igual configuracion pero otro nombre
 * \attention el VectorStyle retornado (y su eliminacion) es
 *            responsabilidad del invocante
 */
VectorStyle * VectorStyle::Clone(const std::string &NewName) const {
   std::string newname = NewName;
   if (newname.empty()) {
      newname = name_;
   }

   VectorStyle * pvstyle = new VectorStyle(vType_, NewName);
   switch (vType_) {
      case Vector::Polygon: {
         *(pvstyle->pBrush_) = *pBrush_;
         pvstyle->invert_ = invert_;
      }
      case Vector::Line: {
         *(pvstyle->pPen_) = *pPen_;
      }
      case Vector::Point: {
         *(pvstyle->pSymbol_) = *pSymbol_;
         *(pvstyle->pLabel_) = *pLabel_;
         break;
      }
      default:
         break;
   }

   return pvstyle;
}

/** Copia el label */
/**
 * Copia los datos del label actual al puntero destino
 * @param[in] PDestinationVector Puntero al VectorStyle destino
 * @return bool Devuelve verdadero si tuvo exito
 */
bool VectorStyle::CopyLabel(const VectorStyle* PDestinationVector) const {
   if (PDestinationVector->GetLabel() == NULL)
      return false;

   PDestinationVector->GetLabel()->system_ = this->GetLabel()->system_.substr();
   PDestinationVector->GetLabel()->id_ = this->GetLabel()->id_;
   PDestinationVector->GetLabel()->expression_ = this->GetLabel()->expression_.substr();
   PDestinationVector->GetLabel()->color_.red_ = this->GetLabel()->color_.red_;
   PDestinationVector->GetLabel()->color_.green_ = this->GetLabel()->color_.green_;
   PDestinationVector->GetLabel()->color_.blue_ = this->GetLabel()->color_.blue_;
   PDestinationVector->GetLabel()->color_.alpha_ = this->GetLabel()->color_.alpha_;
   PDestinationVector->GetLabel()->backColor_.red_ = this->GetLabel()->backColor_.red_;
   PDestinationVector->GetLabel()->backColor_.green_ = this->GetLabel()->backColor_.green_;
   PDestinationVector->GetLabel()->backColor_.blue_ = this->GetLabel()->backColor_.blue_;
   PDestinationVector->GetLabel()->backColor_.alpha_ = this->GetLabel()->backColor_.alpha_;
   PDestinationVector->GetLabel()->size_ = this->GetLabel()->size_;
   PDestinationVector->GetLabel()->italic_ = this->GetLabel()->italic_;
   PDestinationVector->GetLabel()->bold_ = this->GetLabel()->bold_;
   PDestinationVector->GetLabel()->underlined_ = this->GetLabel()->underlined_;
   PDestinationVector->GetLabel()->angle_ = this->GetLabel()->angle_;
   PDestinationVector->GetLabel()->dx_ = this->GetLabel()->dx_;
   PDestinationVector->GetLabel()->dy_ = this->GetLabel()->dy_;
   PDestinationVector->GetLabel()->anchor_ = this->GetLabel()->anchor_;
   return true;
}

/** Copia el pen */
/**
 * Copia los datos del pen actual al puntero destino
 * @param[in] PDestinationVector Puntero al VectorStyle destino
 * @return bool Devuelve verdadero si tuvo exito
 */
bool VectorStyle::CopyPen(const VectorStyle* PDestinationVector) const {
   if (PDestinationVector->GetPen() == NULL)
      return false;

   PDestinationVector->GetPen()->system_ = this->GetPen()->system_.substr();
   PDestinationVector->GetPen()->id_ = this->GetPen()->id_;
   PDestinationVector->GetPen()->color_.red_ = this->GetPen()->color_.red_;
   PDestinationVector->GetPen()->color_.green_ = this->GetPen()->color_.green_;
   PDestinationVector->GetPen()->color_.blue_ = this->GetPen()->color_.blue_;
   PDestinationVector->GetPen()->color_.alpha_ = this->GetPen()->color_.alpha_;
   PDestinationVector->GetPen()->width_ = this->GetPen()->width_;
   return true;
}

/** Copia el brush */
/**
 * Copia los datos del brush actual al puntero destino
 * @param[in] PDestinationVector Puntero al VectorStyle destino
 * @return bool Devuelve verdadero si tuvo exito
 */
bool VectorStyle::CopyBrush(const VectorStyle* PDestinationVector) const {
   if (PDestinationVector->GetBrush() == NULL)
      return false;

   PDestinationVector->GetBrush()->system_ = this->GetBrush()->system_.substr();
   PDestinationVector->GetBrush()->id_ = this->GetBrush()->id_;
   PDestinationVector->GetBrush()->color_.red_ = this->GetBrush()->color_.red_;
   PDestinationVector->GetBrush()->color_.green_ = this->GetBrush()->color_.green_;
   PDestinationVector->GetBrush()->color_.blue_ = this->GetBrush()->color_.blue_;
   PDestinationVector->GetBrush()->color_.alpha_ = this->GetBrush()->color_.alpha_;
   return true;
}

/** Copia el symbol */
/**
 * Copia los datos del symbol actual al puntero destino
 * @param[in] PDestinationVector Puntero al VectorStyle destino
 * @return bool Devuelve verdadero si tuvo exito
 */
bool VectorStyle::CopySymbol(const VectorStyle* PDestinationVector) const {
   if (PDestinationVector->GetSymbol() == NULL)
      return false;

   PDestinationVector->GetSymbol()->system_ = this->GetSymbol()->system_.substr();
   PDestinationVector->GetSymbol()->id_ = this->GetSymbol()->id_;
   PDestinationVector->GetSymbol()->color_.red_ = this->GetSymbol()->color_.red_;
   PDestinationVector->GetSymbol()->color_.green_ = this->GetSymbol()->color_.green_;
   PDestinationVector->GetSymbol()->color_.blue_ = this->GetSymbol()->color_.blue_;
   PDestinationVector->GetSymbol()->color_.alpha_ = this->GetSymbol()->color_.alpha_;
   PDestinationVector->GetSymbol()->size_ = this->GetSymbol()->size_;
   return true;
}

/** Verifica si el estilo es invertido **/
bool VectorStyle::IsInverted() const {
   return invert_;
}
} /*namespace suri*/

