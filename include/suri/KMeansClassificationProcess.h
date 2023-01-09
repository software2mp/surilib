/*! \file KMeansClassificationProcess.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef __KMEANS_CLASSIFICATION_PROCESS_H_
#define __KMEANS_CLASSIFICATION_PROCESS_H_

// Includes standard
#include <string>
#include <vector>
// Includes Suri
#include "suri/FileExporterProcess.h"
#include "ClassInformation.h"

class wxXmlNode;
class GDALDataset;

/** namespace suri */
namespace suri {
// foward
class ViewcontextManagerInterface;
class DatasourceManagerInterface;
class RasterElement;
class LayerList;
class Element;
class DataViewManager;

/**
 * Proceso que realiza una clasificacion no supervisada utilizando kmeans como algoritmo
 * Crea un elemento nuevo con el resultado de la clasificacion, que tiene una
 * tabla de color con los colores de las clases.
 */
class KMeansClassificationProcess : public FileExporterProcess {
   /** Ctor. de copia */
   KMeansClassificationProcess(const KMeansClassificationProcess &);

public:
   /** Ctor. */
   explicit KMeansClassificationProcess(Element *pInputElement,
                                        const Subset &WindowSubset,
                                        DataViewManager* pDataViewManager);
   /** Dtor. */
   virtual ~KMeansClassificationProcess();
   /** Corre el proceso y genera la salida usando los metodos de configuracion */
   virtual bool RunProcess();
   /** Nombre del proceso **/
   static const std::string kProcessName;
   static const std::string ThresholdAttrKey;
   static const std::string IterationsAttrKey;
   static const std::string ClassCountAttrKey;
   static const std::string MeansMethodAttrKey;
   static const int RandomMeans = 0;
   static const int PartialRandomMeans = 1;
   static const int MaximumDispersionMeans = 2;
   static const int QuantilMeans = 3;
   static const int ModeMeans = 4;
     /** 
    * Metodo que a partir de un xml que contiene los parametros del proceso configura el mismo.
    * @param[in] XmStr string en formato xml que contiene los parametros necesarios para la 
    * correcta ejecucion del proceso
    * @return true en caso de poder configurar correctamente el proceso en funcion de los parametros
    * @return false en caso contrario
   **/
   bool ConfigureProcessFromXmlString(const std::string& XmlStr) {
      return false;
   }

   
protected:
   /** Metodo auxiliar que configura los metadatos de la imagen que genera 
     * el proceso de clasificacion
    **/
   void ConfigureMetadata(GDALDataset* pDataset, int XOffset, int XBlockSize, int InitialLine,
                           int YBlockSize, const std::string& SpatialReference, GDALDataset* pDestDataset);
   /** A traves del algoritmo de kmeans clasifica los pixeles del bloque se pasa por parametro **/
   void ClassifyBlock(std::vector<float*>& BlockData, int BlockSize, int BandCount,
               double NoDataValue, bool NoDataValueEnable,
               const std::vector<std::vector<double> >& InitialMeans,
               std::vector<std::vector<double> >& StepMeans, std::vector<int>& PixelCount,
               float* pOutputData);

   /** Obtiene las estadisticas de la imagen **/
   bool GetStatistics(std::vector<int> BandIndex, RasterElement* pRaster, 
               std::vector<double>& Min, std::vector<double>& Max,
               std::vector< std::vector<int> > &Histogram);
   /** Obtiene las medias iniciales de acuerdo al metodo de calculo pasado por parametro **/
   std::vector< std::vector<double> > GetInitialMeans(int Method, int Classes,
                                    int BandCount,
                                    const std::vector<double>& Min, 
                                    const std::vector<double>& Max,
                                    const std::vector< std::vector<int> >& Histogram,
                                    const Subset& SpatialSelection, 
                                    GDALDataset* pSource);
   /**
    * Se toma como media inicial el valor dn de un pixel en la posicion (pixel,linea) de
    * aleatoria
   **/
   std::vector< std::vector<double> >GetRandomMeans(int Classes, 
                                          int BandCount, const Subset& SpatialSelection,
                                          GDALDataset* pSource);
   /**
    * En funcion del minimo y maximo de la imagen (por banda) se generan clases iniciales 
    * utilizando maxima dispercion por la diagonal
   **/
   std::vector<std::vector<double> > GetMaxDispersionMeans(int Classes,
                                          int BandCount,
                                          const std::vector<double>& Min,
                                          const std::vector<double>& Max);
   /**
    *  En funcion del minimo y maximo de la imagen (por banda) se generan clases iniciales de forma
    *  aleatoria.
   **/
   std::vector< std::vector<double> > GetPartialRandomMeans(int Classes, 
                                          int BandCount,
                                          const std::vector<double>& Min,
                                          const std::vector<double>& Max);
   /**
    * En funcion del histograma de la imagen asigna medias utilizando los extremos inferiores 
    * de los bins que tengan la mayor cantidad de pixeles (mayor frecuencia en la imagen)
   **/
   std::vector< std::vector<double> > GetModeMeans(int Classes, int BandCount,
                                                   const std::vector<double>& Min,
                                                   const std::vector<double>& Max,
                                                   const std::vector< std::vector<int> >& Histogram);
   /** En funcion del histograma asociado a la imagen asigna medias en intervalos uniformes
    *  en cantidad de pixeles 
    */
   std::vector< std::vector<double> > GetQuantilMeans(int Classes, 
                                          int BandCount,
                                          const std::vector<double>& Min,
                                          const std::vector<double>& Max,
                                          const std::vector< std::vector<int> >& Histogram);
                                          /**
    * Calcula que porcentaje se modificaron las medias. Para cada media calcula la
    * diferencia entre la nueva posicion y la anterior y la divide por el modulo
    * de la nueva media. Se queda con la distancia mas grande.
    */
   double CalculateChange(const std::vector<std::vector<double> > &InitialMeans,
                          const std::vector<std::vector<double> > &NextMean);
   /** Actuaiza los centroides de las clases */
   void UpdateMeans(int Classes, int BandCount,
                  const std::vector< std::vector<double > >& ClassCentreData, 
                  const std::vector<int>& Histogram,
                  std::vector<std::vector<double> >& Means);
   /** Configura el elemento creado con la herramienta */
   virtual bool ConfigureOutput();
   /** 
    * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
    * agreguen sus atributos especificos
    * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
    * @return vector vacio en caso de que no tenga parametros adicionales
   **/
   virtual std::vector<wxXmlNode*> DoGetXmlParameters() const;
   /** flag que determina si se encuentra iterando el proceso **/
   bool running_;
   double threshold_;
   int iterations_, classcount_;
};
}  // suri

#endif /* __KMEANS_CLASSIFICATION_PROCESS_H_ */
