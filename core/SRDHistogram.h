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

#ifndef SRDHISTOGRAM_H_
#define SRDHISTOGRAM_H_

// Includes Estandard
#include <cmath>
#include <limits>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>

// Includes Suri
#include "NoDataValue.h"
#include "suri/XmlFunctions.h"
#include "suri/DataTypes.h"
#include "SRHistogramCanvas.h"

namespace suri {
namespace raster {
namespace data {

/**
 * Clase base para el histograma que ofrece la funcionalidad
 * base no especializada.
 */
class HistogramBase : public suri::NoDataValue {
public:
   /**
    * Dtor.
    */
   virtual ~HistogramBase() {
   }
   /**
    * Convierte un histograma en un nodo xml.
    * @param[out] Histograms vector con nodos xml con una tabla que representa el histograma.
    * @return true si pudo generar el nodo
    * @return false si no pudo generar el nodo
    */
   virtual bool ToXml(std::vector<wxXmlNode*>& Histograms)=0;

   /**
    * Cuenta la cantidad de pixels en el conjunto de datos de entrada.
    */
   virtual void CountPixels(int Band, int DataLength, void* pData) = 0;

   /**
    * Devuelve la cantidad de bandas.
    */
   int GetBandCount() { return bandCount_; }

   /**
    * Obtiene el histograma de frecuencia.
    */
   int** GetBins() { return pBins_; }

   /**
    * Devuelve los bins por banda.
    */
   int* GetNumBins() { return pNumBins_; }
   
   /**
    * Devuelve la distancia entre bins
    */
   double* GetScale() { return pScale_; }
   
   /**
    * Devuelve los valores minimos por banda.
    */
   double* GetMin() { return pMinValue_; }

   /**
    * Devuelve los valores maximos por banda.
    */
   double* GetMax() { return pMaxValue_; }

   /**
    * Obtiene la frecuencia acumulada por banda.
    */
   int* GetBandAccumFrequency() { return pAccumFreq_; }

   /**
    * Obtiene la el porcentaje de frecuencia acumulada por bin por banda.
    */
   double** GetBinAccumFrequencyPercentage() {
      double** ppBAFP = new double*[bandCount_];
      for (int ix = 0; ix < bandCount_; ++ix) {
         ppBAFP[ix] = new double[pNumBins_[ix]];
         memset(ppBAFP[ix], 0, sizeof(ppBAFP[ix]) * pNumBins_[ix]);
      }

      int* accumfreq = GetBandAccumFrequency();

      for (int bandix = 0; bandix < bandCount_; ++bandix) {
         int accum = 0;
         for (int binix = 0; binix < pNumBins_[bandix]; ++binix) {
            accum += pBins_[bandix][binix];
            ppBAFP[bandix][binix] = (static_cast<double>(accum)
                  / static_cast<double>(accumfreq[bandix])) * 100.0;
         }
      }
      return ppBAFP;
   }

   /**
    * Recibe un porcentaje y devuelve el limite del intervalo que deja a la izquierda
    * dicho porcentaje de valores.
    * @param[in] Percent porcentaje de valores que se quiere dejar a la izquierda
    * @return binlimit limite del intervalo que deja a la izquierda el porcentaje de valores
    */
   double* GetMaxNPercent(double Percent) {
      return GetMinNPercent(100-Percent);
   }

   /**
    * Recibe un porcentaje y devuelve el limite del intervalo que deja a la izquierda
    * dicho porcentaje de valores.
    * @param[in] Percent porcentaje de valores que se quiere dejar a la izquierda
    * @return binlimit limite del intervalo que deja a la izquierda el porcentaje de valores
    */
   double* GetMinNPercent(double Percent) {
      // Bin limite calculado por banda.
      double normalPercent = Percent / 100;
      double* pBinLimit = new double[bandCount_];
      for (int bandix = 0; bandix < bandCount_; ++bandix) {
         double accumPix = 0;
         int totalPix = pAccumFreq_[bandix];
         for (int binix = 0; binix < pNumBins_[bandix]; ++binix) {
            accumPix += pBins_[bandix][binix];
            if ((accumPix / totalPix) > normalPercent) {
               // Si ya cumplo con el limite, calculo cual es el valor del bin que supero.
               pBinLimit[bandix] = pMinValue_[bandix] + (pScale_[bandix] * binix);
               break;
            }
         }
      }
      return pBinLimit;
   }

