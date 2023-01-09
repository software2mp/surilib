/** \file TextFormatConverter.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <vector>
#include <string>
#include <map>
#include <utility>

// Includes suri
#include "StatisticsXmlToHtmlConverter.h"
#include "suri/StatisticsFunctions.h"
#include "logmacros.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/messages.h"
#include "suri/DataTypes.h"
#include "HtmlFunctions.h"
#include "SRDHistogram.h"
#include "SRDStatistics.h"

#if defined(__LINUX__)
#include "suri/XmlElement.h"
#endif

// Defines
#define STATISTICSXML_FORMATNAME "xml:statistics"
#define STATISTICSHTML_FORMATNAME "html:statistics"
#define PIXELAREA_OPTIONVALUE "pixel"

#define BASICPARAMETERCOUNT      4
#define EIGENVALUESCOUNT         1
#define HISTOGRAMROWCOUNT 256
#define AVOID_ROUNDERROR_OFFSETPERCENTAGE 0.1
#define DEFAULT_BINNAME_PRECISION 6
#define TRUE_FLAG_VALUE "true"

#define HISTOGRAMCONTAINER_TABLE_ATRIBUTES "border=\"0\" valign=\"top\""
#define HISTOGRAMCONTAINER_ROW_ATRIBUTES "valign=\"top\""
#define NOBORDER_TABLE_ATRIBUTES "cellspacing=\"10\" border=\"0\""

#define AREA_FP_PRECISION        6
#define BASIC_STATS_PRECISION    6

namespace suri {
/**
 * Se agregaron opciones que permiten transformar los valores obtenidos
 * del xml con estadisticas y mostrar el resultado como areas.
 * Causa: el histograma solo permite guardar cantidad de pixes.
 * Refactor pendiente: TextFormatConverter no deberia tener logica
 * para transformar entre unidades. Ver TCK #841
 */
const std::string StatisticsXmlToHtmlConverter::OUTPUTUNITS_OP = "unit";
const std::string StatisticsXmlToHtmlConverter::PIXELAREA_OP = "pixel_area";

/**
 * Se agregaron las opciones: url, tipo de dato, bandas selecciondas y
 * parte de la imagen leida.
 * Causa: falta de tiempo
 * Refactor pendiente: Agregar esta informacion al xml de entrada.
 * Ver TCK #841
 */
const std::string StatisticsXmlToHtmlConverter::URL_OP = "url";
const std::string StatisticsXmlToHtmlConverter::EXTENT_OP = "extent";
const std::string StatisticsXmlToHtmlConverter::DATATYPE_OP = "datatype";
const std::string StatisticsXmlToHtmlConverter::SELECTEDBANDS_OP = "selectedbands";
const std::string StatisticsXmlToHtmlConverter::SELECTEDBANDIDS_OP = "selectedbandids";
const std::string StatisticsXmlToHtmlConverter::BINNAMES_OP = "bin_names";
const std::string StatisticsXmlToHtmlConverter::SHOWMODE_OP = "show_mode";
const std::string StatisticsXmlToHtmlConverter::BINS_0_255_OP = "bins_0_255";
const std::string StatisticsXmlToHtmlConverter::HISTOGRAM_BIN_COLUMN_NAME_OP =
      "histogram_bin_column_name";
const std::string StatisticsXmlToHtmlConverter::STATS_FLAGS = "stats_flags";
const std::string StatisticsXmlToHtmlConverter::COMPONENTS = "components";

/** Ctor */
StatisticsXmlToHtmlConverter::StatisticsXmlToHtmlConverter(
      std::map<std::string, std::string> Options) {
   options_ = Options;
}

/** Dtor */
StatisticsXmlToHtmlConverter::~StatisticsXmlToHtmlConverter() {
}

/**
 * Transforma el contenido del texto.
 * @param[in] InputText texto que deseo convertir
 * @param[out] OutputText texto en nuevo formato.
 * @return true en caso de tener exito
 * @return false si ocurre un error.
 */
