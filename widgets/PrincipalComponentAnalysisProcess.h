/*! \file PrincipalComponentAnalysisProcess.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef PRINCIPALCOMPONENTANALYSISPROCESS_H_
#define PRINCIPALCOMPONENTANALYSISPROCESS_H_

#include "suri/DataViewManager.h"
#include "suri/Element.h"
#include "suri/FileExporterProcess.h"

namespace suri {

class HtmlReportWidget;

class PrincipalComponentAnalysisProcess : public suri::FileExporterProcess {
public:
   static const std::string kToken;
   /** Nombre del proceso **/
   static const std::string kProcessName;

   /**
    * Ctor.
    */
   PrincipalComponentAnalysisProcess(Element* pInputElement, Element*& pOutputElement,
                                     const Subset& WindowSubset,
                                     DataViewManager* pDataViewManager,
                                     HtmlReportWidget* pStatisticsReport = NULL);
   /**
    * Dtor.
    */
   virtual ~PrincipalComponentAnalysisProcess();

protected:
   /**
    * Pemite configurar el RasterElement que se va a renderizar.
    */
   virtual bool ConfigureRaster(RasterElement* pRasterElement);

   /**
    * Obtiene el tamano del objeto a salvar.
    */
   virtual wxLongLong GetNeededSpace();

   /**
    * Retorna el nodo datacast configurado con el tipo de dato de salida.
    */
   wxXmlNode* GetDataCastRenderizationNode();

   /**
    * Obtiene el nodo con los autovalores y autovectores.
    */
   wxXmlNode* GetComponentsInfo();   
   /** 
    * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
    * agreguen sus atributos especificos
    * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
    * @return vector vacio en caso de que no tenga parametros adicionales
    **/
   std::vector<wxXmlNode*> DoGetXmlParameters() const;

   /** Configura opciones con datos de la imagen de entrada */
   virtual void ConfigureOutputOptions(std::map<std::string, std::string> &pOptions);

private:
   HtmlReportWidget* pStatisticsReport_;
};

}  // namespace suri

#endif  // PRINCIPALCOMPONENTANALYSISPROCESS_H_
