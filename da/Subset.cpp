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

#include "suri/Subset.h"

// Includes standard
#include <cmath>

// Includes suri
#include "suri/AuxiliaryFunctions.h"
#include "suri/Dimension.h"
#include "suri/XmlFunctions.h"
/** namespace suri */
namespace suri {
/**
 * Construye un subset a partir de las esquinas
 * @param[in]	Ulx: upper-left x
 * @param[in]	Uly: upper-left y
 * @param[in]	Lrx: lower-right x
 * @param[in]	Lry: lower-right y
 */
Subset::Subset(double Ulx, double Uly, double Lrx, double Lry) :
      ul_(Coordinates(Ulx, Uly)), lr_(Coordinates(Lrx, Lry)) {
}

/**
 * Construye un subset a partir de las esquinas
 * @param[in] UL esquina upper-left
 * @param[in] LR esquina lower-right
 */
Subset::Subset(const Coordinates &UL, const Coordinates &LR) :
      ul_(UL), lr_(LR) {
}

/**
 * Verifica si el punto cae adentro
 * @param[in] Point punto que quiero analizar
 * @return informa si punto esta dentro del subset
 */
bool Subset::IsInside(const Coordinates &Point) const {
   Dimension dim(ul_, lr_);
   bool xinside = false, yinside = false;
   if (dim.XSign() > 0) {
      if (Point.x_ >= ul_.x_ && Point.x_ <= lr_.x_) {
         xinside = true;
      }
   } else if (Point.x_ <= ul_.x_ && Point.x_ >= lr_.x_) {
      xinside = true;
   }
   if (dim.YSign() > 0) {
      if (Point.y_ >= ul_.y_ && Point.y_ <= lr_.y_) {
         yinside = true;
      }
   } else if (Point.y_ <= ul_.y_ && Point.y_ >= lr_.y_) {
      yinside = true;
   }
   return xinside && yinside;
}

/**
 * Encuentra el punto mas cercano dentro del subset
 * @param[in] Point punto que quiero analizar
 * @return coordenadas del punto mas cercano dentro del subset
 */
Coordinates Subset::NearestPoint(const Coordinates &Point) {
   if (IsInside(Point)) {
      return Point;
   }

   Dimension dim(ul_, lr_);
   Coordinates nearestpoint = Point;

   if (dim.XSign() > 0) {
      if (Point.x_ < ul_.x_) {
         nearestpoint.x_ = ul_.x_;
      }
      if (Point.x_ > lr_.x_) {
         nearestpoint.x_ = lr_.x_;
      }
   } else {
      if (Point.x_ > ul_.x_) {
         nearestpoint.x_ = ul_.x_;
      }
      if (Point.x_ < lr_.x_) {
         nearestpoint.x_ = lr_.x_;
      }
   }

   if (dim.YSign() > 0) {
      if (Point.y_ < ul_.y_) {
         nearestpoint.y_ = ul_.y_;
      }
      if (Point.y_ > lr_.y_) {
         nearestpoint.y_ = lr_.y_;
      }
   } else {
      if (Point.y_ > ul_.y_) {
         nearestpoint.y_ = ul_.y_;
      }
      if (Point.y_ < lr_.y_) {
         nearestpoint.y_ = lr_.y_;
      }
   }

   return nearestpoint;
}

/**
 * Verifica que un subset sea valido.
 * return true: el subset no esta formado por un solo punto en la coordenada 0.0
 * return false: el subset corresponde a un punto en la coordenada 0.0 (para ul
 * y lr).
 * @return informa si el subset es valido
 * \note: se considera como subset invalido al punto 0.0, pero hay casos, por
 * ejemplo el resultado de una interseccion que podrian dar este subset como
 * resultado, y no deberia ser invalido.
 * \todo si las coordenadas no fuera publicas se podria distinguir cuando un
 * subset tiene coordenadas 0.0 porque fue creado con esos valores, o cuando
 * tiene esas coordenadas porque fueron seteadas asi.
 */
bool Subset::IsValid() const {
   Coordinates zeropoint;

   if (ul_.IsEqual(zeropoint) && lr_.IsEqual(zeropoint)) {
      return false;
   }
   return true;
}

/**
 * Operador de suma de un entero a un subset (lo hace crecer)
 * @param[in] Lhs subset original
 * @param[in] Buffer entero que quiero sumar
 * @return subset con entero sumado
 */
Subset operator+(const Subset &Lhs, double Buffer) {
   Dimension dim(Lhs);
   Subset result;
   result.ul_.x_ = Lhs.ul_.x_ - dim.XSign() * Buffer;
   result.ul_.y_ = Lhs.ul_.y_ - dim.YSign() * Buffer;

   result.lr_.x_ = Lhs.lr_.x_ + dim.XSign() * Buffer;
   result.lr_.y_ = Lhs.lr_.y_ + dim.YSign() * Buffer;

   return result;
}

Subset Subset::ToOrigin() {
   Coordinates origin;
   Coordinates newLr(lr_.x_ - ul_.x_, lr_.y_ - ul_.y_);
   Subset result(origin, newLr);
   return result;
}


/**
 * Funcion que genera un subset con la interseccion de los subsets
 * pasados por parametro.
 * \pre los subsets deben tener el mismo sistema de coordenadas
 * \post el subset retornado tendra el sistema de coordenadas de los subset
 *       de entrada
 * @param[in] Subset1 primer subset a intersectar
 * @param[in] Subset2 segundo subset a intersectar
 * @return subset que contiene con la interseccion
 * @return Subset(0,0,0,0) en caso de error
 * \todo Definir una forma de indicar el error sin devolver un Subset
 *       potencialmente valido.
 */
Subset Subset::Intersect(const Subset &Subset2) const{
   // Verifico que los ejes sean compatibles
   Dimension dim1(*this);
   Dimension dim2(Subset2);
   if ((dim1.XSign() != dim2.XSign()) || (dim1.YSign() != dim2.YSign())) {
      return Subset(0, 0, 0, 0);
   }

   Subset ret(0, 0, 0, 0);
   // Segun coordenadas asigno los extremos a punta correcta
   // en x
   if (dim1.XSign() > 0) {
      ret.ul_.x_ = std::max(this->ul_.x_, Subset2.ul_.x_);
      ret.lr_.x_ = std::min(this->lr_.x_, Subset2.lr_.x_);
   } else {
      ret.ul_.x_ = std::min(this->ul_.x_, Subset2.ul_.x_);
      ret.lr_.x_ = std::max(this->lr_.x_, Subset2.lr_.x_);
   }
   // en y
   if (dim1.YSign() > 0) {
      ret.ul_.y_ = std::max(this->ul_.y_, Subset2.ul_.y_);
      ret.lr_.y_ = std::min(this->lr_.y_, Subset2.lr_.y_);
   } else {
      ret.ul_.y_ = std::min(this->ul_.y_, Subset2.ul_.y_);
      ret.lr_.y_ = std::max(this->lr_.y_, Subset2.lr_.y_);
   }
   // Si no hay interseccion los puntos devueltos no estaran en posicion correcta
   // dentro del subset y parecera que los signos de las coordenadas cambiaron
   Dimension retdim(ret);
   if ((retdim.XSign() != dim2.XSign()) || (retdim.YSign() != dim2.YSign())) {
      return Subset(0, 0, 0, 0);
   }

   return ret;
}

/**
 * Retorna un subset que contenga a Subset1 y Subset2
 * @param[in] Subset1 primer subset a contener
 * @param[in] Subset2 segundo subset a contener
 * return Subset que contiene a Subset1 y a Subset2
 */
Subset Join(const Subset &Subset1, const Subset &Subset2) {
   Dimension dim(Subset1);
   int sx, sy;
   dim.GetSign(sx, sy);

   double ulx, uly;
   double lrx, lry;
   if (sx > 0) {
      ulx = std::min(Subset1.ul_.x_, Subset2.ul_.x_);
      lrx = std::max(Subset1.lr_.x_, Subset2.lr_.x_);
   } else {
      ulx = std::max(Subset1.ul_.x_, Subset2.ul_.x_);
      lrx = std::min(Subset1.lr_.x_, Subset2.lr_.x_);
   }
   if (sy > 0) {
      uly = std::min(Subset1.ul_.y_, Subset2.ul_.y_);
      lry = std::max(Subset1.lr_.y_, Subset2.lr_.y_);
   } else {
      uly = std::max(Subset1.ul_.y_, Subset2.ul_.y_);
      lry = std::min(Subset1.lr_.y_, Subset2.lr_.y_);
   }

   return Subset(ulx, uly, lrx, lry);
}

/** 
 * Retorna un string que representa la definicion del subset dada  por las esquinas que lo componen
 * @param[in] os stream de salida a utilizar
 * @param[in] Asubset subste sobre el cual se obtendra el string
 * @return string con las coordenadas que corresponden a la esquina sup. izq. y la esq. inf. der.
**/
std::ostream & operator<<(std::ostream &os, const Subset& Asubset) {
   os << "ul=" << Asubset.ul_.x_ << "," << Asubset.ul_.y_ << ";";
   os << "lr=" << Asubset.lr_.x_ << "," << Asubset.lr_.y_;
   return os;
}

/** Obtiene el centro */
Coordinates Subset::GetCenter() const {
   return Coordinates((ul_.x_ + lr_.x_) / 2.0, (ul_.y_ + lr_.y_) / 2.0,
                      (ul_.z_ + lr_.z_) / 2.0);
}

/**
 * Extrae un vector de coordenadas con una determinada distancia
 * entre cada una.
 *
 * @param[in] SubsetP Subset del cual sacar las coordenadas.
 * @param[in] PointsCountP Limite de coordenadas a calcular para el subset.
 * @param[in] DeltaP Cada cuanto hay que sacar una coordenada.
 */
std::vector<Coordinates> Subset::ExtractCoordinates(const Subset& SubsetP,
                                                    int PointsCountP, int DeltaP) {
   std::vector<Coordinates> coords;

   double dx = (SubsetP.lr_.x_ - SubsetP.ul_.x_) / (PointsCountP - 1);
   double dy = (SubsetP.lr_.y_ - SubsetP.ul_.y_) / (PointsCountP - 1);

   for (int ix = 0; ix < PointsCountP; ++ix) {
      for (int jx = 0; jx < PointsCountP; jx += DeltaP) {
         coords.push_back(Coordinates(SubsetP.ul_.x_ + ix * dx, SubsetP.ul_.y_ + jx * dy));
      }
   }

   return coords;
}

wxXmlNode* Subset::SerializeXml() const{
   wxXmlNode* pXmlRoot = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(SUBSET_NODE));
   //Recordar que los xml hijos se agregan FIFO
   //Coordenadas UL
   wxXmlNode* pXmlChild = new wxXmlNode(pXmlRoot, wxXML_ELEMENT_NODE, wxT(SUBSET_UL_NODE));
   CreateKeyValueNode(SUBSET_Z_NODE, NumberToString<double>(this->ul_.z_), pXmlChild);
   CreateKeyValueNode(SUBSET_Y_NODE, NumberToString<double>(this->ul_.y_), pXmlChild);
   CreateKeyValueNode(SUBSET_X_NODE, NumberToString<double>(this->ul_.x_), pXmlChild);
   //Coordenadas LR
   pXmlChild = new wxXmlNode(pXmlRoot, wxXML_ELEMENT_NODE, wxT(SUBSET_LR_NODE));
   CreateKeyValueNode(SUBSET_Z_NODE, NumberToString<double>(this->lr_.z_), pXmlChild);
   CreateKeyValueNode(SUBSET_Y_NODE, NumberToString<double>(this->lr_.y_), pXmlChild);
   CreateKeyValueNode(SUBSET_X_NODE, NumberToString<double>(this->lr_.x_), pXmlChild);
   return pXmlRoot;
}

