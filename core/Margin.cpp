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

// Includes Suri
#include "Margin.h"
#include "suri/VectorElement.h"
#include "suri/VectorEditor.h"
#include "suri/VectorStyleManager.h"
#include "suri/ToolSupport.h"
#include "suri/DataViewManager.h"

// Includes Wx
#include "wx/utils.h"
#include "wx/filename.h"

// Defines
#define TEXT_FIELD "Text"
#define TEXT_FIELD_LENGTH 50

namespace suri {

/** Nombre de las capas utilizadas para margenes, titulo y subtitulo */
const std::string Margin::TitleLayerName = "tmp_visualization_exporter_titlelayername";
const std::string Margin::SubtitleLayerName = "tmp_visualization_exporter_subtitlelayername";
const std::string Margin::TopMarginLayerName = "tmp_visualization_exporter_tm";
const std::string Margin::BottomMarginLayerName = "tmp_visualization_exporter_bm";
const std::string Margin::LeftMarginLayerName = "tmp_visualization_exporter_lm";
const std::string Margin::RightMarginLayerName = "tmp_visualization_exporter_rm";

/** Estilo vectorial para los titulos */
const std::string Margin::TitleFontStyle =
      "VECTORSTYLE[1,\"tmp_visualization_exporter_titlelayername\",SYMBOL[\"Suri-symbol\",0,COLOR \
      [179,145,204,255],SIZE[1]],LABEL[\"Suri-label\",1,EXPRESSION[\"{Text}\"],COLOR[1,1,1,255],\
      BACKCOLOR[1,1,1,0],SIZE[16],ITALIC[0],BOLD[0],UNDERLINED[0],ANGLE[0],OFFSET[0,0],ANCHOR[4]]]";

/** Estilo vectorial para los subtitulos */
const std::string Margin::SubtitleFontStyle =
      "VECTORSTYLE[1,\"tmp_visualization_exporter_subtitlelayername\",SYMBOL[\"Suri-symbol\",0, \
      COLOR[179,145,204,255],SIZE[1]],LABEL[\"Suri-label\",1,EXPRESSION[\"{Text}\"], \
      COLOR[1,1,1,255],BACKCOLOR[1,1,1,0],SIZE[16],ITALIC[0],BOLD[0],UNDERLINED[0],ANGLE[0], \
      OFFSET[0,0],ANCHOR[4]]]";

/** Estilo vectorial para los margenes */
const std::string Margin::MarginStyle = "VECTORSTYLE[3,\"tmp_visualization_exporter_tm\", \
      BRUSH[\"Suri-brush\",1,COLOR[255,255,255,255]],PEN[\"Suri-pen\",1,COLOR[255,255,255,255], \
      WIDTH[1]],SYMBOL[\"Suri-symbol\",0,COLOR[0,0,0,255],SIZE[1]],LABEL[\"Suri-label\",0, \
      EXPRESSION[],COLOR[0,0,0,255],BACKCOLOR[0,0,0,0],SIZE[8],ITALIC[0],BOLD[0],UNDERLINED[0], \
      ANGLE[0],OFFSET[0,0],ANCHOR[4]]]";

/** Constructor */
Margin::Margin(ReprojectionProcess::ReprojectionParameters *pReprojectionParameters,
               std::string Title, std::string Subtitle) :
      pReprojectionParameters_(pReprojectionParameters), topmargin_(0), bottommargin_(0),
      leftmargin_(0), rightmargin_(0), title_(Title), subtitle_(Subtitle), pOutputWorld_(NULL) {
}

/** Destructor */
Margin::~Margin() {
}

/**
 * Crea un margen en el raster con el tamanio especificado por parametro
 * @param[in] TopMargin tamanio del margen superior
 * @param[in] BottomMargin tamanio del margen inferior
 * @param[in] LeftMargin tamanio del margen izquierdo
 * @param[in] RightMargin tamanio del margen derecho
 */
void Margin::SetMargins(double TopMargin, double BottomMargin, double LeftMargin,
                        double RightMargin, double Pixels, double Lines) {
   pPal_->GetAttribute<World*>(ProcessAdaptLayer::OutputWorldKeyAttr, pOutputWorld_);
   // Factor de conversion utilizado para convertir el tamano del margen en centimetros ingresado
   // 1 centimetro = 37.79527559055 pixeles
   double factor = 37.79527559055;
   topmargin_ = TopMargin * factor;
   bottommargin_ = BottomMargin * factor;
   leftmargin_ = LeftMargin * factor;
   rightmargin_ = RightMargin * factor;
   int sizex = leftmargin_;
   int sizey = topmargin_;
   pPal_->AddAttribute<int>(ProcessAdaptLayer::XMarginSizeKeyAttr, sizex);
   pPal_->AddAttribute<int>(ProcessAdaptLayer::YMarginSizeKeyAttr, sizey);
   RemoveTmpFiles();
   CreateTopMargin(Pixels, topmargin_);
   CreateBottomMargin(Pixels, Lines);
   CreateLeftMargin(Lines);
   CreateRightMargin(Pixels, Lines);
   UpdateOutputWorld(GetPoint(0, 0), GetPoint(Pixels, Lines));
   double posx = (Pixels + leftmargin_ + rightmargin_) / 2;
   double posy = topmargin_ / 2;
   CreateLabel(posx, posy, TitleLayerName, title_);
   posx = (Pixels + leftmargin_ + rightmargin_) / 2;
   posy = Lines - bottommargin_ / 2;
   CreateLabel(posx, posy, SubtitleLayerName, subtitle_);
}

/**
 * Crea una capa de punto con el texto especificado
 * @param[in] PositionX posicion en el eje x
 * @param[in] PositionY posicion en el eje y
 * @param[in] LayerName nombre de la capa
 * @param[in] Text texto del label
 */
void Margin::CreateLabel(double PositionX, double PositionY, std::string LayerName,
                         std::string Text) {
   VectorEditor pvectoreditor;
   pvectoreditor.OpenVector(GetTmpFilename(LayerName, ".shp"));
   pvectoreditor.OpenLayer(LayerName, pOutputWorld_->GetSpatialReference(), Vector::Point);
   pvectoreditor.CreateField(TEXT_FIELD, DataInfo<unsigned char>::Name, TEXT_FIELD_LENGTH);
   pvectoreditor.OpenFeature(0);
   pvectoreditor.SetPoint(GetPoint(PositionX, PositionY));
   pvectoreditor.SetField(TEXT_FIELD, Text);
   pvectoreditor.CloseFeature();
   pvectoreditor.CloseLayer();
   pvectoreditor.CloseVector();
}

/**
 * Retorna la ruta a un archivo con el nombre y la extension especificados en el directorio
 * temporal del sistema
 * @param[in] LayerName nombre de la capa
 * @param[in] Ext extension del archivo
 * @return string con path a un archivo temporal
 */
std::string Margin::GetTmpFilename(std::string LayerName, std::string Ext) {
   std::string tmpfilename = wxFileName::GetTempDir().c_str();
   tmpfilename.append("/");
   tmpfilename.append(LayerName);
   tmpfilename.append(Ext);
   return tmpfilename;
}

/**
 * Crea un vector que contiene los pixeles que formaran parte del margen superior
 * @param[in] Pixels ancho del margen
 * @param[in] Lines alto del margen
 */
void Margin::CreateTopMargin(double Pixels, double Lines) {
   if (topmargin_ <= 0)
      return;
   VectorEditor pvectoreditor;
   pvectoreditor.OpenVector(GetTmpFilename(TopMarginLayerName, ".shp"));
   pvectoreditor.OpenLayer(TopMarginLayerName, pOutputWorld_->GetSpatialReference(), Vector::Polygon);
   pvectoreditor.OpenFeature(0);
   // Esquina inferior izquierda
   Coordinates lowerleft = GetPoint(0, 0);
   pvectoreditor.AddPoint(lowerleft);
   // Esquina inferior derecha
   Coordinates lowerright = GetPoint(Pixels, 0);
   pvectoreditor.AddPoint(lowerright);
   // Esquina superior derecha
   pReprojectionParameters_->pRasterSpatialModelIn_->InverseTransform(lowerright);
   lowerright.y_ -= topmargin_;
   pReprojectionParameters_->pRasterSpatialModelIn_->Transform(lowerright);
   pvectoreditor.AddPoint(lowerright);
   // Esquina superior izquierda
   pReprojectionParameters_->pRasterSpatialModelIn_->InverseTransform(lowerleft);
   lowerleft.y_ -= topmargin_;
   pReprojectionParameters_->pRasterSpatialModelIn_->Transform(lowerleft);
   pvectoreditor.AddPoint(lowerleft);
   pvectoreditor.CloseFeature();
   pvectoreditor.CloseLayer();
   pvectoreditor.CloseVector();
}

/**
 * Crea un vector que contiene los pixeles que formaran parte del margen izquierdo
 * @param[in] Pixels ancho del margen
 */
void Margin::CreateLeftMargin(double Lines) {
   if (leftmargin_ <= 0)
      return;
   VectorEditor pvectoreditor;
   pvectoreditor.OpenVector(GetTmpFilename(LeftMarginLayerName, ".shp"));
   pvectoreditor.OpenLayer(LeftMarginLayerName, pOutputWorld_->GetSpatialReference(), Vector::Polygon);
   pvectoreditor.OpenFeature(0);
   // Esquina superior derecha
   Coordinates upperright = GetPoint(0, 0);
   if (topmargin_ > 0) {
      pReprojectionParameters_->pRasterSpatialModelIn_->InverseTransform(upperright);
      upperright.y_ -= topmargin_;
      pReprojectionParameters_->pRasterSpatialModelIn_->Transform(upperright);
   }
   pvectoreditor.AddPoint(upperright);
   // Esquina inferior derecha
   Coordinates lowerright = GetPoint(0, Lines);
   if (bottommargin_ > 0) {
      pReprojectionParameters_->pRasterSpatialModelIn_->InverseTransform(lowerright);
      lowerright.y_ += bottommargin_;
      pReprojectionParameters_->pRasterSpatialModelIn_->Transform(lowerright);
   }
   pvectoreditor.AddPoint(lowerright);
   // Esquina inferior izquierda
   pReprojectionParameters_->pRasterSpatialModelIn_->InverseTransform(lowerright);
   lowerright.x_ -= leftmargin_;
   pReprojectionParameters_->pRasterSpatialModelIn_->Transform(lowerright);
   pvectoreditor.AddPoint(lowerright);
   // Esquina superior izquierda
   pReprojectionParameters_->pRasterSpatialModelIn_->InverseTransform(upperright);
   upperright.x_ -= leftmargin_;
   pReprojectionParameters_->pRasterSpatialModelIn_->Transform(upperright);
   pvectoreditor.AddPoint(upperright);
   pvectoreditor.CloseFeature();
   pvectoreditor.CloseLayer();
   pvectoreditor.CloseVector();
}

/**
 * Crea un vector que contiene los pixeles que formaran parte del margen inferior
 * @param[in] Pixels ancho del margen
 * @param[in] Lines alto del margen
 */
void Margin::CreateBottomMargin(double Pixels, double Lines) {
   if (bottommargin_ <= 0)
      return;
   VectorEditor pvectoreditor;
   pvectoreditor.OpenVector(GetTmpFilename(BottomMarginLayerName, ".shp"));
   pvectoreditor.OpenLayer(BottomMarginLayerName, pOutputWorld_->GetSpatialReference(), Vector::Polygon);
   pvectoreditor.OpenFeature(0);
   // Esquina superior izquierda
   Coordinates upperleft = GetPoint(0, Lines);
   pvectoreditor.AddPoint(upperleft);
   // Esquina superior derecha
   Coordinates upperright = GetPoint(Pixels, Lines);
   pvectoreditor.AddPoint(upperright);
   // Esquina inferior derecha
   pReprojectionParameters_->pRasterSpatialModelIn_->InverseTransform(upperright);
   upperright.y_ += bottommargin_;
   pReprojectionParameters_->pRasterSpatialModelIn_->Transform(upperright);
   pvectoreditor.AddPoint(upperright);
   // Esquina inferior izquierda
   pReprojectionParameters_->pRasterSpatialModelIn_->InverseTransform(upperleft);
   upperleft.y_ += bottommargin_;
   pReprojectionParameters_->pRasterSpatialModelIn_->Transform(upperleft);
   pvectoreditor.AddPoint(upperleft);
   pvectoreditor.CloseFeature();
   pvectoreditor.CloseLayer();
   pvectoreditor.CloseVector();
}

/**
 * Crea un vector que contiene los pixeles que formaran parte del margen derecho
 * @param[in] Pixels ancho del margen
 * @param[in] Lines alto del margen
 */
void Margin::CreateRightMargin(double Pixels, double Lines) {
   if (rightmargin_ <= 0)
      return;
   VectorEditor pvectoreditor;
   pvectoreditor.OpenVector(GetTmpFilename(RightMarginLayerName, ".shp"));
   pvectoreditor.OpenLayer(RightMarginLayerName, pOutputWorld_->GetSpatialReference(), Vector::Polygon);
   pvectoreditor.OpenFeature(0);
   // Esquina superior izquierda
   Coordinates upperleft = GetPoint(Pixels, 0);
   if (topmargin_ > 0) {
      pReprojectionParameters_->pRasterSpatialModelIn_->InverseTransform(upperleft);
      upperleft.y_ -= topmargin_;
      pReprojectionParameters_->pRasterSpatialModelIn_->Transform(upperleft);
   }
   pvectoreditor.AddPoint(upperleft);
   // Esquina inferior derecha
   Coordinates lowerleft = GetPoint(Pixels, Lines);
   if (bottommargin_ > 0) {
      pReprojectionParameters_->pRasterSpatialModelIn_->InverseTransform(lowerleft);
      lowerleft.y_ += bottommargin_;
      pReprojectionParameters_->pRasterSpatialModelIn_->Transform(lowerleft);
   }
   pvectoreditor.AddPoint(lowerleft);
   // Esquina inferior derecha
   pReprojectionParameters_->pRasterSpatialModelIn_->InverseTransform(lowerleft);
   lowerleft.x_ += rightmargin_;
   pReprojectionParameters_->pRasterSpatialModelIn_->Transform(lowerleft);
   pvectoreditor.AddPoint(lowerleft);
   // Esquina superior derecha
   pReprojectionParameters_->pRasterSpatialModelIn_->InverseTransform(upperleft);
   upperleft.x_ += rightmargin_;
   pReprojectionParameters_->pRasterSpatialModelIn_->Transform(upperleft);
   pvectoreditor.AddPoint(upperleft);
   pvectoreditor.CloseFeature();
   pvectoreditor.CloseLayer();
   pvectoreditor.CloseVector();
}

/**
 * Modifica el mundo de salida para contemplar los margenes que fueron configurados
 * @param[in] UpperLeft coordenada del punto superior izquierdo del mundo inicial
 * @param[in] UpperRight coordenada del punto inferior derecho del mundo inicial
 */
void Margin::UpdateOutputWorld(Coordinates UpperLeft, Coordinates LowerRight) {
   pReprojectionParameters_->pRasterSpatialModelIn_->InverseTransform(UpperLeft);
   pReprojectionParameters_->pRasterSpatialModelIn_->InverseTransform(LowerRight);
   UpperLeft.x_ = UpperLeft.x_ - leftmargin_;
   UpperLeft.y_ = UpperLeft.y_ - topmargin_;
   LowerRight.x_ = LowerRight.x_ + rightmargin_;
   LowerRight.y_ = LowerRight.y_ + bottommargin_;
   pReprojectionParameters_->pRasterSpatialModelIn_->Transform(UpperLeft);
   pReprojectionParameters_->pRasterSpatialModelIn_->Transform(LowerRight);
   Subset newsubset;
   newsubset.ul_.x_ = UpperLeft.x_;
   newsubset.ul_.y_ = UpperLeft.y_;
   newsubset.lr_.x_ = LowerRight.x_;
   newsubset.lr_.y_ = LowerRight.y_;
   pOutputWorld_->SetWorld(newsubset);
   pOutputWorld_->SetWindow(newsubset);
   pPal_->AddAttribute<World*>(ProcessAdaptLayer::OutputWorldKeyAttr, pOutputWorld_);
}

/**
 * Retorna las coordenadas de un punto especifico en el raster
 * @param[in] PositionX posicion en el eje x
 * @param[in] PositionY posicion en el eje y
 * @return coordenadas al punto x,y especificado
 */
Coordinates Margin::GetPoint(double PositionX, double PositionY) {
   double wx = 0, wy = 0;
   pOutputWorld_->V2WTransform(PositionX, PositionY, wx, wy);
   Coordinates point(wx, wy);
   return point;
}

/** Borra los archivos temporales creados por la clase */
void Margin::RemoveTmpFiles() {
   std::string tmpfile[] = {TitleLayerName, SubtitleLayerName, TopMarginLayerName,
                             BottomMarginLayerName, LeftMarginLayerName, RightMarginLayerName};

   std::string extension[] = {".dbf", ".prj", ".shp", ".shx"};

   for (int i = 0; i  < 6; ++i) {
      for (int j = 0; j  < 4; ++j) {
         wxRemoveFile(wxT(GetTmpFilename(tmpfile[i], extension[j])));
      }
   }
}
} /** namespace suri */
