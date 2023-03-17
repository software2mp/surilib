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