bool StatisticsXmlToHtmlConverter::Transform(const std::string &InputText,
                                             std::string &OutputText) {
   // Lee opciones con datos de la imagen
   std::string selectedbands, pixelareaasstring = "", components;
   ReadOption(SELECTEDBANDS_OP, selectedbands);

   // Flags
   std::string strflags;
   ReadOption(STATS_FLAGS, strflags);
   int flags = StringToNumber<int>(strflags);

   // Obtengo las estadisticas calculadas
   Statistics statistics;
   std::string html;
   suri::raster::data::HistogramBase* phistogram = NULL;
   suri::raster::data::StatisticsBase* pstats = NULL;
   // Obtengo el nombre de las bandas
   std::vector<std::string> bandnames, componentnames;
   GetBandNames(selectedbands, bandnames);

   if (GetStatsFromXmlString(InputText, pstats, phistogram)) {
      size_t bandcount = bandnames.size() > 0 ? bandnames.size() : pstats->GetBandCount();

      // Esto es para "salvar" el caso de cuando el reporte viene
      // pedido de componentes principales.
      if (bandnames.size() == 0) {
         for (size_t i = 0; i < bandcount; i++) {
            bandnames.push_back("banda:#");
            bandnames[i] += LongToString(i + 1);
         }
      }

      // Genero html a partir de estadisticas
      html = "";

      // Muestro informacion general de la imagen
      AddRasterDescription(pstats->GetPointCount(0), html);

      AddHtmlHeader(html, _(html_STATISTICS));

      // Muestro parametros basicos de las estadisticas
      if (flags & Statistics::Mean)
         AddBasicStatisticsParameters(pstats, bandnames, html);

      // Inicializo header para estadisticas entre bandas
      std::vector<std::string> tableheader;
      tableheader.push_back("");
      for (size_t i = 0; i < bandcount; i++)
         tableheader.push_back(bandnames[i]);

      // Agrego estadisticas entre bandas a html
      if (flags & Statistics::CorrelationMatrix || flags & Statistics::CovarianceMatrix) {
         ReadOption(COMPONENTS, components);
         if (!components.empty())
            AddEigenValues(pstats, bandnames, html);

         AddHtmlNumericTable(html, _(html_CORRELATIONMATRIX), tableheader, bandnames,
               GetMatrix(
                     pstats->GetCorrelationMatrix(), bandcount, pstats->GetBandCount(), components),
               1, BASIC_STATS_PRECISION);

         AddHtmlNumericTable(html, _(html_COVARIANCEMATRIX), tableheader, bandnames,
               GetMatrix(
                     pstats->GetCovarianceMatrix(), bandcount, pstats->GetBandCount(), components),
               1, BASIC_STATS_PRECISION);

         if (!components.empty()) {
            size_t compcount = StringToNumber<size_t>(components);
            for (size_t i = 0; i < compcount; i++) {
               componentnames.push_back("Componente #");
               componentnames[i] += LongToString(i + 1);
            }

            std::string eigtitle;
            if (pstats->GetEig() == EIG_NONE || pstats->GetEig() == EIG_COVARIANCE) {
               eigtitle = "Autovectores (Matriz de Covarianza)";
            } else {
               eigtitle = "Autovectores (Matriz de Correlacion)";
            }

            AddHtmlNumericTable(html, _(eigtitle.c_str()), tableheader, componentnames,
                                GetEigenVectors(), 1, BASIC_STATS_PRECISION);
         }
      }

      //Agrego histograma a estadisticas
      if (phistogram > 0)
         AddHistogram(phistogram, bandnames, html);

   } else if (GetStatsFromXmlString(InputText, statistics)) {

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
      html = "";

      // Muestro informacion general de la imagen
      AddRasterDescription(statistics.count_, html);

      AddHtmlHeader(html, _(html_STATISTICS));

      // Muestro parametros basicos de las estadisticas
      if (statistics.mean_.size() > 0)
         AddBasicStatisticsParameters(statistics, bandnames, html);

      // Muestro parametros basicos de las estadisticas
      if (GetFlagOptionValue(SHOWMODE_OP))
         AddMode(statistics, bandnames, html);

      // Inicializo header para estadisticas entre bandas
      std::vector<std::string> tableheader;
      tableheader.push_back("");
      for (size_t i = 0; i < bandcount; i++)
         tableheader.push_back(bandnames[i]);

      // Agrego estadisticas entre bandas a html
      if (statistics.correlationMatrix_.size() > 0)
         AddHtmlNumericTable(html, _(html_CORRELATIONMATRIX), tableheader, bandnames,
                             statistics.correlationMatrix_, 1, BASIC_STATS_PRECISION);
      if (statistics.covarianceMatrix_.size() > 0)
         AddHtmlNumericTable(html, _(html_COVARIANCEMATRIX), tableheader, bandnames,
                             statistics.covarianceMatrix_, 1, BASIC_STATS_PRECISION);
      // Agrego histograma a estadisticas
      if (statistics.histogram_.size() > 0)
         AddHistogram(statistics, bandnames, html);
   } else {
      return false;
   }

   OutputText = html;
#if defined(__LINUX__)
   // Correccion a codificacion en Linux debido a url
   OutputText = wxString(wxConvUTF8.cMB2WC(html.c_str()),
                         wxCSConv(XmlElement::xmlEncoding_.c_str()));
   if (OutputText.empty())
      OutputText = html;
#endif

   return true;
}

