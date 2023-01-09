/** \file TextFormatConverter.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TEXTFORMATCONVERTER_H_
#define TEXTFORMATCONVERTER_H_

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
class StatisticsXmlToHtmlConverter : public TextFormatConverterInterface {
public:
   /** Ctor */
   StatisticsXmlToHtmlConverter(std::map<std::string, std::string> Options);
   /** Dtor */
   virtual ~StatisticsXmlToHtmlConverter();
   /** Transforma el contenido del texto. */
   bool Transform(const std::string &InputText, std::string &OutputText);

private:
   /** Asigna el nombre de las bandas */
   void GetBandNames(const std::string &SelectedBands, std::vector<std::string> &BandNames);
   /** Asigna el id de las bandas */
   void GetBandIds(const std::string &SelectedBandIds, std::vector<int> &BandIds);
   /** Retorna el valor de una opcion */
   bool ReadOption(const std::string &Option, std::string &OptionValue);
   /** Retorna true si existe la opcion y tiene valor true */
   bool GetFlagOptionValue(const std::string &Option);

   /** Agrega descripcion del raster de entrada a html */
   void AddRasterDescription(long Count, std::string &Html);
   /** Agrega parametros basicos de estadisticas a html */
   void AddBasicStatisticsParameters(const Statistics& statistics,
        const std::vector<std::string> &TableHeader, std::string &Html);
   /**
    * Agrega parametros basicos de estadisticas a html
    * @param[in] pStatistics estadisticas a imprimir
    * @param[in] BandNames Nombre de bandas en imagen de entrada
    * @param[out] Html donde se agrega informacion de archivo
    */
   void AddBasicStatisticsParameters(suri::raster::data::StatisticsBase* pStatistics,
                                     const std::vector<std::string> &BandNames,
                                     std::string &Html);

   /**
    * Agrega los autovalores al html.
    * @param[in] pStatistics estadisticas a imprimir
    * @param[in] BandNames Nombre de bandas en imagen de entrada
    * @param[out] Html donde se agrega informacion de archivo
    */
   void AddEigenValues(suri::raster::data::StatisticsBase* pStatistics,
                       const std::vector<std::string>& BandNames, std::string& Html);

   /** Agrega tabla con la moda a html */
   void AddMode(const Statistics& statistics,
        const std::vector<std::string> &TableHeader, std::string &Html);
   /** Agrega histograma a html */
   void AddHistogram(const Statistics& statistics,
          const std::vector<std::string> &BandNames, std::string &Html);
   /** Agrega histograma a html */
   void AddHistogram(suri::raster::data::HistogramBase* pHistogram,
          const std::vector<std::string> &BandNames, std::string &Html);
   /** Retorna el nombre de un bin en html de salida */
   std::string GetBinName(double CurrentBin);
   /** Actualiza binNames_ opcion BINNAMES_OP */
   bool UpdateBinNames();
   /** Retorna los autovectores en el orden correspondiente de los autovalores */
   std::vector<std::vector<double> > GetEigenVectors();
   /** Retorna una matriz con estadisticas de las bandas seleccionadas */
   std::vector<std::vector<double> > GetMatrix(std::vector<std::vector<double> > Matrix,
                                               size_t SelectedBandsCount, int BandCount,
                                               std::string Components);

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
   static const std::string SELECTEDBANDIDS_OP;
   static const std::string BINNAMES_OP;
   static const std::string HISTOGRAM_BIN_COLUMN_NAME_OP;
   static const std::string SHOWMODE_OP;
   static const std::string BINS_0_255_OP;
   static const std::string STATS_FLAGS;
   static const std::string COMPONENTS;
};

}  // namespace

#endif /* TEXTFORMATCONVERTER_H_ */
