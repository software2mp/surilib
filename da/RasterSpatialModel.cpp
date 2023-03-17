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
#include <math.h>
#include <map>
#include <stdlib.h>
#include <sstream>
#include <cassert>
#include <limits>

// Includes suri
#include "suri/RasterSpatialModel.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Wkt.h"
#include "suri/Dimension.h"

/** namespace suri */
namespace suri {
/** Ctor */
RasterSpatialModel::Parameters::Parameters() : isDirty_(false) {
   matrixModel_.assign(6, 0.0);
   pixelSizes_.assign(2, 0.0);
}

RasterSpatialModel::Parameters::~Parameters() {
}

/**
 * @return indica si es valido el modelo matricial
 */
bool RasterSpatialModel::Parameters::MatrixIsValid() const {
   // Verifica que el modelo sea inversible
   if (matrixModel_.size() != 6) return false;
   if (AreEqual(matrixModel_[1], fabs(matrixModel_[5])) && AreEqual(matrixModel_[2],
         matrixModel_[4])) {
      return true;
   } else if (AreEqual(matrixModel_[1] * matrixModel_[5], matrixModel_[2] *
                          matrixModel_[4])) {
      return false;
   } else {
      return true;
   }
}

/**
 * @return Indica si es valido el modelo pixels
 */
bool RasterSpatialModel::Parameters::PixelsIsValid() const {
   // Verifica que el modelo sea inversible
   if (pixelSizes_.size() == 2 && pixelSizes_[0] > 0.0 && pixelSizes_[1] > 0.0) {
      return true;
   } else {
      return false;
   }
}

/** Constructor */
RasterSpatialModel::RasterSpatialModel() : isDirty_(false) {
}

/** Destructor */
RasterSpatialModel::~RasterSpatialModel() {
}

// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------

/**
 * Devuelve la WKT del modelo a partir de los parametros (matriz o pixel)
 * @param[in] ModelParameters Parametros del modelo (modo matriz o pixel)
 * @param[in] Inverse Indica si se desea la transformacion inversa
 * @return Wkt del modelo
 */
std::string RasterSpatialModel::ParametersToWkt(const Parameters &ModelParameters,
                                                const bool Inverse) {
   RasterSpatialModel * tempmodel = Create(ModelParameters, Inverse);
   if (tempmodel == NULL) {
      return std::string();
   }

   std::string wkt = tempmodel->GetWkt();
   Destroy(tempmodel);

   return wkt;
}

/**
 * Devuelve los parametros del modelo (matriz y pixel) a partir de la WKT
 * @param[in] WktString WKT del modelo
 * @param[in] Inverse Indica si se desea la transformacion inversa
 * @return Parametros del modelo (matriz y pixel)
 */
RasterSpatialModel::Parameters RasterSpatialModel::WktToParameters(
      const std::string &WktString, const bool Inverse) {
   RasterSpatialModel * tempmodel = Create(WktString, Inverse);
   if (tempmodel == NULL) {
      return Parameters();
   }

   Parameters parameters = tempmodel->GetModelParameters();
   Destroy(tempmodel);

   return parameters;
}

// --------------------- METODOS ESTATICOS DE CREACION ----------------------

/**
 * Destruye el objeto
 * @param[in] pModel RasterSpatialModel que quiero destruir
 */
void RasterSpatialModel::Destroy(RasterSpatialModel * &pModel) {
   delete pModel;
   pModel = NULL;
}

/** Crea el modelo de transformacion a partir del WKT */
/**
 * Ejemplo de WKT:
 * INVERSE_MT[
 PARAM_MT["Affine",
 PARAMETER["num_row",2],
 PARAMETER["num_col",3],
 PARAMETER["elt_0_0",0],
 PARAMETER["elt_0_1",1],
 PARAMETER["elt_0_2",0],
 PARAMETER["elt_1_0",0],
 PARAMETER["elt_1_1",0],
 PARAMETER["elt_1_2",1],
 ]
 *
 * @param[in] WktString WKT del modelo
 * @param[in] Inverse Indica si se desea la transformacion inversa
 * @return puntero a objeto modelo, NULL si fallo
 */
RasterSpatialModel * RasterSpatialModel::Create(const std::string &WktString,
                                                const bool Inverse) {
   Wkt * pwkt = Wkt::Create(WktString);
   if (pwkt == NULL) {
      return NULL;
   }

   // Busca las inversiones
   bool inverse = Inverse; /*! Indica si es la transformacion inversa */
   std::string path = ""; /*! Path a la transformacion afin */
   while (!pwkt->GetWktString(path + "INVERSE_MT").empty()) {
      path += "INVERSE_MT|";
      inverse = !inverse;
   }

   // Verifica el formato de la transformacion
   path += "PARAM_MT";
   std::vector<std::string> nodechildren = pwkt->GetNodeChildren(path);
   std::vector<std::string> leafchildren = pwkt->GetLeafChildren(path);
   if (leafchildren.size() != 1 || leafchildren[0] != "Affine"
         || nodechildren.size() < 3) {
      Wkt::Destroy(pwkt);
      return NULL;
   }

   // Parsea los parametros de la transformacion (espera 2 filas y 3 columnas)
   path += "|PARAMETER";
   std::map<std::string, double> parameters;
   std::vector<double> paramvalues;
   for (leafchildren = pwkt->GetLeafChildren(path); leafchildren.size() == 2;
         pwkt->Delete(path), leafchildren = pwkt->GetLeafChildren(path))
      parameters[leafchildren[0]] = atof(leafchildren[1].c_str());
   Wkt::Destroy(pwkt);
   unsigned int rows = 0, columns = 0;
   if (parameters.find("num_row") != parameters.end()) {
      rows = (unsigned int) (parameters["num_row"] + 0.5);
   }
   if (parameters.find("num_col") != parameters.end()) {
      columns = (unsigned int) (parameters["num_col"] + 0.5);
   }
   if (rows != 2 || columns != 3 || parameters.size() != 2 + rows * columns) {
      return NULL;
   }
   for (unsigned int i = 0; i < rows; i++)
      for (unsigned int j = 0; j < columns; j++) {
         std::stringstream eltname;
         eltname << "elt_" << i << "_" << j;
         if (parameters.find(eltname.str()) != parameters.end()) {
            paramvalues.push_back(parameters[eltname.str()]);
         }
      }
   if (paramvalues.size() != rows * columns) {
      return NULL;
   }

   // Crea un objeto de parametros
   Parameters pmodelparameters;
   pmodelparameters.matrixModel_ = paramvalues;

   // Llama al metodo de creacion respectivo

   return Create(pmodelparameters, inverse);
}

/** Crea el modelo de transformacion a partir de los parametros */
/**
 * @param[in] ModelParameters Parametros del modelo(matriz o pixel)
 * @param[in] Inverse Indica si se desea la transformacion inversa
 * @return puntero a objeto modelo, NULL si fallo
 */
RasterSpatialModel *RasterSpatialModel::Create(const Parameters &ModelParameters,
                                               const bool Inverse) {
   RasterSpatialModel * pmodel = NULL;

   std::vector<double> transformmodel;
   std::vector<double> itransformmodel;

   // Verifica tipo de modelo
   if (ModelParameters.MatrixIsValid() && !ModelParameters.IsDirty()) {
      transformmodel = ModelParameters.matrixModel_;
   } else if (ModelParameters.PixelsIsValid()) {
      double ca2 = cos(ModelParameters.rotation_ * M_PI / 180.0);
      double sa2 = sin(ModelParameters.rotation_ * M_PI / 180.0);
      transformmodel.push_back(
            ModelParameters.tiePointXy_.x_
                  - ModelParameters.tiePointPl_.x_ * ca2
                        * ModelParameters.pixelSizes_[0]
                  + ModelParameters.tiePointPl_.y_ * sa2
                        * ModelParameters.pixelSizes_[1]);
      transformmodel.push_back(ca2 * ModelParameters.pixelSizes_[0]);
      transformmodel.push_back(sa2 * ModelParameters.pixelSizes_[1]);
      transformmodel.push_back(
            ModelParameters.tiePointXy_.y_
                  - ModelParameters.tiePointPl_.x_ * sa2
                        * ModelParameters.pixelSizes_[0]
                  - ModelParameters.tiePointPl_.y_ * ca2
                        * ModelParameters.pixelSizes_[1]);
      transformmodel.push_back(sa2 * ModelParameters.pixelSizes_[0]);
      transformmodel.push_back(-ca2 * ModelParameters.pixelSizes_[1]);
   } else {
      return NULL;
   }

   // Modelo inverso
   double det = transformmodel[1] * transformmodel[5]
         - transformmodel[2] * transformmodel[4];
   itransformmodel.push_back(
         (transformmodel[2] * transformmodel[3] - transformmodel[5] * transformmodel[0])
               / det);
   itransformmodel.push_back(transformmodel[5] / det);
   itransformmodel.push_back(-transformmodel[2] / det);
   itransformmodel.push_back(
         (transformmodel[0] * transformmodel[4] - transformmodel[1] * transformmodel[3])
               / det);
   itransformmodel.push_back(-transformmodel[4] / det);
   itransformmodel.push_back(transformmodel[1] / det);

   pmodel = new RasterSpatialModel;
   // Verifica si hay que invertir modelo
   if (Inverse) {
      pmodel->transformModel2d_ = itransformmodel;
      pmodel->inverseTransformModel2d_ = transformmodel;
   } else {
      pmodel->transformModel2d_ = transformmodel;
      pmodel->inverseTransformModel2d_ = itransformmodel;
   }

   return pmodel;
}

/** Crea el modelo de transformacion a partir de las esquinas de entrada y salida */
/**
 * @param[in] InUl UL del sistema de entrada
 * @param[in] InLr LR del sistema de entrada
 * @param[in] OutUl UL del sistema de salida
 * @param[in] OutLr LR del sistema de salida
 * @param[in] Rotation Rotacion del eje Y respecto al eje L en sentido horario (en grados)
 * @return puntero a objeto modelo, NULL si fallo
 * \todo Incorporar calculos utilizando Rotation
 */
RasterSpatialModel *RasterSpatialModel::Create(const Coordinates &InUl,
                                               const Coordinates &InLr,
                                               const Coordinates &OutUl,
                                               const Coordinates &OutLr,
                                               const double &Rotation) {
   if (FLOAT_COMPARE(InUl.x_, InLr.x_) || FLOAT_COMPARE(InUl.y_, InLr.y_)) {
      return NULL;
   }

   Parameters parameters;
   /*parameters.TiePointPl_ = InUl;
    parameters.TiePointXy_ = OutUl;
    parameters.Rotation_ = Rotation;
    parameters.PixelSizes_[0] = (OutLr.x_ - OutUl.x_)/(InLr.x_ - InUl.x_);
    parameters.PixelSizes_[1] = -(OutLr.y_ - OutUl.y_)/(InLr.y_ - InUl.y_);*/
   parameters.matrixModel_[1] = OutLr.x_ / (InLr.x_ - InUl.x_)
         - OutUl.x_ / (InLr.x_ - InUl.x_);
   parameters.matrixModel_[0] = OutUl.x_ - parameters.matrixModel_[1] * InUl.x_;
   parameters.matrixModel_[5] = OutLr.y_ / (InLr.y_ - InUl.y_)
         - OutUl.y_ / (InLr.y_ - InUl.y_);
   parameters.matrixModel_[3] = OutUl.y_ - parameters.matrixModel_[5] * InUl.y_;
   return Create(parameters);
}

// ------------------- METODOS DE CONSULTA DE CONFIGURACION -----------------

/** Devuelve el WKT de la transformacion espacial */
/**
 * @param[in] Inverse Indica si se desea la transformacion inversa
 * @return WKT del modelo de transformacion
 */
std::string RasterSpatialModel::GetWkt(const bool Inverse) const {
   if (transformModel2d_.empty() || inverseTransformModel2d_.empty()) {
      return std::string();
   }

   std::vector<double> transformmodel =
         Inverse ? inverseTransformModel2d_ : transformModel2d_;

   // Tipo de transformacion (afin)
   Wkt * pwkt = Wkt::Create("PARAM_MT[Affine]");
   if (pwkt == NULL) {
      return std::string();
   }

   // Filas y columnas
   unsigned int numrows = 2, numcols = 3;
   std::stringstream rowwkt, colwkt;
   rowwkt << "PARAMETER[\"num_row\"," << numrows << "]";
   colwkt << "PARAMETER[\"num_col\"," << numcols << "]";
   pwkt->InsertWkt(rowwkt.str(), "PARAM_MT");
   pwkt->InsertWkt(colwkt.str(), "PARAM_MT");
   // Elementos
   for (unsigned int i = 0; i < numrows; i++)
      for (unsigned int j = 0; j < numcols; j++) {
         std::stringstream eltname;
         eltname << "PARAMETER[\"elt_" << i << "_" << j << "\","
                 << DoubleToString(transformmodel.at(i * numcols + j)) << "]";
         pwkt->InsertWkt(eltname.str(), "PARAM_MT");
      }

   std::string wktmodel = pwkt->GetWktString();
   Wkt::Destroy(pwkt);
   return wktmodel;
}

/** Devuelve parametros del modelo (matriz y pixel) */
/**
 * @param[in] Inverse Indica si se desea la transformacion inversa
 * @return Parametros del modelo (matriz y pixel)
 * \todo Verificar esta condicion para modelo de tipo pixel
 */
RasterSpatialModel::Parameters RasterSpatialModel::GetModelParameters(
      const bool Inverse) const {
   if (transformModel2d_.empty() || inverseTransformModel2d_.empty()) {
      return Parameters();
   }

   Parameters modelparameters;
   std::vector<double> transformmodel =
         Inverse ? inverseTransformModel2d_ : transformModel2d_;

   // Modelo matriz
   modelparameters.matrixModel_ = transformmodel;

   // Modelo de pixel
   std::vector<double> pixelsizes;
   pixelsizes.push_back(
         sqrt(transformmodel.at(1) * transformmodel.at(1)
               + transformmodel.at(4) * transformmodel.at(4)));
   pixelsizes.push_back(
         sqrt(transformmodel.at(2) * transformmodel.at(2)
               + transformmodel.at(5) * transformmodel.at(5)));

   // Verifica si es posible ir del modelo matricial al modelo pixels
   // \todo Verificar esta condicion para modelo de tipo pixel
   double rotationx = acos(transformmodel.at(1) / pixelsizes[0]) * 180.0 / M_PI;
   double rotationy = acos(-transformmodel.at(5) / pixelsizes[1]) * 180.0 / M_PI;
   if (!FLOAT_COMPARE(rotationx, rotationy)) {
      pixelsizes[0] = pixelsizes[1] = 0.0;
   }
   modelparameters.pixelSizes_ = pixelsizes;
   modelparameters.rotation_ = rotationx;
   Coordinates tiepointPL(0.0, 0.0);
   Coordinates tiepointXY(transformmodel.at(0), transformmodel.at(3));
   modelparameters.tiePointPl_ = tiepointPL;
   modelparameters.tiePointXy_ = tiepointXY;
   return modelparameters;
}

// -------------------- METODOS DE TRANSFORMACION ---------------------------
/** Transforma de P-L a X-Y */
/**
 * @param[in] InOut Coordenadas a transformar por el modelo
 * @param[out] InOut Coordenadas transformadas por el modelo
 */
void RasterSpatialModel::Transform(Coordinates &InOut) const {
   assert(transformModel2d_.size()==6);
   double x = InOut.x_;
   double y = InOut.y_;
   InOut.x_ = transformModel2d_[0] + x * transformModel2d_[1]
         + y * transformModel2d_[2];
   InOut.y_ = transformModel2d_[3] + x * transformModel2d_[4]
         + y * transformModel2d_[5];
}

/**
 * Transforma un subset de (pixel, linea) a (x, y)
 * @param[in] SubsetInOut subset a transformar por el modelo
 * @param[out] SubsetInOut subset a transformado por el modelo
 */
void RasterSpatialModel::Transform(Subset &SubsetInOut) const {
   Dimension subsetdim(SubsetInOut);
   int signx = subsetdim.XSign();
   int signy = subsetdim.YSign();
   // transformo las otras dos esquinas del subset
   Coordinates ur(SubsetInOut.lr_.x_, SubsetInOut.ul_.y_);
   Coordinates ll(SubsetInOut.ul_.x_, SubsetInOut.lr_.y_);
   // transformo las esquina a xy
   Transform(SubsetInOut.ul_);
   Transform(SubsetInOut.lr_);
   Transform(ur);
   Transform(ll);
   std::vector<Coordinates*> coordinates;
   coordinates.push_back(&SubsetInOut.ul_);
   coordinates.push_back(&SubsetInOut.lr_);
   coordinates.push_back(&ur);
   coordinates.push_back(&ll);
   double minx = GetMinX(coordinates);
   double miny = GetMinY(coordinates);
   double maxx = GetMaxX(coordinates);
   double maxy = GetMaxY(coordinates);
   // en base a los signos de los ejes ajusto la esquinas(ul y lr) del subset
   Coordinates outputul, outputlr;
   if (signx > 0 && signy > 0 ) {
      outputul = Coordinates(minx, miny);
      outputlr = Coordinates(maxx, maxy);
   } else if (signx > 0 && signy < 0 ) {
      outputul = Coordinates(maxx, miny);
      outputlr = Coordinates(maxx, maxy);
   } else if (signx < 0 && signy > 0 ) {
      outputul = Coordinates(minx, maxy);
      outputlr = Coordinates(maxx, miny);
   } else if (signx < 0 && signy < 0 ) {
      outputul = Coordinates(minx, maxy);
      outputlr = Coordinates(minx, miny);
   }
   // genero el subset de salida
   SubsetInOut = Subset(outputul.x_, outputul.y_, outputlr.x_, outputlr.y_);
}

/** Transforma de P-L a X-Y */
/**
 * @param[in] InOut Vector con coordenadas a transformar por el modelo
 * @param[out] InOut Vector con transformadas por el modelo
 */
void RasterSpatialModel::Transform(std::vector<Coordinates> &InOut) const {
   size_t totalcoord = InOut.size();
   for (size_t i = 0; i < totalcoord; i++)
      Transform(InOut[i]);
}

/** Transforma de X-Y a P-L */
/**
 * @param[in] InOut Coordenadas a transformar por el modelo inverso
 * @param[out] InOut Coordenadas transformadas por el modelo inverso
 */
void RasterSpatialModel::InverseTransform(Coordinates &InOut) const {
   assert(inverseTransformModel2d_.size()==6);
   double x = InOut.x_;
   double y = InOut.y_;
   InOut.x_ = inverseTransformModel2d_[0] + x * inverseTransformModel2d_[1]
         + y * inverseTransformModel2d_[2];
   InOut.y_ = inverseTransformModel2d_[3] + x * inverseTransformModel2d_[4]
         + y * inverseTransformModel2d_[5];
}

/** Transforma un subset de X-Y a P-L */
Subset RasterSpatialModel::InverseTransform(const Subset& SubsetIn) const {
   Subset result = SubsetIn;
   InverseTransform(result.lr_);
   InverseTransform(result.ul_);
   return result;
}


/** Transforma de X-Y a P-L */
/**
 * @param[in] InOut Vector con coordenadas a transformar por el modelo inverso
 * @param[out] InOut Vector con transformadas por el modelo inverso
 */
void RasterSpatialModel::InverseTransform(std::vector<Coordinates> &InOut) const {
   size_t totalcoord = InOut.size();
   for (size_t i = 0; i < totalcoord; i++)
      InverseTransform(InOut[i]);
}

/** */
void RasterSpatialModel::GetGeoTransform(double* GetTrans) const {
   for (int ix = 0, lenix = transformModel2d_.size(); ix < lenix; ++ix) {
      GetTrans[ix] = transformModel2d_[ix];
   }
}

/** Metodo auxiliar que dado un vector de coordenadas busca la menor en coordenadas
 *  del eje x.
 * @param CoordinatesVector vector de coordenadas
 * @return double con coordenada x
 */
double RasterSpatialModel::GetMinX(
      const std::vector<Coordinates*>& CoordinatesVector) const {
   double minx = std::numeric_limits<double>::max();
   std::vector<Coordinates*>::const_iterator it = CoordinatesVector.begin();
   for ( ; it != CoordinatesVector.end(); ++it ) {
      if ((*it)->x_ <= minx) {
         minx = (*it)->x_;
      }
   }
   return minx;
}

/** Metodo auxiliar que dado un vector de coordenadas busca la menor en coordenadas
 *  del eje y
 * @param CoordinatesVector vector de coordenadas
 * @return double con coordenada y
 */
double RasterSpatialModel::GetMinY(
      const std::vector<Coordinates*>& CoordinatesVector) const {
   double miny = std::numeric_limits<double>::max();
   std::vector<Coordinates*>::const_iterator it = CoordinatesVector.begin();
   for ( ; it != CoordinatesVector.end(); ++it ) {
      if ((*it)->y_ <= miny) {
         miny = (*it)->y_;
      }
   }
   return miny;
}

/** Metodo auxiliar que dado un vector de coordenadas busca la mayor en coordenadas
 *  del eje x.
 * @param CoordinatesVector vector de coordenadas
 * @return double con coordenada x
 */
double RasterSpatialModel::GetMaxX(
      const std::vector<Coordinates*>& CoordinatesVector) const {
   double maxx = std::numeric_limits<double>::min();
   std::vector<Coordinates*>::const_iterator it = CoordinatesVector.begin();
   for ( ; it != CoordinatesVector.end(); ++it ) {
      if ((*it)->x_ >= maxx) {
         maxx = (*it)->x_;
      }
   }
   return maxx;
}

/** Metodo auxiliar que dado un vector de coordenadas busca la mayor en coordenadas
 *  del eje y.
 * @param CoordinatesVector vector de coordenadas
 * @return double con coordenada y
 */
double RasterSpatialModel::GetMaxY(
      const std::vector<Coordinates*>& CoordinatesVector) const {
   double maxy = std::numeric_limits<double>::min();
   std::vector<Coordinates*>::const_iterator it = CoordinatesVector.begin();
   for ( ; it != CoordinatesVector.end(); ++it ) {
      if ((*it)->y_ >= maxy) {
         maxy = (*it)->y_;
      }
   }
   return maxy;
}


} /*namespace suri*/

