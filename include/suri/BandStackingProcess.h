/*! \file BandStackingProcess.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#ifndef __BAND_STAC_PROCESS_H_
#define __BAND_STAC_PROCESS_H_

#include <vector>

#include "suri/FileExporterProcess.h"

namespace suri {

class BandStackingProcess : public FileExporterProcess {
   /** ctor copia **/
   BandStackingProcess(const BandStackingProcess&);

public:
   /** Nombre del proceso **/
   static const std::string kProcessName;
    /** Ctor. */
   BandStackingProcess(Element *pElement, const Subset &WindowSubset,
                       DataViewManager* pDataViewManager);

   /** Ctor. */
   BandStackingProcess(std::vector<Element*> InputElements, const Subset &ViewerSubset,
                       DataViewManager* pDataViewManager);
   virtual ~BandStackingProcess();

protected:
   /** 
    * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
    * agreguen sus atributos especificos
    * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
    * @return vector vacio en caso de que no tenga parametros adicionales
   **/
   virtual std::vector<wxXmlNode*> DoGetXmlParameters() const;
};

}  /** namespace suri **/
#endif   /** __BAND_STAC_PROCESS_H_ **/
