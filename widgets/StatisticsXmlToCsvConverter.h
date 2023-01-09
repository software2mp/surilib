/** \file TextFormatConverter.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CSVTEXTFORMATCONVERTER_H_
#define CSVTEXTFORMATCONVERTER_H_

// Includes standard
#include <string>
#include <vector>
#include <map>

// Includes suri
#include "TextFormatConverterInterface.h"
#include "SRDHistogram.h"
#include "SRDStatistics.h"

namespace suri {

// Fordwards
struct Statistics;

/**
 * Tranforma las estadisticas desde xml a html.
 */
class StatisticsXmlToCsvConverter : public TextFormatConverterInterface {
public:
   /** Ctor */
   StatisticsXmlToCsvConverter(std::map<std::string, std::string> Options);
   /** Dtor */
   virtual ~StatisticsXmlToCsvConverter();
   /** Transforma el contenido del texto. */
   bool Transform(const std::string &InputText, std::string &OutputText);

private:
   /** Asigna nombres default a las badas */
   void GetBandNames(const std::string &SelectedBands,
                     std::vector<std::string> &BandNames);
   /** Retorna el valor de una opcion */
   bool ReadOption(const std::string &Option, std::string &OptionValue);
   /** Retorna true si existe la opcion y tiene valor true */
   bool GetFlagOptionValue(const std::string &Option);

   /** Agrega descripcion del raster de entrada a html */
   void AddRasterDescription(long Count, int BandCount, const std::string& DataType,
                             std::string &Html);

   /** Agrega parametros basicos de estadisticas a html */
   void AddBasicStatisticsParameters(std::vector<double>& Mean,
                                     std::vector<double>& Variance,
                                     std::vector<double>& Min, std::vector<double>& Max,
        const std::vector<std::string> &TableHeader, std::string &Html);


   /**
    * Agrega los autovalores al html.
    */
   void AddEigenValues(suri::raster::data::StatisticsBase* pStatistics,
                       const std::vector<std::string>& BandNames, std::string& Csv);

   /** Agrega tabla con la moda a html */
   void AddMode(const Statistics& statistics,
        const std::vector<std::string> &TableHeader, std::string &Html);
   void AddMode(suri::raster::data::HistogramBase* pHistogram,
                const std::vector<std::string> &BandNames, std::string &Html);
   /** Agrega histograma a html */
   void AddHistogram(const Statistics& statistics,
          const std::vector<std::string> &BandNames, std::string &Html);
   /**
    * Agrega histograma a html
    * @param[in] Statistics estadisticas a imprimir
    * @param[out] Html donde se agrega informacion de archivo
    */
   void AddHistogram(suri::raster::data::HistogramBase* pHistogram,
                     const std::vector<std::string> &BandNames, std::string &Csv);
   /** Retorna el nombre de un bin en html de salida */
   std::string GetBinName(double CurrentBin);
   /** Actualiza binNames_ opcion BINNAMES_OP */
   bool UpdateBinNames();
   /** Metodo auxiliar que convierte las nuevas estadisticas
    *  a texto  csv
    */
   bool TransformNewStats(suri::raster::data::StatisticsBase* pStatistics,
                          suri::raster::data::HistogramBase* pHistogram,
                          const std::vector<std::string>& BandNames,
                          std::string& OutputText);
   std::map<std::string, std::string> options_;

   /** Contiene el nombre de los bines. Se configuran con las opciones */
   std::map<int, std::string> binNames_;

   /** Opciones que soporta el generador de html */
   static const std::string OUTPUTUNITS_OP;
   static const std::string PIXELAREA_OP;
   static const std::string URL_OP;
   static const std::string EXTENT_OP;
   static const std::string DATATYPE_OP;
   static const std::string SELECTEDBANDS_OP;
   static const std::string BINNAMES_OP;
   static const std::string HISTOGRAM_BIN_COLUMN_NAME_OP;
   static const std::string SHOWMODE_OP;
   static const std::string BINS_0_255_OP;
   static const std::string STATS_FLAGS;
};

}  // namespace

#endif /* CSVTEXTFORMATCONVERTER_H_ */
