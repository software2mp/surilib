/*! \file Margin.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MARGIN_H_
#define MARGIN_H_

// Includes Suri
#include "suri/ProcessAtributeProvider.h"
#include "ReprojectionProcess.h"

namespace suri {
/**
 * Clase encargada de generar margenes (superior, inferior, izquierdo, derecho) para un raster.
 * Para generar los margenes la clase crea un vector que contiene los pixeles que se encuentran en
 * el visualizador y agranda el subset del mundo de salida de acuerdo al tamanio de los margenes.
 * Los pixeles que se encuentran fuera del vector corresponden al margen, con lo cual se aplicara
 * una mascara utilizando color blanco (ndvalue = 255). El tamanio de los margenes debe
 * especificarse en centimetros ya que la clase se encarga de realizar la conversion a pixeles.
 */
class Margin : public suri::ProcessAtributeProvider {
public:
   /** Constructor */
   Margin(ReprojectionProcess::ReprojectionParameters *pReprojectionParameters,
          std::string Title = "", std::string Subtitle = "");
   /** Destructor */
   virtual ~Margin();
   /** Crea un margen en el raster con el tamanio especificado por parametro */
   void SetMargins(double TopMargin, double BottomMargin, double LeftMargin, double RightMargin,
                   double Pixels, double Lines);
   /** Borra los archivos temporales creados por la clase */
   void RemoveTmpFiles();
   /** Retorna la ruta a un archivo con el nombre y la extension especificados en el directorio */
   std::string GetTmpFilename(std::string LayerName, std::string Ext);
   static const std::string TitleFontStyle;
   static const std::string SubtitleFontStyle;
   static const std::string MarginStyle;
   static const std::string TitleLayerName;
   static const std::string SubtitleLayerName;
   static const std::string TopMarginLayerName;
   static const std::string BottomMarginLayerName;
   static const std::string LeftMarginLayerName;
   static const std::string RightMarginLayerName;

private:
   /** Retorna las coordenadas de un punto especifico en el raster */
   Coordinates GetPoint(double PositionX, double PositionY);
   /** Modifica el mundo de salida para contemplar los margenes que fueron configurados */
   void UpdateOutputWorld(Coordinates UpperLeft, Coordinates LowerRight);
   /** Crea un vector que contiene los pixeles que formaran parte del margen superior */
   void CreateTopMargin(double Pixels, double Lines);
   /** Crea un vector que contiene los pixeles que formaran parte del margen inferior */
   void CreateBottomMargin(double Pixels, double Lines);
   /** Crea un vector que contiene los pixeles que formaran parte del margen izquierdo */
   void CreateLeftMargin(double Lines);
   /** Crea un vector que contiene los pixeles que formaran parte del margen derecho */
   void CreateRightMargin(double Pixels, double Lines);
   /** Crea una capa de punto con el texto especificado */
   void CreateLabel(double PositionX, double PositionY, std::string LayerName, std::string Text);
   /** Parametros de reproyeccion */
   ReprojectionProcess::ReprojectionParameters *pReprojectionParameters_;
   /** Margenes */
   double topmargin_, bottommargin_, leftmargin_, rightmargin_;
   /** Labels */
   std::string title_, subtitle_;
   /** Mundo de salida */
   World* pOutputWorld_;
};
} /** namespace suri */

#endif /* MARGIN_H_ */