/**
 * Agrega informacion del raster que se uso para calcular estadisticas a html
 * @param[in] Statistics estadisticas a imprimir
 * @param[out] Html donde se agrega informacion de archivo
 */
void StatisticsXmlToHtmlConverter::AddRasterDescription(long Count, std::string &Html) {
   std::string url, extentasstring, pixelareaasstring, selectedbands = "";
   std::string units = PIXELAREA_OPTIONVALUE;
   double pixelarea = 1;

   ReadOption(URL_OP, url);
   ReadOption(EXTENT_OP, extentasstring);
   if (ReadOption(OUTPUTUNITS_OP, units) && units != PIXELAREA_OPTIONVALUE) {
      if (ReadOption(PIXELAREA_OP, pixelareaasstring))
         pixelarea = StringToNumber<double>(pixelareaasstring);
   }

   AddHtmlHeader(Html, _(html_IMAGEINFO));

   Html = Html + "<table " + NOBORDER_TABLE_ATRIBUTES + ">";
   if (!extentasstring.empty()) {
      Html = Html + "<tr><td>" + _(html_URL) + wxT(": ") + url + "</td></tr>";
      std::vector<std::string> extent = split<std::string>(extentasstring);
      Html = Html + "<tr><td>" + _(html_EXTENT) + wxT(": ") + "[" + extent[0] + ", "
            + extent[1] + "] - ";
      Html = Html + "[" + extent[2] + ", " + extent[3] + "] </td></tr>";
      Html = Html + "<tr><td>" + _(html_UNIT) + wxT(": ") + _(units.c_str())
            + "</td></tr>";
   }
   if (units != PIXELAREA_OPTIONVALUE) {
      Html = Html + "<tr><td>" + _(html_IMAGEAREA) + wxT(": ")
            + DoubleToStringEx(pixelarea * Count, AREA_FP_PRECISION) + "</td></tr>";
      Html = Html + "<tr><td>" + _(html_PIXELAREA) + wxT(": ")
            + DoubleToStringEx(pixelarea, AREA_FP_PRECISION) + "</td></tr>";
   } else {
      Html = Html + "<tr><td>" + _(html_PIXELCOUNT) + wxT(": ") + LongToString(Count)
            + "</td></tr>";
   }
   Html = Html + "</table>";
}

/**
 * Agrega parametros basicos de estadisticas a html
 * @param[in] Statistics estadisticas a imprimir
 * @param[in] BandNames Nombre de bandas en imagen de entrada
 * @param[out] Html donde se agrega informacion de archivo
 */