   /** Actualiza desde codigo cliente las frecuencias de los bins **/
   void SetBinFrecuency(int** pBins) {
      pBins_ = pBins;
   }
   /** Devuelve la entropia de la imagen a partir del histograma */
   double* GetEntropy() const {
      // Entropia calculada por banda.
      double* pEntropy = new double[bandCount_];
      for (int bandix = 0; bandix < bandCount_; ++bandix) {
         double entropy = 0;
         for (int binix = 0; binix < pNumBins_[bandix]; ++binix) {
            double numPix = pBins_[bandix][binix];
            int totalPix = pAccumFreq_[bandix];
            double prob =  numPix / totalPix;
            if (prob) {
               entropy += prob * log(prob);
            }
         }
         pEntropy[bandix] = -entropy/log(2);
      }
      return pEntropy;
   }

protected:
   int bandCount_;  // Cantidad de bandas.
   int* pNumBins_;  // Numero de bins por banda.
   double* pMinValue_;  // Valor mas chico por banda.
   double* pMaxValue_;  // Valor mas grande por banda.
   double* pScale_;  // Factor de escala por banda.
   int** pBins_;  // Histograma de frecuencia por banda.
   int* pAccumFreq_;  // Frecuencia acumulada por banda.
};

/**
 * Clase especializada para el histograma. Complementa con funcionalidad
 * especializada.
 */
template<typename T>
class Histogram : public HistogramBase {
   /**
    * Constructor copia
    */
   explicit Histogram(const Histogram& hist);

public:
   /**
    * Ctor.
    */
   Histogram(int BandCount, int* pNumBins, double* pMinValue, double* pMaxValue);

   /**
    * Dtor.
    */
   virtual ~Histogram();

   /**
    * Obtiene los valores minimos y maximos del dato pasado por parametro.
    */
   static void GetLimits(const void* data, int dataSize, double& min, double& max);

   /**
    * Cuenta la cantidad de pixels en el conjunto de datos de entrada.
    */
   virtual void CountPixels(int Band, int DataLength, void* pData);
   /**
    * Convierte un histograma en un nodo xml.
    * @param[out] Histograms vector con nodos xml con una tabla que representa el histograma.
    * @return true si pudo generar el nodo
    * @return false si no pudo generar el nodo
    */
   virtual bool ToXml(std::vector<wxXmlNode*>& Histograms);
   /** Obtiene los parametros del histograma de un nodo xml */
   static HistogramBase* FromXml(wxXmlNode* &pHistogramNode);
};

template<typename T>
Histogram<T>::Histogram(int BandCount, int* pNumBins, double* pMinValue, double* pMaxValue) {
   bandCount_ = BandCount;

   pMaxValue_ = new double[BandCount];
   pMinValue_ = new double[BandCount];
   pNumBins_ = new int[BandCount];
   pScale_ = new double[BandCount];
   pBins_ = new int*[BandCount];
   pAccumFreq_ = new int[BandCount];

   memset(pAccumFreq_, 0, sizeof(pAccumFreq_[0]) * BandCount);
   memcpy(pMaxValue_, pMaxValue, sizeof(pMaxValue[0]) * BandCount);
   memcpy(pMinValue_, pMinValue, sizeof(pMinValue[0]) * BandCount);
   memcpy(pNumBins_, pNumBins, sizeof(pNumBins[0]) * BandCount);
   for (int ix = 0; ix < BandCount; ++ix) {
      pScale_[ix] =
            (static_cast<double>(pMaxValue[ix]) - static_cast<double>(pMinValue[ix]))
                  / static_cast<double>((pNumBins[ix] - 1.0));

      pBins_[ix] = new int[pNumBins[ix]];
      memset(pBins_[ix], 0, sizeof(pBins_[ix][0]) * pNumBins[ix]);
   }
}

template<typename T>
Histogram<T>::~Histogram() {
   delete[] pAccumFreq_;
   for (int ix = 0; ix < bandCount_; ++ix) {
      delete[] pBins_[ix];
   }
   delete[] pBins_;
   delete[] pScale_;
   delete[] pMinValue_;
   delete[] pMaxValue_;
   delete[] pNumBins_;
}

template<typename T>
void Histogram<T>::CountPixels(int Band, int DataLength, void* pData) {
   T* pdata = static_cast<T*>(pData);

   for (int ix = 0; ix < DataLength; ++ix) {
      if (pdata[ix] != INFINITY && pdata[ix] != -INFINITY && !std::isnan(pdata[ix])
            && !((IsNoDataValueAvailable() && AreEqual(pdata[ix], GetNoDataValue()))
                  || (IsNdvAvailableForBand(Band) && AreEqual(pdata[ix], GetBandNdv(Band))))) {
         if (pdata[ix] >= pMinValue_[Band] && pdata[ix] <= pMaxValue_[Band]) {
            int binvalue = static_cast<int>(floor(
                  (pdata[ix] - pMinValue_[Band]) / (pScale_[Band] != 0 ? pScale_[Band] : 1)));
            pBins_[Band][binvalue] += 1;
            pAccumFreq_[Band] += 1;
         }
      }
   }
}

template<typename T>
void Histogram<T>::GetLimits(const void* data, int canvasSize, double& min, double& max) {
   const T* band = static_cast<const T*>(data);
   min = std::numeric_limits<T>::max();
   max = -min;
   for (int i = 0; i < canvasSize; ++i) {
      if (band[i] < min) min = band[i];
      if (band[i] > max) max = band[i];
   }
}

/**
 * Convierte un histograma en un nodo xml.
 * @param[out] Histograms vector con nodos xml con una tabla que representa el histograma.
 * @return true si pudo generar el nodo
 * @return false si no pudo generar el nodo
 */
template<typename T>
bool Histogram<T>::ToXml(std::vector<wxXmlNode*>& Histograms) {
   for (int b = 0; b < bandCount_; ++b) {
      wxXmlNode* phisaux = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(HISTOGRAM_NODE));
      wxXmlNode *ptablenode = NULL;
      std::vector<std::vector<double> > histogramtable(1);
      for (int bin = 0; bin < pNumBins_[b]; ++bin) {
         histogramtable[0].push_back(pBins_[b][bin]);
      }

      phisaux->AddProperty(wxT(DATA_TYPE_NODE), DataInfo<T>::Name);

      phisaux->AddProperty(wxT(MINIMUM_PROPERTY),
                           NumberToString<double>(pMinValue_[b]));
      phisaux->AddProperty(wxT(MAXIMUM_PROPERTY),
                           NumberToString<double>(pMaxValue_[b]));

      CreateTable(ptablenode, histogramtable);
      phisaux->AddChild(ptablenode);
      Histograms.push_back(phisaux);
   }

