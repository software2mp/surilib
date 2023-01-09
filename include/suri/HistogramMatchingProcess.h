/*! \file HistogramMatchingProcess.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


#ifndef HISTOGRAMMATCHINGPROCESS_H_
#define HISTOGRAMMATCHINGPROCESS_H_

// Includes standard
#include <vector>

// Includes suri
#include "FileExporterProcess.h"
#include "ClassInformation.h"
#include "suri/LayerList.h"
#include "suri/Dimension.h"
#include "SREnhancementRenderer.h"

/** namespace suri */
namespace suri {
// Forward
class DatasourceManagerInterface;

class HistogramMatchingProcess: public FileExporterProcess {

public:
   /** Ctor de copia */
   HistogramMatchingProcess(HistogramMatchingProcess &);
   /** Constructor */
   HistogramMatchingProcess(std::vector<Element*> InputElements, const Subset &WindowSubset,
                        DataViewManager* pDataViewManager = NULL,
                        bool ShowSpectralSelectionPart = false);
   /** Dtor */
   virtual ~HistogramMatchingProcess();
   /** Nombre del proceso **/
   static const std::string kProcessName;
   virtual World *GetOutputWorld();
   bool  ConfigureProcess();
   LayerList * GetElementList();
protected:
// ------ Metodos que permiten configurar el proceso ------
   /** Pemite configurar el RasterElement que se va a renderizar */
   virtual bool ConfigureRaster(RasterElement *pRasterElement);
   /** Configura el elemento creado con la herramienta */
   virtual bool ConfigureOutput();
   /** Metodo que se llama cuando se configura el ProcessAdaptLayer del proceso**/
   virtual void DoOnProcessAdaptLayerSet();
private:
   void SetNewBandsCombinationNode(Element* pElement);
   bool SetEnhancement(wxXmlNode*&pNode, Element* pElementA, Element* pElementB);
   /** Informacion de las clasesen la imagen de entrada **/
   std::vector<ClassInformation> classInformation_;
   /** Temporal hasta que los procesos usen datasources **/
   DatasourceManagerInterface* pDatasourceManager_;
   bool HasEquation(const wxXmlNode* pNode) const;
   bool sameBandCount_;
};
}

#endif /* HISTOGRAMMATCHINGPROCESS_H_ */
