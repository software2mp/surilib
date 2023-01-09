/*! \file TasseledCapProcess.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TASSELEDCAPPROCESS_H_
#define TASSELEDCAPPROCESS_H_

#include "suri/DataViewManager.h"
#include "suri/Element.h"
#include "suri/FileExporterProcess.h"

namespace suri {

class TasseledCapProcess : public FileExporterProcess {
public:
   /** Nombre del proceso **/
   static const std::string kProcessName;
   static const std::string kToken;

   /**
    * Ctor.
    */
   TasseledCapProcess(Element* pInputElement, const Subset& WindowSubset,
                      DataViewManager* pDataViewManager);

   /**
    * Dtor.
    */
   virtual ~TasseledCapProcess();

protected:
   /**
    * Pemite configurar el RasterElement que se va a renderizar.
    */
   virtual bool ConfigureRaster(RasterElement* pRasterElement);

   /**
    * Otiene el tamano del objeto a salvar.
    */
   virtual wxLongLong GetNeededSpace();

   /**
    * Retorna el nodo datacast configurado con el tipo de dato de salida.
    */
   wxXmlNode* GetDataCastRenderizationNode();
   /** 
    * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
    * agreguen sus atributos especificos
    * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
    * @return vector vacio en caso de que no tenga parametros adicionales
    **/
   std::vector<wxXmlNode*> DoGetXmlParameters() const;
};

}  // namespace suri

#endif  // TASSELEDCAPPROCESS_H_