   return true;
}

template<typename T>
HistogramBase* Histogram<T>::FromXml(wxXmlNode* &pHistogramNode) {
   if (pHistogramNode->GetName().compare(HISTOGRAM_NODE) != 0) {
      return NULL;
   }

   wxXmlNode* phisto = pHistogramNode;
   int bandcount = 0;
   std::string datatype;
   std::vector<std::vector<double> > vbins;
   std::vector<double> mins, maxs;
   while (phisto != NULL && phisto->GetName().compare(HISTOGRAM_NODE) == 0) {
      wxXmlNode* ptablenode = phisto->GetChildren();
      std::vector<std::vector<double> > histogramtable;
      int rows = 0, columns = 0;
      GetDimension(ptablenode, rows, columns);
      // 1 fila (fsrecuencias) y al menos 1 columna (bin)
      if (rows != 1 && columns < 1) {
         return NULL;
      }
      LoadTable(ptablenode, histogramtable);
      vbins.push_back(histogramtable.at(0));
      // Obtengo los parametros de las propiedades del nodo
      wxString binpropertyvalue, minpropertyvalue, maxpropertyvalue,
      datatypevalue;
      if (!phisto->GetPropVal(wxT(MINIMUM_PROPERTY), &minpropertyvalue)
            || !phisto->GetPropVal(wxT(MAXIMUM_PROPERTY), &maxpropertyvalue)
            || !phisto->GetPropVal(wxT(DATA_TYPE_NODE), &datatypevalue)) {
         return NULL;
      }
      mins.push_back(StringToNumber<double>(minpropertyvalue.c_str()));
      maxs.push_back(StringToNumber<double>(maxpropertyvalue.c_str()));
      datatype = datatypevalue.c_str();

      /** se computa un histograma por banda **/
      bandcount++;
      phisto = phisto->GetNext();
   }
   // Inicializacion de atributos **/
   int* pbins = new int[vbins.size()];
   for (size_t bin = 0; bin < vbins.size(); ++bin) {
      pbins[bin] = 256;
   }

   int** pbinvaluess = new int*[vbins.size()];
   for (size_t bin = 0; bin < vbins.size(); ++bin) {
      pbinvaluess[bin] = new int[vbins[bin].size()];
      for (size_t f = 0; f < vbins[bin].size(); ++f) {
         pbinvaluess[bin][f] = vbins[bin][f];
      }
   }
   double* pmins = new double[mins.size()];
   for (size_t m = 0; m < mins.size() ; ++m) {
      pmins[m] = mins[m];
   }
   double* pmaxs = new double[maxs.size()];
   for (size_t m = 0; m < maxs.size(); ++m) {
      pmaxs[m] = maxs[m];
   }
   HistogramBase* phist = render::HistogramCanvas::CreateHistogramFromDataType(
         datatype, bandcount, pbins, pmins, pmaxs);

   phist->SetBinFrecuency(pbinvaluess);
   return phist;
}
}  // namespace data
}  // namespace raster
}  // namespace suri

#endif  // SRDHISTOGRAM_H_