void StatisticsXmlToHtmlConverter::AddBasicStatisticsParameters(
      const Statistics& statistics, const std::vector<std::string> &BandNames, std::string &Html) {
   // Obtengo el id de las bandas
   std::string selectedbandids = "";
   ReadOption(SELECTEDBANDIDS_OP, selectedbandids);
   std::vector<int> bandids;
   if (!selectedbandids.empty()) {
      GetBandIds(selectedbandids, bandids);
   } else {
      for (size_t i = 0; i < statistics.mean_.size(); i++) {
         bandids.push_back(i);
      }
   }

   // Obtengo header de tabla
   std::vector<std::string> tableheader;
   tableheader.push_back(_(html_PARAMETERS));
   for (size_t i = 0; i < BandNames.size(); i++)
      tableheader.push_back(BandNames[i]);

   // Obtengo nombre de filas de tabla
   std::vector<std::string> rownames;
   rownames.push_back(_(html_MEAN));
   rownames.push_back(_(html_VARIACE));
   rownames.push_back(_(html_MINIMUN));
   rownames.push_back(_(html_MAXIMUN));

   // Obtengo contenido tabla
   std::vector<std::vector<double> > stats(BASICPARAMETERCOUNT);
   for (size_t i = 0; i < bandids.size(); i++) {
      stats[0].push_back(statistics.mean_[bandids[i]]);
      stats[1].push_back(statistics.variance_[bandids[i]]);
      stats[2].push_back(statistics.min_[bandids[i]]);
      stats[3].push_back(statistics.max_[bandids[i]]);
   }

   AddHtmlNumericTable(Html, _(html_BASICPARAMETERS), tableheader, rownames, stats, 1,
                       BASIC_STATS_PRECISION);
}

/**
 * Agrega parametros basicos de estadisticas a html
 * @param[in] pStatistics estadisticas a imprimir
 * @param[in] BandNames Nombre de bandas en imagen de entrada
 * @param[out] Html donde se agrega informacion de archivo
 */
void StatisticsXmlToHtmlConverter::AddBasicStatisticsParameters(
      suri::raster::data::StatisticsBase* pStatistics,
      const std::vector<std::string> &BandNames, std::string &Html) {

   // Obtengo header de tabla
   std::vector<std::string> tableheader;
   tableheader.push_back(_(html_PARAMETERS));
   for (size_t i = 0; i < BandNames.size(); i++)
      tableheader.push_back(BandNames[i]);

   // Obtengo nombre de filas de tabla
   std::vector<std::string> rownames;
   rownames.push_back(_(html_MEAN));
   rownames.push_back(_(html_VARIACE));
   rownames.push_back(_(html_MINIMUN));
   rownames.push_back(_(html_MAXIMUN));

   // Obtengo contenido tabla
   std::vector<std::vector<double> > stats(BASICPARAMETERCOUNT);
   for (size_t i = 0; i < BandNames.size(); i++) {
      stats[0].push_back(pStatistics->pMean_[i]);
      stats[1].push_back(pStatistics->pAccumVariance_[i]);
      stats[2].push_back(pStatistics->pMin_[i]);
      stats[3].push_back(pStatistics->pMax_[i]);
   }

   AddHtmlNumericTable(Html, _(html_BASICPARAMETERS), tableheader, rownames, stats, 1,
                       BASIC_STATS_PRECISION);
}

/**
 * Agrega los autovalores al html.
 * @param[in] pStatistics estadisticas a imprimir
 * @param[in] BandNames Nombre de bandas en imagen de entrada
 * @param[out] Html donde se agrega informacion de archivo
 */
