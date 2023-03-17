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
#include <vector>
#include <string>
#include <map>
#include <utility>

// Includes suri
#include "StatisticsXmlToCsvConverter.h"
#include "suri/StatisticsFunctions.h"
#include "logmacros.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/messages.h"
#include "suri/DataTypes.h"
#include "CsvFunctions.h"

#if defined(__LINUX__)
#include "suri/XmlElement.h"
#endif

// Defines
#define STATISTICSXML_FORMATNAME "xml:statistics"
#define STATISTICSCSV_FORMATNAME "csv:statistics"
#define PIXELAREA_OPTIONVALUE "pixel"
#define DATATYPE_OPTIONVALUE "datatype"
#define BANDCOUNT_OPTIONVALUE "bandcount"
#define BASICPARAMETERCOUNT 4
#define HISTOGRAMROWCOUNT 256
#define AVOID_ROUNDERROR_OFFSETPERCENTAGE 0.1
#define DEFAULT_BINNAME_PRECISION 6
#define TRUE_FLAG_VALUE "true"
#define BASIC_STATS_PRECISION    6

namespace suri {
/**
 * Se agregaron opciones que permiten transformar los valores obtenidos
 * del xml con estadisticas y mostrar el resultado como areas.
 * Causa: el histograma solo permite guardar cantidad de pixes.
 * Refactor pendiente: TextFormatConverter no deberia tener logica
 * para transformar entre unidades. Ver TCK #841
 */
const std::string StatisticsXmlToCsvConverter::OUTPUTUNITS_OP = "unit";
const std::string StatisticsXmlToCsvConverter::PIXELAREA_OP = "pixel_area";

/**
 * Se agregaron las opciones: url, tipo de dato, bandas selecciondas y
 * parte de la imagen leida.
 * Causa: falta de tiempo
 * Refactor pendiente: Agregar esta informacion al xml de entrada.
 * Ver TCK #841
 */
const std::string StatisticsXmlToCsvConverter::URL_OP = "url";
const std::string StatisticsXmlToCsvConverter::EXTENT_OP = "extent";
const std::string StatisticsXmlToCsvConverter::DATATYPE_OP = "datatype";
const std::string StatisticsXmlToCsvConverter::SELECTEDBANDS_OP = "selectedbands";
const std::string StatisticsXmlToCsvConverter::BINNAMES_OP = "bin_names";
const std::string StatisticsXmlToCsvConverter::SHOWMODE_OP = "show_mode";
const std::string StatisticsXmlToCsvConverter::BINS_0_255_OP = "bins_0_255";
const std::string StatisticsXmlToCsvConverter::HISTOGRAM_BIN_COLUMN_NAME_OP =
                                                     "histogram_bin_column_name";
const std::string StatisticsXmlToCsvConverter::STATS_FLAGS = "stats_flags";

/** Ctor */
StatisticsXmlToCsvConverter::StatisticsXmlToCsvConverter(
                           std::map<std::string, std::string> Options) {
   options_ = Options;
}

/** Dtor */
StatisticsXmlToCsvConverter::~StatisticsXmlToCsvConverter() {
}

/** Metodo auxiliar que convierte las nuevas estadisticas
    *  a texto  csv
    */
bool StatisticsXmlToCsvConverter::TransformNewStats(
      suri::raster::data::StatisticsBase* pStatistics,
      suri::raster::data::HistogramBase* pHistogram,
      const std::vector<std::string>& BandNames,
      std::string& OutputText) {
   std::string csv = "";

   // Muestro informacion general de la imagen
   AddRasterDescription(pStatistics->pPointCount_[0], pStatistics->GetBandCount(),
                        pStatistics->GetDataName(), csv);

   std::string statheader = "[";
   statheader += _(html_STATISTICS);
   statheader += "]\n";
   AddCsvHeader(csv, statheader);

   // Flags
   std::string strflags;
   ReadOption(STATS_FLAGS, strflags);
   int flags = StringToNumber<int>(strflags);

   // Muestro parametros basicos de las estadisticas
   if (flags & Statistics::Mean) {
      std::vector<double> mean(pStatistics->GetBandCount());
      for (int i = 0; i < pStatistics->GetBandCount(); ++i)
         mean[i] = pStatistics->GetMean(i);

      std::vector<double> variance(pStatistics->GetBandCount());
      for (int i = 0; i < pStatistics->GetBandCount(); ++i)
         variance[i] = pStatistics->pAccumVariance_[i];

      std::vector<double> min(pStatistics->GetBandCount());
      for (int i = 0; i < pStatistics->GetBandCount(); ++i)
         min[i] = pStatistics->GetMin(i);

      std::vector<double> max(pStatistics->GetBandCount());
      for (int i = 0; i < pStatistics->GetBandCount(); ++i)
         max[i] = pStatistics->GetMax(i);

      AddBasicStatisticsParameters(mean, variance, min, max, BandNames, csv);
   }

   // Muestro parametros basicos de las estadisticas
   if (pHistogram && GetFlagOptionValue(SHOWMODE_OP))
      AddMode(pHistogram, BandNames, csv);

   // Inicializo header para estadisticas entre bandas
   std::vector<std::string> tableheader;

   // Agrego estadisticas entre bandas a html
   if (flags & Statistics::CorrelationMatrix
               || flags & Statistics::CovarianceMatrix) {

      AddEigenValues(pStatistics, BandNames, csv);

      AddCsvNumericTable(csv, _("correlacion"), tableheader, BandNames,
                         pStatistics->GetCorrelationMatrix(), 1, BASIC_STATS_PRECISION);

      AddCsvNumericTable(csv, _("covarianza"), tableheader, BandNames,
                         pStatistics->GetCovarianceMatrix(), 1, BASIC_STATS_PRECISION);

      int eigtype = EIG_NONE;
      std::string eigtitle;
      if (pStatistics->GetEig() == EIG_NONE || pStatistics->GetEig() == EIG_COVARIANCE) {
         eigtitle = "autovectores_cov";
         eigtype = EIG_COVARIANCE;
      } else {
         eigtitle = "autovectores_corr";
         eigtype = EIG_CORRELATION;
      }

      AddCsvNumericTable(csv, _(eigtitle.c_str()), tableheader, BandNames,
                         pStatistics->GetEigenVectors(eigtype), 1, 6);
   }

   // Agrego histograma a estadisticas
   if (pHistogram != NULL)
      AddHistogram(pHistogram, BandNames, csv);

   // TODO: (Federico - 04/12/2013): Generalizar la forma de generar los reportes para evitar
   // parsear el string de esta forma

   Replace(csv, "<table border=\"0\" valign=\"top\"><tr valign=\"top\">", "\n");
   Replace(csv, "<h1><font color=\"#041ACC\">", "");
   Replace(csv, "</font></h1>", "\n");

   OutputText = csv;
#if defined(__LINUX__)
   // Correccion a codificacion en Linux debido a url
   OutputText = wxString(wxConvUTF8.cMB2WC(csv.c_str()),
                         wxCSConv(XmlElement::xmlEncoding_.c_str()));
   if (OutputText.empty())
      OutputText = csv;
#endif
   return true;
}

/**
 * Transforma el contenido del texto.
 * @param[in] InputText texto que deseo convertir
 * @param[out] OutputText texto en nuevo formato.
 * @return true en caso de tener exito
 * @return false si ocurre un error.
 */
bool StatisticsXmlToCsvConverter::Transform(const std::string &InputText,
                                                   std::string &OutputText) {
   // Lee opciones con datos de la imagen
   std::string selectedbands, pixelareaasstring = "";
   ReadOption(SELECTEDBANDS_OP, selectedbands);

   // Obtengo el nombre de las bandas
   std::vector<std::string> bandnames;
   GetBandNames(selectedbands, bandnames);

   suri::raster::data::HistogramBase* phistogram = NULL;
   suri::raster::data::StatisticsBase* pstats = NULL;

   // Obtengo las estadisticas calculadas
   if (GetStatsFromXmlString(InputText, pstats, phistogram)) {
      return TransformNewStats(pstats, phistogram, bandnames, OutputText);
   }
#ifdef __OLD_STATS_TRANSFORM__
   Statistics statistics;
   if (!GetStatsFromXmlString(InputText, statistics)) return false;

   // Obtengo la cantidad de bandas
   size_t bandcount = 0;
   if (statistics.mean_.size() > 0)
      bandcount = statistics.mean_.size();
   else if (statistics.correlationMatrix_.size() > 0)
      bandcount = statistics.correlationMatrix_.size();
   else if (statistics.histogram_.size() > 0)
      bandcount = statistics.histogram_.size();
   if (bandcount == 0 || bandcount != bandnames.size())
      return false;

   // Genero html a partir de estadisticas
   std::string csv = "";

   // Muestro informacion general de la imagen
   AddRasterDescription(statistics.count_, csv);

   AddCsvHeader(csv, _(html_STATISTICS));
   AddCsvHeader(csv, "\n");

   // Muestro parametros basicos de las estadisticas
   if (statistics.mean_.size() > 0)
      AddBasicStatisticsParameters(statistics.mean_, statistics.variance_,
                                   statistics.min_, statistics.max_, bandnames, csv);

   // Muestro parametros basicos de las estadisticas
   if (GetFlagOptionValue(SHOWMODE_OP))
      AddMode(statistics, bandnames, csv);

   // Inicializo header para estadisticas entre bandas
   std::vector<std::string> tableheader;
   tableheader.push_back("");
   for (size_t i = 0; i < bandcount; i++)
      tableheader.push_back(bandnames[i]);

   // Agrego estadisticas entre bandas a html
   if (statistics.correlationMatrix_.size() > 0)
      AddCsvNumericTable(csv, _(html_CORRELATIONMATRIX), tableheader, bandnames,
                         statistics.correlationMatrix_, 1, BASIC_STATS_PRECISION);
   if (statistics.covarianceMatrix_.size() > 0)
      AddCsvNumericTable(csv, _(html_COVARIANCEMATRIX), tableheader, bandnames,
                         statistics.covarianceMatrix_, 1, BASIC_STATS_PRECISION);

   // Agrego histograma a estadisticas
   if (statistics.histogram_.size() > 0)
      AddHistogram(statistics, bandnames, csv);

   // TODO: (Federico - 04/12/2013): Generalizar la forma de generar los reportes para evitar
   // parsear el string de esta forma

   Replace(csv, "<table border=\"0\" valign=\"top\"><tr valign=\"top\">", "\n");
   Replace(csv, "<h1><font color=\"#041ACC\">", "");
   Replace(csv, "</font></h1>", "\n");

   OutputText = csv;
#if defined(__LINUX__)
   // Correccion a codificacion en Linux debido a url
   OutputText = wxString(wxConvUTF8.cMB2WC(csv.c_str()),
                         wxCSConv(XmlElement::xmlEncoding_.c_str()));
   if (OutputText.empty()) OutputText = csv;
#endif
#endif
   return true;
}

/**
 * Agrega informacion del raster que se uso para calcular estadisticas a html
 * @param[in] Count cantidad de estadisticas a imprimir
 * @param[out] Html donde se agrega informacion de archivo
 */
void StatisticsXmlToCsvConverter::AddRasterDescription(long Count, int BandCount,
                                                       const std::string& DataType,
                                                       std::string &Csv) {
   std::string url, extentasstring, pixelareaasstring, selectedbands = "";
   std::string units = PIXELAREA_OPTIONVALUE;
   double pixelarea = 1;

   ReadOption(URL_OP, url);
   ReadOption(EXTENT_OP, extentasstring);
   if (ReadOption(OUTPUTUNITS_OP, units) && units != PIXELAREA_OPTIONVALUE) {
      if (ReadOption(PIXELAREA_OP, pixelareaasstring))
         pixelarea = StringToNumber<double>(pixelareaasstring);
   }

   //Csv += "\n";
   std::string headerinfo = "[";
   headerinfo += _(html_IMAGEINFO);
   headerinfo += "]";
   AddCsvHeader(Csv, headerinfo);
   Csv = Csv + "\n" + _(html_URL) + wxT(":") + url + "\n";
   Csv += DATATYPE_OPTIONVALUE "," + DataType + "\n";
   Csv += BANDCOUNT_OPTIONVALUE "," + NumberToString<int>(BandCount) + "\n";
   std::vector<std::string> extent = split<std::string>(extentasstring);
   Csv = Csv + _(html_EXTENT) + wxT(":") + "\"[" + extent[0] + ", " + extent[1] + "] - ";
   Csv = Csv + "[" + extent[2] + ", " + extent[3] + "]\"\n";
   Csv = Csv + _(html_UNIT) + wxT(":") + _(units.c_str()) + "\n";
   if (units != PIXELAREA_OPTIONVALUE) {
      Csv = Csv + _(html_IMAGEAREA) + wxT(":") + DoubleToString(pixelarea * Count) + "\n";
      Csv = Csv + _(html_PIXELAREA) + wxT(":") + DoubleToString(pixelarea) + "\n";
   } else {
//      Csv = Csv + _(html_PIXELCOUNT) + wxT(":,") + LongToString(Count) + "\n\n";
      Csv += "[";
      Csv += _(PIXELAREA_OPTIONVALUE);
      Csv += "]\n";
      Csv +=  LongToString(Count);
      Csv += "\n";
   }

}

/**
 * Agrega parametros basicos de estadisticas a html
 * @param[in] Mean array con las medias a imprimir
 * @param[in] Variance array con las varianzas a imprimir
 * @param[in] Min array con los minimos a imprimir
 * @param[in] Max array con los maximos a imprimir
 * @param[in] BandNames Nombre de bandas en imagen de entrada
 * @param[out] Html donde se agrega informacion de archivo
 */
void StatisticsXmlToCsvConverter::AddBasicStatisticsParameters(
                                    std::vector<double>& Mean,
                                    std::vector<double>& Variance,
                                    std::vector<double>& Min,
                                    std::vector<double>& Max,
                                    const std::vector<std::string> &BandNames,
                                    std::string &Html) {
   // Obtengo header de tabla
   std::string meancsv = "[media]\n";
   for (size_t i = 0; i < Mean.size(); i++) {
      meancsv += BandNames[i];
      meancsv += ",";
      meancsv += NumberToString<double>(Mean[i]);
      meancsv += "\n";
   }   
   Html.append(meancsv);

   std::string mincsv = "[minimo]\n";
   for (size_t i = 0; i < Min.size(); i++) {
      mincsv += BandNames[i];
      mincsv += ",";
      mincsv += NumberToString<double>(Min[i]);
      mincsv += "\n";
   }  
   Html.append(mincsv);
   std::string maxcsv = "[maximo]\n";
   for (size_t i = 0; i < Max.size(); i++) {
      maxcsv += BandNames[i];
      maxcsv += ",";
      maxcsv += NumberToString<double>(Max[i]);
      maxcsv += "\n";
   }   
   Html.append(maxcsv);
   std::string variancecsv = "[varianza]\n";
   for (size_t i = 0; i < Variance.size(); i++) {
      variancecsv += BandNames[i];
      variancecsv += ",";
      variancecsv += NumberToString<double>(Variance[i]);
      variancecsv += "\n";
   }
   Html.append(variancecsv);
}

/**
 * Agrega los autovalores al html.
 * @param[in] pStatistics estadisticas a imprimir
 * @param[in] BandNames Nombre de bandas en imagen de entrada
 * @param[out] Html donde se agrega informacion de archivo
 */
void StatisticsXmlToCsvConverter::AddEigenValues(
      suri::raster::data::StatisticsBase* pStatistics,
      const std::vector<std::string>& BandNames, std::string& Csv) {

   std::string eigtitle;
   if (pStatistics->GetEig() == EIG_NONE || pStatistics->GetEig() == EIG_COVARIANCE) {
      eigtitle = "autovalores_cov";
   } else {
      eigtitle = "autovalores_corr";
   }

   std::vector<double> eigvals = pStatistics->GetEigenValues(pStatistics->GetEig());

   std::string meancsv = "[" + eigtitle + "]\n";
   for (size_t i = 0; i < eigvals.size(); i++) {
      meancsv += BandNames[i];
      meancsv += ",";
      meancsv += NumberToString<double>(eigvals[i]);
      meancsv += "\n";
   }
   Csv.append(meancsv);
}

/**
 * Muestra tabla con la moda.
 * @param[in] Statistics estadisticas a imprimir
 * @param[in] BandNames Nombre de bandas en imagen de entrada
 * @param[out] Html donde se agrega informacion de archivo
 */
void StatisticsXmlToCsvConverter::AddMode(
                                    const Statistics& statistics,
                                    const std::vector<std::string> &BandNames,
                                    std::string &Html) {
   // TODO(Ramiro - TCK #2671) Integrar la moda a la tabla generada por
   // AddBasicStatisticsParameters

   // Obtengo header de tabla
   std::vector<std::string> tableheader;
   tableheader.push_back(_(html_PARAMETERS));
   for (size_t i = 0; i < BandNames.size(); i++)
      tableheader.push_back(BandNames[i]);

   double binindex;
   std::vector<std::string> rownames;
   rownames.push_back(_(html_MODE));

   UpdateBinNames();
   std::vector<std::string> modes;
   for (size_t i = 0; i < statistics.histogram_.size(); i++) {
      if (GetMode(statistics.histogram_[i], binindex))
         modes.push_back(GetBinName(binindex));
   }

   std::vector<std::vector<std::string> > stats;
   stats.push_back(modes);

   AddCsvTable(Html, _(html_BASICPARAMETERS), tableheader, rownames, stats);
}

/**
 * Muestra tabla con la moda.
 * @param[in] pHistogram histograma para el calculo de la moda
 * @param[in] BandNames Nombre de bandas en imagen de entrada
 * @param[out] Html donde se agrega informacion de archivo
 */
void StatisticsXmlToCsvConverter::AddMode(
                                    suri::raster::data::HistogramBase* pHistogram,
                                    const std::vector<std::string> &BandNames,
                                    std::string &Html) {
   // TODO(Ramiro - TCK #2671) Integrar la moda a la tabla generada por
   // AddBasicStatisticsParameters

   // Obtengo header de tabla
   std::vector<std::string> tableheader;
   tableheader.push_back(_(html_PARAMETERS));
   for (size_t i = 0; i < BandNames.size(); i++)
      tableheader.push_back(BandNames[i]);

   std::vector<std::string> rownames;
   rownames.push_back(_(html_MODE));

   UpdateBinNames();
   std::vector<std::string> modenames;
   std::vector<double> modes;
   GetMode(pHistogram, modes);

   for (size_t m = 0; m < modes.size(); ++m) {
      modenames.push_back(GetBinName(modes[m]));
   }

   std::vector<std::vector<std::string> > stats;
   stats.push_back(modenames);

   AddCsvTable(Html, _(html_BASICPARAMETERS), tableheader, rownames, stats);
}
/**
 * Agrega histograma a html
 * @param[in] Statistics estadisticas a imprimir
 * @param[out] Html donde se agrega informacion de archivo
 */
void StatisticsXmlToCsvConverter::AddHistogram(const Statistics& statistics,
                                 const std::vector<std::string> &BandNames,
                                 std::string &Csv) {
   std::string datatype, columnname, pixelareaasstring = "";
   ReadOption(DATATYPE_OP, datatype);
   UpdateBinNames();

   // Analiza si la salida se debe mostrar como areas o cantidad de pixels
   double pixelarea = 1;
   int histogramprecision = 0;
   std::string histogramdataname = _(html_COUNT);
   std::string units = PIXELAREA_OPTIONVALUE;
   if (ReadOption(OUTPUTUNITS_OP, units) && units != PIXELAREA_OPTIONVALUE) {
      if (ReadOption(PIXELAREA_OP, pixelareaasstring)) {
         histogramprecision = 3;
         histogramdataname = _(html_AREA);
         pixelarea = StringToNumber<double>(pixelareaasstring);
      }
   }

   // Modifico la cantidad de bis para que sean 256
   bool bins_0_255 = GetFlagOptionValue(BINS_0_255_OP) ||
                                                IsIntegerType(datatype);
   std::vector<Histogram> newhistogram;
   for (size_t i = 0; i < statistics.histogram_.size(); i++) {
      if (bins_0_255)
         newhistogram.push_back(Histogram(-0.5, 255.5, HISTOGRAMROWCOUNT + 1));
      else
         newhistogram.push_back(Histogram(statistics.histogram_[i].GetMinimum(),
                      statistics.histogram_[i].GetMaximum(), HISTOGRAMROWCOUNT));
      Histogram::HistogramType histogramdata;
      statistics.histogram_[i].GetHistogram(histogramdata);
      Histogram::HistogramType::iterator it = histogramdata.begin();
      Histogram::HistogramType::iterator end = histogramdata.end();
      while (it != end) {
         newhistogram[i].IncreaseValue(it->first, it->second);
         ++it;
      }
   }

   AddCsvHeader(Csv, _(html_HISTOGRAM));
   AddCsvHeader(Csv, "\n");

   // Genero header de histograma
   std::vector<std::string> tableheader;
   if (ReadOption(HISTOGRAM_BIN_COLUMN_NAME_OP, columnname))
      tableheader.push_back(columnname);
   else
      tableheader.push_back(_(html_DN));
   tableheader.push_back(histogramdataname);

   // Muestro los histogramas por pantalla
   for (size_t i = 0; i < statistics.histogram_.size(); i++) {
      std::vector<std::string> rownames;
      std::vector<std::vector<double> > histogram;

      // Genero matriz que voy a imprimir a html
      double binsize = newhistogram[i].GetBinSize();
      double currentbin = newhistogram[i].GetMinimum() +
                  binsize * AVOID_ROUNDERROR_OFFSETPERCENTAGE;
      double lastbin = newhistogram[i].GetMaximum();
      while (currentbin < lastbin) {
         rownames.push_back(GetBinName(currentbin));
         std::vector<double> newvect;
         newvect.push_back(newhistogram[i].GetFrequency(currentbin));
         histogram.push_back(newvect);
         currentbin += binsize;
      }

      AddCsvNumericTable(Csv, BandNames[i], tableheader, rownames, histogram,
                         pixelarea, histogramprecision);
   }
}

/**
 * Retorna el nombre que el bin deberia tener en html de salida.
 * Puede ser un nombre asignado por opcion, o en caso de no existir,
 * transformar el identificado del bin a string.
 * @param[in] CurrentBin Valor con el que quiero generar el nombre
 * @return Nombre del bin.
 */
std::string StatisticsXmlToCsvConverter::GetBinName(double CurrentBin) {
   std::string datatype, binname = "";
   int binprecision = DEFAULT_BINNAME_PRECISION;

   ReadOption(DATATYPE_OP, datatype);
   if (GetFlagOptionValue(BINS_0_255_OP) || datatype == "uchar") {
      binprecision = 0;

      int value = SURI_ROUND(int, CurrentBin);
      std::map<int, std::string>::iterator it = binNames_.find(value);
      if (it != binNames_.end())
         return it->second;
   }

   return DoubleToString(CurrentBin, binprecision);
}

/**
 * Actualiza binNames_ con contenido de opcion BINNAMES_OP
 * @return bool que indica si tuvo exito
 */
bool StatisticsXmlToCsvConverter::UpdateBinNames() {
   std::string binnames, datatype;
   ReadOption(DATATYPE_OP, datatype);
   if (!ReadOption(BINNAMES_OP, binnames) || !IsIntegerType(datatype))
      return false;

   std::map<std::string, std::string> binnamesmap =
                                    StringToMap(binnames, ";");
   binNames_.clear();
   std::map<std::string, std::string>::iterator it = binnamesmap.begin();
   for (; it != binnamesmap.end(); ++it) {
      int binvalue = StringToNumber<int>(it->first);
      binNames_.insert(std::pair<int, std::string>(binvalue, it->second));
   }
   return true;
}

// Asigna nombres default a las badas
void StatisticsXmlToCsvConverter::GetBandNames(const std::string &SelectedBands,
                                       std::vector<std::string> &BandNames) {
   BandNames.clear();
   std::vector<int> selectedbands = split<int>(SelectedBands);
   for (size_t i = 0; i < selectedbands.size(); i++) {
      BandNames.push_back("banda:#");
      BandNames[i] += LongToString(selectedbands[i] + 1);
   }
}

// Retorna el valor de una opcion
bool StatisticsXmlToCsvConverter::ReadOption(const std::string &Option,
                                                    std::string &OptionValue) {
   std::map<std::string, std::string>::iterator it = options_.find(Option);
   if (it != options_.end())
      OptionValue = it->second;

   return (it != options_.end());
}

/**
 * Retorna true si existe la opcion y tiene valor true
 * @param[in] Option nombre de la opcion tipo flag(valor true/false)
 * @return bool que indica si la opcion existia y era true.
 */
bool StatisticsXmlToCsvConverter::GetFlagOptionValue(
                                          const std::string &Option) {
   std::string value;
   if (!ReadOption(Option, value))
      return false;
   return value == TRUE_FLAG_VALUE;
}

/**
 * Agrega histograma a html
 * @param[in] Statistics estadisticas a imprimir
 * @param[out] Html donde se agrega informacion de archivo
 */
void StatisticsXmlToCsvConverter::AddHistogram(
      suri::raster::data::HistogramBase* pHistogram,
      const std::vector<std::string> &BandNames, std::string &Csv) {
   std::string datatype, columnname, pixelareaasstring = "";
   ReadOption(DATATYPE_OP, datatype);
   UpdateBinNames();

   // Analiza si la salida se debe mostrar como areas o cantidad de pixels
   double pixelarea = 1;
   int histogramprecision = 0;
   std::string histogramdataname = _(html_COUNT);
   std::string units = PIXELAREA_OPTIONVALUE;
   if (ReadOption(OUTPUTUNITS_OP, units) && units != PIXELAREA_OPTIONVALUE) {
      if (ReadOption(PIXELAREA_OP, pixelareaasstring)) {
         histogramprecision = 3;
         histogramdataname = _(html_AREA);
         pixelarea = StringToNumber<double>(pixelareaasstring);
      }
   }

   AddCsvHeader(Csv, _(html_HISTOGRAM));
   AddCsvHeader(Csv, "\n");

   // Genero header de histograma
   std::vector<std::string> tableheader;
   if (ReadOption(HISTOGRAM_BIN_COLUMN_NAME_OP, columnname))
      tableheader.push_back(columnname);
   else
      tableheader.push_back(_(html_DN));

   tableheader.push_back(histogramdataname);

   // Muestro los histogramas por pantalla
   for (int i = 0; i < pHistogram->GetBandCount(); i++) {
      std::vector<std::vector<double> > histogram;
      std::vector<std::string> rownames;
      for (int bin = 0; bin < pHistogram->GetNumBins()[i]; ++bin) {
         double min = pHistogram->GetMin()[i];
         double max = pHistogram->GetMax()[i];
         double binsize = (max - min)
               / static_cast<double>(pHistogram->GetNumBins()[i]);
         double currentbin = bin * (binsize);
         double freq = pHistogram->GetBins()[i][bin];
         if (freq != 0) {
            std::string binname = GetBinName(currentbin);
            rownames.push_back(binname);
            std::vector<double> bandhistogram;
            bandhistogram.push_back(freq);
            histogram.push_back(bandhistogram);
         }
      }
      AddCsvNumericTable(Csv, BandNames[i], tableheader, rownames, histogram, pixelarea,
                         histogramprecision);
   }
}
} // namespace suri
