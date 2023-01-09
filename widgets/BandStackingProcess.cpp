/*! \file BandStackingProcess.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#include <vector>
#include <string>

#include "suri/BandStackingProcess.h"
#include "BandInfo.h"
#include "suri/XmlFunctions.h"

namespace suri {

/** Nombre del proceso **/
const std::string BandStackingProcess::kProcessName = "BandStackingProcess";

/** Ctor. */
BandStackingProcess::BandStackingProcess(Element *pElement,
                                         const Subset &WindowSubset,
                                         DataViewManager* pDataViewManager) :
               FileExporterProcess(pElement, WindowSubset, pDataViewManager) {
   SetProcessName(kProcessName);
}


/** Ctor. */
BandStackingProcess::BandStackingProcess(std::vector<Element*> InputElements,
                                         const Subset &ViewerSubset,
                                         DataViewManager* pDataViewManager) :
            FileExporterProcess(InputElements, ViewerSubset, pDataViewManager) {
}

BandStackingProcess::~BandStackingProcess() {
}

/** 
 * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
 * agreguen sus atributos especificos
 * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
 * @return vector vacio en caso de que no tenga parametros adicionales
**/
std::vector<wxXmlNode*> BandStackingProcess::DoGetXmlParameters() const {
   std::vector<wxXmlNode*> params;
   /** Configuracion de seleccion espectral **/   
   std::vector<BandInfo> bands;
   pAdaptLayer_->GetAttribute<std::vector<BandInfo> >(ProcessAdaptLayer::BandInfoKeyAttr, bands);
   std::string stackingbands;
   for (size_t b = 0; b < bands.size(); ++b) {
      stackingbands += bands[b].GetElement()->GetUrl().c_str();
      stackingbands += ",";
   }
#ifdef __WINDOWS__
   stackingbands = wxString(wxConvUTF8.cMB2WC(stackingbands.c_str()),
                  wxCSConv(suri::XmlElement::xmlEncoding_.c_str())).c_str();
#endif
   params.push_back(CreateKeyValueNode("bandas-origen", stackingbands.substr(0, stackingbands.size()-1)));
   return params;
}

}  /** namespace suri */
