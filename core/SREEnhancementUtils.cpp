/** \file SREEnhancementUtils.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "SREEnhancementUtils.h"

namespace suri {
namespace raster {
namespace enhancement {
namespace utils {

/**
 * Persiste toda la informacion necesaria para el uso del LUT en un XML.
 */
bool LutToXml(wxXmlNode*& pXmlNode, bool Active, int BandCount, std::string& LutName,
           int* pNumBins, double* pMin, double* pMax, int** pLut) {
   pXmlNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(ENHANCEMENT_NODE));

   // Agrego la propiedad de activacion
   pXmlNode->AddProperty(
         new wxXmlProperty(
               wxT(ENHANCEMENT_NODE_PROP_ACTIVE),
               Active ? wxT(ACTIVE_PROPERTY_VALUE) : wxT(INACTIVE_PROPERTY_VALUE)));

   // Agrego la propiedad nombre
   std::stringstream ss;
   ss << BandCount;
   if (!LutName.empty()) {
      pXmlNode->AddProperty(
            new wxXmlProperty(wxT(ENHANCEMENT_NODE_PROP_BAND_COUNT), ss.str().c_str()));
   }

   // Agrego la propiedad nombre
   if (!LutName.empty()) {
      pXmlNode->AddProperty(
            new wxXmlProperty(wxT(ENHANCEMENT_NODE_PROP_NAME), LutName.c_str()));
   }

   wxXmlNode* pband = NULL;
   for (int bandix = 0; bandix < BandCount; ++bandix) {
      pband = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                       wxT(ENHANCEMENT_NODE_BAND));

      // Numero de Bins
      std::stringstream ssnumbins;
      ssnumbins << pNumBins[bandix];
      pband->AddProperty(
            new wxXmlProperty(wxT(ENHANCEMENT_NODE_BAND_PROP_BINS),
                              wxT(ssnumbins.str().c_str())));

      // Min
      std::stringstream ssmin;
      ssmin << pMin[bandix];
      pband->AddProperty(
            new wxXmlProperty(wxT(ENHANCEMENT_NODE_BAND_PROP_MIN),
                              wxT(ssmin.str().c_str())));

      // Max
      std::stringstream ssmax;
      ssmax << pMax[bandix];
      pband->AddProperty(
            new wxXmlProperty(wxT(ENHANCEMENT_NODE_BAND_PROP_MAX),
                              wxT(ssmax.str().c_str())));

      // Valores
      std::stringstream ssvalues;
      for (int binix = 0; binix < 256; ++binix) {
         if (ssvalues.str().length() > 0) ssvalues << " ";
         ssvalues << pLut[bandix][binix];
      }
      wxXmlNode* pvalues = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                         wxT(ENHANCEMENT_NODE_BAND_VALUES));
      wxXmlNode* pvaluesctt = new wxXmlNode(NULL, wxXML_TEXT_NODE, wxEmptyString,
                                            wxT(ssvalues.str().c_str()));

      pvalues->AddChild(pvaluesctt);
      pband->AddChild(pvalues);

      wxXmlNode* pnode = pXmlNode->GetChildren();
      if (pnode == NULL) {
         pXmlNode->AddChild(pband);
      } else {
         while (pnode->GetNext() != NULL)
            pnode = pnode->GetNext();
         pnode->SetNext(pband);
      }
   }

   return true;
}

/**
 * Lee toda la informacion necesaria para el uso del LUT desde un XML.
 */
bool LutFromXml(wxXmlNode* pXmlNode, int& BandCount, int*& pNumBins, double*& pMin,
             double*& pMax, int**& pLut, bool& Active, std::string& Name) {
   if (pXmlNode->GetName() == wxT(ENHANCEMENT_NODE)) {
      wxString propval;

      pXmlNode->GetPropVal(ENHANCEMENT_NODE_PROP_ACTIVE, &propval);
      Active = (propval.compare(wxT(ACTIVE_PROPERTY_VALUE)) == 0);

      pXmlNode->GetPropVal(ENHANCEMENT_NODE_PROP_BAND_COUNT, &propval);
      BandCount = StringToNumber<int>(propval.c_str());

      pXmlNode->GetPropVal(ENHANCEMENT_NODE_PROP_NAME, &propval);
      Name = std::string(propval.mb_str());

      pNumBins = new int[BandCount];
      pMin = new double[BandCount];
      pMax = new double[BandCount];
      pLut = new int*[BandCount];

      int bandix = 0;
      wxXmlNode* pband = pXmlNode->GetChildren();
      while (pband != NULL) {
         if (pband->GetName() == wxT(ENHANCEMENT_NODE_BAND)) {
            pband->GetPropVal(ENHANCEMENT_NODE_BAND_PROP_MIN, &propval);
            pMin[bandix] = StringToNumber<double>(propval.c_str());

            pband->GetPropVal(ENHANCEMENT_NODE_BAND_PROP_MAX, &propval);
            pMax[bandix] = StringToNumber<double>(propval.c_str());

            int bincount = 0;
            pband->GetPropVal(ENHANCEMENT_NODE_BAND_PROP_BINS, &propval);
            bincount = StringToNumber<int>(propval.c_str());
            pLut[bandix] = new int[bincount];
            pNumBins[bandix] = bincount;

            wxXmlNode* pvalues = pband->GetChildren();
            if (pvalues != NULL) {
               wxString ctt = pvalues->GetNodeContent();
               std::vector<std::string> values = split<std::string>(std::string(ctt.mb_str()));
               for (int ix = 0; ix < bincount; ++ix) {
                  pLut[bandix][ix] = StringToNumber<int>(values[ix]);
               }
            }
            pband = pband->GetNext();
            ++bandix;
         }
      }

      return true;
   }
   return false;
}

}  // namespace utils
}  // namespace enhancement
}  // namespace raster
}  // namespace suri