Subset* Subset::DeserializeXml(wxXmlNode* pRoot){
   wxXmlNode* pChild = pRoot->GetChildren();
   Subset* pSubset = new Subset();
   wxXmlNode* pAuxChild = pChild->GetChildren();
   pSubset->lr_.x_ = Serializable::DeserializeDouble(pAuxChild, SUBSET_X_NODE);
   pAuxChild = pAuxChild->GetNext();
   pSubset->lr_.y_ = Serializable::DeserializeDouble(pAuxChild, SUBSET_Y_NODE);
   pAuxChild = pAuxChild->GetNext();
   pSubset->lr_.z_ = Serializable::DeserializeDouble(pAuxChild, SUBSET_Z_NODE);
   pChild = pChild->GetNext();
   pAuxChild = pChild->GetChildren();
   pSubset->ul_.x_ = Serializable::DeserializeDouble(pAuxChild, SUBSET_X_NODE);
   pAuxChild = pAuxChild->GetNext();
   pSubset->ul_.y_ = Serializable::DeserializeDouble(pAuxChild, SUBSET_Y_NODE);
   pAuxChild = pAuxChild->GetNext();
   pSubset->ul_.z_ = Serializable::DeserializeDouble(pAuxChild, SUBSET_Z_NODE);
   return pSubset;
}

Subset Intersect(const Subset& Subset1, const Subset& Subset2) {
   return Subset1.Intersect(Subset2);
}


} /*namespace suri*/