void StatisticsXmlToHtmlConverter::AddEigenValues(
      suri::raster::data::StatisticsBase* pStatistics,
      const std::vector<std::string>& BandNames, std::string& Html) {

   // Obtengo header de tabla
   std::string components;
   ReadOption(COMPONENTS, components);
   size_t compcount = StringToNumber<size_t>(components);
   std::vector<std::string> tableheader;
   tableheader.push_back(_(html_PARAMETERS));

   if (compcount > 0) {
      for (size_t i = 0; i < compcount; ++i) {
         tableheader.push_back("Componente #");
         tableheader[i + 1] += LongToString(i + 1);
      }
   } else {
      for (size_t i = 0; i < BandNames.size(); i++) {
         tableheader.push_back(BandNames[i]);
      }
   }

   // Obtengo nombre de filas de tabla
   std::vector<std::string> rownames;
   rownames.push_back(_(html_EIGENVALUE));

   std::vector<double> eigvals = pStatistics->GetEigenValues(pStatistics->GetEig());
   std::sort(eigvals.begin(), eigvals.end());
   std::reverse(eigvals.begin(), eigvals.end());

   // Obtengo contenido tabla
   std::vector<std::vector<double> > evtable(EIGENVALUESCOUNT);
   for (int i = 0; i < (int)compcount; i++) {
      evtable[0].push_back(eigvals[i]);
   }

   std::string eigtitle;
   if (pStatistics->GetEig() == EIG_NONE || pStatistics->GetEig() == EIG_COVARIANCE) {
      eigtitle = _("Autovalores (Matriz de Covarianza)");
   } else {
      eigtitle = _("Autovalores (Matriz de Correlacion)");
   }

   AddHtmlNumericTable(Html, _(eigtitle.c_str()), tableheader, rownames, evtable, 1,
                       BASIC_STATS_PRECISION);
}

/**
 * Muestra tabla con la moda.
 * @param[in] Statistics estadisticas a imprimir
 * @param[in] BandNames Nombre de bandas en imagen de entrada
 * @param[out] Html donde se agrega informacion de archivo
 */
void StatisticsXmlToHtmlConverter::AddMode(const Statistics& statistics,
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

   AddHtmlTable(Html, _(html_BASICPARAMETERS), tableheader, rownames, stats);
}

/**
 * Agrega histograma a html
 * @param[in] Statistics estadisticas a imprimir
 * @param[out] Html donde se agrega informacion de archivo
 */
void StatisticsXmlToHtmlConverter::AddHistogram(
      const Statistics& statistics, const std::vector<std::string> &BandNames,
      std::string &Html) {
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
   bool bins_0_255 = GetFlagOptionValue(BINS_0_255_OP) || IsIntegerType(datatype);
   std::vector<Histogram> newhistogram;
   for (size_t i = 0; i < statistics.histogram_.size(); i++) {
      if (bins_0_255)
         newhistogram.push_back(Histogram(-0.5, 255.5, HISTOGRAMROWCOUNT + 1));
      else
         newhistogram.push_back(
               Histogram(statistics.histogram_[i].GetMinimum(),
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

   AddHtmlHeader(Html, _(html_HISTOGRAM));

   // Genero tabla para que los histogramas esten uno al lado del otro
   Html = Html + "<table " + HISTOGRAMCONTAINER_TABLE_ATRIBUTES + "><tr "
         + HISTOGRAMCONTAINER_ROW_ATRIBUTES + ">";

   // Genero header de histograma
   std::vector<std::string> tableheader;
   if (ReadOption(HISTOGRAM_BIN_COLUMN_NAME_OP, columnname))
      tableheader.push_back(columnname);
   else
      tableheader.push_back(_(html_DN));
   tableheader.push_back(histogramdataname);

   // Muestro los histogramas por pantalla
   for (size_t i = 0; i < statistics.histogram_.size(); i++) {
      Html += "<td>";
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

      AddHtmlNumericTable(Html, BandNames[i], tableheader, rownames, histogram,
                          pixelarea, histogramprecision);
      Html += "</td>";
   }
   Html += "</tr></table>";
}


/**
 * Agrega histograma a html
 * @param[in] pHistogram Histograma a agregar
 * @param[in] BandNames nombre de las bandas
 * @param[out] Html donde se agrega informacion de archivo
 */
void StatisticsXmlToHtmlConverter::AddHistogram(
      suri::raster::data::HistogramBase* pHistogram,
      const std::vector<std::string> &BandNames, std::string &Html) {
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
         histogramprecision = AREA_FP_PRECISION;
         histogramdataname = _(html_AREA);
         pixelarea = StringToNumber<double>(pixelareaasstring);
      }
   }

   AddHtmlHeader(Html, _(html_HISTOGRAM));

   // Genero tabla para que los histogramas esten uno al lado del otro
   Html = Html + "<table " + HISTOGRAMCONTAINER_TABLE_ATRIBUTES + "><tr "
         + HISTOGRAMCONTAINER_ROW_ATRIBUTES + ">";

   // Genero header de histograma
   std::vector<std::string> tableheader;
   if (ReadOption(HISTOGRAM_BIN_COLUMN_NAME_OP, columnname))
      tableheader.push_back(columnname);
   else
      tableheader.push_back(_(html_DN));

   tableheader.push_back(histogramdataname);

   // Muestro los histogramas por pantalla
   for (int i = 0, bandcount = pHistogram->GetBandCount(); i < bandcount; i++) {
      Html += "<td>";
      std::vector<std::vector<double> > histogram;
      std::vector<std::string> rownames;
      double min = pHistogram->GetMin()[i];
      double max = pHistogram->GetMax()[i];
      double binsize = (max - min) / static_cast<double>(pHistogram->GetNumBins()[i]);
      for (int bin = 0, numbins = pHistogram->GetNumBins()[i]; bin < numbins; ++bin) {
         double freq = pHistogram->GetBins()[i][bin];
         if (freq != 0) {
            double lower = min + (binsize * bin);
            std::string binname = GetBinName(lower);
            rownames.push_back(binname);
            std::vector<double> bandhistogram;
            bandhistogram.push_back(freq);
            histogram.push_back(bandhistogram);
         }
      }

      AddHtmlNumericTable(Html, BandNames[i], tableheader, rownames, histogram,
                          pixelarea, histogramprecision);
      Html += "</td>";
   }
   Html += "</tr></table>";
}
/**
 * Retorna el nombre que el bin deberia tener en html de salida.
 * Puede ser un nombre asignado por opcion, o en caso de no existir,
 * transformar el identificado del bin a string.
 * @param[in] CurrentBin Valor con el que quiero generar el nombre
 * @return Nombre del bin.
 */
std::string StatisticsXmlToHtmlConverter::GetBinName(double CurrentBin) {
   std::string datatype, binname = "";
   int binprecision = DEFAULT_BINNAME_PRECISION;

   ReadOption(DATATYPE_OP, datatype);
   if (GetFlagOptionValue(BINS_0_255_OP) || IsIntegerType(datatype)) {
      binprecision = 0;

      int value = SURI_ROUND(int, CurrentBin);
      std::map<int, std::string>::iterator it = binNames_.find(value);
      if (it != binNames_.end())
         return it->second;
   }

   return DoubleToStringEx(CurrentBin, binprecision);
}

/**
 * Actualiza binNames_ con contenido de opcion BINNAMES_OP
 * @return bool que indica si tuvo exito
 */
bool StatisticsXmlToHtmlConverter::UpdateBinNames() {
   std::string binnames, datatype;
   ReadOption(DATATYPE_OP, datatype);
   if (!ReadOption(BINNAMES_OP, binnames) || !IsIntegerType(datatype))
      return false;

   std::map<std::string, std::string> binnamesmap = StringToMap(binnames, ";");
   binNames_.clear();
   std::map<std::string, std::string>::iterator it = binnamesmap.begin();
   for (; it != binnamesmap.end(); ++it) {
      int binvalue = StringToNumber<int>(it->first);
      binNames_.insert(std::pair<int, std::string>(binvalue, it->second));
   }
   return true;
}

/** Asigna el nombre de las bandas */
void StatisticsXmlToHtmlConverter::GetBandNames(const std::string &SelectedBands,
                                                std::vector<std::string> &BandNames) {
   BandNames.clear();
   BandNames = SplitEx(SelectedBands, "|");
}

/** Asigna el nombre de las bandas */
void StatisticsXmlToHtmlConverter::GetBandIds(const std::string &SelectedBandIds,
                                              std::vector<int> &BandIds) {
   BandIds.clear();
   std::vector<std::string> aux = SplitEx(SelectedBandIds, "|");
   for (size_t i = 0; i < aux.size(); ++i)
      BandIds.push_back(StringToNumber<int>(aux[i]));
}

// Retorna el valor de una opcion
bool StatisticsXmlToHtmlConverter::ReadOption(const std::string &Option,
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
bool StatisticsXmlToHtmlConverter::GetFlagOptionValue(const std::string &Option) {
   std::string value;
   if (!ReadOption(Option, value))
      return false;
   return value == TRUE_FLAG_VALUE;
}

/**
 * Retorna los autovectores en el orden correspondiente de los autovalores
 * @return eigenvectors vector con autovectores
 */
std::vector<std::vector<double> > StatisticsXmlToHtmlConverter::GetEigenVectors() {
   std::vector<std::vector<double> > eigenvectors;
   std::string components = "", aux = "";
   ReadOption(COMPONENTS, components);
   int compcount = StringToNumber<int>(components);
   // Recorro cada componente y agrego su autovector correspondiente a eigenvectors
   for (int i = 0; i < compcount; i++) {
      std::string autovector = "autovector";
      autovector.append(NumberToString(i));
      ReadOption(autovector, aux);
      std::vector<std::string> tmp = SplitEx(aux, " ");
      std::vector<double> eigenvector;
      for (size_t x = 0; x < tmp.size(); ++x)
         eigenvector.push_back(StringToNumber<double>(tmp[x]));
      if (eigenvector.size() > 0)
         eigenvectors.push_back(eigenvector);
   }
   return eigenvectors;
}

/**
 * Retorna una matriz con estadisticas de las bandas seleccionadas
 * @param[in] Matrix matriz de correlacion o covarianza que posee todas las bandas
 * @param[in] SelectedBandCount cantidad de bandas seleccionadas
 * @param[in] BandCount cantidad de bandas de la imagen
 * @param[in] Components cantidad de componentes
 * @return Matrix/simplifiedmatrix matriz con las bandas seleccionadas. Si todas las bandas estan
 * selccionadas no se genera una matriz nueva
 */
std::vector<std::vector<double> > StatisticsXmlToHtmlConverter::GetMatrix(
      std::vector<std::vector<double> > Matrix, size_t SelectedBandsCount, int BandCount,
      std::string Components) {
   // Obtengo el id de las bandas
   std::string selectedbandids = "";
   ReadOption(SELECTEDBANDIDS_OP, selectedbandids);
   std::vector<int> bandids;
   GetBandIds(selectedbandids, bandids);

   if (Components.empty() || StringToNumber<int>(Components) == BandCount)
      return Matrix;

   // Armo una matriz que unicamente contiene las bandas seleccionadas en analisis de componentes
   std::vector < std::vector<double> > simplifiedmatrix = std::vector< std::vector<double> > (
         bandids.size(), std::vector<double>(bandids.size(), 0.0));
   int matrixcol = 0, simplifiedcol = -1;
   for (int colix = 0; colix < BandCount; ++colix) {
      std::vector<int>::iterator it = std::find(bandids.begin(), bandids.end(), colix);
      int row = 0;
      if (it != bandids.end())
         simplifiedcol++;
      for (size_t count = 0; it != bandids.end() && count < bandids.size(); ++count, ++row) {
         simplifiedmatrix[row][simplifiedcol] = Matrix[bandids[count]][matrixcol];
      }
      matrixcol++;
   }

   return simplifiedmatrix;
}

} // namespace suri
